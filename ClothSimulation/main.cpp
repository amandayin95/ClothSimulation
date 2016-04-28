//
//  main.cpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/10/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <glut/glut.h>
#include "Cloth.h"
#include "Control.hpp"
#include "Particle.h"
#include "main.hpp"
#include <math.h>
#include <vector>


using namespace std;

// window size
int windowWidth=800;
int windowHeight=800;
bool ambientLightOn = true;
bool pointLightOn = true;
int windEffect = 0;
double phi = 0.;
double theta = 0.;


double cameraX = 30, cameraY = 50, cameraZ = 60;
double cameraLookAtX = 0, cameraLookAtY = 0, cameraLookAtZ = 0;
double cameraDisplacementX = 0, cameraDisplacementZ = 0;

bool firstPersonView = false;
bool headLightOn = true;
double robotX = 0, robotZ = 0;
double robotHeadPhi = 0, robotHeadTheta = 0;
double robotShoulderTheta = 0, robotShoulderPhi = 0;
double robotElbowTheta = 0, robotElbowPhi = 0;
double robotWristTheta = 0, robotWristPhi = 0;

bool doArmTrick = false;
int armTrickIndex = -45;
int armTrickLoopIndex = 0;


// first cloth object
Cloth my_cloth;
int cloth_width = 20;
int cloth_height = 20;
double particle_mass = 0.01;
double time_step = 0.1;
double pacing = 0.5;
Vector3d damp_factors = Vector3d(0.01,0.01,0.01);
Vector3d spring_constants = Vector3d(0.1,0.1,0.1);

// second cloth object
Cloth second_cloth;
int cloth2_width = 20;
int cloth2_height = 20;
double particle_mass2 = 0.01;
double time_step2 = 0.1;
double pacing2 = 0.5;
Vector3d damp_factors2 = Vector3d(0.01,0.01,0.01);
Vector3d spring_constants2 = Vector3d(0.1,0.1,0.1);


// 1 0.75 0.79


void display();
void init();
void reshape(int width, int height);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void drawLine();
void drawCloth();
void updateCloth(int value);

int main(int argc, char **argv)
{
    
    // initialize glut
    glutInit(&argc, argv);
    
    // set window size
    glutInitWindowSize(windowWidth,windowHeight);
    
    // establish glut display parameters
    glutInitDisplayMode(GLUT_DOUBLE   | GLUT_RGB  |GLUT_DEPTH|GLUT_STENCIL);
    
    // create window
     glutCreateWindow("~Cloth Simulation~");
    
    // register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard_func);
//    glutKeyboardUpFunc(keyboardup);
    
    // initalize opengl parameters
    init();
    
    // loop until something happens
    glutMainLoop();
    return 0;
}

void init(){
    // initialize viewing system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20.0, 1.0, .05, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // initialize background color to black
    glClearColor(0,0,0,0);
    glClearStencil(0);
    
    // enable light0 and lighting
    GLfloat white[] = {1,1,1,0};
    GLfloat green[] = {0,1,0,0};// light color
    glEnable(GL_LIGHTING);
    
    glShadeModel(GL_SMOOTH);
    
    // set color of light0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);   // set diffuse light color
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);  // set specular light color
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.3);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0);
    
    // initialize parameters for cloth
    my_cloth = Cloth(cloth_width, cloth_height, particle_mass, damp_factors, spring_constants, pacing);
    second_cloth = Cloth(cloth2_width, cloth2_height, particle_mass2, damp_factors2, spring_constants2, pacing2);
    
    glEnable(GL_DEPTH_TEST);
}

void reshape(int width, int height)
{
    if (width<height)
        glViewport(0,0,width,width);
    else
        glViewport(0,0,height,height);
    
}

void display(){
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // initialize modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // set viewpoint position/orientation
    double currentY = std::max(cameraY*cos(theta*3.14/180.0), minY);
    gluLookAt(
              cameraX*sin(phi*3.14/180.0) + cameraDisplacementX, currentY, cameraZ*cos(phi*3.14/180.0)+cameraDisplacementZ,
              cameraDisplacementX, cameraLookAtY, cameraDisplacementZ,
              0,1,0);
    
//    if (ambientLightOn) {
//        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat[]){.3, .3, .3, 1});
//    } else {
//        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat[]){0,0,0,0});
//    }
//    
    // position of light0
    GLfloat lightPosition[]={0, 50, 60, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    if (pointLightOn) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }
    
    drawLine();
    drawCloth();
    if (1){
        updateCloth(0);
        updateCloth(1);
        glutPostRedisplay();
    }
    // swap buffers
    glutSwapBuffers();
}

void drawLine(){
    glPushMatrix();
    GLfloat c1[] = {0.3, 0.3, 0.9, 0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c1);
    glBegin(GL_QUAD_STRIP);
    Particle p1 = my_cloth.getParAt(0, cloth_height-1);
    glVertex3f(p1.getX()-15, p1.getY(), p1.getZ());
    glVertex3f(p1.getX()-15, p1.getY()+0.2, p1.getZ());
    Particle p2 = my_cloth.getParAt(cloth_width-1, cloth_height-1);
    glVertex3f(p2.getX()+15, p2.getY(), p2.getZ());
    glVertex3f(p2.getX()+15, p2.getY()+0.2, p2.getZ());
    glEnd();
    glPopMatrix();
}


void drawCloth(){
    glPushMatrix();
    for(int i = 0; i < cloth_width-1; ++i){
        for (int j = 0; j < cloth_height-1; ++j){
            
            GLfloat c1[] = {0.45, 0.74, 0.96, 0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c1);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c1);
            
            glBegin(GL_TRIANGLES);
            Particle p1 = my_cloth.getParAt(i, j);
            glVertex3f(p1.getX(), p1.getY(), p1.getZ());
            Particle p2 = my_cloth.getParAt(i+1, j);
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
            Particle p3 = my_cloth.getParAt(i, j+1);
            glVertex3f(p3.getX(), p3.getY(), p3.getZ());
            glEnd();
            
            //glColor3f(0, 0, 1);
            glBegin(GL_TRIANGLES);
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
            glVertex3f(p3.getX(), p3.getY(), p3.getZ());
            Particle p4 = my_cloth.getParAt(i+1, j+1);
            glVertex3f(p4.getX(), p4.getY(), p4.getZ());
            glEnd();
        }
    }
    glPopMatrix();
    
    
    glPushMatrix();
    glTranslated(-12, 0, 0);
    for(int i = 0; i < cloth2_width-1; ++i){
        for (int j = 0; j < cloth2_height-1; ++j){
            GLfloat c2[] = {0.8, 0.375, 0.2, 0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c2);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c2);
            
            glBegin(GL_TRIANGLES);
            Particle p1 = second_cloth.getParAt(i, j);
            glVertex3f(p1.getX(), p1.getY(), p1.getZ());
            Particle p2 = second_cloth.getParAt(i+1, j);
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
            Particle p3 = second_cloth.getParAt(i, j+1);
            glVertex3f(p3.getX(), p3.getY(), p3.getZ());
            glEnd();
            
            glBegin(GL_TRIANGLES);
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
            glVertex3f(p3.getX(), p3.getY(), p3.getZ());
            Particle p4 = second_cloth.getParAt(i+1, j+1);
            glVertex3f(p4.getX(), p4.getY(), p4.getZ());
            glEnd();
        }
    }
    glPopMatrix();

}


void updateCloth(int clothIndex){
    
    Cloth currentCloth;
    int localwidth, localheight;
    Particle** p_copy;
    // add gravity
    Vector3d gravity;
    
    if (clothIndex == 0){
        currentCloth = my_cloth;
        gravity = Vector3d(0, -0.1, 0);
    }else if (clothIndex == 1){
        currentCloth = second_cloth;
        gravity = Vector3d(0, -0.5, 0);
    }
    
    localwidth = currentCloth.getWidth();
    localheight = currentCloth.getHeight();
    p_copy = new Particle*[localwidth];
    for (int i = 0; i < localwidth; ++i){
        p_copy[i] = new Particle[localheight];
        for (int j = 0; j < localheight; ++j){
            p_copy[i][j] = currentCloth.getParAt(i, j);
        }
    }
    
    
    
    
    
    Vector3d** forces = new Vector3d*[localwidth];
    for (int i = 0; i < localwidth; ++i){
        forces[i] = new Vector3d[localheight];
        for (int j = 0; j < cloth_height; ++j){
            forces[i][j] = Vector3d(0,0,0);
        }
    }
    
    if (windEffect){
        for (int i = 0; i < localwidth; ++i){
            for (int j = 0; j < localheight; ++j){
                forces[i][j] = Vector3d(0,-0.05,-0.1);
            }
        }
        windEffect -= 1;
    }
   
    
    // loop through structural springs, restlength = 1
    int springIndex = 2*(localwidth-1)*(localheight-1) + (localheight + localwidth - 2);
    Spring* structSpringCopy = currentCloth.getStructuralSprings();
    double structK = structSpringCopy[0].getSpringConst();
    double dampF = structSpringCopy[0].getDampFactor();
    for (int i = 0; i < springIndex; ++i){
        int p1i, p1j, p2i, p2j;
        p1i = structSpringCopy[i].getP1i();
        p1j = structSpringCopy[i].getP1j();
        p2i = structSpringCopy[i].getP2i();
        p2j = structSpringCopy[i].getP2j();
        Vector3d springVector = Vector3d(p_copy[p1i][p1j].getPos(), p_copy[p2i][p2j].getPos());
        Vector3d normSv = springVector;
        normSv.normalize();
        
        forces[p1i][p1j] += (1)*structK*(springVector.length() - pacing)*(normSv);
        forces[p1i][p1j] += (-1)*dampF*(p_copy[p1i][p1j].getVelocity() - p_copy[p2i][p2j].getVelocity() );
        forces[p2i][p2j] += (-1)*structK*(springVector.length() - pacing)*(normSv);
        forces[p2i][p2j] += dampF*(p_copy[p1i][p1j].getVelocity()- p_copy[p2i][p2j].getVelocity());
    }
    
    springIndex = 2*(localwidth-1)*(localheight-1);
    Spring* sheerSpringCopy = currentCloth.getSheerSprings();
    double sheerK = sheerSpringCopy[0].getSpringConst();
    double sheerdampF = sheerSpringCopy[0].getDampFactor();
    for (int i = 0; i < springIndex; ++i){
        int p1i, p1j, p2i, p2j;
        p1i = sheerSpringCopy[i].getP1i();
        p1j = sheerSpringCopy[i].getP1j();
        p2i = sheerSpringCopy[i].getP2i();
        p2j = sheerSpringCopy[i].getP2j();
        Vector3d springVector = Vector3d(p_copy[p1i][p1j].getPos(), p_copy[p2i][p2j].getPos());
        Vector3d normSv = springVector;
        normSv.normalize();
        
        forces[p1i][p1j] += (1)*sheerK*(springVector.length() - pacing*sqrt(2))*(normSv);
        forces[p1i][p1j] += (-1)*sheerdampF*(p_copy[p1i][p1j].getVelocity() - p_copy[p2i][p2j].getVelocity() );
        forces[p2i][p2j] += (-1)*sheerK*(springVector.length() - pacing*sqrt(2))*(normSv);
        forces[p2i][p2j] += sheerdampF*(p_copy[p1i][p1j].getVelocity()- p_copy[p2i][p2j].getVelocity());
    }
    
    
    springIndex = (localwidth-2)*localheight + (localheight - 2)* localwidth;
    Spring* bendSpringCopy = currentCloth.getBendSprings();
    double bendK = bendSpringCopy[0].getSpringConst();
    double benddampF = bendSpringCopy[0].getDampFactor();
    for (int i = 0; i < springIndex; ++i){
        int p1i, p1j, p2i, p2j;
        p1i = bendSpringCopy[i].getP1i();
        p1j = bendSpringCopy[i].getP1j();
        p2i = bendSpringCopy[i].getP2i();
        p2j = bendSpringCopy[i].getP2j();
        Vector3d springVector = Vector3d(p_copy[p1i][p1j].getPos(), p_copy[p2i][p2j].getPos());
        Vector3d normSv = springVector;
        normSv.normalize();
        
        forces[p1i][p1j] += (1)*bendK*(springVector.length() - pacing*2)*(normSv);
        forces[p1i][p1j] += (-1)*benddampF*(p_copy[p1i][p1j].getVelocity() - p_copy[p2i][p2j].getVelocity() );
        forces[p2i][p2j] += (-1)*bendK*(springVector.length() - pacing*2)*(normSv);
        forces[p2i][p2j] += benddampF*(p_copy[p1i][p1j].getVelocity()- p_copy[p2i][p2j].getVelocity());
    }
    
    
    
    for(int i = 0; i < localwidth; ++i){
        for (int j = 0; j < localheight; ++j){
            forces[i][j] += gravity*p_copy[i][j].getMass();
        }
    }
    
    
    
    
    
    

    if (clothIndex == 0){
        // get new position
        for(int i = 0; i < localwidth; ++i){
            for (int j = 0; j < localheight; ++j){
                if (!p_copy[i][j].getFixed()){
                    Point3d newPos = p_copy[i][j].getPos()+p_copy[i][j].getVelocity()*time_step;
                    my_cloth.getParAt(i, j).setPos(newPos);
                    
                    Vector3d newVel = p_copy[i][j].getVelocity() + forces[i][j]/p_copy[i][j].getMass()*time_step;
                    my_cloth.getParAt(i, j).setVel(newVel);
                    
                    my_cloth.getParAt(i, j).setForce(forces[i][j]);
                }
            }
        }

    }else if (clothIndex == 1){
        // get new position
        for(int i = 0; i < localwidth; ++i){
            for (int j = 0; j < localheight; ++j){
                if (!p_copy[i][j].getFixed()){
                    Point3d newPos = p_copy[i][j].getPos()+p_copy[i][j].getVelocity()*time_step;
                    second_cloth.getParAt(i, j).setPos(newPos);
                    
                    Vector3d newVel = p_copy[i][j].getVelocity() + forces[i][j]/p_copy[i][j].getMass()*time_step;
                    second_cloth.getParAt(i, j).setVel(newVel);
                    
                    second_cloth.getParAt(i, j).setForce(forces[i][j]);
                }
            }
        }

    }
    
   
}






