//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#include "classes.h"
#include <iostream>
#include <cstring>
using namespace std;

point::point(float X, float Y, int theValue, char* theName, int theNameSize){ //constructor, with value initialization
    x = X;
    y = Y;
    value = theValue;

    name = new char[theNameSize+1]();
    strcpy(name, theName);
    name_size = theNameSize+1;
}

point::point(){ //!ADD CHAR TO CONSTRUCTORS
    x = 0;
    y = 0;
    value = 0;
}

point::point(const point &inputPoint){
    x = inputPoint.getX();
    y = inputPoint.getY();
    value = inputPoint.getValue();
}

point::~point(){
    delete &x;
    delete &y;
    delete &value;
    //delete this; //<- a bit too much
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

point& point::setName(char* nameToSet, int newNameSize){
    if(newNameSize < name_size){
        strcpy(name, nameToSet);
    }else{
        delete[] name;
        name = new char[newNameSize+1]();
        name_size = newNameSize+1;
    }
    return *this;
}

//get functions
inline float point::getY() const {return y;};
inline float point::getX() const {return x;};
inline int point::getValue() const {return value;};

char* point::getName() const {return name;};
int point::getNameSize() const {return name_size;};

//user interaction functions
void point::printData(){
    cout << "Coordinates: " << x << ", " << y << endl;
    cout << "Value: " << value << endl;
}

void point::printName(){
    char* tmpChar = new char[name_size+1]();
    strcpy(tmpChar, name);
    cout << tmpChar << endl;
    delete[] tmpChar;
}

point& point::pollData(){
    cout << "\nChanging data.\nEnter new X:" << endl;
    cin >> x;
    cout << "New Y: " << endl;
    cin >> y;
    cout << "And new value: " << endl;
    cin >> value;
    return *this;
}
