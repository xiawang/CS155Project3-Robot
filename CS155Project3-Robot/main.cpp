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
#include <vector>
#include <array>

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
void pick();

// daring the robot
float ver[8][3] =
{
    {-1.0,-1.0,1.0},
    {-1.0,1.0,1.0},
    {1.0,1.0,1.0},
    {1.0,-1.0,1.0},
    {-1.0,-1.0,-1.0},
    {-1.0,1.0,-1.0},
    {1.0,1.0,-1.0},
    {1.0,-1.0,-1.0},
};

//additional features
std::vector<std::array<double, 4>> pickable;
double picked = 0;

double obj_01 = 0;
double obj_01_x = 0;
double obj_01_y = 0;
double obj_01_z = 0;

// Set lighting intensity and color
GLfloat qaAmbientLighth[]    = {0.1, 0.1, 0.1, 1.0};
GLfloat qaDiffuseLighth[]    = {1, 1, 1, 1.0};
GLfloat qaSpecularLighth[]    = {1.0, 1.0, 1.0, 1.0};
GLfloat emitLighth[] = {0.9, 0.9, 0.9, 0.01};
GLfloat Noemith[] = {0.0, 0.0, 0.0, 1.0};
// Light source position
GLfloat qaLightPositionh[]    = {-9, 1, 0, 1};
GLfloat qaLightDirectionh[]    = {-1, -1, -1, 0};
GLfloat dirVector0h[]={ 1.0, 0.0, 0.0, 0.0};

double robot_w = 0;
double robot_a = 0;
double robot_s = 0;
double robot_d = 0;

double robot_turn_w = 0;
double robot_turn_s = 0;
double robot_turn_a = 0;
double robot_turn_d = 0;

void quad(int a,int b,int c,int d,GLfloat q_color[4]);
void colorcube(GLfloat q_color[4]);
void drawHead(GLfloat q_color[4]);
void drawBody(GLfloat q_color[4]);


void drawArms1(GLfloat q_color[4],double armd);
void drawArms1u(GLfloat q_color[4]);
void drawArms1d(GLfloat q_color[4]);
void drawArms2(GLfloat q_color[4],double armd);
void drawArms2u(GLfloat q_color[4]);
void drawArms2d(GLfloat q_color[4]);

void drawLegs1(GLfloat q_color[4],double legd);
void drawLegs1u(GLfloat q_color[4]);
void drawLegs1d(GLfloat q_color[4]);
void drawLegs2(GLfloat q_color[4],double legd);
void drawLegs2u(GLfloat q_color[4]);
void drawLegs2d(GLfloat q_color[4]);

void drawHands1(GLfloat q_color[4]);
void drawHands2(GLfloat q_color[4]);
void drawFoot1(GLfloat q_color[4]);
void drawFoot2(GLfloat q_color[4]);

void drawRobot(GLfloat q_color[4], double head_move, double armr, double legr, double arm_angl_u1, double arm_angl_u2, double arm_angl_d, double leg_angl);

void drawFloor();


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
double head_mode = 0;
double temp_phi = 0;
double temp_theta = 0;
double temp_camdy = 0;

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
double h_pressed = 0;

// first person view
double first_person = 0;
double view_changed = 0;


// -------------------------------------------------------------------
//                                  menu
// -------------------------------------------------------------------

enum MENU_TYPE
{
    M_OPTIONS_AMB_LIGHT,
    M_OPTIONS_P_LIGHT,
    M_OPTIONS_D_LIGHT,
    M_OPTIONS_S_LIGHT,
    M_HELP_CONTROL,
    M_VIEW_FIRSTPERSON,
};

double amb_light = 1;
double p_light = 1;
double d_light = 1;
double s_light = 1;

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

void initLighting()
{
    // Set lighting intensity and color
    GLfloat qaAmbientLight1[]    = {0.88, 0.88, 0.88, 1.0};
    GLfloat qaDiffuseLight1[]    = {1, 1, 1, 1.0};
    GLfloat qaAmbientLight2[]    = {0.66, 0.66, 0.66, 1.0};
    GLfloat qaDiffuseLight2[]    = {1, 1, 1, 1.0};
    
    // Light source position
    GLfloat qaLightPosition1[]    = {-2, 5, -18, 1};
    GLfloat qaLightPosition2[]    = {1, -0.9, -7, 1};
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    
    // Set lighting intensity and color
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight1);
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition1);
    ////////////////////////////////////////////////
//    glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2f );
    glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION , 0.4f );
    glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION , 0.32f );
    
    // Set lighting intensity and color
    glLightfv(GL_LIGHT1, GL_AMBIENT, qaAmbientLight2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight2);
    glLightfv(GL_LIGHT1, GL_POSITION, qaLightPosition2);
    ////////////////////////////////////////////////
//    glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION, 5.0 );
    glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION , 0.09f );
//    glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION , 1.3 );
    
    // smooth shading
    glShadeModel(GL_SMOOTH);
    
    // init ambient lightening
    GLfloat ambientColor[] = {0.4, 0.4, 0.4, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    
}


void init()
{
    // initialize viewing system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(25.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // initialize background color to black
    glClearColor(0,0,0,0);
    
    initLighting();
    
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
//        cout << "Left click with cursor at " << x << " " << y << endl;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
//        cout << "Finished left click with cursor at " << x << " " << y << endl;
    }
}

void keyboardup(unsigned char key, int x, int y) {
//    cout << "key " << key << " released." << endl;
    switch (key) {
        case 'q':
            q_pressed = 0;
            break;
        case 'e':
            e_pressed = 0;
            break;
        case 'h':
            h_pressed = 0;
        case 'w':
            robot_turn_w = 0;
            glutPostRedisplay();
            break;
        case 'a':
            robot_turn_a = 0;
            glutPostRedisplay();
            break;
        case 's':
            robot_turn_s = 0;
            glutPostRedisplay();
            break;
        case 'd':
            robot_turn_d = 0;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}


void keyboard(unsigned char key, int x, int y) {
//    cout << "key " << key << " pressed." << endl;
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
        // turn the camera 180 degree
        case 't':
            phi = phi+180;
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
        // only change the direction of the head
        case 'h':
            h_pressed = 1;
            glutPostRedisplay();
            break;
        case 'w':
            robot_w += 1;
            robot_turn_w = 1;
            glutPostRedisplay();
            break;
        case 'a':
            robot_a += 1;
            robot_turn_a = 1;
            glutPostRedisplay();
            break;
        case 's':
            robot_s += 1;
            robot_turn_s = 1;
            glutPostRedisplay();
            break;
        case 'd':
            robot_d += 1;
            robot_turn_d = 1;
            glutPostRedisplay();
            break;
        case 'f':
            if (first_person == 0) {
                first_person = 1;
                cout << "First person view." << endl;
            } else {
                first_person = 0;
                cout << "Third person view." << endl;
            }
            
            glutPostRedisplay();
            break;
        case 'p':
            pick();
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
            cout << "camera back   -------   't'" << endl;
            cout << "" << endl;
            cout << "          ~ Robot ~" << endl;
            cout << "================================" << endl;
            cout << "forward       -------   'w'" << endl;
            cout << "backward      -------   's'" << endl;
            cout << "left          -------   'a'" << endl;
            cout << "right         -------   'd'" << endl;
            cout << "move head     -------   'h' (and move the mouse)" << endl;
            cout << "first person  -------   'f'" << endl;
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
                p_light = 0;
                cout << "Point light off." << endl;
            } else {
                glEnable(GL_LIGHT0);
                p_light = 1;
                cout << "Point light on." << endl;
            }
        }
            break;
        case M_OPTIONS_D_LIGHT: {
            if (d_light == 1) {
                glDisable(GL_LIGHT1);
                d_light = 0;
                cout << "Directional light off." << endl;
            } else {
                glEnable(GL_LIGHT1);
                d_light = 1;
                cout << "Directional light on." << endl;
            }
        }
            break;
        case M_OPTIONS_S_LIGHT: {
            if (s_light == 1) {
                glDisable(GL_LIGHT2);
                s_light = 0;
                cout << "Spot light off." << endl;
            } else {
                glEnable(GL_LIGHT2);
                s_light = 1;
                cout << "Spot light on." << endl;
            }
        }
            break;
        case M_VIEW_FIRSTPERSON: {
            if (first_person == 1) {
                first_person = 0;
                cout << "Third person view." << endl;
            } else {
                first_person = 1;
                cout << "First person view." << endl;
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
            cout << "camera back   -------   't'" << endl;
            cout << "" << endl;
            cout << "          ~ Robot ~" << endl;
            cout << "================================" << endl;
            cout << "forward       -------   'w'" << endl;
            cout << "backward      -------   's'" << endl;
            cout << "left          -------   'a'" << endl;
            cout << "right         -------   'd'" << endl;
            cout << "move head     -------   'h' (and move the mouse)" << endl;
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
    glutAddMenuEntry("Toggle Directional Light", M_OPTIONS_D_LIGHT);
    glutAddMenuEntry("Toggle Spot Light", M_OPTIONS_S_LIGHT);
    
    int help = glutCreateMenu(menu_func);
    glutAddMenuEntry("Control", M_HELP_CONTROL);
    
    int view = glutCreateMenu(menu_func);
    glutAddMenuEntry("First Person View", M_VIEW_FIRSTPERSON);
    
    int main = glutCreateMenu(menu_func);
    glutAddSubMenu("Manage Light", options);
    glutAddSubMenu("Help", help);
    glutAddSubMenu("View", view);
    
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
//                                        Drawing Objects
// ----------------------------------------------------------------------------------------------

void quad(int a,int b,int c,int d,GLfloat q_color[4])
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, q_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, q_color);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,0);
    glNormal3f(0,1,0);
    
    glBegin(GL_QUADS);
    glVertex3fv(ver[a]);
    glVertex3fv(ver[b]);
    glVertex3fv(ver[c]);
    glVertex3fv(ver[d]);
    glEnd();
}

void colorcube(GLfloat q_color[4])
{
    quad(0,3,2,1,q_color);
    quad(2,3,7,6,q_color);
    quad(0,4,7,3,q_color);
    quad(1,2,6,5,q_color);
    quad(4,5,6,7,q_color);
    quad(0,1,5,4,q_color);
}

void drawRobot(GLfloat q_color[4], double head_move, double armr, double legr, double arm_angl_u1, double arm_angl_u2, double arm_angl_d, double leg_angl){
    glPushMatrix();
    glTranslatef(0.0-robot_a+robot_d, 0.91, 0.0-robot_w+robot_s);
    
    
    glPushMatrix();
    glScalef(0.3, 0.3, 0.3);
    glTranslatef(-4.3, 10, 0.0);
    glRotatef(armr*arm_angl_u1, 1, 0, 0); // 18
    drawArms1(q_color,armr*arm_angl_d); // 15
    glPopMatrix();
    glutPostRedisplay();
    
    glPushMatrix();
    glScalef(0.3, 0.3, 0.3);
    glTranslatef(4.3, 10, 0.0);
    glRotatef(-armr*arm_angl_u2, 1, 0, 0); // 8
    drawArms2(q_color,-armr*arm_angl_d); //15
    glPopMatrix();
    glutPostRedisplay();
    
    glPushMatrix();
    glScalef(0.7, 0.7, 0.7);
    glTranslatef(0.0, 6.0, 0.0);
    if (head_move == 1) {
        glRotatef(25*phi*3.14/180.0, 0, 1, 0);
        glRotatef(25*theta*3.14/180.0, 1, 0, 0);
    }
    drawHead(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.7, 0.7, 0.7);
    glTranslatef(0.0, 3.2, 0.0);
    drawBody(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(legr*leg_angl, 1, 0, 0); // 15
    drawLegs1(q_color, legr);
    glPopMatrix();
    glutPostRedisplay();
    
    glPushMatrix();
    glRotatef(-legr*leg_angl, 1, 0, 0); // 15
    drawLegs2(q_color, legr);
    glPopMatrix();
    glutPostRedisplay();
    
    glPopMatrix();
}


void drawHead(GLfloat q_color[4]){
    GLfloat black[] = {0,0,0,0};
    GLfloat yellow[] = {1,1,0,0};
    
    glPushMatrix();
    glScalef(0.9, 1.0, 0.8);
    colorcube(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.1, 0.1, 0.1);
    glTranslatef(0.0, 0.0, -8.0);
    colorcube(black);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.1, 0.1, 0.1);
    glTranslatef(-4.0, 4.0, -8.0);
    colorcube(black);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.1, 0.1, 0.1);
    glTranslatef(4.0, 4.0, -8.0);
    colorcube(black);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.4, 0.067f, 0.067f);
    glTranslatef(0.0, -5.5, -12.0);
    colorcube(black);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.15, 0.15, 0.15);
    glTranslatef(0.0, 8.0, -3.6);
    colorcube(yellow);
    // spotlight for the robot
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, qaAmbientLighth);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, qaDiffuseLighth);
    glLightfv(GL_LIGHT2, GL_POSITION, qaLightPositionh);
    glLightfv(GL_LIGHT2, GL_SPECULAR, qaSpecularLighth);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);// set cutoff angle
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dirVector0h);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.5); // set focusing strength
    glPopMatrix();
}

void drawBody(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.3, 1.7, 0.9);
    colorcube(q_color);
    glPopMatrix();
}

// --------------- first arm ---------------

void drawArms1(GLfloat q_color[4], double armd){
    glRotatef(90, 0, 1, 0);
    
    glPushMatrix();
    drawArms1u(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(20+armd, 0, 0, 1);
    glTranslatef(-0.7, -4.0, 0.0);
    drawArms1d(q_color);
    
    glPushMatrix();
    glTranslatef(0.0, -3.3, 0.0);
    drawHands1(q_color);
    glPopMatrix();
    
    glPopMatrix();
    glutPostRedisplay();
}

void drawArms1u(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 2.0, 1.0);
    colorcube(q_color);
    glPopMatrix();
}

void drawArms1d(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 2.5, 1.0);
    colorcube(q_color);
    glPopMatrix();
}

void drawHands1(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(0.9, 0.7, 0.3);
    colorcube(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.8, 0.75, 0.3);
    glTranslatef(0.0, -2.0, -0.5);
    glRotatef(42, 1, 0, 0);
    colorcube(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.2, 0.62, 0.3);
    glTranslatef(6.0, 0.0, -0.1);
    glRotatef(18, 0, 0, 1);
    colorcube(q_color);
    glPopMatrix();
}


// --------------- second arm ---------------
void drawArms2(GLfloat q_color[4], double armd){
    glRotatef(90, 0, 1, 0);
    
    glPushMatrix();
    drawArms2u(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(20+armd, 0, 0, 1);
    glTranslatef(-0.7, -4.0, 0.0);
    drawArms2d(q_color);
    
    glPushMatrix();
    glTranslatef(0.0, -3.3, 0.0);
    drawHands2(q_color);
    glPopMatrix();
    
    glPopMatrix();
    glutPostRedisplay();
}

void drawArms2u(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 2.0, 1.0);
    colorcube(q_color);
    glPopMatrix();
}

void drawArms2d(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 2.5, 1.0);
    colorcube(q_color);
    glPopMatrix();
}
void drawHands2(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(0.9, 0.7, 0.3);
    colorcube(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.8, 0.75, 0.3);
    glTranslatef(0.0, -2.0, 0.5);
    glRotatef(-42, 1, 0, 0);
    colorcube(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.2, 0.62, 0.3);
    glTranslatef(6.0, 0.0, -0.1);
    glRotatef(18, 0, 0, 1);
    colorcube(q_color);
    glPopMatrix();
}


// --------------- first leg ---------------
void drawLegs1(GLfloat q_color[4], double legd){
    glPushMatrix();
    glRotatef(legd, 1, 0, 0);
    glScalef(0.45, 0.45, 0.45);
    glTranslatef(-1.2, 0.7, -0.2);
    drawLegs1u(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-legd, 1, 0, 0);
    glScalef(0.45, 0.45, 0.45);
    glTranslatef(-1.2, -2.3, -0.2);
    drawLegs1d(q_color);
    glPopMatrix();
    glutPostRedisplay();
    
    glPushMatrix();
    glScalef(0.45, 0.45, 0.45);
    glTranslatef(-1.2, -3.9, -1.0);
    drawFoot1(q_color);
    glPopMatrix();
    glutPostRedisplay();
}

void drawLegs1u(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 1.4, 1.0);
    colorcube(q_color);
    glPopMatrix();
}

void drawLegs1d(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 1.4, 1.0);
    colorcube(q_color);
    glPopMatrix();
}

void drawFoot1(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 0.3, 1.4);
    colorcube(q_color);
    glPopMatrix();
}


// --------------- second leg ---------------
void drawLegs2(GLfloat q_color[4], double legd){
    glPushMatrix();
    glRotatef(legd, 1, 0, 0);
    glScalef(0.45, 0.45, 0.45);
    glTranslatef(1.2, 0.7, -0.2);
    drawLegs1u(q_color);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-legd, 1, 0, 0);
    glScalef(0.45, 0.45, 0.45);
    glTranslatef(1.2, -2.3, -0.2);
    drawLegs1d(q_color);
    glPopMatrix();
    glutPostRedisplay();
    
    glPushMatrix();
    glScalef(0.45, 0.45, 0.45);
    glTranslatef(1.2, -3.9, -1.0);
    drawFoot2(q_color);
    glPopMatrix();
    glutPostRedisplay();
}

void drawLegs2u(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 1.4, 1.0);
    colorcube(q_color);
    glPopMatrix();
}

void drawLegs2d(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 1.4, 1.0);
    colorcube(q_color);
    glPopMatrix();
}

void drawFoot2(GLfloat q_color[4]){
    glPushMatrix();
    glScalef(1.0, 0.3, 1.4);
    colorcube(q_color);
    glPopMatrix();
}


void drawFloor(){
    GLfloat green[] = {0,1,0,0};			// green
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,10);
    
    double tile_size = 100;
    double square_size = 10;
    double drawx = -tile_size*square_size/2;
    double drawy = -1;
    double drawz = -tile_size*square_size/2;
    
    glNormal3f(0,1,0);
    
    glBegin(GL_QUADS);
    for (double i = 0; i < square_size; i++) {
        for (double j = 0; j < square_size; j++) {
            glVertex3f(drawx + i*tile_size,drawy,drawz +j*tile_size);
            glVertex3f(drawx + i*tile_size,drawy,drawz +(j+1)*tile_size);
            glVertex3f(drawx + (i+1)*tile_size,drawy,drawz+(j+1)*tile_size);
            glVertex3f(drawx + (i+1)*tile_size,drawy,drawz+j*tile_size);
        }
    }
    
    glEnd();
}


// ----------------------------------------------------------------------------------------------
//                                    Additional Features
// ----------------------------------------------------------------------------------------------

void pick(){
    for (int i = 0; i < 1; i++) {
        if (picked == 0) {
            if(fabs(pickable[i][0]-(-robot_a+robot_d)) <= 1 && fabs(pickable[i][2]-(-robot_w+robot_s)) <= 1){
                picked = 1;
//                cout << "picked at x: " << -robot_a+robot_d << " - z: " << -robot_w+robot_s << endl;
            }
        } else {
            picked = 0;
//            cout << "picked at x: " << -robot_a+robot_d << " - z: " << -robot_w+robot_s << endl;
        }
    }
}


// ----------------------------------------------------------------------------------------------
//                                            Display
// ----------------------------------------------------------------------------------------------
double p = 0;
void display()
{
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                           error reports
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    cout << gluErrorString(glGetError()) << endl;
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                           init display
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // initialize modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                         all about viewpoint
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // set viewpoint position/orientation
    // head_mode is the mode that only move the head but not camera
    if (h_pressed == 1) {
        if (head_mode == 0) {
            temp_phi = phi;
            temp_theta = theta;
            temp_camdy = camdy;
            head_mode = 1;
            phi = 0;
            theta = 0;
        }
    } else {
        if (head_mode == 1) {
            phi = temp_phi;
            theta = temp_theta;
            camdy = temp_camdy;
            head_mode = 0;
        } else {
            temp_phi = phi;
            temp_theta = theta;
            temp_camdy = camdy;
        }
    }
    
    // check if first persion perspective
    if (first_person == 1) {
        // view direction
        if (h_pressed == 0) {
            gluLookAt(25*sin(phi*3.14/180.0)-robot_a+robot_d,6+camdy,-25+25*cos(phi*3.14/180.0)-robot_w+robot_s,
                      -robot_a+robot_d,25*sin((theta)*3.14/180.0)+camdy,-26-robot_w+robot_s,
                      0,1,0);
        } else {
            gluLookAt(25*sin(temp_phi*3.14/180.0)-robot_a+robot_d,6+temp_camdy,-25+25*cos(temp_phi*3.14/180.0)-robot_w+robot_s,
                      -robot_a+robot_d,25*sin((temp_theta)*3.14/180.0)+temp_camdy,-26-robot_w+robot_s,
                      0,1,0);
        }

    } else {
        if (h_pressed == 0) {
            gluLookAt(25*sin(phi*3.14/180.0),10+camdy,25*cos(phi*3.14/180.0),
                      0,25*sin(theta*3.14/180.0)+camdy,0,
                      0,1,0);
        } else {
            gluLookAt(25*sin(temp_phi*3.14/180.0),10+temp_camdy,25*cos(temp_phi*3.14/180.0),
                      0,25*sin(temp_theta*3.14/180.0)+temp_camdy,0,
                      0,1,0);
        }
    }
    
    
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
    //                draw triangles
    // =============================================
    GLfloat red[] = {1,0,0,0};              // red
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,40);
    
    glNormal3f(1,0,0);
    glBegin(GL_TRIANGLES);
    glVertex3f(0,-1,-8);
    glVertex3f(3,-1,-10);
    glVertex3f(0,5,-9);
    glEnd();
    
    GLfloat yellow[] = {1,1,0,0};              // yellow
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellow);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,20);
    
    glNormal3f(1,0,0);
    glBegin(GL_TRIANGLES);
    glVertex3f(0,-1,-50);
    glVertex3f(3,-1,-50);
    glVertex3f(0,5,-50);
    glEnd();
    
    // =============================================
    //                  draw quads
    // =============================================
    drawFloor();
    
    // =============================================
    //               draw Sphere objects
    // =============================================
    GLfloat purple[] = {1,0,1,0};			// purple
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, purple);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, purple);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,80);
    glNormal3f(0,0,1);
    
    glPushMatrix();
    if (obj_01 == 0) { // initialization
        pickable.push_back({0,-0.2,4,0}); // initial position
        obj_01 = 1;
    } else { // after initialization
        if (picked == 1) { // if picking the object
            obj_01_x = -robot_a+robot_d;
            obj_01_y = -0.2;
            obj_01_z = -robot_w+robot_s;
//            cout << "left the obj at x: " << obj_01_x << " - z: " << obj_01_z << endl;
            if (fmod(-robot_w+robot_s, 2) == 0) {
                obj_01_z = obj_01_z-2;
            } else {
                obj_01_z = obj_01_z+1;
            }
            pickable[0] = {obj_01_x-1,obj_01_y,obj_01_z,1}; // last position when picking
            obj_01 = 2;
        } else { // if not picking the object
            if (obj_01 == 1) {
                // do nothing
            } else if (obj_01 == 2) {
                obj_01_x = pickable[0][0];
                obj_01_y = pickable[0][1];
                obj_01_z = pickable[0][2];
                pickable[0] = {obj_01_x,obj_01_y,obj_01_z,0};
                obj_01 = 3;
//                cout << "robot at x: " << -robot_a+robot_d << " - z: " << -robot_w+robot_s << endl;
            } else {
                // do nothing
//                cout << "robot at x: " << -robot_a+robot_d << " - z: " << -robot_w+robot_s << endl;
            }
        }
    }
    
    if (pickable[0][3] == 1) { // if picking the object
        // move the object with the left hand
        if (fmod(-robot_w+robot_s, 2) == 0) {
            glTranslatef(-robot_a+robot_d-1, 2, -robot_w+robot_s-2);
        } else {
            glTranslatef(-robot_a+robot_d-1, 1.3, -robot_w+robot_s+0.7);
        }
        
    } else { // if not picking the object
        glTranslatef(pickable[0][0], pickable[0][1], pickable[0][2]);
    }
    
    glutSolidSphere(.75,100,100);
    glPopMatrix();

//
//    glMatrixMode(GL_MODELVIEW);
//    glTranslatef(0, 0, -3);
//    
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
//    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
//    glNormal3f(0,0,1);
//    glutSolidSphere(.74,200,100);
//
//    glMatrixMode(GL_MODELVIEW);
//    
//    GLUquadricObj *quadratic;
//    quadratic = gluNewQuadric();
//    GLfloat rcolor[] = {0.5,0.4,0.3};
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, rcolor);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rcolor);
//    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
//    glNormal3f(0,0,1);
//    glPushMatrix();
//    glRotatef(45, 0, 1, 0);
//    glTranslatef(0, 3, 2);
//    gluCylinder(quadratic, 0.1f, 0.1f, 10, 32, 32);
//    glPopMatrix();
    
    double temp_move1 = -robot_w+robot_s;
    double temp_move2 = -robot_a+robot_d;
    double armr = pow(-1,fmod(temp_move1,2.0));
    double legr = pow(-1,fmod(temp_move1-1,2.0));
    
    double arm_angl_u1 = 18;
    double arm_angl_u2 = 8;
    double arm_angl_d = 15;
    double leg_angl = 15;
    
    glPushMatrix();
    drawRobot(white,h_pressed,armr,legr,arm_angl_u1,arm_angl_u2,arm_angl_d,leg_angl);
    glPopMatrix();
    
    
    // swap buffers
    glutSwapBuffers();
    
}