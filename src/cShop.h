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

/** Shop containing the machines required by jobs */

class cShop
{
public:
    /** construct with all machines mentioned
        @parasm[in] S the schedule where the macines are mentioned

        The machines are constructed as they are found in the jobs found in the schedule.
        The machines are stored in a set, keyed by their names,
        so one machine is construted for each unique name found
        and they are stored in alphabetica order.
    */
    cShop( cSchedule& S );

    /** Assign steps to machines
        @return a measure of the cost
     */
    float Manufacture( cSchedule& S );

    /** Schedule jobs with multiple sequential steps to machines
        @param[in/out] S the schedule
    */
    void ManufactureSequential( cSchedule& S );

    /** Use Hungarian algorithm to assign jobs to machines,

    @param[in/out] S specified schedule
    @return Total Cost

    For this, every cJob must be type anyone
    and each step in the job models the cost of assigning the job to a different machine
    Jobs without steps mentioning a machine are assumed to be too expensive to run there.

    https://en.wikipedia.org/wiki/Hungarian_algorithm

    */
    float Hungarian( cSchedule& S );

    set< cMachine >::const_iterator begin()
    {
        return myMachine.begin();
    }
    set< cMachine >::const_iterator end()
    {
        return myMachine.end();
    }

private:
    set < cMachine > myMachine;
};

