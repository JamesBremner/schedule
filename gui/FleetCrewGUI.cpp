
#include <iostream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include  <nana/gui/msgbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <nana/gui/widgets/menubar.hpp>

#include "JobResource.h"

int main()
{
    // construct application form
    nana::form fm( nana::rectangle( 100,100, 300, 450 ));
    fm.caption("Scheduler");

    cFleet theFleet( fm );

    //theFleet.Test();

    nana::menubar mb( fm );
    nana::menu& mf = mb.push_back("File");
    nana::menu& md = mb.push_back("Domain");
    nana::menu& ms = mb.push_back("Specification");

    mf.append("Read",[&theFleet, &ms](nana::menu::item_proxy& ip)
    {
        theFleet.Read();
        ms.text(0,"Add " + theFleet.JobTerm() + " type");
        ms.text(1,"Add " + theFleet.JobTerm());
        ms.text(2,"Add " + theFleet.ResourceTerm() + " type");
        ms.text(3,"Add " + theFleet.ResourceTerm());
    });
    mf.append("Write",[&theFleet](nana::menu::item_proxy& ip)
    {
        theFleet.Write();
    });
    md.append("Problem Domain Terms", [fm, &theFleet, &ms](nana::menu::item_proxy& ip)
    {
        nana::inputbox::text job("Job term ( e.g. vehicle, machine, order, ...", "");
        nana::inputbox::text resource("Resource term ( e.g. person, ...", "");
        nana::inputbox inbox(fm,"Problem Domain Terms\n"
                             "The resources will be assigned to the jobs as needed.");
        if (inbox.show_modal(job, resource))
        {
            theFleet.JobTerm( job.value() );
            theFleet.ResourceTerm( resource.value() );
            ms.text(0,"Add " + theFleet.JobTerm() + " type");
            ms.text(1,"Add " + theFleet.JobTerm());
            ms.text(2,"Add " + theFleet.ResourceTerm() + " type");
            ms.text(3,"Add " + theFleet.ResourceTerm());
        }
    });

    ms.append("Add Job Type",[fm, &theFleet](nana::menu::item_proxy& ip)
    {
        theFleet.NewJobType( fm );
    });
    ms.append("Add Job",[ &theFleet](nana::menu::item_proxy& ip)
    {
        theFleet.NewJob();
        theFleet.Display();
    });
    ms.append("Add Resource Type",[&fm, &theFleet](nana::menu::item_proxy& ip)
    {
        theFleet.NewResourceType( fm );
    });
    ms.append("Add Resource",[&fm, &theFleet](nana::menu::item_proxy& ip)
    {
        theFleet.NewResource( fm );
        theFleet.Display();
    });

    nana::button schedule_button( fm, nana::rectangle( 10, 50, 100, 20 ) );
    schedule_button.caption("SCHEDULE");
    schedule_button.events().click([&]
    {
        theFleet.Schedule( 21 );
        theFleet.Display( );
    });

    // show & run
    fm.show();
    nana::exec();

}
