#include <gl/glut.h>
#include <iostream>
using namespace std;
int iPointNum = 0;
int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
int winWidth = 400, winHeight = 300, mode;
void Initial(void){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,200.0,0.0,150.0);
}
void rubber(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    if (iPointNum >= 1){
        glBegin(GL_LINES);
        glVertex2i(x1, y1);
        glVertex2i(x2, y2);
        glEnd();
    }
    glutSwapBuffers();
}
void ChangeSize(int w, int h){
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}
void  onMidPointEllispe(int a,int b){
	int x,y;
    float d1,d2;
    double center_x = (x1 + x2) / 2.;
    double center_y = (y1 + y2) / 2.;
    glTranslated (center_x, center_y, 0);
	glBegin(GL_POINTS);
		x=0;y=b;
		d1=b*b+a*a*(-b+0.25);
		glVertex2i(x,y);
		glVertex2i(-x,-y);
		glVertex2i(-x,y);
		glVertex2i(x,-y);
		while(b*b*(x+1)<a*a*(y-0.5))
		{
			if(d1<=0){
				d1+=b*b*(2*x+3);
				x++;
			}
			else{
				d1+=b*b*(2*x+3)+a*a*(-2*y+2);
				x++; y--;
			}
			glVertex2f(x,y);
			glVertex2f(-x,-y);
			glVertex2f(-x,y);
			glVertex2f(x,-y);
		}
		d2=b*b*(x+0.5)*(x+0.5)+a*a*(y-1)*(y-1)-a*a*b*b;
		while(y>0)
		{
			if(d2<=0){
        		d2+=b*b*(2*x+2)+a*a*(-2*y+3);
				x++; y--;
			}
			else{
				d2+=a*a*(-2*y+3);
				y--;
			}
			glVertex2f(x,y);
			glVertex2f(-x,-y);
			glVertex2f(-x,y);
			glVertex2f(x,-y);
		}
	glEnd();
	glTranslated (-1*center_x, -1*center_y, 0);
}
void Display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    if (iPointNum == 1){
        glBegin(GL_LINES);
        glVertex2i(x1,y1);
        glVertex2i(x2,y1);
        glVertex2i(x2,y1);
        glVertex2i(x2,y2);
        glVertex2i(x2,y2);
        glVertex2i(x1,y2);
        glVertex2i(x1,y2);
        glVertex2i(x1,y1);
    }
    double xx = abs(x2 - x1) / 2.;
    double yy = abs(y2 - y1) / 2.;
    glEnd();
    onMidPointEllispe(xx,yy);
    glutSwapBuffers();
}
void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN){
        if (iPointNum == 0 || iPointNum == 2){
            iPointNum = 1;
            x1 = xMouse;
            y1 = winHeight - yMouse;
        }
        else{
            iPointNum = 2;
            x2 = xMouse;
            if (mode == 0) y2 = winHeight - yMouse;
            else y2 = x2 - x1 + y1;
            glutPostRedisplay();
        }
    }
    if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN){
        iPointNum = 0;
        glutPostRedisplay();
    }
}
void Passive(GLint x, GLint y){
        if (iPointNum == 1){
            x2 = x;
            y2 = winHeight - y;
            glutPostRedisplay();
        }
}
int menu;
void menufunc(int data){
    switch (data){
        case 1:
            glutCreateWindow("rubber sheeting");
            glutDisplayFunc(rubber);
            glutReshapeFunc(ChangeSize);
            glutMouseFunc(MousePlot);
            glutPassiveMotionFunc(Passive);
            Initial();
            break;
        case 2:
            glutCreateWindow("Ellipse");
            glutDisplayFunc(Display);
            glutReshapeFunc(ChangeSize);
            glutMouseFunc(MousePlot);
            glutPassiveMotionFunc(Passive);
            Initial();
            break;
    }
}
void Menu(){}
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("menu");
    glutDisplayFunc(Menu);
    glutReshapeFunc(ChangeSize);
    menu = glutCreateMenu(menufunc);
    glutAddMenuEntry("rubber sheeting",1);
    glutAddMenuEntry("ellipse",2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    Initial();
    glutMainLoop();
    return 0;
}
