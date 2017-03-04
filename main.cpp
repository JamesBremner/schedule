#include <iostream>
#include <vector>
#include <set>

#include "cSchedule.h"
#include "cShop.h"

using namespace std;


cSchedule ConstructTestSchedule1()
{
    cSchedule S;

    cJob J("A-B",0);

    J.Add( cStep(
               "A",
               "A",
               10) );
    J.Add( cStep(
               "B",
               "B",
               2) );
    S.Add( J );

    cJob J2("A-C",0);
    J2.Add( cStep(
                "A",
                "A",
                1) );
    J2.Add( cStep(
                "C",
                "C",
                20) );
    S.Add( J2 );

    return S;
}


int main()
{

    cSchedule S = ConstructTestSchedule1();
    //cout << S.json() << "\n";
    cShop Shop( S );
    Shop.Manufacture( S );
    cout << S.json() << "\n";
    return 0;
}
