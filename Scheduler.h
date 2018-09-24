// Scheduler.h
/*
This is the Scheduler base class.  The job of a Scheduler is to determine
which job to schedule next, and in some cases when to preempt a running
job.

The Scheduler points to the 'cpu', which holds the various queues of all the
jobs.

The primary routine in the Scheduler is the 'tick' routine.  This routine
is called every timestep.  This is where the Scheduler decides how to move
the jobs around in the various CPU queues.

A Scheduler also has two test cases.  These test cases contain the list of
jobs that are to be run, including their start times, durations, and when
they block for I/O.  These lists also give the statistics for the jobs based
on my version of the scheduler.  This information allows the students to
determine if their version of the scheduler is running appropriately.  Note
that these test cases are provided by the subclasses, as the values are
scheduler-dependent.
 */
class Processor;

class Scheduler
{
public:
    // The processor used by this Scheduler:
    Processor *cpu;

    Scheduler(Processor *theCPU);

    virtual ~Scheduler();

    // This is the function that should be implemented by the sub-class to
    // perform the scheduling logic.
    virtual void tick(void) = 0;

    // This function is also implemented in the sub-class
    virtual char const *algName(void) = 0;

    // You guessed it!  This is also implemented in the sub-class
    virtual void testCase1(Processor *cpu) = 0;

    // Yet another method implemented in the sub-class
    virtual void testCase2(Processor *cpu) = 0;
};
