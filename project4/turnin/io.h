#ifndef __IO_H__
#define __IO_H__

#include <string>
#include <iostream>
#include <fstream>

#include "./mesh.h"

void ParseObj(const std::string& fn, Mesh& mesh);

#endif
