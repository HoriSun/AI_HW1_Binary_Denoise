#include <opencv2\opencv.hpp>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <conio.h>
#include <ctime>

using namespace cv;

float lonely(Mat & m, int r, int c, int radius){
	int nBlack = 0, nWhite = 0;
	int r0 = r < radius ? 0 : r - radius;
	int r1 = r > m.rows - radius ? m.rows - 1 : r + radius;
	int c0 = c < radius ? 0 : c - radius;
	int c1 = c > m.cols - radius ? m.cols - 1 : c + radius;
	for (int i = r0; i <= r1; i++){
		for (int j = c0; j <= c1; j++){
			if (m.at<char>(i, j) == 0) ++nBlack;
			else ++nWhite;
		}
	}
	return m.at<char>(r, c) == 0 ? (float)nWhite / (float)nBlack : (float)nBlack / (float)nWhite;
}

int main(int argc , char* argv[]){
	char * _filename = "noise8b.bmp";
	char * filename;
	Mat image = imread(filename = argc>1 ? argv[1] : _filename, 0);
	puts(filename);
	if (image.empty()){
		puts("image open failed.");
		_getch();
		return 0;
	}
	srand(time(0));
	Mat now = image.clone();
	int nNoise = image.cols * image.rows *0.05;
	for (int i, j,k = 0;k<nNoise;k++){
		i = rand() % image.rows;
		j = rand() % image.cols;
		now.at<char>(i, j) = now.at<char>(i, j) == 0 ? 255 : 0;
	}
	Mat reduce = now.clone();
	namedWindow(filename);
	imshow(filename, now);
	imshow("ori", image);
	waitKey(0);
	for (int i = 0; i < now.rows; ++i){
		for (int j = 0; j < now.cols; ++j){
			if (lonely(now, i, j, 1) > 1.25){
				reduce.at<char>(i, j) = now.at<char>(i, j) == 0 ? 255 : 0;
				//continue;
			}
			else if (lonely(now, i, j, 3) > 1){
				reduce.at<char>(i, j) = now.at<char>(i, j) == 0 ? 255 : 0;
			}
		}
	}
	

	int diffn = 0, diffd = 0;
	for (int i = 0; i < now.rows; ++i){
		for (int j = 0; j < now.cols; ++j){
			if (image.at<char>(i, j) != now.at<char>(i, j))diffn++;
			if (image.at<char>(i, j) != reduce.at<char>(i, j))diffd++;
		}
	}
	printf("diffn: %d\t diffd: %d\n", diffn, diffd);
	imshow("reduced", reduce);
	waitKey(0);
	return 0;
}