// Queue.cpp
/*
This implements a fairly simple Queue.  These are usually first-in, first-out,
although there are routines to support priority or shortest-job mechanisms, which
pull jobs from the middle of the queue.

Note that these queues are different from the traditional queue:  The next pointers
are actually in the job class, and the job points to the queue which contains the
job.  This means that a job can only be in one queue at a time (which is exactly
what we want for our simulation).  For a traditional queue, there would not be the
pointers in the object, and the queue would have a sub-structure used for forming
the lists.  This would also allow a job to exist in multiple queues, and by using
templates, the Queue class could be used for any type of data.  However, these
features are not important for our example, and we really do want jobs to only be
in a single queue.

Consequently, the job has a dequeue function, which pulls the job out of its existing
queue (along with priority and size related versions), and it has functions for
placing the job into a queue.  Actually, the routine for placing a job into a queue
will also remove the job from its existing queue if necessary.

The routine here for pulling the job from the queue allows any job to be pulled from
the queue.  It doesn't have to be the first or last job, it can be any job.
 */
#include "Queue.h"
#include "Job.h"

Queue::Queue()
{
    _firstJob = nullptr;
    _lastJob = nullptr;
}

Queue::~Queue()
{
    while (_firstJob)
    {
        delete _firstJob;
    }
}

Job *Queue::highestPriorityJob(void)
{
  if (_firstJob == nullptr)
  {
    return nullptr;
  }
  Job *best = nullptr;
  int bestPriority = -999;
  for (Job *job = _firstJob; job != nullptr; job = job->next())
  {
      int curPriority = job->priority();
      if (best == nullptr || curPriority > bestPriority)
      {
          best = job;
          bestPriority = curPriority;
      }
  }
  return best;
}

Job *Queue::shortestJob(void)
{
  if (_firstJob == nullptr)
  {
    return nullptr;
  }
  Job *best = nullptr;
  int bestLength = 9999999;
  for (Job *job = _firstJob; job != nullptr; job = job->next())
  {
      int curLen = job->timeLeft();
      if (best == nullptr || curLen < bestLength)
      {
          best = job;
          bestLength = curLen;
      }
  }
  return best;
}
