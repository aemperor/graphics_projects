#include <string>
#include <vector>
#include <sstream>
#include "./io.h"

using namespace std;

namespace {

string Dir(const string& fn) {
  int i = fn.rfind('/');
  if (i == string::npos) {
    i = fn.find('\\');
  }
  if (i != string::npos) {
    return fn.substr(0, i);
  }
  return ".";
}

float atof(const string& s) {
  return ::atof(s.c_str());
}

int atoi(const string& s) {
  return ::atoi(s.c_str());
}

string Trim(const string& s) {
  int i = 0;
  int j = s.size()-1;
  while (i < s.size() && (s[i] == ' ' || s[i] == '\t')) {
    i++;
  }
  while (j > i && (s[j] == ' ' || s[j] == '\t')) {
    j--;
  }
  if (i == j) return "";
  return s.substr(i, j+1-i);
}

string GetLine(istream* pin) {
  istream& in = *pin;
  string line;
  do {
    getline(in, line);
    line = Trim(line);
  } while (!in.eof() && line[0] == '#');
  return line;
}

void Parse(const string& line, string* t, vector<string>* values) {
  stringstream ss(line);
  ss >> *t;
  while (!ss.eof()) {
    string s;
    ss >> s;
    values->push_back(s);
  }
}

void ParseVertex(const string& s, int* vi, int* ti, int* ni) {
  // Find the offsets of the texture and normal indices
  int t_offset = s.find('/');
  int n_offset = s.size();
  if (t_offset != string::npos) {
    n_offset = s.find('/', t_offset+1);
    if (n_offset == string::npos) {
      n_offset = s.size();
    }
  } else {
    t_offset = s.size();
  }

  *vi = atoi(s.substr(0, t_offset)) - 1;
  string ts, ns;
  if (t_offset < n_offset-1) {
    ts = s.substr(t_offset+1, n_offset-(t_offset+1));
    if (n_offset < s.size()-1) {
      ns = s.substr(n_offset+1);
    }
  }
  if (!ts.empty()) {
    *ti = atoi(ts) - 1;
  }
  if (!ns.empty()) {
    *ni = atoi(ns) - 1;
  }
}

void ParseMaterial(const string& fn, Mesh& mesh) {
  const string dir = Dir(fn);
  ifstream in(fn.c_str());
  string line = GetLine(&in);
  int cnt = -1, line_cnt = 0;
  while (!in.eof()) {
    string t;
    vector<string> values;
    Parse(line, &t, &values);
    if (line.empty()) {
      // do nothing
    } else if (t == "newmtl") {
      mesh.new_material(++cnt, values[0]);
    } else if (t == "Ka") {
      mesh.set_ambient(cnt,
          Vec3f::makeVec(atof(values[0]), atof(values[1]), atof(values[2])));
    } else if (t == "Kd") {
      mesh.set_diffuse(cnt,
          Vec3f::makeVec(atof(values[0]), atof(values[1]), atof(values[2])));
    } else if (t == "Ks") {
      mesh.set_specular(cnt,
          Vec3f::makeVec(atof(values[0]), atof(values[1]), atof(values[2])));
    } else if (t == "Ns") {
      mesh.set_specular_coeff(cnt, atof(values[0]));
    } else if (t == "map_Ka" || t == "map_Kd") {
      string tfn = values[0];
      if (!dir.empty()) {
        tfn = dir + "/" + tfn;
      }
      mesh.set_texture(cnt, tfn);
    } else {
      // cout << "Unrecognized material line: " << line
      //      << " " << line_cnt << endl;
    }
    line = GetLine(&in);
    line_cnt++;
  }
}

}  // end empty namespace

void ParseObj(const string& fn, Mesh& mesh) {
  const string dir = Dir(fn);
  ifstream in(fn.c_str());
  string line = GetLine(&in);
  int cnt = 0;
  while (!in.eof()) {
    if (line.empty()) {
      // do nothing
    } else if (line.find("mtllib") != string::npos) {
      string mfn = fn;
      if (!dir.empty()) {
        mfn = dir + "/" + line.substr(7);
      }
      ParseMaterial(mfn, mesh);
    } else if (line.find("usemtl") != string::npos) {
      const string fn = line.substr(7);
      mesh.set_cur_material(fn);
    } else if (line[0] == 'g') {
      // cout << "group " << line.substr(2) << endl;
    } else if (line[0] == 'o') {
      // cout << "object " << line.substr(2) << endl;
    } else if (line[0] == 's') {
      // cout << "smooth shading " << line.substr(2) << endl;
    } else {
      string t;
      vector<string> values;
      Parse(line, &t, &values);
      if (t == "v") {
        mesh.AddVertex(Vec3f::makeVec(
            atof(values[0]), atof(values[1]), atof(values[2])));
      } else if (t == "vt") {
        float v[3];
        for (int i = 0; i < 3; ++i) {
          v[i] = atof(values[i]);
        }
        mesh.AddTextureVertex(Vec3f::makeVec(v[0], v[1], v[2]));
      } else if (t == "f") {
        vector<int> vertices(values.size());
        vector<int> texture(values.size(), -1);
        vector<int> normals(values.size(), -1);
        for (int i = 0; i < values.size(); ++i) {
          ParseVertex(values[i], &vertices[i], &texture[i], &normals[i]);
        }
        mesh.AddPolygon(vertices, texture);
      } else {
        cout << "Unknown type " << t << " on line " << cnt+1 << endl;
      }
    }
    line = GetLine(&in);
    cnt++;
  }
}

