
attribute vec2 parametric;

uniform vec3 lightPosition;  // Object-space
uniform vec3 eyePosition;    // Object-space
uniform vec2 torusInfo;

varying vec2 normalMapTexCoord;

varying vec3 lightDirection;
varying vec3 halfAngle;
varying vec3 eyeDirection;
varying vec3 c0, c1, c2;

void main()
{
  const float pi2 = 6.28318530;  // 2 times Pi
  normalMapTexCoord = vec2(6.0*parametric[0], -2.0*parametric[1]);

  float M = torusInfo[0];
  float N = torusInfo[1];
  float theta = parametric[0] * pi2;
  float phi = parametric[1] * pi2;

  float coeff = M + N*cos(phi);
  float x = coeff*cos(theta);
  float y = coeff*sin(theta);
  float z = N*sin(phi);
  gl_Position = gl_ModelViewProjectionMatrix * vec4(x, y, z, 1);

  eyeDirection = normalize(eyePosition - vec3(x, y, z));  // XXX fix me
  lightDirection = normalize(lightPosition - vec3(x, y, z));

  vec3 gradientU = normalize(vec3(-(M + N*cos(phi))*sin(theta),
                        (M + N*cos(phi))*cos(theta),
                        0.0));
  
  vec3 gradientV = normalize(vec3(-N*sin(phi)*cos(theta),
                        -N*sin(theta)*sin(phi),
                        N*cos(phi)));

  c0 = gradientU;  // tangent
  c1 = gradientV; // binormal
  c2 = normalize(cross(gradientU,  // normal
             gradientV));
}
// Wolfram
// x
// diff((M + N*cos(phi))*cos(theta), theta)
// diff((M + N*cos(phi))*cos(theta), phi)
// y
// diff((M + N*cos(phi))*sin(theta), theta)
// diff((M + N*cos(phi))*sin(theta), phi)
// z
// diff(N*sin(phi), theta)
// diff(N*sin(phi), phi)
