
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
  vec4 texture = texture2D(decal, normalMapTexCoord);

  mat3 M = mat3(c0, c1, c2);

  halfAngle = (lightDirection + eyeDirection)/2.0;
  vec3 halfAngleSurface = normalize(halfAngle * M);

  vec3 lightSurface = normalize(lightDirection * M);
  float diffuseCoeff = max(normals * lightSurface[2], 0.0); // use z component

  vec3 specularContribution = normals * halfAngleSurface[2];
  float specularCoeff = max(pow(specularContribution, shininess), 0.0);
  if (diffuseCoeff == 0.0)
    specularCoeff = 0.0;

  //gl_FragColor = vec4(LMa + LMd*diffuseCoeff) * texture2D(decal, normalMapTexCoord) + specularCoeff*LMs;
  gl_FragColor = vec4(LMa + LMd*diffuseCoeff*texture + specularCoeff);
}
