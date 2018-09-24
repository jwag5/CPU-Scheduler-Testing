// Processor.cpp
/*
This code models the operation of the CPU.  It has several queues for
holding jobs.
 */
#include <stdio.h>
#include "Processor.h"
#include "Queue.h"
#include "Scheduler.h"
#include "Job.h"

////////////////////////////////////////////////////////////////////

Processor::Processor()
{
    time = 0;
    allJobQueue = new Queue();
    newJobQueue = new Queue();
    currentJob = new Queue();
    readyQueue = new Queue();
    blockedQueue = new Queue();
    unblockedQueue = new Queue();
    doneQueue = new Queue();
    scheduler = nullptr;
}

////////////////////////////////////////////////////////////////////

Processor::~Processor()
{
    delete allJobQueue;
    delete newJobQueue;
    delete currentJob;
    delete readyQueue;
    delete blockedQueue;
    delete unblockedQueue;
    delete doneQueue;
    delete scheduler;
}

////////////////////////////////////////////////////////////////////
// After initializing the processor, when the scheduler is created, it
// will call this, linking the scheduler to the processor.
void Processor::setScheduler(Scheduler *theScheduler)
{
    scheduler = theScheduler;
    _algName = scheduler->algName();
}

////////////////////////////////////////////////////////////////////
// This function is also implemented in the sub-class
char const *Processor::algName(void)
{
    return _algName;
}

////////////////////////////////////////////////////////////////////
// Add a new job to the all-job queue.  These jobs should come in order,
// so they are automatically added to the end of the allJobsQueue.
Job *Processor::addJob(int id, int startTime, int length, int numBlocks, int priority, int answerEndTime)
{
    Job *job = new Job(id, startTime, length, numBlocks, priority, answerEndTime);
    job->enqueueEnd(allJobQueue);
    return job;
}

////////////////////////////////////////////////////////////////////

void Processor::addJob(int id, int startTime, int length, char const *blocks, int priority, int answerEndTime)
{
    Job *job = new Job(id, startTime, length, blocks, priority, answerEndTime);
    job->enqueueEnd(allJobQueue);
}

////////////////////////////////////////////////////////////////////
// This function is called once the Processor and Scheduler are set up, and
// all the jobs are added to allJobQueue.  It runs the simulation.
void Processor::runSim(void)
{
    for (time = 0; time < 100000; time++)
    {
        Job *job;

        // If every queue (except doneQueue) is empty, we are done running the simulation
        if (allJobQueue->empty() && newJobQueue->empty() && currentJob->empty() && readyQueue->empty() && blockedQueue->empty() && unblockedQueue->empty())
        {
            break;
        }

        // See if any tasks should be moved from the allJobQueue to the newJobQueue
        while (true)
        {
            job = allJobQueue->first();
            if (job == nullptr)
            {
                break;
            }
            if (job->startTime() > time)
            {
                // Not ready to schedule this (or anyone else!) yet
                break;
            }
            job->dequeue();
            job->enqueueEnd(newJobQueue);
        }

        // If there is a job in the CPU, 'run it' for one cycle.
        job = currentJob->first();
        if (job != nullptr)
        {
            job->computeOneCycle(time);
        }

        // TBD -- If there are multiple jobs in the CPU, complain

        // Run through the blocked queue, decrementing their times.
        // Any job no longer blocked goes to the unblocked queue.
        Job *next;
        for (job = blockedQueue->first(); job != nullptr; job = next)
        {
            next = job->next();
            job->decrementBlock();
            if (job->blocked() == false)
            {
                job->dequeue();
                job->enqueueEnd(unblockedQueue);
            }
        }

        // Run the scheduler for one cycle
        scheduler->tick();

        // If there is any job in the done queue that does not have its
        // end-time set, set it now.
        for (job = doneQueue->first(); job != nullptr; job = next)
        {
            next = job->next();
            if (job->endTime() < 0)
            {
                job->setEndTime(time);
            }
        }
    }

    // Break the link to the scheduler
    scheduler = nullptr;
}

////////////////////////////////////////////////////////////////////
// Generate a report from this run
void Processor::generateReport(void)
{
    int turnaround = 0;
    int count = 0;
    for (Job *job = doneQueue->first() ; job != nullptr; job = job->next())
    {
        count++;
        turnaround += job->elapsedTime();
        printf("Job %d: priority %d, estimated length %d, start time %d, "
              "end time %d, compute time %d, turnaround %d\n",
              job->id(), job->priority(), job->estimatedLength(), job->startTime(),
              job->endTime(), job->computeTime(), job->elapsedTime());
    }
    if (count > 0)
    {
        printf("Average turnaround: %f\n", static_cast<float>(turnaround) / static_cast<float>(count));
    }
}

////////////////////////////////////////////////////////////////////
// Generate a report from this run
void Processor::checkAnswer(void)
{
    int turnaround = 0;
    int count = 0;
    for (Job *job = doneQueue->first() ; job != nullptr; job = job->next())
    {
        count++;
        turnaround += job->elapsedTime();
        printf("Job %d: priority %d, estimated length %d, start time %d, "
              "end time %d, compute time %d, turnaround %d",
              job->id(), job->priority(), job->estimatedLength(), job->startTime(),
              job->endTime(), job->computeTime(), job->elapsedTime());
        if (job->endTime() == job->answerEndTime())
        {
            printf("  ** matches my answer\n");
        }
        else
        {
            printf("  ** my answer had endTime = %d\n", job->answerEndTime());
        }
    }
    if (count > 0)
    {
        printf("Average turnaround: %f\n", static_cast<float>(turnaround) / static_cast<float>(count));
    }
}

////////////////////////////////////////////////////////////////////
// Generate a homework report from this run
void Processor::checkHomework(void)
{
    int turnaround = 0;
    int count = 0;
    for (Job *job = doneQueue->first() ; job != nullptr; job = job->next())
    {
        count++;
        turnaround += job->elapsedTime();
    }
    if (count > 0)
    {
        printf("%s: Average turnaround: %f\n", _algName, static_cast<float>(turnaround) / static_cast<float>(count));
    }
}

////////////////////////////////////////////////////////////////////
// Build the 'answer' for this run
void Processor::generateAnswer(void)
{
    for (Job *job = doneQueue->first() ; job != nullptr; job = job->next())
    {
        job->printAnswer();
    }
}
