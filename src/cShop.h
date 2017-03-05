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

    /** Assign steps to machines */
    void Manufacture( cSchedule& S );

    /** Process jobs on machines
        @param[in/out] S the schedule
    */
    void ManufactureSequential( cSchedule& S );

    /** Use Hungarion qalgorithm to assign workers to tasks

    For this, every cJob must be type anyone
    The cJob models a worker
    The cMachine models a task
    and each cStep in the job models the cost of assigning the worker to the task

    https://en.wikipedia.org/wiki/Hungarian_algorithm

    */
    void Hungarian( cSchedule& S );

private:
    set < cMachine > myMachine;
};

