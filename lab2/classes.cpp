//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#include "classes.h"
#include <iostream>
#include <cstring>
using namespace std;

//deriative classes (FractionPoint and ComplexPoint) are also here
//constructors
FractionPoint::FractionPoint(){
    type = FRACTION;
    x = 0;
    y = 0;
    value = 0;
    denominator = 1;
    *name = NULL;
}

FractionPoint::FractionPoint(float X, float Y, int theValue, int theDenominator, char* theName)
: point(X, Y, theValue, theName) {
    type = FRACTION;
    denominator = theDenominator;
}

//get function
int FractionPoint::getDenominator() const {return denominator;};

//setFunction
FractionPoint& FractionPoint::setDenominator(int theDenominator){
    denominator = theDenominator;
    return *this;
}

//this was in the task, it requires a +/- do be dealt with
void FractionPoint::print(){
    if(value > 0) cout << "+" << value << "/" << denominator << endl;
    else cout << value << "/" << denominator << endl;
}



//constructors
ComplexPoint::ComplexPoint(){
    type = COMPLEX;
    x = 0;
    y = 0;
    value = 0;
    complexPart = 1;
    *name = NULL;
}

ComplexPoint::ComplexPoint(float X, float Y, int theValue, int theComplexPart, char* theName)
: point(X, Y, theValue, theName) {
    type = COMPLEX;
    complexPart = theComplexPart;
}

//get function
int ComplexPoint::getComplexPart() const {return complexPart;};

//setFunction
ComplexPoint& ComplexPoint::setComplexPart(int theComplexPart){
    complexPart = theComplexPart;
    return *this;
}

//this was in the task, it requires a +/- do be dealt with
void ComplexPoint::print(){
    if(value > 0) cout << "+" << value;
    else cout << value;
    if(complexPart > 0) cout << "+" << complexPart << "i" << endl;
    else cout << complexPart << "i" << endl;
}



//original class here
point::point(float X, float Y, int theValue, char* theName){ //constructor, with value initialization
    type = POINT;
    x = X;
    y = Y;
    value = theValue;
    name = new char[strlen(theName)];
    strcpy(name, theName);
}

point::point(){
    type = POINT;
    x = 0;
    y = 0;
    value = 0;
    name = NULL;
}

point::point(const point &inputPoint){
    type = POINT;
    x = inputPoint.getX();
    y = inputPoint.getY();
    value = inputPoint.getValue();
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
inline int point::getType() const {return type;};
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
