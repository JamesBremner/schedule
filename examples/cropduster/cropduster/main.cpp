#include <iostream>
#include <vector>
#include <chrono>
#include <set>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;

struct cSeconds : std::tm
{
    cSeconds(const int year, const int month, const int mday,
             const int hour = 0,
             const int min = 0, const int sec = 0, const int isDST = -1)
    {
        tm_year = year - 1900; // [0, 60] since 1900
        tm_mon = month - 1;    // [0, 11] since Jan
        tm_mday = mday;        // [1, 31]
        tm_hour = hour;        // [0, 23] since midnight
        tm_min = min;          // [0, 59] after the hour
        tm_sec = sec;          // [0, 60] after the min
        //         allows for 1 positive leap second
        tm_isdst = isDST;      // [-1...] -1 for unknown, 0 for not DST,
        //         any positive value if DST.
    }

    float secs()
    {
        return (float) mktime(this);
    }
    static float Oneday()
    {
        return (float)( 24 * 60 * 60 );
    }


};

class cFarm
{
public:
    vector< float > vDate;
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
    for( auto& f : vFarm )
    {
        int count = 1;
        for( auto& d : f.vDate )
        {
            stringstream ss;
            ss << f.myName << "(" << count << "}";
            count++;
            cJob J(ss.str(), cJob::eType::anyone );
            J.EarlistStart( d );
            for( auto& p : vPilot )
            {
                J.Add( p.myName, 1 );
            }
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

void testInput()
{
    cFarm f1;
    f1.myName = 'F';
    f1.vDate.push_back( 1 );
    f1.vDate.push_back( 3 );
    f1.vDate.push_back( 6 );
    f1.vDate.push_back( 9 );
    vFarm.push_back( f1 );

    cFarm f2;
    f2.myName = 'G';
    f2.vDate.push_back( 3 );
    f2.vDate.push_back( 6 );
    f2.vDate.push_back( 9 );
    f2.vDate.push_back( 12 );
    vFarm.push_back( f2 );

    cPilot p1;
    p1.myName = "P";
    vPilot.push_back(p1);
    p1.myName = "Q";
    vPilot.push_back(p1);
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

    cSeconds t(year, month, day );
    float d = t.secs();
    f.vDate.push_back( d + 30 * t.Oneday());
    f.vDate.push_back( d + 60 * t.Oneday() );
    f.vDate.push_back( d + 80 * t.Oneday() );
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
                    char no[250];
                    time_t tt = d;
                    strftime(no, sizeof(no), "%Y %m %d", localtime(&tt));
                    cout << no << ", ";
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
