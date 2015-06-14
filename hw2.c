/*
*     Vahid Mazdeh - 06/14/2015 
*     A 3D scene of the oldest World Wonder, the Egyptian Pyramids of Giza.
*/



#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=80;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=1200;   //  Size of world

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

   // Render a pyramid consists of 4 triangles
static void pyramid(double x, double y, double z,
                     double dx, double dy, double dz,
                     double th) {
   glPushMatrix();                  // Reset the model-view matrix
   
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
 
   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
      // Front
      glColor3f(1, 0.55, 0.34);
      glVertex3f( 0.0f, 2.0f, 0.0f);
      glColor3f(0.730, 0.55, 0.34);
      glVertex3f(-1.0f, 0.0f, 1.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);
 
      // Right
      glColor3f(1, 0.55, 0.34);
      glVertex3f(0.0f, 2.0f, 0.0f);
      glColor3f(0.730, 0.55, 0.34);
      glVertex3f(1.0f, 0.0f, 1.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
 
      // Back
      glColor3f(1, 0.55, 0.34);
      glVertex3f(0.0f, 2.0f, 0.0f);
      glColor3f(0.730, 0.55, 0.34);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
 
      // Left
      glColor3f(1, 0.55, 0.34);
      glVertex3f( 0.0f, 2.0f, 0.0f);
      glColor3f(0.730, 0.55, 0.34);
      glVertex3f(-1.0f,0.0f,-1.0f);
      glVertex3f(-1.0f,0.0f, 1.0f);
   
   glEnd();   // Done drawing the pyramid

   glBegin(GL_QUADS);
      // Bottom
      glColor3f(0.730, 0.55, 0.34);
      glVertex3f(-1.0f,0.0f, 1.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);

   glEnd();
 
   glPopMatrix();
}


/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   glColor3f(0.9, 0.67, 0.440);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}


/*
 *  Draw a sphere (version 2)
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere2(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}



/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,0,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(0.730, 0.55, 0.34);
   glVertex3f(-1,0, 1);
   glVertex3f(+1,0, 1);
   glVertex3f(+1,+2, 1);
   glVertex3f(-1,+2, 1);
   //  Back
   glColor3f(0.730, 0.55, 0.34);
   glVertex3f(+1,0,-1);
   glVertex3f(-1,0,-1);
   glVertex3f(-1,+2,-1);
   glVertex3f(+1,+2,-1);
   //  Right
   glColor3f(0.730, 0.55, 0.34);
   glVertex3f(+1,0,+1);
   glVertex3f(+1,0,-1);
   glVertex3f(+1,+2,-1);
   glVertex3f(+1,+2,+1);
   //  Left
   glColor3f(0.730, 0.55, 0.34);
   glVertex3f(-1,0,-1);
   glVertex3f(-1,0,+1);
   glVertex3f(-1,+2,+1);
   glVertex3f(-1,+2,-1);
   //  Top
   glColor3f(0.730, 0.55, 0.34);
   glVertex3f(-1,+2,+1);
   glVertex3f(+1,+2,+1);
   glVertex3f(+1,+2,-1);
   glVertex3f(-1,+2,-1);
   //  Bottom
   glColor3f(0.730, 0.55, 0.34);
   glVertex3f(-1,0,-1);
   glVertex3f(+1,0,-1);
   glVertex3f(+1,0,+1);
   glVertex3f(-1,0,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2000;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   //  Draw cubes
   // for (i=-1;i<=1;i++)
   //    for (j=-1;j<=1;j++)
   //       for (k=-1;k<=1;k++)
   //          cube(i,j,k , 0.3,0.3,0.3 , 0);


   // drawing the 3 great pyramids of Egypt
   pyramid(0,0,0,      230.124,138.684,230.124, 0);
    
   pyramid(300,0,115,  30,30,30, 0);
   pyramid(300,0,20,  30,30,30, 0);
   pyramid(300,0,-75,  30,30,30, 0);

   pyramid(-500,0,600, 230,136,230, 0);

   cube(-100, 0, 600, 115, 30, 50, 0);
   sphere2(-20, 90, 600, 30);

   pyramid(-900,0,990, 103.327,60.96,103.327, 0);
   
   // drawig the queens' pyramids
   pyramid(-900,0, 1200, 30, 30, 30, 0);
   pyramid(-1000,0, 1200, 30, 30, 30, 0);
   pyramid(-800,0, 1200, 30, 30, 30, 0);

   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}


/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   // zoom in and out
   else if (ch == 'y' || ch == 'Y')
      dim -= 10;
   else if (ch == 'h' || ch == 'H')
      dim += 10;


   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Vahid Mazdeh - hw2 - Egyptian Pyramids");
   // background color
   glClearColor(0.95, 0.810, 0.4, 0.0);
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}