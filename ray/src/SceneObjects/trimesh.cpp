#include <cmath>
#include <float.h>
#include <algorithm>
#include <assert.h>
#include "trimesh.h"
#include "../ui/TraceUI.h"
extern TraceUI* traceUI;

using namespace std;

Trimesh::~Trimesh()
{
	for( Materials::iterator i = materials.begin(); i != materials.end(); ++i )
		delete *i;
}

// must add vertices, normals, and materials IN ORDER
void Trimesh::addVertex( const Vec3d &v )
{
    vertices.push_back( v );
}

void Trimesh::addMaterial( Material *m )
{
    materials.push_back( m );
}

void Trimesh::addNormal( const Vec3d &n )
{
    normals.push_back( n );
}

// Returns false if the vertices a,b,c don't all exist
bool Trimesh::addFace( int a, int b, int c )
{
    int vcnt = vertices.size();

    if( a >= vcnt || b >= vcnt || c >= vcnt ) return false;

    TrimeshFace *newFace = new TrimeshFace( scene, new Material(*this->material), this, a, b, c );
    newFace->setTransform(this->transform);
    if (!newFace->degen) faces.push_back( newFace );


    // Don't add faces to the scene's object list so we can cull by bounding box
    // scene->add(newFace);
    return true;
}

char* Trimesh::doubleCheck()
// Check to make sure that if we have per-vertex materials or normals
// they are the right number.
{
    if( !materials.empty() && materials.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of materials.";
    if( !normals.empty() && normals.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of normals.";

    return 0;
}

bool Trimesh::intersectLocal(ray& r, isect& i) const
{
	double tmin = 0.0;
	double tmax = 0.0;
	typedef Faces::const_iterator iter;
	bool have_one = false;
	for( iter j = faces.begin(); j != faces.end(); ++j )
	  {
	    isect cur;
	    if( (*j)->intersectLocal( r, cur ) )
	      {
		if( !have_one || (cur.t < i.t) )
		  {
		    i = cur;
		    have_one = true;
		  }
	      }
	  }
	if( !have_one ) i.setT(1000.0);
	return have_one;
}

bool TrimeshFace::intersect(ray& r, isect& i) const {
  return intersectLocal(r, i);
}

// Intersect ray r with the triangle abc.  If it hits returns true,
// and put the parameter in t and the barycentric coordinates of the
// intersection in u (alpha) and v (beta).
bool TrimeshFace::intersectLocal(ray& r, isect& i) const
{

    const Vec3d& a = parent->vertices[ids[0]];
    const Vec3d& b = parent->vertices[ids[1]];
    const Vec3d& c = parent->vertices[ids[2]];

    // YOUR CODE HERE
    Vec3d N = (b - a) ^ (c - a);
    if (N == Vec3d(0,0,0)) return false;
    N.normalize();
    
    
    Vec3d p = r.getPosition();
    Vec3d d = r.getDirection();
    double t = N * (a - p) / (N * d);
    if (t <= RAY_EPSILON) return false;
    
    Vec3d q = r.at(t);
    
    if ((((b - a) ^ (q - a)) * N) < 0 || (((c - b) ^ (q - b)) * N) < 0  || (((a - c) ^ (q - c)) * N) < 0 ) return false;
    
    
    i.obj = this;
    i.t = t;
 
    // u and v
    
    double areaABC2 = ((b - a) ^ (c - a)).length();
    double alpha = ((c - b) ^ (q - b)).length() / areaABC2;
    double beta = ((a - c) ^ (q - c)).length() / areaABC2;
    double gamma = ((b - a) ^ (q - a)).length() / areaABC2;
    
    
    // material properties
    if (parent->materials.size()) {
        
        Material* m = new Material();
        
        *m = alpha * (*parent->materials[ids[0]]);
        *m += beta * (*parent->materials[ids[1]]);
        *m += gamma * (*parent->materials[ids[2]]);
        
        i.setMaterial(*m);
        
    } else {
        
         i.setMaterial(this->getMaterial());
    }
    
    
    // normals
    if (parent->normals.size()) {
        
       Vec3d n = alpha * parent->normals[ids[0]];
        n += beta * parent->normals[ids[1]];
        n += gamma * parent->normals[ids[2]];
        
        i.setN(n);
        
    } else {
        
           i.setN(N);
    }
    
    return true;
}

void Trimesh::generateNormals()
// Once you've loaded all the verts and faces, we can generate per
// vertex normals by averaging the normals of the neighboring faces.
{
    int cnt = vertices.size();
    normals.resize( cnt );
    int *numFaces = new int[ cnt ]; // the number of faces assoc. with each vertex
    memset( numFaces, 0, sizeof(int)*cnt );
    
    for( Faces::iterator fi = faces.begin(); fi != faces.end(); ++fi )
    {
		Vec3d faceNormal = (**fi).getNormal();
        
        for( int i = 0; i < 3; ++i )
        {
            normals[(**fi)[i]] += faceNormal;
            ++numFaces[(**fi)[i]];
        }
    }

    for( int i = 0; i < cnt; ++i )
    {
        if( numFaces[i] )
            normals[i]  /= numFaces[i];
    }

    delete [] numFaces;
    vertNorms = true;
}
