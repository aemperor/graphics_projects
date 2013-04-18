
uniform vec4 LMa; // Light-Material ambient
uniform vec4 LMd; // Light-Material diffuse
uniform vec4 LMs; // Light-Material specular
uniform float shininess;

uniform sampler2D normalMap;
uniform sampler2D decal;
uniform sampler2D heightField;
uniform samplerCube envmap;

uniform mat3 objectToWorld;

varying vec2 normalMapTexCoord;
varying vec3 lightDirection;
varying vec3 eyeDirection;
varying vec3 halfAngle;
varying vec3 c0, c1, c2;

void main()
{
  // sample normalMap and expand range from [0..1] to [-1..+1]
  vec4 normals = (texture2D(normalMap, normalMapTexCoord) * 2.0) - 1.0;

  mat3 M = mat3(c0, c1, c2);

  vec3 lightSurface = normalize(lightDirection * M);
  float diffuseCoeff = max(dot(normals, lightSurface[2]), 0.0); // use z component

  gl_FragColor = vec4((diffuseCoeff * LMd) + LMa);
}
