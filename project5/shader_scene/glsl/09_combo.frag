
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
  mat3 M = mat3(c0, c1, c2);

  vec3 normals = (texture2D(normalMap, normalMapTexCoord) * 2.0) - 1.0;
  eyeDirection = eyeDirection * M;

  vec3 reflection = reflect(-1 * eyeDirection, normals);
  reflection = normalize(M * reflection);
  reflection = normalize(objectToWorld * reflection);

  vec3 lightSurface = normalize(lightDirection * M);
  float diffuseCoeff = max(dot(normals[2], lightSurface[2]), 0.0); // use z component

  halfAngle = (lightDirection + eyeDirection)/2.0;
  vec3 halfAngleSurface = halfAngle * M;
  vec3 specularContribution = normals * halfAngleSurface[2];
  float specularCoeff = max(pow(specularContribution[2], shininess), 0.0);
  if (diffuseCoeff == 0.0)
    specularCoeff = 0.0;

  gl_FragColor = textureCube(envmap, reflection) * 0.6 +
                             (0.5 * (LMd*diffuseCoeff + LMa)) +
                             (0.5 * LMs*specularCoeff);
}
