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
#include "Particle.h"
#include <math.h>
#include <vector>


using namespace std;

// window size
int windowWidth=800;
int windowHeight=800;

// viewing angles and zoom
double phi = 0;
double theta = 0;
double camdy = 0;

int dummy = 0;

// cloth object
Cloth my_cloth;
int cloth_width = 20;
int cloth_height = 20;
double particle_mass = 1.0;
double time_step = 0.1;
Vector3d damp_factors = Vector3d(0,0,0);
Vector3d spring_constants = Vector3d(0.8,0.8,0.8);

void display();
void init();
void reshape(int width, int height);
void drawCloth();
void startMovement();
void updateCloth();

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
//    glutMouseFunc(mouse);
//    glutMotionFunc(motion);
//    glutKeyboardFunc(keyboard);
//    glutKeyboardUpFunc(keyboardup);s
    
    // initalize opengl parameters
    init();
    
    gluLookAt(25*sin(phi*3.14/180.0),10+camdy,25*cos(phi*3.14/180.0),
              0,25*sin(theta*3.14/180.0)+camdy,0,
              0,1,0);
    
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
    
    // initialize parameters for cloth
    Cloth newCloth = Cloth(cloth_width, cloth_height, particle_mass, damp_factors, spring_constants);
    my_cloth.setWidth(newCloth.getWidth());
    my_cloth.setHeight(newCloth.getHeight());
    my_cloth.setParticles(newCloth.getParticles());
    my_cloth.setBendSprings(newCloth.getBendSprings());
    my_cloth.setSheerSprings(newCloth.getSheerSprings());
    my_cloth.setStructuralSprings(newCloth.getStructuralSprings());
    my_cloth.setMass(1);
    printf(" cloth width  %d ", cloth_width);
    printf("width is %d" , my_cloth.getWidth());
    
    
    
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
    
    gluLookAt(25*sin(phi*3.14/180.0),20+camdy,40*cos(phi*3.14/180.0)+40,
              5,25*sin(theta*3.14/180.0)+camdy+10,15,
              0,1,0);
    
    drawCloth();
    if (dummy == 0){
        dummy++;
        startMovement();  //TODO
    }
    // swap buffers
    glutSwapBuffers();
}

void drawCloth(){
    for(int i = 0; i < cloth_width-1; ++i){
        for (int j = 0; j < cloth_height-1; ++j){
            glPushMatrix();
            glColor3f(1, 0, 0);
            glBegin(GL_TRIANGLES);
            Particle p1 = my_cloth.getParAt(i, j);
            glVertex3f(p1.getX(), p1.getY(), p1.getZ());
            Particle p2 = my_cloth.getParAt(i+1, j);
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
            Particle p3 = my_cloth.getParAt(i, j+1);
            glVertex3f(p3.getX(), p3.getY(), p3.getZ());
            glEnd();
            glPopMatrix();
            glPushMatrix();
            glColor3f(0, 0, 1);
            glBegin(GL_TRIANGLES);
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
            glVertex3f(p3.getX(), p3.getY(), p3.getZ());
            Particle p4 = my_cloth.getParAt(i+1, j+1);
            glVertex3f(p4.getX(), p4.getY(), p4.getZ());
            glEnd();
            glPopMatrix();
        }
    }
}

void startMovement(){
    // change particles
    my_cloth.getParAt(1, 0).setZ(1);
    my_cloth.getParAt(5, 10).setZ(1);
    my_cloth.getParAt(7, 8).setZ(1);
    my_cloth.getParAt(19, 12).setZ(1);
    
    while(true){
        updateCloth();
        glutPostRedisplay();
    }
    
}

void updateCloth(){
    Particle** p_copy = my_cloth.getParticles();
    Vector3d** forces = new Vector3d*[cloth_width];
    for (int i = 0; i < cloth_width; ++i){
        forces[i] = new Vector3d[cloth_height];
        for (int j = 0; j < cloth_height; ++j){
            forces[i][j] = Vector3d(0,0,0);
        }
    }
    Vector3d gravity = Vector3d(0, -9.8, 0);
    
    // loop through structural springs, restlength = 1
    int springIndex = 2*(cloth_width-1)*(cloth_height-1) + (cloth_height + cloth_width - 2);
    Spring* structSpringCopy = my_cloth.getStructuralSprings();
    double structK = structSpringCopy[0].getSpringConst();
    double dampF = structSpringCopy[0].getDampFactor();
    for (int i = 0; i < springIndex; ++i){
        int p1i, p1j, p2i, p2j;
        p1i = structSpringCopy[i].getP1i();
        p1j = structSpringCopy[i].getP1j();
        p2i = structSpringCopy[i].getP2i();
        p2j = structSpringCopy[i].getP2j();
        Vector3d springVector = Vector3d(p_copy[p1i][p1j].getPos(), p_copy[p2i][p2j].getPos());
        forces[p1i][p1j] += (-1)*structK*(springVector.length() - 1)*(springVector.normalize());
        forces[p1i][p1j] += (-1)*dampF*(p_copy[p1i][p1j].getVelocity() - p_copy[p2i][p2j].getVelocity() );
        // TODO TOOOOOOOODOOOOOOOOOO!!!!!!!!!!!!
        forces[p2i][p2j] += structK*(springVector.length() - 1)*(springVector.normalize());
        forces[p2i][p2j] += dampF*(p_copy[p1i][p1j].getVelocity()- p_copy[p2i][p2j].getVelocity());
        
        
    }
    
    
    // add gravity
    for(int i = 0; i < cloth_width; ++i){
        for (int j = 0; j < cloth_height; ++j){
            forces[i][j] += gravity*p_copy[i][j].getMass();
        }
    }

    // get new position
    for(int i = 0; i < cloth_width; ++i){
        for (int j = 0; j < cloth_height; ++j){
            Point3d newPos = p_copy[i][j].getPos()+p_copy[i][j].getVelocity()*time_step;
            my_cloth.getParAt(i, j).setPos(newPos);
            
            Vector3d newVel = p_copy[i][j].getVelocity() + forces[i][j]/p_copy[i][j].getMass()*time_step;
            my_cloth.getParAt(i, j).setVel(newVel);
        }
    }
}






