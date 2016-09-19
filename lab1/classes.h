//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

enum {
    POINT = 0,
    RATIONAL,
    COMPLEX
};

class point{
private:
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
    char* getName() const;
    void printData(); //user interaction
    point& pollData();
    point& setCoords(float, float); //set functions
    point& setName(char*);
    point& setValue(int);
};

#endif // CLASSES_H_INCLUDED
