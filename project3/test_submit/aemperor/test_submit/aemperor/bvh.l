%{
#include <stdio.h>
#include <iostream>
#include "bvh_defs.h"

using namespace std;
extern "C" 
{
	extern int yylex(void);
}

#include "y.tab.h"

%}

%option yylineno

delim	[ \t\r\f]
ws	{delim}+
nl	[\n]
digit	[0-9]
partype	position|rotation
param	(X|Y|Z){partype}
letter 	[a-zA-Z]
id	{letter}|({letter}|{digit})* 
lbrack  \{
rbrack	\} 
sign	[\+\-]{0,1}
int	{sign}{digit}+
float	{sign}{digit}+\.{digit}*|{sign}\.{digit}+
colon	[:]

%x SG
%x MO
%x FL
%%
<SG,MO,FL,INITIAL>{ws}
<SG,INITIAL>{nl}
<MO,FL>{nl} {return NEWLINE;}
HIERARCHY { return HIERARCHY; }
ROOT { BEGIN(SG);return ROOT; }
<SG>JOINT { return JOINT; }
<SG>OFFSET {return OFFSET; }
<SG>CHANNELS { return CHANNELS; }
<SG>MOTION { BEGIN(MO);return MOTION; }
<SG>{param}	{ 
	// remember to delete sval after using it!
	if(strcmp(yytext,"Xposition")==0) yylval.ival=BVH_XPOS_IDX; 
	if(strcmp(yytext,"Yposition")==0) yylval.ival=BVH_YPOS_IDX;
	if(strcmp(yytext,"Zposition")==0) yylval.ival=BVH_ZPOS_IDX;
	if(strcmp(yytext,"Xrotation")==0) yylval.ival=BVH_XROT_IDX;
	if(strcmp(yytext,"Yrotation")==0) yylval.ival=BVH_YROT_IDX;
	if(strcmp(yytext,"Zrotation")==0) yylval.ival=BVH_ZROT_IDX;
	return PARAM;
}
<MO>Frames { return FRAMES;}
<MO>Frame { return FRAME;}
<MO>Time { return TIME;}
<SG>End { return END;}
<SG>Site { return SITE;}
<SG>{lbrack} { return LBRACK; }
<SG>{rbrack} { return RBRACK; }
<SG,MO,FL>{float} { yylval.fval=atof(yytext); return FLOAT;}
<SG,MO,FL>{int} { yylval.ival = atoi(yytext);return INT;}
<SG>{id} {
	// remember to delete sval after using it!
	strcpy((yylval.sval=new char[strlen(yytext)+1]),yytext);
	return ID;
}
<MO>{colon} {return COLON;}
%%
void BEGIN_FL(void)
{
	BEGIN(FL);
}
