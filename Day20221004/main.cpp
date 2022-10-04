#include<opencv2/opencv.hpp>
#include<iostream>
#include"mouse_Move.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
	String image_Path = "D:/OpenCV_Learning/��������/1.��ɡ������ĵ����ڵ�����һ/΢��ͼƬ_20211103234539.jpg";
	Mat image;
	image = imread(image_Path);
	if (image.empty())
	{
		cout << "Falied to get the image!" << endl;
		return -1;
	}
	//cout << image.rows << endl; // ��ȡ��ǰͼ��������ص���� 599
	//cout << image.cols << endl;	// ��ȡ��ǰͼ��ĺ����ص���� 710
	
	imshow("Image", image);

		/*	setMouseCallback()
		*	@param1: Ҫ��Ӧ��궯���Ĵ��ڵ�����
		*	@param2: ��궯��ʱ���õĻص�����MouseCallback()�ĵ�ַ
		*		��귢������ʱ����øõ�ַ����Ӧ�Ļص�����
		*		ֻҪ����ڶ�Ӧ�����ڷ����κζ����������ƶ������������֣��������ִ�иú���
		*		�����������ڴ����������ƶ�	����ú���������ִ��
		*		ע�⣺�������ڴ����ڲ���������������꽨ʱ�ú���ִ��1�Σ��ɿ���꽨ʱ�ú�����ִ��2��
		*		�������ڴ����ڲ������֣�����ֹ���һ��ִ�иú���1��
		*	@param3: Ҫ����ص�����MouseCallback()�Ĳ���/�����ĵ�ַ��ҪǿתΪvoid*���ͣ�
		*/
	setMouseCallback("Image", mouse_Move, (void*)&image);

	waitKey(0);
	return 0;
}
