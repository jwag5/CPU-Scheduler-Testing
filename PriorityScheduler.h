class PriorityScheduler : public Scheduler
{
public:

    PriorityScheduler(Processor *theCPU) : Scheduler(theCPU)
    {
        cpu->setScheduler(this);
    }

    char const *algName(void) { return "Priority Scheduler"; }

    // This function does the scheduling work!
    void tick(void)
    {
		Job *job;

		// If there are any new jobs being added to the processor, move them
		// from the newJobQueue to the end of the readyQueue.
		while ((job = cpu->newJobQueue->first()) != nullptr)
		{
			job->enqueueEnd(cpu->readyQueue);
		}

		// If there were any jobs that used to be blocked, but no longer are,
		// move them from the unblockedQueue to the end of the readyQueue.
		while ((job = cpu->unblockedQueue->first()) != nullptr)
		{
			job->enqueueEnd(cpu->readyQueue);
		}

		//If there is a job(s) in the readyQueue, check the highest priority 
		//to the running process, if any. If higher swap, else continue running.
		if (((job = cpu->readyQueue->highestPriorityJob()) != nullptr) && (! cpu->currentJob->empty()))
		{
			if (job->priority() > cpu->currentJob->first()->priority())
			{
				cpu->currentJob->first()->enqueueEnd(cpu->readyQueue);
				job->enqueueStart(cpu->currentJob);
			}
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
			job = cpu->readyQueue->highestPriorityJob();
			if (job != nullptr)
			{
				job->enqueueStart(cpu->currentJob);
			}
		}


    }

    void testCase1(Processor *cpu)
    {
        cpu->addJob(1, 1, 50, "10,20,30,40", 1, 141);
        cpu->addJob(2, 1, 30, "7,15,22", 3, 51);
        cpu->addJob(3, 1, 20, "6,14", 2, 81);
        cpu->addJob(4, 1, 20, "6,14", 3, 43);
    }

    void testCase2(Processor *cpu)
    {
        cpu->addJob(1, 0, 30, "9,18,27", 4, 77);
        cpu->addJob(2, 1, 20, "3,6,9,12,15", 5, 46);
        cpu->addJob(3, 2, 30, "5,10,15,20,25", 2, 189);
        cpu->addJob(4, 21, 21, "20", 4, 70);
        cpu->addJob(5, 42, 29, "28", 3, 131);
        cpu->addJob(6, 71, 18, "5,10,15", 3, 137);
        cpu->addJob(7, 89, 13, "4,8,12", 4, 117);
        cpu->addJob(8, 102, 19, "4,8,12,16", 2, 182);
        cpu->addJob(9, 121, 19, "9,18", 1, 220);
        cpu->addJob(10, 140, 26, "6,12,18,24", 1, 237);
    }
};
