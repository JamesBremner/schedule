#include <iostream>
#include <vector>
#include <set>

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
            start = S.FindStep( s.Previous() ).Finish();
        }
        if( it->BusyUntil() > start )
        {
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
