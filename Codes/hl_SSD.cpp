#include "qx_basic.h"
#include "hl_SSD.h"
#include "qx_image_display.h"
#include "bilateral.h"

hl_SSD::hl_SSD()
{
	m_disparity = NULL;
}
hl_SSD::~hl_SSD()
{
    clean();
}
void hl_SSD::clean()
{
	qx_freeu(m_disparity); m_disparity = NULL;
}
int hl_SSD::init(int h, int w, int nr_plane, int window_size,int change_size)
{
	clean();
	m_h = h; m_w = w; m_nr_plane = nr_plane; m_change_size = change_size;
	m_window_radius = (window_size >> 1);

	m_raw_pixel_matching_cost = qx_allocd_3(m_h, m_w, m_nr_plane);
	m_aggregated_window_matching_cost = qx_allocd_3(m_h, m_w, m_nr_plane);

	m_disparity = qx_allocu(m_h, m_w);
	//true_disparity = qx_allocu(m_h,m_w);

    return(0);
}
int hl_SSD::matching_cost(unsigned char***left,unsigned char***right, unsigned char **disp)
{
	m_left = left; m_right = right; true_disparity=disp;
	//matching_cost_from_color(left, right);
    return(0);
}
int hl_SSD::disparity(unsigned char**disparity)
{
	image_zero(m_aggregated_window_matching_cost, m_h, m_w, m_nr_plane);

	double ***support_weight_table;
	unsigned char ***Disparity_Table;
	int d = 0; //disparity size

	Disparity_Table = qx_allocu_3(m_h,m_w,8);

	Compute_Possible_Disparity(Disparity_Table,true_disparity,m_h,m_w,m_change_size);
	int filter_size = 2 * m_window_radius + 1;
	int Max_diff_Color = 442; //delta E Euclidean distance
	support_weight_table = qx_allocd_3(Max_diff_Color,filter_size,filter_size);
	int segma_c,segma_p;//

	segma_c = 25;       ///////////////////////////////////////
	segma_p = 40;
	compute_support_weight(support_weight_table,Max_diff_Color,segma_c,segma_p,m_window_radius);

	int T = 40;

	unsigned char *pixel_left_p, *pixel_right_q,*pixel_left_pd, *pixel_right_qd;
	double wp_q, wp_qd, ep_qd;
	double sum_w_e, sum_w;
	for (int y = 0; y < m_h; y++)
		for (int x = 0; x < m_w; x++)
			//for (int d = 0; d < m_nr_plane; d++)
			for (int i = 0; i < 8; i++)
			{
				d = Disparity_Table[y][x][i]/4;  // cones 4  255/4 = 63;tuskub 17 255/17=15
				if (d == 63)
				{
					m_aggregated_window_matching_cost[y][x][i] = 3*QX_DEF_CHAR_MAX*QX_DEF_CHAR_MAX;
					for (int j = i+1; j< 8;++j) 
						m_aggregated_window_matching_cost[y][x][j] = 3*QX_DEF_CHAR_MAX*QX_DEF_CHAR_MAX;
					break;
				}
				sum_w = 0;
				sum_w_e = 0;
				pixel_left_p = m_left[y][x];
				if(x- d < 0 ) pixel_left_pd = m_right[y][d-x];
				else pixel_left_pd = m_right[y][x-d];
				for (int y_offset = -m_window_radius; y_offset <= m_window_radius; y_offset++)
				{
					int wy = y + y_offset;
					for (int x_offset = -m_window_radius; x_offset <= m_window_radius; x_offset++)
					{
						int wx = x + x_offset;

						if (0 <= wy && wy < m_h && 0 <= wx && wx < m_w){
							pixel_right_q = m_left[wy][wx];
							if (wx - d < 0) break;
							pixel_right_qd = m_right[wy][wx-d];
							wp_q = support_weight_table[qx_square_difference_u3(pixel_left_p,pixel_right_q)][m_window_radius + y_offset][m_window_radius + x_offset];
							wp_qd = support_weight_table[qx_square_difference_u3(pixel_left_pd,pixel_right_qd)][m_window_radius + y_offset][m_window_radius + x_offset];
							ep_qd = min(qx_difference_u3(pixel_right_q,pixel_right_qd),T);
							//cost += m_raw_pixel_matching_cost[wy][wx][d];

							sum_w_e = sum_w_e + wp_q * wp_qd * ep_qd;
							sum_w = sum_w + wp_q * wp_qd;
						}
					}
				}
				m_aggregated_window_matching_cost[y][x][i] = sum_w_e /sum_w;
			}

	depth_best_cost(disparity, m_aggregated_window_matching_cost, m_h, m_w, m_nr_plane);

	for (int j = 0 ; j < m_h; j++)
		for (int i = 0; i < m_w; i++)
		{
			int temp = disparity[j][i];
			disparity[j][i] = Disparity_Table[j][i][temp];
		}

    return(0);
}

void hl_SSD::matching_cost_from_color(unsigned char ***left,unsigned char ***right)
{
	image_zero(m_raw_pixel_matching_cost, m_h, m_w, m_nr_plane);

	int x_offset; unsigned char *pixel_left, *pixel_right;
	for (int y = 0; y < m_h; y++)
	for (int x = 0; x < m_w; x++)
	{
		pixel_left = left[y][x];
		for (int d = 0; d < m_nr_plane; d++)
		{
			x_offset = x - d;
			if (x_offset > 0){
				pixel_right = right[y][x_offset];
				m_raw_pixel_matching_cost[y][x][d] = qx_square_difference_u3(pixel_left, pixel_right);
			}
			else{
				m_raw_pixel_matching_cost[y][x][d] = 3*QX_DEF_CHAR_MAX*QX_DEF_CHAR_MAX;
			}
		}
	}
}

void hl_SSD::compute_support_weight(double ***support_weight_table, int Max_diff_Color,int segma_c, int segma_p, int window_size)
{

	int i, j, k;
	double support_weight;
	for (i = 0; i < Max_diff_Color; ++i)
	{
		for(j = -window_size; j <= window_size; ++j)
		{
			for(k = -window_size; k <= window_size; ++k)
			{
				support_weight = exp (-((i / segma_c) + sqrt(j * j + k * k)/ segma_p));		
				support_weight_table[i][j+window_size][k+window_size] = support_weight;
			}
		}
	}
}

