//
//  main.cpp
//  a16w
//
//  Created by Rongmin Jin on 3/28/19.
//  Copyright © 2019 Rongmin Jin. All rights reserved.
//

#include <iostream>
#include "Header.h"

string customerArrivalRate , serviceTime, randomSeed;
size_t lengthOfDecimalPart;
int randomSeedInt;

double randomDecimalGenerator ( ) {
    double randomDecimal;
    
    srand(randomSeedInt);
    int denominator = pow(10,lengthOfDecimalPart);
    double  temp = rand() % denominator;
    randomDecimal = temp /denominator;
    randomSeedInt++;
    return randomDecimal;
}

unsigned customerArrivalRate2PerSecond () {
    unsigned numberOfCustomers;
    double customerArrivalRate_Double = stod(customerArrivalRate);
    double CAR_perSecond = customerArrivalRate_Double /60;
    
    int integerPart = floor(CAR_perSecond);
    double tempDecimalPart = CAR_perSecond - integerPart;
    int lengthOfDecimal = 4;
    double denominator = pow(10,lengthOfDecimal);
    double decimalPart = floor(tempDecimalPart * pow(10,lengthOfDecimal)) / denominator;
    //    cout << CAR_perSecond << "\t" << integerPart << "\t"<< decimalPart << endl;
    
    double criticalValve = 1 -  (1/denominator) - decimalPart;
    int trueOfCustomer = 0 ;
    if (randomDecimalGenerator() >  criticalValve )
        trueOfCustomer = 1;
    numberOfCustomers = integerPart + trueOfCustomer;
    return numberOfCustomers;
}

vector<customer> customerGenerator () {
    vector<customer> temp ;
    double maxStime= stod(serviceTime);
    int numberOfCustomers = customerArrivalRate2PerSecond ();
    if (numberOfCustomers > 0) {
        for (int  i = 0  ; i < numberOfCustomers ; ++i ) {
            customer cTemp;
            cTemp.sTime = floor(maxStime * randomDecimalGenerator() *60);
            temp.push_back(cTemp);
        }
        return temp;
    }
    else {
        return temp;
    }
}

int shortestHandler(vector<handler> vh1) {
    int tempMin = vh1[0].getTotalWaitTime();
    int index = 0 ;
    for (int i =1; i < vh1.size(); ++i) {
        if (vh1[i].getTotalWaitTime() < tempMin) {
            tempMin = vh1[i].getTotalWaitTime();
            index = i;
        }
    }
    return index;
}

vector<customer> simulator_market() {
    int hours = 12;
    int totalseconds = 60*60*hours;
    //Queue for 6 cashiers
    int numberOfHandlers = 6;
    vector<handler> handlers(numberOfHandlers);
    vector<customer> totalCustomers;
    //simulation for operation
    for (int j = 0 ; j < totalseconds ; ++j) {
        srand(randomSeedInt+j);
        //check each queue of handler
        for (int m=0 ;m < numberOfHandlers; ++m ) {
            if (handlers[m].getTotalWaitTime() >0 ) {
                handlers[m].setTotalWaitTime(handlers[m].getTotalWaitTime()-1);
                if (handlers[m].getMyqueue().front().sTime == 0 ){
                    customer temp = handlers[m].getMyqueue().front();
                    temp.total_sTime = j - temp.arrivalTime-1;
                    totalCustomers.push_back(temp);
                    handlers[m].getMyqueue().pop();
                    handlers[m].getMyqueue().front().sBeginTime = j;
                }
                if (handlers[m].getMyqueue().front().sTime ==j- handlers[m].getMyqueue().front().sBeginTime) {
                    customer temp = handlers[m].getMyqueue().front();
                    temp.total_sTime = j - temp.arrivalTime;
                    totalCustomers.push_back(temp);
                    handlers[m].popOfMyqueue();
                    handlers[m].setBeginTime(j);
                }
            }
        }
        
        //distribution of customer
        vector<customer> vTemp = customerGenerator();
        if (vTemp.size() >0) {
            for (int k = 0; k < vTemp.size(); ++ k) {
                int index = shortestHandler(handlers);
                if (handlers[index].getMyqueue().empty())
                    vTemp[k].sBeginTime = j;
                vTemp[k].arrivalTime = j;
                handlers[index].addToMyqueue(vTemp[k]);
                handlers[index].setTotalWaitTime(handlers[index].getTotalWaitTime()+vTemp[k].sTime);
            }
        }
    }
    return totalCustomers;
}

vector<customer> simulator_bank() {
    int hours = 12;
    int totalseconds = 60*60*hours;
    //Queue for 6 cashiers
    int numberOfHandlers = 6;
    vector<handler> handlers(numberOfHandlers);
    
    vector<customer> totalCustomers;
    queue<customer> queueOfCustomers;
    
    
    for (int j = 0 ; j < totalseconds; ++j) {
        srand(randomSeedInt+j);
        vector<customer> vTemp = customerGenerator(); // generation of customers.
        
        // add arrivalTime to customer and add customer to waitting queue
        if (vTemp.size() >0) {
            for (int k = 0; k < vTemp.size(); ++k) {
                vTemp[k].arrivalTime= j;
                queueOfCustomers.push(vTemp[k]);
            }
        }
        
        
        for (int l = 0 ; l < numberOfHandlers; ++l) {
            if(handlers[l].getTotalWaitTime() >0 ) {
                handlers[l].setTotalWaitTime(handlers[l].getTotalWaitTime()-1);
            }
            
            //check status of handler and move customer to idle handler.
            if (handlers[l].getTotalWaitTime() ==0 && !handlers[l].getMyqueue().empty() ) {
                customer ctemp1 = handlers[l].getMyqueue().front();
                handlers[l].popOfMyqueue();
                ctemp1.sEndTime = j;
                ctemp1.total_sTime = j- ctemp1.arrivalTime;
                totalCustomers.push_back(ctemp1);
            }
            
            //distribution of customer from waitting queue
            if (handlers[l].getMyqueue().empty() && !queueOfCustomers.empty() ) {
                customer ctemp = queueOfCustomers.front();
                ctemp.sBeginTime = j;
                queueOfCustomers.pop();
                if (ctemp.sTime == 0 ) {
                    ctemp.total_sTime = j - ctemp.arrivalTime;
                    totalCustomers.push_back(ctemp);
                    continue;
                }
                handlers[l].addToMyqueue(ctemp);
                handlers[l].setTotalWaitTime( ctemp.sTime);
            }
            
        }
        
    }
    return totalCustomers;
}

void output (vector<customer> bank, vector<customer> supermarket) {
    sort(bank.begin(),bank.end());
    sort(supermarket.begin(),supermarket.end());
    double b10 , b50, b90;
    double s10 , s50, s90;
    double secsPerMin = 60.0;
    b10 = bank[bank.size()/10].total_sTime / secsPerMin;
    b50 = bank[bank.size()/10 *5].total_sTime / secsPerMin;
    b90 = bank[bank.size()/10 * 9].total_sTime / secsPerMin;
    s10 = supermarket[supermarket.size()/10].total_sTime / secsPerMin;
    s50 = supermarket[supermarket.size()/10*5].total_sTime / secsPerMin;
    s90 = supermarket[supermarket.size()/10 * 9].total_sTime / secsPerMin;
    
    cout << "Bank service time in minutes:  10th %ile " << b10 << ", 50th %ile " << b50 << ", 90th %ile " << b90 << endl;
    cout << "Supermark service time in minutes:  10th %ile " << s10 << ", 50th %ile " << s50 << ", 90th %ile " << s90 << endl;
}


double  output4SP (vector<customer> input ) {
    sort(input.begin(),input.end());
    double i90;
    
    double secsPerMin = 60.0;
    if (input.size() == 0 )
        return 0;
    else
      i90 = input[input.size()/10 * 9].total_sTime / secsPerMin;
    
    return i90;
}

int main(int argc, const char * argv[]) {
    
    //    srand(time(NULL));
    //    if (argc < 4) {
    //        cout << " No enough parameters" << endl;
    //        return 0;
    //    }
    int ca =-1;
    vector<int> x;
    vector<double> y;
    vector<double> z;
    
    
    for(int i = 0; i <11; ++i) {
        ca = ca+1;
        double st = -0.5;
        for (int j = 0; j<11 ; ++j) {
            st=st+0.5;
            
            customerArrivalRate= to_string(ca);
            
            serviceTime = to_string(st);
            lengthOfDecimalPart = 4;
            randomSeed = "10";
            
            
            //    customerArrivalRate=argv[1] ;
            //    serviceTime = argv[2];
            //    lengthOfDecimalPart = 4;
            //    randomSeed = argv[3];
            randomSeedInt = stoi(randomSeed);
            
            x.push_back(ca);
            y.push_back(st);
            z.push_back(output4SP(simulator_bank()));
        }
    }
    for(int k =0; k < x.size(); ++k)
        cout << x[k] << "," ;
    cout << "XXX" << endl;
    for(int l =0; l < y.size(); ++l)
        cout << y[l] << "," ;
    cout <<"YYY" << endl;
    for(int m =0; m < z.size(); ++m)
        cout << z[m] << "," ;
    cout <<"ZZZ" << endl;
    
    
    return 0;
}
