#include<opencv2/opencv.hpp>
#include<iostream>
#include"mouse_Move.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
	String image_Path = "D:/OpenCV_Learning/考核任务/1.完成《任务文档》内的任务一/微信图片_20211103234539.jpg";
	Mat image;
	image = imread(image_Path);
	if (image.empty())
	{
		cout << "Falied to get the image!" << endl;
		return -1;
	}
	//cout << image.rows << endl; // 获取当前图像的竖像素点个数 599
	//cout << image.cols << endl;	// 获取当前图像的横像素点个数 710
	
	imshow("Image", image);

		/*	setMouseCallback()
		*	@param1: 要响应鼠标动作的窗口的名字
		*	@param2: 鼠标动作时调用的回调函数MouseCallback()的地址
		*		鼠标发生动作时会调用该地址所对应的回调函数
		*		只要鼠标在对应窗口内发生任何动作（包括移动、键击、滚轮）都会调用执行该函数
		*		其中如果鼠标在窗口内连续移动	，则该函数会连续执行
		*		注意：如果鼠标在窗口内操作鼠标键，则按下鼠标建时该函数执行1次，松开鼠标建时该函数会执行2次
		*		如果鼠标在窗口内操作滚轮，则滚轮滚动一次执行该函数1次
		*	@param3: 要传入回调函数MouseCallback()的参数/变量的地址（要强转为void*类型）
		*/
	setMouseCallback("Image", mouse_Move, (void*)&image);

	waitKey(0);
	return 0;
}
