#include <vector>
#include <iostream>
#include <fstream>

/*
Problem:
There are n machines and m jobs.
Each job consists of si stages.
A stage is characterized by a pair of (k, t) numbers,
    where k is the machine number,
    t is the duration of the stage.
For each job, the order of stages is strictly specified.
At any given time, any job can only be performed on one machine,
and any machine can only perform one job.
It is necessary to make a schedule so that all work is done in a minimum of time.

*/
class cStage
{
public:
    cStage(int machine, int duration)
        : myMachine(machine),
          myDuration(duration),
          myfDone(false)
    {
    }
    bool isDone() const
    {
        return myfDone;
    }
    void run(int &t)
    {
        std::cout << "Machine " << myMachine
                  << " starts at " << t;
        t += myDuration;
        myfDone = true;
        std::cout << " completes at " << t << "\n";
    }

private:
    int myMachine;
    int myDuration;
    bool myfDone;
};
class cJobStages
{
public:
    void clear()
    {
        myStage.clear();
    }
    void add(const cStage &stage)
    {
        myStage.push_back(stage);
    }
    cStage &nextStage()
    {
        for (auto &stage : myStage)
        {
            if (!stage.isDone())
                return stage;
        }
        static cStage alldone(-1, -1);
        return alldone;
    }

private:
    std::vector<cStage> myStage;
};

class cSchedule
{
public:
    void clear()
    {
        myJobs.clear();
    }
    void add(const cJobStages &job)
    {
        myJobs.push_back(job);
    }
    int jobCount() const
    {
        return myJobs.size();
    }
    void run();

private:
    std::vector<cJobStages> myJobs;
};

void cSchedule::run()
{
    int t = 0;
    for (auto &j : myJobs)
        j.nextStage().run(t);
}
/// @brief read problem specification from file
/// @param sked schedule to be populated
/// @param fname path to specification file
/// Fole format documentation https://github.com/JamesBremner/schedule/wiki/Stage-Jobs

void readfile(cSchedule &sked, const std::string &fname)
{
    sked.clear();
    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open input file");
    std::string linetype;
    int machine, duration;
    cJobStages job;

    bool firstjob = true;
    while (ifs.good())
    {
        ifs >> linetype;
        switch (linetype[0])
        {
        case 'j':
            if( firstjob ) {
                firstjob = false;
                break;
            }
            sked.add(job);
            job.clear();
            break;
        case 's':
            ifs >> machine >> duration;
            cStage stage(machine, duration);
            job.add(stage);
            break;
        }
    }
    sked.add(job);
}

int main()
{
    cSchedule sked;
    cJobStages job1;
    cJobStages job2;
    job1.add(cStage(1, 2));
    job2.add(cStage(1, 3));
    sked.add(job1);
    sked.add(job2);

    sked.run();

    readfile( sked, "../problems/stage.txt");
    sked.run();
}