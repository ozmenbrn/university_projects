/*
Student Name: Barın Özmen
Student Number: 2012400045
Project Number: 2
Operating System: Ubuntu
Compile Status: YES! It compiles
Program Status: Not perfect alghoritm
Notes: I sense that with small touch the project will calculate perfectly. But for 5 days in a row i am trying to do this and try many different
 algorithms and i cannot think clearly. I think i should have started 2 weeks ago and not rush like this. But unfortunately, i didn't have a time
 to look at it. Not because of fun :).. Projects exams job ...

*/
#include <algorithm>
#include <queue>
#include <random>
#include <thread>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Task{

private:
    double arrivalTime, cpuWork, outWork, processTime, queuetime , finishT;
    int id;
    bool inpq ,inqueue;
public:

    Task(double arrivalTime, double cpuWork, double outWork, int id){
        this->arrivalTime = arrivalTime;
        this->cpuWork = cpuWork;
        this->outWork = outWork;
        this->id = id;
    }

    bool inPQ() const{
        return inpq;
    }
    bool setinPQ(bool inpq){
        this->inpq = inpq;
    }
    bool inQueue() const{
        return inqueue;
    }
    bool setinQueue(bool inqueue){
        this->inqueue = inqueue;
    }

    int getID() const{
        return id;
    }

    double getcpuW() const{
        return cpuWork;
    }

    double getoutW() const{
        return outWork;
    }

    double getArrival() const{
        return arrivalTime;
    }

    double getFinish() const{
        return finishT;
    }

    double setFinish(double finishT){
        this->finishT = finishT;
    }

    double getProcessTime() const{
        return processTime;
    }

    double setProcessTime(double processTime){
        this->processTime = processTime;
    }

    double setcpuW(double cpuWork){
        this->cpuWork = cpuWork;
    }

    double setoutW(double outWork){
        this->outWork = outWork;
    }

    double getQueueTime() const {
        return queuetime;
    }

    double setQueueTime(double queuetime){
        this->queuetime = queuetime;
    }

    bool operator<(const Task&)const;



};

bool Task::operator<(const Task& right)const{
    return cpuWork > right.cpuWork;
}

struct compare {
    bool operator()(const double& l, const double& r)
    {
        return l < r;
    }
};


struct CPU{
private:
    double duration;
    int id;
    double frequency;
    bool isempty = true;
    double activetime;
public:

    CPU(int id ,double duration , double frequency){
        this->id = id;
        this->duration= duration;
        this->frequency = frequency;
    }

    double getID() const{
        return id;
    }

    double getFrequency() const{
        return frequency;
    }

    double getDuration() const{
        return duration;
    }

    double setDuration(double duration){
        this->duration = duration;
    }

    bool isEmpty()const {
        return isempty;
    }

    double setActiveTime(double activetime){
        this->activetime = activetime;
    }

    double getActiveTime() const{
        return activetime;
    }

    bool setEmpty(bool isempty){
        this->isempty = isempty;
    }




};

struct OUTD{
private:
    double duration;
    int id;
    double quantum;
    bool isempty = true;
    double activetime;
public:
    OUTD(int id ,double duration , double quantum){
        this->id = id;
        this->duration= duration;
        this->quantum = quantum;
    }

    double getID() const{
        return id;
    }

    double getQuantum() const{
        return quantum;
    }

    double getDuration() const{
        return duration;
    }

    double setDuration(double duration){
        this->duration = duration;
    }

    double setActiveTime(double activetime){
        this->activetime = activetime;
    }

    double getActiveTime() const{
        return activetime;
    }

    bool isEmpty() const{
        return isempty;
    }

    bool setEmpty(bool isempty){
        this->isempty = isempty;
    }



};


int main (int argc, char* argv[]) {

    double eventTime = 0;
    int cpuNum;
    int outNum;
    int taskNum;
    int tempi;
    double tempd;
    vector<double> cpuFrequency;
    vector<double> outquantum;
    vector<double> arrivalTime;
    vector<double> cpuWork;
    vector<double> outWork;
    bool isTrue = true;
    int cpuTask=0;
    int outTask=0;



    freopen(argv[1], "r" , stdin);
     freopen(argv[2], "w" , stdout);

    cin >> cpuNum;

    for(int i=0; i<cpuNum ; i++){
        cin >> tempi;
        cpuFrequency.push_back(tempi);
    }

    cin >> outNum;

    for(int i=0; i<outNum ; i++){
        cin >> tempd;
        outquantum.push_back(tempd);
    }

    cin >> taskNum;

    for(int i=0; i<taskNum ; i++){
        cin >> tempd;
        arrivalTime.push_back(tempd);
        cin >> tempd;
        cpuWork.push_back(tempd);
        cin >> tempd;
        outWork.push_back(tempd);

    }

    vector<Task> task;
    vector<CPU> cpu;
    vector<OUTD> outd;
    priority_queue<Task> pq;
    queue<Task> queue;
    priority_queue<double,vector<double>, compare> compare;
    for(int i=0; i<taskNum; i++){
        task.push_back(Task(arrivalTime[i],cpuWork[i],outWork[i], i));
        task[i].setProcessTime(task[i].getArrival());
        task[i].setinPQ(false);
        task[i].setinQueue(false);
        task[i].setQueueTime(0);
    }

    for(int i=0; i<cpuNum; i++){
        cpu.push_back(CPU(i+1, 0, cpuFrequency[i]));
    }

    for(int i=0; i<outNum; i++){
        outd.push_back(OUTD(i+1, 0, outquantum[i]));
    }


    double temptime = 0;
    for(int i=0; i<taskNum; i++){
        if(task[i].getArrival() > temptime){
            temptime = task[i].getArrival();
        }
    }

    double maxArrival =temptime;
    int tempCPUnum;
    int tempOUTnum;
    int tempID;
    double futureTime;
    int count = 0;
    double longestWait =0;
    double queueTime = 0;
    double timeSpent = 0;
    int maxLengthPQ=0;
    int maxLengthQ=0;
    double greatestActiveQueue;
    int idqueue;
    double greatestActivePQ;
    int idpq;


    while(isTrue){

        cpuTask =0;
        outTask =0;
        bool tempEmpty = false;
        for(int i=0; i<taskNum; i++) {
            tempEmpty = false;
            if ((task[i].getProcessTime() == eventTime) && task[i].getcpuW() !=0) {
                if (pq.empty()) {                                     //if PQ is empty
                    tempEmpty=false;
                    for (int v = cpuNum - 1; v >= 0; v--) {
                        if (cpu[v].isEmpty() == true) {
                            tempEmpty = true;
                            tempCPUnum = v;                         //Minimum CPU[id] is captured
                        }
                    }
                    if (tempEmpty) {                                                      //if PQ && CPU are empty!

                        futureTime = task[i].getcpuW() / cpu[tempCPUnum].getFrequency();
                        task[i].setProcessTime(task[i].getProcessTime() + futureTime);
                        task[i].setcpuW(0);
                        cpu[tempCPUnum].setDuration(task[i].getProcessTime());
                        cpu[tempCPUnum].setEmpty(false);

                    }
                    else {                                                               //PQ empty CPUs are not !!!
                        double tempCPUtime = cpu[0].getDuration();
                        for (int p = 0; p < cpuNum; p++) {
                            if (tempCPUtime > cpu[p].getDuration()) {
                                tempCPUtime = cpu[p].getDuration();
                                tempCPUnum = p;
                            }
                        }
                        if(task[i].inPQ() == false){
                            queueTime += cpu[tempCPUnum].getDuration() - task[i].getProcessTime();
                            task[i].setQueueTime(task[i].getQueueTime() + cpu[tempCPUnum].getDuration() - task[i].getProcessTime());
                            task[i].setProcessTime(cpu[tempCPUnum].getDuration());
                            task[i].setinPQ(true);
                            pq.push(task[i]);

                            if(maxLengthPQ < pq.size()){
                                maxLengthPQ = pq.size();
                            }
                        }
                        else{
                            queueTime += cpu[tempCPUnum].getDuration() - task[i].getProcessTime();
                            task[i].setQueueTime(task[i].getQueueTime() + cpu[tempCPUnum].getDuration() - task[i].getProcessTime());
                            task[i].setProcessTime(cpu[tempCPUnum].getDuration());
                            if(maxLengthPQ < pq.size()){
                                maxLengthPQ = pq.size();
                            }
                        }
                    }
                }
                else {                                                                           // PQ is not empty !!!
                    tempEmpty=false;
                    double tempCPUtime = cpu[1].getDuration();
                    /* double tempCPUtime = cpu[0].getDuration();
                     for (int p = 0; p < cpuNum; p++) {
                         if (tempCPUtime > cpu[p].getDuration()) {
                             tempCPUtime = cpu[p].getDuration();
                             tempCPUnum = p;
                         }
                     }*/
                    for (int g = cpuNum - 1; g >= 0; g--) {
                        if (cpu[g].isEmpty() == true) {
                            tempEmpty = true;
                            tempCPUnum = g;                         //Minimum CPU[id] is captured   //NOT SURE !!
                        }
                    }
                    // it means CPU is not either !!
                    if (tempEmpty == false) {
                        for (int l = 0; l < cpuNum; l++) {
                            if (tempCPUtime > cpu[l].getDuration()) {                // LOOK HERE !!! BLA BLA BLA
                                tempCPUtime = cpu[l].getDuration();
                                tempCPUnum = l;
                                //    cout << " -asd-asd-as-da-sd-asd" << endl;
                            }
                        }
                        if(!task[i].inPQ()) {
                            queueTime += cpu[tempCPUnum].getDuration() - task[i].getProcessTime();
                            task[i].setQueueTime(task[i].getQueueTime() + cpu[tempCPUnum].getDuration() - task[i].getProcessTime());
                            task[i].setProcessTime(cpu[tempCPUnum].getDuration());
                            task[i].setinPQ(true);
                            pq.push(task[i]);
                            if(maxLengthPQ < pq.size()){
                                maxLengthPQ = pq.size();
                            }
                        }
                        else{
                            queueTime += cpu[tempCPUnum].getDuration() - task[i].getProcessTime();
                            task[i].setQueueTime(task[i].getQueueTime() + cpu[tempCPUnum].getDuration() - task[i].getProcessTime());
                            task[i].setProcessTime(cpu[tempCPUnum].getDuration());
                        }
                    }
                    else{
                        /*    for (int g = cpuNum - 1; g >= 0; g--) {
                                if (cpu[g].isEmpty() == true) {
                                    tempEmpty = true;
                                    tempCPUnum = g;                         //Minimum CPU[id] is captured   //NOT SURE !!
                                }
                            }*/
                        tempID = pq.top().getID();
                        pq.pop();
                        futureTime = task[tempID].getcpuW() / cpu[tempCPUnum].getFrequency();
                        task[tempID].setProcessTime(task[tempID].getProcessTime() + futureTime);
                        task[tempID].setcpuW(0);
                        cpu[tempCPUnum].setDuration(task[tempID].getProcessTime());
                        cpu[tempCPUnum].setEmpty(false);
                    }
                    //    cout << " -asd-asd-as-da-sd-asd" << endl;
                }
            }


            //     tempEmpty = false;                                                                //SECOND STAGE
            if((task[i].getProcessTime() == eventTime) && (task[i].getoutW() != 0)){
                if(queue.empty()) {                                          //Round Robin QUEUE is empty
                    tempEmpty = false;
                    for (int h = outNum - 1; h >= 0; h--) {
                        if (outd[h].isEmpty() == true) {
                            tempEmpty = true;
                            tempOUTnum = h;                         //Minimum OUTD[id] is captured
                        }
                    }
                    if (tempEmpty) {                                      //QUEUE   ++   OUTD  are empty
                        if((task[i].getoutW() - outd[tempOUTnum].getQuantum() >=0)) {           // finished or not
                            task[i].setProcessTime(task[i].getProcessTime() + outd[tempOUTnum].getQuantum());
                            task[i].setoutW(task[i].getoutW() - outd[tempOUTnum].getQuantum());
                            outd[tempOUTnum].setDuration(task[i].getProcessTime());
                            outd[tempOUTnum].setEmpty(false);
                        }
                        else{
                            futureTime = task[i].getoutW();
                            task[i].setProcessTime(task[i].getProcessTime() + futureTime);
                            task[i].setoutW(0);
                            outd[tempOUTnum].setDuration(task[i].getProcessTime());
                            outd[tempOUTnum].setEmpty(false);
                        }
                    }
                    else {                                                   // QUEUE is empty  OUTD!!!! not
                        double tempOUTtime = outd[outNum - 1].getDuration();
                        for (int p = 0; p < outNum; p++) {
                            if (tempOUTtime > outd[p].getDuration()) {
                                tempOUTtime = outd[p].getDuration();
                                tempOUTnum = p;
                            }
                        }
                        if(!task[i].inQueue()){
                            queueTime += outd[tempOUTnum].getDuration() - task[i].getProcessTime();
                            task[i].setQueueTime(task[i].getQueueTime() + outd[tempOUTnum].getDuration() - task[i].getProcessTime());
                            task[i].setProcessTime(outd[tempOUTnum].getDuration());
                            task[i].setinQueue(true);
                            queue.push(task[i]);
                            if(maxLengthQ< queue.size()){
                                maxLengthQ = queue.size();
                            }
                        }
                        else{
                            queueTime += outd[tempOUTnum].getDuration() - task[i].getProcessTime();
                            task[i].setQueueTime(task[i].getQueueTime() + outd[tempOUTnum].getDuration() - task[i].getProcessTime());
                            task[i].setProcessTime((outd[tempOUTnum].getDuration()));
                        }
                        //      }

                    }
                }
                else{                                                                             //QUEUE not empty means
                    double tempOUTime = outd[outNum-1].getDuration();
                    tempEmpty=false;
                    for (int c = outNum - 1; c >= 0; c--) {
                        if (outd[c].isEmpty() == true) {
                            tempEmpty = true;
                            tempOUTnum = c;                         //Minimum OUTD[id] is captured
                        }
                    }
                    if(!tempEmpty) {
                        for (int y = 0; y < outNum; y++) {
                            if (tempOUTime > outd[y].getDuration()) {                                // IAM HEREE
                                tempOUTime = outd[y].getDuration();
                                tempOUTnum = y;
                            }
                        }
                        if(!task[i].inQueue()){
                            queueTime += outd[tempOUTnum].getDuration() - task[i].getProcessTime();
                            task[i].setQueueTime(task[i].getQueueTime() + outd[tempOUTnum].getDuration() - task[i].getProcessTime());
                            task[i].setProcessTime(outd[tempOUTnum].getDuration());
                            task[i].setinQueue(true);
                            queue.push(task[i]);
                            if(maxLengthQ< queue.size()){
                                maxLengthQ = queue.size();
                            }
                        }
                        else{
                            queueTime += outd[tempOUTnum].getDuration() - task[i].getProcessTime();
                            task[i].setQueueTime(task[i].getQueueTime() + outd[tempOUTnum].getDuration() - task[i].getProcessTime());
                            task[i].setProcessTime((outd[tempOUTnum].getDuration()));
                        }
                    }
                    else{                                                           //QUEUE not empty but OUTD empty

                        // In this case, Task which is in the queue is looking for an empty OUTD
                        tempID = queue.front().getID();
                        queue.pop();
                        if ((task[tempID].getoutW() - outd[tempOUTnum].getQuantum() >= 0)) {           // finished or not
                            task[tempID].setProcessTime(task[tempID].getProcessTime() + outd[tempOUTnum].getQuantum());
                            task[tempID].setoutW(task[tempID].getoutW() - outd[tempOUTnum].getQuantum());
                            outd[tempOUTnum].setDuration(task[tempID].getProcessTime());
                            outd[tempOUTnum].setEmpty(false);
                        } else {
                            futureTime = task[i].getoutW();
                            task[tempID].setProcessTime(task[tempID].getProcessTime() + futureTime);
                            task[tempID].setoutW(0);
                            outd[tempOUTnum].setDuration(task[tempID].getProcessTime());
                            outd[tempOUTnum].setEmpty(false);
                        }

                    }

                    // OUTD not empty too
                }
            }

        }

        // UPDATİNG time one by one

        double whichtime = task[0].getProcessTime();
        for(int i=0; i<taskNum ; i++){
            if(whichtime > task[i].getProcessTime() && task[i].getProcessTime()<123456789){
                whichtime = task[i].getProcessTime();
            }
        }

        for(int s=0; s<taskNum; s++){
            if(task[s].getcpuW() == 0){
                cpuTask++;
            }
        }
        for(int j=0; j<cpuNum; j++){
            if((whichtime > cpu[j].getDuration()) && cpu[j].getDuration() != 0 && (cpuTask != taskNum)){
                whichtime = cpu[j].getDuration();
            }
        }


        for(int s=0; s<taskNum; s++){
            if(task[s].getoutW() == 0){
                outTask++;
            }
        }

        //(taskNum-outTask) >= outNum
        for(int k=0; k<outNum; k++){
            if(whichtime > outd[k].getDuration() && outd[k].getDuration() != 0 && (taskNum-outTask) >= outNum){
                whichtime = outd[k].getDuration();
            }
        }
        if(whichtime < 123456789){
            eventTime = whichtime;
        }




        for(int i=0; i<cpuNum; i++){
            if((cpu[i].isEmpty() == false) && (cpu[i].getDuration() == eventTime)){
                cpu[i].setEmpty(true);
            }
        }

        for(int x=0; x<outNum; x++){
            if(outd[x].isEmpty() == false && (outd[x].getDuration() == eventTime)){
                outd[x].setEmpty(true);
            }
        }


        double temptasktime=task[0].getProcessTime();
        for(int op=0; op<taskNum; op++){
            if(temptasktime > task[op].getProcessTime()){
                temptasktime = task[op].getProcessTime();
            }
        }
        for(int jk =0; jk<cpuNum; jk++){
            if(cpu[jk].isEmpty() == true && whichtime <temptasktime && (cpuTask != taskNum)){
                cpu[jk].setDuration(temptasktime);
            }
        }
        for(int u=0; u<outNum; u++){
            if(outd[u].isEmpty() == true && whichtime <temptasktime && (taskNum-outTask) >= outNum){
                outd[u].setDuration(temptasktime);
            }
        }







        for(int i=0; i<taskNum; i++){               // Terminating WHİLE LOOP! Means all work done

            if((task[i].getcpuW() == 0) && (task[i].getoutW() ==0) && task[i].getProcessTime() <123456789 ){
                count++;
                task[i].setFinish(task[i].getProcessTime());
                task[i].setProcessTime(1234567890);

                //     i=taskNum;
            }
        }
        /*
        cout << "pq size : " << pq.size() << "   "  << "queue size : " << queue.size() << endl;
        cout << "CPU EMpty or not : " << cpu[0].isEmpty() << " --2nd-:  " << cpu[1].isEmpty() << endl;
        cout << "CPU DURATİON(0): " << cpu[0].getDuration() << " --2nd-:  " << cpu[1].getDuration() << endl;
        cout << "Outdevice EMpty or not : " << outd[0].isEmpty() << " --2nd-:  " << outd[1].isEmpty() << endl;
        cout << "OUTD DURATİON(0) : " << outd[0].getDuration() << " --2nd-:  " << outd[1].getDuration() << endl;
        cout << "-----------" << endl;
        cout << task[0].getcpuW() << "   " << task[0].getoutW() <<  endl;
        cout << task[1].getcpuW()  << "   " << task[1].getoutW() <<  endl;
        cout << task[2].getcpuW() << "   " << task[2].getoutW() <<  endl;
        cout << task[3].getcpuW() << "   " << task[3].getoutW() <<  endl;
        cout << task[4].getcpuW() << "   " << task[4].getoutW() <<  endl;
        cout << task[5].getcpuW() << "   " << task[5].getoutW() <<  endl;
        cout << task[6].getcpuW() << "   " << task[6].getoutW() <<  endl;
        cout << task[7].getcpuW()  << "   " << task[7].getoutW() <<  endl;
        cout << task[8].getcpuW()  << "   " << task[8].getoutW() <<  endl;
        cout << task[9].getcpuW()  << "   " << task[9].getoutW() <<  endl;
        cout << "-------------" << endl;
        cout <<"eventTime: " << eventTime  << endl;
        cout << "------------"  << endl;
        cout << " process time  :  " << task[0].getProcessTime() << "  "
             << task[1].getProcessTime() << "  " << task[2].getProcessTime() << "  "
             << task[3].getProcessTime() << "  " << task[4].getProcessTime() << "  " << task[5].getProcessTime() <<
             "  " << task[6].getProcessTime() << "  " << task[7].getProcessTime() << "  "
             << task[8].getProcessTime() <<
             "  " << task[9].getProcessTime() << endl;
        cout << " ---------- "<< endl;  */

        for(int i =0 ; i<taskNum; i++) {
            if (count == taskNum) {
                timeSpent += task[i].getFinish() - task[i].getArrival();
            }
        }
        greatestActivePQ=cpu[0].getDuration();

        for(int lo=0; lo<cpuNum ; lo++){
            if(greatestActivePQ < cpu[lo].getDuration()){
                greatestActivePQ = cpu[lo].getDuration();
                idpq = cpu[lo].getID();
            }
        }

        greatestActiveQueue = outd[0].getDuration();
        for(int lo=0; lo<outNum ; lo++){
            if(greatestActiveQueue < outd[lo].getDuration()){
                greatestActiveQueue = outd[lo].getDuration();
                idqueue = outd[lo].getID();
            }
        }


        if(count == taskNum){
            isTrue = false;
        }



    }


    for(int iop= 0 ;iop <taskNum; iop++){
        if(longestWait< task[iop].getQueueTime()){
            longestWait = task[iop].getQueueTime();
        }
    }

    /*  double
      for(int ii=0; ii<outNum; ii++){
          if(outd[ii].isEmpty() == false){
              outd[ii].getDuration() - eventTime;
          }
      }*/

    cout << maxLengthPQ << endl;
    cout << maxLengthQ << endl;
    cout << idpq << endl;
    cout << idqueue << endl;
    cout <<fixed<<setprecision(6);
    cout << queueTime/taskNum << endl;
    cout << longestWait << endl;
    cout << timeSpent/taskNum << endl;



    return EXIT_SUCCESS;
}

