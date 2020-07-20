/**
 *  This file defines and demonstrates that given a set of data, a larger scaled network can be created.
 *
 *  Written by Zachery Morris, 2019
 */

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <time.h>
#include <set>

using namespace std;

// global declaration/initialization
int cloningFactor = 0;
float rewiringFactor = 0.0;

// object declaration
struct edge {
    int idNodeOne;
    int activityNodeOne;
    int idNodeTwo;
    int activityNodeTwo;
    int duration;
};

// forward declarations
vector<edge> cloningEdges(int maxID, int minID, int cloningFactor, vector<edge> &vecEdges);
vector<int> genRandNums(double rewiringFactor, vector<edge> &clonedEdges);

/**
 * The main() function reads in a CSV file, stores the data into edge objects,
 * calls cloningEdges() helper function, genRandNums helper function, rewires
 * edges based on inputted rewiringFactor command line parameter, and writes
 * the scaled network to a text file.
 */
int main(int argc, char * argv[]) {
    string fileName = argv[1];

    // convert nodeCopies string to int
    stringstream numConvert(argv[2]);
    numConvert >> cloningFactor;

    // convert resizing string to float
    rewiringFactor = std::stof(argv[3]);

    // try to read in the file
    ifstream myFileStream(fileName);
    if (!myFileStream.is_open()) {
        cout << "File failed to open." << endl;
        exit(1); // requires <stdlib.h>
    }

    string strSourcePID, strSourceActivity, strTargetPID, strTargetActivity, strDuration;
    int sourcePID, sourceActivity, targetPID, targetActivity, duration;
    string line;

    vector<edge> vecEdges;
    edge originalEdge;
    string line1, line2;

    // for finding min/max
    int maxID, minID;
    int mxSourceID = 0;
    int mnSourceID = INT32_MAX;
    int mxTargetID = 0;
    int mnTargetID = INT32_MAX;

    // puts data members into edge objects, then a vector of edges
    getline(myFileStream, line1); // removes/stores 2 header lines in data file
    getline(myFileStream, line2);

    while(getline(myFileStream, line)) {
        stringstream ss(line);

        getline(ss, strSourcePID, ',');
        sourcePID = stoi(strSourcePID);

        getline(ss, strSourceActivity, ',');
        sourceActivity = stoi(strSourceActivity);

        getline(ss, strTargetPID, ',');
        targetPID = stoi(strTargetPID);

        getline(ss, strTargetActivity, ',');
        targetActivity = stoi(strTargetActivity);

        getline(ss, strDuration, ',');
        duration = stoi(strDuration);

        // finding min/max of sourceIDs
        if(mxSourceID < sourcePID){
            mxSourceID = sourcePID;
        }
        if(mnSourceID > sourcePID){
            mnSourceID = sourcePID;
        }
        // finding min/max of targetIDs
        if(mxTargetID < targetPID){
            mxTargetID = targetPID;
        }
        if(mnTargetID > targetPID){
            mnTargetID = targetPID;
        }
        // finding absolute max b/w source and target
        if(mxSourceID < mxTargetID){
            maxID = mxTargetID;
        }
        else{
            maxID = mxSourceID;
        }
        // finding absolute min b/w source and target
        if(mnSourceID < mnTargetID){
            minID = mnTargetID;
        }
        else{
            minID = mnSourceID;
        }

        if(sourcePID > targetPID){ // bidirectional to directed
            if(((double) rand() / (RAND_MAX)) < 0.5){ // randomizes
                originalEdge.idNodeOne = sourcePID;
                originalEdge.activityNodeOne = sourceActivity;
                originalEdge.idNodeTwo = targetPID;
                originalEdge.activityNodeTwo = targetActivity;
                originalEdge.duration = duration;
                vecEdges.push_back(originalEdge);
            }
            else{
                originalEdge.idNodeOne = targetPID;
                originalEdge.activityNodeOne = targetActivity;
                originalEdge.idNodeTwo = sourcePID;
                originalEdge.activityNodeTwo = sourceActivity;
                originalEdge.duration = duration;
                vecEdges.push_back(originalEdge);
            }
        }
        else{
            // skip line
        }
    }
    myFileStream.close();

    vector<edge> clonedEdges = cloningEdges(maxID, minID, cloningFactor, vecEdges); // call to cloningEdges helper function

    // rewiring -- rewires node ID and activity
    vector<int> randomNums = genRandNums(rewiringFactor, clonedEdges); // call to genRandNums helper function
    while(!randomNums.empty()){
        int nodeTwoId_a = clonedEdges.at(randomNums.back()).idNodeTwo; // get first ID to swap
        int nodeTwoAct_a = clonedEdges.at(randomNums.back()).activityNodeTwo; // get first activity to swap
        int index_a = randomNums.back(); // store index of first node to rewire
        randomNums.pop_back(); // remove rand num for next node to swap

        int nodeTwoId_b = clonedEdges.at(randomNums.back()).idNodeTwo;
        int nodeTwoAct_b = clonedEdges.at(randomNums.back()).activityNodeTwo;
        int index_b = randomNums.back();
        randomNums.pop_back();

        // swap the first node to the second
        clonedEdges.at(index_a).idNodeTwo = nodeTwoId_b;
        clonedEdges.at(index_a).activityNodeTwo = nodeTwoAct_b;

        // swap the second node to the first
        clonedEdges.at(index_b).idNodeTwo = nodeTwoId_a;
        clonedEdges.at(index_b).activityNodeTwo = nodeTwoAct_a;
    }

    // writing scaled network to text file
    ofstream outputFile;
    outputFile.open("scaled_network.txt");
    outputFile << line1 << "\n";
    outputFile << line2 << "\n";
    for(auto edge : clonedEdges){ // output each edge, make bidirectional
        outputFile << edge.idNodeOne << "," << edge.activityNodeOne << "," << edge.idNodeTwo << "," << edge.activityNodeTwo << "," << edge.duration << "\n";
        outputFile << edge.idNodeTwo << "," << edge.activityNodeTwo << "," << edge.idNodeOne << "," << edge.activityNodeOne << "," << edge.duration << "\n";
    }
    outputFile.close();

    return 0;
}

/**
 * Clones the original amount of edges by "K". Returns a vector of cloned and original edges.
 * @param maxID
 * @param minID
 * @param cloningFactor
 * @param vecEdges
 * @return vector of cloned and original edges
 */
vector<edge> cloningEdges(int maxID, int minID, int cloningFactor, vector<edge> &vecEdges){
    int nodeOneID = maxID + 1;
    int nodeTwoID = maxID + 2;

    edge clonedEdge;
    vector<edge> scaledNetwork;

    int count = 0;
    while(count < cloningFactor - 1){
        for(auto & edge : vecEdges){
            clonedEdge.idNodeOne = nodeOneID;
            clonedEdge.activityNodeOne = edge.activityNodeOne; // keep activities/duration the same for clones
            clonedEdge.idNodeTwo = nodeTwoID;
            clonedEdge.activityNodeTwo = edge.activityNodeTwo;
            clonedEdge.duration = edge.duration;

            scaledNetwork.push_back(clonedEdge); // adding the cloned edges

            nodeOneID = nodeTwoID + 1; // incrementing -- takes care of continuous number IDs
            nodeTwoID = nodeOneID + 1;
        }
        count++;
    }
    for(auto & edge : vecEdges){ // add original edges
        scaledNetwork.push_back(edge);
    }
    return scaledNetwork;
}

/**
 * Generates random numbers based off of the rewiring factor and the total number of edges.
 * @param rewiringFactor
 * @param clonedEdges
 * @return a vector of rewired edges
 */
vector<int> genRandNums(double rewiringFactor, vector<edge> &clonedEdges){
    int numberToRewire = rewiringFactor * (clonedEdges.size());
    numberToRewire = numberToRewire*2; // 2R >= Mf

    // generate random numbers
    int floor = 0;
    int ceiling = clonedEdges.size() - 1;
    int range =(ceiling - floor) + 1;
    int rndNum;
    int count = 0;
    vector<int> randomNums;
    set<int> randSet;
    srand48(time(NULL)); // srand48() seeds lrand48() automatically
    while(count < numberToRewire){
        rndNum = floor + lrand48() % range;
        if(randSet.insert(rndNum).second) { // if found num already in vector need to generate another num
            randomNums.push_back(rndNum);
            count++;
        }
    }
    return randomNums;
}
