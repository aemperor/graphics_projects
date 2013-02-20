%{
	#include <iostream>
	#include <cstring>
	#include <cstdio>
	#include <stdint.h>
	#include "joint_info.h"
	#include "bvh_cb_info.h"
	#include "bvh_defs.h"
	using namespace std;
	void yyerror(const char * s);
	char endsitestr[]="_end_site_";
	int lb=0, rb=0,numframes=0,frameidx=0,framecnt=0;
	uint32_t framesz=0;
	extern "C"
	{
		extern int yylex(void);
	}	
	extern void BEGIN_FL(void);
	extern bvh_cb_info * bci;
	float * frameflt=0;
%}

%token HIERARCHY ROOT JOINT OFFSET CHANNELS
%token MOTION FRAMES FRAME TIME END SITE
%token LBRACK RBRACK COLON 
%token NEWLINE 

%union {
	int ival;
	float fval;
	char * sval;
}

%token <ival>  INT
%token <fval>  FLOAT
%token <sval>  ID
%token <ival> PARAM
%type  <fval> num
%%
bvh:header root motion; 
;
header:HIERARCHY {
	joints.clear();
	last_id=0;
	jcnt=0;
}
;
root: root_tag id joint_post
;
joint: joint_tag id joint_post
;
id: ID {
	curr.name=yylval.sval;
	curr.id=last_id++;
}
;
joint_post: lbrack joint_info end_site rbrack | lbrack joint_info joints rbrack 
;
joint_info: offset channels | channels offset | offset
;
end_site: end_site_tag lbrack offset rbrack
;
joints: joint | joints joint
;
root_tag: ROOT {curr.type=BVH_ROOT;jcnt++;}
;
joint_tag: JOINT {curr.type=BVH_JOINT;jcnt++;}
;
end_site_tag: END SITE
{curr.id=last_id++;curr.type=BVH_END_SITE;strcpy((curr.name=new char[strlen(endsitestr)+1]),endsitestr);jcnt++;}
;
lbrack: LBRACK {
	lb++;
	if(bci) {
		void (*create_fn)(const char *,unsigned int)=0;
		switch(curr.type)
		{
			case BVH_ROOT: create_fn=bci->create_root; break;
			case BVH_JOINT: create_fn=bci->create_joint; break;
			case BVH_END_SITE: create_fn=bci->create_end_site; break;
		}
		if(create_fn)
			create_fn(curr.name,curr.id);
	}
	delete [] curr.name;
	if(curr.type!=BVH_ROOT) {
		curr.parent=joints.back().id;
		if(bci && bci->set_child)
			bci->set_child(curr.parent,curr.id);
	}
	joints.push_back(curr);
	curr.chanflags=0;
}
;
rbrack: RBRACK {
	rb++;
	if(joints.size()>0) 
		joints.pop_back();
	else 
		yyerror("Unmatched bracket");
}
;
offset: OFFSET num num num  {
	curr.offset[0]=$2;
	curr.offset[1]=$3;
	curr.offset[2]=$4;
	if(bci && bci->set_offset)
		bci->set_offset(curr.id,curr.offset);
}
;
num: FLOAT | INT {
	$$=(float)$1;
};
channels: CHANNELS numchans params {
	if(paramcount!=curr.numchans) 
		yyerror("Number of params and actual number parsed do not match.");
	if(bci && bci->set_channel_flags)
		bci->set_channel_flags(curr.id,curr.chanflags);
	if(bci && bci->set_frame_index)
		bci->set_frame_index(curr.id,framesz);
	if(bci && bci->set_channel_order)
		bci->set_channel_order(curr.id,curr.order);
	framesz+=curr.numchans;
}
;
numchans: INT {
	if($1<0) yyerror("Negative argument not allowed for numchans.");
	curr.numchans=$1;
	memset(curr.order,BVH_CHAN_INVALID,BVH_MAX_CHANS*sizeof(int));
	paramcount=0;
	if(bci && bci->set_num_channels)
		bci->set_num_channels(curr.id,curr.numchans);
		if (curr.numchans>BVH_MAX_CHANS) 
			yyerror("Number of declared channels exceeds maximum number of allowed channels.");
}
;
params: param | params param
;
param: PARAM {
	if(((0x1<<(yylval.ival)) & curr.chanflags) != 0) yyerror("Chanel flag already set.");
	curr.chanflags |= (0x1<<(yylval.ival));
	curr.order[paramcount]=yylval.ival;
	paramcount++;
}
;
motion: motion_header frames frame_time frame_info {
	if(framecnt<numframes) yyerror("Not enough frames read.");
}
;
motion_header: MOTION NEWLINE
;
frames: FRAMES COLON INT NEWLINE {
	frameflt=new float[framesz];
	numframes=yylval.ival;
	if(numframes<1) yyerror("Bad argument for number of frames.");
	if(bci && bci->set_num_frames) bci->set_num_frames(numframes);
} 
;
frame_time: FRAME TIME COLON num NEWLINE {
	if(bci && bci->set_frame_time)
		bci->set_frame_time($4);
	BEGIN_FL();
}
;
frame_info: frame_line | frame_info frame_line 
;
frame_line: frame_body NEWLINE  {
	if(framecnt==numframes) yyerror("Too many frames read.");
	framecnt++;
	if(frameidx<framesz) yyerror("Not enough values for frame.");
	if(framecnt==1&&bci&&bci->set_frame_size)
		bci->set_frame_size(framesz);
	if(bci&&bci->add_frame) 
		bci->add_frame(frameflt);
	if(numframes==framecnt) 
		delete [] frameflt;
	frameidx=0;
}
;
frame_body: frame_float | frame_body frame_float 
;
frame_float: num {
	if(frameidx>framesz) yyerror("Too many values for frame.");
	frameflt[frameidx++]=$1;
}
;
%%
#include <stdio.h>
#include <iostream>
using namespace std;

extern "C"
{
	extern FILE * yyin;
	extern int yylex(void);
	extern int yylineno;
}

void yyerror(const char * s);

int load_bvh(const char * filename)
{
	FILE * file = fopen(filename,"r");
	if(!file)
	{
		cout << "can't open file"<<endl;
		return -1;
	}
	yyin=file;
	yylineno=1;
	do
	{
		yyparse();
	}
	while(!feof(yyin));
	return 0;
}

void yyerror(const char * s)
{
	cout <<"Parse error:"<<s<<" line:"<<yylineno<<endl;
	//exit(-1);
}

