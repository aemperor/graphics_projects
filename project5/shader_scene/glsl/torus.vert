
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
  normalMapTexCoord = vec2(-6.0*parametric[0], -2.0*parametric[1]);

  float M = torusInfo[0];
  float N = torusInfo[1];
  float theta = parametric[0];
  float phi = parametric[1];
  
  float coeff = M + N*cos(phi*pi2);
  float y = coeff*sin(theta*pi2);
  float x = coeff*cos(theta*pi2);
  float z = N*sin(phi*pi2);
  gl_Position = gl_ModelViewProjectionMatrix * vec4(x, y, z, 1);
  
  eyeDirection = vec3(0);  // XXX fix me
  lightDirection = lightPosition - vec3(x, y, z);
  halfAngle = normalize((lightPosition + gl_Position)/2.0);
  
  vec3 gradientU = vec3((-1.0*pi2*sin(pi2*theta)) * (M + N*cos(pi2*phi)), 
                         pi2*cos(pi2*theta) * (M+N*cos(pi2*phi)), 
                         0.0);  
  vec3 gradientV = vec3((-1.0*pi2*N*cos(pi2*theta)) * sin(pi2*phi), 
                        (-1.0*pi2*N*sin(pi2*theta)) * sin (pi2*phi), 
                        pi2*N*cos(pi2*phi));

  c0 = normalize(gradientU);  // tangent
  c1 = cross(normalize(gradientU),  // normal
             normalize(gradientV));
  c2 = cross(c1, c0);;  // binormal
}
// Wolfram
// x
// diff((M + N*cos(phi*pi2))*cos(theta*pi2), theta)
// diff((M + N*cos(phi*pi2))*cos(theta*pi2), phi)
// y
// diff((M + N*cos(phi*pi2))*sin(theta*pi2), theta)
// diff((M + N*cos(phi*pi2))*sin(theta*pi2), phi)
// z
// diff(N*sin(phi*pi2), theta)
// diff(N*sin(phi*pi2), phi)
