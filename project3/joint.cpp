#include <stdint.h>
#include <iostream>
#include <vector>

#include "./joint.h"

using namespace std;

void SceneGraph::CreateRoot(const char * name, uint32_t id) {
  cout << "createRoot:name=" << name << " id=" << id << endl;
  // TODO
  listOfJoints.push_back(Joint3(name, id));
  cout << "LOJ SIZE = " << listOfJoints.size() << endl;
}

void SceneGraph::CreateJoint(const char * name, uint32_t id) {
  cout << "createJoint:name=" << name << " id=" << id << endl;
  // TODO
  listOfJoints.push_back(Joint3(name, id));
  cout << "LOJ SIZE = " << listOfJoints.size() << endl;
}

void SceneGraph::CreateEndSite(const char * name, uint32_t id) {
  cout << "createEndSite:name=" << name << " id=" << id;
  // TODO
  listOfJoints.push_back(Joint3(name, id));
  listOfBodyParts.push_back(listOfJoints);
  listOfJoints.clear();
}

void SceneGraph::SetChild(uint32_t parent, uint32_t child) {
  cout << "setChild:parent=" << parent << " child=" << child << endl;
  // TODO
  for (int i = 0; i < listOfBodyParts.size(); ++i) {
    for (int j = 0; j < listOfBodyParts[i].size(); ++j) {
      if (listOfBodyParts[i][j].id == parent)
        listOfBodyParts[i][j].childId = child;
    }
  }
}

void SceneGraph::SetOffset(uint32_t id, float * offset) {
  cout << "setOffset:id=" << id << " offset=(" << offset[0] << "," << offset[1]
       << "," << offset[2] << ")" << endl;
  // TODO
  for (int i = 0; i < listOfBodyParts.size(); ++i) {
    for (int j = 0; j < listOfBodyParts[i].size(); ++j) {
      if (listOfBodyParts[i][j].id == id)
        listOfBodyParts[i][j].x = offset[0];
        listOfBodyParts[i][j].y = offset[1];
        listOfBodyParts[i][j].z = offset[2];
    }
  }
}

void SceneGraph::SetNumChannels(uint32_t id, uint16_t num) {
  cout << "setNumChannels:id=" << id << " num=" << num << endl;
  // TODO
  // right now all this does is set the number of channels in each joint
  if (num < 6) {
    for (int i = 0; i < listOfBodyParts.size(); ++i) {
      for (int j = 0; j < listOfBodyParts[i].size(); ++j) {
        if (listOfBodyParts[i][j].id == id)
          listOfBodyParts[i][j].channel = num;
      }
    }
  } else {  // this is root case
    for (int i = 0; i < listOfBodyParts.size(); ++i) {
      for (int j = 0; j < listOfBodyParts[i].size(); ++j) {
        if (listOfBodyParts[i][j].id == id)
          listOfBodyParts[i][j].channel = num;
      }
    }
  }
}

void SceneGraph::SetChannelFlags(uint32_t id, uint16_t flags) {
  cout << "setChannelFlags:id=" << id << " flags=" << flags << endl;
  // TODO
}

void SceneGraph::SetChannelOrder(uint32_t id, int * order) {
  cout  << "setChannelOrder:id=" << id << endl;
  // TODO
}

void SceneGraph::SetFrameIndex(uint32_t id, uint32_t index) {
  cout << "setFrameIndex:id=" << id << " index=" << index << endl;
  // TODO
}

void SceneGraph::SetFrameTime(float delta) {
  cout << "setFrameTime:delta=" << delta << endl;
  // TODO
}

void SceneGraph::SetNumFrames(uint32_t num) {
  cout << "setNumFrames:num=" << num << endl;
  // TODO
}

void SceneGraph::SetFrameSize(uint32_t size) {
  cout << "setFrameSize:size=" << size << endl;
  // TODO
}

void SceneGraph::AddFrame(float * data) {
  cout << "addFrame" << endl;
  // TODO
}

void SceneGraph::SetCurrentFrame(uint32_t frameNumber) {
  cout << "setCurrentFrame:frameNumber=" << frameNumber << endl;
}
