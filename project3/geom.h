#ifndef _GEOM_H_
#define _GEOM_H_

#include<cmath>
#include<algorithm>
#include<iostream>
#include<vector>
#include<map>
#include <limits>
#include "types.h"
#include "vec.h"

using namespace std;

struct BoundingBox
{
	Vec3f min;
	Vec3f max;
	friend ostream & operator<<(ostream & out, const BoundingBox & b);
};

ostream & operator<<(ostream & out, const BoundingBox & b)
{
	out << "[box min="<<b.min<<" max="<<b.max<<"]";
	return out;
}
	
#endif
