#include <stdio.h>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES
#include "bernoulli_b.h"
#include <iomanip>
#include <GL/glut.h>
#include <conio.h>

#define MAX_INDEX 100
#define ZOOM M_PI

using namespace std;
int factorial(int n);
double bicyleCTG(double arg, double precision);
void display();
double argument;
double myResult;

int main(int argc, char* argv[]){
    cout << "Enter the argument in rads: ";
    cin >> argument;
    argument /= M_PI;
    cout << "Enter the precision: ";
    double precison;
    cin >> precison;
    myResult = bicyleCTG(argument, precison);
    double result = (double) 1/tan(argument);
    cout << "Math.h result is: " << result << endl;
    cout << "My result is:     " << myResult << endl;
    cout << "Absolute error is: " << fabs(myResult-result) << endl;
    cout << "Relative error is: " << fabs(fabs(myResult-result)*100/result) << "%\nPress any key." << endl;
    _getch();
    glutInit(&argc, argv);
    glutCreateWindow("Graph");
    glutInitWindowSize(1, 1);
    glutReshapeWindow(800, 600);
    glutInitWindowPosition(0, 0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

//2 0.1
//1 0.0~01

double bicyleCTG(double arg, double precision){
    if(arg > -M_PI && arg < M_PI){
        double dBernoulli[MAX_INDEX+1];
        Maths::NumberTheory::bernoulli_B(MAX_INDEX, dBernoulli);
        int n = 1;
        float u = 0;
        float sum = 0;
        do{
            float numerator = pow(2, 2*n) * pow(arg, (2*n-1)) * fabs(dBernoulli[2*n]);
            float denumerator = factorial(2*n);
            u = (float) numerator/denumerator;
            sum += u;
            //cout << dBernoulli[2*n] << endl;
            cout << "#" << n << ": " << u << endl;
            n++;
        }while(u > precision);
        float result = (float) 1/arg - sum;
        return result;
    }else{
        return 0;
    }
}

int factorial(int n){
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
        glVertex2f(-1.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);
        glVertex2f(0.0f, -1.0f);
        glVertex2f(0.0f, 1.0f);
    glEnd();

    glBegin(GL_LINES);
    for (float i = 0.0f; i<M_PI; i+=0.1) {
        glVertex2f(i, 0.01f);
        glVertex2f(i, -0.01f);
    }
    for (float i = 0.0f; i>-M_PI; i-=0.1) {
        glVertex2f(i, 0.01f);
        glVertex2f(i, -0.01f);
    }
    glEnd();

    glBegin(GL_LINES);
    for (float i = 0.0f; i<M_PI; i+=0.1) {
        glVertex2f(0.01f, i);
        glVertex2f(-0.01f, i);
    }
    for (float i = 0.0f; i>-M_PI; i-=0.1) {
        glVertex2f(0.01f, i);
        glVertex2f(-0.01f, i);
    }
    glEnd();

    glRasterPos2f(0.1f, -0.05f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '1');
    glRasterPos2f(-0.05f, 0.1f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '1');

    glBegin(GL_LINE_STRIP);
        glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
        for (float x = -M_PI; x <= M_PI; x += 0.01) {
            float y = 1 / tan(x) / ZOOM;
            glVertex2f(x /ZOOM, y);
        }
    glEnd();


    glBegin(GL_LINE_STRIP);
        glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
        glVertex2f(0.0f, myResult/ZOOM);
        glVertex2f(argument/ZOOM, myResult/ZOOM);
        glVertex2f(argument/ZOOM, 0.0f);
    glEnd();

    glFlush();
}
