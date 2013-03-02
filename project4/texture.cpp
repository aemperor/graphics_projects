#include "./texture.h"

#include <jpeglib.h>
#include <jerror.h>

#include <iostream>
#include <cstring>
#include <cstdio>

#include "./common.h"

using namespace std;

namespace {
typedef unsigned char BYTE;
typedef GLuint UINT;
}  // end empty namespace

void LoadTexture(const string& filename, int texture_id) {
  jpeg_decompress_struct info;
  unsigned char* data;
  const string fn = filename;
  if (!fn.empty() && LoadJPEG(fn.c_str(), &data, &info, true)) {
    InitTexture(texture_id, data, info.output_width,
                info.output_height, info.num_components, true);
  } else {
    cout << "Failed to load " << filename << endl;
  }
}

// Load a texture into OpenGL
GLuint InitTexture(GLuint texture, unsigned char* data,
                   int width, int height, int channels, bool wrap) {
  typedef unsigned char BYTE;

  // select our current texture
  glBindTexture(GL_TEXTURE_2D, texture);

  if (channels == 3) {
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
  } else if (channels == 4) {
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Mix texture with color for shading
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // If wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  if (wrap) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  }

  free(data);

  return texture;
}

bool LoadJPEG(const char* FileName, BYTE** out_data,
              jpeg_decompress_struct* out_info,
              bool Fast) {
  FILE* file = fopen(FileName, "rb");
  jpeg_decompress_struct& info = *out_info;
  // error handler
  struct jpeg_error_mgr err;

  // Tell the jpeg decompression handler to send the errors to err
  info.err = jpeg_std_error(&err);
  // Set info defaults
  jpeg_create_decompress(&info);

  if (!file) {
    fprintf(stderr, "Error reading JPEG file %s\n", FileName);
    return false;
  }

  // Tell the jpeg lib the file we're reading and read it
  jpeg_stdio_src(&info, file);
  jpeg_read_header(&info, TRUE);

  if (Fast) {
    info.do_fancy_upsampling = FALSE;
  }

  jpeg_start_decompress(&info);

  const size_t x = info.output_width;
  const size_t y = info.output_height;
  const int channels = info.num_components;
  const size_t size = x * y * channels;

  // Turn the uncompressed data into something ogl can read
  BYTE* data = new BYTE[size];

  BYTE* p1 = data;
  BYTE** p2 = &p1;
  int numlines = 0;

  while (info.output_scanline < info.output_height) {
    numlines = jpeg_read_scanlines(&info, p2, 1);
    *p2 += numlines * channels * info.output_width;
  }

  jpeg_finish_decompress(&info);
  fclose(file);

  // If scan lines are not a multiple of 4, then change channels to 4.
  // We only support converting from 3 right now.
  if ((x * channels) % 4 != 0 && channels == 3) {
    BYTE* new_data = new BYTE[x * y * 4];
    memset(new_data, x*y*4, 1);
    for (int i = 0; i < y; ++i) {
      for (int j = 0; j < x; ++j) {
        memcpy(new_data+(i*x+j)*4, data+(i*x+j)*3, 3);
      }
    }
    delete data;
    data = new_data;

    info.num_components = 4;
  }

  *out_data = data;

  return true;
}
