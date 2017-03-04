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
        @param[in] time spent o machine
    */
    cStep( string name, string machine, float time );

    /** CTOR of null step */
    cStep()
    : myID( -99 )
    {

    }

    /** Output the step in JSON format */
    json::Object json();

    string Machine()
    {
        return myMachine;
    }
    void Start( float t )
    {
        myStart = t;
    }
    float Time()
    {
        return myTime;
    }
    int ID() const
    {
        return myID;
    }

    float Start()
    {
        return myStart;
    }

    bool IsNull()
    {
        return ( myID == -99 );
    }

    int Previous()
    {
        return myPrevious;
    }
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
    cJob( string name, float earliest )
        : myName( name )
        , myEarliestStart( earliest )
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

private:
    string myName;
    vector< cStep > myStep;
    float myEarliestStart;
};

class cSchedule
{
public:
    void Add( cJob& job )
    {
        myJob.push_back( job );
    }

    /** Schedule in JSON format */
    string json();

     /** All the steps in a schedule, copied into a vector */
    void Steps( vector< cStep >& vStep );

    /** Find step from ID */
    cStep& FindStep( int id );

private:
    vector<cJob> myJob;
};
