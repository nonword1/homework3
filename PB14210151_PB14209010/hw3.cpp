#include “ConnectedComponentLabeling.h”

//#include "opencv2/opencv.hpp"
//
//#define MY_OK 1
//#define MY_FAIL -1
//
////#define IMG_SHOW
////#define DEBUG
////#define COMPARE
//using namespace cv;
//using namespace std;

// thresholding processing, binarization of image 
int ustc_Threshold(
	Mat grayImg,
	Mat& binaryImg,
	int th)
{
	int height = grayImg.rows;
	int width = grayImg.cols;
	uchar* data = grayImg.data;
	uchar* Bdata = binaryImg.data;

	// binary value stored in COMP array 
	int low_value = 0;
	int high_value = 1;
	uchar* COMP = new uchar[256];
	int i = 0;
	for (; i < th; i++)
	{
		COMP[i] = low_value;
	}
	for (; i < 256; i++)
	{
		COMP[i] = high_value;
	}

	// traverse the image 
	for (int i_row = 0; i_row < height; i_row++)
	{
		int temp_width = width - 3;
		int j_col = 0;
		int mult = i_row * width;
		for (; j_col < temp_width; j_col += 4)
		{
			int indx = mult + j_col;
			Bdata[indx] = COMP[data[indx]];
			Bdata[indx + 1] = COMP[data[indx + 1]];
			Bdata[indx + 2] = COMP[data[indx + 2]];
			Bdata[indx + 3] = COMP[data[indx + 3]];
		}
		for (; j_col < width; j_col++)
		{
			int indx = mult + j_col;
			Bdata[indx] = COMP[data[indx]];
		}
	}

	return MY_OK;
}

//////////////////////////////////// by Zhixian Lin ////////////////////////////////////
int label_to_min(int* &label_connection_table, int label) {
	int label_tmp = label;
	int label_pre = label_connection_table[label];
	while (label_pre != 0) {
		label_tmp = label_pre;
		label_pre = label_connection_table[label_tmp];
	}
	return label_tmp;
}


// !!! the value in curr_pos should be changed to min_new before calling this function !!! 
void set_to_min(int* &label_connection_table, int curr_pos, int min_old, int min_new, int curr_max) {
	int i;
	//int min_new = label_connection_table[pos_start]; // if it hasn't changed yet
	for (i = curr_pos + 1; i <= curr_max; i++) {
		if (min_old == label_connection_table[i]) {
			label_connection_table[i] = min_new;
		}
	}
}

bool labelImg_update_round2(Mat labelImg, int* &label_connection_table) {
	int width = labelImg.cols;
	int height = labelImg.rows;
	int last_pixel = width * height - 1;
	int curr_pos = last_pixel;
	int label_final_tmp = 0;
	int label_original = 0;
	while (curr_pos >= 0) {
		label_original = ((int *)labelImg.data)[curr_pos];
		label_final_tmp = label_connection_table[label_original];
		// label_final_tmp == 0 indicates that label_original is actually the final label we want 
		if (label_final_tmp == 0) {
			label_final_tmp = label_original;
		}
		((int *)labelImg.data)[curr_pos] = label_final_tmp;
		curr_pos--;
	}
	return true;
}


// after this function,  () is needed 
bool MergeLabels(int* &label_connection_table, int label_low, int label_high, int curr_max) {
	// if there is no need to merge, becase they are the same label  
	if (label_low == label_high) {
		return false;
	}
	int label_tmp = 0;
	// defensive programming 
	if (label_low > label_high) {
		label_tmp = label_high;
		label_high = label_low;
		label_low = label_tmp;
	}

	int label_low_min = label_to_min(label_connection_table, label_low);
	int label_old = label_connection_table[label_high];
	// if there is no need to update
	if (label_old == label_low_min) {
		return false;
	}
	// labels following label_high may have connections with label_high, they should be updated to label_low_min too
	set_to_min(label_connection_table, label_high, label_high, label_low_min, curr_max);
	if (label_old == 0) {
		label_connection_table[label_high] = label_low_min;
	}
	else { // already has connection with another label, extra updates needed   
		if (label_old > label_low_min) {
			label_connection_table[label_high] = label_low_min;
			label_connection_table[label_old] = label_low_min;
			// labels following label_old may have connections with label_old, they should be updated to label_low_min too
			set_to_min(label_connection_table, label_old, label_old, label_low_min, curr_max);
		}
		else { // label_old <= label_low_min 
			label_connection_table[label_low_min] = label_old;
			// update previous label_low_min to label_old 
			set_to_min(label_connection_table, label_low_min, label_low_min, label_old, curr_max);
		}
	}
	return true;
}

//////////////////////////////////// 连通域检测 ////////////////////////////////////
// 功能说明: 找到二值图像中所有的连通域，并且进行标记 
// 返回值: MY_OK或者MY_FAIL 
int ustc_ConnectedComponentLabeling(
	Mat grayImg,        // grayImg: 用来进行连通域分析的目标图, 8UC1 
	Mat& labelImg,      // labelImg: 用来存储标记结果的图像, CV_32SC1, 尺寸与grayImg一致 
	int bin_th)         // bin_th: 用来对灰度图进行二值化的阈值 
{
	//////////////////////////////////// defensive programming ////////////////////////////////////
	// if image exists 
	if (NULL == grayImg.data || NULL == labelImg.data)
	{
		std::cout << "ERROR: image is NULL " << std::endl;
		return MY_FAIL;
	}

	// if legal threshold  
	if (bin_th <= 0 || bin_th >= 255)
	{
		std::cout << "ERROR: illegal threshold " << std::endl;
		return MY_FAIL;
	}

	int height = grayImg.rows;
	int width = grayImg.cols;
	int channel = grayImg.channels();
	int Lchannel = labelImg.channels();

	// if image size matches 
	if (height != labelImg.rows || width != labelImg.cols)
	{
		std::cout << "ERROR: image size does not match " << std::endl;
		return MY_FAIL;
	}

	// if legal channel 
	if (channel != 1 || Lchannel != 1)
	{
		std::cout << "ERROR: channel != 1 " << std::endl;
		return MY_FAIL;
	}

	///////////////////////////////// image binarization /////////////////////////////////
	Mat binaryImg(height, width, CV_8UC1);
	if (MY_FAIL == ustc_Threshold(grayImg, binaryImg, bin_th))
	{
		std::cout << "ERROR: ustc_Threshold failed " << std::endl;
		return MY_FAIL;
	}

	///////////////////////////////// edge extension /////////////////////////////////
	Mat Bin_Img(height + 2, width + 2, CV_8UC1);
	Mat Label_Img(height + 2, width + 2, CV_32SC1);
	copyMakeBorder(binaryImg, Bin_Img, 1, 1, 1, 1, BORDER_CONSTANT, 0);
	copyMakeBorder(labelImg, Label_Img, 1, 1, 1, 1, BORDER_CONSTANT, 0);
	Label_Img.setTo(0);

	///////////////////////////////// labelling round 1 /////////////////////////////////
	int len = width * height / 4;
	int* Label = new int[len];
	memset(Label, 0, sizeof(int) * len);
	int curr_max = 0;

	int width_plus = width + 2;
	int height_plus = height + 2;
	for (int i_row = 1; i_row <= height; i_row++)
	{
		int row_offset = i_row * width_plus;
		for (int j_col = 1; j_col <= width; j_col++)
		{
			int indx = row_offset + j_col;
			if (Bin_Img.data[indx])
			{
				int c1 = Bin_Img.data[indx - 1];
				int c2 = Bin_Img.data[indx - 1 - width_plus];
				int c3 = Bin_Img.data[indx - width_plus];
				int c4 = Bin_Img.data[indx - width_plus + 1];
				int result = c1 + (c2 << 1) + (c3 << 2) + (c4 << 3);

				int label_low = 0;
				int label_high = 0;
				switch (result)
				{
				// case 0: increase curr_max and set the pixel to label curr_max; no operation needed for label_connection_table
				case 0:
					curr_max++;
					((int *)(Label_Img.data))[indx] = curr_max;
					break;
				case 1:
				case 3:
				case 5:
				case 7:
				case 13:
				case 15:
					((int *)(Label_Img.data))[indx] = ((int *)(Label_Img.data))[indx - 1];
					break;
				case 2:
				case 6:
				case 14:
					((int *)(Label_Img.data))[indx] = ((int *)(Label_Img.data))[indx - 1 - width_plus];
					break;
				case 12:
				case 4:
					((int *)(Label_Img.data))[indx] = ((int *)(Label_Img.data))[indx - width_plus];
					break;
				case 8:
					((int *)(Label_Img.data))[indx] = ((int *)(Label_Img.data))[indx - width_plus + 1];
					break;

				case 9:
					label_low = ((int *)(Label_Img.data))[indx - 1];
					label_high = ((int *)(Label_Img.data))[indx - width_plus + 1];
#ifdef COMPARE
					if (label_low > label_high)
					{
						int temp = label_low;
						label_low = label_high;
						label_high = temp;
					}
#endif   
					
					((int *)(Label_Img.data))[indx] = label_low;
					MergeLabels(Label, label_low, label_high, curr_max);
					break;
				case 10:
				case 11:
					label_low = ((int *)(Label_Img.data))[indx - width_plus - 1];
					label_high = ((int *)(Label_Img.data))[indx - width_plus + 1];
#ifdef COMPARE
					if (label_low > label_high)
					{
						int temp = label_low;
						label_low = label_high;
						label_high = temp;
					}
#endif   
					
					((int *)(Label_Img.data))[indx] = label_low;
					MergeLabels(Label, label_low, label_high, curr_max);
					break;
				}

			}
		}
	}

	///////////////////////////////// remove edge /////////////////////////////////
	Mat curr_labelImg(height, width, CV_32SC1);
	curr_labelImg = Label_Img(Rect(1, 1, width, height)).clone();

	///////////////////////////////// labelling round 2 /////////////////////////////////
	labelImg_update_round2(curr_labelImg, Label);

	labelImg = curr_labelImg;

	
#ifdef IMG_SHOW
	// show different labels by gray scale 
	Mat showImg(height, width, CV_8UC1);
	showImg.setTo(0);
	for (int i_row = 0; i_row < height; i_row++)
	{
		int row_offset = i_row * width;
		for (int j_col = 0; j_col < width; j_col++)
		{
			int indx = row_offset + j_col;
			int pixel = ((int *)curr_labelImg.data)[indx];
			if (pixel)
			{
				showImg.data[indx] = (pixel * 50) % 255;
			}
		}
	}

	// binarization of the original image 
	Mat bin_grayImg(height, width, CV_8UC1);
	bin_grayImg.setTo(0);
	for (int i_row = 0; i_row < height; i_row++)
	{
		int row_offset = i_row * width;
		for (int j_col = 0; j_col < width; j_col++)
		{
			int indx = row_offset + j_col;
			int pixel = grayImg.data[indx];
			if (pixel >= bin_th)
			{
				bin_grayImg.data[indx] = 255;
			}
		}
	}
	imwrite("./labelled.png", showImg);
	namedWindow("LabelImg", 0);
	cv::imshow("LabelImg", showImg);
	imwrite("./binary.png", bin_grayImg);
	namedWindow("binaryImg", 0);
	cv::imshow("binaryImg", bin_grayImg);
	cv::waitKey(0);
#endif //IMG_SHOW 

#ifdef DEBUG
	for (int i_row = 1; i_row < height; i_row++)
	{
		int row_offset = i_row * width;
		for (int j_col = 1; j_col < width; j_col++)
		{
			int indx = row_offset + j_col;
			int curr_label = ((int *)LabelImg.data)[indx];
			if (curr_label >= 40 && curr_label < 49)
			{
				cout << ((int *)LabelImg.data)[indx] << endl;
			}
		}
	}

	cout << "done" << endl;
#endif // DEBUG

	return MY_OK;
}
