#include "ConnectedComponentLabeling.h"

void gray_expand2_binary(const Mat &grayImg, Mat &binaryImg, const int bin_th)
{
	int i = 0, j = 0;
	int i_gray = 0, j_gray = 0;
	int i_binary = 0, j_binary = 0;
	int gray_row = grayImg.rows;
	int gray_col = grayImg.cols;
	int tmp_crt_row_gray = 0;
	int tmp_crt_row_bi = 0;
	int threshold[256] = { 0 };
	
	for (i = 255; i >= bin_th; i--)
	{
		threshold[i] = 1;
	}

	int j_quotient = 0;
	int j_remainder = 0;
	for (i_gray = gray_row - 1, i_binary = gray_row; i_gray >= 0; i_gray--, i_binary--)
	{
		tmp_crt_row_bi = i_binary * binaryImg.cols;
		tmp_crt_row_gray = i_gray * gray_col;
		j_quotient = grayImg.cols >> 2;
		j_remainder = grayImg.cols - (j_quotient << 2);
		for (j_gray = gray_col - 1, j_binary = gray_col; j_quotient >= 0; j_quotient--)
		{
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
		}
		for (; j_remainder >= 0; j_remainder--)
		{
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
		}
	}

	//
	i = (binaryImg.rows - 1) * binaryImg.cols;
	for (j = binaryImg.cols - 1; j >= 0; j--)
	{
		binaryImg.data[j] = 0;
		binaryImg.data[i + j] = 0;
	}

	//
	j = binaryImg.cols - 1;
	for (i = binaryImg.rows - 1; i >= 0; i--)
	{
		tmp_crt_row_bi = i * binaryImg.cols;
		binaryImg.data[tmp_crt_row_bi] = 0;
		binaryImg.data[tmp_crt_row_bi + j] = 0;
	}
}


void label_tmp_shrink2_label(const Mat &labelImg_tmp, Mat &labelImg)
{
	int i = 0, j = 0;
	int i_tmpIm = 0, j_tmpIm = 0;
	int row = labelImg.rows;
	int col = labelImg.cols;
	int row_tmpIm = labelImg_tmp.rows;
	int col_tmpIm = labelImg_tmp.cols;
	int mark = 0;
	int crt_rowhead = 0;
	int crt_rowhead_tmpIm = 0;

	for (i = row - 1, i_tmpIm = row; i >= 0; i--, i_tmpIm--)
	{
		crt_rowhead = i * col;
		crt_rowhead_tmpIm = (i + 1) * col_tmpIm;
		for (j = col - 1; j >= 0; j--)
		{
			mark = ((int*)labelImg_tmp.data)[crt_rowhead_tmpIm + j + 1];
			((int*)labelImg.data)[crt_rowhead + j] = mark;
		}
	}
}



int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& labelImg, int bin_th)
{
	if (nullptr == grayImg.data || nullptr == labelImg.data)
	{
		std::cout << "exist null image!\n";
		return MY_FAIL;
	}
	if (bin_th < 0 || bin_th > 255)
	{
		std::cout << "bin_th < 0  or bin_th > 255!\n";
		return MY_FAIL;
	}
	if (grayImg.rows != labelImg.rows || grayImg.cols != labelImg.cols)
	{
		std::cout << "sizes of grayImg and labelImg don't match!\n";
		return MY_FAIL;
	}

	Mat binaryImg(grayImg.rows + 2, grayImg.cols + 2, CV_8UC1);//
	Mat labelImg_temp(grayImg.rows + 2, grayImg.cols + 2, CV_32SC1);//
	int *data_label = (int*)labelImg_temp.data;
	gray_expand2_binary(grayImg, binaryImg, bin_th);//

	int i_local = 0;
	int i_row = 0, j_col = 0;
	int row_scan = binaryImg.rows - 1;
	int col_scan = binaryImg.cols - 1;
	int bi_row = binaryImg.rows;
	int bi_col = binaryImg.cols;
	
	//********************************** 1_st time scan **********************************
	//scanner
	//bcd
	//a
	int length = binaryImg.rows * binaryImg.cols / 4;//
	int *mark = new int[length];
	int *link = new int[length];
	memset(mark, 0, length * sizeof(int));
	memset(link, 0, length * sizeof(int));
	int max_mark = 0;
	int scanner = 0;
	int b = 0, c = 0, d = 0;
	int a = 0, e = 0;

	int b_index = 0, c_index = 0, d_index = 0;
	int a_index = 0, e_index = 0;
	int b_mark = 0, c_mark = 0, d_mark = 0;
	int a_mark = 0, e_mark = 0;

	int link_min = 0, link_max = 0, link_tmp = 0;

	int tmp_curt_row = 0;
	int tmp_pre_row = 0;
	for (i_row = 1; i_row < row_scan; i_row++)
	{
		tmp_curt_row = i_row * binaryImg.cols;
		tmp_pre_row = (i_row - 1) * binaryImg.cols;
		for (j_col = 1; j_col < col_scan; j_col++)
		{
			e_index = tmp_curt_row + j_col;
			e = binaryImg.data[e_index];
			if (!e) //background
			{
				data_label[e_index] = 0;
			}
			else  //forthground
			{
				a_index = tmp_curt_row + j_col - 1;
				b_index = tmp_pre_row + j_col - 1;
				c_index = tmp_pre_row + j_col;
				d_index = tmp_pre_row + j_col + 1;

				a = binaryImg.data[a_index];
				b = binaryImg.data[b_index];
				c = binaryImg.data[c_index];
				d = binaryImg.data[d_index];
				scanner = (a << 3) + (b << 2) + (c << 1) + d;
				switch (scanner)
				{
				case 0://0000
					max_mark++;
					data_label[e_index] = max_mark;
					mark[max_mark] = max_mark;
					link[max_mark] = max_mark;
					break;
				case 1://0001  d   //e = d
					d_mark = data_label[d_index];
					data_label[e_index] = d_mark;
					break;
				case 2://0010  c   //e = c
					c_mark = data_label[c_index];
					data_label[e_index] = c_mark;
					break;
				case 3://0011  cd  d <= c   //e = d
					d_mark = data_label[d_index];
					data_label[e_index] = d_mark;
					break;
				case 4://0100  b   //e = b
					b_mark = data_label[b_index];
					data_label[e_index] = b_mark;
					break;
				case 5://0101  bd   //connect b & d
					b_mark = data_label[b_index];
					d_mark = data_label[d_index];
					if (b_mark == d_mark)
					{
						data_label[e_index] = b_mark;
					}
					else //b_mark != d_mark
					{
						if (b_mark < d_mark)
						{
							data_label[e_index] = b_mark;
						}
						else
						{
							data_label[e_index] = d_mark;
						}
						//connect b_link & d_link
						link_min = link[b_mark];
						link_max = link[d_mark];
						if (link_min == link_max)
						{
							//nothing to do
						}
						else if (link_max > link_min)
						{
							link[link_max] = link[link_min];
							for (i_local = link_max + 1; i_local <= max_mark; i_local++)
							{
								link[i_local] = link[link[i_local]];
							}
						}
						else //link_max < link_min 
						{
							link_tmp = link_min;
							link_min = link_max;
							link_max = link_tmp;
							link[link_max] = link[link_min];
							for (i_local = link_max + 1; i_local <= max_mark; i_local++)
							{
								link[i_local] = link[link[i_local]];
							}
						}
					}
					break;
				case 6://0110  bc  c <= b   //e = c
					c_mark = data_label[c_index];
					data_label[e_index] = c_mark;
					break;
				case 7://0111  bcd  d <= c <= b   //e = d
					d_mark = data_label[d_index];
					data_label[e_index] = d_mark;
					break;
				case 8://1000  a   //e = a
					a_mark = data_label[a_index];
					data_label[e_index] = a_mark;
					break;
				case 9://1001  ad  connect a & d
					a_mark = data_label[a_index];
					d_mark = data_label[d_index];
					if (a_mark == d_mark)
					{
						data_label[e_index] = a_mark;
					}
					else
					{
						if (a_mark < d_mark)
						{
							data_label[e_index] = a_mark;
						}
						else
						{
							data_label[e_index] = d_mark;
						}
						link_min = link[a_mark];
						link_max = link[d_mark];
						if (link_min == link_max)
						{
							//nothing to do
						}
						else if (link_max > link_min)
						{
							link[link_max] = link[link_min];
							for (i_local = link_max + 1; i_local <= max_mark; i_local++)
							{
								link[i_local] = link[link[i_local]];
							}
						}
						else //link_max < link_min
						{
							link_tmp = link_min;
							link_min = link_max;
							link_max = link_tmp;
							link[link_max] = link[link_min];
							for (i_local = link_max + 1; i_local <= max_mark; i_local++)
							{
								link[i_local] = link[link[i_local]];
							}
						}
					}
					break;
				case 10://1010  ac  a <= c   //e = a
					a_mark = data_label[a_index];
					data_label[e_index] = a_mark;
					break;
				case 11://1011  acd  a <= c, d <= c
					a_mark = data_label[a_index];
					d_mark = data_label[d_index];
					if (a_mark <= d_mark)
					{
						data_label[e_index] = a_mark;
					}
					else
					{
						data_label[e_index] = d_mark;
					}
					break;
				case 12://1100  ab  a <= b   //e = a
					a_mark = data_label[a_index];
					data_label[e_index] = a_mark;
					break;
				case 13://1101  abd  //a <= b   connect a & d
					a_mark = data_label[a_index];
					d_mark = data_label[d_index];
					if (a_mark == d_mark)
					{
						data_label[e_index] = a_mark;
					}
					else
					{
						if (a_mark < d_mark)
						{
							data_label[e_index] = a_mark;
						}
						else
						{
							data_label[e_index] = d_mark;
						}
						link_min = link[a_mark];
						link_max = link[d_mark];
						if (link_min == link_max)
						{
							//nothing to do
						}
						else if (link_max > link_min)
						{
							link[link_max] = link[link_min];
							for (i_local = link_max + 1; i_local <= max_mark; i_local++)
							{
								link[i_local] = link[link[i_local]];
							}
						}
						else //link_max < link_min
						{
							link_tmp = link_min;
							link_min = link_max;
							link_max = link_tmp;
							link[link_max] = link[link_min];
							for (i_local = link_max + 1; i_local <= max_mark; i_local++)
							{
								link[i_local] = link[link[i_local]];
							}
						}
					}
					break;
				case 14://1110  abc  a <= b, a <= c   //e = a
					a_mark = data_label[a_index];
					data_label[e_index] = a_mark;
					break;
				case 15://1111  abcd  //compare a d
					a_mark = data_label[a_index];
					d_mark = data_label[d_index];
					if (a_mark <= d_mark)
					{
						data_label[e_index] = a_mark;
					}
					else
					{
						data_label[e_index] = d_mark;
					}
					break;
				}
			}
			
		}
	}
	//*********************************** end of 1_st time scan ******************************

	//*********************************** 2_nd time scan *************************************
	for (i_row = row_scan - 1; i_row > 0; i_row--)
	{
		tmp_curt_row = i_row * binaryImg.cols;
		for (j_col = col_scan - 1; j_col > 0; j_col--)
		{
			e_index = tmp_curt_row + j_col;
			e_mark = data_label[e_index];
			data_label[e_index] = link[e_mark];
		}
	}
	//********************************** end of 2_nd time scan *******************************

	//shrink and save
	label_tmp_shrink2_label(labelImg_temp, labelImg);

	delete[] mark;
	delete[] link;
	return MY_OK;
}
