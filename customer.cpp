//
//  customer.cpp
//  a16w
//
//  Created by Rongmin Jin on 3/28/19.
//  Copyright © 2019 Rongmin Jin. All rights reserved.
//

#include "Header.h"

customer::customer() {
    arrivalTime = 0;
    sBeginTime = 0;
    sEndTime = 0;
    sTime = 0;
    total_sTime = 0;
}

bool operator<(const customer &s1, const customer &s2){
    return s1.total_sTime < s2.total_sTime;
}


