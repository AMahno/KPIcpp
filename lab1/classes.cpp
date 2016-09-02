//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#include "classes.h"
#include <iostream>
using namespace std;

point::point(float X = 0, float Y = 0){ //constructor, with value initialization
    x = X;
    y = Y;
}

//!point::point(const point &); //copying one
//! x and y init here?

point::~point(){
    //cout << "deleted" << endl; //ok, so that works
}

//set functions, returning "this", as a reference
point& point::setCoords(float X, float Y){
    x = X;
    y = Y;
    return *this;
}

point& point::setValue(int valueToSet){
    value = valueToSet;
    return *this;
}

//user interaction functions
void point::printData(){
    cout << "Created point" << endl;
    cout << "Coordinates: " << x << ", " << y << endl;
    cout << "Value: " << value << endl;
}

point& point::pollData(){
    cout << "Changing data.\nEnter new X:" << endl;
    cin >> x;
    cout << "New Y: " << endl;
    cin >> y;
    cout << "And new value: " << endl;
    cin >> value;
    return *this;
}
