#ifndef __BVH_LOADER_H_
#define __BVH_LOADER_H_

#include "bvh_cb_info.h"
#include "joint.h"

class BVHLoader
{
 public:
  static SceneGraph * psg;
  static bvh_cb_info bci;
  static void loadBVH(const char * filename, SceneGraph * sg)
  {
    BVHLoader::psg=sg;
    set_bvh_cb_info(&BVHLoader::bci);
    load_bvh(filename);
  }
  static void createRoot(const char * name, uint32_t id)
  {
    BVHLoader::psg->CreateRoot(name,id);
  }
  static void createJoint(const char * name, uint32_t id) 
  {
    BVHLoader::psg->CreateJoint(name,id);
  }
  static void createEndSite(const char * name, uint32_t id)
  {
    BVHLoader::psg->CreateEndSite(name,id);
  }
  static void setChild(uint32_t parent, uint32_t child)
  {
    BVHLoader::psg->SetChild(parent,child);
  }
  static void setOffset(uint32_t id, float * offset)
  {
    BVHLoader::psg->SetOffset(id,offset);
  }
  static void setNumChannels(uint32_t id, uint16_t num)
  {
    BVHLoader::psg->SetNumChannels(id,num);
  }
  static void setChannelFlags(uint32_t id, uint16_t flags)
  {
    BVHLoader::psg->SetChannelFlags(id,flags);
  }
  static void setChannelOrder(uint32_t id, int * order)
  {
    BVHLoader::psg->SetChannelOrder(id,order);
  }
  static void setFrameIndex(uint32_t id, uint32_t index)
  {
    BVHLoader::psg->SetFrameIndex(id,index);
  }
  static void setFrameTime(float delta)
  {
    BVHLoader::psg->SetFrameTime(delta);
  }
  static void setNumFrames(uint32_t num)
  {
    BVHLoader::psg->SetNumFrames(num);
  }
  static void setFrameSize(uint32_t size)
  {
    BVHLoader::psg->SetFrameSize(size);
  }
  static void addFrame(float * data)
  {
    BVHLoader::psg->AddFrame(data);
  }
};

SceneGraph * BVHLoader::psg=0;

bvh_cb_info BVHLoader::bci={
  BVHLoader::createRoot,
  BVHLoader::createJoint,
  BVHLoader::createEndSite,
  BVHLoader::setChild,
  BVHLoader::setOffset,
  BVHLoader::setNumChannels,
  BVHLoader::setChannelFlags,
  BVHLoader::setChannelOrder,
  BVHLoader::setFrameIndex,
  BVHLoader::setFrameTime,
  BVHLoader::setNumFrames,
  BVHLoader::setFrameSize,
  BVHLoader::addFrame
};

#endif
