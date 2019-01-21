# Schedule
Schedule jobs on machines

The program inputs a json formatted schedule and outputs the schedule with the calculated start time of each job step added.

The following cases are handled:

1. Jobs with multiple steps, each on a specified machine taking a specified time, which must be completed in sequence.
1. Jobs with a single step that can be completed on different machines at different cost.

The examples folder contains a strightforward adaptation of the scheduling engine to a real world problem.  Crop duster inputs a list of pilots, and a list of farms with seeding dates.  Each farm must be dusted 30, 60 and 80 days after seeding.  The applications schedules the pilots to dust the farms when required.  Here is the outpurt from a small run:

<pre>
F add farm
P add pilot
C calculate
>p
Enter pilot's name
>P
Pilots:  P,
F add farm
P add pilot
C calculate
>p
Enter pilot's name
>Q
Pilots:  P,  Q,
F add farm
P add pilot
C calculate
>f
Enter farm's name
>F
Enter seeding date: YYYY MM DD
>2017 3 22
requesting dusting 30, 60 and 80 days after seeding
Farm F dates: 2017 04 21, 2017 05 21, 2017 06 10,
Pilots:  P,  Q,
F add farm
P add pilot
C calculate
>f
Enter farm's name
>G
Enter seeding date: YYYY MM DD
>2017 3 23
requesting dusting 30, 60 and 80 days after seeding
Farm F dates: 2017 04 21, 2017 05 21, 2017 06 10,
Farm G dates: 2017 04 21, 2017 05 21, 2017 06 10,
Pilots:  P,  Q,
F add farm
P add pilot
C calculate
>c
Pilot P dusts farm F(1} on 2017 04 21
Pilot P dusts farm G(1} on 2017 04 21
Pilot P dusts farm F(2} on 2017 05 21
Pilot P dusts farm G(2} on 2017 05 21
Pilot P dusts farm F(3} on 2017 06 10
Pilot P dusts farm G(3} on 2017 06 10
</pre>

In progress is a GUI front end to help with scheduling crew to a fleet of vehicles

![screenshot](https://github.com/JamesBremner/schedule/blob/master/doc/GUIScreenShot.PNG)
