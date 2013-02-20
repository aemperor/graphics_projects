#include <stdint.h>
#include <iostream>

#include "./joint.h"

using namespace std;

void SceneGraph::CreateRoot(const char * name, uint32_t id) {
  cout << "createRoot:name=" << name << " id=" << id << endl;
  // TODO
}

void SceneGraph::CreateJoint(const char * name, uint32_t id) {
  cout << "createJoint:name=" << name << " id=" << id << endl;
  // TODO
}

void SceneGraph::CreateEndSite(const char * name, uint32_t id) {
  cout << "createEndSite:name=" << name << " id=" << id;
  // TODO
}

void SceneGraph::SetChild(uint32_t parent, uint32_t child) {
  cout << "setChild:parent=" << parent << " child=" << child << endl;
  // TODO
}

void SceneGraph::SetOffset(uint32_t id, float * offset) {
  cout << "setOffset:id=" << id << " offset=(" << offset[0] << "," << offset[1]
       << "," << offset[2] << ")" << endl;
  // TODO
}

void SceneGraph::SetNumChannels(uint32_t id, uint16_t num) {
  cout << "setNumChannels:id=" << id << " num=" << num << endl;
  // TODO
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
