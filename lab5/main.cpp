#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>
#include <pthread.h>
#include <GL/glut.h>

using namespace std;

void* callback(void* obj);
void display();

class Thread{
public:
    int main(){
        Serial* SP = new Serial("\\\\.\\COM56");    // adjust as needed

    if (SP->IsConnected())
        printf("We're connected");

    char incomingData[256] = "";			// don't forget to pre-allocate memory
    //printf("%s\n",incomingData);
    int dataLength = 255;
    int readResult = 0;

    while(SP->IsConnected()){
        readResult = SP->ReadData(incomingData,dataLength);
        printf("Bytes read: (0 means no data available) %i\n",readResult);
        incomingData[readResult] = 0;

        printf("%s",incomingData);
        Sleep(500);
    }
        return(0);
    }

    void run(){
        pthread_create(&thread, 0, &callback, this);
    }

    pthread_t thread;
}; // class Thread


void* callback(void* obj){
    static_cast<Thread*>(obj)->main();
    return(0);
} // callback


// application reads from the specified serial port and reports the collected data
int main(int argc, char* argv[]){

    printf("Welcome to the serial test app!\n\n");

    Thread thread;
    thread.run();

    glutInit(&argc, argv);
    glutCreateWindow("Graph");
    glutInitWindowSize(1, 1);
    glutReshapeWindow(800, 600);
    glutInitWindowPosition(0, 0);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
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
    glFlush();
}
