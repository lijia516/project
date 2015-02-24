#include <cmath>

#include "Square.h"

using namespace std;


//Test
bool Square::intersectLocal(ray& r, isect& i) const
{
    
  //  std::cout<< "sqaure intersectLocal\n";
    
	Vec3d p = r.getPosition();
	Vec3d d = r.getDirection();
    
    
  //  std::cout<< "p: " << p[0]<< ", " << p[1]<< ", "<< p[2]<< ", "<<"\n";
  //  std::cout<< "d: " << d[0]<< ", " << d[1]<< ", "<< d[2]<< ", "<<"\n";
    

	if( d[2] == 0.0 ) {
        
     //   std::cout<< "d[2] == 0.0 false intersectLocal\n";
    //
		return false;
	}

	double t = -p[2]/d[2];

    std::cout<<"t: "<< t <<"\n";
    
	if( t <= RAY_EPSILON ) {
        
     //   std::cout<< "t <= RAY_EPSILON false intersectLocal\n";
        
		return false;
	}

	Vec3d P = r.at( t );
    
  //  std::cout<< "r.at( t ) " << r.at( t )[0]<< ", " << r.at( t )[1]<< ", "<< r.at( t )[2]<< ", "<<"\n";

	if( P[0] < -0.5 || P[0] > 0.5 ) {	
		return false;
	}

	if( P[1] < -0.5 || P[1] > 0.5 ) {	
		return false;
	}

	i.obj = this;
	i.setMaterial(this->getMaterial());
	i.t = t;
	if( d[2] > 0.0 ) {
		i.N = Vec3d( 0.0, 0.0, -1.0 );
	} else {
		i.N = Vec3d( 0.0, 0.0, 1.0 );
	}

    i.setUVCoordinates( Vec2d(P[0] + 0.5, P[1] + 0.5) );
    
  //  std::cout<< "true intersectLocal\n";
	return true;
}
