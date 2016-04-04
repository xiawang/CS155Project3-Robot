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
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <glut/glut.h>
#include <math.h>
#include <vector>
#include <array>
#include "SOIL.h"

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
void render_obj();
void drawTexturedCube();
void draw_mirror();
void draw_2D_headup(double a, double b, double c, double d);
void draw_text(std::string text, int x, int y, void * font);
void draw_intro_text();
void draw_intro_box();
void draw_intro_start();
void draw_billboard();
void animate(int value);

bool makeFnameWithPath(char* fname, char* pathName, char* fnameWithPath);
bool LoadGLTextures(char* fname);

void billboardCheatSphericalBegin();
void billboardEnd();
void draw_clipping_plane();
void drawFog();

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

// rendering
void render_obj();
void draw_mirror();


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

// roller coaster
std::vector<std::array<double, 3>> rcl;
std::vector<std::array<double, 3>> rcr;
int numPts = 16;
double x1_pt[] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
double x2_pt[] = {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6};
double y_pt[] = {0,1,4,6,5,9.10,11,7,8,5,3,4,6,7,5};
double z_pt[] = {-1,-4,-8,-12,-18,-20,-22,-34,-40,-46,-55,-60,-68,-70,-89,-94};

// animation
int anim = 0;
int startTime = 0;
int prevTime = 0;
int travel = 0;
int startanim = 0;
#define TIMERMSECS 33

// starting menu
int started = 0;

// texture
#define MAX_NO_TEXTURES 1
#define MAX_FILE_NAME 512

// fog
double fog = 0;


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
    M_ANIM_RC,
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
    char* fname="./glass.tga";
    
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
    glutTimerFunc(TIMERMSECS, animate, 0);
    startTime = glutGet(GLUT_ELAPSED_TIME);
    prevTime = startTime;
    
    gluLookAt(25*sin(phi*3.14/180.0),10+camdy,25*cos(phi*3.14/180.0),
              0,25*sin(theta*3.14/180.0)+camdy,0,
              0,1,0);
    
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
    GLfloat qaLightPosition1[]    = {-6, 8, -14, 1};
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
    
    // preload textures
    LoadGLTextures("./glass.tga");
    
    // initialize the light
    initLighting();
    
    // init speciall effect
//    glEnable(GL_FOG);
    
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
        // button 01
        if (x >= 120 && x<= 220 && y >= 565 && y <= 645) {
            cout << "button1" << endl;
            startanim = 1;
        }
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
        case 'F':
            pick();
            if (fog == 0) {
                fog  = 1;
            } else {
                fog = 0;
            }
            cout << fog << endl;
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
            cout << "fog mode      -------   'F'" << endl;
            cout << "" << endl;
            cout << "          ~ Robot ~" << endl;
            cout << "================================" << endl;
            cout << "forward       -------   'w'" << endl;
            cout << "backward      -------   's'" << endl;
            cout << "left          -------   'a'" << endl;
            cout << "right         -------   'd'" << endl;
            cout << "move head     -------   'h' (and move the mouse)" << endl;
            cout << "first person  -------   'f'" << endl;
            cout << "pick/leave    -------   'p'" << endl;
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
            cout << "fog mode      -------   'F'" << endl;
            cout << "" << endl;
            cout << "          ~ Robot ~" << endl;
            cout << "================================" << endl;
            cout << "forward       -------   'w'" << endl;
            cout << "backward      -------   's'" << endl;
            cout << "left          -------   'a'" << endl;
            cout << "right         -------   'd'" << endl;
            cout << "move head     -------   'h' (press and move the mouse)" << endl;
            cout << "first person  -------   'f'" << endl;
            cout << "pick/leave    -------   'p'" << endl;
            cout << "" << endl;
            cout << "" << endl;
        }
            break;
        case M_ANIM_RC: {
            if (startanim == 0) {
                startanim = 1;
                anim = 1;
                cout << "Roller Ccoaster Animation." << endl;
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
    glutAddMenuEntry("Toggle Directional Light", M_OPTIONS_D_LIGHT);
    glutAddMenuEntry("Toggle Spot Light", M_OPTIONS_S_LIGHT);
    
    int help = glutCreateMenu(menu_func);
    glutAddMenuEntry("Control", M_HELP_CONTROL);
    
    int view = glutCreateMenu(menu_func);
    glutAddMenuEntry("First Person View", M_VIEW_FIRSTPERSON);
    
    int animation = glutCreateMenu(menu_func);
    glutAddMenuEntry("Roller Ccoaster", M_ANIM_RC);
    
    int main = glutCreateMenu(menu_func);
    glutAddSubMenu("Manage Light", options);
    glutAddSubMenu("Help", help);
    glutAddSubMenu("View", view);
    glutAddSubMenu("Animation", animation);
    
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

void drawTexturedCube()
{
    glPushMatrix();
    glTranslatef(0,2,0);
    
    glBegin(GL_QUADS);
    // front face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
    glPopMatrix();
    
}

bool makeFnameWithPath(char* fname, char* pathName, char* fnameWithPath)
{
    
    int last = -1;
    for (int i = 0; last==-1 && i < MAX_FILE_NAME; ++i) {
        if (pathName[i] == 0) {
            last=i;
            if (i>0 && pathName[i-1]!='/') {
                fnameWithPath[i]='/';
                last ++;
            }
            
        }
        else {
            fnameWithPath[i]=pathName[i];
        }
    }
    
    // if the pathname exceeds our space bound we return false
    // we could make this more robust by dynamically allocating the right amout of space!
    if (last == -1) {
        return false;
    }
    
    bool done=false;
    
    for (int i=0; !done && i<MAX_FILE_NAME; ++i) {
        fnameWithPath[last+i] = fname[i];
        if (fname[i]==0) {
            done=true;
        }
    }
    return done;
}

bool LoadGLTextures(char* fname)
{
    int textureId = SOIL_load_OGL_texture(fname, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    cout << textureId << endl;
    if(textureId == 0){
        return false;
    }
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    return true;
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
    GLfloat green[] = {0,1,0,0.5};			// green
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


void render_obj()
{
    // -------------------------- triangles --------------------------
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
    
    GLfloat blue[] = {0.5,0.8,0.9,0};              // blue
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,20);
    
    glNormal3f(1,0,0);
    glBegin(GL_TRIANGLES);
    glVertex3f(0,-1,-50);
    glVertex3f(3,-1,-50);
    glVertex3f(0,5,-50);
    glEnd();
    
    
    // -------------------------- sphere --------------------------
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
    
    
    // -------------------------- roller coaster --------------------------
    
    GLfloat grey[] = {0.9,0.9,0.9,0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grey);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,10);
    
    glBegin(GL_LINE_STRIP);
    double xcr, ycr, zcr;   //Points on the Catmull-Rom spline
    
    for(int i = 1; i < numPts-2; i++) {
        for(int k = 0;  k < 50; k++){    //50 points
            float t = k*0.02;  //Interpolation parameter
            //--Eq. (7.34)--
            xcr = x1_pt[i] + 0.5*t*(-x1_pt[i-1]+x1_pt[i+1])
            + t*t*(x1_pt[i-1] - 2.5*x1_pt[i] + 2*x1_pt[i+1] - 0.5*x1_pt[i+2])
            + t*t*t*(-0.5*x1_pt[i-1] + 1.5*x1_pt[i] - 1.5*x1_pt[i+1] + 0.5*x1_pt[i+2]);
            ycr = y_pt[i] + 0.5*t*(-y_pt[i-1]+y_pt[i+1])
            + t*t*(y_pt[i-1] - 2.5*y_pt[i] + 2*y_pt[i+1] - 0.5*y_pt[i+2])
            + t*t*t*(-0.5*y_pt[i-1] + 1.5*y_pt[i] - 1.5*y_pt[i+1] + 0.5*y_pt[i+2]);
            zcr = z_pt[i] + 0.5*t*(-z_pt[i-1]+z_pt[i+1])
            + t*t*(z_pt[i-1] - 2.5*z_pt[i] + 2*z_pt[i+1] - 0.5*z_pt[i+2])
            + t*t*t*(-0.5*z_pt[i-1] + 1.5*z_pt[i] - 1.5*z_pt[i+1] + 0.5*z_pt[i+2]);
            glVertex3f(xcr, ycr, zcr);
            rcl.push_back({xcr, ycr, zcr});
        }
    }
    glEnd();
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grey);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,10);
    glBegin(GL_LINE_STRIP);
    
    for(int i = 1; i < numPts-2; i++) {
        for(int k = 0;  k < 50; k++){    //50 points
            float t = k*0.02;  //Interpolation parameter
            //--Eq. (7.34)--
            xcr = x2_pt[i] + 0.5*t*(-x2_pt[i-1]+x2_pt[i+1])
            + t*t*(x2_pt[i-1] - 2.5*x2_pt[i] + 2*x2_pt[i+1] - 0.5*x2_pt[i+2])
            + t*t*t*(-0.5*x2_pt[i-1] + 1.5*x2_pt[i] - 1.5*x2_pt[i+1] + 0.5*x2_pt[i+2]);
            ycr = y_pt[i] + 0.5*t*(-y_pt[i-1]+y_pt[i+1])
            + t*t*(y_pt[i-1] - 2.5*y_pt[i] + 2*y_pt[i+1] - 0.5*y_pt[i+2])
            + t*t*t*(-0.5*y_pt[i-1] + 1.5*y_pt[i] - 1.5*y_pt[i+1] + 0.5*y_pt[i+2]);
            zcr = z_pt[i] + 0.5*t*(-z_pt[i-1]+z_pt[i+1])
            + t*t*(z_pt[i-1] - 2.5*z_pt[i] + 2*z_pt[i+1] - 0.5*z_pt[i+2])
            + t*t*t*(-0.5*z_pt[i-1] + 1.5*z_pt[i] - 1.5*z_pt[i+1] + 0.5*z_pt[i+2]);
            glVertex3f(xcr, ycr, zcr);
            rcr.push_back({xcr, ycr, zcr});
        }
    }
    glEnd();
    
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grey);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,10);
    
    double tile_size = 2;
    double quads_size = (numPts-3)*50-1;
    
    glNormal3f(0,1,0);
    
    glBegin(GL_QUADS);
    for (double i = 0; i < quads_size; i++) {
        glVertex3f(rcl[i][0], rcl[i][1], rcl[i][2]);
        glVertex3f(rcr[i][0], rcr[i][1], rcr[i][2]);
        glVertex3f(rcr[i+1][0], rcr[i+1][1], rcr[i+1][2]);
        glVertex3f(rcl[i+1][0], rcl[i+1][1], rcl[i+1][2]);
    }
    
    glEnd();
    
    
    // -------------------------- robot --------------------------
    GLfloat white[] = {1,1,1,0};
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
    
    
    // -------------------------- textured --------------------------
    glPushMatrix();
    glTranslatef(+4, -2, 0);
    
    // cube 1
    glEnable(GL_TEXTURE_2D);
    drawTexturedCube();
    glDisable(GL_TEXTURE_2D);
    
    // cube 2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glScalef(0.7, 0.7, 0.7);
    glTranslatef(-2.0, 0.3, -4.0);
    drawTexturedCube();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPopMatrix();
    
}


void draw_mirror()
{
    // -------------------------- mirror --------------------------
    GLfloat gold[] = {0.9,0.9,0,0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gold);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,60);
    
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-5, -1, -16);
    glVertex3f(-1, -1, -16);
    glVertex3f(-1, 7, -16);
    glVertex3f(-5, 7, -16);
    glEnd();
    glPopMatrix();
}


void draw_2D_headup(double a, double b, double c, double d)
{
    // -------------------------- 2D head up display --------------------------
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    
    //* set up a 2d world
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);
    
    // draw a heads up display
    GLfloat green[] = {0,1,0,0}; // green
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,40);
    
    glBegin(GL_QUADS);
    glVertex2d(a, b);
    glVertex2d(a, d);
    glVertex2d(c, d);
    glVertex2d(c, b);
    glEnd();
    
    // restore 3d viewing
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
}


void draw_text(std::string text, int x, int y, void * font)
{
    // display text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    GLfloat white[] = {1,1,1,0}; // white
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,40);
    
    glNormal3f(1,0,0);
    
    glRasterPos2i(x, y);
    
    for (string::iterator i = text.begin(); i != text.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void draw_intro_text()
{
    draw_text("Welcome to the Robot Land!", 20, 530, GLUT_BITMAP_TIMES_ROMAN_24);
    draw_text("Please at first take the roller coaster", 20, 500, GLUT_BITMAP_TIMES_ROMAN_24);
    draw_text("by clicking the button below. Then ", 20, 470, GLUT_BITMAP_TIMES_ROMAN_24);
    draw_text("you can control the robot and do", 20, 440, GLUT_BITMAP_TIMES_ROMAN_24);
    draw_text("some tricks.", 20, 410, GLUT_BITMAP_TIMES_ROMAN_24);
    draw_text("Note that the menu is accessable", 20, 380, GLUT_BITMAP_TIMES_ROMAN_24);
    draw_text("by right clicking of the mouse, or", 20, 350, GLUT_BITMAP_TIMES_ROMAN_24);
    draw_text("by pressing the 'm' button.", 20, 320, GLUT_BITMAP_TIMES_ROMAN_24);
}


void draw_intro_box()
{
    draw_2D_headup(-1, 1, -0.2, 0.6);
    draw_intro_start();
    
    draw_text("ROBOT LAND", 40, 740, GLUT_BITMAP_TIMES_ROMAN_24);
    draw_text("Xiaotian Wang", 130, 700, GLUT_BITMAP_HELVETICA_18);
    draw_text("Amanda Yin", 130, 680, GLUT_BITMAP_HELVETICA_18);
}


void draw_intro_start()
{
    draw_2D_headup(-0.7, -0.6, -0.45, -0.4);
    draw_text("Start!", 145, 195, GLUT_BITMAP_HELVETICA_18);
}


void animate(int value)
{
    if (anim == 0) {
        // comment to disable rc at first
//        anim = 1;
    } else {
        // Set up the next timer tick (do this first)
        if (travel < 13*50) {
            glutTimerFunc(TIMERMSECS, animate, 0);
        }
        
        // Measure the elapsed time
        int currTime = glutGet(GLUT_ELAPSED_TIME);
        int timeSincePrevFrame = currTime - prevTime;
        int elapsedTime = currTime - startTime;
        
        if (travel < 13*50) {
            travel += 1;
        } else {
            travel = 0;
//            cout << "here" << endl;
            anim = 0;
        }
        
        // Force a redisplay to render the new image
        glutPostRedisplay();
        
        prevTime = currTime;
    }
}

void billboardCheatSphericalBegin()
{
    float modelview[16];
    int i,j;
    
    // save the current modelview matrix
    glPushMatrix();
    
    // get the current modelview matrix
    glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    
    // undo all rotations
    // beware all scaling is lost as well
    for( i=0; i<3; i++ )
        for( j=0; j<3; j++ ) {
            if ( i==j )
                modelview[i*4+j] = 1.0;
            else
                modelview[i*4+j] = 0.0;
        }
    
    // set the modelview with no rotations
    glLoadMatrixf(modelview);
}

void billboardEnd()
{
    // restore the previously
    // stored modelview matrix
    glPopMatrix();
}

void draw_billboard()
{
    // -------------------------- billboard --------------------------
    GLfloat red[] = {1,0.4,0.2,0};              // red
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,40);

    billboardCheatSphericalBegin();
    glNormal3f(1,0,0);
    glBegin(GL_TRIANGLES);
    glVertex3f(-7,6,-8);
    glVertex3f(-3,6,-8);
    glVertex3f(-5,9,-8);
    glEnd();
    billboardEnd();
}

void draw_clipping_plane()
{
    // -------------------------- clipping plane --------------------------
    float size=12;
    
    // draw small red floor
    glPushMatrix();
    
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_EQUAL, 0, 3);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
    glVertex3f (-size/4, size/2, -size/2);
    glVertex3f (-size/4, 0, -size/2);
    glVertex3f (size/4, 0, -size/2);
    glVertex3f (size/4, size/2, -size/2);
    glEnd();
    
    glEnable(GL_DEPTH_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    
    // draw big blue floor
    glStencilFunc(GL_EQUAL, 0, 3);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    glColor3f(0,0,1);
    glBegin(GL_QUADS);
    glVertex3f (-size/2, size, -size);
    glVertex3f (-size/2, 0, -size);
    glVertex3f (size/2, 0, -size);
    glVertex3f (size/2, size, -size);
    glEnd();
    
    // glEnable(GL_DEPTH_TEST);
    
    glDisable(GL_STENCIL_TEST);
    
    glPopMatrix();
}

void drawFog() {
    // -------------------------- fog --------------------------
    if (fog == 1) {
        glEnable(GL_FOG);
        glClearColor(0.5f,0.5f,0.5f,1.0f);
    } else {
        glDisable(GL_FOG);
    }
    GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};
    
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.25f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0f);
    glFogf(GL_FOG_END, 100.0f);
    
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
    //                           set viewpoint
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
                      5*sin(phi*3.14/180.0)-robot_a+robot_d,5*sin((theta)*3.14/180.0)+camdy,-26+5*cos(phi*3.14/180.0)-robot_w+robot_s,
                      0,1,0);
        } else {
            cout << "Currently could not move the view." << endl;
            gluLookAt(25*sin(temp_phi*3.14/180.0)-robot_a+robot_d,6+temp_camdy,-25+25*cos(temp_phi*3.14/180.0)-robot_w+robot_s,
                      -robot_a+robot_d,25*sin((temp_theta)*3.14/180.0)+temp_camdy,-26-robot_w+robot_s,
                      0,1,0);
        }

    } else {
        if (anim == 0) {
            if (h_pressed == 0) {
//                                cout << "no animation" << endl;
                //                cout << "x: " << 25*sin(phi*3.14/180.0) << " z: " << 25*cos(phi*3.14/180.0) << endl;
                gluLookAt(25*sin(phi*3.14/180.0),10+camdy,25*cos(phi*3.14/180.0),
                          0,25*sin(theta*3.14/180.0)+camdy,0,
                          0,1,0);
            } else {
                gluLookAt(25*sin(temp_phi*3.14/180.0),10+temp_camdy,25*cos(temp_phi*3.14/180.0),
                          0,25*sin(temp_theta*3.14/180.0)+temp_camdy,0,
                          0,1,0);
            }
        } else {
//            cout << "there" << endl;
            gluLookAt(rcl[travel][0],rcl[travel][1]+0.1,rcl[travel][2],
                      rcl[travel+1][0],rcl[travel+1][1]+0.1,rcl[travel+1][2],
                      0,1,0);
        }
        
    }
    
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                                menu
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    if (anim == 0) {
        make_menu();
    }
    
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                           Introduction
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    if (started == 0) {
        draw_intro_box();
        draw_intro_text();
    }
    
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                               animation
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    if (startanim == 1) {
        startanim = 0;
        if (started == 0) {
            started = 1;
        }
        glutTimerFunc(TIMERMSECS, animate, 0);
    } else {
        // do nothing
    }
    
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                               objects
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // begin reflection
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    
    glEnable(GL_STENCIL_TEST);
    
    // tag the floor
    glStencilFunc(GL_EQUAL, 0, 3);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glBegin(GL_QUADS);
    glVertex3f(-500,-1,500);
    glVertex3f(-500,-1,-500);
    glVertex3f(500,-1,-500);
    glVertex3f(500,-1,500);
    glEnd();
    
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    
    glDisable(GL_STENCIL_TEST);
    
    // draw objects
    render_obj();
    draw_mirror();
    draw_billboard();
    
    // draw "reflected objects"
    glEnable(GL_STENCIL_TEST);
    
    glStencilFunc(GL_EQUAL, 1, 3);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    glPushMatrix();
    glScalef(1,-1,1);
    glTranslatef(0, +2, 0);
    render_obj();
    draw_mirror();
    glPopMatrix();
    
    glDisable(GL_STENCIL_TEST);
    
    // draw blended floor
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    drawFloor();
    
    glDisable(GL_BLEND);
    
    if (fog == 1) {
        drawFog();
    } else {
        glDisable(GL_FOG);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
    }

    // swap buffers
    glutSwapBuffers();
    
}


