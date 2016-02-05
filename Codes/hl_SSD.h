#ifndef HL_SSD_H
#define HL_SSD_H

class hl_SSD
{
public:
    hl_SSD();
    ~hl_SSD();

    void clean();
	int init(int h, int w, int nr_plane, int window_size, int change_size);
    int matching_cost(unsigned char ***left, unsigned char ***right,unsigned char **disp);
    int disparity(unsigned char **disparity);

	void matching_cost_from_color(unsigned char ***left,unsigned char ***right);
	void compute_support_weight(double ***support_weight_table, int Max_diff_Color,int segma_c, int segma_p,int window_size);  //w(p,q)
private:
	int	m_h,m_w,m_nr_plane,m_change_size; 

	double ***m_raw_pixel_matching_cost;
	double ***m_aggregated_window_matching_cost;
	unsigned char ***m_left, ***m_right;
	unsigned char **m_disparity;
	unsigned char **true_disparity;
	int m_window_radius;
};
#endif
