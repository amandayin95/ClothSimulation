//
//  Cloth.hpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/12/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//
class Cloth;

#ifndef Cloth_h
#define Cloth_h

#include <stdio.h>
#include "Spring.h"
#include "Particle.h"

class Cloth
{
public:
    Cloth ();
    ~Cloth ();
    
    // create Cloths with specified characteristics
    Cloth (int width, int height, double mass, Vector3d dampFactors, Vector3d sprConstants);
    
    // copy constructor and assignment operator
    // _deep_ copy!
    Cloth (const Cloth& Cloth);
   // Cloth&	operator= (const Cloth& Cloth);
    
    // accessors
    int	getWidth ()   { return width;  }
    int	getHeight ()   { return height;  }
    Particle** getParticles()  {  return particles; }
    Particle& getParAt(int x, int y)  { return particles[x][y];  }
    Spring* getSheerSprings()  {  return sheer_springs;}
    Spring* getBendSprings()  {  return bend_springs;}
    Spring* getStructuralSprings()  {  return structural_springs;}
    //  setters
    // accessors
    void setWidth (int w)   { width = w;  }
    void setHeight (int h)   { height = h;  }
    void setParticles(Particle** p)  { particles = p; }
    void setParAt(int x, int y, Particle& p)  { particles[x][y] = p;  }
    void setSheerSprings(Spring* sheerS)  { sheer_springs = sheerS;}
    void setBendSprings(Spring* bendS)  {  bend_springs = bendS;}
    void setStructuralSprings(Spring* structuralS)  { structural_springs = structuralS;}
    void setMass(double mass) {paticle_mass = mass;}
    
    //    // OpenGL call wrappers ---- in case we need it
    //    void		glDrawPixelsWrapper ();
    //    void		glTexImage2DWrapper ();
    //    void		glReadPixelsWrapper ();
    
    // methods
    // drawCloth();
    
    
    
private:
    
    int width;
    int height;
    double paticle_mass;
    
    Particle**  particles;
    Spring*  bend_springs;
    Spring*  structural_springs;
    Spring*  sheer_springs;
    
//    Vector3d dampFactors;
//    Vector3d sprConstants;
};


#endif /* Cloth_h */
