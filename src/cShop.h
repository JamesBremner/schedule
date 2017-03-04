#include <iostream>
#include <vector>
#include <set>

using namespace std;

class cMachine
{
public:
    cMachine( const string& name )
        : myName( name )
        , myBusyUntil( 0 )
    {

    }
    string Name() const
    {
        return myName;
    }

    float BusyUntil() const
    {
        return myBusyUntil;
    }

    void Add( cStep& step );

    bool operator<( const cMachine& m2) const
    {
        return myName < m2.myName;
    }
private:
    string myName;
    float myBusyUntil;
};

/** Shop containing the machines reuired by jobs */

class cShop
{
public:
    /** construct with all machines mentioned
        @parasm[in] S the schedule where the macines are mentioned
    */
    cShop( cSchedule& S );

    /** Process jobs on machines
        @param[in/out] S the schedule
    */
    void Manufacture( cSchedule& S );

private:
    set < cMachine > myMachine;
};

