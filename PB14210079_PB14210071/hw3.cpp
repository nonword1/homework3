#include “ConnectedComponentLabeling.h”
int ustc_ConnectedComponentLabeling( Mat grayImg, Mat& labelImg, int bin_th)
{
	if (NULL == grayImg.data || 1 != grayImg.channels())
	{
		cout << "invalid image input" << endl;
		return MY_FIND_FAIL;
	}
	int cols = grayImg.cols;
	int rows = grayImg.rows;
	

	//connection map
	Mat connection_map(rows, cols, CV_8UC1);
	grayImg.copyTo(connection_map);
	//mask
	int forward_x[5] = { -1,-1,0,1,0 };
	int forward_y[5] = { 0,-1,-1,-1,0 };
	int backward_x[5] = { 1,1,0,-1,0 };
	int backward_y[5] = { 0,1,1,1,0 };
	
	int m = 1;//incrementing index
	// pre-scan approximate the size of m, proposed by Yuhang
	for (int j = 0; j< rows; j++)
		for (int k = 0; k < cols; k++)
		{
			int cord = j*cols + k;
			int pix_value = connection_map.data[cord];
			if (pix_value <= bin_th)
			{
				int min_neighbour = bin_th + 1;
				int foreground_neighbour = 0;
				for (int i = 0; i < 4; i++)
				{
					int j_neighbour = j + forward_y[i];
					int k_neighbour = k + forward_x[i];
					if (k_neighbour >=0 && k_neighbour < cols && j_neighbour>=0 && j_neighbour < rows)
					{
						int cord_neighbour = j_neighbour*cols + k_neighbour;
						if (min_neighbour > connection_map.data[cord_neighbour])
						{
							foreground_neighbour = 1;
							break;
						}
					}
				
				}
				if (!foreground_neighbour)
					m++;

			}
		}
	int* T = new int[m+1];//connection table
	for (int i = 0; i < m+1; i++)
	{
		T[i] = i;
	}
	m = 1;
	for(int j = 0; j< rows; j++)
		for (int k = 0; k < cols; k++)
		{
			int cord = j*cols + k;
			int pix_value = connection_map.data[cord];
			if (pix_value <= bin_th)
			{
				int min_neighbour = bin_th + 1;
				int j_neighbour_min = j;
				int k_neighbour_min = k;
				int cord_neighbour_min = cord;
				for (int i = 0; i < 4; i++)
				{
					int j_neighbour = j + forward_y[i];
					int k_neighbour = k + forward_x[i];
					if (k_neighbour >=0 && k_neighbour < cols && j_neighbour>=0 && j_neighbour < rows)
					{
						int cord_neighbour = j_neighbour*cols + k_neighbour;
						if (connection_map.data[cord_neighbour]<= bin_th && min_neighbour > T[connection_map.data[cord_neighbour]])
						{
							min_neighbour = T[connection_map.data[cord_neighbour]];
							j_neighbour_min = j_neighbour;
							k_neighbour_min = k_neighbour;
							cord_neighbour_min = cord;
						}
					}
				
				}
				if (min_neighbour <= bin_th)
				{
					connection_map.data[cord] = min_neighbour;
					for (int i = 0; i < 4; i++)
					{
						int j_neighbour = j + forward_y[i];
						int k_neighbour = k + forward_x[i];
						if (k_neighbour >= 0 && k_neighbour < cols && j_neighbour>=0 && j_neighbour < rows)
						{
							int cord_neighbour = j_neighbour*cols + k_neighbour;
							if(connection_map.data[cord_neighbour]<= bin_th)
							T[connection_map.data[cord_neighbour]] = min_neighbour;
						}

					}
				}
				else {
					connection_map.data[cord] = m;
					T[m] = m;
					m++;
				}

			}
			
		}
	int final_flag = 0;
	while (!final_flag)
	{
		final_flag = 1;
		//backward
		for (int j = rows-1; j>=0; j--)
			for (int k = cols-1; k>=0; k--)
			{
				int cord = j*cols + k;
				int pix_value = connection_map.data[cord];
				if (pix_value <= bin_th)
				{
					int min_neighbour = bin_th +1;
					for (int i = 0; i < 5; i++)
					{
						int j_neighbour = j + backward_y[i];
						int k_neighbour = k + backward_x[i];
						if (k_neighbour >= 0 && k_neighbour < cols && j_neighbour>=0 && j_neighbour < rows)
						{
							int cord_neighbour = j_neighbour*cols + k_neighbour;
							if (connection_map.data[cord_neighbour] <= bin_th && min_neighbour > T[connection_map.data[cord_neighbour]])
							{
								min_neighbour = T[connection_map.data[cord_neighbour]];
							}
						}

					}
					for (int i = 0; i < 5; i++)
					{
						int j_neighbour = j + backward_y[i];
						int k_neighbour = k + backward_x[i];
						if (k_neighbour >= 0 && k_neighbour < cols && j_neighbour>=0 && j_neighbour < rows)
						{
							int cord_neighbour = j_neighbour*cols + k_neighbour;
							if (connection_map.data[cord_neighbour] <= bin_th && T[connection_map.data[cord_neighbour]] != min_neighbour)
							{
								T[connection_map.data[cord_neighbour]] = min_neighbour;
								final_flag = 0;

							}
						}
					}

				}
			}
		//forward
		for (int j = 0; j <rows; j++)
			for (int k = 0; k <cols; k++)
			{
				int cord = j*cols + k;
				int pix_value = connection_map.data[cord];
				if (pix_value <= bin_th)
				{
					int min_neighbour = bin_th +1;
					for (int i = 0; i < 5; i++)
					{
						int j_neighbour = j + forward_y[i];
						int k_neighbour = k + forward_x[i];
						if (k_neighbour >= 0 && k_neighbour < cols && j_neighbour>=0 && j_neighbour < rows)
						{
							int cord_neighbour = j_neighbour*cols + k_neighbour;
							if (connection_map.data[cord_neighbour] <= bin_th && min_neighbour > T[connection_map.data[cord_neighbour]])
							{
								min_neighbour = T[connection_map.data[cord_neighbour]];
							}
						}

					}
					//connection_map.data[cord] = min_neighbour;
					for (int i = 0; i < 5; i++)
					{
						int j_neighbour = j + forward_y[i];
						int k_neighbour = k + forward_x[i];
						if (k_neighbour >= 0 && k_neighbour < cols && j_neighbour>=0 && j_neighbour < rows)
						{
							int cord_neighbour = j_neighbour*cols + k_neighbour;
							if (connection_map.data[cord_neighbour] <= bin_th && T[connection_map.data[cord_neighbour]] != min_neighbour)
							{
								T[connection_map.data[cord_neighbour]] = min_neighbour;
								final_flag = 0;

							}
						}
					}

				}
			}
	}
  for (int j = 0; j <rows; j++)
		for (int k = 0; k < cols; k++)
		{
			int cord = j*cols + k;
			if (connection_map.data[cord] <= bin_th)
				connection_map.data[cord] = T[connection_map.data[cord]];
		}
	connection_map.copyTo(labelImg);

	return MY_FIND_OK;

}
