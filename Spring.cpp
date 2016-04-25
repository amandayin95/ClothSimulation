//
//  String.cpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/10/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//

#include "Spring.h"

Spring::Spring(){
    
}


Spring::~Spring(){
    
}


Spring::Spring (int p1i, int p1j, int p2i, int p2j, double sprConst, double dampFactor){
    this->p1i = p1i;
    this->p1j = p1j;
    this->p2i = p2i;
    this->p2j = p2j;
    this->springConstant = sprConst;
    this->dampFactor = dampFactor;
}