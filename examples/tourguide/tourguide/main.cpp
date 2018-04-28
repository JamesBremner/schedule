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
};

class cGuide
{
public:
    string myName;
};

vector< cTour > vTour;
vector< cGuide > vGuide;

cSchedule theSchedule;

void Input()
{
    // loop over tours
    for( auto& t : vTour )
    {
        // counstruct job
        cJob J(
             "guides tour " + t.myName,
             cJob::eType::anyone );

        // loopp over guides
        for( auto& g : vGuide )
        {
            // every guide can do every tour!
            J.Add( g.myName, 1, chrono::hours{24} );
        }

        // add job to schedule
        theSchedule.Add( J );
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
            // << " on " << step.fStartTime("%Y %m %d")
             << "\n";
    }
}

void TourPrompt()
{
    cout << "Enter tour's name\n";
    string name;
    cin >> name;
    cTour f;
    f.myName = name;

//    cout << "Enter seeding date: YYYY MM DD\n";
//    int year, month, day;
//    cin >> year >> month >> day;
//
//    //cout << year <<" "<< month <<" " << day << "\n";
//
//    cout << "requesting dusting 30, 60 and 80 days after seeding\n";
//
//    raven::sch::date_t tp = raven::sch::fTime( year, month, day );
//
//    f.vDate.push_back( tp + chrono::hours{30*24} );
//    f.vDate.push_back( tp + chrono::hours{60*24} );
//    f.vDate.push_back( tp + chrono::hours{80*24} );
    vTour.push_back( f );
}
void GuidePrompt()
{
    cout << "Enter guide's name\n";
    string name;
    cin >> name;
    cGuide p1;
    p1.myName = name;
    vGuide.push_back(p1);

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
        cTour f;
        f.myName = jf["name"];

        int year, month, day;
        year = atoi( jf["seed"].get<std::string>().substr(0,4).c_str());
        month = atoi( jf["seed"].get<std::string>().substr(6,2).c_str());
        day = atoi( jf["seed"].get<std::string>().substr(9,2).c_str());
        raven::sch::date_t tp = raven::sch::fTime( year, month, day );
        for( int jd : jf["days"] )
        {
            f.vDate.push_back( tp + chrono::hours{jd*24} );
        }
        vTour.push_back( f );
    }
    for( auto& jp : j["guides"])
    {
        cGuide p;
        p.myName = jp["name"];
        vGuide.push_back(p);

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
                cout << "Tour " << t.myName;
//                cout << "Farm " << f.myName << " dates: ";
//                for( auto d : f.vDate )
//                {
//                    cout << raven::sch::fTime("%Y-%m-%d",d);
//                    cout << ", ";
//                }
                cout << "\n";
            }
        }
        if( vGuide.size() )
        {
            cout << "guides: ";
            for ( auto& p : vGuide )
            {
                cout << " " << p.myName << ", ";
            }
            cout << "\n";
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
