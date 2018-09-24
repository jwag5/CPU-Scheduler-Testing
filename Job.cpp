// Job.cpp
/*
This class holds one 'job' for the simulation.  Each job exists in some
CPU queue (ready, blocked, running, and several others).  The scheduler's
duty is to move jobs from queue to queue as the simulation proceeds.
 */
#include <stdio.h>
#include <string.h>
#include "Queue.h"
#include "Job.h"

Job::Job(int ID, int curTime, int length, int numBlocks, int priority, int answerEndTime)
{
    _id = ID;
    _next = nullptr;
    _prev = nullptr;
    _queue = nullptr;
    _estimatedLength = length;
    _computeTime = 0;
    _priority = priority;
    _curPriority = priority;
    _numBlocks = numBlocks;
    if (numBlocks == 0)
    {
        _blockTimes = new int[1];
        _blockTimes[0] = -1;
    }
    else
    {
        _blockTimes = new int[numBlocks];
        int i;
        for (i = 0; i < numBlocks; i++)
        {
            _blockTimes[i] = -1;
        }
    }
    _done = false;
    _startTime = curTime;
    _endTime = -1;
    _answerEndTime = answerEndTime;
    _block = 0;
}

Job::Job(int ID, int curTime, int length, char const *blocks, int priority, int answerEndTime)
{
    _id = ID;
    _next = nullptr;
    _prev = nullptr;
    _queue = nullptr;
    _estimatedLength = length;
    _computeTime = 0;
    _priority = priority;
    _curPriority = priority;
    _done = false;
    _startTime = curTime;
    _endTime = -1;
    _answerEndTime = answerEndTime;
    _block = 0;
    _numBlocks = 1;
    int len = static_cast<int>(strlen(blocks));
    for (int i = 0 ; i < len; i++)
    {
        char ch = blocks[i];
        if (ch == ',')
        {
            _numBlocks++;
        }
    }
    _blockTimes = new int[_numBlocks];
    _numBlocks = 0;
    int time = 0;
    for (int i = 0; i < len; i++)
    {
        char ch = blocks[i];
        if (ch == ',')
        {
            _blockTimes[_numBlocks++] = time;
            time = 0;
        }
        else if (ch >= '0' && ch <= '9')
        {
            time = 10 * time + ch - '0';
        }
    }
    _blockTimes[_numBlocks++] = time;
}

Job::~Job()
{
    dequeue();
    if (_blockTimes)
    {
        delete[] _blockTimes;
    }
}

// Set one of the block times for this Job.  Find an unset slot (its value is -1) and
// place this value there.
void Job::setBlock(int time)
{
    int len = _numBlocks;
    int i;
    for (i = 0 ; i < len; i++)
    {
        if (_blockTimes[i] < 0)
        {
            _blockTimes[i] = time;
            return;
        }
    }
    _blockTimes[0] = time;
}

// Remove the job from its current queue
void Job::dequeue(void)
{
    // If this job has a _prev value, there is a job before
    // this one in the queue.  Hence, we will tell that job
    // to point around us.  If there is NO job before this one,
    // this must be the first job in the queue, in which case
    // we will tell the queue to use the next job as the first.
    if (_prev != nullptr)
    {
        _prev->_next = _next;
    }
    else if (_queue != nullptr)
    {
        _queue->setFirst(_next);
    }

    // In a similar way we update the _next, either pointing
    // around this job or changing the queue's last job pointer.
    if (_next != nullptr)
    {
        _next->_prev = _prev;
    }
    else if (_queue != nullptr)
    {
        _queue->setLast(_prev);
    }

    // Now clear the three pointers for this job: the job is
    // no longer a part of that queue.
    _prev = nullptr;
    _next = nullptr;
    _queue = nullptr;
}

// Add the job to the start of a queue
void Job::enqueueStart(Queue *queue)
{
    // If already in a queue, remove it
    if (_queue != nullptr)
    {
        dequeue();
    }

    // Grab the first job in the queue.  If there is one, have it point
    // back to this new job.  If there is NOT a job currently in the
    // queue, this becomes the queue's LAST job (it will also become the
    // queue's FIRST job, but that's in a few lines).
    _next = queue->first();
    if (_next != nullptr)
    {
        _next->_prev = this;
    }
    else
    {
        queue->setLast(this);
    }

    // Since this will be the first job in the queue, the _prev pointer
    // will be nullptr.  Also point to the queue, and have this job be the
    // queue's first job.
    _prev = nullptr;
    _queue = queue;
    queue->setFirst(this);
}

// Add the job to the end of a queue
void Job::enqueueEnd(Queue *queue)
{
    // If already in a queue, remove it
    if (_queue != nullptr)
    {
        dequeue();
    }

    // Grab the last job in the queue.  If there is one, have it point
    // forward to this new job.  If there is NOT a job currently in the
    // queue, this becomes the queue's FIRST job (it will also become the
    // queue's LAST job, but that's in a few lines).
    _prev = queue->last();
    if (_prev != nullptr)
    {
        _prev->_next = this;
    }
    else
    {
        queue->setFirst(this);
    }

    // Since this will be the last job in the queue, the _next pointer
    // will be nullptr.  Also point to the queue, and have this job be the
    // queue's last job.
    _next = nullptr;
    _queue = queue;
    queue->setLast(this);
}

// This is called if the job is currently the running job.  It
// increments the compute time, checks to see if the job is done
// running, and checks to see if the job should be blocked.
void Job::computeOneCycle(int curTime)
{
    // TBD: Print an error message if we are done -- we should not still be
    // in the processor!

    // TBD: Print an error message if we are blocked -- we should be in the
    // blocked queue!

    _computeTime++;

    // See if we are done
    if (_computeTime >= _estimatedLength)
    {
        _done = true;
        _endTime = curTime;
    }

    // See if we should block
    int len = _numBlocks;
    int i;
    for (i = 0 ; i < len; i++)
    {
        if (_blockTimes[i] == _computeTime)
        {
            _block += 5;   // Block for a period of time
            break;
        }
    }
}

// This is called if the job is in the CPU's blocked queue.  It
// decrements the block counter.  When this counter gets to 0, the
// CPU will move this from the blocked queue to the unblocked queue.
void Job::decrementBlock(void)
{
    if (_block > 0)
    {
        _block--;
    }
}

// Print the information about this job.
void Job::printAnswer(void)
{
    printf("cpu.addJob(%d, %d, %d, \"",
        _id, _startTime, _estimatedLength);
    int len = _numBlocks;
    if (len > 0)
    {
        for (int i = 0 ; i < len; i++)
        {
            if (i > 0)
            {
                printf(",");
            }
            printf("%d", _blockTimes[i]);
        }
    }
    printf("\", %d, %d);\n", _priority, _endTime);
}
