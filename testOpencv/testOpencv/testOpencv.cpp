// testOpencv.cpp: 定义控制台应用程序的入口点。
//
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <conio.h>
using namespace std;
using namespace cv;

void detectAndDisplay();
int catdetect();
Mat detectmodule(Mat);
#define threshold_diff 25
Mat srcImage;
Mat dstImage;
string xmlPath = "../haarcascade_frontalcatface.xml";
int contrastValue;
int brightValue;
int treshold;
bool havecat;

string HashValue(Mat &src) //c'est pour calculer le valeur de l'image
{
	string rst(64, '\0');
	Mat img;
	if (src.channels() == 3)cvtColor(src, img, CV_BGR2GRAY);
	else
	   img = src.clone();
	resize(img, img, Size(8, 8));
	uchar *pData;
	for (int i = 0; i < img.rows; i++)
	{
		pData = img.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			pData[j] = pData[j] / 4;
		}
	}
	int average = mean(img).val[0];
	Mat mask = (img >= (uchar)average);
	int index = 0;
	for (int i = 0; i < mask.rows; i++)
	{
		pData = mask.ptr<uchar>(i);
		for (int j = 0; j < mask.cols; j++)
		{
			if (pData[j] == 0)
				rst[index++] = '0';
			else
				rst[index++] = '1';
		}
	}
	return rst;
}
int HanmingDistance(string str1, string str2)   // c'est pour calculer le différence entre deux valeurs de Hash
{
	if ((str1.size() != 64) || (str2.size() != 64))
		return -1;
	int diff = 0;
	for (int i = 0; i < 64; i++)
	{
		if (str1[i] != str2[i])
			diff++;
	}
	return diff;
}

int main(int argc, char ** argv)
{
	catdetect();

	return 0;
}
int catdetect() {
	havecat = false;

	CascadeClassifier a;     //creer objet de visage de chat
	if (!a.load(xmlPath))     //si le path de xml nest pas correct
	{
		cout << "xml file peut pas access" << endl;
		return 0;
	}
	cout << "ici" << endl;
	detectAndDisplay();// detect cat
	return 0;
}

void detectAndDisplay()
{
	cout << "ici" << endl;
	VideoCapture capture(0);
	Mat image1;
	String str1, str2, path2, str;
	vector<String> str2v;
	Mat frame;
	Mat newFrame;
	CascadeClassifier ccf;      //creer objet de visage de chat
	ccf.load(xmlPath);           //importer xml
	vector<Rect> faces;
	Mat gray;
	int diff[2];
	capture >> frame;
	cv::Point end = cv::Point(300, 400);
	int min = 1000, t, i, j;
	for (i = 1; i <= 3; i++)
	{
		path2 = format("../../../../%d.jpg", i);
		image1 = imread(path2, -1);
		image1 = detectmodule(image1);
		str2 = HashValue(image1);
		str2v.push_back(str2);

	}
	cout << "ici" << endl;
	Mat image;
	while (1)
	{



		if (frame.empty())
		{
			printf("--(!) No captured frame -- Break!");

		}
		else
		{

			capture >> frame;

			cvtColor(frame, gray, CV_BGR2GRAY);
			equalizeHist(gray, gray);
			ccf.detectMultiScale(gray, faces, 1.1, 3, 0);

			for (vector<Rect>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
			{
				rectangle(frame, *iter, Scalar(0, 0, 255), 2, 8); //jouer le sphere de visage
				havecat = true;
			}
			for (size_t i = 0; i < faces.size(); i++)
			{
				Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
				image = frame(Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height));
			}
			if (havecat == true) {
				str1 = HashValue(image);
				for (i = 1; i <= 3; i++)
				{

					diff[i] = HanmingDistance(str1, str2v[i - 1]);
				

				}
				/*cout << "aiji:" << diff[2] << endl;
				cout << "buou:" << diff[1] << endl;
				cout << "dongfang:" << diff[3] << endl;*/
				min = 40;
				for (j = 1; j <= 3; j++)
				{
					if (min > diff[j] && diff[j] != 0)
					{
						min = diff[j];
						t = j;
					}
				}
				if (min > 40) {
					t = 0;
				}

				if (t == 0) {
					str = "je sais pas";
					putText(frame, str, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 2);

				}
				if (t == 2) {
					str = "C'est Mau egyptien";
					putText(frame, str, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 2);

				}if (t == 1) {
					str = "C'est Ragdoll";

					putText(frame, str, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 2);

				}
				
				if (t == 3) {
					str = "C'est Oriental shorthair";

					putText(frame, str, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
					
				}
				


				havecat = false;


			}
			imshow("src", frame);

		}


		waitKey(16);
		capture >> frame;
	}
}
Mat detectmodule(Mat image) {
	CascadeClassifier ccf;      //creer objet de visage de chat
	ccf.load(xmlPath);           //importer xml
	vector<Rect> faces;
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	equalizeHist(gray, gray);
	ccf.detectMultiScale(gray, faces, 1.1, 3, 0);

	for (vector<Rect>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
	{
		rectangle(image, *iter, Scalar(0, 0, 255), 2, 8); //jouer le sphere de visage
		//havecat = true;
	}
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		image = image(Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height));
	}

	return image;

}

