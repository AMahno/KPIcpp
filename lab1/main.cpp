//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#include "classes.h"

int main(){
    point myPoint(0, 0, 0);

    myPoint.setValue(12345);
    myPoint.setCoords(115.6, 124.743);
    myPoint.printData();

    myPoint.pollData();
    myPoint.printData();

    return 0;
}
