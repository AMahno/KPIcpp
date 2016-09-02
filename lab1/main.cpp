//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#include "classes.h"

int main(){
    point myPoint(10.5, 12.56);
    myPoint.setValue(12345);
    myPoint.printData();
    myPoint.pollData();
    myPoint.printData();
    return 0;
}


