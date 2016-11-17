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
void specialKey(int key, int x, int y);

int top = 1000;
vector<int> spectrum(1000);

class Thread{
public:
    int main(){
        Serial* SP;
        do{
            SP = new Serial("\\\\.\\COM42");    // adjust as needed
            Sleep(1000);
        }while(!SP->IsConnected());
        cout << "Connection: OK" << endl;

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
            printf("Bytes read: %i\n",readResult);
            incomingData[readResult] = 0;
            printf("%s",incomingData);
            int channel = 0;
            int number = 0;
            //This is serial parser. Could be dangerous, do more testing.
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
            system("cls");
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
    printf("Spectrum viewer startup...\n\n");

    Thread thread;
    thread.run();

    glutInit(&argc, argv);
    glutCreateWindow("Graph");
    glutInitWindowSize(1, 1);
    glutReshapeWindow(1000, 600);
    glutInitWindowPosition(0, 0);
    glutSpecialFunc(specialKey);
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vector<int>::iterator it;
    glColor3f(0, 1, 0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x00FF);
    glBegin(GL_LINES);
        glVertex2f(-0.99f, 0.99f);
        glVertex2f(0.99f, 0.99f);

        glVertex2f(-0.99f, 0.525f);
        glVertex2f(0.99f, 0.525f);

        glVertex2f(-0.99f, 0.05f);
        glVertex2f(0.99f, 0.05f);

        glVertex2f(-0.99f, -0.425f);
        glVertex2f(0.99f, -0.425f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glBegin(GL_LINES); //Vertical and horizontal graph edges
        glVertex2f(-0.9f, 1.0f);
        glVertex2f(-0.9f, -0.9f);
        glVertex2f(-0.9f, -0.9f);
        glVertex2f(2.0f, -0.9f);
    glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_LINES); //spectrum graph
        for(it = spectrum.begin(); it != spectrum.end(); it++){
            float y = 1.0f/top * (*it) - 0.9f;
            float x = 2.0f/1000 * (it-spectrum.begin()) - 0.9f;
            glVertex2f(x, -0.9f);
            glVertex2f(x, y);
        }
    glEnd();

    for(int j = 0; j<4; j++){
        char buffer[20];
        itoa(top-j*(top/4), buffer, 10);
        for(int i = 0; i<strlen(buffer); i++){
            glRasterPos2f(-1.0f+0.015f*i, 0.95f-0.475*j);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, buffer[i]);
        }
    }
    glFinish();
    glutPostRedisplay();
}

void specialKey(int key, int x, int y){
    switch(key){
    case GLUT_KEY_UP:
        top*=2;
        break;
    case GLUT_KEY_DOWN:
        top/=2;
        break;
    }
}
