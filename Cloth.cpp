//
//  Cloth.cpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/12/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//

#include "Cloth.h"

Cloth::Cloth (int width, int height, double mass, Vector3d dampFactors, Vector3d sprConstants, double pacing){
    this->setWidth(width);
    this->setHeight(height);
    this->setMass(mass);
    double xCenter = 0;
    double yCenter = 0;
    double zCenter = 0;
    particles = new Particle*[width];
    for (int i = 0; i < width; ++i){
        particles[i] = new Particle[height];
        particles[i][height-1] = Particle(xCenter+i*pacing, yCenter+(height-1)*pacing, zCenter);
        particles[i][height-1].setMass(mass);
        particles[i][height-1].setVel(Vector3d(0,0,0));
        particles[i][height-1].setFixed(false);
        
    }
    particles[0][height-1].setFixed(true);
    //particles[5][height-1].setFixed(true);
    //particles[width-2][height-1].setFixed(true);
    particles[width-1][height-1].setFixed(true);
    
    
    for (int i = 0; i < width; ++i){
        for (int j = 0; j < height-1; ++j){
            particles[i][j] = Particle(xCenter+i*pacing, yCenter+j*pacing, zCenter);
            particles[i][j].setMass(mass);
            particles[i][j].setVel(Vector3d(0,0,0));
            particles[i][j].setFixed(false);
        }
    }
    //  TODO initialize spring
    
    // structural springs
    double structralDamp = dampFactors[0];
    double structuralK = sprConstants[0];
    this->structural_springs = new Spring[2*(width-1)*(height-1) + (width + height - 2)];
    int springIndex = 0;
    for (int i = 0; i < width-1; ++i){
        for (int j = 0; j < height-1; ++j){
            this->structural_springs[springIndex] = Spring(i, j, i+1, j, structuralK, structralDamp );
            springIndex++;
            this->structural_springs[springIndex] = Spring(i, j, i, j+1, structuralK, structralDamp );
            springIndex++;
        }
    }
    for (int w = 0; w < width-1; ++w){
        this->structural_springs[springIndex] = Spring(w, height-1, w+1, height-1, structuralK, structralDamp );
        springIndex++;
    }
    for (int h = 0; h < height-1; ++h){
        this->structural_springs[springIndex] = Spring(width-1, h, width-1, h+1, structuralK, structralDamp );
        springIndex++;
    }
    
    // sheer springs
    double sheerDamp = dampFactors[1];
    double sheerK = sprConstants[1];
    this->sheer_springs = new Spring[2*(width-1)*(height-1)];
    springIndex = 0;
    for (int i = 0; i < width-1; ++i){
        for (int j = 0; j < height-1; ++j){
            this->sheer_springs[springIndex] = Spring(i, j, i+1, j+1, sheerK, sheerDamp );
            springIndex++;
        }
    }
    for (int i = 1; i < width; ++i){
        for (int j = 0; j < height-1; ++j){
            this->sheer_springs[springIndex] = Spring(i, j, i-1, j+1, sheerK, sheerDamp );
            springIndex++;
        }
    }
    
    // bend springs
    double bendDamp = dampFactors[2];
    double bendK = sprConstants[2];
    this->bend_springs = new Spring[(width-2)*height + (height - 2)* width];
    springIndex = 0;
    for (int i = 0; i < width-2; ++i){
        for (int j = 0; j < height; ++j){
            this->bend_springs[springIndex] = Spring(i, j, i+2, j, bendK, bendDamp );
            springIndex++;
        }
    }
    for (int j = 0; j < height-2; ++j){
        for (int i = 0; i < width; ++i){
            this->bend_springs[springIndex] = Spring(i, j, i, j+2, bendK, bendDamp );
            springIndex++;
        }
    }
}

Cloth::Cloth(){
    
}

Cloth::~Cloth(){
    
}


Cloth::Cloth (const Cloth& Cloth){
    
}

//Cloth&  Cloth:: operator= (const Cloth& oldCloth){
//    if (this != &oldCloth) { // self-assignment check expected
//        this->width = oldCloth.width;
//        this->height = oldCloth.height;
//        this->paticle_mass = oldCloth.paticle_mass;
//        this->structural_springs = oldCloth.structural_springs;
//        this->sheer_springs = oldCloth.sheer_springs;
//        this->bend_springs = oldCloth.bend_springs;
//         printf(" in Cloth copy constructor : width %d, height %d \n", this->width, this->height);
//    }
//    return *this;
//}