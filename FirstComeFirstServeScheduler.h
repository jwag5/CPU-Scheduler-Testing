class FirstComeFirstServedScheduler : public Scheduler
{
public:

    FirstComeFirstServedScheduler(Processor *theCPU) : Scheduler(theCPU)
    {
        cpu->setScheduler(this);
    }

    char const *algName(void) { return "First Come First Served Scheduler"; }

    // This function does the scheduling work!
    void tick(void)
    {
        Job *job;

        // If there were any jobs that used to be blocked, but no longer are,
        // move them from the unblockedQueue to the end of the readyQueue.
        while ((job = cpu->unblockedQueue->first()) != nullptr)
        {
            job->enqueueEnd(cpu->readyQueue);
        }

        // If there are any new jobs being added to the processor, move them
        // from the newJobQueue to the end of the readyQueue.
        while ((job = cpu->newJobQueue->first()) != nullptr)
        {
            job->enqueueEnd(cpu->readyQueue);
        }

        // Check the job in the cpu (if any).  If it is done, move it to the done queue,
        // and if it is blocked, move it to the blocked queue.
        job = cpu->currentJob->first();
        if (job != nullptr)
        {
            if (job->done())
            {
                job->enqueueEnd(cpu->doneQueue);
            }
            else if (job->blocked())
            {
                job->enqueueEnd(cpu->blockedQueue);
            }
        }

        // If the CPU is ready for a new job, start that job
        if (cpu->currentJob->empty())
        {
            job = cpu->readyQueue->first();
            if (job != nullptr)
            {
                job->enqueueStart(cpu->currentJob);
            }
        }
    }

    void testCase1(Processor *cpu)
    {
        cpu->addJob(1, 1, 50, "10,20,30,40", 1, 121);
        cpu->addJob(2, 1, 30, "7,15,22", 1, 111);
        cpu->addJob(3, 1, 20, "6,14", 1, 87);
        cpu->addJob(4, 1, 20, "6,14", 1, 93);
    }

    void testCase2(Processor *cpu)
    {
        cpu->addJob(1, 0, 30, "9,18,27", 4, 103);
        cpu->addJob(2, 1, 20, "3,6,9,12,15", 5, 160);
        cpu->addJob(3, 2, 30, "5,10,15,20,25", 2, 193);
        cpu->addJob(4, 21, 21, "20", 4, 95);
        cpu->addJob(5, 42, 29, "28", 3, 116);
        cpu->addJob(6, 71, 18, "5,10,15", 3, 196);
        cpu->addJob(7, 89, 13, "4,8,12", 4, 203);
        cpu->addJob(8, 102, 19, "4,8,12,16", 2, 217);
        cpu->addJob(9, 121, 19, "9,18", 1, 204);
        cpu->addJob(10, 140, 26, "6,12,18,24", 1, 232);
    }
};
