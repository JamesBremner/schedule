#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>


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

void cShop::Manufacture( cSchedule& S )
{
    switch( S.Type() )
    {
    case cJob::eType::sequential:
        ManufactureSequential( S );
        break;
    case cJob::eType::anyone:
        Hungarian( S );
        break;
    default:
        break;
    }
}

void cShop::Hungarian( cSchedule& S )
{
    cout << "Hungarian NYI\n";

    ////// step 1

    // ensure workers = tasks
    if( (int)myMachine.size() != S.CountJobs() )
        throw std::runtime_error(
            "Hungarian algorithm needs same number of workers and tasks");

    // construct cost matrix
    // assume tasks are specified in same order for every worker
    vector< cStep > cost_matrix;

    for( auto& worker : S )
    {
        for( auto& task : worker )
        {
            cost_matrix.push_back( task );
        }
    }

    // subtract row minumums

    for( int row = 0; row < (int)myMachine.size(); row++ )
    {
        float min = cost_matrix[ myMachine.size() * row ].Time();

        for( int col = 0; col < (int)myMachine.size(); col++ )
        {
            float cost = cost_matrix[ myMachine.size() * row + col ].Time();
            if( cost < min )
            {
                min = cost;
            }
        }
        for( int col = 0; col < (int)myMachine.size(); col++ )
        {
            int offset = myMachine.size() * row + col;
            cost_matrix[ offset ].Time(
                cost_matrix[ offset ].Time() - min );

        }
    }

    for( int row = 0; row < (int)myMachine.size(); row++ )
    {
        for( int col = 0; col < (int)myMachine.size(); col++ )
        {
            if( cost_matrix[  myMachine.size() * row + col ].Time() == 0 )
            {
                // assign worker to task
                S.FindStep( cost_matrix[  myMachine.size() * row + col ].ID() ).Start( 1 );

                cout << (S.begin()+row)->Name() <<"->"
                    << cost_matrix[  myMachine.size() * row + col ].Machine() <<"\n";
            }
        }
    }
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
