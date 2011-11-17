/*
Left mouse: rotate;
Right mouse:   menu;
ESC to quit
*/

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <time.h>
#include <conio.h>
#define MAX_TRIANGLES (10)
using namespace std;

struct Point {float x[3];   float n[3]; };
struct Triangle {   Point v[3];  };

//Triangle triangleList[MAX_TRIANGLES];
//int triangleCount = 0;
void InitGeometry();

/* Viewer state */
float sphi=90.0, stheta=45.0;
float sdepth = 10;
float zNear=0.1, zFar=100.0;
float aspect = 5.0/4.0;
float xcam = 0, ycam = 0;

long xsize, ysize;
int downX, downY;
bool leftButton = false, middleButton = false;

int i,j;
GLfloat light0Position[] = { 0, 1, 0, 1.0};

int displayMenu, mainMenu;
enum {WIREFRAME, FLATSHADED, SMOOTHSHADED, LIGHTED, GIVENFRACTAL};
int displayMode = WIREFRAME;
bool drawGiven, drawFrames, drawFractal;
float step, step1, step2, step3, step4, step5;

void MyIdleFunc(void) {
    //printf("lala");
    Sleep(100);
    if (step < 5.9)
        step = step + 0.05;
    if (step < 1)
    {
        step1 = step;
        step2 = 0;
        step3 = 0;
        step4 = 0;
        step5 = 0;
    }
    else if (step < 2)
    {
        step1 = 1;
        step2 = step - 1;
        step3 = 0;
        step4 = 0;
        step5 = 0;
    }
    else if (step < 3)
    {
        step1 = 1;
        step2 = 1;
        step3 = step - 2;
        step4 = 0;
        step5 = 0;
    }
    else if (step < 4)
    {
        step1 = 1;
        step2 = 1;
        step3 = 1;
        step4 = step - 3;
        step5 = 0;
    }
    else if (step < 5.05)
    {
        step1 = 1;
        step2 = 1;
        step3 = 1;
        step4 = 1;
        step5 = step - 4;
    }
    glutPostRedisplay();
} /* things to do while idle */
void RunIdleFunc(void) {   glutIdleFunc(MyIdleFunc); }
void PauseIdleFunc(void) {   glutIdleFunc(NULL); }

GLfloat off[] = {0.0, 0.0, 0.0, 0.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
GLfloat deep_blue[] = {0.1, 0.5, 0.8, 1.0};
GLfloat shiny[] = {50.0};
GLfloat dull[] = {0.0};

int maxPoints = 12288;
float points[12288];
float sceneSize = 5;
// Number of step
int CurrentNum = 0;
const int MaxNum = 9;
int fractalNo;
void ReadPoints() {
    FILE *fp;   /* file pointer */
    int i, n;

    if((fp=fopen("11_points.txt", "r"))==NULL) {
       printf("Cannot open file.\n");
     }

   for (i = 0; i < maxPoints; i++) {
       if (fscanf(fp, "%f", &points[i]) != 1) {
           break;
       }
   }
   fclose(fp);
   n = i;
   printf("Number read = %d\n", n);

}

void CubeAtPoint(float size, float spread, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(-0.5*spread,-0.5*spread,-0.5*spread);
    glTranslatef(x*spread,y*spread,z*spread);
    float green, red, blue;
    if (x < 0.5) red = 0;
    else red = 1;
    if (y < 0.5) green = 0;
    else green = 1;
    if (z < 0.5) blue = 0;
    else blue = 1;
    if (x < 0.5 & y < 0.5 & z < 0.5)
    {
        red = 0.75;
        green = 0.75;
        blue = 0.75;
    }
    glColor3f(red,green,blue); //(x,y,z);

    glBegin( GL_POINTS );
    glVertex3f(0, 0, 0 );
    glEnd();
    //glutSolidCube (size);
    glPopMatrix();
}
void DrawFrames(void) {

    /* draw Smoth */
    glColor3f (1,0,0);

    glPushMatrix();
        glTranslatef (-0.5,-0.5,-0.5);
        glutSolidCube (0.1);

        glPushMatrix();
            glTranslatef (0.05,0,0);
            glRotatef(90,0,1,0);
            glutSolidCone (0.05,0.25,3,3);
        glPopMatrix();

        glPushMatrix();
            glColor3f (0,1,0);
            glTranslatef (0,0.05,0);
            glRotatef(-90,1,0,0);
            glutSolidCone (0.05,0.25,4,4);
        glPopMatrix();

        glPushMatrix();

            glColor3f (0,0,1);
            glTranslatef (0,0,0.05);
            glutSolidCone (0.05,0.25,4,4);
        glPopMatrix();
    glPopMatrix();
    //glTranslatef (1,1,1);
    glutWireCube (1);

    glPushMatrix();
        glColor3f (0,1,0);
        glTranslatef (-0.25,0.25,-0.25);
        glutWireCube(0.5);
    glPopMatrix();

    glPushMatrix();
        glTranslatef (-0.25,-0.25,-0.25);
        glutWireCube(0.5);
    glPopMatrix();

    glPushMatrix();
        glColor3f (1,0,0);
        glTranslatef (0.25,0.25,-0.25);
        glutWireCube(0.5);
    glPopMatrix();

    glPushMatrix();
        glTranslatef (0.25,-0.25,-0.25);
        glutWireCube(0.5);
    glPopMatrix();
}

void DrawGivenFractal(void) {
    glColor3f (0,1,0);
    for (i = 0; i < maxPoints; i++) {
        CubeAtPoint(0.05, sceneSize, points[i], points[i++], points[i++]);
    }
}
void DrawShape()
{
    glPushMatrix();
       glTranslatef(0.375,0.375,0.375);
       glScalef(0.5,0.5,0.5);
       glutSolidCube (0.5);
       glPushMatrix();
           glTranslatef(0,0.5,0);
           glutSolidCube (0.5);
       glPopMatrix();
       glPushMatrix();
           glTranslatef(0.5,0,0);
           glutSolidCube (0.5);
       glPopMatrix();
       glPushMatrix();
           glTranslatef(0.5,0,0.5);
           glutSolidCube (0.5);
       glPopMatrix();
   glPopMatrix();
}

void DrawFractal(int num)
{
 switch(num) {
   case 0:
        //DrawShape ();
        glTranslatef(0.5,0.5,0.5);
        glutSolidCube (1);
        break;
   default:
        //-|4
        //-|-
        //-|3
        //1|2
        glPushMatrix();
            glPushMatrix();
                glPushMatrix();
                    glPushMatrix();
                        //First fractal
                        if (num == CurrentNum) glColor3f(1,0,0);
                        if (fractalNo == 1 && num == CurrentNum)
                        {
                            glTranslatef(0.0,1-0.5*step1,0.0);
                            glScalef(1-0.5*step2,1-0.5*step2,1-0.5*step2);
                        }
                        else
                        {
                        glTranslatef(0.0,0.5,0.0);
                        glScalef(0.5,0.5,0.5);
                        }
                        //glTranslatef(0.25,0.25,0.25);
                        DrawFractal(num - 1);
                    glPopMatrix();
                    //Second fractal
                    if (num == CurrentNum) glColor3f(0,1,0);
                    if (fractalNo == 2 && num == CurrentNum)
                    {
                        glScalef(1-0.5*step1,1-0.5*step1,1-0.5*step1);
                        glTranslatef(1.0*step1,1.0*step1,0.0);
                        glRotatef(180*step2,0,0,1);
                    }
                    else
                    {
                        glScalef(0.5,0.5,0.5);
                        glTranslatef(1.0,1.0,0.0);
                        glRotatef(180,0,0,1);
                    }
                    //glTranslatef(0.25,0.25,0.25);
                    DrawFractal(num - 1);
                glPopMatrix();
                //Third fractal
                if (num == CurrentNum) glColor3f(1,1,0);
                if (fractalNo == 3 && num == CurrentNum)
                {
                    glTranslatef(0.5*step1,0.0,0.0);
                    glRotatef(90*step1,0,1,0);
                    glScalef(1,1-2*step2,1);
                    glRotatef(90*step3,0,0,1);
                    glScalef(1-2*step4,1,1);
                    glScalef(1-0.5*step5,1-0.5*step5,1-0.5*step5);
                }
                else
                {
                    glTranslatef(0.5,0.0,0.0);
                    glRotatef(90,0,1,0);
                    glScalef(1,-1,1);
                    glRotatef(90,0,0,1);
                    glScalef(-1,1,1);
                    glScalef(0.5,0.5,0.5);
                }
                DrawFractal(num - 1);
            glPopMatrix ();
            //Fourth fractal
            if (num == CurrentNum) glColor3f(0,0,1);
            if (fractalNo == 4 && num == CurrentNum)
            {
                glTranslatef(0.5*step1,0.5*step1,1.0*step1);
                glRotatef(90*step2,0,1,0);
                glScalef(1-0.5*step2,1-0.5*step2,1-0.5*step2);
                glScalef(1,1-2*step3,1);
            }
            else
            {
                glTranslatef(0.5,0.5,1.0);
                glRotatef(90,0,1,0);
                glScalef(0.5,0.5,0.5);
                glScalef(1,-1,1);
            }
            DrawFractal(num - 1);
        glPopMatrix ();
        break;
 }
}

void DrawWireframe(void) {
  /* draw Wireframe */
  glDisable(GL_LIGHT1);
  glEnable(GL_LIGHT0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, off);
  glMaterialfv(GL_FRONT, GL_SHININESS, dull);

  if (drawFrames)
  {
      glPushMatrix();
          glScalef(sceneSize,sceneSize,sceneSize);
          DrawFrames();
      glPopMatrix();
  }
  if (drawFractal)
  {
      glPushMatrix();
          glScalef(sceneSize,sceneSize,sceneSize);
          glTranslatef (-0.5,-0.5,-0.5);
          DrawFractal(CurrentNum);
      glPopMatrix();
  }
  glPushMatrix();
    if (drawGiven) DrawGivenFractal ();
  glPopMatrix();
  RunIdleFunc ();
}

void DrawFlatShaded(void) {
  /* draw FlatShaded */
  glDisable(GL_LIGHT1);
  glEnable(GL_LIGHT0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, off);
  glMaterialfv(GL_FRONT, GL_SHININESS, dull);
  glutSolidTorus(1.0, 2.0, 30, 30);
  //glutSolidCube(3.0);
  PauseIdleFunc ();
}
void DrawSmoothShaded(void) {
  /* draw Smoth */
  glDisable(GL_LIGHT1);
  glEnable(GL_LIGHT0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, off);
  glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
  glutSolidTorus(1.0, 2.0, 30, 30);
}

void DrawLighted(void) {
  /* draw Lighted by Light1 */
  GLfloat ambientIntensity[4]={0.2,0.2,0.2,1.0};
  GLfloat diffuseIntensity[4]={1.0,0.0,0.0,1.0};
  GLfloat pos[4]={1.5,1.0,1.0,0.0};
  glDisable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambientIntensity);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseIntensity);
  glLightfv(GL_LIGHT1, GL_POSITION, pos);
  glutSolidTorus(1.0, 2.0, 30, 30);
}


void ReshapeCallback(int width, int height) {
  xsize = width;
  ysize = height;
  aspect = (float)xsize/(float)ysize;
  glViewport(0, 0, xsize, ysize);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glutPostRedisplay();}

void SetDisplayMenu(int value) {
  displayMode = value;
  switch(value) {
    case WIREFRAME:
        glShadeModel(GL_FLAT);
        break;
    case FLATSHADED:
        glShadeModel(GL_FLAT);
        break;
    case SMOOTHSHADED:
        glShadeModel(GL_SMOOTH);
        break;
    case LIGHTED:
        glShadeModel(GL_SMOOTH);
        break;
    case GIVENFRACTAL:
      glShadeModel(GL_SMOOTH);
      break;
  }
  glutPostRedisplay();
}

void SetMainMenu(int value) {switch(value) {case 99: exit(0); break;}}

void DisplayCallback(void) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(64.0, aspect, zNear, zFar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,-sdepth);
  glRotatef(-stheta, 1.0, 0.0, 0.0);
  glRotatef(sphi, 0.0, 0.0, 1.0);
  switch (displayMode) {
    case WIREFRAME: DrawWireframe();     break;
    case FLATSHADED: fractalNo = 1; DrawWireframe();     break;
    case SMOOTHSHADED: fractalNo = 2;DrawWireframe();     break;
    case LIGHTED: fractalNo = 3; DrawWireframe();     break;
    case GIVENFRACTAL: fractalNo = 4; DrawWireframe();    break;
  }
  glutSwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void KeyboardCallback(unsigned char ch, int x, int y) {
  switch (ch) {
    case ' ':// Space bar
        // Increment the current step number, and tell operating system screen needs redrawing
        CurrentNum = (CurrentNum+1)%MaxNum;
        step = 0;
        break;
    case 'z':
        // Increment the current step number, and tell operating system screen needs redrawing
        if (CurrentNum > 0) CurrentNum = (CurrentNum-1)%MaxNum;
        step = 0;
        break;
    case 'b':
        if (drawFractal == true) drawFractal = false;
            else drawFractal = true;
        break;
    case 'n':
        if (drawGiven == true) drawGiven = false;
            else drawGiven = true;
        break;
    case 'm':
        if (drawFrames == true) drawFrames = false;
            else drawFrames = true;
        break;
    case ',':
        sdepth++;
        //sceneSize++;
        break;
    case '.':
        sdepth--;
        //sceneSize--;
        break;
    case '1':
        fractalNo = 1;
        break;
  case '2':
      fractalNo = 2;
      break;
  case '3':
      fractalNo = 3;
      break;
  case '4':
      fractalNo = 4;
      break;
    case 27: exit(0);break;
  }
  printf("%d ",ch);
  glutPostRedisplay();
}

void MouseCallback(int button, int state, int x, int y) {
  downX = x; downY = y;
  leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));
  middleButton = ((button == GLUT_MIDDLE_BUTTON) &&  (state == GLUT_DOWN));

  glutPostRedisplay();}

void MotionCallback(int x, int y) {
  if (leftButton){sphi+=(float)(x-downX)/4.0;stheta+=(float)(downY-y)/4.0;} // rotate
  if (middleButton){sdepth += (float)(downY - y) / 10.0;  } // scale

  downX = x;   downY = y;
  glutPostRedisplay();}

void InitGL() {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Fraktalizuokimes!");
  glEnable(GL_LIGHTING); // new
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glLightfv (GL_LIGHT0, GL_POSITION, light0Position);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  //glutIdleFunc (myIdleFunc);
  glutReshapeFunc(ReshapeCallback);
  glutDisplayFunc(DisplayCallback);
  glutKeyboardFunc(KeyboardCallback);
  glutMouseFunc(MouseCallback);
  glutMotionFunc(MotionCallback);
}

void InitMenu() {
  displayMenu = glutCreateMenu(SetDisplayMenu);
  glutAddMenuEntry("Be animaciju", WIREFRAME);
  glutAddMenuEntry("Pirmas", FLATSHADED);
  glutAddMenuEntry("Antras", SMOOTHSHADED);
  glutAddMenuEntry("Trecias", LIGHTED);
  glutAddMenuEntry ("Ketvirtas", GIVENFRACTAL);
  mainMenu = glutCreateMenu(SetMainMenu);
  glutAddSubMenu("Shading", displayMenu);
  glutAddMenuEntry("Exit", 99);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
  drawFrames = true;
  drawFractal = true;
  ReadPoints();
  glutInit(&argc, argv);
  InitGL();
  InitMenu();
  glutMainLoop();
}
