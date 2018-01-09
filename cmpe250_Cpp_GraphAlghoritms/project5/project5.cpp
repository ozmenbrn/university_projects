/*
Student Name:Barın Özmen
Student Number:2012400045
Project Number:5
Operating System:Linux/Ubuntu
Compile Status:Yes
Program Status:Working well!
Notes: Anything you want to say about your code that will be helpful in the grading process.

*/

#include <fstream>
#include <iostream>
#include <vector>
#include <float.h>
#include <algorithm>
#include <queue>


using namespace std;


struct Edge {
    int v1;
    int v2;
    int weight;
    Edge(int v1, int v2, int weight) {
        this->v1 = v1;
        this->v2 = v2;
        this->weight = weight;
    }
};

struct Graph{
    vector<Edge> adj;
    int f;
    int g;
    int h;
    int id;
};


bool operator<(const Graph& g1, const Graph& g2) {
    return g1.f > g2.f;
};




int verticiesNum, edgeNum, source, goal, temp ,v1 ,v2 ,weight;
priority_queue<Graph> pq;
vector<Graph> graph;



int A_Star(int source, int goal, bool isVisited[]){
    bool isTrue = true;
    pq.push(graph[source]);
    graph[source].g = 0;
    graph[source].f = graph[source].g + graph[source].h;
    int current;
    int tempGScore;
    int result = -1;
    isVisited[source]=true;

    while(!pq.empty() && isTrue){
        current = pq.top().id;
        isVisited[graph[current].id] = true;
        if(current == goal){
            result = graph[current].f - graph[current].h;
            isTrue = false;
        }
        pq.pop();

        for(int k=0; k<graph[current].adj.size(); k++){
            tempGScore = graph[current].g + graph[current].adj[k].weight;
            if(graph[current].adj[k].v1 == graph[current].id){
                if(!isVisited[graph[current].adj[k].v2]){
                    pq.push(graph[graph[current].adj[k].v2]);
                    isVisited[graph[current].adj[k].v2] = true;

                }
                graph[graph[current].adj[k].v2].g = tempGScore;
                graph[graph[current].adj[k].v2].f = graph[graph[current].adj[k].v2].g +graph[graph[current].adj[k].v2].h;
            }
            if(graph[current].adj[k].v2 == graph[current].id){
                if(!isVisited[graph[current].adj[k].v1]){
                    pq.push(graph[graph[current].adj[k].v1]);
                    isVisited[graph[current].adj[k].v1] = true;

                }
                graph[graph[current].adj[k].v1].g = tempGScore;
                graph[graph[current].adj[k].v1].f = graph[graph[current].adj[k].v1].g +graph[graph[current].adj[k].v1].h;
            }
        }
    }
    return result;
}



int main (int argc, char* argv[]) {
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w" , stdout);


    cin>> verticiesNum >> edgeNum;
    bool isVisited[verticiesNum];
    int heuristic[verticiesNum];

    for(int i=0; i<verticiesNum; i++){
        heuristic[i] = 0;
    }

    for(int i=0; i<verticiesNum; i++){
        cin >> temp;
        heuristic[i] = temp;
        Graph v;
        graph.push_back(v);
        isVisited[i] = false;
    }

    for(int i=0; i<edgeNum; i++){
        cin >> v1 >> v2 >> weight;
        graph[v1].adj.push_back(Edge(v1,v2,weight));
        graph[v2].adj.push_back(Edge(v2,v1,weight));

        if(i<verticiesNum){
            graph[i].h = heuristic[i];
            graph[i].id = i;
            graph[i].g = INT32_MAX;
            graph[i].f = INT32_MAX;
        }
    }


    cin >> source >> goal;

    cout << A_Star(source, goal, isVisited) << endl;

    return EXIT_SUCCESS;
}