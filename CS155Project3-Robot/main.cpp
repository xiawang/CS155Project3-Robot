//
//  main.cpp
//  CS155Project3-Robot
//
//  Created by 王小天 on 16/3/4.
//  Copyright (c) 2016年 Xiaotian Wang. All rights reserved.
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
    
//    menu_window = glutCreateSubWindow(main_window, 100, 100, 600, 600);
//    glutDisplayFunc(menu_display);
    
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
    gluPerspective(26.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // initialize background color to black
    glClearColor(0,0,0,0);
    
    // position of light0
    GLfloat lightPosition[]={10,10,10,0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // set color of light0
    GLfloat white[] = {1,1,1,0};		      // light color
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);   // set diffuse light color
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);  // set specular light color
    
    // enable light0 and lighting
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
    // init ambient lightening
    GLfloat ambientColor[] = {1, 1, 1, 0};
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

void keyboard(unsigned char key, int x, int y) {
    cout << "key " << key << " pressed." << endl;
    switch (key) {
        // zoom in
        case 'q':
            q_pressed = 1;
            zoom += 1;
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
        // see control options
        case 'c':
            cout << "c" << endl;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glutHideWindow();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glutPostRedisplay();
            break;
            
        default:
            break;
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

void menu_func(int value)
{
    switch (value)
    {
        case M_OPTIONS_AMB_LIGHT: {
            if (amb_light == 1) {
                glDisable(GL_LIGHT_MODEL_AMBIENT);
                amb_light = 0;
                cout << "Ambient light off." << endl;
            } else {
                glEnable(GL_LIGHT_MODEL_AMBIENT);
                amb_light = 1;
                cout << "Ambient light on." << endl;
            }
        }
            break;
        case M_OPTIONS_P_LIGHT: {
            if (p_light == 1) {
                glDisable(GL_LIGHTING);
                p_light = 0;
                cout << "Point light off." << endl;
            } else {
                glEnable(GL_LIGHTING);
                p_light = 1;
                cout << "Point light on." << endl;
            }
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
    
    int main = glutCreateMenu(menu_func);
    glutAddSubMenu("Manage Light", options);
    
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
    gluLookAt(25*sin(phi*3.14/180.0),0,25*cos(phi*3.14/180.0),
              0,25*sin(theta*3.14/180.0),0,
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
    
    //Add ambient light
//    GLfloat ambientColor[] = {1, 1, 1, 0};
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    // position of light0
//    GLfloat lightPosition[]={20,20,20,0};
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // set color of light0
//    GLfloat white[] = {1,1,1,0};	        // white
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);   // set diffuse light color
//    glLightfv(GL_LIGHT0, GL_SPECULAR, white);  // set specular light color
    
    // enable light0 and lighting
//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT_MODEL_AMBIENT);
    
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
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
    glMaterialfv(GL_FRONT, GL_SPECULAR, green);
    glMateriali(GL_FRONT,GL_SHININESS,0);
    
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-1000000.0,-1.0,-1000000.0);
    glVertex3f(-1000000.0,-1.0,1000000.0);
    glVertex3f(1000000.0,-1.0,-1000000.0);
    glVertex3f(1000000.0,-1.0,1000000.0);
    glEnd();
    glNormal3f(0,1,0);
    
    // =============================================
    //                  draw Sphere
    // =============================================
    GLfloat purple[] = {1,0,1,0};			// purple
    glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
    glMaterialfv(GL_FRONT, GL_SPECULAR, purple);
    glMateriali(GL_FRONT,GL_SHININESS,50);
    glutSolidSphere(.75,100,100);
    glNormal3f(0,0,1);
    
    // swap buffers
    glutSwapBuffers();
    
}