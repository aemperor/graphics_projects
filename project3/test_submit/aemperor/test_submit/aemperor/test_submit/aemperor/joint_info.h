#ifndef _JOINT_INFO_H_
#define _JOINT_INFO_H_

#include<vector>

#include "bvh_cb_info.h"

using namespace std;

struct joint_info
{
	unsigned char type; // 0=root, 1=joint, 2=end site
	unsigned int id;
	unsigned int parent;
	char * name; 
	unsigned int numchans;
	unsigned short chanflags;
	int order[6];
	float offset[3];
};

vector<joint_info> joints;
joint_info   curr;
extern bvh_cb_info * bvh_cbs;
unsigned int last_id;
unsigned int jcnt;
unsigned int paramcount;

#endif
