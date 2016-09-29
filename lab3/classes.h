//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED
#include <ostream>

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
    virtual ~point(); //destructor
    friend std::ostream& operator << (std::ostream &, const point &);
    float getX() const;
    float getY() const;
    int getValue() const;
    char* getName() const;
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
    void operator = (const FractionPoint &);
    bool operator < (const FractionPoint &);
    FractionPoint operator + (const FractionPoint &);
    FractionPoint& operator - ();
    friend std::ostream& operator << (std::ostream &, const FractionPoint &);
    int getDenominator() const;
    FractionPoint& setDenominator(int);
    virtual void print();
};

class ComplexPoint : public point {
private:
    int complexPart;
public:
    ComplexPoint();
    ComplexPoint(float, float, int, int, char*);
    void operator = (const ComplexPoint &);
    bool operator < (const ComplexPoint &);
    ComplexPoint operator + (const ComplexPoint &);
    ComplexPoint& operator - ();
    friend std::ostream& operator << (std::ostream &, const ComplexPoint &);
    int getComplexPart() const;
    ComplexPoint& setComplexPart(int);
    virtual void print();
};


#endif // CLASSES_H_INCLUDED
