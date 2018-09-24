// Queue.h
class Job;

class Queue
{
    Job *_firstJob;
    Job *_lastJob;

public:
    Queue();
    ~Queue();

    // Getters:
    inline Job *first(void) { return _firstJob; }
    inline Job *last(void) { return _lastJob; }
    inline bool empty(void) { return _firstJob == nullptr; }

    // Setters -- these should only be called by the Job
    void setFirst(Job *job) { _firstJob = job; }
    void setLast(Job *job) { _lastJob = job; }

    // This scans the queue looking for the job with the highest priority,
    // which is returned.  If there are multiple jobs with the highest
    // priority, the routine returns the one that is nearest the start of
    // the list.  This routine may return null, indicating that there are
    // no jobs in the queue.
    Job *highestPriorityJob(void);

    // This scans the queue, looking for the job with the smallest
    // remaining execution time.  If there are multiple jobs with
    // the smallest time, the routine returns the job closest to the
    // start of the queue.  The routine may return null, which
    // indicates that there are no jobs left in the queue.
    Job *shortestJob(void);
};
