#include <iostream>
#include <time.h>
#include <cstdlib>
#include <GL/glut.h>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.4, 0.4, 1.0);
    glBegin(GL_LINES);
    glVertex2f(110, 40);
    glVertex2f(220, 200);
    glVertex2f(110, 40);
    glVertex2f(220, 40);
    glVertex2f(110, 200);
    glVertex2f(220, 200);
    glEnd();
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(320, 240);
    glutCreateWindow("zgothic.ru - Название окна");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 320, 0, 240);

    glutDisplayFunc(display);
    glutMainLoop();
}
