//
//  main.cpp
//  CS155Project3-Robot
//
//  Created by 王小天 on 16/3/4.
//  Copyright (c) 2016年 Xiaotian Wang. All rights reserved.
//
//  Author: Xiaotian Wang / Amanda Yin
//

#include <iostream>
#include <glut/glut.h>
#include <math.h>

// -------------------------------------------------------------------
//                        function prototypes
// -------------------------------------------------------------------

void display(void);
void menu_display(void);
void reshape(int width, int height);
void init();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void menu_func(int value);
void keyboard(unsigned char key, int x, int y);
void keyboardup(unsigned char key, int x, int y);

// -------------------------------------------------------------------
//                   basic windows / viewing tuning
// -------------------------------------------------------------------

// window size
int windowWidth=800;
int windowHeight=800;

// viewing angles and zoom
double phi = 0;
double theta = 0;
double zoom = 0;
double camdy = 0;

// current mouse position
GLboolean firstMouse = true;
double lastx = 0;
double lasty = 0;
double offsetx = 0;
double offsety = 0;
float sensitivity = 0.15;

// keyboard control
double q_pressed = 0;
double e_pressed = 0;

// -------------------------------------------------------------------
//                                  menu
// -------------------------------------------------------------------

enum MENU_TYPE
{
    M_OPTIONS_AMB_LIGHT,
    M_OPTIONS_P_LIGHT,
    M_HELP_CONTROL,
};

double amb_light = 1;
double p_light = 1;

int main_window;
int menu_window;

// -------------------------------------------------------------------
//                        function prototypes
// -------------------------------------------------------------------

using namespace std;

int main(int argc, char **argv)
{
    
    // initialize glut
    glutInit(&argc, argv);
    
    // set window size
    glutInitWindowSize(windowWidth,windowHeight);
    
    // establish glut display parameters
    glutInitDisplayMode(GLUT_DOUBLE   | GLUT_RGB  |GLUT_DEPTH);
    
    // create window
    main_window = glutCreateWindow("~Robot Land~");
    
    // register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
    
    
    // initalize opengl parameters
    init();
    
    // loop until something happens
    glutMainLoop();
    return 0;
}


void init()
{
    // initialize viewing system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(24.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // initialize background color to black
    glClearColor(0,0,0,0);
    
    // position of light0
    GLfloat lightPosition[]={10,10,10,1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    // set color of light0
    GLfloat white[] = {1,1,1,0};		      // light color
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);   // set diffuse light color
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);  // set specular light color
    
    // position of light1
    GLfloat lightPosition1[]={-10,10,-10,0};
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
    // set color of light1
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);   // set diffuse light color
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);  // set specular light color
    
    // enable light
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    
    // init ambient lightening
    GLfloat ambientColor[] = {0.2, 0.2, 0.2, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    // enable ambient lighting
    glEnable(GL_LIGHT_MODEL_AMBIENT);
    
    // enable depth buffering
    glEnable(GL_DEPTH_TEST);
}

void reshape(int width, int height)
{
    if (width<height)
        glViewport(0,0,width,width);
    else
        glViewport(0,0,height,height);
    
}


// ----------------------------------------------------------------------------------------------
//                                         Controlling
// ----------------------------------------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        lastx = x;
        lasty = y;
        cout << "Left click with cursor at " << x << " " << y << endl;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        cout << "Finished left click with cursor at " << x << " " << y << endl;
    }
}

void keyboardup(unsigned char key, int x, int y) {
    cout << "key " << key << " released." << endl;
    switch (key) {
        case 'q':
            q_pressed = 0;
            break;
        case 'e':
            e_pressed = 0;
            break;
        case 'z':
            break;
        case 'x':
            break;
            
        default:
            break;
    }
}


void keyboard(unsigned char key, int x, int y) {
    cout << "key " << key << " pressed." << endl;
    switch (key) {
        // zoom in
        case 'q':
            q_pressed = 1;
            zoom += 1;
            if (zoom >= 60) {
                zoom = 60;
            }
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(20.0-zoom*0.3, 1.0, 1.0, 100.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glutPostRedisplay();
            break;
        // zoom out
        case 'e':
            e_pressed = 1;
            zoom -= 1;
            if (zoom <= -60) {
                zoom = -60;
            }
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(20.0-zoom*0.3, 1.0, 1.0, 100.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glutPostRedisplay();
            break;
        // reset zoom facter
        case 'r':
            zoom = 0;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(20.0-zoom*0.3, 1.0, 1.0, 100.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glutPostRedisplay();
            break;
        // move the camera upward
        case 'z':
            camdy += sensitivity*3;
            glutPostRedisplay();
            break;
        // move the camera downward
        case 'x':
            camdy -= sensitivity*3;
            glutPostRedisplay();
            break;
        // see control options
        case 'm':
            cout << "" << endl;
            cout << "" << endl;
            cout << "         ~ Controls ~" << endl;
            cout << "================================" << endl;
            cout << "control menu  -------   'm'" << endl;
            cout << "zoom in       -------   'q'" << endl;
            cout << "zoom out      -------   'e'" << endl;
            cout << "zoom reset    -------   'r'" << endl;
            cout << "camera up     -------   'z'" << endl;
            cout << "camera down   -------   'x'" << endl;
            cout << "" << endl;
            cout << "" << endl;
            break;
            
        default:
            break;
    }
}

void motion(int x, int y)
{
    offsetx = x - lastx;
    offsety = y - lasty;
    lastx = x;
    lasty = y;
    offsetx *= sensitivity;
    offsety *= sensitivity;
    
    phi += offsetx;
    theta += offsety;

    glutPostRedisplay();
}


// ----------------------------------------------------------------------------------------------
//                                             Menu
// ----------------------------------------------------------------------------------------------

void menu_func(int value)
{
    switch (value)
    {
        case M_OPTIONS_AMB_LIGHT: {
            if (amb_light == 1) {
                GLfloat ambientColor[] = {0.0, 0.0, 0.0, 1};
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
                amb_light = 0;
                cout << "Ambient light off." << endl;
            } else {
                GLfloat ambientColor[] = {0.2, 0.2, 0.2, 1};
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
                amb_light = 1;
                cout << "Ambient light on." << endl;
            }
        }
            break;
        case M_OPTIONS_P_LIGHT: {
            if (p_light == 1) {
                glDisable(GL_LIGHT0);
                glDisable(GL_LIGHT1);
                p_light = 0;
                cout << "Point light off." << endl;
            } else {
                glEnable(GL_LIGHT0);
                glEnable(GL_LIGHT1);
                p_light = 1;
                cout << "Point light on." << endl;
            }
        }
            break;
        case M_HELP_CONTROL: {
            cout << "" << endl;
            cout << "" << endl;
            cout << "         ~ Controls ~" << endl;
            cout << "================================" << endl;
            cout << "control menu  -------   'm'" << endl;
            cout << "zoom in       -------   'q'" << endl;
            cout << "zoom out      -------   'e'" << endl;
            cout << "zoom reset    -------   'r'" << endl;
            cout << "camera up     -------   'z'" << endl;
            cout << "camera down   -------   'x'" << endl;
            cout << "" << endl;
            cout << "" << endl;
        }
            break;
        default:
            break;
    }
    
    glutPostRedisplay();
    
    return;
}

int make_menu ()
{
    int options = glutCreateMenu(menu_func);
    glutAddMenuEntry("Toggle Ambient Light", M_OPTIONS_AMB_LIGHT);
    glutAddMenuEntry("Toggle Point Light", M_OPTIONS_P_LIGHT);
    
    int help = glutCreateMenu(menu_func);
    glutAddMenuEntry("Control", M_HELP_CONTROL);
    
    int main = glutCreateMenu(menu_func);
    glutAddSubMenu("Manage Light", options);
    glutAddSubMenu("Help", help);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    return main;
}

void menu_display()
{
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // initialize modelview matrix
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glLoadIdentity();
}


// ----------------------------------------------------------------------------------------------
//                                            Display
// ----------------------------------------------------------------------------------------------

void display()
{
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // initialize modelview matrix
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glLoadIdentity();
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                         all about viewpoint
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // set viewpoint position/orientation
    gluLookAt(25*sin(phi*3.14/180.0),10+camdy,25*cos(phi*3.14/180.0),
              0,25*sin(theta*3.14/180.0)+camdy,0,
              0,1,0);
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                                menu
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    make_menu();
    
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                          light and object
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // =============================================
    //                 setting light
    // =============================================
    
    GLfloat white[] = {1,1,1,0};
    
    // =============================================
    //             draw a red triangle
    // =============================================
    GLfloat red[] = {1,0,0,0};              // red
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    glMaterialfv(GL_FRONT, GL_SPECULAR, red);
    glMateriali(GL_FRONT,GL_SHININESS,0);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(-3,-1,-8);
    glVertex3f(3,-1,-10);
    glVertex3f(0,3,-9);
    glEnd();
    glNormal3f(1,0,0);
    
    // =============================================
    //              draw Triangle strip
    // =============================================
    GLfloat green[] = {0,1,0,0};			// green
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    glMaterialfv(GL_FRONT, GL_SPECULAR, green);
    glMateriali(GL_FRONT,GL_SHININESS,10);
    
    double square_size = 1000;
    double drawx = -1*square_size/2;
    double drawy = -1;
    double drawz = -1*square_size/2;
    double originx = drawx;
    double originy = drawy;
    double originz = drawz;
    
    glBegin(GL_QUAD_STRIP);
    for (double j = 0; j < square_size; j++) {
        for (double i = 0; i < square_size; i++) {
            glVertex3f(drawx,drawy,drawz);
            drawz = drawz+1;
            glVertex3f(drawx,drawy,drawz);
            drawx = drawx+1;
            drawz = drawz-1;
            glVertex3f(drawx,drawy,drawz);
            drawz = drawz+1;
            glVertex3f(drawx,drawy,drawz);
            drawz--;
        }
        drawx = originx;
        drawz = originz+j+1;
    }
    
    glEnd();
    glNormal3f(0,1,0);
    
    // =============================================
    //                  draw Sphere
    // =============================================
    GLfloat purple[] = {1,0,1,0};			// purple
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purple);
    glMaterialfv(GL_FRONT, GL_SPECULAR, purple);
    glMateriali(GL_FRONT,GL_SHININESS,80);
    glutSolidSphere(.75,100,100);
    glNormal3f(0,0,1);
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glTranslatef(0, 0, -3);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMateriali(GL_FRONT,GL_SHININESS,50);
    glutSolidSphere(.74,200,100);
    glNormal3f(0,0,1);
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glTranslatef(0, 2, 2);
    glRotatef(25, 6, 5, 4);
    
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    GLfloat rcolor[] = {0.5,0.4,0.3};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, rcolor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, rcolor);
    glMateriali(GL_FRONT,GL_SHININESS,50);
    gluCylinder(quadratic, 0.1f, 0.1f, 10, 32, 32);
    
    // swap buffers
    glutSwapBuffers();
    
}