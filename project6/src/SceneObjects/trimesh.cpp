#include <cmath>
#include <float.h>
#include "trimesh.h"

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
  faces.push_back( newFace );
  return true;
}

char *
Trimesh::doubleCheck()
    // Check to make sure that if we have per-vertex materials or normals
    // they are the right number.
{
  if( !materials.empty() && materials.size() != vertices.size() )
    return "Bad Trimesh: Wrong number of materials.";
  if( !normals.empty() && normals.size() != vertices.size() )
    return "Bad Trimesh: Wrong number of normals.";

  return 0;
}

bool Trimesh::intersectLocal(const ray&r, isect&i) const
{
  double tmin = 0.0;
  double tmax = 0.0;
  typedef Faces::const_iterator iter;
  bool have_one = false;
  for( iter j = faces.begin(); j != faces.end(); ++j ) {
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

// Intersect ray r with the triangle abc.  If it hits returns true,
// and puts the t parameter, barycentric coordinates, normal, object id,
// and object material in the isect object
bool TrimeshFace::intersectLocal( const ray& r, isect& i ) const
{
  const Vec3d& a = parent->vertices[ids[0]];
  const Vec3d& b = parent->vertices[ids[1]];
  const Vec3d& c = parent->vertices[ids[2]];

  // YOUR CODE HERE
  Vec3d v1 = b - a;
  Vec3d v2 = c - b;

  Vec3d normal = v1 ^ v2;
  
  double d = -a * normal;

  double t = - (r.getPosition() * normal + d) / (normal * r.getDirection());

  Vec3d p = r.at(t);

  Vec3d aToP = p - a;
  Vec3d bToP = p - b;
  Vec3d cToP = p - c;
  Vec3d aToB = b - a;
  Vec3d bToC = c - b;
  Vec3d cToA = c - a;

  bool inTriangle;
  
  if ( ((aToP ^ aToB)[2] >= 0) && ((bToP ^ bToC)[2] >= 0) && ((cToP ^ cToA)[2] >= 0))
    inTriangle = true;
  else if ( ((aToP ^ aToB)[2] < 0) && ((bToP ^ bToC)[2] < 0) && ((cToP ^ cToA)[2] < 0))
  {
    inTriangle = true;
  }
  else
    inTriangle = false;

  i.setT(t);
  i.setN(normal);
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

