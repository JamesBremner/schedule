
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
    nana::form fm( nana::rectangle( 100,100, 500, 450 ));
    fm.caption("Scheduler");

    cFleet theFleet( fm );

    //theFleet.Test();

    nana::menubar mb( fm );
    nana::menu& mf = mb.push_back("File");
    nana::menu& md = mb.push_back("Domain");

    mf.append("Read",[&theFleet](nana::menu::item_proxy& ip)
    {
        theFleet.Read();
    });
    mf.append("Write",[&theFleet](nana::menu::item_proxy& ip)
    {
        theFleet.Write();
    });
    md.append("Terms", [fm, &theFleet](nana::menu::item_proxy& ip)
    {
        nana::inputbox::text job("Job term ( e.g. vehicle, machine, order, ...", "");
        nana::inputbox::text resource("Resource term ( e.g. person, ...", "");
        nana::inputbox inbox(fm,"Problem Domain Terms\n"
                             "The resources will be assigned to the jobs as needed.");
        if (inbox.show_modal(job, resource))
        {
            theFleet.JobTerm( job.value() );
            theFleet.ResourceTerm( resource.value() );
        }
    });
    md.append("Rotation",[&fm, &theFleet](nana::menu::item_proxy& ip)
    {
        nana::inputbox::integer rotation("Minimum shifts between assignments", 2, 0, 21, 1 );
        nana::inputbox inbox(fm,"Input");
        if (inbox.show_modal( rotation ) )
        {
            theFleet.Rotation( rotation.value() );
        }
    });

    nana::button job_button( fm, nana::rectangle(10, 50, 100, 20));
    job_button.caption("JOBS");
    job_button.events().click([&]
    {
        theFleet.JobEditor();
        theFleet.Display();
    });
    nana::button resource_button( fm, nana::rectangle(110, 50, 100, 20));
    resource_button.caption("RESOURCES");
    resource_button.events().click([&]
    {
        theFleet.ResourceEditor();
        theFleet.Display();
    });
    nana::button constraint_button( fm, nana::rectangle(210, 50, 100, 20));
    constraint_button.caption("CONSTRAINTS");
    constraint_button.events().click([&]
    {
        theFleet.ConstraintEditor();
    });
    nana::button schedule_button( fm, nana::rectangle( 350, 50, 100, 20 ) );
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
