//
//  main.hpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/27/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//

#ifndef main_h
#define main_h

#include <vector>
#include <array>

const double minY = 0.01;

void display(void);
void reshape(int width, int height);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void init();

void drawLine();
void drawCloth();
void updateCloth();

extern bool ambientLightOn;
extern bool pointLightOn;

extern bool firstPersonView;
extern bool headLightOn;
extern bool doArmTrick;
extern double robotX, robotZ;
extern double robotHeadPhi, robotHeadTheta;
extern double robotShoulderTheta, robotShoulderPhi;
extern double robotElbowTheta, robotElbowPhi;
extern double robotWristTheta, robotWristPhi;
extern int windEffect;


extern int windowWidth;
extern int windowHeight;

extern double phi;
extern double theta;
extern double x, y, z;
extern double cameraX, cameraY, cameraZ;
extern double cameraDisplacementX, cameraDisplacementZ;
extern double cameraLookAtX, cameraLookAtY, cameraLookAtZ;

#define clamp(x,a,b)    (  ((a)<(b))				\
? ((x)<(a))?(a):(((x)>(b))?(b):(x))	\
: ((x)<(b))?(b):(((x)>(a))?(a):(x))	\
)


#endif /* main_h */
