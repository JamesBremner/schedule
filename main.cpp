#include <iostream>
#include <vector>
#include <set>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;


void ConstructTestSchedule1()
{
    cSchedule S;

    cJob J("A-B",0);

    J.Add(
        "A",
        10) ;
    J.Add(
        "B",
        2 );
    S.Add( J );

    cJob J2("A-C",0);
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


int main()
{

    ConstructTestSchedule1();

    return 0;
}
