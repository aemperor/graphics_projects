#include <stdint.h>
#include <iostream>
#include <vector>

#include "./joint.h"

using namespace std;

void SceneGraph::CreateRoot(const char * name, uint32_t id) {
  cout << "createRoot:name=" << name << " id=" << id << endl;
  // TODO
  // special case for root -- does not need Resize()
  listOfJoints.push_back(Joint3(name, id));
  listOfJoints[id].type = 0;
}

void SceneGraph::CreateJoint(const char * name, uint32_t id) {
  cout << "createJoint:name=" << name << " id=" << id << endl;
  // TODO
  if (listOfJoints.size() <= id)
    Resize(id);
  listOfJoints[id] = Joint3(name, id);
  listOfJoints[id].isChild = 0;
  listOfJoints[id].type = 1;
}

void SceneGraph::CreateEndSite(const char * name, uint32_t id) {
  cout << "createEndSite:name=" << name << " id=" << id;
  // TODO
  if (listOfJoints.size() <= id)
    Resize(id);
  listOfJoints[id] = Joint3(name, id);
  listOfJoints[id].type = 2;
}

void SceneGraph::SetChild(uint32_t parent, uint32_t child) {
  cout << "setChild:parent=" << parent << " child=" << child << endl;
  // TODO
  listOfJoints[parent].childIds.push_back(child);
  listOfJoints[child].isChild = 1;
}

void SceneGraph::SetOffset(uint32_t id, float * offset) {
  cout << "setOffset:id=" << id << " offset=(" << offset[0] << "," << offset[1]
       << "," << offset[2] << ")" << endl;
  // TODO
  listOfJoints[id].x = offset[0];
  listOfJoints[id].y = offset[1];
  listOfJoints[id].z = offset[2];
}

void SceneGraph::SetNumChannels(uint32_t id, uint16_t num) {
  cout << "setNumChannels:id=" << id << " num=" << num << endl;
  // TODO
  // right now all this does is set the number of channels in each joint
  listOfJoints[id].channel = num;
}

void SceneGraph::SetChannelFlags(uint32_t id, uint16_t flags) {
  cout << "setChannelFlags:id=" << id << " flags=" << flags << endl;
  // TODO
}

void SceneGraph::SetChannelOrder(uint32_t id, int * order) {
  cout  << "setChannelOrder:id=" << id << endl;
  for (int i = 0; i < 6; i++) {
    cout << "order[]" << order[i] << endl;
    listOfJoints[id].ordr[i] = order[i];
  }
  // TODO
}

void SceneGraph::SetFrameIndex(uint32_t id, uint32_t index) {
  cout << "setFrameIndex:id=" << id << " index=" << index << endl;
  // TODO
  // listOfJoints[id].frameIdx = index;
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
  Frame f = Frame();
  int i = 0;
  while (data[i] != NULL) {
    cout << "data: " << data[i] <<endl;
    f.dataF.push_back(data[i]);
    i++;
  }
  frames.push_back(f);
  // TODO
}

void SceneGraph::SetCurrentFrame(uint32_t frameNumber) {
  cout << "setCurrentFrame:frameNumber=" << frameNumber << endl;
}

void SceneGraph::Resize(uint32_t id) {
  while (id >= listOfJoints.size()) {
    listOfJoints.resize(listOfJoints.size()*2);
  }
}

