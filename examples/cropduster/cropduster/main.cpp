#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <set>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;


class cFarm
{
public:
    vector< raven::sch::tp_t > vDate;
    string myName;
};

class cPilot
{
public:
    string myName;
};

vector< cFarm > vFarm;
vector< cPilot > vPilot;

cSchedule theSchedule;

void Input()
{
    // loop over farms
    for( auto& f : vFarm )
    {
        // loop over dusting requests for farm
        int count = 1;
        for( auto& d : f.vDate )
        {
            // construct job
            stringstream ss;
            ss << f.myName << "(" << count << "}";
            count++;
            cJob J(ss.str(), cJob::eType::anyone );
            J.EarlistStart( d );

            // loop over pilots
            // every pilot can do every job at same cost!
            // and takes them the entire day
            for( auto& p : vPilot )
            {
                J.Add( p.myName,
                       1,
                       chrono::hours{24} );
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
//    nlohmann::json s = theSchedule.json();
//
//    stringstream ss;
//    ss << std::setw(4) << s;
//    cout << ss.str() << endl;

    multiset< cStep > assigns;
    theSchedule.Assignments( assigns );
    for( auto& step : assigns )
    {
        cout << "Pilot " << step.Machine()
             << " dusts farm " << step.Job()
             << " on " << step.fStartTime("%Y %m %d")
             << "\n";
    }
}

void FarmPrompt()
{
    cout << "Enter farm's name\n";
    string name;
    cin >> name;
    cFarm f;
    f.myName = name;

    cout << "Enter seeding date: YYYY MM DD\n";
    int year, month, day;
    cin >> year >> month >> day;

    //cout << year <<" "<< month <<" " << day << "\n";

    cout << "requesting dusting 30, 60 and 80 days after seeding\n";

    raven::sch::tp_t tp = raven::sch::fTime( year, month, day );

    f.vDate.push_back( tp + chrono::hours{30*24} );
    f.vDate.push_back( tp + chrono::hours{60*24} );
    f.vDate.push_back( tp + chrono::hours{80*24} );
    vFarm.push_back( f );
}
void PilotPrompt()
{
    cout << "Enter pilot's name\n";
    string name;
    cin >> name;
    cPilot p1;
    p1.myName = name;
    vPilot.push_back(p1);

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
    for( auto& jf : j["farms"] )
    {
        cFarm f;
        f.myName = jf["name"];

        int year, month, day;
        year = atoi( jf["seed"].get<std::string>().substr(0,4).c_str());
        month = atoi( jf["seed"].get<std::string>().substr(6,2).c_str());
        day = atoi( jf["seed"].get<std::string>().substr(9,2).c_str());
        raven::sch::tp_t tp = raven::sch::fTime( year, month, day );
        for( int jd : jf["days"] )
        {
            f.vDate.push_back( tp + chrono::hours{jd*24} );
        }
        vFarm.push_back( f );
    }
    for( auto& jp : j["pilots"])
    {
        cPilot p;
        p.myName = jp["name"];
        vPilot.push_back(p);

    }

}


void Prompt()
{
    while( 1 )
    {
        if( vFarm.size() )
        {
            for( auto& f : vFarm )
            {
                cout << "Farm " << f.myName << " dates: ";
                for( auto d : f.vDate )
                {
                    cout << raven::sch::fTime("%Y-%m-%d",d);
                    cout << ", ";
                }
                cout << "\n";
            }
        }
        if( vPilot.size() )
        {
            cout << "Pilots: ";
            for ( auto& p : vPilot )
            {
                cout << " " << p.myName << ", ";
            }
            cout << "\n";
        }
        cout << "R read problem from file\n";
        cout << "F add farm\n";
        cout << "P add pilot\n";
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
        case 'f':
        case 'F':
            FarmPrompt();
            break;
        case 'P':
        case 'p':
            PilotPrompt();
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

//    nlohmann::json j;
//    j = {{
//    "farms",{{
//        {
//            "name","F"
//        },
//        {
//            "seed","2017-03-22"
//        },
//        {
//            "days",{ 30, 60, 80 }
//        }
//    }}}};

//    ofstream ofs("test.txt");
//    ofs << setw(2) << j;
//    ofs.close();



    Prompt();
    Input();
    Calculate();
    DisplayResults();

    return 0;
}
