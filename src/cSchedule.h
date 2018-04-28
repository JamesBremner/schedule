#pragma once

#include <iostream>
#include <vector>
#include <chrono>

#include "nlohmann_json.hpp"

using namespace std;

namespace raven
{
namespace sch
{
typedef chrono::time_point<chrono::system_clock> date_t;
string fTime( const string& format, date_t tp );
date_t fTime( int year, int month, int day );



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

    */
    cStep( string name, string machine );

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
    nlohmann::json json();

    /** Name of machine used to process step */
    string Machine() const
    {
        return myMachine;
    }

    /** Cost of step on machine */
    float Cost()
    {
        return myCost;
    }
    void Cost( float c )
    {
        myCost = c;
    }

    /** Duration of step on machine */
    std::chrono::seconds Duration()
    {
        return myDuration;
    }
    void Duration( std::chrono::seconds s )
    {
        myDuration = s;
    }

    /** Start time of step on machine */
    date_t Start() const
    {
        return myStart;
    }

    /** Start time of step on machine in formatted text
        @param[in] format string, e.g. "%Y %m %d"
    */
    string fStartTime( const string& format ) const
    {
        return raven::sch::fTime( format, myStart );
    }

    /** Set start time of step on machine */
    void Start( date_t t )
    {
        myStart = t;
        myAssigned = true;
    }

    /** True if step has been assigned to machine */
    bool IsAssigned() const
    {
        return myAssigned;
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
    date_t Finish()
    {
        return myStart + myDuration;
    }

    /** Name of job of which this is a step */
    void Job( const string& name )
    {
        myJob = name;
    }
    string Job() const
    {
        return myJob;
    }

    /** Steps are ordered by their assigned start time */
    bool operator<( const cStep& other) const
    {
        return myStart < other.myStart;
    }

    void Print()
    {
        cout << myMachine << " ";
    }



private:
    string myName;
    string myMachine;
    string myJob;
    float  myCost;
    chrono::seconds myDuration;
    date_t   myStart;
    bool   myAssigned;
    int myPrevious;
    int myID;
    static int LastID;
};

/** Processing steps that must be done to complete job */
class cJob
{
public:

    enum class eType
    {
        none,
        sequential,     // all steps must be completed in order
        anyone          // any one step can be done
    };

    /** CTOR

    @param[in] name
    @param[in] type: sequential or anyone

    Sequential jobs have multiple steps that must be done in sequence

    Anyone jobs are complete in one step

    */
    cJob( const string& name,
          eType type )
        : myName( name )
        , myType( type )
    {

    }

    void Name( const string& name )
    {
        myName = name;
    }
    void EarlistStart( date_t t )
    {
        myEarliestStart = t;
    }


    /** Add next step in job
        @param[in] machine name
        @param[in] cost of running job on machine
        @param[in] time needed on machine
    */
    void Add( const string& machine,
              float cost,
              chrono::seconds time = chrono::seconds{0} );

    /** Job in JSON format */
    nlohmann::json json();

    /** Steps in the job
        @return new vector containing steps in this job

        These are copies of the steps in the job,
        with the same ID as the originals
    */
    vector< cStep > Steps();

    /** All the steps in a job, copies added to a vector */
    void AddSteps( vector< cStep >& vStep ) ;

    void SetSteps( vector< cStep >& vStep )
    {
        myStep = vStep;
    }

    /** Find step with id */
    cStep& FindStep( int id );

    /** Find step with machine name */
    cStep& FindStep( const string& machineName );

    /** True if job type is anyone and one of the steps has been assigned to a machine */
    bool IsAnyoneAssigned();

    eType Type()
    {
        return myType;
    }
    date_t EarliestStart() const
    {
        return myEarliestStart;
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
    date_t myEarliestStart;
    eType myType;
};

/** A collection of jobs that must be assigned to machines */
class cSchedule
{
public:

    /** Add job to schedule */
    void Add( cJob& job );

    /** Schedule in JSON format */
    nlohmann::json json();

    /** All the steps in a schedule, copied into a vector */
    //void Steps( vector< cStep >& vStep );

    /** Find step from ID */
    cStep& FindStep( int id );

    /** Type of job in schedule
        @return job type

        This is the type of the first job,
        all jobs are assumed to be of the same type
    */
    cJob::eType Type()
    {
        if( ! myJob.size() )
            return cJob::eType::none;
        return myJob[0].Type();
    }

    /** Number of jobs in schedule */
    int CountJobs()
    {
        return (int) myJob.size();
    }

    /** Job steps assigned to machines
        @param[out] assigns a set of job steps assigned to machines, ordered by start time

        This is useful when jobs are composed of steps
        any one of which needs to be completed.  usually this
        is some sort of shift scheduling where peaople ( machines )
        are assigned to jobs.  For these, each job has many steps,
        one for each possible machine, and in the result, just one step
        in each job is assigned to a machine.  So, it is convenient
        to have a list of the steps that have been assigned
    */

    void Assignments( multiset< cStep >& assigns );

    /** iterator pointing to first job */
    vector< cJob >::iterator begin()
    {
        return myJob.begin();
    }
    /** iterator pointing one past last job */
    vector< cJob >::iterator end()
    {
        return myJob.end();
    }
private:
    vector<cJob> myJob;
};
}
}
