//Copyright (c) Aleksander Mahnyov, 2016. All rights reserved
#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

class point{
private:
    float x, y; //somehow I think that this is better than the array
    int value;
    char *name;
    int name_size;
public:
    point(); //default constructor
    point(float, float, int, char*, int); //constructor with input values
    point(const point &); //with copying
    ~point(); //destructor
    float getX() const;
    float getY() const;
    int getValue() const;
    char* getName() const;
    int getNameSize() const;
    void printName();
    void printData(); //user interaction
    point& pollData();
    point& setCoords(float, float); //set functions
    point& setName(char*, int);
    point& setValue(int);
};

void printData(const point &inputPoint);

#endif // CLASSES_H_INCLUDED
