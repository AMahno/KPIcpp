//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#include "classes.h"
#include <iostream>
#include <cstring>
using namespace std;

point::point(float X, float Y, int theValue, char* theName){ //constructor, with value initialization
    x = X;
    y = Y;
    value = theValue;
    type = POINT;
    name = new char[strlen(theName)];
    strcpy(name, theName);
}

point::point(){
    x = 0;
    y = 0;
    value = 0;
    type = POINT;
    name = NULL;
}

point::point(const point &inputPoint){
    x = inputPoint.getX();
    y = inputPoint.getY();
    value = inputPoint.getValue();
    type = POINT;
    if(inputPoint.getName()){
        name = new char[strlen(inputPoint.getName())];
        strcpy(name, inputPoint.getName());
    }else{
        name = NULL;
    }
}

point::~point(){
    if(name) delete []name;
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

point& point::setName(char* nameToSet){
    if(strlen(nameToSet) < strlen(name)){
        strcpy(name, nameToSet);
    }else{
        delete[] name;
        name = new char[strlen(nameToSet)]();
        strcpy(name, nameToSet);
    }
    return *this;
}

//get functions
inline float point::getY() const {return y;};
inline float point::getX() const {return x;};
inline int point::getValue() const {return value;};
char* point::getName() const {return name;};

//user interaction functions
void point::printData(){
    cout << "Coordinates: " << x << ", " << y << endl;
    cout << "Value: " << value << endl;
    if(name){
        cout << "Name: " << name << endl;
    }else{
        cout << "Name: empty" << endl;
    }
    cout << "Type: " << type << endl;
}


point& point::pollData(){
    cout << "\nChanging data.\nEnter new X:" << endl;
    cin >> x;
    cout << "New Y: " << endl;
    cin >> y;
    cout << "New value: " << endl;
    cin >> value;
    cout << "And new name: " << endl;
    cin >> name;
    return *this;
}
