
#include <iostream>
#include <fstream>
#include "wex.h"

#include "cSchedule.h"
#include "cShop.h"

static std::vector<std::string> tokenize(const std::string &line)
{
    std::vector<std::string> ret;
    std::stringstream sst(line);
    std::string a;
    while (getline(sst, a, ' '))
        ret.push_back(a);
    return ret;
}

void read(
    raven::sch::cSchedule &S,
    const std::string &path)
{
    std::ifstream ifs(path);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open file");

    std::string line;
    while (getline(ifs, line))
    {
        auto tokens = tokenize(line);

        if (tokens[0] == "j") {
         
            auto job = raven::sch::cJob(
                tokens[1],
                raven::sch::cJob::eType::anyone);

            for( int k = 3; k < tokens.size(); k++ ) 
                job.Add( tokens[k], 1 );

            S.Add( job );
        }
    }
}

void manufacture(
    const std::string& paths,
    wex::label &lbDisplay)
{
    try
    {
        raven::sch::cSchedule theSchedule;
        read(theSchedule, paths);

        raven::sch::cShop Shop(theSchedule);
        Shop.Manufacture(theSchedule);

        lbDisplay.text(
            theSchedule.display() + Shop.display());
        lbDisplay.update();
    }
    catch (std::runtime_error &e)
    {
        lbDisplay.text(e.what());
        lbDisplay.update();
    }
}

int main()
{
    // construct application form
    wex::gui &fm = wex::maker::make();
    fm.move(100, 100, 750, 800);
    fm.text("Manufacture");
    wex::label &lbDisplay = wex::maker::make<wex::label>(fm);
    lbDisplay.move(10, 10, 750, 800);
    lbDisplay.bgcolor(0xFFFFFF);
    lbDisplay.fontName("Courier");
    lbDisplay.text("");

    wex::menubar mb(fm);
    wex::menu mf(fm);
    mf.append("Calculate",
              [&](const std::string &title)
              {
                  wex::filebox fb(fm);
                  auto paths = fb.open();
                  if (paths.empty())
                      return;
                  manufacture(
                      paths,
                      lbDisplay);
              });
    mb.append("File", mf);

    // show & run
    fm.show();
    fm.run();
}
