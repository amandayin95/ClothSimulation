//
//  Control.cpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/27/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//

#include "Control.hpp"

#include <iostream>
#include <glut/glut.h>
#include <math.h>
#include "main.hpp"

#include <fstream>
#include <time.h>


enum {
    M_QUIT = 0,
    M_HELP,
    
    M_FILE_SAVE_IMAGE,
    
    M_OPTIONS_AMBIENT,
    M_OPTIONS_POINT,
    M_OPTIONS_HEADLAMP,
    M_OPTIONS_FPV,
    M_OPTIONS_RESET,
    
    M_LAST_ENUM
};


int  mouse_state  = GLUT_UP;
int  mouse_button = -1;
bool shiftKey     = false;
int globalX, globalY;

enum ModeEnum {
    NO_MODE,
    SHOULDER_MODE,
    WRIST_MODE,
    ELBOW_MODE,
    HEAD_MODE
};

ModeEnum mode = NO_MODE;

int make_menu ()
{
    int options = glutCreateMenu(menu_func);
    glutAddMenuEntry( "Toggle Ambient Light",		M_OPTIONS_AMBIENT);
    glutAddMenuEntry( "Toggle Point Light",	M_OPTIONS_POINT);
    glutAddMenuEntry( "Toggle Head Lamp", M_OPTIONS_HEADLAMP);
    glutAddMenuEntry( "Toggle Viewpoint", M_OPTIONS_FPV);
    glutAddMenuEntry("Reset globals", M_OPTIONS_RESET);
    
    int main = glutCreateMenu(menu_func);
    glutAddSubMenu  ( "Options",			options);
    glutAddMenuEntry( "Help",			M_HELP);
    glutAddMenuEntry( "Quit",			M_QUIT);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    return main;
}


void menu_func (int value)
{
    switch (value)
    {
        case M_QUIT:
            exit(0);
            break;
            
        case M_HELP:
            menu_help();
            break;
            
        case M_OPTIONS_AMBIENT:
            ambientLightOn = !ambientLightOn;
            std::cerr << "Ambient lights " << (ambientLightOn?"on":"off") << std::endl;
            glutPostRedisplay();
            break;
            
        case M_OPTIONS_POINT:
            pointLightOn = !pointLightOn;
            std::cerr << "Point lights " << (pointLightOn?"on":"off") << std::endl;
            glutPostRedisplay();
            break;
            
        case M_OPTIONS_HEADLAMP:
            headLightOn = !headLightOn;
            std::cerr << "Headlamp " << (headLightOn?"on":"off") << std::endl;
            glutPostRedisplay();
            break;
            
        case M_OPTIONS_FPV:
            firstPersonView = !firstPersonView;
            std::cerr << "First Person View " << (firstPersonView?"on":"off") << std::endl;
            glutPostRedisplay();
            break;
            
        case M_OPTIONS_RESET:
            reset_globals();
            break;
            
        default:
            break;
    }
    
    std::cerr << "done!" << std::endl;
}


void change_theta(double amount)
{
    switch (mode) {
        case SHOULDER_MODE:
            robotShoulderTheta += amount; return;
        case ELBOW_MODE:
            robotElbowTheta += amount; return;
        case WRIST_MODE:
            robotWristTheta += amount; return;
        case HEAD_MODE:
            robotHeadTheta += amount; return;
        default:
            return;
    }
}

void change_phi(double amount)
{
    switch (mode) {
        case SHOULDER_MODE:
            robotShoulderPhi += amount; return;
        case ELBOW_MODE:
            robotElbowPhi += amount; return;
        case WRIST_MODE:
            robotWristPhi += amount; return;
        case HEAD_MODE:
            robotHeadPhi += amount; return;
        default:
            return;
    }
}


void keyboard_func (unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'Q':
        case 'q':
            exit(0); break;
        case 'z':
            zoom(false); break;
        case 'Z':
            zoom(true); break;
        case 's':
        case 'S':
            mode = SHOULDER_MODE;
            printf("%u\n", mode);break;
        case 'e':
        case 'E':
            mode = ELBOW_MODE;
            printf("%u\n", mode);break;
        case 'w':
            windEffect = 2; break;
        case 'W':
            mode = WRIST_MODE;
            printf("%u\n", mode);break;
        case 'h':
        case 'H':
            mode = HEAD_MODE;
            printf("%u\n", mode);break;
        case 'T':
            change_theta(2); break;
        case 't':
            change_theta(-2); break;
        case 'p':
            change_phi(-2); break;
        case 'P':
            change_phi(2); break;
        case 'v':
        case 'V':
            firstPersonView = !firstPersonView; break;
        case 'r':
        case 'R':
        case 'a':
        case 'A':
            doArmTrick = true; break;
        default:
            break;
    }
    glutPostRedisplay();
}

void special_keyboard_func (int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT) {
        robotX--;
    }
    if (key == GLUT_KEY_RIGHT) {
        robotX++;
    }
    if (key == GLUT_KEY_UP) {
        robotZ--;
    }
    if (key == GLUT_KEY_DOWN) {
        robotZ++;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    globalX = x;
    globalY = y;
    mouse_button = button;
    mouse_state = state;
    shiftKey = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
}

void motion(int x, int y)
{
    if (mouse_button == GLUT_LEFT_BUTTON && mouse_state == GLUT_DOWN && !shiftKey) {
        mouse_rotate_camera(x, y);
    } else if (mouse_button == GLUT_LEFT_BUTTON && mouse_state == GLUT_DOWN && shiftKey) {
        mouse_translate_camera(x, y);
    }
}

void menu_help ()
{
    std::cerr << "hmc cs155 ray tracer\n\
    \n\
    action                result\n\
    ------                ------\n\
    left mouse            rotate view left-right and up-down\n\
    shift-left mouse      move camera left-right and up-down\n\
    Z                     zoom in\n\
    z                     zoom out\n\
    arrow keys            move robot\n\
    s|S                   set shoulder rotation mode\n\
    w|W                   Add wind effect\n\
    e|E                   set elbow rotation mode\n\
    h|H                   set head rotation mode\n\
    p|P                   decrease/increase phi (circular rotation) (depends on current rotation mode)\n\
    t|T                   decrease/increase theta (up/down rotation) (depends on current rotation mode)\n\
    right mouse           access pop-up menu\n\
    r key                 ride the roller coaster\n\
    v key                 first person viewpoint\n\
    a key                 robot arm trick\n\
    q key                 quit\n"
    << std::endl;
}

void reset_globals()
{
    windowWidth = 600;
    windowHeight = 600;
    
    ambientLightOn = true;
    pointLightOn = true;
    
    phi = 0.;
    theta = 0.;
    
    cameraX = 30;
    cameraY = 50;
    cameraZ = 30;
    
    cameraLookAtX = 0;
    cameraLookAtY = 0;
    cameraLookAtZ = 0;
    
    cameraDisplacementX = 0;
    cameraDisplacementZ = 0;
    
    mouse_state  = GLUT_UP;
    mouse_button = -1;
    shiftKey     = false;
    
    firstPersonView = false;
    headLightOn = true;
    robotX = 0, robotZ = 0;
    robotHeadPhi = 0, robotHeadTheta = 0;
    robotShoulderTheta = 0, robotShoulderPhi = 0;
    robotElbowTheta = 0, robotElbowPhi = 0;
    robotWristTheta = 0, robotWristPhi = 0;
    
    mode = NO_MODE;
    
    glutPostRedisplay();
}


void mouse_rotate_camera (const int x, const int y)
{
    static const double scale = 1;
    int deltaX = globalX - x;
    int deltaY = globalY - y;
    double xPosition = scale * (deltaX % windowWidth);
    double yPosition = scale * (deltaY % windowHeight);
    phi += xPosition / M_2_PI;
    theta += yPosition / M_PI_2;
    theta = std::min(theta, M_PI_2);
    globalX = x;
    globalY = y;
    glutPostRedisplay();
}

void mouse_translate_camera (const int ix, const int iz)
{
    static const double scale = 0.05;
    double dx = (globalX - ix) % windowWidth;
    double dz = (globalY - iz) % windowHeight;
    cameraDisplacementX -= scale*dx;
    cameraDisplacementZ -= scale*dz;
    globalX = ix;
    globalY = iz;
    glutPostRedisplay();
}


void zoom(const bool zoomIn)
{
    cameraX += (zoomIn ? -1 : 1);
    cameraY += (zoomIn ? -1 : 1);
    cameraZ += (zoomIn ? -1 : 1);
    glutPostRedisplay();
}

