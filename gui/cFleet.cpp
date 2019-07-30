#include <iostream>

#include <nana/gui.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/filebox.hpp>
#include "JobResource.h"

#include "raven_sqlite.h"

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
        myResourceVector.push_back( cResource( DB.myResultA[kr*2+1], DB.myResultA[kr*2]));
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
