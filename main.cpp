/*
Left mouse: rotate;
Right mouse:   menu;
ESC to quit
*/

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#define MAX_TRIANGLES (10)

struct Point {float x[3];   float n[3]; };
struct Triangle {   Point v[3];  };

//Triangle triangleList[MAX_TRIANGLES];
//int triangleCount = 0;
void InitGeometry();

/* Viewer state */
float sphi=90.0, stheta=45.0;
float sdepth = 10;
float zNear=1.0, zFar=100.0;
float aspect = 5.0/4.0;
float xcam = 0, ycam = 0;

long xsize, ysize;
int downX, downY;
bool leftButton = false, middleButton = false;

int i,j;
GLfloat light0Position[] = { 0, 1, 0, 1.0};

int displayMenu, mainMenu;
enum {WIREFRAME, FLATSHADED, SMOOTHSHADED, LIGHTED};
int displayMode = WIREFRAME;

void MyIdleFunc(void) { glutPostRedisplay();} /* things to do while idle */
void RunIdleFunc(void) {   glutIdleFunc(MyIdleFunc); }
void PauseIdleFunc(void) {   glutIdleFunc(NULL); }

GLfloat off[] = {0.0, 0.0, 0.0, 0.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
GLfloat deep_blue[] = {0.1, 0.5, 0.8, 1.0};
GLfloat shiny[] = {50.0};
GLfloat dull[] = {0.0};


void DrawWireframe(void) {
  /* draw Wireframe */
  glDisable(GL_LIGHT1);
  glEnable(GL_LIGHT0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, off);
  glMaterialfv(GL_FRONT, GL_SHININESS, dull);
  glutWireTorus(1.0, 2.0, 30, 30);
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
    case FLATSHADED: DrawFlatShaded();     break;
    case SMOOTHSHADED: DrawSmoothShaded();     break;
    case LIGHTED: DrawLighted();     break;   }
  glutSwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void KeyboardCallback(unsigned char ch, int x, int y) {
  switch (ch) {case 27: exit(0);break; }
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
  glutCreateWindow("Torus surface");
  glEnable(GL_LIGHTING); // new
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glLightfv (GL_LIGHT0, GL_POSITION, light0Position);
  glEnable(GL_LIGHT0);
  glutReshapeFunc(ReshapeCallback);
  glutDisplayFunc(DisplayCallback);
  glutKeyboardFunc(KeyboardCallback);
  glutMouseFunc(MouseCallback);
  glutMotionFunc(MotionCallback);
}

void InitMenu() {
  displayMenu = glutCreateMenu(SetDisplayMenu);
  glutAddMenuEntry("Wireframe", WIREFRAME);
  glutAddMenuEntry("Flat Shaded", FLATSHADED);
  glutAddMenuEntry("Smooth Shaded", SMOOTHSHADED);
  glutAddMenuEntry("Lighted", LIGHTED);
  mainMenu = glutCreateMenu(SetMainMenu);
  glutAddSubMenu("Shading", displayMenu);
  glutAddMenuEntry("Exit", 99);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  InitGL();
  InitMenu();
  glutMainLoop();
}
