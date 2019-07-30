#include <iostream>
#include <sstream>
#include <iomanip>

#include <nana/gui.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/filebox.hpp>
#include "JobResource.h"

#include "raven_sqlite.h"

#define INSTRUMENT 1

cFleet::cFleet( nana::form& fm )
    : myfm( fm )
    , myShiftRotation( 3 )
    , fleet_text( fm, nana::rectangle( 10,100, 550, 300 ))

{
    fleet_text.typeface(  nana::paint::font("Courier", 10) );
}

void cFleet::Write()
{
    nana::filebox fb( myfm, false );
    auto paths = fb();
    if( paths.empty() )
        return;

    raven::sqlite::cDB DB( paths[0].c_str() );
    DB.Query("CREATE TABLE domain ( job, resource );");
    DB.Query("DELETE FROM domain;");
    DB.Query("INSERT INTO domain VALUES ( '%s', '%s' );",
             myJobTerm.c_str(), myResourceTerm.c_str() );
    DB.Query("CREATE TABLE job_type ( name );");
    DB.Query("DELETE FROM job_type;");
    DB.Query("CREATE TABLE job_resource ( type, resource );");
    DB.Query("DELETE FROM job_resource;");
    int jti = 0;
    for( auto& jt : myTypeVector )
    {
        DB.Query("INSERT INTO job_type VALUES ( '%s' );",
                 jt.Name().c_str() );
        for( auto& jr : jt.CrewType() )
        {
            if( jr.length() )
                DB.Query("INSERT INTO job_resource VALUES ( %d, '%s' );",
                         jti, jr.c_str() );
        }
        jti++;
    }
    DB.Query("CREATE TABLE resource_type ( name );");
    DB.Query("DELETE FROM resource_type;");
    for( auto& rt : myResourceTypeVector )
    {
        DB.Query("INSERT INTO resource_type VALUES ( '%s' );",
                 rt.Type().c_str() );
    }
    DB.Query("CREATE TABLE job ( name, type );");
    DB.Query("DELETE FROM job;");
    for( auto& j : myJobVector )
    {
        DB.Query("INSERT INTO job VALUES ( '%s', '%s' );",
                 j.Plate().c_str(), j.Type().c_str() );
    }
    DB.Query("CREATE TABLE resource ( name, type );");
    DB.Query("DELETE FROM resource;");
    for( auto& r : myResourceVector )
    {
        DB.Query("INSERT INTO resource VALUES ( '%s', '%s' );",
                 r.Name().c_str(), r.Type().c_str() );
    }
}

void cFleet::Read()
{
    nana::filebox fb( myfm, true );
    auto paths = fb();
    if( paths.empty() )
        return;

    raven::sqlite::cDB DB( paths[0].c_str() );
    int ret = DB.Query( "SELECT * FROM domain;");
    if( ret != 1 )
        return;
    myJobTerm = DB.myResultA[0];
    myResourceTerm = DB.myResultA[1];

    myTypeVector.clear();
    ret = DB.Query("SELECT * FROM job_type;");
    std::vector<std::string> jtv = DB.myResultA;
    int jti = 0;
    for( auto& n : jtv )
    {
        DB.Query("SELECT resource FROM job_resource WHERE type = %d",
                 jti++ );
        cJobType jt( n, DB.myResultA.size() );
        jt.CrewType( DB.myResultA );
        myTypeVector.push_back( jt );
    }

    myJobVector.clear();
    ret = DB.Query("SELECT * FROM job;");
    for( int kj = 0; kj < ret; kj++ )
    {
        myJobVector.push_back( cJob( DB.myResultA[kj*2+1], DB.myResultA[kj*2] ) );
    }

    myResourceVector.clear();
    ret = DB.Query("SELECT * FROM resource;");
    for( int kr = 0; kr < ret; kr++ )
    {
        myResourceVector.push_back( cResource( DB.myResultA[kr*2], DB.myResultA[kr*2+1]));
    }

    Display();
}
void cFleet::NewJob()
{
    std::vector<std::string> type_names;
    for( auto& t : myTypeVector )
    {
        type_names.push_back( t.Name() );
    }
    nana::inputbox::text type("Type", type_names );
    char plate_default[] { 65 + myJobVector.size(), 0 };
    nana::inputbox::text plate("Plate", std::string(plate_default));
    nana::inputbox inbox( myfm, myJobTerm + " Type to add" );
    if( inbox.show(type, plate ) )
    {
        myJobVector.push_back( cJob( type.value(), plate.value() ));
    }
}

void cFleet::Display(  )
{
    fleet_text.select( true );
    fleet_text.del();
    fleet_text.append(std::to_string( myJobVector.size())+ " " + myJobTerm + ":\n", false);
    for( auto& v : myJobVector )
    {
        cJobType vt;
        FindType( v.Type(), vt );
        std::vector<std::string> vn = vt.CrewType();
        fleet_text.append( v.Plate()+" "+v.Type()+" needs ", false );
        for( int kct=0; kct<vt.Crew(); kct++)
            fleet_text.append( vn[kct]+" ",false);
        fleet_text.append("\n",false);
    }
    fleet_text.append("\n" + std::to_string( myResourceVector.size())+ " " + myResourceTerm + ":\n", false);
    for( auto& p : myResourceVector )
    {
        fleet_text.append( p.Text() + "\n", false );
    }

//    if( myAssign.size() )
//    {
//        int ks = 1;
//        for( auto& sa : myAssign )
//        {
//            fleet_text.append("\nSchedule Shift "+std::to_string(ks++)+":\n",
//                              false);
//            for( auto& a : sa )
//            {
//                fleet_text.append(
//                    a.second.Name()+" in "+a.first.Type()+" plate "+a.first.Plate()+" "+"\n",
//                    false);
//            }
//        }
//    }

    std::stringstream l;
    l << "\nShift";
    for( int ks = 1; ks <= myAssign.size(); ks++ )
        l << std::setw(4) <<  std::to_string( ks );
    l << "\n";

    // loop over resources
    for( auto& r : myResourceVector )
    {
        l << std::setw(5) << r.Name() << " ";

        // loop over shifts
        int ks = 0;
        for( auto& sa : myAssign )
        {
            ks++;
            std::string sep = " ";
            if( ! (ks % 3) )
                sep = "|";
            std::cout << ks << " " << sep << "\n";
            bool assigned = false;
            // loop over assignments in shift
            for( auto& a : sa )
            {
                 if( a.second.Name() == r.Name() )
                {
                    l << std::setw(3) << a.first.Plate() << sep;
                    assigned = true;
                    break;
                }
            }
            if( ! assigned )
            {
                l << "..." << sep;
            }
        }
        l << "\n";
    }
    fleet_text.append( l.str(), false );
}

void cResource::Assign( bool f, int shift )
{
    myfAssign = f;
    if( myfAssign )
        myLastShift = shift;
#ifdef INSTRUMENT
    if( myfAssign )
        std::cout << myName << " " << myfAssign << " " << shift << "\n";
#endif
}

bool cResource::IsAvailable( int shift, int rotation ) const
{
    bool ret;
    if( myfAssign )
        ret = false;
    else if( shift - myLastShift < rotation )
        ret = false;
    else
        ret = true;
#ifdef INSTRUMENT
    std::cout << myName << " not available shift " << shift << "\n";
#endif // INSTRUMENT
    return ret;
}
