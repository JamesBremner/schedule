# Schedule
Schedule jobs on machines

The program inputs a json formatted schedule and outputs the schedule with the calculated start time of each job step added.

The following cases are handled:

1. Jobs with multiple steps, each on a specified machine taking a specified time, which must be completed in sequence.
1. Jobs with a single step that can be completed on different machines at different cost.

Here is the output for a simple test run for case 1.

<pre>
{
  "jobs": [
    {
      "earliest": 0,
      "name": "A-B",
      "steps": [
        {
          "machine": "A",
          "name": "A",
          "previous": -99,
          "start": 0,
          "time": 10
        },
        {
          "machine": "B",
          "name": "B",
          "previous": 0,
          "start": 10,
          "time": 2
        }
      ]
    },
    {
      "earliest": 0,
      "name": "A-C",
      "steps": [
        {
          "machine": "A",
          "name": "A",
          "previous": -99,
          "start": 10,
          "time": 1
        },
        {
          "machine": "C",
          "name": "C",
          "previous": 2,
          "start": 11,
          "time": 20
        }
      ]
    }
  ]
}
</pre>
