
Welcome to Project 2 for CS 354 Computer Graphics (Spring 2012)

In this project, you'll write GLSL shaders.  These shaders will:

  *  roll up a flat 2D grid into the shape a torus (doughnut) in a
     vertex shader

  *  implement ambient, diffuse, and specular light contribution

  *  sample a surface decal stored in a 2D texture

  *  implement per-fragment normal mapping

  *  implement reflection mapping into a cube map environment

Build instructions:

  cd shader_scene  # this directory
  make debug      # builds debug version
  make release    # builds release version
  make both       # build both versions

Starting the project:

  Run the project (making sure the executable is up-to-date):

  make rrun     # run the release version
  make drun     # run the debug version
  make gdb      # run the debug version in the gdb debugger

  Or run the executables directly

  ./bin.release64/shader_scene
  ./bin.debug/shader_scene -gldebug

  (If you see a red square, that's good; your mission is to "fix"
  that red square and make it a torus with the proper shading.)

Controling the project:

  Left mouse button rotates eye point around object

  Ctrl+Left mouse button spins the object itself

  Middle mouse button rotates light source around object

  Right mouse button shows pop-up menu

  'w' key toggles wireframe

  'v' key toggles vertical refresh synchronization (on by default)

  '0' through '9' keys switch shaders (also possible from menu)

What you need to do:

  *  Fix the vertex shader to fold the grid into a torus

  *  Fix the vertex shader to send the varying values needed by
     your fragment shaders

  *  Fix the fragment shaders glsl/0*_*.frag as described in the PDF

  *  Fix texture.c's computeNormal routine to convert a height map
     texel into a normal map texel (by computng gradient with neighbor
     height field texels)

How to do this?

  *  Edit fragment shaders in glsl/*.frag

  *  Edit the glsl/torus.vert

  *  Fix computeNormal

  Other than fixing computeNormal, you should not need to modify the
  C/C++ code -- but you are free to do so.

Need more details?

  Look for the PDF

Questions?

  Ask on piazza.com

      http://piazza.com/class#spring2012/cs354

- Mark Kilgard
  March 27, 2012
