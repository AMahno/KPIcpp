//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

class point{
private:
    float x, y; //somehow I think that this is better than the array
    int value;
public:
    point(float, float); //constructor
    point(const point &); //with copying (look into classes.cpp, there is some weird stuff about this)
    ~point(); //destructor
    inline float getX() const {return x;};
    inline float getY() const {return y;};
    inline int getValue() const {return value;}; //yes, these should be outside-defined, but I don't how how to do that
    void printData(); //user interaction
    point& pollData();
    point& setCoords(float, float); //set functions
    point& setValue(int);
};

#endif // CLASSES_H_INCLUDED
