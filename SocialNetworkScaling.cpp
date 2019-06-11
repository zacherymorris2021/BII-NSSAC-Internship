//
// Created by Zachery Morris on 6/11/2019.
//
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//#include "SocialNetworkScaling.h"

using namespace std;

int main(int argc, char * argv[]) {
    string fileName = argv[1];

    // convert nodeCopies string to int
    stringstream numConvert(argv[2]);
    int nodeCopies = 0;
    numConvert >> nodeCopies;

    // convert resizing string to float
    float resizingFactor = std::stof(argv[3]);

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

    // puts 5 data pieces into vectors
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

    cout << "sourcePIDs 1st: " << sourcePIDs.front()  << " last: " << sourcePIDs.back() << endl;

    cout << "--------------------------------" << endl;

    cout << "sourceActivities 1st: " << sourceActivities.front() << " last: " << sourceActivities.back() << endl;

    cout << "--------------------------------" << endl;

    cout << "targetPIDs 1st: " << targetPIDs.front() << " last: " << targetPIDs.back() << endl;

    cout << "--------------------------------" << endl;

    cout << "targetActivities 1st: " << targetActivities.front() << " last: " << targetActivities.back() << endl;

    cout << "--------------------------------" << endl;

    cout << "durations 1st: " << durations.front() << " last: " << durations.back() << endl;

//        for(vector<int>::const_iterator iter=sourcePIDs.begin(); iter != sourcePIDs.end(); iter++){
//            cout << *iter << endl;
//        }
//
//        cout << "--------------------------------" << endl;
//
//        for(vector<int>::const_iterator iter=sourceActivities.begin(); iter != sourceActivities.end(); iter++){
//            cout << *iter << endl;
//        }
//
//        cout << "--------------------------------" << endl;
//
//        for(vector<int>::const_iterator iter=targetPIDs.begin(); iter != targetPIDs.end(); iter++){
//            cout << *iter << endl;
//        }
//
//        cout << "--------------------------------" << endl;
//
//        for(vector<int>::const_iterator iter=targetActivities.begin(); iter != targetActivities.end(); iter++){
//            cout << *iter << endl;
//        }
//
//        cout << "--------------------------------" << endl;
//
//        for(vector<int>::const_iterator iter=durations.begin(); iter != durations.end(); iter++){
//            cout << *iter << endl;
//        }

    myFileStream.close();
    return 0;
}

