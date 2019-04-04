//
//  handler.cpp
//  a16w
//
//  Created by Rongmin Jin on 3/31/19.
//  Copyright © 2019 Rongmin Jin. All rights reserved.
//

#include "Header.h"

handler::handler()  {
    myqueue = queue<customer>();
    currentTotalWaitTime = 0;
}

queue<customer> handler::getMyqueue ()  {
    return myqueue;
}

void handler::addToMyqueue(customer input) {
     myqueue.push(input);
}

void handler::setBeginTime(int input) {
    myqueue.front().sBeginTime = input;
}

void handler::popOfMyqueue() {
    myqueue.pop();
}


int handler::getTotalWaitTime () {
    return currentTotalWaitTime;
}

void handler::setTotalWaitTime (int input) {
    currentTotalWaitTime = input;
}


