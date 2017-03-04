#include <iostream>
#include <vector>
#include "json.h"

using namespace std;

class cStep
{
public:
    cStep( string name, string machine, float time, int previous );

    cStep()
    : myID( -99 )
    {

    }

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

class cJob
{
public:
    cJob( string name, float earliest )
        : myName( name )
        , myEarliestStart( earliest )
    {

    }
    int Add( const cStep& step )
    {
        myStep.push_back( step );
        return step.ID();
    }

    json::Object json();
    void Steps( vector< cStep >& vStep ) ;

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
    string json();
    void Steps( vector< cStep >& vStep );

    cStep& FindStep( int id );

private:
    vector<cJob> myJob;
};
