#include <iostream>
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
            for( auto& p : vPilot )
            {
                J.Add( p.myName, 1 );
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

    set< cStep > assigns;
    theSchedule.Assignments( assigns );
    for( auto& step : assigns )
    {
        cout << "Pilot " << step.Machine()
             << " dusts farm " << step.Job()
             << " on " << step.fStartTime("%Y %m %d")
             << "\n";
    }
}

//void testInput()
//{
//    cFarm f1;
//    f1.myName = 'F';
//    f1.vDate.push_back( 1 );
//    f1.vDate.push_back( 3 );
//    f1.vDate.push_back( 6 );
//    f1.vDate.push_back( 9 );
//    vFarm.push_back( f1 );
//
//    cFarm f2;
//    f2.myName = 'G';
//    f2.vDate.push_back( 3 );
//    f2.vDate.push_back( 6 );
//    f2.vDate.push_back( 9 );
//    f2.vDate.push_back( 12 );
//    vFarm.push_back( f2 );
//
//    cPilot p1;
//    p1.myName = "P";
//    vPilot.push_back(p1);
//    p1.myName = "Q";
//    vPilot.push_back(p1);
//}

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
        cout << "F add farm\n";
        cout << "P add pilot\n";
        cout << "C calculate\n";
        char ans;
        cin >> ans;
        switch( ans )
        {
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
        }
    }
}

int main()
{

    //testInput();
    Prompt();
    Input();
    Calculate();
    DisplayResults();

    return 0;
}
