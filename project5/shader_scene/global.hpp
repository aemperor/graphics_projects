

#ifndef __global_hpp__
#define __global_hpp__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "scene.hpp"

extern ScenePtr scene;
extern MaterialPtr material;
extern LightPtr light;
extern CubeMapPtr envmap;

extern float bump_height;

void stopObjectSpinning();
void toggleWireframe();

#endif // __global_hpp__

