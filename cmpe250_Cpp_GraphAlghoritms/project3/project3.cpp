/*
Student Name:Barın ÖZMEN
Student Number:2012400045
Project Number:3
Operating System:Linux
Compile Status:YES
Program Status:Works Fine!
Notes: Anything you want to say about your code that will be helpful in the grading process.

*/

#include <algorithm>
#include <queue>
#include <thread>
#include <iostream>
#include <iomanip>
#include <array>

using namespace std;

struct Process{
    double processTime, tempTime;

public:
    Process(double processTime){
        this->processTime = processTime;
    }

    double getProcessTime()const{
        return processTime;
    }

    double setProcessTime(double processTime){
        this->processTime = processTime;
    }


};


struct Neighbour{
    deque<int> queue;

public:
    Neighbour(int id){
        queue.push_front(id);
    }

    void addQueue(int a){
        queue.push_front(a);
    }

    int getQueue()const {
        return queue.front();

    }

    void popQueue(){
        queue.pop_front();
    }

    bool isEmpty()const {
        if(queue.size()>0){
            return false;
        }
        else{
            return true;
        }
    }

};



int main (int argc, char* argv[]) {
    freopen(argv[1], "r" , stdin);
 //   freopen(argv[2], "w" , stdout);

    double eventTime;
    double temp;
    int verticiesNum,edgeNum;
    cin >> verticiesNum >> edgeNum;
    double *maxProcessTime = new double[verticiesNum];
    int indegree[verticiesNum];
    vector<Process> process;
    vector<Neighbour> neighbour;
//    vector<MaxProcess> maxProcess;
    vector<vector<int>> graph(verticiesNum);
    //  double maxProcessTime =0;

    int from, to;


    for(int i=0; i<verticiesNum; i++){
        indegree[i]=0;
        cin >> temp;
        process.push_back(Process(temp));
        neighbour.push_back(Neighbour(-2));

    }


    for(int i=0; i<edgeNum; i++) {
        cin >> from >> to;
        graph[from].push_back(to);
        neighbour[to].addQueue(from);
        indegree[to]++;

    }


    queue<int> zero_list;
    int count =0;
    for(int i=0; i<verticiesNum; i++){
        if(indegree[i]==0){
            zero_list.push(i);
            count++;
        }
    }

    bool isTrue=true;

    double final = -1;
    int tempcount=0;
    int cnt = verticiesNum;

    cnt--;


    while(!zero_list.empty() && isTrue){
        tempcount++;
        int node = zero_list.front();
        zero_list.pop();


        for(int i=0; i<graph[node].size(); i++){
            indegree[graph[node][i]]--;
            if(indegree[graph[node][i]]==0){
                if(tempcount > count){
                    process[i].setProcessTime(process[i].getProcessTime() + process[node].getProcessTime());
                }
                zero_list.push(graph[node][i]);
            }

            if(eventTime < process[i].getProcessTime()){
                eventTime = process[i].getProcessTime();

            }
        }
        if(cnt>verticiesNum){
       //     cout << "Not a DAG!!!!" << endl;
            eventTime =0;
            isTrue = false;
        }
        else{
            if(zero_list.empty()){
                final = process[node].getProcessTime();
            }
        }
    }
    cout << fixed << setprecision(6);
    cout << eventTime + final << endl;


    return EXIT_SUCCESS;
}