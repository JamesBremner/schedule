#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "wex.h"

#include "JobResource.h"

#include "raven_sqlite.h"

// #define INSTRUMENT 1

cFleet::cFleet()
    : myJobTerm("Site"), myResourceTerm("Resource"), myShiftRotation(3)

{
}

void cFleet::Write()
{
    // wex::filebox fb(myfm);
    // auto paths = fb.open();
    // if (paths.empty())
    //     return;

    // raven::sqlite::cDB DB(paths.c_str());
    // DB.Query("CREATE TABLE domain ( job, resource );");
    // DB.Query("DELETE FROM domain;");
    // DB.Query("INSERT INTO domain VALUES ( '%s', '%s' );",
    //          myJobTerm.c_str(), myResourceTerm.c_str());
    // DB.Query("CREATE TABLE job_type ( name );");
    // DB.Query("DELETE FROM job_type;");
    // DB.Query("CREATE TABLE job_resource ( type, resource );");
    // DB.Query("DELETE FROM job_resource;");
    // int jti = 0;
    // for (auto &jt : myJobTypeVector)
    // {
    //     DB.Query("INSERT INTO job_type VALUES ( '%s' );",
    //              jt.Name().c_str());
    //     for (auto &jr : jt.CrewType())
    //     {
    //         if (jr.length())
    //             DB.Query("INSERT INTO job_resource VALUES ( %d, '%s' );",
    //                      jti, jr.c_str());
    //     }
    //     jti++;
    // }
    // DB.Query("CREATE TABLE resource_type ( name );");
    // DB.Query("DELETE FROM resource_type;");
    // for (auto &rt : myResourceTypeVector)
    // {
    //     DB.Query("INSERT INTO resource_type VALUES ( '%s' );",
    //              rt.Name().c_str());
    // }
    // DB.Query("CREATE TABLE job ( name, type );");
    // DB.Query("DELETE FROM job;");
    // for (auto &j : myJobVector)
    // {
    //     DB.Query("INSERT INTO job VALUES ( '%s', '%s' );",
    //              j.Name().c_str(), j.Type().c_str());
    // }
    // DB.Query("CREATE TABLE resource ( name, type );");
    // DB.Query("DELETE FROM resource;");
    // for (auto &r : myResourceVector)
    // {
    //     DB.Query("INSERT INTO resource VALUES ( '%s', '%s' );",
    //              r.Name().c_str(), r.Type().c_str());
    // }
}

void cFleet::Read()
{

    // wex::filebox fb(myfm);
    // auto paths = fb.open();
    // if (paths.empty())
    //     return;

    // raven::sqlite::cDB DB(paths.c_str());
    // int ret = DB.Query("SELECT * FROM domain;");
    // if (ret != 1)
    //     return;
    // myJobTerm = DB.myResultA[0];
    // myResourceTerm = DB.myResultA[1];

    // myJobTypeVector.clear();
    // ret = DB.Query("SELECT * FROM job_type;");
    // std::vector<std::string> jtv = DB.myResultA;
    // int jti = 0;
    // for (auto &n : jtv)
    // {
    //     DB.Query("SELECT resource FROM job_resource WHERE type = %d",
    //              jti++);
    //     cJobType jt(n, DB.myResultA.size());
    //     jt.CrewType(DB.myResultA);
    //     myJobTypeVector.push_back(jt);
    // }

    // myJobVector.clear();
    // ret = DB.Query("SELECT * FROM job;");
    // for (int kj = 0; kj < ret; kj++)
    // {
    //     myJobVector.push_back(cJob(DB.myResultA[kj * 2], DB.myResultA[kj * 2 + 1]));
    // }

    // myResourceVector.clear();
    // ret = DB.Query("SELECT * FROM resource;");
    // for (int kr = 0; kr < ret; kr++)
    // {
    //     myResourceVector.push_back(cResource(DB.myResultA[kr * 2], DB.myResultA[kr * 2 + 1]));
    // }

    // myResourceTypeVector.clear();
    // ret = DB.Query("SELECT * FROM resource_type;");
    // for (int kr = 0; kr < ret; kr++)
    // {
    //     myResourceTypeVector.push_back(cResourceType(DB.myResultA[kr]));
    // }

    // Display();
}

void cFleet::JobEditor()
{
    wex::gui &fm = wex::maker::make();
    fm.move(100, 100, 400, 300);
    fm.text("Job Editor");

    // int x_entry = 120;
    // nana::label lbcb( fm, nana::rectangle(20,50,100,20));
    // lbcb.caption("Select Job");
    // nana::combox cb(  fm, nana::rectangle(x_entry,50,100,20));
    // for( auto& j : myJobVector )
    //     cb.push_back( j.Name() );

    wex::button &add_button = wex::maker::make<wex::button>(fm);
    add_button.move(230, 50, 60, 20);
    add_button.text("Add Job");

    // nana::label lbname( fm, nana::rectangle(20,90,100,20));
    // lbname.caption("Name");
    // nana::textbox jname( fm, nana::rectangle(x_entry,90, 100, 20 ));
    // nana::label lbtype( fm, nana::rectangle(20,120,100,20));
    // lbtype.caption("Type");
    // nana::combox jtype( fm, nana::rectangle(x_entry,120, 100, 20 ));
    // for( auto& t : myJobTypeVector )
    //     jtype.push_back( t.Name() );

    wex::button &add_type = wex::maker::make<wex::button>(fm);
    add_type.move(230, 120, 60, 20);
    add_type.text("Add Type");

    // nana::button sv(fm, nana::rectangle(120, 260, 70, 20));
    // sv.caption("SAVE");
    // nana::button done(fm, nana::rectangle(220, 260, 70, 20));
    // done.caption("DONE");

    // add_button.events().click([&]
    //                           {
    //                               cJob j("Job" + std::to_string(myJobVector.size()), "?");
    //                               myJobVector.push_back(j);
    //                               // cb.push_back( j.Name() );
    //                               // cb.option( myJobVector.size()-1);
    //                           });
    // add_type.events().click([this]
    //                         { NewJobType(); });
    // cb.events().selected([&,this](const nana:: arg_combox&arg)
    // {
    //     // clear old selection
    //     jname.select(true);
    //     jname.del();

    //     cJob& j = myJobVector[cb.option()];
    //     jname.append( j.Name(), false);
    //     cJobType t;
    //     int i;
    //     if( FindType( j.Type(), t, i ))
    //     {
    //         jtype.option( i );
    //     }
    // });
    // sv.events().click([&,this]
    // {
    //     int d1 = cb.option();
    //     int d2 = myJobVector.size();
    //     myJobVector[cb.option()].Name( jname.text() );
    //     myJobVector[cb.option()].Type( jtype.text( jtype.option() ) );
    // });
    // done.events().click([&fm]
    // {
    //     nana::API::close_window( fm ) ;
    // });

    fm.show();
}

void cFleet::ConstraintEditor(wex::gui &fm)
{
    // nana::inputbox::integer rotation("Minimum shifts between assignments", 2, 0, 21, 1 );
    // nana::inputbox inbox(fm,"Input","Constraint Editor");
    // if (inbox.show_modal( rotation ) )
    // {
    //     Rotation( rotation.value() );
    // }
}

void cFleet::ResourceEditor()
{
    // nana::form fm( nana::rectangle( 100,100, 300, 300 ));
    // fm.caption("Resource Editor");

    // int x_entry = 120;
    // nana::label lbcb( fm, nana::rectangle(20,50,100,20));
    // lbcb.caption("Select Resource");
    // nana::combox cb(  fm, nana::rectangle(x_entry,50,100,20));
    // for( auto& j : myResourceVector )
    //     cb.push_back( j.Name() );
    // nana::button add_button( fm, nana::rectangle(230, 50, 60, 20));
    // add_button.caption("Add Resource");

    // nana::label lbname( fm, nana::rectangle(20,90,100,20));
    // lbname.caption("Name");
    // nana::textbox jname( fm, nana::rectangle(x_entry,90, 100, 20 ));
    // nana::label lbtype( fm, nana::rectangle(20,120,100,20));
    // lbtype.caption("Type");
    // nana::combox jtype( fm, nana::rectangle(x_entry,120, 100, 20 ));
    // for( auto& t : myResourceTypeVector )
    //     jtype.push_back( t.Name() );
    // nana::button add_type(fm, nana::rectangle(230, 120, 60, 20));
    // add_type.caption("Add Type");

    // nana::button sv(fm, nana::rectangle(120, 260, 70, 20));
    // sv.caption("SAVE");
    // nana::button done(fm, nana::rectangle(220, 260, 70, 20));
    // done.caption("DONE");

    // add_button.events().click([&]
    // {
    //     cResource j( "Res"+std::to_string( myResourceVector.size()), "?" );
    //     myResourceVector.push_back( j );
    //     cb.push_back( j.Name() );
    //     cb.option( myResourceVector.size()-1);
    // });
    // cb.events().selected([&,this](const nana:: arg_combox&arg)
    // {
    //     jname.select(true);
    //     jname.del();
    //     cResource& R = myResourceVector[cb.option()];
    //     jname.append( R.Name(), false);

    //     cResourceType r;
    //     int i;
    //     if( FindResourceType( R.Type(), r, i ))
    //     {
    //         jtype.option( i );
    //     }
    // });
    // sv.events().click([&,this]
    // {
    //     int d1 = cb.option();
    //     int d2 = myResourceVector.size();
    //     myResourceVector[cb.option()].Name( jname.text() );
    //     myResourceVector[cb.option()].Type( jtype.text( jtype.option() ) );
    // });
    // add_type.events().click([this,&fm]
    // {
    //     NewResourceType( fm );
    // });
    // done.events().click([&fm]
    // {
    //     nana::API::close_window( fm ) ;
    // });

    // fm.modality();
}

void cFleet::NewJob()
{
    //    std::vector<std::string> type_names;
    //    for( auto& t : myJobTypeVector )
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
    // std::vector<std::string> type_names;
    // for( auto& t : myResourceTypeVector )
    // {
    //     type_names.push_back( t.Name() );
    // }
    // nana::inputbox::text name("Name", "");
    // nana::inputbox::text type("Type", type_names );
    // nana::inputbox inbox( myfm, "New " + myResourceTerm );
    // if( inbox.show( name, type ) )
    // {
    //     if( FindPerson( name.value() ))
    //     {
    //         nana::msgbox msg("Already have person with this name");
    //         msg.show();
    //     }
    //     else
    //     {
    //         myResourceVector.push_back( cResource( name.value(), type.value() ));
    //     }
    // }
}

bool cFleet::FindType(
    const std::string &type_name,
    cJobType &type,
    int &index)
{
#ifdef INSTRUMENT
    std::cout << type_name << " in ";
#endif // INSTRUMENT

    index = 0;
    for (auto &t : myJobTypeVector)
    {
#ifdef INSTRUMENT
        std::cout << t.Name() << " ";
#endif // INSTRUMENT
        if (t.Name() == type_name)
        {
            type = t;
            return true;
        }
        index++;
    }
#ifdef INSTRUMENT
    std::cout << "not found\n";
#endif // INSTRUMENT
    return false;
}

void cFleet::Test()
{
    myJobTypeVector.push_back(cJobType("SiteA", 2));
    myJobTypeVector.back().CrewType({"canUseDrill", "canUseDigger"});
    myJobTypeVector.push_back(cJobType("SiteB", 1));
    myJobTypeVector.back().CrewType({"canUseDrill"});
    myJobTypeVector.push_back(cJobType("SiteC", 1));
    myJobTypeVector.back().CrewType({"canUseDigger"});
    myJobVector.push_back(cJob("A", "SiteA"));
    myJobVector.push_back(cJob("B", "SiteB"));
    myJobVector.push_back(cJob("C", "SiteC"));
    // myResourceVector.push_back(cResource("Alice", "canUseDrill"));
    // myResourceVector.push_back(cResource("Bob", "canUseDigger"));
    // myResourceVector.push_back(cResource("Chad", "canUseDrill"));
    // myResourceVector.push_back(cResource("Derek", "canUseDigger"));
}

static std::vector<std::string> tokenize(const std::string &line)
{
    std::vector<std::string> ret;
    std::stringstream sst(line);
    std::string a;
    while (getline(sst, a, ' '))
        ret.push_back(a);
    return ret;
}

void cFleet::ReadSimpleText(const std::string &path)
{

    myResourceVector.clear();
    myJobTypeVector.clear();
    myJobVector.clear();

    std::ifstream ifs(path);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open file");

    std::string line;
    while (getline(ifs, line))
    {
        auto tokens = tokenize(line);
        if (tokens[0] == "r")
        {
            cResource res(tokens[1]);
            for (int k = 2; k < tokens.size(); k++)
                res.Ability(tokens[k]);
            myResourceVector.push_back(res);
        }
        else if (tokens[0] == "j")
        {
            std::vector<std::string> vneeds(tokens.begin() + 3, tokens.end());
            myJobTypeVector.push_back(cJobType(tokens[1], vneeds.size()));
            myJobTypeVector.back().CrewType(vneeds);
            myJobTypeVector.back().GeneralCount(atoi(tokens[2].c_str()));
            myJobVector.push_back(cJob(tokens[1], tokens[1]));
        }
    }
}

bool cFleet::Schedule(int shifts)
{
    // clear all previous assignments
    myAssign.clear();

    // loop over shifts
    for (int kshift = 0; kshift < shifts; kshift++)
    {
        // clear previous shift's assignments
        for (auto &p : myResourceVector)
        {
            p.Assign(false);
        }

        // the assignments for this shift
        std::vector<std::pair<cJob, cResource>> shift_assignments;

        // loop over jobs
        for (auto &v : myJobVector)
        {
            std::cout << "assigning resources to job " << v.Name() << "\n";
            cJobType vt;
            int i;
            if (!FindType(v.Type(), vt, i))
                throw std::runtime_error("Job type error");

            // loop over resources needed by type
            for (int kp = 0; kp < vt.Crew(); kp++)
            {
                bool success = false;

                // loop over people
                for (auto &p : myResourceVector)
                {
                    if (!p.IsAvailable(kshift, myShiftRotation))
                        continue;
                    if (!p.hasAbility(vt.CrewType()[kp]))
                        continue;

                    // assign resource to job
                    std::cout << "assigning " << p.Name() << " to " << v.Name() << "\n";
                    shift_assignments.push_back(std::pair<cJob, cResource>(v, p));
                    p.Assign(true, kshift);
                    success = true;
                    break;
                }
                if (!success)
                {
                    wex::msgbox msg("Insufficient resources");
                    myAssign.clear();
                    return false;
                }
            }
        }

        // assign general workers
        for (auto &v : myJobVector)
        {
            std::cout << "assigning general to job " << v.Name() << "\n";
            cJobType jt;
            int i;
            if (!FindType(v.Type(), jt, i))
                throw std::runtime_error("Job type error");

            // count specialized workers assigned
            int count = 0;
            for (auto &p : shift_assignments)
            {
                if (p.first.Name() == v.Name())
                    count++;
            }
            int generalNeed = jt.GeneralCount() - count;
            if (!generalNeed)
                continue;
            bool success = false;
            for (int k = 0; k < generalNeed; k++)
            {
                for (auto &p : myResourceVector)
                {
                    if (!p.IsAvailable(kshift, myShiftRotation))
                        continue;

                    // assign resource to job
                    std::cout << "assigning " << p.Name() << " to " << v.Name() << "\n";
                    shift_assignments.push_back(std::pair<cJob, cResource>(v, p));
                    p.Assign(true, kshift);
                    success = true;
                    break;
                }
            }
            if (!success)
            {
                wex::msgbox msg("Insufficient resources");
                myAssign.clear();
                return false;
            }
        }

        myAssign.push_back(shift_assignments);

        std::random_shuffle(myResourceVector.begin(), myResourceVector.end());
    }
    return true;
}

std::string cFleet::Display()
{
    std::stringstream ss;

    //     // display jobs
    ss << std::to_string(myJobVector.size()) + " " + myJobTerm + ":\n";
    for (auto &v : myJobVector)
    {
        ss << v.Name();
        cJobType vt;
        int i;
        if (!FindType(v.Type(), vt, i))
            continue;
        std::vector<std::string> vn = vt.CrewType();
        ss << " needs ";
        for (int kct = 0; kct < vt.Crew(); kct++)
            ss << vn[kct] << " ";
        ss << " and " << vt.GeneralCount() - vn.size() << " general workers\n";
    }

    //     // display resources
    ss << "\n"
       << std::to_string(myResourceVector.size()) << " " << myResourceTerm << ":\n";
    for (auto &p : myResourceVector)
    {
       p.Display(ss);
    }

    ss << "\nShift";
    for (int ks = 1; ks <= (int)myAssign.size(); ks++)
        ss << std::setw(8) << std::to_string(ks);
    ss << "\n";

    // loop over resources
    for (auto &r : myResourceVector)
    {
        ss << std::setw(7) << r.Name() << " ";

        // loop over shifts
        int ks = 0;
        for (auto &sa : myAssign)
        {
            ks++;
            std::string sep = " ";
            if (!(ks % 3))
                sep = "|";
            bool assigned = false;
            // loop over assignments in shift
            for (auto &a : sa)
            {
                if (a.second.Name() == r.Name())
                {
                    ss << std::setw(6) << a.first.Name() << sep;
                    assigned = true;
                    break;
                }
            }
            if (!assigned)
            {
                ss << "..." << sep;
            }
        }
        ss << "\n";
    }
    return ss.str();
}

bool cResource::hasAbility(const std::string &a)
{
    return (std::find(
                myAbility.begin(), myAbility.end(), a) != myAbility.end());
}
void cResource::Display(std::stringstream &ss)
{
    ss << myName << " can " << " ";
    for( auto& a : myAbility )
        ss << a << " ";
    ss << "\n";
}

void cResource::Assign(bool f, int shift)
{
    myfAssign = f;
    if (myfAssign)
        myLastShift = shift;
#ifdef INSTRUMENT
    if (myfAssign)
        std::cout << myName << " " << myfAssign << " " << shift << "\n";
#endif
}

bool cResource::IsAvailable(int shift, int rotation) const
{
    bool ret;
    if (myfAssign)
        ret = false;
    else if (shift - myLastShift < rotation)
        ret = false;
    else
        ret = true;
#ifdef INSTRUMENT
    std::cout << myName << " not available shift " << shift << "\n";
#endif // INSTRUMENT
    return ret;
}
