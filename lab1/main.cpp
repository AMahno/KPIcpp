//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#include "classes.h"
#include <iostream>
using namespace std;

int main(){
    point myPoint(1, 2, 3); //using constructor with values
    point thePoint(myPoint); //using copying constructor
    point anotherPoint; //using default constructor

    cout << "Data from parameter-constructed object:" << endl;
    myPoint.printData();
    cout << "Data from default-constructed object:" << endl;
    anotherPoint.printData();
    cout << "Data from copy-constructed object:" << endl;
    thePoint.printData();
    cout << "Changing data using methods:" << endl;
    myPoint.setValue(12345);
    myPoint.setCoords(115.6, 124.743);
    myPoint.printData();

    myPoint.pollData();
    myPoint.printData();

    return 0;
}
