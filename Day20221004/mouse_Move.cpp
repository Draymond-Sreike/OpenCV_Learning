#include<opencv2/opencv.hpp>
#include<iostream>
#include<typeinfo>

using namespace std;
using namespace cv;

// ����ȫ�ֱ�������ּ��Ϊmouse_Move()����ص���������
// ��Ϊ����ִ��ʱ���ú��������������ã�Ϊ���⺯���ĵ��õ��¾ֲ�������Ƶ������������
// �ʽ��ú�������ʹ�õı�������Ϊȫ�ֱ���
int mouse_Current_Point_BGR_B, mouse_Current_Point_BGR_B_Ptr;	// ��¼��ǰ���ص��Bֵ
int mouse_Current_Point_BGR_G, mouse_Current_Point_BGR_G_Ptr;	// ��¼��ǰ���ص��Gֵ
int mouse_Current_Point_BGR_R, mouse_Current_Point_BGR_R_Ptr;	// ��¼��ǰ���ص��Rֵ
Mat image_Temp;	// imageͼ���ʵʱ���¿���
Mat image_ROI;		// imageͼ�񱻿�ѡ����
Rect drawing_Rectangle;		// �����ľ���
Point mouse_Current_Point;	// ��¼��ǰ���ص�����
Point drawing_Rectangle_Start_Point;		// ��¼�������˲������ص����꣬������Ϊ�������ο����ʼ�㣨�����ο����Ͻǵĵ㣩
Point drawing_Rectangle_Center_Point;	// �����ľ��ο�����ĵ�
String mouse_Current_Point_String;			// �ı���ʾ��ǰ���ص����꣬����putText()
String mouse_Current_Point_BGR_String;	// �ı���ʾ��ǰ���ص�BGR������putText()
String image_Save_Path = "D:/OpenCV_Learning/��������/1.��ɡ������ĵ����ڵ�����һ/7.����¼�Ӧ��/Ч��ͼ����Ƶ";
/*	mouse_Move(int event, int x, int y, int flags, void* userdata)��귢������ʱ��ִ�еĺ���
*	���²��������������˶�Ӧ�����Ӧ�Ĵ�����
*	@param1: ��굥����Ķ�����־����ͬ�ĵ���"�����Ӧ�¼���־event���ձ�.jpg"��
*		�������ڸú����н��ж����жϲ�������Ӧ����
*	@param2: ������ڵ����ص�ĺ�����x
*	@param3: ������ڵ����ص��������y
*	@param4: �����ҷʱ������������������־����ͬ�ĵ���"�����Ӧ��־flags���ձ�.jpg"��
*		����õ�������ҷ��flagsΪ0��ֻ��������µ���ҷ��flagsΪ1��ֻ��Ctrl�����µ���ҷ��flagsΪ8
*		��������������Ctrl�����µ���ҷ��flagsΪ9(1+8)
*	@param5: ����������setMouseCallback()�����Ĳ���/�����ĵ�ַ
*		�õ�ַ�ѱ�ǿתΪvoid*���ͣ��ǵ�ת��Ϊ��Ӧ���͵�ָ�������ٽ���ʹ�ã�
*	@attention: 1.����ڴ��ڷ����ް�����ҷʱevent��flags��ֵ��Ϊ0
*						2.���°������ڴ�����ҷʱeventֻ�ڰ��º��ɿ�����˲����ӦΪ��Ӧֵ
*							��������ҷ��ֵ��Ϊ0
*						3.���°������ڴ�����ҷʱflags�ں�������ҷ�����ж��Ƕ�Ӧ����Ӧֵ
*						4.���ֻ����ҷ��꣬������뿪����˲������¼�뿪���ڴ���x��y�������ڴ��������ҷ����ı�x��y
*						   ������ڴ����а��°�����ҷ�������ڴ��ڼ䰴�����ſ��������ҷ�������⣬���Ի�ı�x��y
*						   ����������ֵ���Դ��ڵ����Ͻ�����λ��Ϊԭ��(0,0)������������ĻΪ�߽��x��y
*						5.��Ϊmouse_Move()�������ڶ�����main.cpp�ļ��⣬Ϊ��֤main.cpp�е�setMouseCallback�ܵ��øú���
						   ������Ҫ����Ϊstatic
*/
void mouse_Move(int event, int x, int y, int flags, void* image_Addr)// �ص�����MouseCallback()
{
	if (0 <= x && x <= (((Mat*)(image_Addr))->cols - 1))
	{// �����к����ص��ȡֵ��Χ:0~(image.cols - 1)����image.cols�����ص�
	 // �˴������жϵ�ǰ���ص������x(col)�Ƿ�Ϸ�(����[0, (image.cols - 1)]������)

		mouse_Current_Point.x = x;
	}
	if (0 <= y && y <= (((Mat*)(image_Addr))->rows - 1))
	{// �������������ص��ȡֵ��Χ:0~(image.rows - 1)����image.rows�����ص�
	 // �˴������жϵ�ǰ���ص�������y(row)�Ƿ�Ϸ�(����[0, (image.rows - 1)]������)

		mouse_Current_Point.y = y;
	}
	//cout << mouse_Current_Point << endl;				// ���Ե�ǰ���ص����꣨�˴�����ʾ�����������ʽ��

	mouse_Current_Point_String = "(" + to_string(mouse_Current_Point.x) + "," + to_string(mouse_Current_Point.y) + ")";
		// ����ǰ���ص�����ת��ΪString���ͱ��ں���putText()��ʾ

	//cout << mouse_Current_Point_String << endl;	// ����ת�����String

		 /*		BGR����putText()�ı���ʾ
		 *		ע��:
		 *		1.at<Vec3b>(row, col)�����е���ֵ��������(x, y)��Ҫ���Ҳ��Ҫ����(y, x)
		 *		2.at<Vec3b>(row,col)�е�
		*/
	uchar* ptr = ((Mat*)(image_Addr))->ptr<uchar>(mouse_Current_Point.y);	// ��ȡ��ǰ���ص����
	//cout << typeid(*ptr).name() << endl;	// ���*ptrΪuchar����
		// BGR����Version2.0
	mouse_Current_Point_BGR_B = *(ptr + 3 * mouse_Current_Point.x);
	mouse_Current_Point_BGR_G = *(ptr + 3 * mouse_Current_Point.x + 1);
	mouse_Current_Point_BGR_R = *(ptr + 3 * mouse_Current_Point.x + 2);
	//cout << mouse_Current_Point_BGR_B << mouse_Current_Point_BGR_G << mouse_Current_Point_BGR_R << endl;	
		// ����ָ���������Ƿ���ȷ
	
	// �˴������ص�BGR�ķ��ʵ���Ϊָ����ʣ�����߷����ٶ�
	
		// BGR����Version1.0
	//mouse_Current_Point_BGR_B = (int)( ( (Mat*)(image_Addr) )->at<Vec3b>(mouse_Current_Point.y, mouse_Current_Point.x)[0]);
	//mouse_Current_Point_BGR_G = (int)( ( (Mat*)(image_Addr) )->at<Vec3b>(mouse_Current_Point.y, mouse_Current_Point.x)[1]);
	//mouse_Current_Point_BGR_R = (int)( ( (Mat*)(image_Addr) )->at<Vec3b>(mouse_Current_Point.y, mouse_Current_Point.x)[2]);
	
	mouse_Current_Point_BGR_String = "BGR:[" +
		to_string(mouse_Current_Point_BGR_B) +
		"," +
		to_string(mouse_Current_Point_BGR_G) +
		"," +
		to_string(mouse_Current_Point_BGR_R) +
		"]";
		// ����ǰ���ص�BGRת��ΪString���ͱ��ں���putText()��ʾ
		
	//cout << mouse_Current_Point_BGR_String << endl;// ����ת�����String

	((Mat*)(image_Addr))->copyTo(image_Temp);
		// ����һ��image����ʱͼ��image_Temp������ʵʱ������һ��image_Temp
		// �Ա�֤ÿ��putText()ʱֻ��һ���ı���image_Temp����ʾ
		// ��ΪputText()���޸�������Ӧ��ͼ��(Mat ����)�������ñ�������޸ģ��ڳ��������ڼ䣩
		// ���ֱ��putText()��image���в�������ÿ��putText()�����ݶ��ᱻ������image��

	putText(image_Temp,
		mouse_Current_Point_String + mouse_Current_Point_BGR_String,
		mouse_Current_Point,
		FONT_HERSHEY_SIMPLEX,
		0.4,
		Scalar(0, 0, 0));
		// ��ʾ����������Ϸ���putText()
		// ��ʾ����Ϊ��ǰ���ص�����(mouse_Current_Point_String)����ǰ���ص�BGR(mouse_Current_Point_BGR_String)

	if (EVENT_LBUTTONDOWN == event)	// ����������˲��
	{
		// ������������˲������ص�Ϊ�����ľ��ο����ʼ�㣨�����ο����Ͻǣ�
		drawing_Rectangle_Start_Point.x = mouse_Current_Point.x;
		drawing_Rectangle_Start_Point.y = mouse_Current_Point.y;
	}
	//cout << drawing_Rectangle_Start_Point << endl;	// ���Ը���ʼ���ص�

	if ((EVENT_MOUSEMOVE == event) && (flags & EVENT_FLAG_LBUTTON))	// ������������ҷ
	{
		drawing_Rectangle = Rect(drawing_Rectangle_Start_Point, mouse_Current_Point);
			// �����ľ���
		rectangle(image_Temp, drawing_Rectangle, Scalar(0, 255, 0), 2);
			// ��image_Temp�ϣ�������Ĵ����ľ�����image_Temp�ϻ��Ƴ���
	}

	if (EVENT_LBUTTONUP == event)	// �������ͷ�˲��
	{
		image_Temp(drawing_Rectangle).copyTo(image_ROI);
			// �����ο�����ͼ�����һ����image_ROI
		imshow("Image ROI", image_ROI);
			// ��ʾ�����ο��ѡ��ͼ��image_ROI
		imwrite(image_Save_Path + "/Image ROI.png", image_ROI);
			// �����ο��ѡ��ͼ����б���
		//cout << typeid(drawing_Rectangle.tl().x).name() << endl;	// ��ⷢ��drawing_Rectangle.tl().xΪint����

		drawing_Rectangle_Center_Point.x = (drawing_Rectangle.tl().x + drawing_Rectangle.br().x) / 2;
		drawing_Rectangle_Center_Point.y = (drawing_Rectangle.tl().y + drawing_Rectangle.br().y) / 2;
			// ������ο����ĵ㣬�������ص�����Ϊ����ֵ���˴����㲻��ת��Ϊ������
		cout << "���ο����ĵ�����:("
			<< drawing_Rectangle_Center_Point.x
			<< ","
			<< drawing_Rectangle_Center_Point.y
			<< ")" << endl;
			// cout������ο��������ص�����
	}

	imshow("Image", image_Temp);
		// ʵʱ��ʾ���±�putText()��rectangle()�޸ĵ�imageͼ�񿽱�:image_Temp
	
	{// mouse_Move()�������ܲ��Դ����
		//cout << "Mouse move!" << endl;
		//cout << "x:" << x << " " << "y:" << y << endl;

		//cout << "MouseCallback event:" << event << endl;
		//cout << "MouseCallback flags:" << flags << endl;

		//cout << (flags & EVENT_FLAG_LBUTTON) << endl;
			// ִ�и���䷢��������Σ�ֻҪû�а������������ҷ(EVENT_FLAG_LBUTTON)��ô��λ��Ľ��һ����0
			// ��Ϊflags�ı�־ֵ����1/2/4/8/16/32���й��ɵ�ȡ�����Ƶ�ÿһλ
			// ����������ҷʱ��0: 0000 0000���������ҷʱ��1: 0000 0001����Ctrl����ҷʱ��8: 0000 1000
			// ��ͬʱ�������Ctrl����ҷʱ��9: 0000 1001
			// ��Ҳ����˵�κ�ʱ�������ҷʱ�����ӵ��������������Ӧ�������ڶ����Ƶ�ĳһλ�ϣ�����Ӱ������λ
			// ��˵�ʹ��(flags & EVENT_FLAG_XXXXXXX)ʱ��
				//	ֻ�е�ǰ����ҷ�����и��ӵĶ�����EVENT_FLAG_XXXXXXX��Ӧ�Ķ������Ż�ʹ�ø�&�����Ľ����ֵ����0��
				// �����ǰ����ҷ�����и��ӵĶ���û��EVENT_FLAG_XXXXXXX��Ӧ�Ķ��������&�����Ľ����ԶΪ0
	}
}

