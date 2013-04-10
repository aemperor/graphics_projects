
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
  float theta = normalMapTexCoord[0];
  float phi = normalMapTexCoord[1];

  vec3 gradientU = vec3(cos(theta), sin(phi), 0.0);  
  vec3 tangent = normalize(gradientU);
  vec3 gradientV = vec3(cos(theta)*cos(phi), sin(theta)*cos(phi), sin(phi));
  vec3 normal2surface = cross(normalize(gradientU), 
                              normalize(gradientV));
  vec3 binormal = cross(normal2surface, tangent);
  mat3 M = mat3(tangent, binormal, normal2surface);

  vec3 lightSurface = normalize(lightDirection * inverse(M));
  float diffuseCoeff = max(lightSurface[2], 0.0); // z component

  gl_FragColor = vec4((diffuseCoeff * LMd) + LMa);
}

