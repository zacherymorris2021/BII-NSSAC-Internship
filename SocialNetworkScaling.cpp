//
// Created by Zachery Morris on 6/11/2019.
//
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>

using namespace std;

// forward declarations
vector<int> newInterval(int endRange, int vectorSize);

// global initalization
int numNodeCopies = 0;
float rewiringFactor = 0.0;

int main(int argc, char * argv[]) {
    string fileName = argv[1];

    // convert nodeCopies string to int
    stringstream numConvert(argv[2]);
    numConvert >> numNodeCopies;

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
    vector<int> sourcePIDs, sourceActivities, targetPIDs, targetActivities, durations;

    // puts the 5 data pieces into vectors
    while(getline(myFileStream, line)) {
        if (line.length() < 58) { // skips first two lines
            stringstream ss(line);

            getline(ss, strSourcePID, ',');
            sourcePID = stoi(strSourcePID);
            sourcePIDs.push_back(sourcePID);

            getline(ss, strSourceActivity, ',');
            sourceActivity = stoi(strSourceActivity);
            sourceActivities.push_back(sourceActivity);

            getline(ss, strTargetPID, ',');
            targetPID = stoi(strTargetPID);
            targetPIDs.push_back(targetPID);

            getline(ss, strTargetActivity, ',');
            targetActivity = stoi(strTargetActivity);
            targetActivities.push_back(targetActivity);

            getline(ss, strDuration, ',');
            duration = stoi(strDuration);
            durations.push_back(duration);
        }
    }
    myFileStream.close();

    // make copy nodes for original nodes(IDs)
    int beginRangeSourcePIDs = *min_element(sourcePIDs.begin(), sourcePIDs.end());
    int endRangeSourcePIDs = *max_element(sourcePIDs.begin(), sourcePIDs.end());
    int beginRangeTargetPIDs = *min_element(targetPIDs.begin(), targetPIDs.end());
    int endRangeTargetPIDs = *max_element(targetPIDs.begin(), targetPIDs.end());
    vector<int> totalCopies;
    if(endRangeSourcePIDs < endRangeTargetPIDs){
        totalCopies = newInterval(endRangeTargetPIDs, sourcePIDs.size() + targetPIDs.size());
    }
    else if(endRangeSourcePIDs > endRangeTargetPIDs){
        totalCopies = newInterval(endRangeSourcePIDs, sourcePIDs.size() + targetPIDs.size());
    }

    // split copies into 2 vectors for "source copy" and "target copy" IDs
    vector<int> sourceCopies, targetCopies;
    vector<int>::iterator middleItr(totalCopies.begin() + totalCopies.size() / 2);
    for(auto it = totalCopies.begin(); it != totalCopies.end(); it++){
        if(distance(it, middleItr) > 0){
            sourceCopies.push_back(*it);
        }
        else{
            targetCopies.push_back(*it);
        }
    }

    // rewiring edges -- generating random numbers
    float amountOfRewiring1 = rewiringFactor * sourcePIDs.size(); // amount of edges to rewire
    float amountOfRewiring2 = amountOfRewiring1;
    srand48(time(NULL)); // srand() seeds rand() automatically
    // random original IDs
    vector<int> originalRandomIDs;
    for(int i = 0; i < amountOfRewiring1; i++){ // populate vector with random IDs to rewire
        int random = (lrand48()%(endRangeSourcePIDs - beginRangeSourcePIDs)) + beginRangeSourcePIDs;
        if(find(originalRandomIDs.begin(), originalRandomIDs.end(), random) != originalRandomIDs.end()){ // gets "rid", moves past duplicate random nums
            amountOfRewiring1++;
        }
        else {
            originalRandomIDs.push_back(random);
        }
    }
    // random copy IDs
    vector<int> copyRandomIDs;
    for(int i = 0; i < amountOfRewiring2; i++){ // populate vector with random IDs to rewire
        int random = (rand()%(sourceCopies.back() - sourceCopies.front())) + sourceCopies.front(); // maybe be: low+(rand()%(upper-lower+1))
        if(find(copyRandomIDs.begin(), copyRandomIDs.end(), random) != copyRandomIDs.end()){ // gets "rid", moves past duplicate random nums
            amountOfRewiring2++;
        }
        else {
            copyRandomIDs.push_back(random);
        }
    }

    int indexOfOriginalSourcePID = 0;
    int originalTargetPIDToSave = 0;
    int indexOfCopySourcePID = 0;
    int copyTargetPIDToSave = 0;
    while(!originalRandomIDs.empty() && !copyRandomIDs.empty()){
        // handles original IDs
        vector<int>::iterator itOriginal = find(sourcePIDs.begin(), sourcePIDs.end(), originalRandomIDs.back()); // find ID in source ID vector
        indexOfOriginalSourcePID = distance(sourcePIDs.begin(), itOriginal); // find index of source ID, now have the location of the edge -- randomly chosen
        originalTargetPIDToSave = targetPIDs.at(indexOfOriginalSourcePID); // have source ID index, target at same index, get target number, store to replace

        // handles copy IDs
        vector<int>::iterator itCopy = find(sourceCopies.begin(), sourceCopies.end(), copyRandomIDs.back());
        indexOfCopySourcePID = distance(sourceCopies.begin(), itCopy);
        copyTargetPIDToSave = targetCopies.at(indexOfCopySourcePID); // store copy target ID, to replace

        // replace -- make new edges
        replace(targetPIDs.begin(), targetPIDs.end(), originalTargetPIDToSave, copyTargetPIDToSave); // replaces original target ID with copy target ID
        replace(targetCopies.begin(), targetCopies.end(), copyTargetPIDToSave, originalTargetPIDToSave); // replaces copy target ID with original target ID

        // condition for while loop
        originalRandomIDs.pop_back();
        copyRandomIDs.pop_back();
    }

    //cout << "origSourceIDs: " << sourcePIDs.at(0) << " " << sourcePIDs.at(1) << " " << sourcePIDs.at(2) << " " << sourcePIDs.at(3) << " " << sourcePIDs.at(4) << " " << sourcePIDs.at(5) << " " << sourcePIDs.at(6) << endl;
    //cout << "origTargetIDs: " << sourcePIDs.at(0) << " " << targetPIDs.at(1) << " " << targetPIDs.at(2) << " " << targetPIDs.at(3) << " " << targetPIDs.at(4) << " " << targetPIDs.at(5) << " " << targetPIDs.at(6) << endl;
    //cout << "--------------" << endl;
    //cout << "copySourceIDs: " << sourceCopies.at(0) << " " << sourceCopies.at(1) << " " << sourceCopies.at(2) << " " << sourceCopies.at(3) << " " << sourceCopies.at(4) << " " << sourceCopies.at(5) << " " << sourceCopies.at(6) << endl;
    //cout << "copyTargetIDs: " << targetCopies.at(0) << " " << targetCopies.at(1) << " " << targetCopies.at(2) << " " << targetCopies.at(3) << " " << targetCopies.at(4) << " " << targetCopies.at(5) << " " << targetCopies.at(6) << endl;
    return 0;
}

/**
 * Creates a new range for the copied nodes. Populates a vector with the new IDs.
 * @param endRange
 * @param vectorSize
 * @return sourcePIDsCopy
 */
vector<int> newInterval(int endRange, int vectorSize){
    int maxNewRangeSource = (endRange + 1) + (endRange - 1); // assumes continuous numbers for source/target IDs
    vector<int> PIDsCopy;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    while(count1 < maxNewRangeSource && count2 < numNodeCopies && count3 < vectorSize){
        PIDsCopy.push_back(endRange + 1);
        endRange++;
        count1++;
        count2++;
        count3++;
    }
    return PIDsCopy;
}