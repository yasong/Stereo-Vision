#include <stdlib.h>
#include <stdio.h>
#include "qx_basic.h"
#include "bilateral.h"
#include "qx_image_display.h"

//void Image_to_Gray(unsigned char **Image_out, unsigned char ***Image_in, int h, int w); // Transfor RGB image into Gray
//void bilateral(unsigned char **Image_out, unsigned char **Image_in,int h, int w,int window_size); //Bilateral Filtering for the gray image
//void bilateral_RGB(unsigned char ***Image_out, unsigned char ***Image_in,int h, int w,int window_size); //Bilateral Filtering for the RGB image
//void Transfor(unsigned char**disparity,int h, int w);
//void Image_to_Lower_Resolution(unsigned char **Image_out, unsigned char **Image_in, int h, int w,int size = 2);

void Transfor(unsigned char**disparity,int h, int w)
{
	unsigned char **temp;
	temp = qx_allocu(h, w);//allocate memory
	int i = 0, j = 0;
	for (i = 0; i< h; ++i)
	{
		for (j= 0;j<w;++j)
		{
			temp[i][j]=disparity[h-1-i][w-1-j];
		}
	}
	for (i = 0; i< h; ++i)
	{
		for (j= 0;j<w;++j)
		{
			disparity[i][j] = temp [i][w-1-j];
		}
	}
	qx_freeu(temp); temp=NULL;//free memory
}

void Image_to_Gray(unsigned char **Image_out, unsigned char ***Image_in, int h, int w)
{
	int i, j;
	float wr =0.3, wg = 0.59, wb = 0.11;

	for( i = 0;i < h; ++i)
	{
		for (j = 0;j < w; ++j)
		{
			Image_out[i][j] = wr * Image_in[i][j][0] + wg * Image_in[i][j][1] + wb * Image_in[i][j][2];
		}
	}

}
void bilateral(unsigned char **Image_out, unsigned char **Image_in,int h, int w,int window_size)
{
	int i, j;
	int sigma_d = window_size, sigma_r = 2 * window_size;
	float factor = -0.5;
	int filter_size = 2 * window_size + 1;
	double **spatial_weight_Table;
	double color_weight_Table[256];


	spatial_weight_Table = qx_allocd(filter_size, filter_size);

	double gaussian_d = factor / (sigma_d * sigma_d);
	double gaussian_r = factor / (sigma_r * sigma_r);
	
	//Compute spatial weight table
	for(i = -window_size; i < window_size; ++i)
	{
		for(j = -window_size; j < window_size; ++j)
		{
			spatial_weight_Table[i + window_size][j + window_size] = exp((i * i + j * j) * gaussian_d);
		}
	}

	//Compute similarity weight table
	for(i = 0; i < 256; ++i)
	{
		color_weight_Table[i] = exp((i * i) * gaussian_r);
	}

	//Bilateral filtering for the gray image
	int y_offset, x_offset;
	double pixel_sum = 0, weight_sum = 0;
	int pixel_v;
	int wy, wx;
	double weight_tmp;
	for(i = 0; i < h; ++i)
	{
		for(j = 0; j < w; ++j)
		{
			double pixel_sum = 0, weight_sum = 0;
			for(y_offset = -window_size; y_offset < window_size; ++y_offset)
			{
				wy = i + y_offset;
				for(x_offset = -window_size; x_offset < window_size; ++x_offset)
				{
					wx = j + x_offset;
					//Check for the edge
					if(0 <= wy && wy < h && 0 <= wx && wx < w)
					{
						pixel_v = abs(Image_in[wy][wx] - Image_in[i][j]);
						weight_tmp = spatial_weight_Table[y_offset + window_size][x_offset + window_size] * color_weight_Table[pixel_v];
						
						pixel_sum = pixel_sum + Image_in[wy][wx] * weight_tmp;
						weight_sum = weight_sum + weight_tmp;
					}
				}

			}
			if(weight_sum != 0)
				{
					unsigned char bilateral_pixel = pixel_sum / weight_sum;
					Image_out[i][j] = bilateral_pixel;
			}
		}
	}
	qx_freed(spatial_weight_Table);spatial_weight_Table=NULL;

}

void bilateral_RGB(unsigned char ***Image_out, unsigned char ***Image_in,int h, int w,int window_size)
{
	int i, j;
	int sigma_d = 5, sigma_r = 30;
	float factor = -0.5;
	int filter_size = 2 * window_size + 1;
	double **spatial_weight_Table;
	double color_weight_Table[256];


	spatial_weight_Table = qx_allocd(filter_size, filter_size);

	double gaussian_d = factor / (sigma_d * sigma_d);
	double gaussian_r = factor / (sigma_r * sigma_r);

	//Compute spatial weight table
	for(i = -window_size; i < window_size; ++i)
	{
		for(j = -window_size; j < window_size; ++j)
		{
			spatial_weight_Table[i + window_size][j + window_size] = exp((i * i + j * j) * gaussian_d);
		}
	}

	//Compute similarity weight table
	for(i = 0; i < 256; ++i)
	{
		color_weight_Table[i] = exp((i * i) * gaussian_r);
	}

	//Bilateral filtering for the gray image
	int y_offset, x_offset, r;
	double pixel_sum = 0, weight_sum = 0;
	int pixel_v;
	int wy, wx;
	double weight_tmp;
	for(r = 0; r < 3; ++r)
	for(i = 0; i < h; ++i)
	{
		for(j = 0; j < w; ++j)
		{
			double pixel_sum = 0, weight_sum = 0;
			for(y_offset = -window_size; y_offset < window_size; ++y_offset)
			{
				wy = i + y_offset;
				for(x_offset = -window_size; x_offset < window_size; ++x_offset)
				{
					wx = j + x_offset;
					//Check for the edge
					if(0 <= wy && wy < h && 0 <= wx && wx < w)
					{
						pixel_v = abs(Image_in[wy][wx][r] - Image_in[i][j][r]);
						weight_tmp = spatial_weight_Table[y_offset + window_size][x_offset + window_size] * color_weight_Table[pixel_v];

						pixel_sum = pixel_sum + Image_in[wy][wx][r] * weight_tmp;
						weight_sum = weight_sum + weight_tmp;
					}
				}

			}
			if(weight_sum != 0)
			{
				unsigned char bilateral_pixel = pixel_sum / weight_sum;
				Image_out[i][j][r] = bilateral_pixel;
			}
		}
	}
	image_display(Image_out,h,w);
	qx_freed(spatial_weight_Table);spatial_weight_Table=NULL;
}

void Image_to_Lower_Resolution(unsigned char **Image_out, unsigned char **Image_in, int h, int w,int size)
{
	int i = 0, j = 0, m =0,n=0;
	for(j = 0; j < h; j = j + size)
	{
		for( i = 0,n=0; i < w; i = i + size)
		{
			Image_out[m][n] = Image_in[j][i];
			++n;
		}
		++m;
	}
}

void Compute_Possible_Disparity(unsigned char ***Disparity_Table, unsigned char **Image_in, int h, int w, int change_size)
{
	// h:Image_in height, w: Image_in weight, change_size: true_disparity ----> low_resolution image
	//Disparity_Table size h*change_size * w*change_size * 8
	//now assume the Image_in is the true_disparity
	//int pre_h = h * change_size;
	//int pre_w = w * change_size;
	//change_size = 2;
	int i = 0, j = 0, k = 0,m =0, n =0;
	int x1,y1,x2,y2,x3,y3;
	double a,b,c,d;

	for(j = 0; j < h; j = j + change_size) //edge condition should be considered
		for (i = 0; i < w; i = i + change_size)
		{
			//p1(i,j)
			//p2(i+change_size,j)
			//p3(i,j+change_size)
			//p4(i+change_size,j+change_size)

			// a = (y2 -y1)*(z3-z1)-(y3-y1)*(z2-z1);
			// b = (z2-z1)*(x3-x1)-(z3-z1)*(x2-x1);
			// c = (x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
			// d =a*x1 -b*y1 - c*z1;
			// ax+by+cz+d =0

			Disparity_Table[j][i][0] = Image_in[j][i];
			Disparity_Table[j][i][1] =  255;
			if((i + change_size) < w && (j+change_size) < h && i != w -1 && j != h-1 )
			{
				for(m=0; m < change_size;++m)
					for(n = 0; n < change_size; ++n)
					{
						if( m ==0 && n==0 ) n=1;
						Disparity_Table[j+m][i+n][0] = Image_in[j][i];
						Disparity_Table[j+m][i+n][1] = Image_in[j][i+change_size];
						Disparity_Table[j+m][i+n][2] = Image_in[j+change_size][i];
						Disparity_Table[j+m][i+n][3] = Image_in[j+change_size][i+change_size];

						//p1, p2, p3
						a = -change_size * (Image_in[j][i+change_size] - Image_in[j][i]);
						b = -change_size * (Image_in[j+change_size][i] - Image_in[j][i]);
						c = change_size * change_size;
						d = a * i -b * j - c* Image_in[j][i];


						Disparity_Table[j+m][i+n][4] = - (a *(i+n)+ b *(j+m)+d)/c;

						//p1, p2, p4
						a = -change_size * (Image_in[j][i+change_size] - Image_in[j][i]);
						b = (Image_in[j][i+change_size] - Image_in[j][i]) * change_size - (Image_in[j+change_size][i+change_size] - Image_in[j][i])*change_size;
						c = change_size * change_size;
						d = a * i -b * j - c* Image_in[j][i];
						Disparity_Table[j+m][i+n][5] = - (a *(i+n)+ b *(j+m)+d)/c;

						//p1, p3, p4
						a = change_size * (Image_in[j+change_size][i+change_size] - Image_in[j][i]) - change_size * (Image_in[j][i+change_size] - Image_in[j][i]);
						b = (Image_in[j][i+change_size] - Image_in[j][i]) * change_size;
						c = - change_size * change_size;
						d = a * i -b * j - c* Image_in[j][i];
						Disparity_Table[j+m][i+n][6] = - (a *(i+n)+ b *(j+m)+d)/c;

						//p2, p3, p4
						a = change_size * (Image_in[j+change_size][i+change_size] - Image_in[j][i+change_size]) - change_size * (Image_in[j+change_size][i] - Image_in[j][i+change_size]);
						b = change_size * (Image_in[j+change_size][i+ change_size] - Image_in[j][i+change_size]);
						c = -change_size * change_size;
						d = a * (i+change_size) -b * j - c* Image_in[j+change_size][i];
						Disparity_Table[j+m][i+n][7] = - (a *(i+n)+ b *(j+m)+d)/c;

					}
			}
			else if(i == w - 1)
			{
				for (m = 1; m< change_size;++m)
				{
					Disparity_Table[j+m][i][0] = Image_in[j][i];
					Disparity_Table[j+m][i][1] =255;
				}
			}
			else if(j == h - 1)
			{
				for (m = 1; m< change_size;++m)
				{
					Disparity_Table[j][i+m][0] = Image_in[j][i];
					Disparity_Table[j][i+m][1] = 255;
				}
			}

		}
}
