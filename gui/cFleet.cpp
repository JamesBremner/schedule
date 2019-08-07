#include <iostream>
#include <sstream>
#include <iomanip>

#include <nana/gui.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/button.hpp>
#include  <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/label.hpp>

#include "JobResource.h"

#include "raven_sqlite.h"

#define INSTRUMENT 1

cFleet::cFleet( nana::form& fm )
    : myJobTerm( "Job" )
    , myResourceTerm( "Resource" )
    , myfm( fm )
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
                 j.Name().c_str(), j.Type().c_str() );
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
        myJobVector.push_back( cJob( DB.myResultA[kj*2], DB.myResultA[kj*2+1] ) );
    }

    myResourceVector.clear();
    ret = DB.Query("SELECT * FROM resource;");
    for( int kr = 0; kr < ret; kr++ )
    {
        myResourceVector.push_back( cResource( DB.myResultA[kr*2], DB.myResultA[kr*2+1]));
    }

    Display();
}

void cFleet::JobEditor()
{
    nana::form fm( nana::rectangle( 100,100, 300, 300 ));
    fm.caption("Job Editor");

    int x_entry = 120;
    nana::label lbcb( fm, nana::rectangle(20,50,100,20));
    lbcb.caption("Select Job");
    nana::combox cb(  fm, nana::rectangle(x_entry,50,100,20));
    for( auto& j : myJobVector )
        cb.push_back( j.Name() );
    nana::button add_button( fm, nana::rectangle(230, 50, 60, 20));
    add_button.caption("Add Job");

    nana::label lbname( fm, nana::rectangle(20,90,100,20));
    lbname.caption("Name");
    nana::textbox jname( fm, nana::rectangle(x_entry,90, 100, 20 ));
    nana::label lbtype( fm, nana::rectangle(20,120,100,20));
    lbtype.caption("Type");
    nana::combox jtype( fm, nana::rectangle(x_entry,120, 100, 20 ));
    for( auto& t : myTypeVector )
        jtype.push_back( t.Name() );
    nana::button add_type(fm, nana::rectangle(230, 120, 60, 20));
    add_type.caption("Add Type");

    nana::button sv(fm, nana::rectangle(120, 260, 70, 20));
    sv.caption("SAVE");
    nana::button done(fm, nana::rectangle(220, 260, 70, 20));
    done.caption("DONE");

    add_button.events().click([&]
    {
        cJob j("Job"+std::to_string( myJobVector.size() ), "?" );
        myJobVector.push_back( j );
        cb.push_back( j.Name() );
        cb.option( myJobVector.size()-1);
    });
    add_type.events().click([this]
    {
        NewJobType();
    });
    cb.events().selected([&,this](const nana:: arg_combox&arg)
    {
        // clear old selection
        jname.select(true);
        jname.del();

        cJob& j = myJobVector[cb.option()];
        jname.append( j.Name(), false);
        cJobType t;
        int i;
        if( FindType( j.Type(), t, i ))
        {
            jtype.option( i );
        }
    });
    sv.events().click([&,this]
    {
        int d1 = cb.option();
        int d2 = myJobVector.size();
        myJobVector[cb.option()].Name( jname.text() );
        myJobVector[cb.option()].Type( jtype.text( jtype.option() ) );
    });
    done.events().click([&fm]
    {
        nana::API::close_window( fm ) ;
    });


    fm.modality();
}

void cFleet::ResourceEditor()
{
    nana::form fm( nana::rectangle( 100,100, 300, 300 ));
    fm.caption("Resource Editor");

    int x_entry = 120;
    nana::label lbcb( fm, nana::rectangle(20,50,100,20));
    lbcb.caption("Select Resource");
    nana::combox cb(  fm, nana::rectangle(x_entry,50,100,20));
    for( auto& j : myResourceVector )
        cb.push_back( j.Name() );

    nana::label lbname( fm, nana::rectangle(20,90,100,20));
    lbname.caption("Name");
    nana::textbox jname( fm, nana::rectangle(x_entry,90, 100, 20 ));
    nana::label lbtype( fm, nana::rectangle(20,120,100,20));
    lbtype.caption("Type");
    nana::textbox jtype( fm, nana::rectangle(x_entry,120, 100, 20 ));

    nana::button add_button( fm, nana::rectangle(20, 260, 70, 20));
    add_button.caption("ADD");
    nana::button sv(fm, nana::rectangle(120, 260, 70, 20));
    sv.caption("SAVE");
    nana::button done(fm, nana::rectangle(220, 260, 70, 20));
    done.caption("DONE");

    add_button.events().click([&]
    {
        cResource j( "Res"+std::to_string( myResourceVector.size()), "?" );
        myResourceVector.push_back( j );
        cb.push_back( j.Name() );
        cb.option( myResourceVector.size()-1);
    });
    cb.events().selected([&,this](const nana:: arg_combox&arg)
    {
        jname.select(true);
        jname.del();
        jname.append( myResourceVector[cb.option()].Name(), false);
        jtype.select(true);
        jtype.del();
        jtype.append( myResourceVector[cb.option()].Type(), false);
    });
    sv.events().click([&,this]
    {
        int d1 = cb.option();
        int d2 = myResourceVector.size();
        myResourceVector[cb.option()].Name( jname.text() );
        myResourceVector[cb.option()].Type( jtype.text() );
    });
    done.events().click([&fm]
    {
        nana::API::close_window( fm ) ;
    });


    fm.modality();
}


void cFleet::NewJobType()
{
#ifdef INSTRUMENT
    std::cout << "NewJobType\nJob Types\n";
    for( auto& j : myTypeVector )
        std::cout << j.Name() << " ";
    std::cout << "\nResource Types\n";
    for( auto& r : myResourceTypeVector )
        std::cout << r.Type() << " ";
    std::cout << "\n";
    Display();
#endif // INSTRUMENT


    std::vector<std::string> type_names;
    for( auto& t : myResourceTypeVector )
    {
        type_names.push_back( t.Type() );
    }
    nana::inputbox::text name("Name", "");
    nana::inputbox::integer crew("Number of crew",3,1,10,1);
    nana::inputbox::text crewType1("Crew Type 1", type_names );
    nana::inputbox::text crewType2("Crew Type 2", type_names );
    nana::inputbox::text crewType3("Crew Type 3", type_names );
    nana::inputbox::text crewType4("Crew Type 4", type_names );
    nana::inputbox::text crewType5("Crew Type 5", type_names );
    nana::inputbox inbox( myfm, "New " + JobTerm() + " Type" );

    if( inbox.show( name, crew
                    , crewType1
                    , crewType2
                    , crewType3
                    , crewType4
                    , crewType5 ) )
    {
        cJobType vt;
        int i;
        if( FindType( name.value(), vt, i ))
        {
            nana::msgbox msg("Already have this vehicle type");
            msg.show();
        }
        else
        {
            myTypeVector.push_back( cJobType(
                                        name.value(),
                                        crew.value() ) );
            std::vector< std::string > types;
            types.push_back( crewType1.value() );
            types.push_back( crewType2.value() );
            types.push_back( crewType3.value() );
            types.push_back( crewType4.value() );
            types.push_back( crewType5.value() );
            myTypeVector.back().CrewType( types );
        }
    }
    Display();
}


void cFleet::NewJob()
{
//    std::vector<std::string> type_names;
//    for( auto& t : myTypeVector )
//    {
//        type_names.push_back( t.Name() );
//    }
//    nana::inputbox::text type("Type", type_names );
//    char plate_default[] { 65 + myJobVector.size(), 0 };
//    nana::inputbox::text plate("Plate", std::string(plate_default));
//    nana::inputbox inbox( myfm, myJobTerm + " Type to add" );
//    if( inbox.show(type, plate ) )
//    {
//        myJobVector.push_back( cJob( type.value(), plate.value() ));
//    }
}

void cFleet::NewResource()
{
    std::vector<std::string> type_names;
    for( auto& t : myResourceTypeVector )
    {
        type_names.push_back( t.Type() );
    }
    nana::inputbox::text name("Name", "");
    nana::inputbox::text type("Type", type_names );
    nana::inputbox inbox( myfm, "New " + myResourceTerm );
    if( inbox.show( name, type ) )
    {
        if( FindPerson( name.value() ))
        {
            nana::msgbox msg("Already have person with this name");
            msg.show();
        }
        else
        {
            myResourceVector.push_back( cResource( name.value(), type.value() ));
        }
    }

}

bool cFleet::FindType(
                      const std::string& type_name,
                       cJobType& type,
                       int& index )
{
#ifdef INSTRUMENT
    std::cout << type_name << " in ";
#endif // INSTRUMENT

    index = 0;
    for( auto& t : myTypeVector )
    {
#ifdef INSTRUMENT
        std::cout << t.Name() << " ";
#endif // INSTRUMENT
        if( t.Name() == type_name )
        {
            type = t;
            return true;
        }
        index++;
    }
#ifdef INSTRUMENT
    std::cout  << "not found\n";
#endif // INSTRUMENT
    return false;
}

void cFleet::Display(  )
{
    // clear previous display
    fleet_text.select( true );
    fleet_text.del();

    // display jobs
    fleet_text.append(std::to_string( myJobVector.size())+ " " + myJobTerm + ":\n", false);
    for( auto& v : myJobVector )
    {
        fleet_text.append( v.Name()+" "+v.Type(), false );
        cJobType vt;
        int i;
        if( ! FindType( v.Type(), vt, i ) )
            continue;
        std::vector<std::string> vn = vt.CrewType();
        fleet_text.append( " needs ", false );
        for( int kct=0; kct<vt.Crew(); kct++)
            fleet_text.append( vn[kct]+" ",false);
        fleet_text.append("\n",false);
    }
    fleet_text.append("\n" + std::to_string( myResourceVector.size())+ " " + myResourceTerm + ":\n", false);
    for( auto& p : myResourceVector )
    {
        fleet_text.append( p.Text() + "\n", false );
    }

    std::stringstream l;
    l << "\nShift";
    for( int ks = 1; ks <= (int)myAssign.size(); ks++ )
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
            bool assigned = false;
            // loop over assignments in shift
            for( auto& a : sa )
            {
                if( a.second.Name() == r.Name() )
                {
                    l << std::setw(3) << a.first.Name() << sep;
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
