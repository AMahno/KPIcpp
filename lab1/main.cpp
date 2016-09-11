//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#include "classes.h"
#include <iostream>
#include <cstring>
using namespace std;

int main(){
    point myPoint(1, 2, 3, "FOO", 10); //using constructor with values
    //point thePoint(myPoint); //using copying constructor
    //point anotherPoint; //using default constructor

    myPoint.printName();

    myPoint.setName("BAR", 5);
    myPoint.printName();

   // cout << "Data from parameter-constructed object:" << endl;
   // myPoint.printData();
   // cout << "Data from default-constructed object:" << endl;
    //anotherPoint.printData();
   // cout << "Data from copy-constructed object:" << endl;
   // thePoint.printData();
    //cout << "Changing data using methods:" << endl;
   // myPoint.setValue(12345);
    //myPoint.setCoords(115.6, 124.743);
    //myPoint.printData();

    //myPoint.pollData();
    //myPoint.printData();
    cout << "return here:" << endl;
    return 0;
}
