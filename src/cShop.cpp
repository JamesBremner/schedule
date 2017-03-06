#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <cfloat>
#include <algorithm>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;

void cMachine::Add( cStep& step )
{
    step.Start( myBusyUntil );
    myBusyUntil += step.Time();
}

cShop::cShop( cSchedule& S )
{
    vector< cStep > vStep ;
    S.Steps(  vStep  );
    for ( auto& s : vStep )
    {
        myMachine.insert( cMachine( s.Machine() ));
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
        return Hungarian( S );
        break;
    default:
        return 0;
        break;
    }
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
            float min = M[ N * row ].Time();

            for( int col = 0; col < N; col++ )
            {
                float cost = M[ N * row + col ].Time();
                if( cost < min )
                {
                    min = cost;
                }
            }
            for( int col = 0; col < N; col++ )
            {
                int offset = N * row + col;
                M[ offset ].Time(
                    M[ offset ].Time() - min );

            }
        }
    }

    void SubtractColMinimums()
    {
        for( int col = 0; col < N; col++ )
        {
            float min = M[ col ].Time();

            for( int row = 0; row < N; row++ )
            {
                float cost = M[ N * row + col ].Time();
                if( cost < min )
                {
                    min = cost;
                }
            }
            for( int row = 0; row < N; row++ )
            {
                int offset = N * row + col;
                M[ offset ].Time(
                    M[ offset ].Time() - min );

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
                if( M[ N * row + col ].Time() == 0 )
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
                if( M[  N * row + col ].Time() == 0 )
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
                if( M[ N * row + col ].Time() == 0 )
                {
                    CountZerosInRow++;
                }
            }
            if( CountZerosInRow > 0  )
            {
                for( int col = 0; col < N; col++ )
                {
                    if( M[ N * row + col ].Time() == 0 )
                    {
                        // assign worker to task
                        CountTasksAssigned++;
                        Assign( row, col );

                        // cross out any other zeros in column
                        for( int r = row+1; r < N; r++ )
                        {
                            if(  M[ N * r + col ].Time() == 0 )
                            {
                                M[ N * r + col ].Time( -99 );
                            }
                        }
                        // cross out any other zeros in row
                        for( int c = col+1; c < N; c++ )
                        {
                            if(  M[ N * row + c ].Time() == 0 )
                            {
                                M[ N * row + c ].Time( -99 );
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
        myS.FindStep( M[  N * worker + task ].ID() ).Start( 1 );
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
            if( s.Start() == 1 )
                Cost += s.Time();
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

            for( auto& machine : myShop )
            {

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
                    vNew.push_back( cStep("",machine.Name(),FLT_MAX ));
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
                cout << M[ N * row + col ].Time();
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
        std::set<cMachine>::iterator it;
        it = myMachine.find( cMachine( s.Machine() ));
        if( it == myMachine.end() )
            continue;

        // calculate earliest job can start
        float start = 0;
        if( s.Previous() >= 0 )
        {
            // wait for previous step to complete
            start = S.FindStep( s.Previous() ).Finish();
        }
        if( it->BusyUntil() > start )
        {
            // wait for machine to become free
            start = it->BusyUntil();
        }

        // add step to machine
        cMachine M( *it );
        M.Add( s );
        myMachine.erase( it );
        myMachine.insert( M );

        // set step start time
        S.FindStep( s.ID() ).Start( start );

    }

}
