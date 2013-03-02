#include <string>
#include "./material.h"
#include "./texture.h"

Material::Material()
    : _ambient(Vec3f::makeVec(.1, .1, .1)),
      _diffuse(Vec3f::makeVec(.3, .3, .3)),
      _specular(Vec3f::makeVec(.1, .1, .1)), _specular_coeff(10),
      _texture_id(-1) {
}

Material::Material(const string& name)
    : _name(name),
      _ambient(Vec3f::makeVec(.1, .1, .1)),
      _diffuse(Vec3f::makeVec(.3, .3, .3)),
      _specular(Vec3f::makeVec(.1, .1, .1)), _specular_coeff(10),
      _texture_id(-1) {
}

void Material::set_ambient(const Vec3f& ambient) {
  _ambient = ambient;
}
void Material::set_diffuse(const Vec3f& diffuse) {
  _diffuse = diffuse;
}
void Material::set_specular(const Vec3f& specular) {
  _specular = specular;
}
void Material::set_specular_coeff(const float& coeff) {
  _specular_coeff = coeff;
}
void Material::set_texture(const string& texture) {
  _texture = texture;
}

void Material::LoadTexture(int texture_id) {
  ::LoadTexture(texture(), texture_id);
  _texture_id = texture_id;
}
