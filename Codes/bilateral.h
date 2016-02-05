#ifndef BILATERAL_H
#define BILATERAL_H

void Image_to_Gray(unsigned char **Image_out, unsigned char ***Image_in, int h, int w);
void bilateral(unsigned char **Image_out, unsigned char **Image_in,int h, int w,int window_size);
void bilateral_RGB(unsigned char ***Image_out, unsigned char ***Image_in,int h, int w,int window_size);
void Transfor(unsigned char**disparity,int h, int w);

void Image_to_Lower_Resolution(unsigned char **Image_out, unsigned char **Image_in, int h, int w,int size = 2);

void Compute_Possible_Disparity(unsigned char ***Disparity_Table, unsigned char **Image_in, int h, int w, int change_size);

#endif
