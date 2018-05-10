#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <set>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;
using namespace raven::sch;

class cTour
{
public:
    vector< raven::sch::date_t > vDate;
    string myName;
    int myBuses;
};

class cGuide
{
public:
    string myName;
    int myLevel;
};

vector< cTour > vTour;
vector< cGuide > vGuide;

cSchedule theSchedule;

void Input()
{
    // loop over tours
    for( auto& t : vTour )
    {
        // loop over buses in tour
        for( int b = 0; b < t.myBuses; b++ )
        {
            // construct job
            std::stringstream jobname;
            jobname << "guides tour " << t.myName << " bus " << b+1;
            cJob J(
                jobname.str(),
                cJob::eType::anyone );

            // loop over guides
            for( auto& g : vGuide )
            {
                // check that guide level is sufficient
                if( g.myLevel < 3 - b )
                    continue;

                // guide is up for the job
                J.Add( g.myName, 1, chrono::hours{24} );
            }

            // add job to schedule
            theSchedule.Add( J );
        }
    }
}
void Calculate()
{
    cShop Shop( theSchedule );
    Shop.Manufacture( theSchedule );

}

void DisplayResults()
{

    multiset< cStep > assigns;
    theSchedule.Assignments( assigns );
    for( auto& step : assigns )
    {
        cout << "Guide " << step.Machine()
             << " " << step.Job()
             << " on " << step.fStartTime("%Y %m %d")
             << "\n";
    }
}

void TourPrompt()
{
    cout << "Enter tour's name\n";
    string name;
    cin >> name;
    cTour t;
    t.myName = name;
    cout << "Enter number of buses\n";
    int buses;
    cin >> buses;
    t.myBuses = buses;

    vTour.push_back( t );
}
void GuidePrompt()
{
    cout << "Enter guide's name\n";
    string name;
    cin >> name;
    cGuide g;
    g.myName = name;
    cout << "enter guides level\n";
    int level;
    cin >> level;
    g.myLevel = level;
    vGuide.push_back(g);

}

void FilePrompt()
{
    cout << "Enter file path\n>";
    string path;
    cin >> path;

    ifstream ifs( path );
    if( ! ifs )
    {
        cout << "Cannot open file \"" << path << "\"\n";
        return;
    }

    nlohmann::json j;
    ifs >> j;
    for( auto& jf : j["tours"] )
    {
        cTour t;
        t.myName = jf["name"];
        t.myBuses = jf["buses"];

//        int year, month, day;
//        year = atoi( jf["seed"].get<std::string>().substr(0,4).c_str());
//        month = atoi( jf["seed"].get<std::string>().substr(6,2).c_str());
//        day = atoi( jf["seed"].get<std::string>().substr(9,2).c_str());
//        raven::sch::date_t tp = raven::sch::fTime( year, month, day );
//        for( int jd : jf["days"] )
//        {
//            f.vDate.push_back( tp + chrono::hours{jd*24} );
//        }
        vTour.push_back( t );
    }
    for( auto& jp : j["guides"])
    {
        cGuide g;
        g.myName = jp["name"];
        g.myLevel = jp["level"];
        vGuide.push_back(g);

    }

}


void Prompt()
{
    while( 1 )
    {
        if( vTour.size() )
        {
            for( auto& t : vTour )
            {
                cout << "Tour " << t.myName << " buses " << t.myBuses << "\n";
            }
        }
        if( vGuide.size() )
        {
            for ( auto& p : vGuide )
            {
                cout << "guide " << p.myName << " level " << p.myLevel << "\n";
            }
        }
        cout << "R read problem from file\n";
        cout << "T add tour\n";
        cout << "G add guide\n";
        cout << "C calculate\n";
        cout << "X exit\n";
        cout << ">";
        char ans;
        cin >> ans;
        switch( ans )
        {
        case 'r':
        case 'R':
            FilePrompt();
            break;
        case 't':
        case 'T':
            TourPrompt();
            break;
        case 'g':
        case 'G':
            GuidePrompt();
            break;
        case 'C':
        case 'c':
            return;
        case 'x':
        case 'X':
            exit(0);
        }
    }
}

int main()
{

    cout << "Tour Guide Scheduling Optimization Program\n";

    Prompt();
    Input();
    Calculate();
    DisplayResults();

    return 0;
}
