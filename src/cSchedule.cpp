#include <iostream>
#include <vector>
#include <set>

#include "cSchedule.h"





using namespace std;

namespace raven
{
namespace sch
{
typedef chrono::time_point<chrono::system_clock> date_t;
string fTime( const string& format, date_t tp )
{
    time_t tt = chrono::system_clock::to_time_t( tp );
    char buf[250];
    if( ! strftime(
                buf,
                sizeof(buf),
                format.c_str(),
                localtime( &tt ) ) )
        return "----/--/--";

    return string( buf );
}
date_t fTime( int year, int month, int day )
{

    tm seed;
    seed.tm_year = year - 1900;
    seed.tm_mon = month - 1;    // [0, 11] since Jan
    seed.tm_mday = day;        // [1, 31]
    seed.tm_hour = 0;        // [0, 23] since midnight
    seed.tm_min = 0;          // [0, 59] after the hour
    seed.tm_sec = 0;          // [0, 60] after the min
    seed.tm_isdst = 0;

    return chrono::system_clock::from_time_t( mktime( &seed ));
}
int cStep::LastID = -1;


cStep::cStep( string name, string machine )
    : myName( name )
    , myMachine( machine )
    , myAssigned( false )
{
    myID = ++LastID;
    LastID = myID;
}



nlohmann::json cStep::json()
{
    nlohmann::json j;
    j["name"] = myName;
    j["machine"] = myMachine;
    j["cost"] = myCost;
    j["previous"] = myPrevious;
    j["start"] = raven::sch::fTime("%Y-%m-%d",myStart);
    return j;
}

nlohmann::json cJob::json()
{
    nlohmann::json j;
    j["name"] = myName;
    j["earliest"] =  raven::sch::fTime("%Y-%m-%d",myEarliestStart);
    switch( myType )
    {
    case eType::sequential:
        j["type"] = "squential";
        break;
    case eType::anyone:
        j["type"] = "anyone";
        break;
    default:
        j["type"] = "???";
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
    float cost,
    chrono::seconds duration
)
{
    int previous = -99;
    if( myStep.size() > 0 )
        previous = myStep.back().ID();
    cStep step( "", machine );
    step.Previous( previous );
    step.Cost( cost );
    step.Duration( duration ) ;
    myStep.push_back( step );
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


nlohmann::json cSchedule::json()
{

    nlohmann::json s;
    nlohmann::json ja;
    for( auto& j : myJob )
    {
        ja.push_back( j.json() );
    }
    s["jobs"] = ja;

    return s;


}

void cSchedule::Steps( vector< cStep >& vStep )
{
    vStep.clear();
    for( auto& j : myJob )
    {
        j.AddSteps( vStep );
    }
}

set< date_t > cSchedule::JobStartTimes()
{
    set< date_t > StartTimes;
    for( auto& job : myJob )
    {
        StartTimes.insert( job.EarliestStart() );
    }
    return StartTimes;
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

void cSchedule::Assignments( multiset< cStep >& assigns )
{
    for( auto& job : myJob )
    {
        for( auto& step : job )
        {
            if( step.IsAssigned() )
            {
                step.Job( job.Name() );
                assigns.insert( step );
            }
        }
    }
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

cStep& cJob::FindStep( const string& machineName )
{
    //cout << "cJob::FindStep looking for " << machineName << " in " << myName << "\n";
    for( auto& s : myStep )
    {
        if( s.Machine() == machineName )
        {
            return s;
        }
    }
    static cStep null;
    return null;
}

bool cJob::IsAnyoneAssigned()
{
    if( myType != eType::anyone )
        return false;
    for( auto& step : myStep )
    {
        if( step.IsAssigned() )
            return true;
    }
    return false;
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
}
}
