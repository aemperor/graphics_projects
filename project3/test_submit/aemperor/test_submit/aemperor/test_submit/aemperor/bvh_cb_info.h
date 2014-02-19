#ifndef _BVH_CB_INFO_H_
#define _BVH_CB_INFO_H_

struct bvh_cb_info
{
	void (*create_root)(const char * name,unsigned int id);
	void (*create_joint)(const char * name,unsigned int id);
	void (*create_end_site)(const char * name,unsigned int id);
	void (*set_child)(unsigned int parent_id, unsigned int child_id);
	void (*set_offset)(unsigned int id, float  * off);
	void (*set_num_channels)(unsigned int id, unsigned short int num);
	void (*set_channel_flags)(unsigned int id, unsigned short flags);
	void (*set_channel_order)(unsigned int id, int * order);
	void (*set_frame_index)(unsigned int id, unsigned int index);
	void (*set_frame_time)(float frame_time);
	void (*set_num_frames)(unsigned int num);
	void (*set_frame_size)(unsigned int sz);
	void (*add_frame)(float * frame);
};

void set_bvh_cb_info(bvh_cb_info * info);
extern int load_bvh(const char * filename);

#endif

