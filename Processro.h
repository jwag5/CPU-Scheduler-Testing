// Processor.h
/*
This code models the operation of the CPU.  It has several queues for
holding jobs.
 */
class Queue;
class Scheduler;
class Job;

class Processor
{
public:
    // The current 'time' of the simulation
    int time;

    // This queue is not used by the scheduler.  This holds the list of all the
    // future jobs.
private:
    Queue *allJobQueue;
public:

    // Queue holding new jobs that are being added to the simulation this cycle:
    Queue *newJobQueue;

    // Queue holding the one job that is being processed by the CPU
    Queue *currentJob;

    // Queue holding jobs that are ready to run
    Queue *readyQueue;

    // Queue holding jobs that are blocked for IO
    Queue *blockedQueue;

    // Queue holding jobs that are no longer blocked
    Queue *unblockedQueue;

    // Queue holding jobs that are done
    Queue *doneQueue;

    // The scheduler to be used to move jobs through the processor
    Scheduler *scheduler;
    char const *_algName;

    Processor();

    ~Processor();

    // After initializing the processor, when the scheduler is created, it
    // will call this, linking the scheduler to the processor.
    void setScheduler(Scheduler *theScheduler);

    // This function is also implemented in the sub-class
    char const *algName(void);

    // Add a new job to the all-job queue.  These jobs should come in order,
    // so they are automatically added to the end of the allJobsQueue.
    Job *addJob(int id, int startTime, int length, int numBlocks, int priority, int answerEndTime);

    void addJob(int id, int startTime, int length, char const *blocks, int priority, int answerEndTime);

    // This function is called once the Processor and Scheduler are set up, and
    // all the jobs are added to allJobQueue.  It runs the simulation.
    void runSim(void);

    // Generate a report from this run
    void generateReport(void);

    // Generate a report from this run
    void checkAnswer(void);

    // Generate a homework report from this run
    void checkHomework(void);

    // Build the 'answer' for this run
    void generateAnswer(void);
};
