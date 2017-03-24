#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <cfloat>
#include <algorithm>
#include <climits>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;
namespace raven
{
namespace sch
{
void cMachine::Add( cStep& step, date_t time )
{
    step.Start( time );
    myBusyUntil = time + step.Duration();
}

void cMachine::Assign( cJob& job )
{
    // find step in job that specifies assigning the job to this machine
    cStep& step = job.FindStep( myName );

    // time that job wants to start
    date_t start = job.EarliestStart();

    // check that the machine will be ready when thye job wants to start
    if( myBusyUntil > start )
    {
        // start will have to be delayed until machine is free
        start = myBusyUntil;
    }

    // start the job step as soon as possible
    step.Start( start );

    // machine will be occupied until step is finished
    myBusyUntil = start + step.Duration();
}

cShop::cShop( cSchedule& S )
{
    vector< cStep > vStep ;
    S.Steps(  vStep  );
    for ( auto& s : vStep )
    {
        myMachine.insert( make_pair( s.Machine(), cMachine( s.Machine() )));
    }
}

float cShop::Manufacture( cSchedule& S )
{
    switch( S.Type() )
    {
    case cJob::eType::sequential:
        ManufactureSequential( S );
        return 0;
        break;

    case cJob::eType::anyone:
        if( (int)myMachine.size() == S.CountJobs() )
        {
            // the number of jobs matches the number of machines
            // so we can use the Hungarian algorithm
            return Hungarian( S );
        }
        ManufactureAnyone( S );
        return 0;
        break;
    default:
        return 0;
        break;
    }
}

void cShop::ManufactureAnyone( cSchedule& S )
{
    // for each start time
    for( auto startTime : S.JobStartTimes() )
    {
        //cout << "start time " << raven::sch::fTime("%Y-%m-%d",startTime)   << "\n";

        // loop over jobs
        for( auto& job : S )
        {
            // check if job is ready to start
            if( job.EarliestStart() != startTime )
                continue;


            auto machine = findCheapestReady( job );
            if( machine != myMachine.end() )
            {
                // assign cheapest ready machine

                machine->second.Assign( job );
            }
            else
            {
                // no machines are free to take on the job immediatly
                // assign the first machine that becomes free

                findFirstFree().Assign( job );
            }
        }
    }

}
map<string,cMachine>::iterator
cShop::findCheapestReady(  cJob& job )
{
    auto bestMachine = myMachine.end();
    float leastCost = FLT_MAX;
    // loop over machines
    for( auto it = myMachine.begin(); it != myMachine.end(); it++ )
    {
        cMachine& machine = it->second;

        // check if machine is free
        if( machine.BusyUntil() > job.EarliestStart() )
            continue;

        float cost = job.FindStep( machine.Name() ).Cost();
        if( cost < leastCost )
        {
            leastCost = cost;
            bestMachine = it;
        }
    }
    return bestMachine;
}
cMachine& cShop::findFirstFree()
{
    cMachine* bestMachine = &myMachine.begin()->second;
    auto leastDelay = bestMachine->BusyUntil();

    for( auto& it : myMachine )
    {
        cMachine& machine = it.second;

        auto delay =  machine.BusyUntil();
        if( delay < leastDelay )
        {
            leastDelay = delay;
            bestMachine = &machine;
        }
    }
    return *bestMachine;
}

class cHungarianCostMatrix
{
public:
    cHungarianCostMatrix( cSchedule& S, cShop& shop )
        : myS ( S )
        , myShop( shop )
    {
        // Ensure all jobs have steps for every machine in machine order
        // arranged in the same, alphabetical order
        AddMissingSteps();

        N = S.CountJobs();

        for( auto& worker : S )
        {
            for( auto& task : worker )
            {
                M.push_back( task );
            }
        }
    }

    void SubtractRowMinimums()
    {
        for( int row = 0; row < N; row++ )
        {
            float min = M[ N * row ].Cost();

            for( int col = 0; col < N; col++ )
            {
                float cost = M[ N * row + col ].Cost();
                if( cost < min )
                {
                    min = cost;
                }
            }
            for( int col = 0; col < N; col++ )
            {
                int offset = N * row + col;
                M[ offset ].Cost(
                    M[ offset ].Cost() - min );

            }
        }
    }

    void SubtractColMinimums()
    {
        for( int col = 0; col < N; col++ )
        {
            float min = M[ col ].Cost();

            for( int row = 0; row < N; row++ )
            {
                float cost = M[ N * row + col ].Cost();
                if( cost < min )
                {
                    min = cost;
                }
            }
            for( int row = 0; row < N; row++ )
            {
                int offset = N * row + col;
                M[ offset ].Cost(
                    M[ offset ].Cost() - min );

            }
        }
    }

    bool IsZeroInEveryColumn()
    {
        bool foundZero;
        for( int col = 0; col < N; col++ )
        {
            foundZero = false;
            for( int row = 0; row < N; row++ )
            {
                if( M[ N * row + col ].Cost() == 0 )
                {
                    foundZero = true;
                    break;
                }
            }
            if( ! foundZero )
                break;
        }
        return foundZero;
    }

    void AssignWorkerToCheapestTask()
    {
        for( int row = 0; row < N; row++ )
        {
            for( int col = 0; col < N; col++ )
            {
                if( M[  N * row + col ].Cost() == 0 )
                {
                    // assign worker to task
                    Assign( row, col );
                }
            }
        }
    }

    void Step3()
    {
        cout << "step3\n";
        Print();

        int CountTasksAssigned = 0;
        for( int row = 0; row < N; row++ )
        {
            int CountZerosInRow = 0;
            for( int col = 0; col < N; col++ )
            {
                if( M[ N * row + col ].Cost() == 0 )
                {
                    CountZerosInRow++;
                }
            }
            if( CountZerosInRow > 0  )
            {
                for( int col = 0; col < N; col++ )
                {
                    if( M[ N * row + col ].Cost() == 0 )
                    {
                        // assign worker to task
                        CountTasksAssigned++;
                        Assign( row, col );

                        // cross out any other zeros in column
                        for( int r = row+1; r < N; r++ )
                        {
                            if(  M[ N * r + col ].Cost() == 0 )
                            {
                                M[ N * r + col ].Cost( -99 );
                            }
                        }
                        // cross out any other zeros in row
                        for( int c = col+1; c < N; c++ )
                        {
                            if(  M[ N * row + c ].Cost() == 0 )
                            {
                                M[ N * row + c ].Cost( -99 );
                            }
                        }
                    }

                }
            }
        }
        if( CountTasksAssigned != N )
            throw std::runtime_error("Hungarian step 3 failed");
    }

    void Assign( int worker, int task )
    {
        auto& step = myS.FindStep( M[  N * worker + task ].ID() );
        step.Start( raven::sch::date_t() );
        //auto& machine = myShop.find( step.Machine() );
        //machine.Add( step );
        cout << (myS.begin()+worker)->Name() <<"->"
             << M[  N * worker + task ].Machine() <<"\n";
    }

    float CountCost()
    {
        float Cost = 0;
        vector< cStep > vstep;
        myS.Steps( vstep );
        for( auto& s : vstep )
        {
            if( s.IsAssigned() )
                Cost += s.Cost();
        }
        cout << "Total Cost " << Cost << "\n";
        return Cost;
    }

    void AddMissingSteps()
    {

        for( auto& job : myS )
        {
            vector< cStep > vStep;
            vector< cStep > vNew;
            job.AddSteps( vStep );

            for( auto& s : vStep )
                s.Print();
            cout << "\n";

            for( auto machine_map_it : myShop )
            {
                cMachine& machine = machine_map_it.second;
                auto sit = find_if( vStep.begin(),
                                    vStep.end(),
                                    [&]( const cStep& s )
                {
                    return ( s.Machine() == machine.Name() );
                });
                if( sit == vStep.end() )
                {
                    // the job had no step specify the cost of running on this machine
                    // so add one with a huge cost
                    vNew.push_back( cStep("",machine.Name() ));
                    vNew.back().Cost( FLT_MAX );
                }
                else
                {
                    vNew.push_back( *sit );
                }
            }
            for( auto& s : vNew )
                s.Print();
            cout << "\n";
            job.SetSteps( vNew );
        }
    }

    void Print()
    {
        for( int row = 0; row < N; row++ )
        {
            for( int col = 0; col < N; col++ )
            {
                cout << M[ N * row + col ].Cost();
            }
            cout << "\n";
        }
    }
private:
    cSchedule& myS;
    cShop& myShop;
    int N;
    vector< cStep > M;
};

float cShop::Hungarian( cSchedule& S )
{

    // ensure workers = tasks
    if( (int)myMachine.size() != S.CountJobs() )
        throw std::runtime_error(
            "Hungarian algorithm needs same number of workers and tasks");

    cHungarianCostMatrix H( S, *this );
    H.SubtractRowMinimums();
    if( H.IsZeroInEveryColumn() )
    {
        H.AssignWorkerToCheapestTask();
    }
    else
    {
        H.SubtractColMinimums();
        H.Step3();
    }
    return H.CountCost();

}
void cShop::ManufactureSequential( cSchedule& S )
{
    // loop over steps in order presented
    vector< cStep > vStep ;
    S.Steps(  vStep  );
    for ( auto& s : vStep )
    {

        // find machine required by step
        std::map<string,cMachine>::iterator it;
        it = myMachine.find( s.Machine() );
        if( it == myMachine.end() )
            continue;
        cMachine& machine = it->second;

        // calculate earliest job can start
        raven::sch::date_t start;
        if( s.Previous() >= 0 )
        {
            // wait for previous step to complete
            start = S.FindStep( s.Previous() ).Finish();
        }
        if( machine.BusyUntil() > start )
        {
            // wait for machine to become free
            start = machine.BusyUntil();
        }

        // add step to machine
        machine.Add( s, start );


        // set step start time
        S.FindStep( s.ID() ).Start( start );

    }

}
}
}
