
#include <iostream>
#include "wex.h"

#include "JobResource.h"

int main()
{

    cFleet theFleet;

    // construct application form
    wex::gui &fm = wex::maker::make();
    fm.move(100, 100, 750, 800);
    fm.text("Scheduler");
    wex::label &fleet_text = wex::maker::make<wex::label>(fm);
    fleet_text.move(10, 10, 750, 800);
    fleet_text.bgcolor(0xFFFFFF);
    fleet_text.fontName("Courier");
    fleet_text.text("");

    // theFleet.Test();

    wex::menubar mb(fm);
    wex::menu mf(fm);
    mf.append("Read",
              [&](const std::string &title)
              {
                  wex::filebox fb(fm);
                  auto paths = fb.open();
                  if (paths.empty())
                      return;
                  theFleet.Read(paths);
              });
    mf.append("Write", [&](const std::string &title)
              { theFleet.Write(); });
    mf.append("Schedule",
              [&](const std::string &title)
              {
                  wex::filebox fb(fm);
                  auto paths = fb.open();
                  if (paths.empty())
                      return;
                  theFleet.Read(paths);

                  theFleet.Schedule();

                  fleet_text.text(theFleet.Display());
                  fleet_text.update();
              });
    mb.append("File", mf);

    // wex::menu md(fm);
    // md.append("Terms", [&](const std::string &title)
    //           { theFleet.Read(); });
    // mb.append("Domain", md);

    // // md.append("Terms", [fm, &theFleet](nana::menu::item_proxy &ip)
    // //           {
    // //     nana::inputbox::text job("Job term ( e.g. vehicle, machine, order, ...", "");
    // //     nana::inputbox::text resource("Resource term ( e.g. person, ...", "");
    // //     nana::inputbox inbox(fm,"Problem Domain Terms\n"
    // //                          "The resources will be assigned to the jobs as needed.");
    // //     if (inbox.show_modal(job, resource))
    // //     {
    // //         theFleet.JobTerm( job.value() );
    // //         theFleet.ResourceTerm( resource.value() );
    // //     } });
    // //    md.append("Rotation",[&fm, &theFleet](nana::menu::item_proxy& ip)
    // //    {
    // //        nana::inputbox::integer rotation("Minimum shifts between assignments", 2, 0, 21, 1 );
    // //        nana::inputbox inbox(fm,"Input");
    // //        if (inbox.show_modal( rotation ) )
    // //        {
    // //            theFleet.Rotation( rotation.value() );
    // //        }
    // //    });

    // wex::button &job_button = wex::maker::make<wex::button>(fm);
    // job_button.move(10, 50, 100, 20);
    // job_button.text("JOBS");
    // job_button.events().click([&]
    //                           {
    //     theFleet.JobEditor();
    //     theFleet.Display(); });

    // wex::button &resource_button = wex::maker::make<wex::button>(fm);
    // resource_button.move(110, 50, 100, 20);
    // resource_button.text("RESOURCES");

    // // resource_button.events().click([&]
    // //                                {
    // //     theFleet.ResourceEditor();
    // //     theFleet.Display(); });

    // wex::button &constraint_button = wex::maker::make<wex::button>(fm);
    // constraint_button.move(210, 50, 100, 20);
    // constraint_button.text("CONSTRAINTS");

    // // constraint_button.events().click([&]
    // //                                  { theFleet.ConstraintEditor(fm); });

    // wex::button &schedule_button = wex::maker::make<wex::button>(fm);
    // schedule_button.move(350, 50, 100, 20);
    // schedule_button.text("SCHEDULE");
    // schedule_button.events().click(
    //     [&]
    //     {
    //         theFleet.Schedule(1);

    //         fleet_text.text(theFleet.Display());
    //         fleet_text.update();
    //     });

    // show & run
    fm.show();
    fm.run();
}
