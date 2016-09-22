//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

enum{
    POINT = 0,
    FRACTION,
    COMPLEX
};

class point{
protected:
    float x, y; //somehow I think that this is better than the array
    int value;
    char *name;
public:
    int type;
    point(); //default constructor
    point(float, float, int, char*); //constructor with input values
    point(const point &); //with copying
    ~point(); //destructor
    float getX() const;
    float getY() const;
    int getValue() const;
    int getType() const;
    char* getName() const;
    void printData(); //user interaction
    point& pollData();
    point& setCoords(float, float); //set functions
    point& setName(char*);
    point& setValue(int);
};

class FractionPoint : public point {
private:
    int denominator;
public:
    FractionPoint();
    FractionPoint(float, float, int, int, char*);
    int getDenominator() const;
    FractionPoint& setDenominator(int);
    void print();
};

class ComplexPoint : public point {
private:
    int complexPart;
public:
    ComplexPoint();
    ComplexPoint(float, float, int, int, char*);
    int getComplexPart() const;
    ComplexPoint& setComplexPart(int);
    void print();
};

#endif // CLASSES_H_INCLUDED
