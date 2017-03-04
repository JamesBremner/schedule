# schedule
Schedule jobs on machines

The program inputs a json formatted schedule and outputs the schedule with the calculated start time of each job step added.

Here is the output for a simple test run.

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
