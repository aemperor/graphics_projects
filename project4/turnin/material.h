#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <string>
#include <vector>
#include "./vec.h"

class Material {
 public:
  Material();
  explicit Material(const std::string& name);

  void set_ambient(const Vec3f& ambient);
  void set_diffuse(const Vec3f& diffuse);
  void set_specular(const Vec3f& specular);
  void set_specular_coeff(const float& coeff);
  void set_texture(const std::string& texture);
  void LoadTexture(int texture_id);

  const std::string& name() const { return _name; }
  const Vec3f& ambient() const { return _ambient; }
  const Vec3f& diffuse() const { return _diffuse; }
  const Vec3f& specular() const { return _specular; }
  const float& specular_coeff() const { return _specular_coeff; }
  const std::string& texture() const { return _texture; }
  int texture_id() const { return _texture_id; }

 private:
  std::string _name;
  Vec3f _ambient;
  Vec3f _diffuse;
  Vec3f _specular;
  float _specular_coeff;
  std::string _texture;
  int _texture_id;
};

#endif
