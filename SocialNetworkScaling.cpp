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
//#include "SocialNetworkScaling.h"

using namespace std;

// forward declarations
vector<int> newInterval(int beingRange, int endRange, int vectorSize);

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

    // make copy nodes for source-nodes (sourceIDs)
    int beginRangeSourcePIDs = *min_element(sourcePIDs.begin(), sourcePIDs.end());
    int endRangeSourcePIDs = *max_element(sourcePIDs.begin(), sourcePIDs.end());
    vector<int> sourceCopies = newInterval(beginRangeSourcePIDs, endRangeSourcePIDs, sourcePIDs.size());

    // make copy nodes for target-nodes (targetIDs)
    int beginRangeTargetPIDs = *min_element(targetPIDs.begin(), targetPIDs.end());
    int endRangeTargetPIDs = *max_element(targetPIDs.begin(), targetPIDs.end());
    vector<int> targetCopies = newInterval(beginRangeTargetPIDs, endRangeTargetPIDs, targetPIDs.size());

    // rewiring edges -- generating random numbers
    float amountOfRewiring = rewiringFactor * sourcePIDs.size(); // amount of edges to rewire
    srand(time(NULL)); // srand() seeds rand() automatically
    // original IDs
    vector<int> originalRandomIDs;
    for(int i = 0; i < amountOfRewiring; i++){ // populate vector with random IDs to rewire
        int random = rand()%(endRangeSourcePIDs - beginRangeSourcePIDs) + beginRangeSourcePIDs;
        if(find(originalRandomIDs.begin(), originalRandomIDs.end(), random) != originalRandomIDs.end()){ // gets "rid", moves past duplicate random nums
            amountOfRewiring++;
        }
        else {
            originalRandomIDs.push_back(random);
        }
    }
    // copy IDs
    srand(time(NULL));
    vector<int> copyRandomIDs;
    for(int i = 0; i < amountOfRewiring; i++){ // populate vector with random IDs to rewire
        int random = rand()%(sourceCopies.back() - sourceCopies.front()) + sourceCopies.front();
        if(find(copyRandomIDs.begin(), copyRandomIDs.end(), random) != copyRandomIDs.end()){ // gets "rid", moves past duplicate random nums
            amountOfRewiring++;
        }
        else {
            copyRandomIDs.push_back(random);
        }
    }

    // rewiring edges -- making the new edges


    for(vector<int>::const_iterator iter = originalRandomIDs.begin(); iter != originalRandomIDs.end(); iter++){
        cout << *iter << endl;
    }
    cout << "--------------" << endl;
    for(vector<int>::const_iterator iter = copyRandomIDs.begin(); iter != copyRandomIDs.end(); iter++){
        cout << *iter << endl;
    }

    return 0;
}

/**
 * Creates a new range for the copied nodes. Populates a vector with the new IDs.
 * @param beginRange
 * @param endRange
 * @param vectorSize
 * @return sourcePIDsCopy
 */
vector<int> newInterval(int beginRange, int endRange, int vectorSize){
    int maxNewRangeSource = (endRange - beginRange) + 1; // not really sure why I have this, we came up with this formula monday [ (b-a)+1 ] -- I think this assumes continuous numbers of data --> probably work with full data
    vector<int> sourcePIDsCopy;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    while(count1 < maxNewRangeSource && count2 < numNodeCopies && count3 < vectorSize){
        sourcePIDsCopy.push_back(endRange + 1);
        endRange++;
        count1++;
        count2++;
        count3++;
    }
    return sourcePIDsCopy;
}


