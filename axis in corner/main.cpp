#define FREEGLUT_LIB_PRAGMAS 0
#define _USE_MATH_DEFINES

#include <GL\glew.h>
#include <GL\glut.h>
#include <math.h>

int mouseX;
int mouseY;
float cameraX = 0;
float cameraY = 0;
float cameraZ = 100;
float cameraCenterX = 0;
float cameraCenterY = 0;
float cameraCenterZ = 0;

void myDisplay()
{
   int ww = glutGet(GLUT_WINDOW_WIDTH);
   int wh = glutGet(GLUT_WINDOW_HEIGHT);

   glClearColor(0, 0, 0, 0);
   glClear(GL_COLOR_BUFFER_BIT);

   // --- main scene ---

   glViewport(0, 0, ww, wh);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(20, 1, 0.01, 10000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(
      cameraX, cameraY, cameraZ,
      cameraCenterX, cameraCenterY, cameraCenterZ,
      0, 1, 0);

   glBegin(GL_LINES);
   {
      glColor3f(1, 1, 1);
      glVertex3f(-1, -1, 0);
      glVertex3f(10, 10, 0);

      glColor3f(1, 0, 0);
      glVertex3f(-10, 0, 0);
      glVertex3f(10, 0, 0);

      glColor3f(0, 1, 0);
      glVertex3f(0, -10, 0);
      glVertex3f(0, 10, 0);

      glColor3f(0, 0, 1);
      glVertex3f(0, 0, -10);
      glVertex3f(0, 0, 10);
   } glEnd();

   // --- small axes ---

   glViewport(0, 0, 100, 100);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(20, 1, 0.01, 10000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(
      cameraX, cameraY, cameraZ,
      cameraCenterX, cameraCenterY, cameraCenterZ,
      0, 1, 0);

   glBegin(GL_LINES);
   {
      glColor3f(1, 1, 1);
      glVertex3f(-1, -1, 0);
      glVertex3f(10, 10, 0);

      glColor3f(1, 0, 0);
      glVertex3f(-10, 0, 0);
      glVertex3f(10, 0, 0);

      glColor3f(0, 1, 0);
      glVertex3f(0, -10, 0);
      glVertex3f(0, 10, 0);

      glColor3f(0, 0, 1);
      glVertex3f(0, 0, -10);
      glVertex3f(0, 0, 10);
   } glEnd();

   glutSwapBuffers();
}

void myTimer(int value)
{
   glutPostRedisplay();
   glutTimerFunc(1000 / 60, myTimer, 1);
}

void myMotion(int x, int y)
{
   float alpha;
   float beta;

   // --- calculate dx and dy ---
   float a = 0.01;
   float dx = a * (x - mouseX);
   float dy = a * (y - mouseY);

   // --- calculate alpha and beta ---
   float r = sqrtf(cameraX * cameraX + cameraZ * cameraZ);
   float R = sqrtf(cameraX * cameraX + cameraY * cameraY + cameraZ * cameraZ);
   if (cameraZ >= 0)
   {
      alpha = acosf(cameraX / r);
   }
   else
   {
      alpha = -acosf(cameraX / r);
   }
   beta = asinf(cameraY / R);

   // --- calculate new alpha and beta ---
   alpha += dx;
   beta += dy;
   if (alpha > M_PI)
   {
      alpha -= 2 * M_PI;
   }
   else if (alpha < -M_PI)
   {
      alpha += 2 * M_PI;
   }
   if (beta >= M_PI / 2)
   {
      beta = 0.99 * M_PI / 2;
   }
   else if (beta <= -M_PI / 2)
   {
      beta = -0.99 * M_PI / 2;
   }

   // --- calculate new cameraX and cameraY ---
   r = R * cosf(beta);
   cameraX = r * cosf(alpha);
   cameraY = R * sinf(beta);
   cameraZ = r * sinf(alpha);

   // --- update mouseX and mouseY ---
   mouseX = x;
   mouseY = y;
}

void myMouse(int button, int state, int x, int y)
{
   mouseX = x;
   mouseY = y;
}

int main(int argc, char**argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(640, 480);
   glutCreateWindow("OpenGL Project");

   glutDisplayFunc(myDisplay);
   glutMotionFunc(myMotion);
   glutMouseFunc(myMouse);
   myTimer(0);

   glutMainLoop();

   return 0;
}
