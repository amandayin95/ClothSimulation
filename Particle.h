//
//  Particle.hpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/10/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//
class Particle;

#ifndef Particle_H
#define Particle_H

#include "geometry.h"

class Particle
{
public:
    Particle ();
    ~Particle ();
    
    // create Particles with specified characteristics
    Particle (double x, double y, double z);
    
    // copy constructor and assignment operator
    // _deep_ copy!
//    Particle (const Particle& particle);
//    Particle&	operator= (const Particle& particle);
    
    // accessors
    double	getX ()   { return position[0];   }
    double	getY ()   { return position[1];   }
    double	getZ ()   { return position[2];   }
    Point3d getPos()  { return position;      }
    Vector3d getVelocity() {return velocity;}
  //  Vector3d getAccl() {return acceleration;}
    Vector3d getForce() {return force;}

    bool getFixed() {return fixed;}
    double   getMass() { return mass; }
    
    // setters
    void	setX (double newX)           { position[0] = newX;   }
    void	setY (double newY)           { position[1] = newY;   }
    void	setZ (double newZ)           { position[2] = newZ;   }
    void    setPos(Point3d newPos)       { position = newPos;}
    void    setVel(Vector3d newVel)      { velocity = newVel; }
    void    setMass (double newMass)     { mass = newMass; }
    void    setFixed(bool f)         { fixed = f;}
  
    
//    // OpenGL call wrappers ---- in case we need it
//    void		glDrawPixelsWrapper ();
//    void		glTexImage2DWrapper ();
//    void		glReadPixelsWrapper ();
    
    
    
private:
    
    Point3d  position;
    Vector3d  velocity;
   // Vector3d  acceleration;
    Vector3d  force;
    bool fixed;
    
    double		mass;	// mass of the particle
    
};



#endif /* Particle_H */
