#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include "SerialClass.h"
#include <string>
#include <vector>
#include <pthread.h>
#include <GL/glut.h>

using namespace std;

void* callback(void* obj);
void display();

vector<int> spectrum(1000);

class Thread{
public:
    int main(){
        Serial* SP;
        do{
            SP = new Serial("\\\\.\\COM42");    // adjust as needed
            Sleep(1000);
        }while(!SP->IsConnected());

        printf("Connection: OK");
        char junk[1000] = "";
        SP->ReadData(junk, 999);

        char incomingData[256] = "";			// don't forget to pre-allocate memory
        //printf("%s\n",incomingData);
        int dataLength = 255;
        int readResult = 0;
        bool second = false;
        readResult = SP->ReadData(incomingData,dataLength);
        while(SP->IsConnected()){
            readResult = SP->ReadData(incomingData,dataLength);
            printf("Bytes read: (0 means no data available) %i\n",readResult);
            incomingData[readResult] = 0;
            printf("%s",incomingData);
            int channel = 0;
            int number = 0;
            char * pch = strtok(incomingData, "!");
            while(pch != NULL){
                if(!second){
                    cout << "1: " << pch << endl;
                    channel = atoi(pch);
                    pch = strtok(NULL, "\n");
                    second = true;
                }else{
                    cout << "2: " << pch << endl;
                    number = atoi(pch);
                    pch = strtok(NULL, "!");
                    second = false;
                }
            }
            spectrum[channel]+=number;
            Sleep(250);
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
    fill(spectrum.begin(), spectrum.end(), 0);
    printf("Welcome to the serial test app!\n\n");

    Thread thread;
    thread.run();

    glutInit(&argc, argv);
    glutCreateWindow("Graph");
    glutInitWindowSize(1, 1);
    glutReshapeWindow(1000, 600);
    glutInitWindowPosition(0, 0);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vector<int>::iterator it;
    glBegin(GL_LINES);
    for(it = spectrum.begin(); it != spectrum.end(); it++){
        float y = 1.0f/1000 * (*it) - 1.0f;
        float x = 2.0f/1000 * (it-spectrum.begin()) - 1.0f;
        glVertex2f(x, -1.0f);
        glVertex2f(x, y);
    }
    glEnd();
/*
    glBegin(GL_LINES);
        glVertex2f(-1.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);
        glVertex2f(0.0f, -1.0f);
        glVertex2f(0.0f, 1.0f);
    glEnd();
    */
    glFinish();
    glutPostRedisplay();
}
