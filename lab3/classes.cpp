//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#include "classes.h"
#include <iostream>
#include <cstring>
#include <ostream>
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
inline int FractionPoint::getDenominator() const {return denominator;};

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

//operators here
void FractionPoint::operator = (const FractionPoint& input){
    x = input.getX();
    y = input.getY();
    value = input.getValue();
    denominator = input.getDenominator();
    this->setName(input.getName());
}

//basic fraction compare function. I think, it might be somewhat dangerous
bool FractionPoint::operator < (const FractionPoint& inputPoint){
    long upper = value * inputPoint.getDenominator();
    long lower = denominator * inputPoint.getValue();
    return upper<lower;
}

//!ask if this will copy a whole object
FractionPoint FractionPoint::operator + (const FractionPoint &inputPoint){
    FractionPoint tmpPoint(x, y, value*inputPoint.getDenominator()+denominator*inputPoint.getValue(), denominator*inputPoint.getDenominator(), name);
    return tmpPoint;
}

FractionPoint& FractionPoint::operator - (){
    value = -value;
    return *this;
}

//!ask how to do this better, if possible
ostream& operator << (ostream &stream, const FractionPoint& input){
    //const point *pointPtr = &input;
    //return stream << *pointPtr;
    return stream << "X: " << input.getX() << ", Y: " << input.getY() << ", Value: " << input.getValue() <<
     "\nName: " << input.getName() << "\nDenominator: " << input.getDenominator() << endl;
}

istream& operator >> (istream &stream, FractionPoint& input){
    float tmpX = 0, tmpY = 0;
    int tmpValue = 0, tmpDenom = 1;
    char *tmpChar;
    stream >> tmpX >> tmpY >> tmpValue >> tmpChar >> tmpDenom;
    input.setX(tmpX);
    input.setY(tmpY);
    input.setValue(tmpValue);
    input.setName(tmpChar);
    input.setDenominator(tmpDenom);
    return stream;
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
inline int ComplexPoint::getComplexPart() const {return complexPart;};

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

//operators here:
void ComplexPoint::operator = (const ComplexPoint& input){
    x = input.getX();
    y = input.getY();
    value = input.getValue();
    complexPart = input.getComplexPart();
    this->setName(input.getName());
}

//you can't compare complex numbers in complex space, so I will just compare real parts
bool ComplexPoint::operator < (const ComplexPoint& inputPoint){
    return value<inputPoint.getValue();
}

ComplexPoint ComplexPoint::operator + (const ComplexPoint &inputPoint){
    ComplexPoint tmpPoint(x, y, value+inputPoint.getValue(), complexPart+inputPoint.getComplexPart(), name);
    return tmpPoint;
}

ComplexPoint& ComplexPoint::operator - (){ //re = re1*re2 - im1*im2, where im2 = 0, cos -1 = -1 + i*0
    value = -value;
    return *this;
}

ostream& operator << (ostream &stream, const ComplexPoint& input){
    return stream << "X: " << input.getX() << ", Y: " << input.getY() << ", Value: " << input.getValue() <<
    "\nName: " << input.getName() << "\nComplex part: " << input.getComplexPart() << endl;
}

istream& operator >> (istream &stream, ComplexPoint& input){
    float tmpX = 0, tmpY = 0;
    int tmpValue = 0, tmpComplex = 0;
    char *tmpChar;
    stream >> tmpX >> tmpY >> tmpValue >> tmpChar >> tmpComplex;
    input.setX(tmpX);
    input.setY(tmpY);
    input.setValue(tmpValue);
    input.setName(tmpChar);
    input.setComplexPart(tmpComplex);
    return stream;
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
point& point::setX(float X){
    x = X;
    return *this;
}

point& point::setY(float Y){
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

ostream& operator << (std::ostream &stream, const point& thePoint){
    return stream << "X: " << thePoint.getX() << ", Y: " << thePoint.getY() << ", Value: " << thePoint.getValue() << "\nName: " << thePoint.getName() << endl;
}

istream& operator >> (istream &stream, point& input){
    float tmpX = 0, tmpY = 0;
    int tmpValue = 0;
    char *tmpChar;
    stream >> tmpX >> tmpY >> tmpValue >> tmpChar;
    input.setX(tmpX);
    input.setY(tmpY);
    input.setValue(tmpValue);
    input.setName(tmpChar);
    return stream;
}

void point::print(){
    cout << "(" << x << ", " << y << ")" << endl;
}
