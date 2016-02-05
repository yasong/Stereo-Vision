#include "qx_basic.h"
#include "qx_ppm.h"
#include "hl_SSD.h"
#include "qx_image_display.h"
#include "bilateral.h"

void stereo(char*filename_left_image, char*filename_right_image, char*filename_true_disparity,int max_disparity, int window_size,int change_size)
{
	unsigned char***left, ***right;
	unsigned char**disparity;
	unsigned char**truedisp;
	unsigned char**lower_resolution;
	unsigned char**bilateral_image;
	unsigned char***bilateral_rgb;

	int h, w;
	//int change_size = 2; ///////////////lower the resolution
	bool flag = false;
	qx_image_size(filename_left_image, h, w);//obtain image size


	left = qx_allocu_3(h, w, 3);//allocate memory
	bilateral_rgb = qx_allocu_3(h,w,3);
	right = qx_allocu_3(h, w, 3);//allocate memory
	disparity = qx_allocu(h, w);//allocate memory
	truedisp = qx_allocu(h,w);
	lower_resolution = qx_allocu((h+1)/change_size,(w+1)/change_size);
	//bilateral_image = qx_allocu(h, w);//allocate memory

	hl_SSD m_SSD;//SSD class
	m_SSD.init(h,w,max_disparity, window_size,change_size);//initialization

	qx_loadimage(filename_left_image,left[0][0],h,w);//load left image
	qx_loadimage(filename_right_image,right[0][0],h,w);//load right image
	//qx_loadimage(filename_true_disparity,truedisp[0],h,w);  //load true disparity image
	truedisp = loadimage_pgm_u(filename_true_disparity, h,w);

	Image_to_Lower_Resolution(lower_resolution,truedisp,h,w,change_size);  //get lower resolution disparity Image
	image_display(truedisp,h,w);
	image_display(lower_resolution,h/change_size,w/change_size); 

	unsigned char***bilateral_left, ***bilateral_right;
	bilateral_left = qx_allocu_3(h,w,3);
	bilateral_right = qx_allocu_3(h,w,3);
	if(flag)
	{
		bilateral_RGB(bilateral_left, left, h, w, window_size);
		bilateral_RGB(bilateral_right, right, h, w, window_size);

		m_SSD.matching_cost(bilateral_left,bilateral_right,truedisp);//compute matching cost
	}
	else
	{
		m_SSD.matching_cost(left,right,truedisp);//compute matching cost
	}
	m_SSD.disparity(disparity);//compute left disparity map
	//image_display(disparity, h, w);

	//int scaler = 255 / (max_disparity - 1);
	//for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) disparity[y][x] *= scaler;

	//Image_to_Gray(disparity,left, h,w);
	
	//bilateral(bilateral_image,disparity,h,w,window_size);
	//bilateral_RGB(bilateral_rgb, left, h, w, window_size);
	//Transfor(bilateral_image,h,w);
	//image_display(bilateral_image,h,w);
	//Transfor(disparity,h,w);
	//image_display(bilateral_rgb,h,w);
	image_display(disparity,h,w);
	getchar();


	qx_freeu_3(left); left=NULL;//free memory
	qx_freeu_3(right); right=NULL;//free memory
	qx_freeu_3(bilateral_rgb);bilateral_rgb = NULL;
	qx_freeu_3(bilateral_left);bilateral_left = NULL;
	qx_freeu_3(bilateral_right);bilateral_right = NULL;
	qx_freeu(disparity); disparity=NULL;//free memory
	qx_freeu(truedisp);truedisp = NULL;
	qx_freeu(lower_resolution);lower_resolution=NULL;
	//qx_freeu(bilateral_image);bilateral_image=NULL;
}
int main(int argc, char*argv[])
{
	/*char *filename_left_image = "database/tsukuba_left.ppm";
	char *filename_right_image = "database/tsukuba_right.ppm";
	char *filename_true_disparity = "database/truedisp.pgm";*/
	

	char *filename_left_image = "database/im2.ppm";
	char *filename_right_image = "database/im6.ppm";
	char *filename_true_disparity = "database/disp2.pgm";

	//int max_disparity = 16;
	int max_disparity = 8;
	bool flag = false;
	int window_size;//int window_size = 11;
	int change_size = 2;
	cout<<"Input the window_size and the scale:"<<endl;
	while (cin >> window_size>>change_size)
		stereo(filename_left_image, filename_right_image,filename_true_disparity, max_disparity, window_size,change_size);
	return(0);
}

