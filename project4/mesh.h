#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <string>

#include "./vec.h"
#include "./bb.h"
#include "./material.h"

// This class represents a mesh, which contains vertices, polygons,
// and material properties for each polygon.
class Mesh {
 public:
  Mesh();

  void AddVertex(const Vec3f& v);
  void AddTextureVertex(const Vec3f& v);
  void AddPolygon(const std::vector<int>& p, const std::vector<int>& pt);

  void new_material(int material_idx, const std::string& name) {
    _materials.push_back(Material(name));
  }

  void set_cur_material(const string& name) {
    for (int i = 0; i < _materials.size(); ++i) {
      if (_materials[i].name() == name) {
        set_cur_material(i);
      }
    }
  }

  void set_cur_material(int cur_mtl) {
    _cur_mtl = cur_mtl;
  }

  void set_ambient(int material_idx, const Vec3f& ambient) {
    _materials[material_idx].set_ambient(ambient);
  }

  void set_diffuse(int material_idx, const Vec3f& diffuse) {
    _materials[material_idx].set_diffuse(diffuse);
  }

  void set_specular(int material_idx, const Vec3f& specular) {
    _materials[material_idx].set_specular(specular);
  }

  void set_specular_coeff(int material_idx, const float& coeff) {
    _materials[material_idx].set_specular_coeff(coeff);
  }

  void set_texture(int material_idx, const string& texture) {
    _materials[material_idx].set_texture(texture);
  }

  // TODO - you don't need to modify this function, but it will be useful
  // in your Display() function.
  //
  // Given a polygon index, returns the index of the material
  // that should be used for that polygon.
  int polygon2material(int polygon_idx) const {
    return _polygon2material[polygon_idx];
  }

  const Material& material(int i) const { return _materials[i]; }

  Material& material(int i) { return _materials[i]; }

  const BoundingBox& bb() const { return _bb; }

  int num_materials() const { return _materials.size(); }

  void compute_normals();

 private:
  // TODO add necessary data structures here

  std::vector<Material> _materials;
  std::vector<int> _polygon2material;
  int _cur_mtl;
  BoundingBox _bb;
};

#endif
