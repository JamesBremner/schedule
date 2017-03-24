#include <iostream>
#include <vector>
#include <set>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;
using namespace raven::sch;

bool TestSchedule1()
{
    raven::sch::cSchedule S;

    raven::sch::cJob J("A-B", cJob::eType::sequential );

    J.Add(
        "A",
        10,
        chrono::seconds{10}) ;
    J.Add(
        "B",
        2,
        chrono::seconds{2} );
    S.Add( J );

    raven::sch::cJob J2("A-C", cJob::eType::sequential );
    J2.Add(
        "A",
        1,
        chrono::seconds{1} );
    J2.Add(
        "C",
        20,
        chrono::seconds{20} );
    S.Add( J2 );

    raven::sch::cShop Shop( S );
    Shop.Manufacture( S );
//   cout << S.json() << "\n";

    bool ret = true;
    if( ! S.FindStep( 0 ).IsAssigned() )
    {
        cout << "step 0 failed\n";
        ret = false;
    }
    if( ! S.FindStep( 1 ).IsAssigned() )
    {
        cout << "step 1 failed\n";
        ret = false;
    }
    if( ! S.FindStep( 2 ).IsAssigned() )
    {
        cout << "step 2 failed\n";
        ret = false;
    }
    if( ! S.FindStep( 3 ).IsAssigned() )
    {
        cout << "step 3 failed\n";
        ret = false;
    }
    return ret;

}

bool TestSchedule2()
{
    // Set up sample problem described in
    // https://en.wikipedia.org/wiki/Hungarian_algorithm

    cSchedule S;

    cJob J("Armond", cJob::eType::anyone );
    J.Add(
        "Clean",
        2) ;
    J.Add(
        "Sweep",
        3 );
    J.Add(
        "Wash",
        3 );
    S.Add( J );

    cJob J2("Francine", cJob::eType::anyone );
    J2.Add(
        "Clean",
        3 ) ;
    J2.Add(
        "Sweep",
        2 );
    J2.Add(
        "Wash",
        3 );
    S.Add( J2 );

    cJob J3("Herbert", cJob::eType::anyone );
    J3.Add(
        "Clean",
        3) ;
    J3.Add(
        "Sweep",
        3 );
    J3.Add(
        "Wash",
        2 );
    S.Add( J3 );


    cShop Shop( S );
    float cost = Shop.Manufacture( S );
//   cout << S.json() << "\n";

    bool ret = true;
    if( cost != 6 )
    {
        cout << "Failed to optimize cost\n";
        ret = false;
    }
    if( ! S.begin()->begin()->IsAssigned() )
    {
//        cout << "FAILED 1 " << S.begin()->begin()->Start() << "\n";
        ret = false;
    }
    if( ! ((S.begin()+1)->begin()+1)->IsAssigned() )
    {
        cout << "FAILED 2 " << "\n";
        ret = false;
    }
    if( ! ((S.begin()+2)->begin()+2)->IsAssigned() )
    {
        cout << "FAILED 3 " << "\n";
        ret = false;
    }
    return ret;
}



bool TestSchedule3()
{
    // Set up sample problem described in
    // https://en.wikipedia.org/wiki/Hungarian_algorithm

    cSchedule S;

    cJob J("Armond", cJob::eType::anyone );
    J.Add(
        "Clean",
        2) ;
    J.Add(
        "Sweep",
        3 );
    J.Add(
        "Wash",
        3 );
//    J.Add(
//        "Mend",
//        3 );
    S.Add( J );

    cJob J2("Francine", cJob::eType::anyone );
    J2.Add(
        "Clean",
        3 ) ;
    J2.Add(
        "Sweep",
        2 );
    J2.Add(
        "Wash",
        3 );
//    J2.Add(
//        "Mend",
//        3 );

    S.Add( J2 );

    cJob J3("Herbert", cJob::eType::anyone );
    J3.Add(
        "Clean",
        3) ;
    J3.Add(
        "Sweep",
        3 );
    J3.Add(
        "Wash",
        2 );
//    J3.Add(
//        "Mend",
//        3 );

    S.Add( J3 );

    cJob J4("Xavier", cJob::eType::anyone );
//    J4.Add(
//        "Clean",
//        3) ;
//    J4.Add(
//        "Sweep",
//        3 );
//    J4.Add(
//        "Wash",
//        2 );
    J4.Add(
        "XMend",
        3 );

    S.Add( J4 );

    cShop Shop( S );
    float cost = Shop.Manufacture( S );
    //cout << S.json() << "\n";

    bool ret = true;
    if( cost != 9 )
    {
        cout << "Failed to optimize cost\n";
        ret = false;
    }
    if( ! S.begin()->begin()->IsAssigned()  )
    {
        //cout << "FAILED 1 " << S.begin()->begin()->Start() << "\n";
        ret = false;
    }
    if( ! ((S.begin()+1)->begin()+1)->IsAssigned()  )
    {
        cout << "FAILED 2 " << "\n";
        ret = false;
    }
    if( ! ((S.begin()+2)->begin()+2)->IsAssigned()  )
    {
        cout << "FAILED 3 " << "\n";
        ret = false;
    }
    return ret;

}


bool TestSchedule4()
{
    cSchedule S;

    cJob J("Shift1-1", cJob::eType::anyone );
    J.EarlistStart( chrono::system_clock::now() );

    // Each shift occupies a doctor for one day
    // that is, do not assign a doctor to another shift before 24 hours
    J.Add("D1",0,chrono::hours{24});
    J.Add("D2",0,chrono::hours{24});
    J.Add("D3",0,chrono::hours{24});
    S.Add( J );
    J.Name( "Shift1-2");
    J.EarlistStart( chrono::system_clock::now() + chrono::hours{8} );
    S.Add( J );
    J.Name( "Shift1-3");
    J.EarlistStart( chrono::system_clock::now() + chrono::hours{16} );
    S.Add( J );
    J.Name( "Shift2-1");
    J.EarlistStart(chrono::system_clock::now() + chrono::hours{24} );
    S.Add( J );
    J.Name( "Shift2-2");
    J.EarlistStart( chrono::system_clock::now() + chrono::hours{32} );
    S.Add( J );
    J.Name( "Shift1-3");
    J.EarlistStart( chrono::system_clock::now() + chrono::hours{40} );
    S.Add( J );

    cShop Shop( S );
    Shop.Manufacture( S );
    nlohmann::json s = S.json();

    stringstream ss;
    ss << std::setw(4) << s;
    //cout << ss.str() << endl;

    bool ret = true;
    if( s["jobs"][0]["steps"][0]["start"] < 0 )
        ret = false;
    if(  s["jobs"][1]["steps"][1]["start"] < 0 )
        ret = false;
    if(  s["jobs"][2]["steps"][2]["start"] < 0 )
        ret = false;
    if(  s["jobs"][3]["steps"][0]["start"] < 0 )
        ret = false;
    if(  s["jobs"][4]["steps"][1]["start"] < 0 )
        ret = false;
    if(  s["jobs"][5]["steps"][2]["start"] < 0 )
        ret = false;

    return ret;
}

bool TestSchedule5()
{
    class cFarm
    {
    public:
        vector< raven::sch::date_t > vDate;
        string myName;
    };

    class cPilot
    {
    public:
        cPilot( const string& name ) : myName( name ) {}
        string myName;
    };

    vector< cFarm > vFarm;
    vector< cPilot > vPilot;

    vPilot.push_back( cPilot( "P" ));
    vPilot.push_back( cPilot( "Q" ));

    cFarm f;
    f.myName = "F";
    f.vDate.push_back( raven::sch::fTime( 2017, 3, 22 ));
    f.vDate.push_back( raven::sch::fTime( 2017, 4, 22 ));
    vFarm.push_back( f );
    f.myName = "G";
    vFarm.push_back( f );
    f.myName = "H";
    vFarm.push_back( f );

    cSchedule S;

    // loop over farms
    for( auto& f : vFarm )
    {
        // loop over dusting requests for farm
        int count = 1;
        for( auto& d : f.vDate )
        {
            // construct job
            stringstream ss;
            ss << f.myName << "(" << count << ")";
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
            S.Add( J );
        }
    }

    cShop Shop( S );
    Shop.Manufacture( S );

    bool ret = true;
    int count = 0;
    multiset< cStep > assigns;
    S.Assignments( assigns );
    if( (int)assigns.size() != 6 )
        ret = false;
    for( auto& step : assigns )
    {
        cout << "Pilot " << step.Machine()
             << " dusts farm " << step.Job()
             << " on " << step.fStartTime("%Y %m %d")
             << "\n";

        switch( count )
        {
        case 0:

            if( step.Machine() != "P" )
                ret = false;
            if( step.Job() != "F(1)")
                ret = false;
            if( step.Start() != raven::sch::fTime( 2017, 3, 22 ) )
                ret = false;
            break;

        case 1:

            if( step.Machine() != "Q" )
                ret = false;
            if( step.Job() != "G(1)")
                ret = false;
            if( step.Start() != raven::sch::fTime( 2017, 3, 22 ) )
                ret = false;
            break;

        case 2:

            if( step.Machine() != "P" )
                ret = false;
            if( step.Job() != "H(1)")
                ret = false;
            if( step.Start() != raven::sch::fTime( 2017, 3, 23 ) )
                ret = false;
            break;

        }
        count++;
    }



    return ret;
}


int main()
{

    bool res = true;
    if( ! TestSchedule1() )
        res = false;
    if( ! TestSchedule2() )
        res = false;
    if( ! TestSchedule3() )
        res = false;
    if( ! TestSchedule4() )
        res = false;
    if( ! TestSchedule5() )
        res = false;

    if( res )
    {
        cout << "All tests passed\n";
    }
    else
    {
        cout << "TEST FAILED\n";
    }
    return 0;
}
