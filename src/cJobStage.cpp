#include <vector>
#include <iostream>

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
    void run( int& t)
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

int main()
{
    cJobStages job1;
    cJobStages job2;
    job1.add(cStage(1, 2));
    job2.add(cStage(1, 3));

    int t = 0;
    job1.nextStage().run( t );
    job2.nextStage().run(t);

}