#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
using namespace std;
class Point{
    public:
        GLfloat x, y;
}point;
vector<Point>v_point;
int iPointNum = 0;
int xx1 = 0, xx2 = 0, yy1 = 0, yy2 = 0;
int winWidth = 400, winHeight = 300;
void GetCnk(GLint n, GLint *c){
    GLint i, k;
    for (k = 0; k <= n; k++){
        c[k] = 1;
        for (i = n; i >= k + 1; i--) c[k] = c[k] * i;
        for (i = n - k; i >= 2; i--) c[k] = c[k] / i;
    }
}
void Reshape(int w, int h){
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}
void Menu(){}
void Initial(){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,200.0,0.0,150.0);
}
void GetPointPr(GLint *c, GLfloat t, Point *Pt, int ControlN, Point *ControlP){
    GLint k, n = ControlN - 1;
    GLfloat Bernstein;
    Pt->x = 0.0;
    Pt->y = 0.0;
    for (k = 0; k < ControlN; k++){
        Bernstein = c[k] * pow(t, k) * pow(1 - t, n - k);
        Pt->x += ControlP[k].x * Bernstein;
        Pt->y += ControlP[k].y * Bernstein;
    }
}
void BezierCurve(GLint m, GLint ControlN, Point *ControlP){
    GLint *C, i;
    Point CurvePt;
    C = new GLint[ControlN];
    GetCnk(ControlN - 1, C);
    glBegin(GL_POINTS);
    for (i = 0; i <= m; i++)    {
        GetPointPr(C, (GLfloat)i / (GLfloat)m, &CurvePt, ControlN, ControlP);
        glVertex2f(CurvePt.x, CurvePt.y);
    }
    glEnd();
    delete[] C;
}
void Bezier(){
    glClear(GL_COLOR_BUFFER_BIT);
    vector<Point>::iterator it;
    int n = 0;
    Point ControlP[4];
    for (it = v_point.begin(); it != v_point.end(); it++){
        ControlP[n++] = (*it);
        if (n == 4) break;
    }
    GLint ControlN = n, m = 500;
    glPointSize(2);
    glColor3f(0.0, 0.0, 0.0);
    BezierCurve(m, ControlN, ControlP);
    glBegin(GL_LINE_STRIP);
    for (GLint i = 0; i < n; i++)
      glVertex3f(ControlP[i].x, ControlP[i].y, 0.0);
    glEnd();
    glFlush();
}
void draw(){
    vector<Point>::iterator it;
    glBegin(GL_LINES);
    for (it = v_point.begin() + 1; it != v_point.end(); it++){
        GLint x11 = (*it).x, y11 = (*it).y;
        it--;
        GLint x22 = (*it).x, y22 = (*it).y;
        it++;
        glVertex2i(x11, y11);
        glVertex2i(x22, y22);
    }
    glEnd();
}
void Display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    if (iPointNum >= 1){
        glBegin(GL_LINES);
        glVertex2i(xx1, yy1);
        glVertex2i(xx2, yy2);
        draw();
    }
    glutSwapBuffers();
}
void key(unsigned char key, GLint x, GLint y){
    if (key == 'p'){
        point.x = x;
        point.y = winHeight - y;
        v_point.push_back(point);
        if (iPointNum == 0){
            iPointNum = 1;
            xx1 = x;
            yy1 = winHeight - y;
        }
        else{
            xx1 = xx2; yy1 = yy2;
            iPointNum = 1;
            xx2 = x;
            yy2 = winHeight - y;
        }
    }
    if (key == 'e'){
        glutDisplayFunc(Bezier);
        glutPostRedisplay();
    }
}
void Passive(GLint x, GLint y){
    xx2 = x;
    yy2 = winHeight - y;
    glutPostRedisplay();
}
void menufunc(int data){
    switch (data){
        case 1:
            glutCreateWindow("rubber sheeting");
            Initial();
            break;
        case 2:
            glutCreateWindow("Ellipse");
            Initial();
            break;
        case 3:
            glutCreateWindow("Bezier");
            glutDisplayFunc(Display);
            glutReshapeFunc(Reshape);
            glutKeyboardFunc(key);
            glutPassiveMotionFunc(Passive);
            Initial();
    }
}
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("menu");
    glutDisplayFunc(Menu);
    glutReshapeFunc(Reshape);
    glutCreateMenu(menufunc);
    glutAddMenuEntry("rubber sheeting",1);
    glutAddMenuEntry("ellipse",2);
    glutAddMenuEntry("Bezier", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    Initial();
    glutMainLoop();
    return 0;
}
