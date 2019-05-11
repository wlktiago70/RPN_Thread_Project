////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Embraer
//
// Project  : RPN Test
//
// File     : main.cpp
// Created  : May 2019
// Author   : Wu Liang Kuan
//
////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <math.h>
#include "ReversePolishNotation.hpp"

using namespace std;

static int counter = 0;

static queue<string> A;
static queue<string> B;
static queue<string> C1;
static queue<string> C2;

bool thdADone = false;
bool thdBDone = false;

/**
 * Fills the queues A and B with data retrieved from file
 * @param pString An string containing data to be stored in queue A and B
 */
inline void fillQueues(string&);

/**
 * Perform the first processing of information
 * @param pInQueue An string queue to retrieve the data to be processed
 * @param pOutQueue An string queue to store the processed data
 * @param pInMutex A mutual exclusion object to synchronize with main thread
 * @param pOutMutex A mutual exclusion object to synchronize with thread C
 * @param pCond A synchronization primitive to block a concurrent thread
 * @param pFlag A flag to signalize the end of processing
 */
void firstProcess(queue<string>&, queue<string>&, mutex&, mutex&, condition_variable&, bool&);

/**
 * Perform the second processing of information
 * @param pIn1Mutex A mutual exclusion object to synchronize with thread A
 * @param pIn2Mutex A mutual exclusion object to synchronize with thread B
 */
void secondProcess(mutex&, mutex&);

#define printResult(res) cout<< "linha " << counter << " = " << res << endl;

int main(int argc, char *argv[])
{
    mutex gALock; ///< In case of multiple threads trying to modify the queue A
    mutex gBLock; ///< In case of multiple threads trying to modify the queue B
    mutex gC1Lock; ///< In case of multiple threads trying to modify the queue C1
    mutex gC2Lock; ///< In case of multiple threads trying to modify the queue C2

    condition_variable gCond;

    thread tA(firstProcess, ref(A), ref(C1), ref(gALock), ref(gC1Lock), ref(gCond), ref(thdADone));
    thread tB(firstProcess, ref(B), ref(C2), ref(gBLock), ref(gC2Lock), ref(gCond), ref(thdBDone));
    thread tC(secondProcess, ref(gC1Lock), ref(gC2Lock));

    cout<< fixed << showpoint << setprecision(2);

    if(argc > 1)
    {
        ifstream inputFile(argv[1]);
        string line;

        if(inputFile.is_open())
        {
            unique_lock<mutex> lockA(gALock);
            unique_lock<mutex> lockB(gBLock);
            while(getline(inputFile,line))
            {
                fillQueues(line);
            }
            inputFile.close();
            gCond.notify_all();
        }
        else
        {
            cout<<"Unable to open "<<argv[1]<<endl;
        }
    }
    else
    {
        cout<<"Missing parameter: input file path"<<endl;
    }

    tA.join();
    tB.join();
    tC.join();

    return 0;
}

void fillQueues(string& pString)
{
    string strA,strB;
    size_t start = pString.find_first_of(";");
    strA = pString.substr(0,start);
    strB = pString.substr(start+1);
    A.push(strA.substr(strA.find_first_of("=")+1));
    B.push(strB.substr(strB.find_first_of("=")+1));
    #ifdef DEBUG
    cout<< "A = " << A.back() << endl;
    cout<< "B = " << B.back() << endl;
    #endif // DEBUG
}

void firstProcess(queue<string>& pInQueue,
                  queue<string>& pOutQueue,
                  mutex& pInMutex,
                  mutex& pOutMutex,
                  condition_variable& pCond,
                  bool& pFlag)
{
    unique_lock<mutex> lockIn(pInMutex);
    unique_lock<mutex> lockOut(pOutMutex);
    pCond.wait(lockIn);
    while(!pInQueue.empty())
    {
        ReversePolishNotation rpn(pInQueue.front());
        pInQueue.pop();
        pOutQueue.push(rpn.getStringResult());
    }
    pFlag = true;
}

void secondProcess(mutex& pIn1Mutex, mutex& pIn2Mutex)
{
    unique_lock<mutex> lockC1(pIn1Mutex);
    unique_lock<mutex> lockC2(pIn2Mutex);
    while(!(thdADone&&thdBDone))
    {
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    while(!C1.empty()&&!C2.empty())
    {
        double A,B;
        try
        {
            counter++;
            A = stod(C1.front());
            B = stod(C2.front());
            printResult(sqrt(A*A+B*B));
        }
        catch(const std::invalid_argument&)
        {
            printResult("NaN");
        }
        C1.pop();
        C2.pop();
    }
}
