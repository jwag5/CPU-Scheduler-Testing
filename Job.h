// Job.h

class Job
{
    // The following pointers are used to form the doubly-linked list in the
    // queue.  There is also a pointer to the queue that currently holds
    // the job.
    Job *_next;
    Job *_prev;
    Queue *_queue;

    // Jobs have ID numbers for debugging
    int _id;

    // The estimated time to complete this job
    int _estimatedLength;

    // How much 'compute time' has gone toward this job
    int _computeTime;

    // The priority of this job.  For multiple queue scheduling, this will
    // change over time, so the _curPriority is the current, changing priority.
    int _priority;
    int _curPriority;

    // This job may have a list of times where it becomes blocked for I/O.
    // This is an array of integers: when the computeTime matches one
    // of these values, the job becomes blocked.
    int _numBlocks;
    int *_blockTimes;

    // Is this job done?
    bool _done;

    // At what time did this job start?
    int _startTime;

    // At what time did this job complete?
    int _endTime;

    // If this job is blocked for I/O, how much time is left in the block?
    // If this job is not blocked, the value will be 0.
    int _block;

    // For the cases where we have the 'answer', at what time did the 'answer'
    // end this job?
    int _answerEndTime;
public:

    // Create a new Job
    Job(int ID, int curTime, int length, int numBlocks, int priority, int answerEndTime);

    Job(int ID, int curTime, int length, char const *blocks, int priority, int answerEndTime);

    ~Job();

    // Set one of the block times for this Job.  Find an unset slot (its value is -1) and
    // place this value there.
    void setBlock(int time);

    // Remove the job from its current queue
    void dequeue(void);

    // Add the job to the start of a queue
    void enqueueStart(Queue *queue);

    // Add the job to the end of a queue
    void enqueueEnd(Queue *queue);

    ///// Getters and Setters /////
    inline int id(void) { return _id; }
    inline Job *next(void) { return _next; }
    inline Job *prev(void) { return _prev; }
    inline int estimatedLength(void) { return _estimatedLength; }
    inline int timeLeft(void) { return _estimatedLength - _computeTime; }
    inline int priority(void) { return _priority; }
    inline int curPriority(void) { return _curPriority; }
    inline void decPriority(void) { _curPriority--; }
    inline bool done(void) { return _done; }
    inline int startTime(void) { return _startTime; }
    inline int endTime(void) { return _endTime; }
    inline int answerEndTime(void) { return _answerEndTime; }
    inline void setEndTime(int time) { _endTime = time; }
    inline int computeTime(void) { return _computeTime; }
    inline int elapsedTime(void) { return _endTime - _startTime; }
    inline int answerElapsedTime(void) { return _answerEndTime - _startTime; }
    inline bool blocked(void) { return _block > 0; }

    ///// These should only be called by the 'engine' /////

    // This is called if the job is currently the running job.  It
    // increments the compute time, checks to see if the job is done
    // running, and checks to see if the job should be blocked.
    void computeOneCycle(int curTime);

    // This is called if the job is in the CPU's blocked queue.  It
    // decrements the block counter.  When this counter gets to 0, the
    // CPU will move this from the blocked queue to the unblocked queue.
    void decrementBlock(void);

    // Print the information about this job.
    void printAnswer(void);
};
