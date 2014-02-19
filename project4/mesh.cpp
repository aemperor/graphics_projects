#include <iostream>
#include <vector>

#include "./mesh.h"

using namespace std;

Mesh::Mesh() {
  _cur_mtl = -1;
}

// This will be called by the obj parser
void Mesh::AddVertex(const Vec3f& v) {
  vertices.push_back(v);
  // updates the bounding box
  _bb(v);
}

// This will be called by the obj parser
void Mesh::AddTextureVertex(const Vec3f& v) {
  textures.push_back(v);
}

// p is the list of indices of vertices for this polygon.  For example,
// if p = {0, 1, 2} then the polygon is a triangle with the zeroth, first and
// second vertices added using AddVertex.
//
// pt is the list of texture indices for this polygon, similar to the
// actual vertices described above.
void Mesh::AddPolygon(const std::vector<int>& p, const std::vector<int>& pt) {
  polyVerts.push_back(p);
  polyTexts.push_back(pt);
  // updates the poly2mat map
  _polygon2material.push_back(_cur_mtl);
}

// Computes a normal for each vertex.
// normal of vertex = sum of normals for each incident polygon
void Mesh::compute_normals() {
  normals = std::vector<Vec3f>(vertices.size(), Vec3f());
  for (int i = 0; i < polyVerts.size(); ++i) {
      Vec3f v1 = Vec3f::makeVec(
                 vertices[polyVerts[i][0]][0] - vertices[polyVerts[i][1]][0],
                 vertices[polyVerts[i][0]][1] - vertices[polyVerts[i][1]][1],
                 vertices[polyVerts[i][0]][2] - vertices[polyVerts[i][1]][2]);
      Vec3f v2 = Vec3f::makeVec(
                 vertices[polyVerts[i][2]][0] - vertices[polyVerts[i][1]][0],
                 vertices[polyVerts[i][2]][1] - vertices[polyVerts[i][1]][1],
                 vertices[polyVerts[i][2]][2] - vertices[polyVerts[i][1]][2]);
      Vec3f polyNorm = v1 ^ v2;
      polyNorm.unit();
      for (int j = 0; j < polyVerts[i].size(); ++j) {
          normals[polyVerts[i][j]] += polyNorm;
      }
  }
}
