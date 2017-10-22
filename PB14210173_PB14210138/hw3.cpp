#include "opencv.hpp"
using namespace cv;
#include <iostream>
using namespace std;
#include <time.h>
#include <math.h>

#define IMG_SHOW
#define myok 1
#define myfail -1

int ustc_ConnectedComponentLabeling(Mat grayImg, Mat & labelImg, int bin_th){
	
    int plate[100000] = { 0 };
    int i, j=1;
	int grayindex=1;
	int exchange1, exchange2, label_create = 1;
	int height = grayImg.rows
	int width = grayImg.cols;
	int size = height * width;

	if (NULL == grayImg.data){
		cout << "grayimage is NULL." << endl;
		return myfail;
	}
	if (NULL == labelImg.data){
		cout << "labelimage is NULL." << endl;
		return myfail;
	}

	if (grayImg.data[0] >= bin_th){
		((int *)labelImg.data)[0] = 1;
		plate[1] = label_create+1;
	}else
		((int *)labelImg.data)[0] = 0;



	for (grayindex = 1; grayindex < width; grayindex++)
		if (grayImg.data[grayindex] >= bin_th){
			if (grayImg.data[grayindex - 1] >= bin_th)
				((int *)labelImg.data)[grayindex] = ((int *)labelImg.data)[grayindex - 1];
			else{
				((int *)labelImg.data)[grayindex] = label_create;
				plate[label_create] = label_create+1;
			}
		}else{
			((int *)labelImg.data)[grayindex] = 0;
		}




	for (grayindex = width; grayindex < size; grayindex += width)
		if (grayImg.data[grayindex] >= bin_th){
			if (grayImg.data[grayindex - width] >= bin_th)
				((int *)labelImg.data)[grayindex] = ((int *)labelImg.data)[grayindex - width];
			else{
				((int *)labelImg.data)[grayindex] = label_create;
				plate[label_create] = label_create+1;
			}
		}else{
			((int *)labelImg.data)[grayindex] = 0;
		}



	for (i = width; i < size; i += width)
		for (j = 1, grayindex = i + 1; j < width; j++, grayindex++)
			if (grayImg.data[grayindex] >= bin_th){
				if (grayImg.data[grayindex - width] >= bin_th){
					((int *)labelImg.data)[grayindex] = ((int *)labelImg.data)[grayindex - width];
				}else if (grayImg.data[grayindex - 1] >= bin_th){
					((int *)labelImg.data)[grayindex] = ((int *)labelImg.data)[grayindex - 1];
					if (grayImg.data[grayindex - width + 1] >= bin_th){
						exchange1 = ((int *)labelImg.data)[grayindex - width + 1];
						exchange2 = ((int *)labelImg.data)[grayindex - 1];
						while (plate[exchange1] != exchange1)
							exchange1 = plate[exchange1];
						while (plate[exchange2] != exchange2)
							exchange2 = plate[exchange2];
						if (exchange1 > exchange2)
							plate[exchange1] = plate[exchange2];
						else
							plate[exchange2] = plate[exchange1];
					}
				}else if (grayImg.data[grayindex - width - 1] >= bin_th){
					((int *)labelImg.data)[grayindex] = ((int *)labelImg.data)[grayindex - width - 1];
					if (grayImg.data[grayindex - width + 1] >= bin_th){
						exchange1 = ((int *)labelImg.data)[grayindex - width + 1];
						exchange2 = ((int *)labelImg.data)[grayindex - width - 1];
						while (plate[exchange1] != exchange1)
							exchange1 = plate[exchange1];
						while (plate[exchange2] != exchange2)
							exchange2 = plate[exchange2];
						if (exchange1 > exchange2)
							plate[exchange1] = plate[exchange2];
						else
							plate[exchange2] = plate[exchange1];
					}
				}else if (grayImg.data[grayindex - width + 1] >= bin_th){
					((int *)labelImg.data)[grayindex] = ((int *)labelImg.data)[grayindex - width + 1];
				}else{
					((int *)labelImg.data)[grayindex] = label_create;
					plate[label_create] = label_create++;
				}
			}else{
				((int *)labelImg.data)[grayindex] = 0;
			}



	for (i = 1; i <= 99999; i++){
		if (plate[i]==0)
			break;
		if (plate[i] == i)
			continue;
		exchange1 = plate[i];
		while (plate[exchange1] != exchange1)
			exchange1 = plate[exchange1];
		plate[i] = exchange1;
	}


	for (i = 0; i < size; i++)
		((int *)labelImg.data)[i] = plate[((int *)labelImg.data)[i]];
	return myok;
}
