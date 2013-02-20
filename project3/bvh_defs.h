#ifndef _BVH_DEFS_H_
#define _BVH_DEFS_H_

#define BVH_XPOS		0x1
#define BVH_YPOS		0x2
#define BVH_ZPOS		0x4
#define BVH_XROT		0x8
#define BVH_YROT		0x10
#define BVH_ZROT		0x20

#define BVH_XPOS_IDX		0
#define BVH_YPOS_IDX		1
#define BVH_ZPOS_IDX		2
#define BVH_XROT_IDX		3
#define BVH_YROT_IDX		4
#define BVH_ZROT_IDX		5

#define BVH_MAX_CHANS		6
#define BVH_CHAN_INVALID	-1

#define BVH_ROOT		0x0
#define BVH_JOINT		0x1
#define BVH_END_SITE		0x2

#endif
