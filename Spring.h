//
//  String.hpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/10/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//
class Spring;

#ifndef Spring_H
#define Spring_H

#include "Particle.h"

class Spring
{
public:
    Spring ();
    ~Spring ();
    
    // create Springs with specified characteristics
    Spring (int p1i, int p1j, int p2i, int p2j, double sprConst, double dampFactor);
    
    // copy constructor and assignment operator
    // _deep_ copy!
    Spring (const Spring& spring);
  //  Spring&		operator= (const Spring& spring);
    
    // accessors
    int	getP1i() { return p1i; }
    int	getP1j() { return p1j; }
    int	getP2i() { return p2i; }
    int	getP2j() { return p2j; }
    double	getSpringConst ()  { return springConstant; }
    double	getDampFactor ()   { return dampFactor; }
    
    // setters
    void	setP1i (int p1_i)           { p1i = p1_i;   }
    void	setP1j (int p1_j)           { p1j = p1_j;   }
    void	setP2i (int p2_i)           { p2i = p2_i;   }
    void	setP2j (int p2_j)           { p2j = p2_j;   }

    
    
    //    // OpenGL call wrappers ---- in case we need it
    //    void		glDrawPixelsWrapper ();
    //    void		glTexImage2DWrapper ();
    //    void		glReadPixelsWrapper ();
    
    
    
private:
    
    int  p1i;
    int  p1j;
    int  p2i;
    int  p2j;

    double  springConstant;
    double  dampFactor;
    
};


#endif /* Spring_H */
