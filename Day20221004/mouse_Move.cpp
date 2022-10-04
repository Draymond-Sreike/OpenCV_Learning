#include<opencv2/opencv.hpp>
#include<iostream>
#include<typeinfo>

using namespace std;
using namespace cv;

// 以下全局变量创建旨在为mouse_Move()这个回调函数所用
// 因为程序执行时，该函数将被反复调用，为避免函数的调用导致局部变量的频繁创建和销毁
// 故将该函数中所使用的变量创建为全局变量
int mouse_Current_Point_BGR_B, mouse_Current_Point_BGR_B_Ptr;	// 记录当前像素点的B值
int mouse_Current_Point_BGR_G, mouse_Current_Point_BGR_G_Ptr;	// 记录当前像素点的G值
int mouse_Current_Point_BGR_R, mouse_Current_Point_BGR_R_Ptr;	// 记录当前像素点的R值
Mat image_Temp;	// image图像的实时更新拷贝
Mat image_ROI;		// image图像被框选部分
Rect drawing_Rectangle;		// 待画的矩形
Point mouse_Current_Point;	// 记录当前像素点坐标
Point drawing_Rectangle_Start_Point;		// 记录左键按下瞬间的像素点坐标，后续作为待画矩形框的起始点（即矩形框左上角的点）
Point drawing_Rectangle_Center_Point;	// 待画的矩形框的中心点
String mouse_Current_Point_String;			// 文本显示当前像素点坐标，用于putText()
String mouse_Current_Point_BGR_String;	// 文本显示当前像素点BGR，用于putText()
String image_Save_Path = "D:/OpenCV_Learning/考核任务/1.完成《任务文档》内的任务一/7.鼠标事件应用/效果图及视频";
/*	mouse_Move(int event, int x, int y, int flags, void* userdata)鼠标发生动作时所执行的函数
*	以下参数均是在设置了对应鼠标响应的窗口中
*	@param1: 鼠标单击类的动作标志（见同文档中"鼠标响应事件标志event对照表.jpg"）
*		后续用在该函数中进行动作判断并进行相应处理
*	@param2: 鼠标所在的像素点的横坐标x
*	@param3: 鼠标所在的像素点的纵坐标y
*	@param4: 鼠标拖曳时所附带的其他动作标志（见同文档中"鼠标响应标志flags对照表.jpg"）
*		如果置单纯地拖曳则flags为0，只有左键按下的拖曳则flags为1，只有Ctrl键按下的拖曳则flags为8
*		如果有左键按下且Ctrl键按下的拖曳则flags为9(1+8)
*	@param5: 从主调函数setMouseCallback()传来的参数/变量的地址
*		该地址已被强转为void*类型，记得转化为相应类型的指针类型再进行使用！
*	@attention: 1.鼠标在窗口发生无按键拖曳时event和flags的值均为0
*						2.按下按键地在窗口拖曳时event只在按下和松开按键瞬间响应为对应值
*							后续的拖曳其值均为0
*						3.按下按键地在窗口拖曳时flags在后续的拖曳过程中都是对应的响应值
*						4.如果只是拖曳鼠标，当光标离开窗口瞬间其会记录离开窗口处的x，y，而后在窗口外的拖曳不会改变x，y
*						   但如果在窗口中按下按键拖曳，并且在此期间按键不放开将光标拖曳到窗口外，则仍会改变x，y
*						   并且这两个值是以窗口的左上角所在位置为原点(0,0)，整个电脑屏幕为边界的x，y
*						5.因为mouse_Move()函数现在定义在main.cpp文件外，为保证main.cpp中的setMouseCallback能调用该函数
						   函数不要定义为static
*/
void mouse_Move(int event, int x, int y, int flags, void* image_Addr)// 回调函数MouseCallback()
{
	if (0 <= x && x <= (((Mat*)(image_Addr))->cols - 1))
	{// 窗口中横像素点的取值范围:0~(image.cols - 1)共计image.cols个像素点
	 // 此处用于判断当前像素点横坐标x(col)是否合法(处于[0, (image.cols - 1)]的区间)

		mouse_Current_Point.x = x;
	}
	if (0 <= y && y <= (((Mat*)(image_Addr))->rows - 1))
	{// 窗口中竖列像素点的取值范围:0~(image.rows - 1)共计image.rows个像素点
	 // 此处用于判断当前像素点纵坐标y(row)是否合法(处于[0, (image.rows - 1)]的区间)

		mouse_Current_Point.y = y;
	}
	//cout << mouse_Current_Point << endl;				// 测试当前像素点坐标（此处的显示是以数组的形式）

	mouse_Current_Point_String = "(" + to_string(mouse_Current_Point.x) + "," + to_string(mouse_Current_Point.y) + ")";
		// 将当前像素点坐标转化为String类型便于后续putText()显示

	//cout << mouse_Current_Point_String << endl;	// 测试转化后的String

		 /*		BGR用于putText()文本显示
		 *		注意:
		 *		1.at<Vec3b>(row, col)括号中的数值不能理解成(x, y)，要理解也是要理解成(y, x)
		 *		2.at<Vec3b>(row,col)中的
		*/
	uchar* ptr = ((Mat*)(image_Addr))->ptr<uchar>(mouse_Current_Point.y);	// 获取当前像素点的行
	//cout << typeid(*ptr).name() << endl;	// 测得*ptr为uchar类型
		// BGR访问Version2.0
	mouse_Current_Point_BGR_B = *(ptr + 3 * mouse_Current_Point.x);
	mouse_Current_Point_BGR_G = *(ptr + 3 * mouse_Current_Point.x + 1);
	mouse_Current_Point_BGR_R = *(ptr + 3 * mouse_Current_Point.x + 2);
	//cout << mouse_Current_Point_BGR_B << mouse_Current_Point_BGR_G << mouse_Current_Point_BGR_R << endl;	
		// 测试指针运算结果是否正确
	
	// 此处对像素点BGR的访问迭代为指针访问，以提高访问速度
	
		// BGR访问Version1.0
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
		// 将当前像素点BGR转化为String类型便于后续putText()显示
		
	//cout << mouse_Current_Point_BGR_String << endl;// 测试转化后的String

	((Mat*)(image_Addr))->copyTo(image_Temp);
		// 制作一份image的临时图像image_Temp，用于实时覆盖上一张image_Temp
		// 以保证每次putText()时只有一个文本在image_Temp上显示
		// 因为putText()会修改其所对应的图像(Mat 对象)，并永久保留这个修改（在程序运行期间）
		// 如果直接putText()对image进行操作，则每次putText()的内容都会被保留在image上

	putText(image_Temp,
		mouse_Current_Point_String + mouse_Current_Point_BGR_String,
		mouse_Current_Point,
		FONT_HERSHEY_SIMPLEX,
		0.4,
		Scalar(0, 0, 0));
		// 显示在鼠标光标右上方的putText()
		// 显示内容为当前像素点坐标(mouse_Current_Point_String)，当前像素点BGR(mouse_Current_Point_BGR_String)

	if (EVENT_LBUTTONDOWN == event)	// 鼠标左键按下瞬间
	{
		// 记鼠标左键按下瞬间的像素点为待画的矩形框的起始点（即矩形框左上角）
		drawing_Rectangle_Start_Point.x = mouse_Current_Point.x;
		drawing_Rectangle_Start_Point.y = mouse_Current_Point.y;
	}
	//cout << drawing_Rectangle_Start_Point << endl;	// 测试该起始像素点

	if ((EVENT_MOUSEMOVE == event) && (flags & EVENT_FLAG_LBUTTON))	// 鼠标左键按下拖曳
	{
		drawing_Rectangle = Rect(drawing_Rectangle_Start_Point, mouse_Current_Point);
			// 待画的矩形
		rectangle(image_Temp, drawing_Rectangle, Scalar(0, 255, 0), 2);
			// 在image_Temp上，将上面的待画的矩形在image_Temp上绘制出来
	}

	if (EVENT_LBUTTONUP == event)	// 鼠标左键释放瞬间
	{
		image_Temp(drawing_Rectangle).copyTo(image_ROI);
			// 将矩形框框出的图像深拷贝一份至image_ROI
		imshow("Image ROI", image_ROI);
			// 显示被矩形框框选的图像image_ROI
		imwrite(image_Save_Path + "/Image ROI.png", image_ROI);
			// 将矩形框框选的图像进行保存
		//cout << typeid(drawing_Rectangle.tl().x).name() << endl;	// 检测发现drawing_Rectangle.tl().x为int类型

		drawing_Rectangle_Center_Point.x = (drawing_Rectangle.tl().x + drawing_Rectangle.br().x) / 2;
		drawing_Rectangle_Center_Point.y = (drawing_Rectangle.tl().y + drawing_Rectangle.br().y) / 2;
			// 计算矩形框中心点，由于像素点坐标为整数值，此处计算不需转化为浮点型
		cout << "矩形框中心点坐标:("
			<< drawing_Rectangle_Center_Point.x
			<< ","
			<< drawing_Rectangle_Center_Point.y
			<< ")" << endl;
			// cout输出矩形框中心像素点坐标
	}

	imshow("Image", image_Temp);
		// 实时显示最新被putText()和rectangle()修改的image图像拷贝:image_Temp
	
	{// mouse_Move()函数功能测试代码块
		//cout << "Mouse move!" << endl;
		//cout << "x:" << x << " " << "y:" << y << endl;

		//cout << "MouseCallback event:" << event << endl;
		//cout << "MouseCallback flags:" << flags << endl;

		//cout << (flags & EVENT_FLAG_LBUTTON) << endl;
			// 执行该语句发现无论如何，只要没有按下左键进行拖曳(EVENT_FLAG_LBUTTON)那么按位与的结果一定是0
			// 因为flags的标志值都是1/2/4/8/16/32很有规律地取二进制的每一位
			// 当不按键拖曳时是0: 0000 0000，按左键拖曳时是1: 0000 0001，按Ctrl键拖曳时是8: 0000 1000
			// 当同时按左键和Ctrl键拖曳时是9: 0000 1001
			// 这也就是说任何时刻鼠标拖曳时所附加的其他动作都会对应地体现在二进制的某一位上，而不影响其他位
			// 因此当使用(flags & EVENT_FLAG_XXXXXXX)时，
				//	只有当前的拖曳操作中附加的动作有EVENT_FLAG_XXXXXXX对应的动作，才会使得该&操作的结果有值（非0）
				// 如果当前的拖曳操作中附加的动作没有EVENT_FLAG_XXXXXXX对应的动作，则该&操作的结果永远为0
	}
}

