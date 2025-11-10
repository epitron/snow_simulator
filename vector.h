#ifndef vector_H
#define vector_H

// * vector.h
// *
// * A `vector' class
// *
// *   PUBLIC VARIABLES
// * 
// *     x, y, z          The coordinates
// *
// *   CONSTRUCTORS
// *
// *     vector( x, y, z ) Create a vector with coordinates (x,y)
// *
// *   PUBLIC FUNCTIONS
// *
// *     Operators == and != are provided for comparison of vertices.
// *
// *     I/O operators allow vertices to be read or written:
// *
// *       cin >> p;
// *       cout << p;
// *
// *     A vector is written as a triple of numbers, separated by spaces
// *     (e.g. "3.14 1.01 0.02")
// *



#include <iostream.h>
#include <math.h>

 

class vector {
public:

  float x, y, z;

  vector() {}

  vector( float xx, float yy, float zz )
    { x = xx; y = yy; z = zz; }

  bool operator == (const vector p) {
    return x == p.x && y == p.y && z == p.z;
  }

  bool operator != (const vector p) {
    return x != p.x || y != p.y || z != p.z; 
  }

  vector operator + (vector p)
    { return vector( x+p.x, y+p.y, z+p.z ); }

  vector operator - (vector p)
    { return vector( x-p.x, y-p.y, z-p.z ); }

  float operator * (vector p)	/* dot product */
    { return x * p.x + y * p.y + z * p.z; }

  vector operator ^ (vector p)	/* cross product */
    { return vector( y*p.z-p.y*z, -(x*p.z-p.x*z), x*p.y-p.x*y ); }

  vector normalize() {
    float len;
    len = (float)sqrt( x*x + y*y + z*z );
    return vector( x/len, y/len, z/len );
  }

  float length()
    { return (float)sqrt( x*x + y*y + z*z ); }

  float squaredLength()
    { return x*x + y*y + z*z; }

  vector perp1();
  vector perp2();
};


// Scalar/vector multiplication

vector operator * ( float k, vector const& p );

float distToEdge( vector point, vector head, vector tail );

// I/O operators

ostream& operator << ( ostream& stream, vector const& p );
istream& operator >> ( istream& stream, vector & p );


#endif
