/*
author: Barın Özmen
Student Number: 2012400045
Project Number: 3
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


struct Printer{
    
private:
    bool isUsed, isLocked;
    string result;
    queue<Process> waitQueue;
    int processesExecTime [4], execStartTime[4];

public:
    Printer(){
        this->isUsed = false;
        this->isLocked = false;
        this->result = "";
        for(int i = 0; i < 4; i++){
            this->processesExecTime[i] = 0;
        }
    }

    int getExecStartTime(int id) const{
        return execStartTime[id];
    }

    int setExecStartTime(int id, int execStartTime){
        this->execStartTime[id] = execStartTime;
    }

    void setProcessExecTime(int id, int time){
        this->processesExecTime[id] = time;
    }

    int getProcessExecTime(int id) const{
        return processesExecTime[id];
    }

    void setIsUsed(bool isUsed){
        this->isUsed = isUsed;
    }

    bool getIsUsed() const {
        return isUsed;
    }

    void setIsLocked(bool isLocked){
        this->isLocked = isLocked;
    }

    bool getIsLocked() const {
        return isLocked;
    }

    void pushResult(string line){
        this->result.append(line);
        this->result.append("\n");
    }

    string getResult() const {
        return result;
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

} printer0, printer1, hardDrive;    //2 printers + hardDrive.


int main(int argc, char* argv[]) {

    int programTime = 0;    //program time counter
    //initialized variables, their functions are as it can be understood from their name. cpuTaskIndex is connected with the process id, cpuTaskIndex = processId - 1 in the list
    int execTime, cpuTaskIndex = -1, arrivalTime, sumExecTime=0, count=0, remainingTotalExecTime = 0, listSize = 0, tempTime = 0, printerId, numExit=0;
    string temp, tempString, code, tempInstName, output = "", resultPrinter;     //temporary string variables.
    string cache [2], tempCache;
    int cacheTurn = 0, tempPSize;
    cache[0] = "-1";
    cache[1] = "-1"; 

    //initialized boolean variables, their functions are as it can be understood from their name.
    bool isReadDefinition = false;
    bool isCpuFree = true;
    bool isProgramFinished = false;
    bool isReadyQueueChange = false;
    bool isArrived = false;
    bool isInstructionExit = false;
    bool isPrinterUsed = false;
    bool isAtomic = false;
    bool cacheHit = false;

    vector<string> definition;  //to read definition.txt
    vector<Process> process;    //processes to be created
    queue<string> queueList;    //temp queue list to help ready queue
    queue<Process> readyQueue;  //Ready Queue
    queue<string> tempList;    //temp list to help instruction list
    queue<string> instList; //instruction list
    queue<string> instNameList;  //Names of instructions
    queue<Process> tempQueue;

    ifstream cin(argv[1]);     //open file and takes 1 argument to read
    freopen("output.txt", "w" , stdout);    //open output.txt as cout

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

        if(process[cpuTaskIndex].getSizeInstName() > 0){
            tempInstName = process[cpuTaskIndex].getInstructionName();
            tempTime = process[cpuTaskIndex].getExecProcessTime();
            if (programTime == tempTime + process[cpuTaskIndex].getExecStartTime()){
                if(tempInstName.substr(0, 5) == "readM"){
                    tempCache = tempInstName.substr(6);
                    for(int t = 0; t < 2; t++){
                        if(cache[t] == tempCache){
                            cacheHit = true;
                            if(t == cacheTurn){
                                cacheTurn = (cacheTurn+1)%2;
                            }
                        }
                    }
                    cacheTurn = (cacheTurn+1)%2;
                    cache[cacheTurn] = tempCache;
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
                    isAtomic = true;
                    if(process[cpuTaskIndex].getSizeInst() > 0){ 
                        if(cacheHit){
                            process[cpuTaskIndex].popInstruction();
                            process[cpuTaskIndex].popInstructionName();
                            cacheHit = false;
                        }
                        tempTime = process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getInstructionTime();
                        process[cpuTaskIndex].setExecProcessTime(tempTime);
                    }

                    //Semaphroe part
                    if(process[cpuTaskIndex].getSizeInstName() > 0){
                        tempInstName = process[cpuTaskIndex].getInstructionName();
                    }

                    if(tempInstName.substr(0, 5) == "dispM") {    // if the next instruction for io is printer
                        // DO it mannn!!!!
                        printerId = atoi(tempInstName.substr(6, 7).c_str());
                        if(printerId == 0){
                            printer0.setIsUsed(true);
                            if(printer0.getSizeQueue() == 0){
                                printer0.setExecStartTime(cpuTaskIndex, programTime);
                            }
                            printer0.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                            printer0.pushProcess(process[cpuTaskIndex]);
                            resultPrinter = to_string(programTime);
                            resultPrinter.append("::HEAD-");
                            tempPSize = printer0.getSizeQueue();
                            for(int y = 0; y < tempPSize; y++){
                                resultPrinter.append(printer0.getProcess().getID());
                                tempQueue.push(printer0.getProcess());
                                printer0.popProcess();
                                resultPrinter.append("-");    
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                printer0.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                tempQueue.push(printer0.getProcess());
                                printer0.popProcess();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                printer0.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }    
                            resultPrinter.append("TAIL");
                            printer0.pushResult(resultPrinter);
                        }
                        else{
                            printer1.setIsUsed(true);
                            if(printer1.getSizeQueue() == 0){
                                printer1.setExecStartTime(cpuTaskIndex, programTime);
                            }
                            printer1.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                            printer1.pushProcess(process[cpuTaskIndex]);
                            resultPrinter = to_string(programTime);
                            resultPrinter.append("::HEAD-");
                            tempPSize = printer1.getSizeQueue();
                            for(int y = 0; y < tempPSize; y++){
                                resultPrinter.append(printer1.getProcess().getID());
                                tempQueue.push(printer1.getProcess());
                                printer1.popProcess();
                                resultPrinter.append("-");    
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                printer1.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                tempQueue.push(printer1.getProcess());
                                printer1.popProcess();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                printer1.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }                      
                            resultPrinter.append("TAIL");
                            printer1.pushResult(resultPrinter);
                        }
                        process[cpuTaskIndex].setExecProcessTime(0);
              
                        isCpuFree = true;

                    }

                    if(tempInstName.substr(0, 5) == "readM") {    // if the next instruction for io is printer
                        // DO it mannn!!!!
                        printerId = atoi(tempInstName.substr(6, 7).c_str());
                        hardDrive.setIsUsed(true);
                        if(hardDrive.getSizeQueue() == 0){
                            hardDrive.setExecStartTime(cpuTaskIndex, programTime);
                        }
                        hardDrive.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                        hardDrive.pushProcess(process[cpuTaskIndex]);
                        resultPrinter = to_string(programTime);
                        resultPrinter.append("::HEAD-");
                        tempPSize = hardDrive.getSizeQueue();
                        for(int y = 0; y < tempPSize; y++){
                            resultPrinter.append(hardDrive.getProcess().getID());
                            tempQueue.push(hardDrive.getProcess());
                            hardDrive.popProcess();
                            resultPrinter.append("-");    
                        }   
                        for(int y = 0; y < tempPSize; y++){
                            hardDrive.pushProcess(tempQueue.front());
                            tempQueue.pop();
                        }   
                        for(int y = 0; y < tempPSize; y++){
                            tempQueue.push(hardDrive.getProcess());
                            hardDrive.popProcess();
                        }   
                        for(int y = 0; y < tempPSize; y++){
                            hardDrive.pushProcess(tempQueue.front());
                            tempQueue.pop();
                        }    
                        resultPrinter.append("TAIL");
                        hardDrive.pushResult(resultPrinter);

                        process[cpuTaskIndex].setExecProcessTime(0);
                        isCpuFree = true;
                    }
                    

                    if(tempInstName == "dummy") {
                        numExit++;
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
                    isAtomic = true;
                    process[cpuTaskIndex].setExecTimeLeft(0);

                }
            } else {    //If not, the process must be sent to the ready queue again.

                //While the time quantum is not passed, take the instructions one by one.
                if (programTime < process[cpuTaskIndex].getExecStartTime() + timeQuantum) {
                    tempTime = process[cpuTaskIndex].getExecProcessTime();

                    if (programTime == tempTime + process[cpuTaskIndex].getExecStartTime()) {
                        isAtomic = true;
                        if(process[cpuTaskIndex].getSizeInst() > 0){
                            if(cacheHit){
                                process[cpuTaskIndex].popInstruction();
                                process[cpuTaskIndex].popInstructionName();
                                cacheHit = false;
                            }
                            tempTime = process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getInstructionTime();
                            process[cpuTaskIndex].setExecProcessTime(tempTime);
                        }

                        if(process[cpuTaskIndex].getSizeInstName() > 0){
                            tempInstName = process[cpuTaskIndex].getInstructionName();
                        }

                        if(tempInstName.substr(0, 5) == "dispM") {    // if the next instruction for io is printer
                        // DO it mannn!!!!
                            printerId = atoi(tempInstName.substr(6, 7).c_str());
                            if(printerId == 0){
                                printer0.setIsUsed(true);
                                if(printer0.getSizeQueue() == 0){
                                    printer0.setExecStartTime(cpuTaskIndex, programTime);
                                }
                                printer0.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                                printer0.pushProcess(process[cpuTaskIndex]);
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                tempPSize = printer0.getSizeQueue();
                                for(int y = 0; y < tempPSize; y++){
                                    resultPrinter.append(printer0.getProcess().getID());
                                    tempQueue.push(printer0.getProcess());
                                    printer0.popProcess();
                                    resultPrinter.append("-");    
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    printer0.pushProcess(tempQueue.front());
                                    tempQueue.pop();
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    tempQueue.push(printer0.getProcess());
                                    printer0.popProcess();
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    printer0.pushProcess(tempQueue.front());
                                    tempQueue.pop();
                                }    
                                resultPrinter.append("TAIL");
                                printer0.pushResult(resultPrinter);
                            }
                            else{
                                printer1.setIsUsed(true);
                                if(printer1.getSizeQueue() == 0){
                                    printer1.setExecStartTime(cpuTaskIndex, programTime);
                                }
                                printer1.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                                printer1.pushProcess(process[cpuTaskIndex]);
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                tempPSize = printer1.getSizeQueue();
                                for(int y = 0; y < tempPSize; y++){
                                    resultPrinter.append(printer1.getProcess().getID());
                                    tempQueue.push(printer1.getProcess());
                                    printer1.popProcess();
                                    resultPrinter.append("-");    
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    printer1.pushProcess(tempQueue.front());
                                    tempQueue.pop();
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    tempQueue.push(printer1.getProcess());
                                    printer1.popProcess();
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    printer1.pushProcess(tempQueue.front());
                                    tempQueue.pop();
                                }                      
                                resultPrinter.append("TAIL");
                                printer1.pushResult(resultPrinter);
                            }
                            process[cpuTaskIndex].setExecProcessTime(0);
                  
                            isCpuFree = true;

                        }

                        if(tempInstName.substr(0, 5) == "readM") {    // if the next instruction for io is printer
                        // DO it mannn!!!!
                            printerId = atoi(tempInstName.substr(6, 7).c_str());
                            hardDrive.setIsUsed(true);
                            if(hardDrive.getSizeQueue() == 0){
                                hardDrive.setExecStartTime(cpuTaskIndex, programTime);
                            }
                            hardDrive.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                            hardDrive.pushProcess(process[cpuTaskIndex]);
                            resultPrinter = to_string(programTime);
                            resultPrinter.append("::HEAD-");
                            tempPSize = hardDrive.getSizeQueue();
                            for(int y = 0; y < tempPSize; y++){
                                resultPrinter.append(hardDrive.getProcess().getID());
                                tempQueue.push(hardDrive.getProcess());
                                hardDrive.popProcess();
                                resultPrinter.append("-");    
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                hardDrive.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                tempQueue.push(hardDrive.getProcess());
                                hardDrive.popProcess();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                hardDrive.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }    
                            resultPrinter.append("TAIL");
                            hardDrive.pushResult(resultPrinter);

                            process[cpuTaskIndex].setExecProcessTime(0);
                            isCpuFree = true;
                        }

                        if(tempInstName == "dummy") {
                            numExit++;
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

                        isAtomic = true;
                        isCpuFree = true;
                        if(printer0.getSizeQueue() > 0 && isAtomic){
                            if(programTime >= printer0.getExecStartTime(printer0.getProcess().getProcessId()) + printer0.getProcessExecTime(printer0.getProcess().getProcessId())){
                                //cout << printer0.getProcess().getProcessId() << " ---- " << printer0.getProcess().getExecStartTime() <<  " ---- "  << printer0.getProcessExecTime(printer0.getProcess().getProcessId()) << endl;
                                readyQueue.push(printer0.getProcess());
                                queueList.push(printer0.getProcess().getID());
                                //printer0.getProcess().setExecTimeLeft(printer0.getProcess().getExecTimeLeft() - printer0.getProcessExecTime(printer0.getProcess().getProcessId()));
                                printer0.popProcess();
                                isReadyQueueChange = true;
                                programTime -= programTime%10;
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                
                                if(printer0.getSizeQueue() > 0){
                                    printer0.setExecStartTime(printer0.getProcess().getProcessId(), programTime);
                                    resultPrinter.append(printer0.getProcess().getID());
                                }
                                resultPrinter.append("-");             
                                resultPrinter.append("TAIL");
                                printer0.pushResult(resultPrinter);
                                
                            }
                        }

                        if(printer1.getSizeQueue() > 0 && isAtomic){
                            if(programTime >= printer1.getExecStartTime(printer1.getProcess().getProcessId()) + printer1.getProcessExecTime(printer1.getProcess().getProcessId())){
                                readyQueue.push(printer1.getProcess());
                                queueList.push(printer1.getProcess().getID());
                                //printer1.getProcess().setExecTimeLeft(printer1.getProcess().getExecTimeLeft() - printer1.getProcessExecTime(printer1.getProcess().getProcessId()));
                                printer1.popProcess();
                                isReadyQueueChange = true;
                                programTime -= programTime%10;
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                if(printer1.getSizeQueue() > 0){
                                    printer1.setExecStartTime(printer1.getProcess().getProcessId(), programTime);
                                    resultPrinter.append(printer1.getProcess().getID());
                                }
                                resultPrinter.append("-");             
                                resultPrinter.append("TAIL");
                                printer1.pushResult(resultPrinter);
                            }
                        }

                        if(hardDrive.getSizeQueue() > 0 && isAtomic){
                            if(programTime >= hardDrive.getExecStartTime(hardDrive.getProcess().getProcessId()) + hardDrive.getProcessExecTime(hardDrive.getProcess().getProcessId())){
                                //cout << printer0.getProcess().getProcessId() << " ---- " << printer0.getProcess().getExecStartTime() <<  " ---- "  << printer0.getProcessExecTime(printer0.getProcess().getProcessId()) << endl;
                                readyQueue.push(hardDrive.getProcess());
                                queueList.push(hardDrive.getProcess().getID());
                                //printer0.getProcess().setExecTimeLeft(printer0.getProcess().getExecTimeLeft() - printer0.getProcessExecTime(printer0.getProcess().getProcessId()));
                                hardDrive.popProcess();
                                isReadyQueueChange = true;
                                programTime -= programTime%10;
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                
                                if(hardDrive.getSizeQueue() > 0){
                                    hardDrive.setExecStartTime(hardDrive.getProcess().getProcessId(), programTime);
                                    resultPrinter.append(hardDrive.getProcess().getID());
                                }
                                resultPrinter.append("-");             
                                resultPrinter.append("TAIL");
                                hardDrive.pushResult(resultPrinter);
                                
                            }
                        }

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
                        //programTime -= 1;
                    }
                }

                readyQueue.pop();
                queueList.pop();

                isReadyQueueChange = true;

            }
        }

        /*s
         * Copy paste of the first if part where cpu is not free. This one checks twice if process finishes, reassign cpu and process the missing part without missing +1 program time.
         */
        if(!isCpuFree){

            if(process[cpuTaskIndex].getSizeInstName() > 0){
                tempInstName = process[cpuTaskIndex].getInstructionName();
            }

            if (process[cpuTaskIndex].getExecTimeLeft() <= timeQuantum) {

                tempTime = process[cpuTaskIndex].getExecProcessTime();

                if (programTime == tempTime + process[cpuTaskIndex].getExecStartTime()) {
                    isAtomic = true;
                    if(process[cpuTaskIndex].getSizeInst() > 0){
                        if(cacheHit){
                            process[cpuTaskIndex].popInstruction();
                            process[cpuTaskIndex].popInstructionName();
                            cacheHit = false;
                        }
                        tempTime = process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getInstructionTime();
                        process[cpuTaskIndex].setExecProcessTime(tempTime);
                    }

                    //Semaphroe part
                    if(process[cpuTaskIndex].getSizeInstName() > 0){
                        tempInstName = process[cpuTaskIndex].getInstructionName();
                    }

                    if(tempInstName.substr(0, 5) == "dispM") {    // if the next instruction for io is printer
                        // DO it mannn!!!!
                        printerId = atoi(tempInstName.substr(6, 7).c_str());
                        if(printerId == 0){
                            printer0.setIsUsed(true);
                            if(printer0.getSizeQueue() == 0){
                                printer0.setExecStartTime(cpuTaskIndex, programTime);
                            }
                            printer0.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                            printer0.pushProcess(process[cpuTaskIndex]);
                            resultPrinter = to_string(programTime);
                            resultPrinter.append("::HEAD-");
                            tempPSize = printer0.getSizeQueue();
                            for(int y = 0; y < tempPSize; y++){
                                resultPrinter.append(printer0.getProcess().getID());
                                tempQueue.push(printer0.getProcess());
                                printer0.popProcess();
                                resultPrinter.append("-");    
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                printer0.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                tempQueue.push(printer0.getProcess());
                                printer0.popProcess();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                printer0.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }    
                            resultPrinter.append("TAIL");
                            printer0.pushResult(resultPrinter);
                        }
                        else{
                            printer1.setIsUsed(true);
                            if(printer1.getSizeQueue() == 0){
                                printer1.setExecStartTime(cpuTaskIndex, programTime);
                            }
                            printer1.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                            printer1.pushProcess(process[cpuTaskIndex]);
                            resultPrinter = to_string(programTime);
                            resultPrinter.append("::HEAD-");
                            tempPSize = printer1.getSizeQueue();
                            for(int y = 0; y < tempPSize; y++){
                                resultPrinter.append(printer1.getProcess().getID());
                                tempQueue.push(printer1.getProcess());
                                printer1.popProcess();
                                resultPrinter.append("-");    
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                printer1.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                tempQueue.push(printer1.getProcess());
                                printer1.popProcess();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                printer1.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }                      
                            resultPrinter.append("TAIL");
                            printer1.pushResult(resultPrinter);
                        }
                        process[cpuTaskIndex].setExecProcessTime(0);
              
                        isCpuFree = true;

                    }

                    if(tempInstName.substr(0, 5) == "readM") {    // if the next instruction for io is printer
                        // DO it mannn!!!!
                        printerId = atoi(tempInstName.substr(6, 7).c_str());
                        hardDrive.setIsUsed(true);
                        if(hardDrive.getSizeQueue() == 0){
                            hardDrive.setExecStartTime(cpuTaskIndex, programTime);
                        }
                        hardDrive.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                        hardDrive.pushProcess(process[cpuTaskIndex]);
                        resultPrinter = to_string(programTime);
                        resultPrinter.append("::HEAD-");
                        tempPSize = hardDrive.getSizeQueue();
                        for(int y = 0; y < tempPSize; y++){
                            resultPrinter.append(hardDrive.getProcess().getID());
                            tempQueue.push(hardDrive.getProcess());
                            hardDrive.popProcess();
                            resultPrinter.append("-");    
                        }   
                        for(int y = 0; y < tempPSize; y++){
                            hardDrive.pushProcess(tempQueue.front());
                            tempQueue.pop();
                        }   
                        for(int y = 0; y < tempPSize; y++){
                            tempQueue.push(hardDrive.getProcess());
                            hardDrive.popProcess();
                        }   
                        for(int y = 0; y < tempPSize; y++){
                            hardDrive.pushProcess(tempQueue.front());
                            tempQueue.pop();
                        }    
                        resultPrinter.append("TAIL");
                        hardDrive.pushResult(resultPrinter);

                        process[cpuTaskIndex].setExecProcessTime(0);
                        isCpuFree = true;
                    }
                    

                    if(tempInstName == "dummy") {
                        numExit++;
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
                    isAtomic = true;
                    process[cpuTaskIndex].setExecTimeLeft(0);

                }
            } else {    //If not, the process must be sent to the ready queue again.

                //While the time quantum is not passed, take the instructions one by one.
                if (programTime < process[cpuTaskIndex].getExecStartTime() + timeQuantum) {
                    tempTime = process[cpuTaskIndex].getExecProcessTime();

                    if (programTime == tempTime + process[cpuTaskIndex].getExecStartTime()) {
                        isAtomic = true;
                        if(process[cpuTaskIndex].getSizeInst() > 0){
                            if(cacheHit){
                                process[cpuTaskIndex].popInstruction();
                                process[cpuTaskIndex].popInstructionName();
                                cacheHit = false;
                            }
                            tempTime = process[cpuTaskIndex].getExecProcessTime() + process[cpuTaskIndex].getInstructionTime();
                            process[cpuTaskIndex].setExecProcessTime(tempTime);
                        }

                        if(process[cpuTaskIndex].getSizeInstName() > 0){
                            tempInstName = process[cpuTaskIndex].getInstructionName();
                        }

                        if(tempInstName.substr(0, 5) == "dispM") {    // if the next instruction for io is printer
                        // DO it mannn!!!!
                            printerId = atoi(tempInstName.substr(6, 7).c_str());
                            if(printerId == 0){
                                printer0.setIsUsed(true);
                                if(printer0.getSizeQueue() == 0){
                                    printer0.setExecStartTime(cpuTaskIndex, programTime);
                                }
                                printer0.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                                printer0.pushProcess(process[cpuTaskIndex]);
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                tempPSize = printer0.getSizeQueue();
                                for(int y = 0; y < tempPSize; y++){
                                    resultPrinter.append(printer0.getProcess().getID());
                                    tempQueue.push(printer0.getProcess());
                                    printer0.popProcess();
                                    resultPrinter.append("-");    
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    printer0.pushProcess(tempQueue.front());
                                    tempQueue.pop();
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    tempQueue.push(printer0.getProcess());
                                    printer0.popProcess();
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    printer0.pushProcess(tempQueue.front());
                                    tempQueue.pop();
                                }    
                                resultPrinter.append("TAIL");
                                printer0.pushResult(resultPrinter);
                            }
                            else{
                                printer1.setIsUsed(true);
                                if(printer1.getSizeQueue() == 0){
                                    printer1.setExecStartTime(cpuTaskIndex, programTime);
                                }
                                printer1.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                                printer1.pushProcess(process[cpuTaskIndex]);
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                tempPSize = printer1.getSizeQueue();
                                for(int y = 0; y < tempPSize; y++){
                                    resultPrinter.append(printer1.getProcess().getID());
                                    tempQueue.push(printer1.getProcess());
                                    printer1.popProcess();
                                    resultPrinter.append("-");    
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    printer1.pushProcess(tempQueue.front());
                                    tempQueue.pop();
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    tempQueue.push(printer1.getProcess());
                                    printer1.popProcess();
                                }   
                                for(int y = 0; y < tempPSize; y++){
                                    printer1.pushProcess(tempQueue.front());
                                    tempQueue.pop();
                                }                      
                                resultPrinter.append("TAIL");
                                printer1.pushResult(resultPrinter);
                            }
                            process[cpuTaskIndex].setExecProcessTime(0);
                  
                            isCpuFree = true;

                        }

                        if(tempInstName.substr(0, 5) == "readM") {    // if the next instruction for io is printer
                        // DO it mannn!!!!
                            printerId = atoi(tempInstName.substr(6, 7).c_str());
                            hardDrive.setIsUsed(true);
                            if(hardDrive.getSizeQueue() == 0){
                                hardDrive.setExecStartTime(cpuTaskIndex, programTime);
                            }
                            hardDrive.setProcessExecTime(cpuTaskIndex ,process[cpuTaskIndex].getInstructionTime());
                            hardDrive.pushProcess(process[cpuTaskIndex]);
                            resultPrinter = to_string(programTime);
                            resultPrinter.append("::HEAD-");
                            tempPSize = hardDrive.getSizeQueue();
                            for(int y = 0; y < tempPSize; y++){
                                resultPrinter.append(hardDrive.getProcess().getID());
                                tempQueue.push(hardDrive.getProcess());
                                hardDrive.popProcess();
                                resultPrinter.append("-");    
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                hardDrive.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                tempQueue.push(hardDrive.getProcess());
                                hardDrive.popProcess();
                            }   
                            for(int y = 0; y < tempPSize; y++){
                                hardDrive.pushProcess(tempQueue.front());
                                tempQueue.pop();
                            }    
                            resultPrinter.append("TAIL");
                            hardDrive.pushResult(resultPrinter);

                            process[cpuTaskIndex].setExecProcessTime(0);
                            isCpuFree = true;
                        }

                        if(tempInstName == "dummy") {
                            numExit++;
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

                        isAtomic = true;
                        isCpuFree = true;
                        if(printer0.getSizeQueue() > 0 && isAtomic){
                            if(programTime >= printer0.getExecStartTime(printer0.getProcess().getProcessId()) + printer0.getProcessExecTime(printer0.getProcess().getProcessId())){
                                //cout << printer0.getProcess().getProcessId() << " ---- " << printer0.getProcess().getExecStartTime() <<  " ---- "  << printer0.getProcessExecTime(printer0.getProcess().getProcessId()) << endl;
                                readyQueue.push(printer0.getProcess());
                                queueList.push(printer0.getProcess().getID());
                                //printer0.getProcess().setExecTimeLeft(printer0.getProcess().getExecTimeLeft() - printer0.getProcessExecTime(printer0.getProcess().getProcessId()));
                                printer0.popProcess();
                                isReadyQueueChange = true;
                                programTime -= programTime%10;
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                
                                if(printer0.getSizeQueue() > 0){
                                    printer0.setExecStartTime(printer0.getProcess().getProcessId(), programTime);
                                    resultPrinter.append(printer0.getProcess().getID());
                                }
                                resultPrinter.append("-");             
                                resultPrinter.append("TAIL");
                                printer0.pushResult(resultPrinter);
                                
                            }
                        }

                        if(printer1.getSizeQueue() > 0 && isAtomic){
                            if(programTime >= printer1.getExecStartTime(printer1.getProcess().getProcessId()) + printer1.getProcessExecTime(printer1.getProcess().getProcessId())){
                                readyQueue.push(printer1.getProcess());
                                queueList.push(printer1.getProcess().getID());
                                //printer1.getProcess().setExecTimeLeft(printer1.getProcess().getExecTimeLeft() - printer1.getProcessExecTime(printer1.getProcess().getProcessId()));
                                printer1.popProcess();
                                isReadyQueueChange = true;
                                programTime -= programTime%10;
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                if(printer1.getSizeQueue() > 0){
                                    printer1.setExecStartTime(printer1.getProcess().getProcessId(), programTime);
                                    resultPrinter.append(printer1.getProcess().getID());
                                }
                                resultPrinter.append("-");             
                                resultPrinter.append("TAIL");
                                printer1.pushResult(resultPrinter);
                            }
                        }

                        if(hardDrive.getSizeQueue() > 0 && isAtomic){
                            if(programTime >= hardDrive.getExecStartTime(hardDrive.getProcess().getProcessId()) + hardDrive.getProcessExecTime(hardDrive.getProcess().getProcessId())){
                                //cout << printer0.getProcess().getProcessId() << " ---- " << printer0.getProcess().getExecStartTime() <<  " ---- "  << printer0.getProcessExecTime(printer0.getProcess().getProcessId()) << endl;
                                readyQueue.push(hardDrive.getProcess());
                                queueList.push(hardDrive.getProcess().getID());
                                //printer0.getProcess().setExecTimeLeft(printer0.getProcess().getExecTimeLeft() - printer0.getProcessExecTime(printer0.getProcess().getProcessId()));
                                hardDrive.popProcess();
                                isReadyQueueChange = true;
                                programTime -= programTime%10;
                                resultPrinter = to_string(programTime);
                                resultPrinter.append("::HEAD-");
                                
                                if(hardDrive.getSizeQueue() > 0){
                                    hardDrive.setExecStartTime(hardDrive.getProcess().getProcessId(), programTime);
                                    resultPrinter.append(hardDrive.getProcess().getID());
                                }
                                resultPrinter.append("-");             
                                resultPrinter.append("TAIL");
                                hardDrive.pushResult(resultPrinter);
                                
                            }
                        }

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

        if(programTime > 1600){
        //  cout << process[cpuTaskIndex].getID() << " ----- " << programTime << " ---- "  << process[cpuTaskIndex].getExecTimeLeft() << " --- " << process[cpuTaskIndex].getInstructionName() << endl;
        }

        for(int i = 0; i < process.size() ; i++){
            remainingTotalExecTime += process[i].getExecTimeLeft();
        }

        //Prints the result when the ready queue change
        if(isReadyQueueChange){

            if(programTime%10 > 5){
                programTime += 10 - (programTime%10);
            }
            else{
                programTime -= programTime%10;
            }
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
        if(remainingTotalExecTime == 0 || numExit == 4){
            isProgramFinished = true;
            programTime -= programTime%10;
            cout << programTime;
            cout << "::HEAD--TAIL" << endl;

            
            if(printer0.getIsUsed()){
                tempString = "output_10.txt";
                ofstream cout(tempString.c_str());
                cout << printer0.getResult();
            }
            if(printer1.getIsUsed()){
                tempString = "output_11.txt";
                ofstream cout(tempString.c_str());
                cout << printer1.getResult();
            }
            if(hardDrive.getIsUsed()){
                tempString = "output_12.txt";
                ofstream cout(tempString.c_str());
                cout << hardDrive.getResult();
            }
            
        }

        remainingTotalExecTime = 0;

        programTime++;

        if(printer0.getSizeQueue() > 0 && isAtomic){
            if(programTime >= printer0.getExecStartTime(printer0.getProcess().getProcessId()) + printer0.getProcessExecTime(printer0.getProcess().getProcessId())){
                //cout << printer0.getProcess().getProcessId() << " ---- " << printer0.getProcess().getExecStartTime() <<  " ---- "  << printer0.getProcessExecTime(printer0.getProcess().getProcessId()) << endl;
                readyQueue.push(printer0.getProcess());
                programTime--;
                queueList.push(printer0.getProcess().getID());
                //printer0.getProcess().setExecTimeLeft(printer0.getProcess().getExecTimeLeft() - printer0.getProcessExecTime(printer0.getProcess().getProcessId()));
                printer0.popProcess();
                isReadyQueueChange = true;
                resultPrinter = to_string(programTime);
                resultPrinter.append("::HEAD-");
                
                if(printer0.getSizeQueue() > 0){
                    printer0.setExecStartTime(printer0.getProcess().getProcessId(), programTime);
                    resultPrinter.append(printer0.getProcess().getID());
                }
                resultPrinter.append("-");             
                resultPrinter.append("TAIL");
                printer0.pushResult(resultPrinter);
            }
        }

        if(printer1.getSizeQueue() > 0 && isAtomic){
            if(programTime >= printer1.getExecStartTime(printer1.getProcess().getProcessId()) + printer1.getProcessExecTime(printer1.getProcess().getProcessId())){
                readyQueue.push(printer1.getProcess());
                queueList.push(printer1.getProcess().getID());
                programTime--;
                //printer1.getProcess().setExecTimeLeft(printer1.getProcess().getExecTimeLeft() - printer1.getProcessExecTime(printer1.getProcess().getProcessId()));
                printer1.popProcess();
                isReadyQueueChange = true;
                resultPrinter = to_string(programTime);
                resultPrinter.append("::HEAD-");
                if(printer1.getSizeQueue() > 0){
                    printer1.setExecStartTime(printer1.getProcess().getProcessId(), programTime);
                    resultPrinter.append(printer1.getProcess().getID());
                }
                resultPrinter.append("-");             
                resultPrinter.append("TAIL");
                printer1.pushResult(resultPrinter);
            }
        }

        if(hardDrive.getSizeQueue() > 0 && isAtomic){
            if(programTime >= hardDrive.getExecStartTime(hardDrive.getProcess().getProcessId()) + hardDrive.getProcessExecTime(hardDrive.getProcess().getProcessId())){
                //cout << printer0.getProcess().getProcessId() << " ---- " << printer0.getProcess().getExecStartTime() <<  " ---- "  << printer0.getProcessExecTime(printer0.getProcess().getProcessId()) << endl;
                readyQueue.push(hardDrive.getProcess());
                programTime--;
                queueList.push(hardDrive.getProcess().getID());
                //printer0.getProcess().setExecTimeLeft(printer0.getProcess().getExecTimeLeft() - printer0.getProcessExecTime(printer0.getProcess().getProcessId()));
                hardDrive.popProcess();
                isReadyQueueChange = true;
                resultPrinter = to_string(programTime);
                resultPrinter.append("::HEAD-");
                
                if(hardDrive.getSizeQueue() > 0){
                    hardDrive.setExecStartTime(hardDrive.getProcess().getProcessId(), programTime);
                    resultPrinter.append(hardDrive.getProcess().getID());
                }
                resultPrinter.append("-");             
                resultPrinter.append("TAIL");
                hardDrive.pushResult(resultPrinter);
            }
        }

        cacheHit = false;
        isAtomic = false;
        
        if(isPrinterUsed){
            programTime--;
            isPrinterUsed = false;
        }

    }


    return 0;
}