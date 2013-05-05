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
  bool inTriangle = false;
  Vec3d norm;
  if (parent->normals.empty()) {
    norm = (b-a) ^ (c-a);
    norm.normalize();
  } else {
    //cout << "Parent has Normals!" << endl;
    norm = ((parent->normals[ids[0]]) + (parent->normals[ids[1]]) + (parent->normals[ids[2]])) / 3.0;
    norm.normalize();
  }
  //norm = (b-a) ^ (c-a);
  //norm.normalize();
  double d = -(a * norm);

  double t = -((r.getPosition() * norm) + d) / (norm * r.getDirection());

  if (t >= 0.0) {
    //cout << "t = " << t << " norm = " << norm << endl;
    Vec3d p = r.at(t);

    // Find barycentric coodinates of point on plane of triangle relative
    // to the points on the triangle. If two of the weights added together < 1,
    // then we are inside! In addition, isect wants barycentric coodinates
    // anyways, so we're done!
    Vec3d v0 = b-a;
    Vec3d v1 = c-a;
    Vec3d v2 = p-a;

    double d00 = v0*v0;
    double d01 = v0*v1;
    double d11 = v1*v1;
    double d20 = v2*v0;
    double d21 = v2*v1;

    double denominator = (d00 * d11) - (d01 * d01);
    double v = (d11*d20 - d01*d21) / denominator;
    double w = (d00*d21 - d01*d20) / denominator;
    double u = 1.0-v-w;

    
    inTriangle = (v>=0.0 &&  w >=0.0 && (v+w <= 1.0));
    if (inTriangle) {
      //cout << "\t(" << u << ", " << v << ", " << w << ")" << endl;
      i.setT(t);
      i.setBary(u, v, w);
      i.setN(normal);
      i.setObject(this);
      if (parent->materials.empty()) {
        i.setMaterial(getMaterial());
      } else {
        i.setMaterial(*parent->materials[ids[0]]);
      }
    }
  }

  return inTriangle;
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

