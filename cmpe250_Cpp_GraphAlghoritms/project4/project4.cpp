/*
Student Name:Barın Özmen
Student Number:2012400045
Project Number:4
Operating System:Linux/ubuntu
Compile Status:YES
Program Status:Working
Notes: It compiles and gives correct output in every testcases. Yet it is too slow due to its complexity. There 2 for loops which makes it O(V^2) 

*/

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <queue>
#include <list>

using namespace std;

struct Edge;
int districtNum, cutNum, totalTime=0;
vector<vector<int> >GRAPH(100);
vector<int> markedNode;
vector<Edge> edges;
vector<Edge> inMST;


class DisjointSet{
public:
    int SIZE;
    int* arr;
    DisjointSet(int size){
        SIZE = size;
        arr = new int[SIZE];
        // Make each node a root.
        for (int i = 0; i < SIZE; ++i) {
          arr[i] = -1;
        }
    }
    int find(int set){
        if(arr[set] < 0){
            // Means this is a root
            return set;
        }else{
            // Call find with its root.
            return find(arr[set]);
        }
    }
    void Union(int set1,int set2, vector<int> markedNode){
        set1 = find(set1);
        set2 = find(set2);
        if(arr[set1] > arr[set2]){
            // First one is less deep, so make its root second.
            arr[set1] = set2;
        }else if(arr[set1] < arr[set2]){
            // Vice versa
            arr[set2] = set1;
        }else{
            // They are equal in depth. Set first one as second one's root. (Arbitrarily chosen)
            // And increase the depth.
            arr[set1]--;
            arr[set2] = set1;
        }
    }
};

struct Edge {
    int n1;
    int n2;
    int weight;
    Edge(int n1, int n2, int weight) {
        this->n1 = n1;
        this->n2 = n2;
        this->weight = weight;
    }
};    

bool operator<(const Edge& e1, const Edge& e2) {
    return e1.weight > e2.weight;
}


void kruskal(vector<int> markedNode) {
    sort(edges.begin(), edges.end());

    DisjointSet ds(districtNum);
    int c = 0;
    int count =0;
    while (c < districtNum - 1 && count < markedNode.size()-1) {
        int root1 = ds.find(edges[0].n1);
        int root2 = ds.find(edges[0].n2);


        if (root1 != root2) {
            for(long i= 0; i<markedNode.size(); i++){
                for(long j=0; j<markedNode.size(); j++){
                    if((root1 == markedNode[i] || root1 == ds.find(markedNode[i])) && (root2 == markedNode[j] || root2 == ds.find(markedNode[j])) && i !=j){
                        if(count < markedNode.size()-1){
                            totalTime += edges[0].weight;
                            count++;
                            i = markedNode.size() +1;                      // LONG INTEGER !!
                            j = markedNode.size() +1;                      // LONG INTIGER !!
                        }
                    }
                }
            }

            ds.Union(root1, root2, markedNode);
            c++;
            inMST.push_back(edges[0]);
        }
      //  cout <<"root1: "  << root1 << " root2: "  << root2 << " Edge is: " << edges[0].weight << endl;
        edges.erase(edges.begin());

    }

}

int main (int argc, char* argv[]) {
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w" , stdout);


    int temp ,n1, n2, weight, full =0;
    cin >> districtNum >> cutNum;

   
    
    for(int i=0; i<districtNum-1; i++) {
        cin >> n1 >> n2 >> weight;
        edges.push_back(Edge(n1, n2, weight));
    }


    for(int i=0; i<cutNum; i++){
        cin >> temp;
        markedNode.push_back(temp);
    }

    kruskal(markedNode) ;
    cout << totalTime << endl;

    return EXIT_SUCCESS;
}