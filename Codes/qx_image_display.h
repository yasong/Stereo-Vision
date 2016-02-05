/*$Id: qx_image_display.h,v 1.2 2007/04/04 02:15:14 liiton Exp $*/
/*******************************************************
\Author:	Qingxiong Yang (liiton)
\Function:	Image debugging.
********************************************************/
#ifndef QX_IMAGE_DISPLAY_H
#define QX_IMAGE_DISPLAY_H
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "imdebug.h"
#include "qx_basic.h"
#pragma comment(lib,"imdebug.lib")
/*image debug*/
inline void image_display(int *in,int h,int w)
{
	int i,len=h*w; float *disp;
	disp=new float [len];
	for(i=0;i<len;i++) disp[i]=(float)in[i];
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,disp);
	delete [] disp; disp=NULL;
}
inline void image_display(unsigned int*in,int h,int w)
{
	int i,len=h*w; float *disp;
	disp=new float [len];
	for(i=0;i<len;i++) disp[i]=(float)in[i];
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,disp);
	delete [] disp; disp=NULL;
}
inline void image_display(unsigned char **in,int h,int w)
{
	imdebug("lum *auto w=%d h=%d %p",w,h,&in[0][0]);
}
inline void image_display(char **in,int h,int w)
{
	imdebug("lum *auto w=%d h=%d %p",w,h,&in[0][0]);
}
inline void image_display(float **in,int h,int w)
{
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,&in[0][0]);
}
inline void image_display(float **in,int h,int w,float threshold)
{
	float *disp,*disp_in; int len,i;
	len=h*w;
	disp=new float [len];
	disp_in=in[0];
	for(i=0;i<len;i++) if(disp_in[i]>=threshold) disp[i]=1; else disp[i]=0;
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,disp);
	delete [] disp; disp=NULL;
}
inline void image_display(unsigned short **in,int h,int w)
{
	float **disp=qx_allocf(h,w);
	for(int y=0;y<h;y++) for(int x=0;x<w;x++) disp[y][x]=(float)in[y][x];
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,&disp[0][0]);
	qx_freef(disp); disp=NULL;
}
inline void image_display(int **in,int h,int w)
{
	float **disp=qx_allocf(h,w);
	for(int y=0;y<h;y++) for(int x=0;x<w;x++) disp[y][x]=(float)in[y][x];
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,&disp[0][0]);
	qx_freef(disp); disp=NULL;
}
inline void image_display(long int **in,int h,int w)
{
	float **disp=qx_allocf(h,w);
	for(int y=0;y<h;y++) for(int x=0;x<w;x++) disp[y][x]=(float)in[y][x];
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,&disp[0][0]);
	qx_freef(disp); disp=NULL;
}
inline void image_display(unsigned char ***in,int h,int w)
{
	imdebug("rgb *auto w=%d h=%d %p",w,h,&(in[0][0][0]));
}
inline void image_display(float ***in,int h,int w)
{
	imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,&in[0][0][0]);
}
inline void image_display(double ***in,int h,int w)
{
	float ***out=qx_allocf_3(h,w,3);
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++) 
		{
			out[y][x][0]=(float)in[y][x][0];
			out[y][x][1]=(float)in[y][x][1];
			out[y][x][2]=(float)in[y][x][2];
		}
	}
	imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,&out[0][0][0]);
	qx_freef_3(out); out=NULL;
}
inline void image_display(int***in,int h,int w)
{
	float***out=qx_allocf_3(h,w,3);
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++) 
		{
			out[y][x][0]=(float)in[y][x][0];
			out[y][x][1]=(float)in[y][x][1];
			out[y][x][2]=(float)in[y][x][2];
		}
	}
	imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,&out[0][0][0]);
	qx_freef_3(out); out=NULL;
}
inline void image_display_2(float ***in,int h,int w)
{
	float ***out=qx_allocf_3(h,w,3);
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++) 
		{
			out[y][x][0]=in[y][x][0];
			out[y][x][1]=in[y][x][1];
			out[y][x][2]=0;
		}
	}
	imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,&out[0][0][0]);
	qx_freef_3(out); out=NULL;
}

inline void image_display_2(double***in,int h,int w)
{
	float ***out=qx_allocf_3(h,w,3);
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++) 
		{
			out[y][x][0]=in[y][x][0];
			out[y][x][1]=in[y][x][1];
			out[y][x][2]=0;
		}
	}
	imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,&out[0][0][0]);
	qx_freef_3(out); out=NULL;
}
inline void image_display_2(unsigned char***in,int h,int w)
{
	unsigned char***out=qx_allocu_3(h,w,3);
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++) 
		{
			out[y][x][0]=in[y][x][0];
			out[y][x][1]=in[y][x][1];
			out[y][x][2]=0;
		}
	}
	imdebug("rgb *auto w=%d h=%d %p",w,h,&out[0][0][0]);
	qx_freeu_3(out); out=NULL;
}
inline void image_display_2(int ***in,int h,int w)
{
	float ***out=qx_allocf_3(h,w,3);
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++) 
		{
			out[y][x][0]=(float)in[y][x][0];
			out[y][x][1]=(float)in[y][x][1];
			out[y][x][2]=0;
		}
	}
	imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,&out[0][0][0]);
	qx_freef_3(out); out=NULL;
}
inline void image_display_4(unsigned char ***in,int h,int w,int nr_image)
{
	unsigned char **out=qx_allocu(nr_image,h*w*4);
	for(int i=0;i<nr_image;i++) 
	{
		for(int y=0;y<h;y++) for(int x=0;x<w;x++) for(int c=0;c<4;c++) out[i][y*w*4+x*4+c]=in[y][x][4*i+c];
		imdebug("rgba *auto w=%d h=%d %p",w,h,out[i]);
	}
	qx_freeu(out); out=NULL;
}
inline void image_display_4(float ***in,int h,int w,int nr_image=1)
{
	float**out=qx_allocf(nr_image,h*w*4);
	for(int i=0;i<nr_image;i++) 
	{
		for(int y=0;y<h;y++) for(int x=0;x<w;x++) for(int c=0;c<4;c++) out[i][y*w*4+x*4+c]=in[y][x][4*i+c];
		imdebug("rgba *auto b=32f w=%d h=%d %p",w,h,out[i]);
	}
	qx_freef(out); out=NULL;
}
inline void image_display_3(float ***in,int h,int w,int nr_image)
{
	float**out=qx_allocf(nr_image,h*w*3);
	for(int i=0;i<nr_image;i++) 
	{
		for(int y=0;y<h;y++) for(int x=0;x<w;x++) for(int c=0;c<3;c++) out[i][y*w*4+x*4+c]=in[y][x][4*i+c];
		imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,out[i]);
	}
	qx_freef(out); out=NULL;
}
inline void image_display_4(unsigned char***in,int h,int w){imdebug("rgba *auto w=%d h=%d %p",w,h,&in[0][0][0]);}
inline void image_display_4(unsigned char*in,int h,int w){imdebug("rgba *auto w=%d h=%d %p",w,h,in);}
inline void image_display_4(float*in,int h,int w){imdebug("rgba *auto b=32f w=%d h=%d %p",w,h,in);}
inline void image_display_4(short*in,int h,int w){imdebug("rgba *auto b=16 w=%d h=%d %p",w,h,in);}
inline void image_display_4(unsigned int*in,int h,int w){float*out=new float[h*w*4]; for(int i=0;i<h*w*4;i++) out[i]=(float)in[i]; imdebug("rgba *auto b=32f w=%d h=%d %p",w,h,out); delete [] out;}
inline void image_display_3(unsigned char*in,int h,int w){imdebug("rgb *auto w=%d h=%d %p",w,h,in);}
inline void image_display_3(float*in,int h,int w){imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,in);}
inline void image_display_3(double*in,int h,int w){float*out=new float[h*w*3]; for(int i=0;i<h*w*3;i++) out[i]=(float)in[i]; imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,out); delete [] out;}
inline void image_display_3(int*in,int h,int w){float*out=new float[h*w*3]; for(int i=0;i<h*w*3;i++) out[i]=(float)in[i]; imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,out); delete [] out;}
inline void image_display_3(unsigned int*in,int h,int w){float*out=new float[h*w*3]; for(int i=0;i<h*w*3;i++) out[i]=(float)in[i]; imdebug("rgb *auto b=32f w=%d h=%d %p",w,h,out); delete [] out;}
inline void image_display_3(short*in,int h,int w){imdebug("rgb *auto b=16 w=%d h=%d %p",w,h,in);}
inline void image_display_3(unsigned short*in,int h,int w){imdebug("rgb *auto b=16 w=%d h=%d %p",w,h,in);}
inline void image_display(unsigned char *in,int h,int w){imdebug("lum *auto w=%d h=%d %p",w,h,in);}
inline void image_display_rgb(unsigned char*in,int h,int w){imdebug("rgb *auto w=%d h=%d %p",w,h,in);}
inline void image_display(float *in,int h,int w){imdebug("lum *auto b=32f w=%d h=%d %p",w,h,in);}
inline void image_display(short*in,int h,int w){imdebug("lum *auto b=16 w=%d h=%d %p",w,h,in);}
inline void image_display(short**in,int h,int w){imdebug("lum *auto b=16 w=%d h=%d %p",w,h,in[0]);}
inline void image_display_rgba(float ***in,int h,int w)
{
	imdebug("rgba *auto b=32f w=%d h=%d %p",w,h,&in[0][0][0]);
}
inline void image_display(double *in,int h,int w)
{
	float *disp=new float [h*w];
	for(int y=0;y<h*w;y++) disp[y]=(float)in[y];
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,disp);
	delete [] disp; disp=NULL;
}
inline void image_display(char*in,int h,int w)
{
	float *disp=new float [h*w];
	for(int y=0;y<h*w;y++) disp[y]=(float)in[y];
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,disp);
	delete [] disp; disp=NULL;
}
inline void image_display_4(double*in,int h,int w)
{
	float *disp=new float[h*w*4];
	for(int y=0;y<h*w*4;y++) disp[y]=(float)in[y];
	imdebug("rgba *auto b=32f w=%d h=%d %p",w,h,disp);
	delete [] disp; disp=NULL;
}
inline void image_display(double **in,int h,int w)
{
	float **disp=qx_allocf(h,w);
	for(int y=0;y<h;y++) for(int x=0;x<w;x++) disp[y][x]=(float)in[y][x];
	imdebug("lum *auto b=32f w=%d h=%d %p",w,h,&disp[0][0]);
	qx_freef(disp); disp=NULL;
}
inline unsigned char dist_rgb(unsigned char *a,unsigned char *b)
{
	float out; 
	out=float(abs(a[0]-b[0])+abs(a[1]-b[1])+abs(a[2]-b[2])); 
	return(unsigned char(out/3+0.5f));
}

#endif
