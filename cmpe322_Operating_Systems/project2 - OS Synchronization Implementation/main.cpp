/*
author: Barın Özmen
Student Number: 2012400045
Project Number: 2
*/

#include <iostream>
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

static int const timeQuantum = 100;

/*
 * Sturct for Processes to keep their data and adjust them.
 */
struct Process{

private:
    int execTimeLeft, arrivalTime, execStartTime, execProgressTime, execTime, processId;
    queue<int> inst_time;
    queue<string> inst_name;
    string id;

public:
    Process(string id ,int execTimeLeft, int arrivalTime){
        this->id = id;
        this->execTimeLeft= execTimeLeft;
        this->arrivalTime = arrivalTime;
        this->execProgressTime = 0;
        this->execTime = 0;
        this->processId = atoi(id.substr(1,2).c_str()) - 1;
    }

    string getID() const{
        return id;
    }

    int getProcessId() const {
        return processId;
    }

    int getArrivalTime() const{
        return arrivalTime;
    }

    int getInstructionTime() const{
        return inst_time.front();
    }

    void pushInstructionName(string name){
        inst_name.push(name);
    }

    void popInstructionName(){
        inst_name.pop();
    }

    string getInstructionName(){
        return inst_name.front();
    }

    void pushInstruction(int time){
        inst_time.push(time);
    }

    int getSizeInst(){
        return inst_time.size();
    }
    
    int getSizeInstName(){
        return inst_name.size();
    }


    void popInstruction(){
        inst_time.pop();
    }

    int getExecProcessTime() const{
        return execProgressTime;
    }

    int setExecProcessTime(int execProgressTime){
        this->execProgressTime = execProgressTime;
    }

    int getExecStartTime() const{
        return execStartTime;
    }

    int setExecStartTime(int execStartTime){
        this->execStartTime = execStartTime;
    }

    int getExecTimeLeft() const{
        return execTimeLeft;
    }

    int setExecTimeLeft(int execTimeLeft){
        this->execTimeLeft = execTimeLeft;
    }

    int getExecTime()const {
        return execTime;
    }

    int setExecTime(int execTime){
        this->execTime = execTime;
    }

};

string to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

struct Semaphore{

private:
    int id, lockedBy;
    bool isUsed, isLocked;
    string result;
    queue<Process> waitQueue;

public:
    Semaphore(int id){
        this->id = id;
        this->isUsed = false;
        this->isLocked = false;
        this->lockedBy = -1;
        this->result = "";
    }

    int getId(){
        return id;
    }

    void pushResult(string line){
        this->result.append(line);
        this->result.append("\n");
    }

    string getResult() const {
        return result;
    }

    int getLockedBy() const {
        return lockedBy;
    }

    void setLockedBy(int lockedBy){
        this->lockedBy = lockedBy;
    }

    void setIsLocked(bool isLocked){
        this->isLocked = isLocked;
    }

    bool getIsLocked() const {
        return isLocked;
    }

    void setIsUsed(bool isUsed){
        this->isUsed = isUsed;
    }

    bool getIsUsed() const {
        return isUsed;
    }

    void pushProcess(Process process){
        waitQueue.push(process);
    }

    Process getProcess(){
        return waitQueue.front();
    }

    int getSizeQueue(){
        return waitQueue.size();
    }

    void popProcess(){
        waitQueue.pop();
    }
};


int main(int argc, char* argv[]) {

    int programTime = 0;    //program time counter
    //initialized variables, their functions are as it can be understood from their name. cpuTaskIndex is connected with the process id, cpuTaskIndex = processId - 1 in the list
    int execTime, cpuTaskIndex = -1, arrivalTime, sumExecTime=0, count=0, remainingTotalExecTime = 0, listSize = 0, tempTime = 0, tempSemId, countFail = 0;
    string temp, code, output = "", tempString = "";     //temporary string variables.
    string tempInstName;    //temporary string variables.
    //initialized boolean variables, their functions are as it can be understood from their name.
    bool isReadDefinition = false;
    bool isCpuFree = true;
    bool isProgramFinished = false;
    bool isReadyQueueChange = false;
    bool isArrived = false;
    bool isInstructionExit = false;
    bool isSemaphore = false;

    vector<string> definition;  //to read definition.txt
    vector<Process> process;    //processes to be created
    vector<Semaphore> semaphore;  //to add semeporhes to the class semaphore
    queue<string> queueList;    //temp queue list to help ready queue
    queue<Process> tempSemaphore; //to keep semaphore queues when it is changed.
    queue<Process> readyQueue;  //Ready Queue
    queue<string> tempList;    //temp list to help instruction list
    queue<string> instList; //instruction list
    queue<string> instNameList; //instruction names list to determine semephore.

    ifstream cin(argv[1]);     //open file and takes 1 argument to read
    ofstream cout("output.txt");    //open output.txt as cout

    cin >> temp;
    definition.push_back(temp);

    //reads inputs from the definition.txt file word by word
    while(!isReadDefinition){

        cin >> temp;
        definition.push_back(temp);

        //If the two inputs are same send interrupt by boolean function. Because of that cin reads the last element for ever unless it is interrupted
        if(definition[definition.size()-1] == definition[definition.size()-2]){
            isReadDefinition = true;
        }

    }

    for(int i = 0; i < 10; i++){
        semaphore.push_back(Semaphore(i));  //create semaphore objets for each semaphore.
    }

    definition.pop_back();  //Get rid of last repeated element.

    // This loop is for check each elements of the definition list and create a Process if some conditions are true
    for(int i = 1 ; i <= definition.size(); i++){

        //That means there are enough elements to create a Process (id, codeSegment, arrivalTime)
        if(i%3 == 0){

            code = definition[i-2];  //Get the code file as a string

            ifstream cin(code.c_str());     //change cin from #codes

            //While loop for getting the execution times of instructions and added to Total execution time for a process to be finished.
            do{
                count++;
                cin >> temp;
                if(count%2 == 0){
                    sumExecTime += atoi(temp.c_str());
                    instList.push(temp);
                }
                else{
                    instNameList.push(temp);   //instruction names
                }

            }while(temp != "exit");

            cin >> temp;
            sumExecTime += atoi(temp.c_str());
            instList.push(temp);
            //instNameList.push(temp);
            execTime = sumExecTime;

            arrivalTime = atoi(definition[i-1].c_str());
            process.push_back(Process(definition[i-3], execTime , arrivalTime));

            //Instructions execution times are sent to process's queue list
            for(int j = 0; j <= count/2 ; j++){
                process[i/3-1].pushInstruction(atoi(instList.front().c_str()));
                process[i/3-1].pushInstructionName(instNameList.front());
                instNameList.pop();
                instList.pop();
            }
            process[i/3-1].pushInstruction(5);      //To avoid segmantation error. keep dummy
            process[i/3-1].pushInstructionName("dummy");    //dummy instruction
            sumExecTime = 0;
            count = 0;
        }

    }


    for(int i = 0; i < process.size() ; i++){
        remainingTotalExecTime += process[i].getExecTimeLeft();
    }
    /*
     * This is where all the algorithm takes place.
     */
    while(!isProgramFinished){

        //If process arrives and cpu free, then send it to cpu directly. Else, send it to ready queue..
        for(int i = 0; i < process.size() ; i++){
            if(programTime == process[i].getArrivalTime()){
                if(isCpuFree){
                    cpuTaskIndex = i;
                    process[i].setExecStartTime(programTime);
                    isCpuFree = false;
                    isReadyQueueChange = true;
                    isArrived = true;

                }
                else{
                    readyQueue.push(process[i]);
                    queueList.push(process[i].getID());
                    isReadyQueueChange = true;
                    isArrived = true;
                }
            }
        }

        //Check, if cpu is free or not. If it is not free that means, process are being executed.
        if(!isCpuFree){

            if(process[cpuTaskIndex].getSizeInstName() > 0){
                tempInstName = process[cpuTaskIndex].getInstructionName();
            }

            if (process[cpuTaskIndex].getExecTimeLeft() <= timeQuantum) {

                tempTime = process[cpuTaskIndex].getExecProcessTime();

                if (programTime == tempTime + process[cpuTaskIndex].getExecStartTime()) {
                    if(process[cpuTaskIndex].getSizeInst() > 0){
                    tempTime = process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getInstructionTime();
                    process[cpuTaskIndex].setExecProcessTime(tempTime);
                    }

                    //Semaphroe part
                    if(process[cpuTaskIndex].getSizeInstName() > 0){
                        tempInstName = process[cpuTaskIndex].getInstructionName();
                    }

                    if(tempInstName.substr(0, 4) == "wait" || tempInstName.substr(0, 4) == "sign") {    // if the next instruction wait or sign
                        isSemaphore = true;
                        if(tempInstName.substr(0, 4) == "wait"){    //wait semaphore
                            tempSemId = atoi(tempInstName.substr(6, 7).c_str());
                            if(semaphore[tempSemId].getIsLocked() == false){
                                semaphore[tempSemId].setLockedBy(cpuTaskIndex);
                                semaphore[tempSemId].setIsLocked(true);
                            } else{
                                semaphore[tempSemId].pushProcess(process[cpuTaskIndex]);
                                process[cpuTaskIndex].setExecTimeLeft(process[cpuTaskIndex].getExecTimeLeft() -
                                                                      process[cpuTaskIndex].getExecProcessTime());
                                process[cpuTaskIndex].setExecProcessTime(0);
                                isCpuFree = true;
                                semaphore[tempSemId].setIsUsed(true);

                                tempString = to_string(programTime);
                                tempString.append("::HEAD-");
                                for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                    tempString.append(semaphore[tempSemId].getProcess().getID());
                                    tempString.append("-");
                                    tempSemaphore.push(semaphore[tempSemId].getProcess());
                                    semaphore[tempSemId].popProcess();
                                }
                                tempString.append("TAIL");
                                semaphore[tempSemId].pushResult(tempString);

                                for(int l = 0; l < tempSemaphore.size() ; l++){
                                    semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                    tempSemaphore.pop();
                                }

                                for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                    tempSemaphore.push(semaphore[tempSemId].getProcess());
                                    semaphore[tempSemId].popProcess();
                                }
                                for(int l = 0; l < tempSemaphore.size() ; l++){
                                    semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                    tempSemaphore.pop();
                                }
                            }

                        }
                        else{    //Sign semaphore. That means, either waitQueue is free or not
                            tempSemId = atoi(tempInstName.substr(6, 7).c_str());
                            if(semaphore[tempSemId].getSizeQueue() != 0){
                                int tempProId = semaphore[tempSemId].getProcess().getProcessId();

                                if(semaphore[tempSemId].getSizeQueue() > 1){
                                    semaphore[tempSemId].setIsLocked(true);
                                    readyQueue.push(process[tempProId]);
                                    queueList.push(process[tempProId].getID());
                                    isReadyQueueChange = true;
                                    semaphore[tempSemId].popProcess();
                                } else{
                                    semaphore[tempSemId].setIsLocked(true);
                                    readyQueue.push(process[tempProId]);
                                    queueList.push(process[tempProId].getID());
                                    isReadyQueueChange = true;
                                    semaphore[tempSemId].popProcess();
                                }

                                tempString = to_string(programTime);
                                tempString.append("::HEAD-");
                                for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                    tempString.append(semaphore[tempSemId].getProcess().getID());
                                    tempString.append("-");
                                    tempSemaphore.push(semaphore[tempSemId].getProcess());
                                    semaphore[tempSemId].popProcess();
                                    countFail++;
                                }
                                if(countFail > 0){
                                    tempString.append("TAIL");
                                }
                                else{
                                    tempString.append("-TAIL");
                                }
                                countFail = 0;
                                semaphore[tempSemId].pushResult(tempString);

                                for(int l = 0; l < tempSemaphore.size() ; l++){
                                    semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                    tempSemaphore.pop();
                                }
                                for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                    tempSemaphore.push(semaphore[tempSemId].getProcess());
                                    semaphore[tempSemId].popProcess();
                                }
                                for(int l = 0; l < tempSemaphore.size() ; l++){
                                    semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                    tempSemaphore.pop();
                                }

                            }
                            else{
                                semaphore[tempSemId].setIsLocked(false);
                                isReadyQueueChange = true;
                            }
                        }
                    }

                    if(process[cpuTaskIndex].getSizeInst() > 1){
                        process[cpuTaskIndex].popInstruction();
                        process[cpuTaskIndex].popInstructionName();    
                    }
                    else{
                        process[cpuTaskIndex].popInstruction();
                        process[cpuTaskIndex].popInstructionName();
                        isCpuFree = true;
                    }


                }
                if (programTime == process[cpuTaskIndex].getExecStartTime() + process[cpuTaskIndex].getExecTimeLeft() && process[cpuTaskIndex].getSizeInst() < 2) {
                    isCpuFree = true;
                    process[cpuTaskIndex].setExecTimeLeft(0);

                }
            } else {    //If not, the process must be sent to the ready queue again.

                //While the time quantum is not passed, take the instructions one by one.
                if (programTime < process[cpuTaskIndex].getExecStartTime() + timeQuantum) {
                    tempTime = process[cpuTaskIndex].getExecProcessTime();

                    if (programTime == tempTime + process[cpuTaskIndex].getExecStartTime()) {
                        if(process[cpuTaskIndex].getSizeInst() > 0){
                            tempTime = process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getInstructionTime();
                            process[cpuTaskIndex].setExecProcessTime(tempTime);
                        }

                        if(process[cpuTaskIndex].getSizeInstName() > 0){
                            tempInstName = process[cpuTaskIndex].getInstructionName();
                        }

                        if(tempInstName.substr(0, 4) == "wait" || tempInstName.substr(0, 4) == "sign") {    //semaphore part
                            isSemaphore = true;
                            if(tempInstName.substr(0, 4) == "wait"){    //semapahore is waiting for critical region
                                tempSemId = atoi(tempInstName.substr(6, 7).c_str());
                                if(semaphore[tempSemId].getIsLocked() == false){
                                    semaphore[tempSemId].setLockedBy(cpuTaskIndex);
                                    semaphore[tempSemId].setIsLocked(true);
                                }
                                else{
                                    semaphore[tempSemId].pushProcess(process[cpuTaskIndex]);
                                    process[cpuTaskIndex].setExecTimeLeft(process[cpuTaskIndex].getExecTimeLeft() -
                                                                          process[cpuTaskIndex].getExecProcessTime());
                                    process[cpuTaskIndex].setExecProcessTime(0);
                                    isCpuFree = true;
                                    semaphore[tempSemId].setIsUsed(true);

                                    tempString = to_string(programTime);
                                    tempString.append("::HEAD-");
                                    for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                        tempString.append(semaphore[tempSemId].getProcess().getID());
                                        tempString.append("-");
                                        tempSemaphore.push(semaphore[tempSemId].getProcess());
                                        semaphore[tempSemId].popProcess();
                                    }
                                    tempString.append("TAIL");
                                    semaphore[tempSemId].pushResult(tempString);

                                    for(int l = 0; l < tempSemaphore.size() ; l++){
                                        semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                        tempSemaphore.pop();
                                    }
                                    for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                        tempSemaphore.push(semaphore[tempSemId].getProcess());
                                        semaphore[tempSemId].popProcess();
                                    }
                                    for(int l = 0; l < tempSemaphore.size() ; l++){
                                        semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                        tempSemaphore.pop();
                                    }

                                }

                            } else{   //Sign semaphore. That means, either waitQueue is free or not

                                tempSemId = atoi(tempInstName.substr(6, 7).c_str());
                                if(semaphore[tempSemId].getSizeQueue() != 0){

                                    int tempProId = semaphore[tempSemId].getProcess().getProcessId();
                                    if(semaphore[tempSemId].getSizeQueue() > 1){

                                        semaphore[tempSemId].setIsLocked(true);
                                        readyQueue.push(process[tempProId]);
                                        queueList.push(process[tempProId].getID());
                                        isReadyQueueChange = true;
                                        semaphore[tempSemId].popProcess();

                                    }
                                    else{

                                        semaphore[tempSemId].setIsLocked(true);
                                        readyQueue.push(process[tempProId]);
                                        queueList.push(process[tempProId].getID());
                                        isReadyQueueChange = true;
                                        semaphore[tempSemId].popProcess();
                                    }

                                    tempString = to_string(programTime);
                                    tempString.append("::HEAD-");
                                    for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                        tempString.append(semaphore[tempSemId].getProcess().getID());
                                        tempString.append("-");
                                        tempSemaphore.push(semaphore[tempSemId].getProcess());
                                        semaphore[tempSemId].popProcess();
                                        countFail++;
                                    }
                                    if(countFail > 0){
                                        tempString.append("TAIL");
                                    }
                                    else{
                                        tempString.append("-TAIL");
                                    }
                                    semaphore[tempSemId].pushResult(tempString);
                                    countFail = 0;

                                    for(int l = 0; l < tempSemaphore.size() ; l++){
                                        semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                        tempSemaphore.pop();
                                    }
                                    for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                        tempSemaphore.push(semaphore[tempSemId].getProcess());
                                        semaphore[tempSemId].popProcess();
                                    }
                                    for(int l = 0; l < tempSemaphore.size() ; l++){
                                        semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                        tempSemaphore.pop();
                                    }
                                }
                                else{
                                    semaphore[tempSemId].setIsLocked(false);
                                    isReadyQueueChange = true;
                                }
                            }

                        }

                        if(process[cpuTaskIndex].getSizeInst() > 1){
                            process[cpuTaskIndex].popInstruction();
                            process[cpuTaskIndex].popInstructionName();
                        }
                        else{
                            process[cpuTaskIndex].popInstruction();
                            process[cpuTaskIndex].popInstructionName();
                            isCpuFree = true;
                        }

                    }
                }
                //if time quantum is reached, then take the last instruction, wait it to be finished, then send it to ready queue.
                if (programTime >= process[cpuTaskIndex].getExecStartTime() + timeQuantum) {

                    if (programTime == process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getExecStartTime()) {

                        isCpuFree = true;
                        process[cpuTaskIndex].setExecTimeLeft(process[cpuTaskIndex].getExecTimeLeft() -
                                                              process[cpuTaskIndex].getExecProcessTime());
                        readyQueue.push(process[cpuTaskIndex]);
                        queueList.push(process[cpuTaskIndex].getID());
                        process[cpuTaskIndex].setExecProcessTime(0);
                        isReadyQueueChange = true;
                    }
                }
            }
  
        }

        //If cpu is free then, reassign a process to cpu to be executed.
        if(isCpuFree){
            if(readyQueue.size() != 0){
                for(int i = 0 ; i < process.size(); i++){
                    if(process[i].getID() == readyQueue.front().getID()){
                        cpuTaskIndex = i;
                        process[cpuTaskIndex].setExecStartTime(programTime);
                        isCpuFree = false;
                    }
                }
                readyQueue.pop();
                queueList.pop();
                isReadyQueueChange = true;

            }
        }

        /*
         * Copy paste of the first if part where cpu is not free. This one checks twice if process finishes, reassign cpu and process the missing part without missing +1 program time.
         */
        if(!isCpuFree){

            if(process[cpuTaskIndex].getSizeInstName() > 0){
                tempInstName = process[cpuTaskIndex].getInstructionName();
            }

            if (process[cpuTaskIndex].getExecTimeLeft() <= timeQuantum) {

                tempTime = process[cpuTaskIndex].getExecProcessTime();

                if (programTime == tempTime + process[cpuTaskIndex].getExecStartTime()) {
                    if(process[cpuTaskIndex].getSizeInst() > 0){
                        tempTime = process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getInstructionTime();
                        process[cpuTaskIndex].setExecProcessTime(tempTime);
                    }

                    //Semaphroe part
                    if(process[cpuTaskIndex].getSizeInstName() > 0){
                        tempInstName = process[cpuTaskIndex].getInstructionName();
                    }

                    if(tempInstName.substr(0, 4) == "wait" || tempInstName.substr(0, 4) == "sign") {    // if the next instruction wait or sign
                        isSemaphore = true;
                        if(tempInstName.substr(0, 4) == "wait"){    //wait semaphore
                            tempSemId = atoi(tempInstName.substr(6, 7).c_str());
                            if(semaphore[tempSemId].getIsLocked() == false){
                                semaphore[tempSemId].setLockedBy(cpuTaskIndex);
                                semaphore[tempSemId].setIsLocked(true);
                            } else{
                                semaphore[tempSemId].pushProcess(process[cpuTaskIndex]);
                                process[cpuTaskIndex].setExecTimeLeft(process[cpuTaskIndex].getExecTimeLeft() -
                                                                      process[cpuTaskIndex].getExecProcessTime());
                                process[cpuTaskIndex].setExecProcessTime(0);
                                isCpuFree = true;
                                semaphore[tempSemId].setIsUsed(true);

                                tempString = to_string(programTime);
                                tempString.append("::HEAD-");
                                for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                    tempString.append(semaphore[tempSemId].getProcess().getID());
                                    tempString.append("-");
                                    tempSemaphore.push(semaphore[tempSemId].getProcess());
                                    semaphore[tempSemId].popProcess();
                                }
                                tempString.append("TAIL");
                                semaphore[tempSemId].pushResult(tempString);

                                for(int l = 0; l < tempSemaphore.size() ; l++){
                                    semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                    tempSemaphore.pop();
                                }

                                for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                    tempSemaphore.push(semaphore[tempSemId].getProcess());
                                    semaphore[tempSemId].popProcess();
                                }
                                for(int l = 0; l < tempSemaphore.size() ; l++){
                                    semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                    tempSemaphore.pop();
                                }
                            }

                        }
                        else{    //Sign semaphore. That means, either waitQueue is free or not
                            tempSemId = atoi(tempInstName.substr(6, 7).c_str());
                            if(semaphore[tempSemId].getSizeQueue() != 0){
                                int tempProId = semaphore[tempSemId].getProcess().getProcessId();

                                if(semaphore[tempSemId].getSizeQueue() > 1){
                                    semaphore[tempSemId].setIsLocked(true);
                                    readyQueue.push(process[tempProId]);
                                    queueList.push(process[tempProId].getID());
                                    isReadyQueueChange = true;
                                    semaphore[tempSemId].popProcess();
                                } else{
                                    semaphore[tempSemId].setIsLocked(true);
                                    readyQueue.push(process[tempProId]);
                                    queueList.push(process[tempProId].getID());
                                    isReadyQueueChange = true;
                                    semaphore[tempSemId].popProcess();
                                }

                                tempString = to_string(programTime);
                                tempString.append("::HEAD-");
                                for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                    tempString.append(semaphore[tempSemId].getProcess().getID());
                                    tempString.append("-");
                                    tempSemaphore.push(semaphore[tempSemId].getProcess());
                                    semaphore[tempSemId].popProcess();
                                    countFail++;
                                }
                                if(countFail > 0){
                                    tempString.append("TAIL");
                                }
                                else{
                                    tempString.append("-TAIL");
                                }
                                countFail = 0;
                                semaphore[tempSemId].pushResult(tempString);

                                for(int l = 0; l < tempSemaphore.size() ; l++){
                                    semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                    tempSemaphore.pop();
                                }
                                for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                    tempSemaphore.push(semaphore[tempSemId].getProcess());
                                    semaphore[tempSemId].popProcess();
                                }
                                for(int l = 0; l < tempSemaphore.size() ; l++){
                                    semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                    tempSemaphore.pop();
                                }

                            }
                            else{
                                semaphore[tempSemId].setIsLocked(false);
                                isReadyQueueChange = true;
                            }
                        }
                    }

                    if(process[cpuTaskIndex].getSizeInst() > 1){
                        process[cpuTaskIndex].popInstruction();
                        process[cpuTaskIndex].popInstructionName();
                    }
                    else{
                        process[cpuTaskIndex].popInstruction();
                        process[cpuTaskIndex].popInstructionName();
                        isCpuFree = true;
                    }


                }
                if (programTime == process[cpuTaskIndex].getExecStartTime() + process[cpuTaskIndex].getExecTimeLeft() && process[cpuTaskIndex].getSizeInst() < 2) {
                    isCpuFree = true;
                    process[cpuTaskIndex].setExecTimeLeft(0);

                }
            } else {    //If not, the process must be sent to the ready queue again.

                //While the time quantum is not passed, take the instructions one by one.
                if (programTime < process[cpuTaskIndex].getExecStartTime() + timeQuantum) {
                    tempTime = process[cpuTaskIndex].getExecProcessTime();

                    if (programTime == tempTime + process[cpuTaskIndex].getExecStartTime()) {
                        if(process[cpuTaskIndex].getSizeInst() > 0){
                            tempTime = process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getInstructionTime();
                            process[cpuTaskIndex].setExecProcessTime(tempTime);
                        }

                        if(process[cpuTaskIndex].getSizeInstName() > 0){
                            tempInstName = process[cpuTaskIndex].getInstructionName();
                        }

                        if(tempInstName.substr(0, 4) == "wait" || tempInstName.substr(0, 4) == "sign") {    //semaphore part
                            isSemaphore = true;
                            if(tempInstName.substr(0, 4) == "wait"){    //semapahore is waiting for critical region
                                tempSemId = atoi(tempInstName.substr(6, 7).c_str());
                                if(semaphore[tempSemId].getIsLocked() == false){
                                    semaphore[tempSemId].setLockedBy(cpuTaskIndex);
                                    semaphore[tempSemId].setIsLocked(true);
                                }
                                else{
                                    semaphore[tempSemId].pushProcess(process[cpuTaskIndex]);
                                    process[cpuTaskIndex].setExecTimeLeft(process[cpuTaskIndex].getExecTimeLeft() -
                                                                          process[cpuTaskIndex].getExecProcessTime());
                                    process[cpuTaskIndex].setExecProcessTime(0);
                                    isCpuFree = true;
                                    semaphore[tempSemId].setIsUsed(true);

                                    tempString = to_string(programTime);
                                    tempString.append("::HEAD-");
                                    for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                        tempString.append(semaphore[tempSemId].getProcess().getID());
                                        tempString.append("-");
                                        tempSemaphore.push(semaphore[tempSemId].getProcess());
                                        semaphore[tempSemId].popProcess();
                                    }
                                    tempString.append("TAIL");
                                    semaphore[tempSemId].pushResult(tempString);

                                    for(int l = 0; l < tempSemaphore.size() ; l++){
                                        semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                        tempSemaphore.pop();
                                    }
                                    for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                        tempSemaphore.push(semaphore[tempSemId].getProcess());
                                        semaphore[tempSemId].popProcess();
                                    }
                                    for(int l = 0; l < tempSemaphore.size() ; l++){
                                        semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                        tempSemaphore.pop();
                                    }

                                }

                            } else{   //Sign semaphore. That means, either waitQueue is free or not

                                tempSemId = atoi(tempInstName.substr(6, 7).c_str());
                                if(semaphore[tempSemId].getSizeQueue() != 0){

                                    int tempProId = semaphore[tempSemId].getProcess().getProcessId();
                                    if(semaphore[tempSemId].getSizeQueue() > 1){

                                        semaphore[tempSemId].setIsLocked(true);
                                        readyQueue.push(process[tempProId]);
                                        queueList.push(process[tempProId].getID());
                                        isReadyQueueChange = true;
                                        semaphore[tempSemId].popProcess();

                                    }
                                    else{

                                        semaphore[tempSemId].setIsLocked(true);
                                        readyQueue.push(process[tempProId]);
                                        queueList.push(process[tempProId].getID());
                                        isReadyQueueChange = true;
                                        semaphore[tempSemId].popProcess();
                                    }

                                    tempString = to_string(programTime);
                                    tempString.append("::HEAD-");
                                    for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                        tempString.append(semaphore[tempSemId].getProcess().getID());
                                        tempString.append("-");
                                        tempSemaphore.push(semaphore[tempSemId].getProcess());
                                        semaphore[tempSemId].popProcess();
                                        countFail++;
                                    }
                                    if(countFail > 0){
                                        tempString.append("TAIL");
                                    }
                                    else{
                                        tempString.append("-TAIL");
                                    }
                                    semaphore[tempSemId].pushResult(tempString);
                                    countFail = 0;

                                    for(int l = 0; l < tempSemaphore.size() ; l++){
                                        semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                        tempSemaphore.pop();
                                    }
                                    for(int l = 0; l < semaphore[tempSemId].getSizeQueue() ; l++){
                                        tempSemaphore.push(semaphore[tempSemId].getProcess());
                                        semaphore[tempSemId].popProcess();
                                    }
                                    for(int l = 0; l < tempSemaphore.size() ; l++){
                                        semaphore[tempSemId].pushProcess(tempSemaphore.front());
                                        tempSemaphore.pop();
                                    }
                                }
                                else{
                                    semaphore[tempSemId].setIsLocked(false);
                                    isReadyQueueChange = true;
                                }
                            }

                        }

                        if(process[cpuTaskIndex].getSizeInst() > 1){
                            process[cpuTaskIndex].popInstruction();
                            process[cpuTaskIndex].popInstructionName();
                        }
                        else{
                            process[cpuTaskIndex].popInstruction();
                            process[cpuTaskIndex].popInstructionName();
                            isCpuFree = true;
                        }

                    }
                }
                //if time quantum is reached, then take the last instruction, wait it to be finished, then send it to ready queue.
                if (programTime >= process[cpuTaskIndex].getExecStartTime() + timeQuantum) {

                    if (programTime == process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getExecStartTime()) {

                        isCpuFree = true;
                        process[cpuTaskIndex].setExecTimeLeft(process[cpuTaskIndex].getExecTimeLeft() -
                                                              process[cpuTaskIndex].getExecProcessTime());
                        readyQueue.push(process[cpuTaskIndex]);
                        queueList.push(process[cpuTaskIndex].getID());
                        process[cpuTaskIndex].setExecProcessTime(0);
                        isReadyQueueChange = true;
                    }
                }
            }

        }

        countFail = 0;

        for(int i = 0; i < process.size() ; i++){
            remainingTotalExecTime += process[i].getExecTimeLeft();
        }

        //Prints the result when the ready queue change
        if(isReadyQueueChange){

            output = to_string(programTime);
            output.append("::HEAD-");

            if(cpuTaskIndex != -1){
                output.append(process[cpuTaskIndex].getID());
                output.append("-");
            }

            listSize = queueList.size();
            for(int i = 0; i < listSize; i++){
                output.append(queueList.front());
                output.append("-");
                tempList.push(queueList.front());
                queueList.pop();
            }
            for(int i = 0; i < listSize; i++){
                queueList.push(tempList.front());
                tempList.pop();
            }
            output.append("TAIL");
            cout << output << endl;
            isReadyQueueChange = false;
        }

        //Checks the program if it is finished or not
        if(remainingTotalExecTime == 0){
            isProgramFinished = true;
            cout << programTime;
            cout << "::HEAD--TAIL" << endl;

            for(int m = 0 ; m < 10; m++){
                if(semaphore[m].getIsUsed()){
                    tempString = "output_";
                    tempString.append(to_string(semaphore[m].getId()));
                    tempString.append(".txt");
                    ofstream cout(tempString.c_str());
                    cout << semaphore[m].getResult();
                }
            }
        }

        if(isSemaphore){
            programTime --;
            isSemaphore = false;
        }

        
        remainingTotalExecTime = 0;

        programTime++;
    }


    return 0;
}