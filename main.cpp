#include <iostream>
#include <vector>
#include <set>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;


bool TestSchedule1()
{
    cSchedule S;

    cJob J("A-B", cJob::eType::sequential );

    J.Add(
        "A",
        10) ;
    J.Add(
        "B",
        2 );
    S.Add( J );

    cJob J2("A-C", cJob::eType::sequential );
    J2.Add(
        "A",
        1 );
    J2.Add(
        "C",
        20 );
    S.Add( J2 );

    cShop Shop( S );
    Shop.Manufacture( S );
    cout << S.json() << "\n";

    bool ret = true;
    if( S.FindStep( 0 ).Start() != 0 )
    {
        cout << "step 0 failed\n";
        ret = false;
    }
    if( S.FindStep( 1 ).Start() != 10 )
    {
        cout << "step 1 failed\n";
        ret = false;
    }
    if( S.FindStep( 2 ).Start() != 10 )
    {
        cout << "step 2 failed\n";
        ret = false;
    }
    if( S.FindStep( 3 ).Start() != 11 )
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
    cout << S.json() << "\n";

    bool ret = true;
    if( cost != 6 )
    {
        cout << "Failed to optimize cost\n";
        ret = false;
    }
    if( S.begin()->begin()->Start() != 1 )
    {
        cout << "FAILED 1 " << S.begin()->begin()->Start() << "\n";
        ret = false;
    }
    if( ((S.begin()+1)->begin()+1)->Start() != 1 )
    {
        cout << "FAILED 2 " << "\n";
        ret = false;
    }
    if( ((S.begin()+2)->begin()+2)->Start() != 1 )
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
    cout << S.json() << "\n";

    bool ret = true;
    if( cost != 9 )
    {
        cout << "Failed to optimize cost\n";
        ret = false;
    }
    if( S.begin()->begin()->Start() != 1 )
    {
        cout << "FAILED 1 " << S.begin()->begin()->Start() << "\n";
        ret = false;
    }
    if( ((S.begin()+1)->begin()+1)->Start() != 1 )
    {
        cout << "FAILED 2 " << "\n";
        ret = false;
    }
    if( ((S.begin()+2)->begin()+2)->Start() != 1 )
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
    J.EarlistStart( 0 );

    // Each shift occupies a doctor ofr one day
    // that is, do not assign a doctor to another shift before 24 hours
    J.Add("D1",1);
    J.Add("D2",1);
    J.Add("D3",1);
    S.Add( J );
    J.Name( "Shift1-2");
    J.EarlistStart( .34 );
    S.Add( J );
    J.Name( "Shift1-3");
    J.EarlistStart( 0.67 );
    S.Add( J );
    J.Name( "Shift2-1");
    J.EarlistStart( 1 );
    S.Add( J );
    J.Name( "Shift2-2");
    J.EarlistStart( 1.34 );
    S.Add( J );
    J.Name( "Shift1-3");
    J.EarlistStart( 1.67 );
    S.Add( J );

    cShop Shop( S );
    Shop.Manufacture( S );
    nlohmann::json s = S.json();

    stringstream ss;
    ss << std::setw(4) << s;
    cout << ss.str() << endl;

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
