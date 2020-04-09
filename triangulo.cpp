/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
/* Program entry point */
void inicio()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-5,5,-5,5);
    glClearColor(0.0,0.0,0.0,0.0);
}
void pantalla()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,1.0,0.0);
    // GRAFICAR UN TRIANGULO
    /*
    //METODO 1 - LINEA POR LINEA
    glBegin(GL_LINES);
    glVertex2f(1.5,2); glVertex2i(-1,-1);
    glVertex2i(-1,-1); glVertex2i(3,-1);
    glVertex2i(3,-1); glVertex2f(1.5,2);

    //METODO 2 - UNE LOS PUNTOS EN ORDEN
    glBegin(GL_LINE_STRIP);
    glVertex2f(1.5,2); glVertex2i(-1,-1);
    glVertex2i(3,-1); glVertex2f(1.5,2);
    */
    //METODO 3 - UNE LOS PUNTOS AUTOMATICAMENTE
    glBegin(GL_LINE_LOOP);
    glVertex2f(1.5,2); glVertex2i(-1,-1);
    glVertex2i(3,-1);

    glEnd();
    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(200,200);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Practica II,2 de OpenGL");
    inicio();
    glutDisplayFunc(pantalla);
    glutMainLoop();
    return EXIT_SUCCESS;
}
