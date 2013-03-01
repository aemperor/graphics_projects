#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "./joint.h"

using namespace std;
int lastAddedFrame = 0;

void SceneGraph::CreateRoot(const char * name, uint32_t id) {
  // cout << "createRoot:name=" << name << " id=" << id << endl;
  listOfJoints.push_back(Joint3(name, id));
  listOfJoints[id].type = 0;
  listOfJoints[id].channel = 0;
}

void SceneGraph::CreateJoint(const char * name, uint32_t id) {
  // cout << "createJoint:name=" << name << " id=" << id << endl;
  listOfJoints.push_back(Joint3(name, id));
  listOfJoints[id].isChild = 0;
  listOfJoints[id].type = 1;
  listOfJoints[id].channel = 0;
}

void SceneGraph::CreateEndSite(const char * name, uint32_t id) {
  // cout << "createEndSite:name=" << name << " id=" << id;
  listOfJoints.push_back(Joint3(name, id));
  listOfJoints[id].type = 2;
  listOfJoints[id].channel = 0;
}

void SceneGraph::SetChild(uint32_t parent, uint32_t child) {
  // cout << "setChild:parent=" << parent << " child=" << child << endl;
  listOfJoints[parent].childIds.push_back(child);
  listOfJoints[child].isChild = 1;
}

void SceneGraph::SetOffset(uint32_t id, float * offset) {
  // cout << "setOffset:id=" << id <<
  // " offset=(" << offset[0] << "," << offset[1]
       // << "," << offset[2] << ")" << endl;
  listOfJoints[id].xoff = offset[0];
  listOfJoints[id].yoff = offset[1];
  listOfJoints[id].zoff = offset[2];
}

void SceneGraph::SetNumChannels(uint32_t id, uint16_t num) {
  // cout << "setNumChannels:id=" << id << " num=" << num << endl;
  // right now all this does is set the number of channels in each joint
  listOfJoints[id].channel = num;
}

void SceneGraph::SetChannelFlags(uint32_t id, uint16_t flags) {
  // cout << "setChannelFlags:id=" << id << " flags=" << flags << endl;
}

void SceneGraph::SetChannelOrder(uint32_t id, int * order) {
  // cout  << "setChannelOrder:id=" << id << endl;
  for (int i = 0; i < 6; i++) {
    listOfJoints[id].ordr[i] = order[i];
  }
}

void SceneGraph::SetFrameIndex(uint32_t id, uint32_t index) {
  // cout << "setFrameIndex:id=" << id << " index=" << index << endl;
  listOfJoints[id].frameIdx = index;
}

void SceneGraph::SetFrameTime(float delta) {
  // cout << "setFrameTime:delta=" << delta << endl;
  frameTime = delta;
}

void SceneGraph::SetNumFrames(uint32_t num) {
  // cout << "setNumFrames:num=" << num << endl;
}

void SceneGraph::SetFrameSize(uint32_t size) {
  // cout << "setFrameSize:size=" << size << endl;
  frameSize = size;
}

void SceneGraph::AddFrame(float * data) {
  float bs = 0.0;
  int i = 0;
  //  float * tempFrameData =
  //  reinterpret_cast<float*>(malloc(frameSize*sizeof(bs)));
  //  memcpy(tempFrameData, data, frameSize);
  Frame newFrame = Frame();
  while (i < frameSize) {
    // cout << "i: " << i << "data: " << data[i] << endl;
    newFrame.frameData.push_back(data[i]);
    i++;
  }
  frames.push_back(newFrame);
  lastAddedFrame++;
}

void SceneGraph::SetChannels() {
  // cout << "!!!!!!!!!in setChannels" << endl;
  //  int DataIndex = 0;
  //  for (int i = 1; i < listOfJoints.size(); i++) {
  //  for (int j = 0; j < listOfJoints[i].channel; j++) {
  //    listOfJoints[i].posRot.push_back(frames[0].frameData[DataIndex]);
  //  }
  //  }
}

void SceneGraph::SetCurrentFrame(uint32_t frameNumber) {
  // cout << "setCurrentFrame:frameNumber=" << frameNumber << endl;
}
