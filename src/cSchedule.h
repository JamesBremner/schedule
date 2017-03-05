#include <iostream>
#include <vector>
#include "json.h"

using namespace std;

/** A single processing step: the time a job spends on a nachine */
class cStep
{
public:

    /** CTOR
        @param[in] name of step
        @param[in] machine name of machine
        @param[in] time spent on machine

        The name of the step is not used for anything, and can be set to ""

        The name of the machine, if it has not been mentioned before,
        will cause a machine of that name to be construted.

        The time can be in arbitrary units, so long as they always the same!,
        in which case the first step of the first job starts at time 0.

        Alternatively, the time can be in Julian days witth 1.0 representing 24 hours.
        https://en.wikipedia.org/wiki/Julian_day
    */
    cStep( string name, string machine, float time );

    /** CTOR of null step

    Used by search methods which return the step found
    or the null step on failure.
     */
    cStep()
        : myID( -99 )
    {

    }
    /** True if null step

    Used to check if step returned by search method
    indicates failure
     */
    bool IsNull()
    {
        return ( myID == -99 );
    }

    /** Output the step in JSON format */
    json::Object json();

    /** Name of machine used to process step */
    string Machine()
    {
        return myMachine;
    }
    /** Time taken by step on machine */
    float Time()
    {
        return myTime;
    }
    void Time( float t )
    {
        myTime = t;
    }

    /** Start time of step on machine */
    float Start()
    {
        return myStart;
    }

    /** Set start time of step on machine */
    void Start( float t )
    {
        myStart = t;
    }

    /** ID of step

    This is set automatically when the step is contructed.

    The defualt copy constructor simple copies the ID
    so every time a step is copied, e.g. when it is copied into a STL vector,
    the copy keeps the same ID.  This means that the original step can always be
    found from a copy so that it can be updated.

    */
    int ID() const
    {
        return myID;
    }

    /** ID of previous step in this job, -99 for first step */
    int Previous()
    {
        return myPrevious;
    }
    /** Set ID of previous step in job

    This is set automatically when the step is added to the job

    */
    void Previous( int id )
    {
        myPrevious = id;
    }

    /** time when step will be completed */
    float Finish()
    {
        return myStart + myTime;
    }

private:
    string myName;
    string myMachine;
    float myTime;
    int myPrevious;
    float myStart;
    int myID;
    static int LastID;
};

/** Processing steps that must be done in sequence */
class cJob
{
public:

    enum class eType
    {
        none,
        sequential,     // all steps must be completed in order
        anyone          // any one step can be done
    };

    cJob( const string& name,
          eType type )
        : myName( name )
        , myType( type )
    {

    }

    /** Add next step in job
        @param[in] machine name
        @param[in] time needed on machine
    */
    void Add( const string& machine,
              float time );

    /** Job in JSON format */
    json::Object json();

    /** All the steps in a job, copied into a vector */
    void Steps( vector< cStep >& vStep ) ;

    /** Find step with id */
    cStep& FindStep( int id );

    eType Type()
    {
        return myType;
    }

    string Name()
    {
        return myName;
    }

    vector< cStep >::iterator begin()
    {
        return myStep.begin();
    }
    vector< cStep >::iterator end()
    {
        return myStep.end();
    }

private:
    string myName;
    vector< cStep > myStep;
    float myEarliestStart;
    eType myType;
};

class cSchedule
{
public:

    void Add( cJob& job );

    /** Schedule in JSON format */
    string json();

    /** All the steps in a schedule, copied into a vector */
    void Steps( vector< cStep >& vStep );

    /** Find step from ID */
    cStep& FindStep( int id );

    cJob::eType Type()
    {
        if( ! myJob.size() )
            return cJob::eType::none;
        return myJob[0].Type();
    }

    int CountJobs()
    {
        return (int) myJob.size();
    }

    vector< cJob >::iterator begin()
    {
        return myJob.begin();
    }
    vector< cJob >::iterator end()
    {
        return myJob.end();
    }
private:
    vector<cJob> myJob;
};
