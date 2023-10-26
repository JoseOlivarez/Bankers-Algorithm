#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <queue>
#include <fstream> 
#include <sstream>

using namespace std;


vector<int> availableResources;
vector<vector<int>> allocatedResources;
vector<vector<int>> needResources;
vector<int> deadlineMisses;
vector<vector<int>> workAvailable; 


struct Command {
    enum Type {
        CALCULATE, 
        REQUEST, 
        USE_RESOURCES,
        RELEASE,
        PRINT_RESOURCES_USED,
        END
    } type; 
    vector<int> paramaters;
};


struct Resource {
    string name;
    vector<string> instances;
};

struct Process {
    int id;  
    int deadline; 
    int computationTime; 
    int remainingComputationTime; 
    string currentMasterString; 
    vector <int> maxDemand; 
    vector<int> requestVector;
    vector<Command> comamnds;
};

//Earliest Deardline First Comparator
struct EDFComparator {
    bool operator()(const Process& p1, const Process& p2){
        if(p1.deadline==p2.deadline){
            //Longest Job Will Go First
            if(p1.computationTime > p2.computationTime){
                return true;
            }
            else{
                false; 
            }

        }
    }
};

//Least Laxiity First Comparator

struct LLFComparator {
    bool operator()(const Process& p1, const Process& p2){
        int laxityP1, laxityP2;
        laxityP1= p1.deadline-p1.computationTime; 
        laxityP2= p2.deadline-p2.computationTime;

        return laxityP1 > laxityP2;

    }
};

//Earliest Deadline First Scheduler
priority_queue<Process, vector<Process>, EDFComparator> edfQueue;

// Least Laxity First Scheduler 
priority_queue<Process, vector<Process>, LLFComparator> llfQueue;
//This will run the program.

bool handleResourceRequest(Process& process){
    for(int i = 0; i < availableResources.size(); i++){
        if(process.requestVector[i] > needResources[process.id][i] || process.requestVector[i] > availableResources[i]){
            return false;
        }
    }
    if(safetyAlgorithm()){
        return true;
    }
    //Not safe need to rollback
    for(int i = 0; i < availableResources.size(); i++){
            availableResources[i] += process.requestVector[i];
            allocatedResources[process.id][i] -= process.requestVector[i];
            needResources[process.id][i] += process.requestVector[i];
    }
    return false; 

}

bool safetyAlgorithm() {
    vector<int> work = availableResources;
    vector<bool> finish(allocatedResources.size(), false);

    while (true) {
        bool found = false;
        for (int i = 0; i < finish.size(); i++) {
            if (!finish[i]) {
                bool canFinish = true;
                for (int j = 0; j < work.size(); j++) {
                    if (needResources[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }
                if (canFinish) {
                    for (int j = 0; j < work.size(); j++) {
                        work[j] += allocatedResources[i][j];
                    }
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    for (bool f : finish) {
        if (!f) return false;
    }
    return true;
}


void initalizeResources(const string& wordsFile, vector<Resource>& resources){
    ifstream inputFile(wordsFile);

    if(!inputFile.is_open())
    {
        cerr << "An error occured when opening the resource file" <<  wordsFile << endl;
        exit(EXIT_FAILURE);
    }

    string line; 

    while(getline(inputFile, line)){
        if(line.find("R")!= string::npos){
            stringstream container(line);
            Resource resource;
            string item;

            getline(container, item, ':');
            getline(container, item, ':');
            resource.name = item;

            while(getline(container, item, ',')){
                resource.instances.push_back(item);
            }
         resources.push_back(resource);
        }
        istringstream  tasksContainer(line);
        Resource resource;
        string resourceName;
        string resourceInstance; 
        vector<string> instances;

        // Grab the amount of resources
        tasksContainer >> resourceName;

        
        while(tasksContainer >> resourceInstance)
        {
            instances.push_back(resourceInstance);
        }


        Resource resource = {resourceName, instances};

    }
    inputFile.close();
}

void initalizeProcesses(const string& wordsFile, vector<Resource>& resources){
    ifstream inputFile(wordsFile);
    if(!inputFile.is_open()){
        cerr << "An error occured when opening the process ifle" << wordsFile << endl;
    }

    int mResources, nProcess;

    inputFile >> mResources >> nProcess;

    string line; 
    while(getline(inputFile, line))
    {
        istringstream tasksContainer(line);
        
    }


}



int main(){
    // Parse input and initialize data structures
    
    vector<Resource> resource; 

    initalizeResources("sample_matrix.txt", resource);

    initalizeProcesses("sample_words.txt", resource);
    // Execution loop
    while (!edfQueue.empty() || !llfQueue.empty()) {
        // Execute processes using EDF scheduler
        while (!edfQueue.empty()) {
            Process currentProcess = edfQueue.top();
            edfQueue.pop();

            // Handle requests, releases, and computations
            if (currentProcess.remainingComputationTime > 0) {
                if (currentProcess.requestVector.size() > 0) {
                    bool requestGranted = handleResourceRequest(currentProcess);
                    if (!requestGranted) {
                        // Handle request denial
                        // ...
                    }
                }

                // Perform computations and update state
                // ...

                if (currentProcess.remainingComputationTime > 0) {
                    edfQueue.push(currentProcess);
                }
            }
        }

        // Execute processes using LLF scheduler
        while (!llfQueue.empty()) {
            Process currentProcess = llfQueue.top();
            llfQueue.pop();

            // Handle requests, releases, and computations
            if (currentProcess.remainingComputationTime > 0) {
                if (currentProcess.requestVector.size() > 0) {
                    bool requestGranted = handleResourceRequest(currentProcess);
                    if (!requestGranted) {
                        // Handle request denial
                        // ...
                    }
                }

                // Perform computations and update state
                // ...

                if (currentProcess.remainingComputationTime > 0) {
                    llfQueue.push(currentProcess);
                }
            }
        }
    }

    // Print system state and deadline misses
    // ...

    return 0;
};