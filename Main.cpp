#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Processor.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Job.h"
#include "FirstComeFirstServedScheduler.h"
#include "MultipleQueueScheduler.h"
#include "PriorityScheduler.h"
#include "RoundRobinScheduler.h"
#include "ShortestJobFirstScheduler.h"
#include "ShortestRemainingTimeNextScheduler.h"

void runHomework(void);
void testCase3(Processor *cpu);

////////////////////////////////////////////////////////////////////

char const *studentName(void)
{
    return "Jake Wagner";
}

////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    printf("ScheduleSim!\n");
    while (true)
    {
        printf("0: quit\n");
        printf("1: run a test case\n");
        printf("2: run homework\n");
        printf("Yes? ");
        int op;
        scanf_s(" %d", &op);
        if (op == 0)
        {
            break;
        }
        if (op < 0 || op > 2)
        {
            continue;
        }
        if (op == 2)
        {
            runHomework();
            continue;
        }

        // We are running a test case!
        printf("0: First Come First Served\n");
        printf("1: Shortest Job First\n");
        printf("2: Round Robin\n");
        printf("3: Shortest Remaining Time Next\n");
        printf("4: Priority\n");
        printf("5: Multiple Queue\n");
        printf("Algorithm? ");
        int alg;
        scanf_s(" %d", &alg);

        Processor *cpu = new Processor();
        Scheduler *sched;
        switch (alg)
        {
        case 0:
            sched = new FirstComeFirstServedScheduler(cpu);
            break;
        case 1:
            sched = new ShortestJobFirstScheduler(cpu);
            break;
        case 2:
            sched = new RoundRobinScheduler(cpu);
            break;
        case 3:
            sched = new ShortestRemainingTimeNextScheduler(cpu);
            break;
        case 4:
            sched = new PriorityScheduler(cpu);
            break;
        case 5:
            sched = new MultipleQueueScheduler(cpu);
            break;
        default:
            continue;
        }

        printf("1: Test case 1\n");
        printf("2: Test case 2\n");
        printf("Test case? ");
        int test;
        scanf_s(" %d", &test);

        if (test < 1 || test > 2)
        {
            continue;
        }

        printf("Results for %s\n", studentName());
        if (test == 1)
        {
            sched->testCase1(cpu);
        }
        else
        {
            sched->testCase2(cpu);
        }
        cpu->runSim();
        printf("%s:\n", cpu->algName());
        cpu->checkAnswer();
    }
}

////////////////////////////////////////////////////////////////////

void runHomework(void)
{
    printf("Results for %s\n", studentName());

    Processor *cpu = new Processor();
    Scheduler *sched = new FirstComeFirstServedScheduler(cpu);
    testCase3(cpu);
    cpu->runSim();
    cpu->checkHomework();

    cpu = new Processor();
    sched = new ShortestJobFirstScheduler(cpu);
    testCase3(cpu);
    cpu->runSim();
    cpu->checkHomework();

    cpu = new Processor();
    sched = new RoundRobinScheduler(cpu);
    testCase3(cpu);
    cpu->runSim();
    cpu->checkHomework();

    cpu = new Processor();
    sched = new ShortestRemainingTimeNextScheduler(cpu);
    testCase3(cpu);
    cpu->runSim();
    cpu->checkHomework();

    cpu = new Processor();
    sched = new PriorityScheduler(cpu);
    testCase3(cpu);
    cpu->runSim();
    cpu->checkHomework();

    cpu = new Processor();
    sched = new MultipleQueueScheduler(cpu);
    testCase3(cpu);
    cpu->runSim();
    cpu->checkHomework();
}

////////////////////////////////////////////////////////////////////

void testCase3(Processor *cpu)
{
    cpu->addJob(1, 1, 19, "9,18", 3, -1);
    cpu->addJob(2, 2, 19, "4,8,12,16", 3, -1);
    cpu->addJob(3, 3, 20, "6,12,18", 2, -1);
    cpu->addJob(4, 4, 17, "5,10,15", 1, -1);
    cpu->addJob(5, 5, 10, "1,2,3,4,5", 5, -1);
    cpu->addJob(6, 6, 29, "5,10,15,20,25", 2, -1);
    cpu->addJob(7, 7, 20, "3,6,9,12,15", 1, -1);
    cpu->addJob(8, 16, 28, "5,10,15,20,25", 3, -1);
    cpu->addJob(9, 44, 12, "2,4,6,8", 2, -1);
    cpu->addJob(10, 56, 19, "18", 5, -1);
    cpu->addJob(11, 75, 25, "12,24", 4, -1);
    cpu->addJob(12, 100, 14, "6,12", 5, -1);
    cpu->addJob(13, 114, 10, "3,6,9", 4, -1);
    cpu->addJob(14, 124, 29, "14,28", 3, -1);
    cpu->addJob(15, 153, 12, "11", 5, -1);
    cpu->addJob(16, 165, 19, "3,6,9,12,15", 1, -1);
    cpu->addJob(17, 184, 16, "15", 4, -1);
    cpu->addJob(18, 200, 30, "9,18,27", 3, -1);
    cpu->addJob(19, 230, 16, "7,14", 5, -1);
    cpu->addJob(20, 246, 23, "5,10,15,20", 5, -1);
    cpu->addJob(21, 269, 14, "4,8,12", 1, -1);
    cpu->addJob(22, 283, 25, "24", 2, -1);
    cpu->addJob(23, 308, 28, "13,26", 2, -1);
    cpu->addJob(24, 336, 25, "8,16,24", 3, -1);
    cpu->addJob(25, 361, 26, "6,12,18,24", 4, -1);
    cpu->addJob(26, 387, 20, "3,6,9,12,15", 2, -1);
    cpu->addJob(27, 407, 29, "5,10,15,20,25", 4, -1);
    cpu->addJob(28, 436, 25, "12,24", 5, -1);
    cpu->addJob(29, 461, 29, "7,14,21,28", 5, -1);
    cpu->addJob(30, 490, 29, "9,18,27", 5, -1);
}
