#!/usr/bin/python

#import requests
import json
import inspect
import subprocess, os, sys, glob, boto3, time, pwd
from collections import deque
from collections import defaultdict

def main():
    os.chdir(os.getcwd()+"/"+sys.argv[1])

    firstsection = deque([])  # first part before the %%
    secondsection = deque([])  # second part after %%
    mapProg = []  # mapping for program to run topologically
    from_ = []  # dependency
    to_ = []  # available after dependency task is completed
    taskPosition = []  # get the id of the programs to run
    taskProg = []  # programs and their argumants

    for name in glob.glob('*.wdf'):     #To find the file ends with .wdf
        filename = name

    file = open(filename, 'r')  # to read from file
    isTrue = True  # to determine the postition of %%


    for line in file:
        if line == "%%\n":
            isTrue = False
        else:
            if isTrue == True:
                firstsection.append(line)
            else:
                secondsection.append(line)

    for i in range(len(firstsection)):
        taskPosition.append(ord(firstsection[i][:1]) - 65)  # Took all A,B,C as number to 0,1,2
        taskProg.append(firstsection[i][3:-1])  # ASCI to integer...

    for i in range(len(secondsection)):
        from_.append(ord(secondsection[i][:1]) - 65)
        to_.append(ord(secondsection[i][5:6]) - 65)


    class Graph:
        def __init__(self, task):
            self.graph = defaultdict(list)  # Constructor for python like C++ (where Self means this->)
            self.V = task  # Number of tasks

        def addEdge(self, _from, _to):  # function to add an edge to graph
            self.graph[_from].append(_to)

        def topologicalSort(self):  # The function to do Topological Sort.

            in_degree = [0] * (self.V)  # Create a vector to store indegrees of all vertices firstly 0

            # Traverse adjacency lists to fill indegrees of
            # vertices.  This step takes O(V+E) time
            for i in self.graph:
                for j in self.graph[i]:
                    in_degree[j] += 1

            # Create an queue and enqueue all vertices with indegree 0
            queue = []
            for i in range(self.V):
                if in_degree[i] == 0:
                    queue.append(i)

            # Starts from indegree 0's and take one by one until all the graph is finished
            while queue:

                # Extract front of queue and add it to topological order
                temp = queue.pop(0)
                mapProg.append(temp)

                # Go over all neighbouring nodes of dequeued node temp and decrease their in_degree by 1
                for i in self.graph[temp]:
                    in_degree[i] -= 1
                    # If in_degree becomes zero, add it to queue
                    if in_degree[i] == 0:
                        queue.append(i)


    graph = Graph(len(firstsection))

    for i in range(len(secondsection)):
        graph.addEdge(from_[i], to_[i])

    graph.topologicalSort()  # To call function

    #for loop to run programs one by one
    for i in range(len(mapProg)):
        os.system('chmod +x ./' + taskProg[mapProg[i]][:9])                     #Makes exacutable
        point = subprocess.Popen('./' +taskProg[mapProg[i]], shell=True)        #Open prog#
        point.wait()                                                            #Wait until it's done

main()

os.chdir('../')


def amazon_instance():
    ec2=boto3.resource('ec2')
    response = ec2.create_instances(ImageId='ami-a9d276c9',MinCount=1,MaxCount=1, KeyName='cmpe230KeyPair',InstanceType='t2.micro', SecurityGroups=['cmpe230sg'])
    print(response)
    print("querying for running instances")

    ids = []
    while(1):
        time.sleep(40)
        running=0
        instances=ec2.instances.filter(Filters=[{'Name':'instance-state-name','Values':['running']}])
        for instance in instances:
            print(instance.id,instance.instance_type,instance.public_ip_address,instance.public_dns_name)
            # subprocess.Popen('scp -i cmpe230KeyPair.pem wsfched.py ubuntu@' + instance.public_dns_name+':~/', shell=True)
            scpstring = r'scp -o StrictHostKeyChecking=no '
            subprocess.Popen(scpstring +'-i cmpe230KeyPair.pem -r myworkflow ubuntu@'+instance.public_dns_name+':~/', shell=True)
           # cmd = 'ls -l'
           # keyfile = '-i ./cmpe230KeyPair.pem '
           # sshstring = r'ssh -o StrictHostKeyChecking=no '
           # machine = 'ubuntu@' + instance.public_ip_address + ' '
           # command = sshstring + keyfile + machine + cmd
           # output = subprocess.check_output(command,shell=True)
           # print(output)
           # subprocess.Popen('./wsfched /myworkflow', shell=True)
           # main()
           # time.sleep(10)
           # subprocess.Popen('exit', shell=True)
            time.sleep(10)
            subprocess.Popen(scpstring+ '-i cmpe230KeyPair.pem ubuntu@' + instance.public_dns_name+':myworkflow/out.txt '+os.getcwd(), shell=True)
            print('out.txt is in Downloads')
            time.sleep(5)
            ids.append(instance.id)
            running=1
        if running:
            break

    time.sleep(20)
    ec2.instances.filter(InstanceIds=ids).terminate()

amazon_instance()









