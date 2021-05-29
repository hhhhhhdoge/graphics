#include <iostream>
#include <GL/glut.h>
using namespace std;
#include "getAxis.h"
#include "setLookAt.h"
#include "getTetrahedron.h"

GLuint objectID, axisID;
int winWidth = 400, winHeight = 200;
static float angle = 45.0;
static GLfloat xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;
static GLsizei iMode = 1;
static point3D s_eye(5.0, 5.0, 5.0), s_at(0.0, 0.0, 0.0);
static point3D A(2.0f, 0.0f, 0.0), B(2.0f, 1.0f, 0.0), C(0.0f, 1.0f, 0.0), D(1.0f, 1.0f, 1.0);
void Initial(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    objectID = getTetrahedron(A, B, C, D);
    axisID = getAxis(3.0);
    glEnable(GL_POLYGON_STIPPLE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void ChangeSize(GLsizei w, GLsizei h){
    winHeight = h;
    winWidth = w;
    glLoadIdentity();
}
void draw(GLsizei mode){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    double tmp = (double)vp[2] / (double)vp[3];
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(mode == 1){
        setLookAt(iMode, s_eye, s_at);
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-5.0, 5.0, -5.0, 5.0, 0.0, 10.0);
     }
     else{
        setLookAt(iMode, angle, s_eye, s_at);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, tmp, 0.0f, 10.0f);
    }
    glMatrixMode(GL_MODELVIEW);
    glCallList(objectID);
    glPushMatrix();
    glCallList(axisID);
    glPopMatrix();
}
void Display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glViewport(0, 0, winWidth / 2, winHeight);
    draw(1);
    glColor3f(0.0, 0.0, 1.0);
    glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
    draw(2);
    glFlush();
}
void SpecialKeys(int key,int x,int y){
    if(key == GLUT_KEY_UP) angle += 10.0f;
    if(key == GLUT_KEY_DOWN) angle -= 10.0f;
    glutPostRedisplay();
}
void ProcessMenu(int value){
    iMode = value;
    angle = 45.0f;
    glutPostRedisplay();
}
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE  | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("multi");
	int menu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Front XOZ(V)", 1);
	glutAddMenuEntry("Vertical XOY(H)", 2);
	glutAddMenuEntry("End YOZ(W)", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSpecialFunc(SpecialKeys);
	Initial();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(Display);
	glutMainLoop();
}
