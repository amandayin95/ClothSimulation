//
//  geometry.hpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/10/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>


using namespace std;

template <typename T, int N> class Tuple;
template <typename T, int N> class Point;
template <typename T, int N> class Vector;

typedef Point    <int, 2>    Point2i;
typedef Point    <double, 2> Point2d;
typedef Point    <float, 2>  Point2f;
typedef Point    <char, 2>   Point2c;

typedef Point    <int, 3>    Point3i;
typedef Point    <double, 3> Point3d;
typedef Point    <float, 3>  Point3f;
typedef Point    <char, 3>   Point3c;

typedef Point    <int, 4>    Point4i;
typedef Point    <double, 4> Point4d;
typedef Point    <float, 4>  Point4f;
typedef Point    <char, 4>   Point4c;

typedef Vector   <int, 2>    Vector2i;
typedef Vector   <double, 2> Vector2d;
typedef Vector   <float, 2>  Vector2f;
typedef Vector   <char, 2>   Vector2c;

typedef Vector   <int, 3>    Vector3i;
typedef Vector   <double, 3> Vector3d;
typedef Vector   <float, 3>  Vector3f;
typedef Vector   <char, 3>   Vector3c;

typedef Vector   <int, 4>    Vector4i;
typedef Vector   <double, 4> Vector4d;
typedef Vector   <float, 4>  Vector4f;
typedef Vector   <char, 4>   Vector4c;

template <typename T, int N>
class Tuple
{
public:
    Tuple			();
    Tuple			(const Tuple& toCopy);
    virtual ~Tuple	();
    Tuple& operator =	(const Tuple& toCopy);
    
    T& operator []	(int index);
    const T& operator []	(int index) const;
    T& get		(int index) const;
    
    virtual void glLoad	() = 0;
    int size		()   { return N; }
    
protected:
    T data[N];
};


template <typename T, int N>
class Point : public Tuple<T, N>
{
public:
    Point			(T t1 = 0, T t2 = 0, T t3 = 0, T t4 = 0);
    virtual ~Point	();
    Point			(const Point& toCopy);
    Point& operator =	(const Point& toCopy);
    
    bool   operator ==	(const Point& toCompare);
    bool   operator !=	(const Point& toCompare);
    
    Point  operator +	(const Vector<T, N>& toAdd);
    Point& operator +=	(const Vector<T, N>& toAdd);
    
    
#ifndef WIN32
    template<typename U, int M>
    friend Point<U, M> operator + (const Vector<U, M>& v, const Point<U, M>& p);
#endif
    
    Point  operator -	(const Vector<T, N>& toSub);
    Point& operator -=	(const Vector<T, N>& toSub);
    
#ifndef WIN32
    template<typename U, int M>
    friend Point<U, M> operator - (const Vector<U, M>& v, const Point<U, M>& p);
#endif
    
    void glLoad ();
    
    
};

template <typename T, int N>
class Vector : public Tuple<T, N>
{
public:
    Vector		(T t1 = 0, T t2 = 0, T t3 = 0, T t4 = 0);
    
    virtual ~Vector	();
    Vector		(const Point<T, N>& dest);
    Vector		(const Point<T, N>& start, const Point<T, N>& end);
    Vector		(const Vector& toCopy);
    
    Vector& operator =	(const Point<T, N>& dest);
    Vector& operator =	(const Vector& toCopy);
    
    bool    operator ==	(const Vector& toCompare) const;
    bool    operator !=	(const Vector& toCompare) const;
    
    Vector  operator +	(const Vector& toAdd) const;
    Vector& operator +=	(const Vector& toAdd);
    
    Vector  operator -    () const;
    Vector  operator -	(const Vector& toSub) const;
    Vector& operator -=	(const Vector& toSub);
    
    Vector  operator *	(const Vector& toMult) const;
    Vector& operator *=	(const Vector& toMult);
    
    Vector  operator /	(const Vector& toDiv) const;
    Vector& operator /=	(const Vector& toDiv);
    
    Vector  operator *	(double scaleFactor) const;
    Vector& operator *=	(double scaleFactor);
    
#ifndef WIN32
    template<typename U, int M>
    friend Vector<U, M> operator * (double scaleFactor, const Vector<U, M>& v);
#endif
    
    
    Vector  operator /	(double scaleFactor) const;
    Vector& operator /=	(double scaleFactor);
    
    T dot			(const Vector& toDot);
    Vector cross          (const Vector& toCross);
    
    Vector& normalize 	();
    Vector  getUnit 	();
    
    double length		();
    void   glLoad 	();
};


#include "geometry.cc"


#endif // GEOMETRY_H