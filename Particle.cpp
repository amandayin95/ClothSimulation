//
//  Particle.cpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/10/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//

#include "Particle.h"

Particle::Particle (double x, double y, double z){
    position = Point3d(x, y, z);
}

Particle::Particle(){
    // empty default contructor
}

Particle::~Particle(){
    
}

//Particle&	Particle::operator= (const Particle& particle){
//     if (this != &particle) { // self-assignment check expected
//         this->position = particle.position;
//        // this->acceleration = particle.acceleration;
//         this->velocity = particle.velocity;
//         this->mass = particle.mass;
//         this->force = particle.force;
//     }
//    return *this;
//}