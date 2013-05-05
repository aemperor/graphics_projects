#include <cmath>

#include "light.h"



using namespace std;

double DirectionalLight::distanceAttenuation( const Vec3d& P ) const
{
  // distance to light is infinite, so f(di) goes to 0.  Return 1.
  return 1.0;
}


Vec3d DirectionalLight::shadowAttenuation( const Vec3d& P ) const
{
  isect i;
  Vec3d ret;
  Vec3d vec = -orientation;
  vec.normalize();
  ray shadowRay(P + (0.01)*vec, vec, ray::SHADOW);
  if (scene->intersect(shadowRay, i)) {
    Vec3d tr = i.getMaterial().kt(i);
    ret = tr % Vec3d(1, 1, 1);
  } else {
    ret = Vec3d(1, 1, 1);
  }

  return ret;

}

Vec3d DirectionalLight::getColor( const Vec3d& P ) const
{
  // Color doesn't depend on P 
  return color;
}

Vec3d DirectionalLight::getDirection( const Vec3d& P ) const
{
  return -orientation;
}

double PointLight::distanceAttenuation( const Vec3d& P ) const
{
  double length = (position - P).length();
  double a0 = constantTerm;
  double a1 = linearTerm;
  double a2 = quadraticTerm;

  double attenuation = min(1.0, 1/ (a0 + a1 * length + a2 * pow(length, 2)));

  return attenuation;
}

Vec3d PointLight::getColor( const Vec3d& P ) const
{
  // Color doesn't depend on P 
  return color;
}

Vec3d PointLight::getDirection( const Vec3d& P ) const
{
  Vec3d ret = position - P;
  ret.normalize();
  return ret;
}


Vec3d PointLight::shadowAttenuation(const Vec3d& P) const
{
  isect i;
  Vec3d ret;
  Vec3d dir = position-P;
  double lightLength = dir.length();
  dir.normalize();
  ray shadowRay(P + (0.01)*dir, dir, ray::SHADOW);
  if (scene->intersect(shadowRay, i)) {
    Vec3d collisionPt = shadowRay.at(i.t);
    Vec3d collisionToObj = collisionPt - P;
    double collisionLength = collisionToObj.length();
    
    if (collisionLength < lightLength) {
      Vec3d tr = i.getMaterial().kt(i);
      ret = tr % Vec3d(1, 1, 1);
    } else {
      ret = Vec3d(1, 1, 1);  
    }
  } else {
    ret = Vec3d(1, 1, 1);
  }

  return ret;
}
