//
//  Header.h
//  a16w
//
//  Created by Rongmin Jin on 3/28/19.
//  Copyright © 2019 Rongmin Jin. All rights reserved.
//

#ifndef Header_h
#define Header_h


#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

extern string customerArrivalRate , serviceTime, randomSeed;
extern size_t lengthOfDecimalPart;


class customer {
public:
    int arrivalTime;
    int sBeginTime;
    int sEndTime;
    int sTime;
    int total_sTime;
    
    customer();
    
};

class handler {
private:
    queue<customer> myqueue;
    int currentTotalWaitTime;
public:
    handler() ;
    queue<customer> getMyqueue ();
    int getTotalWaitTime ();
    void addToMyqueue(customer input);
    void setBeginTime(int input);
    void popOfMyqueue();
    void setTotalWaitTime (int input);
    
    struct cmp{
        bool operator() (handler h1,handler h2){
            return h1.currentTotalWaitTime > h2.currentTotalWaitTime;
        }
    };

};


double randomDecimalGenerator ( );

unsigned customerArrivalRate2PerSecond ();

vector<customer> customerGenerator ();

vector<customer> simulator_market();

int shortestHandler(vector<handler> vh1);

vector<customer> simulator_bank();

bool operator<(const customer &s1, const customer &s2);

double  output4SP (vector<customer> input );


#endif /* Header_h */
