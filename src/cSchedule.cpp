#include <iostream>
#include <vector>

#include "cSchedule.h"

int cStep::LastID = -1;

using namespace std;

cStep::cStep( string name, string machine, float time )
    : myName( name )
    , myMachine( machine )
    , myTime( time )
    , myStart( -1 )
{
    myID = ++LastID;
    LastID = myID;
}



nlohmann::json cStep::json()
{
    nlohmann::json j;
    j["name"] = myName;
    j["machine"] = myMachine;
    j["time"] = myTime;
    j["previous"] = myPrevious;
    j["start"] = myStart;
    return j;
}

nlohmann::json cJob::json()
{
    nlohmann::json j;
    j["name"] = myName;
    j["earliest"] = myEarliestStart;
    switch( myType )
    {
    case eType::sequential:
        j["type"] = "squential";
        break;
    case eType::anyone:
        j["type"] = "anyone";
        break;
    }
    nlohmann::json steps;
    for( auto& s : myStep )
    {
        steps.push_back( s.json() );
    }
    j["steps"] = steps;
    return j;

}

void cJob::Add(
    const string& machine,
    float time )
{
    int previous = -99;
    if( myStep.size() > 0 )
        previous = myStep.back().ID();
    myStep.push_back( cStep( "", machine, time ) );
    myStep.back().Previous( previous );
}
vector< cStep > cJob::Steps()
{
    vector< cStep > vs;
    for( auto& s : myStep )
    {
        vs.push_back( s );
    }
    return vs;
}
void cJob::AddSteps( vector< cStep >& vStep )
{
    for( auto& s : myStep )
    {
        vStep.push_back( s );
    }
}


string cSchedule::json()
{

    nlohmann::json s;
    nlohmann::json ja;
    for( auto& j : myJob )
    {
        ja.push_back( j.json() );
    }
    s["jobs"] = ja;
    string ret;
    ret = s.dump();
    return ret;
}

void cSchedule::Steps( vector< cStep >& vStep )
{
    vStep.clear();
    for( auto& j : myJob )
    {
        j.AddSteps( vStep );
    }
}
cStep& cSchedule::FindStep( int id )
{
    for( auto& j : myJob )
    {
        cStep& s = j.FindStep( id );
        if( ! s.IsNull() )
            return s;
    }
    static cStep null;
    return null;
}

cStep& cJob::FindStep( int id )
{
    for( auto& s : myStep )
    {
        if( s.ID() == id )
            return s;
    }
    static cStep null;
    return null;
}

void cSchedule::Add( cJob& job )
{
    // check that all jobs have the same type
    if( myJob.size() )
    {
        if( job.Type() != myJob.back().Type() )
            throw std::runtime_error(
                "All jobs must have same type");
    }

    myJob.push_back( job );
}
