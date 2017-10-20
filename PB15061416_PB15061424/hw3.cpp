#include"ConnectedComponentLabeling.h"
int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th) {

	//defend
	if (NULL == grayImg.data || bin_th <= 0) {
		return MY_FAIL;
	}

	int Height = grayImg.rows;
	int Width = grayImg.cols;

	//threshold
	Mat Img_Bin;
	threshold(grayImg, Img_Bin, bin_th, 255, CV_THRESH_BINARY);
	Img_Bin.convertTo(Img_Bin, CV_32SC1);
	//image pretreatment: add border
	Mat Img_Pro;
	copyMakeBorder(Img_Bin, Img_Pro, 1, 1, 1, 1, BORDER_CONSTANT, 0);
	int Height_Pro = Img_Pro.rows;
	int Width_Pro = Img_Pro.cols;
	int* Ptr_Pro = (int*)Img_Pro.data;

	//first scan
	int m = 1;//label_index
	int* rtable = (int*)malloc(Height*Width * sizeof(int));
	int* next = (int*)malloc(Height*Width * sizeof(int));
	int*tail = (int*)malloc(Height*Width * sizeof(int));


	for (int Y = 1; Y < Height_Pro - 1; Y++) {
		for (int X = 1; X < Width_Pro - 1; X++) {
			int Curr_Label_Index = Y*Width_Pro + X;
			if (Ptr_Pro[Curr_Label_Index] == VO) {

				int c1 = Ptr_Pro[Y *Width_Pro + X - 1];
				int c2 = Ptr_Pro[(Y - 1) *Width_Pro + X - 1];
				int c3 = Ptr_Pro[(Y - 1) *Width_Pro + X];
				int c4 = Ptr_Pro[(Y - 1) *Width_Pro + X + 1];


				if (c3) {
					Ptr_Pro[Curr_Label_Index] = c3;
				}
				else if (c1) {
					Ptr_Pro[Curr_Label_Index] = c1;
					if (c4) {
						//resolve(c1,c4)
						int u = rtable[c1];
						int v = rtable[c4];
						if (u < v) {

							int i = v;
							while (i != -1) {
								rtable[i] = u;
								i = next[i];
							}
							next[tail[u]] = v;
							tail[u] = tail[v];

						}
						else if (u > v) {

							int i = u;
							while (i != -1) {
								rtable[i] = v;
								i = next[i];
							}
							next[tail[v]] = u;
							tail[v] = tail[u];

						}
					}
				}
				else if (c2) {
					Ptr_Pro[Curr_Label_Index] = c2;
					if (c4) {
						//resolve(c4,c2)
						int u = rtable[c4];
						int v = rtable[c2];
						if (u < v) {

							int i = v;
							while (i != -1) {
								rtable[i] = u;
								i = next[i];
							}
							next[tail[u]] = v;
							tail[u] = tail[v];

						}
						else if (u > v) {

							int i = u;
							while (i != -1) {
								rtable[i] = v;
								i = next[i];
							}
							next[tail[v]] = u;
							tail[v] = tail[u];

						}
					}
				}
				else if (c4) {
					Ptr_Pro[Curr_Label_Index] = c4;
				}
				else {
					Ptr_Pro[Curr_Label_Index] = m;
					rtable[m] = m;
					next[m] = -1;
					tail[m] = m;
					m++;
				}

			}
		}
	}
	labelImg = Img_Pro(Rect(1, 1,Width, Height )).clone();
	int* Label_Ptr = (int*)labelImg.data;
	
	//second scan
	int Label_Size = Height*Width;
	int Label_Index = 0;
	for (; Label_Index < Label_Size - 4; Label_Index+=4) {
		if (Label_Ptr[Label_Index]) {
			Label_Ptr[Label_Index] = rtable[Label_Ptr[Label_Index]];
		}
		if (Label_Ptr[Label_Index+1]) {
			Label_Ptr[Label_Index+1] = rtable[Label_Ptr[Label_Index+1]];
		}
		if (Label_Ptr[Label_Index+2]) {
			Label_Ptr[Label_Index+2] = rtable[Label_Ptr[Label_Index+2]];
		}
		if (Label_Ptr[Label_Index+3]) {
			Label_Ptr[Label_Index+3] = rtable[Label_Ptr[Label_Index+3]];
		}
	}
	for (; Label_Index < Label_Size; Label_Index++) {
		if (Label_Ptr[Label_Index]) {
			Label_Ptr[Label_Index] = rtable[Label_Ptr[Label_Index]];
		}
	}

	return MY_OK;
}
