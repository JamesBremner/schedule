#include <iostream>
#include <vector>
#include <set>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;


void TestSchedule1()
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
    if( S.FindStep( 0 ).Start() != 0 )
        cout << "step 0 failed\n";
    if( S.FindStep( 1 ).Start() != 10 )
        cout << "step 1 failed\n";
    if( S.FindStep( 2 ).Start() != 10 )
        cout << "step 2 failed\n";
    if( S.FindStep( 3 ).Start() != 11 )
        cout << "step 3 failed\n";

}

void TestSchedule2()
{
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
    Shop.Manufacture( S );
    cout << S.json() << "\n";

    if( S.begin()->begin()->Start() != 1 )
        cout << "FAILED 1 " << S.begin()->begin()->Start() << "\n";
    if( ((S.begin()+1)->begin()+1)->Start() != 1 )
        cout << "FAILED 2 " << "\n";
    if( ((S.begin()+2)->begin()+2)->Start() != 1 )
        cout << "FAILED 3 " << "\n";


}



int main()
{


    TestSchedule1();
    TestSchedule2();

    return 0;
}
