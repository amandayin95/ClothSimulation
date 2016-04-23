//
//  geometry.cpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/10/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//
#ifndef GEOMETRY_ICC
#define GEOMETRY_ICC


#include "geometry.h"
#include <math.h>
#include <glut/glut.h>
#include <GLUT/glut.h>


template <typename T, int N>
Tuple<T, N>::Tuple ()
{
    for (int i = 0; i < N; ++i)
        data[i] = 0;
}

template <typename T, int N>
Tuple<T, N>::Tuple (const Tuple<T, N>& toCopy)
{
    for (int i = 0; i < N; ++i)
        data[i] = toCopy[i];
}

template <typename T, int N>
Tuple<T, N>::~Tuple ()
{
    
}

template <typename T, int N>
Tuple<T, N>& Tuple<T, N>::operator = (const Tuple<T, N>& toCopy)
{
    if (this == &toCopy)
        return *this;
    
    for (int i = 0; i < N; ++i)
        data[i] = toCopy[i];
    
    return *this;
}


template <typename T, int N>
T& Tuple<T, N>::operator [] (int index)
{
    return data[index];
}

template <typename T, int N>
const T& Tuple<T, N>::operator [] (int index) const
{
    return data[index];
}

template <typename T, int N>
T& Tuple<T, N>::get(int index) const
{
    return data[index];
}

template <typename T, int N>
Point<T, N>::Point (T t1, T t2, T t3, T t4)
: Tuple<T, N>()
{
    if (N >= 4)
        this->data[3] = t4;
    if (N >= 3)
        this->data[2] = t3;
    if (N >= 2)
        this->data[1] = t2;
    if (N >= 1)
        this->data[0] = t1;
}

template <typename T, int N>
Point<T, N>::~Point ()
{
}

template <typename T, int N>
Point<T, N>::Point (const Point<T, N>& toCopy)
: Tuple<T, N>(toCopy)
{
}

template <typename T, int N>
Point<T, N>& Point<T, N>::operator = (const Point<T, N>& toCopy)
{
    if (this == &toCopy)
        return *this;
    
    Tuple<T, N>::operator= (toCopy);
    
    return *this;
}

template <typename T, int N>
bool Point<T, N>::operator == (const Point<T, N>& compare)
{
    for (int i = 0; i < N; ++i)
        if (this->data[i] != compare[i])
            return false;
    
    return true;
}

template <typename T, int N>
bool Point<T, N>::operator != (const Point<T, N>& compare)
{
    for (int i = 0; i < N; ++i)
        if (this->data[i] != compare[i])
            return true;
    
    return false;
}

template <typename T, int N>
Point<T, N> Point<T, N>::operator + (const Vector<T, N>& toAdd)
{
    Point sum = *this;
    sum += toAdd;
    return sum;
}

template <typename T, int N>
Point<T, N>& Point<T, N>::operator += (const Vector<T, N>& toAdd)
{
    for (int i = 0; i < N; ++i)
        this->data[i] += toAdd[i];
    return *this;
}

#ifndef WIN32
template <typename T, int N>
Point<T, N> operator + (const Vector<T, N>& v, const Point<T, N>& p)
{
    return p + v;
}
#endif

template <typename T, int N>
Point<T, N> Point<T, N>::operator - (const Vector<T, N>& toSub)
{
    Point diff = *this;
    diff -= toSub;
    return diff;
}

template <typename T, int N>
Point<T, N>& Point<T, N>::operator -= (const Vector<T, N>& toSub)
{
    for (int i = 0; i < N; ++i)
        this->data[i] -= toSub[i];
    return *this;
}

#ifndef WIN32
template <typename T, int N>
Point<T, N> operator - (const Vector<T, N>& v, const Point<T, N>& p)
{
    Point<T, N> diff;
    for (int i = 0; i < N; ++i)
        diff[i] = v[i] - p[i];
    return diff;
}
#endif

template <typename T, int N>
void Point<T, N>::glLoad()
{
    switch(N)
    {
        case 2:
            glVertex2f(this->data[0], this->data[1]);
            break;
        case 3:
            glVertex3f(this->data[0], this->data[1], this->data[2]);
            break;
        case 4:
            glVertex4f(this->data[0], this->data[1], this->data[2], this->data[3]);
            break;
        default:
            cerr << "glVertex* doesn't support " << N << " arguments\n" << endl;
    }
}

template <typename T, int N>
Vector<T, N>::Vector (T t1, T t2, T t3, T t4)
: Tuple<T, N>()
{
    if (N >= 4)
        this->data[3] = t4;
    if (N >= 3)
        this->data[2] = t3;
    if (N >= 2)
        this->data[1] = t2;
    if (N >= 1)
        this->data[0] = t1;
}

template <typename T, int N>
Vector<T, N>::~Vector ()
{
}

template <typename T, int N>
Vector<T, N>::Vector (const Vector<T, N>& toCopy)
: Tuple<T, N>(toCopy)
{
}

template <typename T, int N>
Vector<T, N>::Vector(const Point<T, N>& dest)
: Tuple<T, N>(dest)
{
}

template <typename T, int N>
Vector<T, N>::Vector(const Point<T, N>& start, const Point<T, N>& end)
: Tuple<T, N>()
{
    for (int i = 0; i < N; ++i)
        this->data[i] = end[i] - start[i];
}

template <typename T, int N>
Vector<T, N>& Vector<T, N>::operator = (const Point<T, N>& dest)
{
    Tuple<T, N>::operator= (dest);
    
    return *this;
}

template <typename T, int N>
Vector<T, N>& Vector<T, N>::operator = (const Vector<T, N>& toCopy)
{
    if (this == &toCopy)
        return *this;
    
    Tuple<T, N>::operator= (toCopy);
    
    return *this;
}

template <typename T, int N>
Vector<T, N> Vector<T, N>::operator + (const Vector<T, N>& toAdd) const
{
    Vector<T, N> sum = *this;
    sum += toAdd;
    return sum;
}

template <typename T, int N>
Vector<T, N>& Vector<T, N>::operator += (const Vector<T, N>& toAdd)
{
    for (int i = 0; i < N; ++i)
        this->data[i] += toAdd[i];
    return *this;
}

template <typename T, int N>
Vector<T, N> Vector<T, N>::operator - () const
{
    Vector<T, N> neg = *this;
    for (int i = 0; i < N; ++i)
        neg[i] *= -1;
    return neg;
}

template <typename T, int N>
Vector<T, N> Vector<T, N>::operator - (const Vector<T, N>& toSub) const
{
    Vector<T, N> diff = *this;
    diff -= toSub;
    return diff;
}

template <typename T, int N>
Vector<T, N>& Vector<T, N>::operator -= (const Vector<T, N>& toSub)
{
    for (int i = 0; i < N; ++i)
        this->data[i] -= toSub[i];
    return *this;
}

template <typename T, int N>
Vector<T, N> Vector<T, N>::operator * (const Vector<T, N>& toMult) const
{
    Vector<T, N> product = *this;
    product *= toMult;
    return product;
}

template <typename T, int N>
Vector<T, N>& Vector<T, N>::operator *= (const Vector<T, N>& toMult)
{
    for (int i = 0; i < N; ++i)
        this->data[i] *= toMult[i];
    return *this;
}

template <typename T, int N>
Vector<T, N> Vector<T, N>::operator / (const Vector<T, N>& toDiv) const
{
    Vector<T, N> quotient = *this;
    quotient /= toDiv;
    return quotient;
}

template <typename T, int N>
Vector<T, N>& Vector<T, N>::operator /= (const Vector<T, N>& toDiv)
{
    for (int i = 0; i < N; ++i)
        this->data[i] /= toDiv[i];
    return *this;
}

template <typename T, int N>
Vector<T, N> Vector<T, N>::operator * (double scaleFactor) const
{
    Vector<T, N> product = *this;
    product *= scaleFactor;
    return product;
}

template <typename T, int N>
Vector<T, N>& Vector<T, N>::operator *= (double scaleFactor)
{
    for (int i = 0; i < N; ++i)
        this->data[i] *= scaleFactor;
    return *this;
}

#ifndef WIN32
template <typename T, int N>
Vector<T, N> operator * (double scaleFactor, const Vector<T, N>& v)
{
    return v * scaleFactor;
}
#endif

template <typename T, int N>
Vector<T, N> Vector<T, N>::operator / (double scaleFactor) const
{
    Vector<T, N> quotient = *this;
    quotient /= scaleFactor;
    return quotient;
}

template <typename T, int N>
Vector<T, N>& Vector<T, N>::operator /= (double scaleFactor)
{
    if (scaleFactor == 0)
    {
        cerr << "cannot divide by zero " << endl;
        return *this;
    }
    for (int i = 0; i < N; ++i)
        this->data[i] /= scaleFactor;
    return *this;
}

template <typename T, int N>
T Vector<T, N>::dot (const Vector<T, N>& toDot)
{
    T dotProduct = 0;
    for (int i = 0; i < N; ++i)
        dotProduct += this->data[i] * toDot[i];
    return dotProduct;
}

/*
 * Take the cross product of two 3-dimensional vectors. Note that if called
 * on anything but 3-dimensional vectors a compiler error will result.
 */
template <typename T, int N>
Vector<T, N> Vector<T, N>::cross (const Vector<T, N>& toCross)
{
    Vector<T, 3> crossProduct;
    crossProduct[0] = this->data[1] * toCross[2] - this->data[2] * toCross[1];
    crossProduct[1] = this->data[2] * toCross[0] - this->data[0] * toCross[2];
    crossProduct[2] = this->data[0] * toCross[1] - this->data[1] * toCross[0];
    return crossProduct;
}

template <typename T, int N>
Vector<T, N>& Vector<T, N>::normalize ()
{
    double len = this->length();
    
    if (len == 0)
    {
        cerr << "Cannot normalize 0-length vector " << endl;
        return *this;
    }
    
    for (int i = 0; i < N; ++i)
        this->data[i] /= len;
    
    return *this;
}

template <typename T, int N>
Vector<T, N> Vector<T, N>::getUnit ()
{
    Vector<T, N> unit;
    unit[0]=this->data[0];
    unit[1]=this->data[1];
    unit[2]=this->data[2];
    if (unit.length() == 0)
    {
        //cerr << "Warning:  cannot normalize zero length vector.";
        return unit;
    }
    return unit.normalize();
}

template <typename T, int N>
double Vector<T, N>::length()
{
    double sumSquares = 0;
    for (int i = 0; i < N; ++i)
        sumSquares += this->data[i] * this->data[i];
    
    return sqrt(sumSquares);
}

template <typename T, int N>
void Vector<T, N>::glLoad ()
{
//    if (N == 3)
//     //   glNormal3f(this->data[0], this->data[1], this->data[2]);
//    else
//        cerr << "glNormal* doesn't support " << N << " arguments\n" << endl;
}


#endif // GEOMETRY_ICC