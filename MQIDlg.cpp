
// MQIDlg.cpp: 实现文件
//

#include "framework.h"
#include "MQI.h"
#include "MQIDlg.h"
#include "afxdialogex.h"

#include "triangle.c"
#include <string>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMQIDlg 对话框
CMQIDlg::CMQIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MQI_DIALOG, pParent)
	, m_CheckMaxRectBoundary(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	offset = 10;

	DrawPointsFlag = false;
	PropertyDataFlag = false;
	BoundaryFlag = false;
	GriddingMethod = false;

	m_XMin = ""; m_XMax = ""; 
	m_YMin = ""; m_YMax = ""; 
	m_ZMin = ""; m_ZMax = "";
	
	DataTotalNum = "";  
	DataPointAverDis = "";	
	m_BoundaryMaxDist = ""; 
	
	m_MaxPointsNumofMQSurface = "";
	m_RecDepth = "";

	m_XEleNum = "";	m_YEleNum = ""; 
	m_XEleSize = ""; m_YEleSize = "";

	m_OutputCalInfo = "";
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//对颜色赋初值
	ColorNum = 21; //个数
	//值
	Data[0] = 0.f;		Data[1] = 5.f;		Data[2] = 10.f;		Data[3] = 15.f;
	Data[4] = 20.f;		Data[5] = 25.f;		Data[6] = 30.f;		Data[7] = 35.f;
	Data[8] = 40.f;		Data[9] = 45.f;		Data[10] = 50.f;	Data[11] = 55.f;
	Data[12] = 60.f;	Data[13] = 65.f;	Data[14] = 70.f;	Data[15] = 75.f;
	Data[16] = 80.f;	Data[17] = 85.f;	Data[18] = 90.f;	Data[19] = 95.f;
	Data[20] = 100.f;

	//红
	Red[0] = 0;	   Red[1] = 0;	    Red[2] = 0;	    Red[3] = 0;
	Red[4] = 0;	   Red[5] = 0;	    Red[6] = 0;	    Red[7] = 0;
	Red[8] = 0;	   Red[9] = 0;	    Red[10] = 0;	Red[11] = 128;
	Red[12] = 255; Red[13] = 191;	Red[14] = 255;	Red[15] = 255;
	Red[16] = 211; Red[17] = 132;	Red[18] = 96;	Red[19] = 80;
	Red[20] = 70;
	//绿
	Green[0] = 0;	  Green[1] = 0;  	Green[2] = 0;	    Green[3] = 0;
	Green[4] = 0;	  Green[5] = 0;	    Green[6] = 128;	    Green[7] = 255;
	Green[8] = 192;	  Green[9] = 255;	Green[10] = 128;	Green[11] = 192;
	Green[12] = 255;  Green[13] = 128;	Green[14] = 128;	Green[15] = 0;
	Green[16] = 0;	  Green[17] = 0;	Green[18] = 0;	    Green[19] = 0;
	Green[20] = 0;
	//蓝
	Blue[0] = 80;	 Blue[1] = 100;		Blue[2] = 128;		Blue[3] = 170;
	Blue[4] = 211;	 Blue[5] = 255;		Blue[6] = 255;		Blue[7] = 255;
	Blue[8] = 128;	 Blue[9] = 0;		Blue[10] = 0;		Blue[11] = 0;
	Blue[12] = 0;    Blue[13] = 0;		Blue[14] = 0;		Blue[15] = 0;
	Blue[16] = 0;    Blue[17] = 64; 	Blue[18] = 96;		Blue[19] = 80;
	Blue[20] = 70;

}

void CMQIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROGRESS_READ_FILE, m_ProgressReadFile);

	DDX_Text(pDX, IDC_EDIT_XMIN, m_XMin);
	DDX_Text(pDX, IDC_EDIT_XMAX, m_XMax);
	DDX_Text(pDX, IDC_EDIT_YMIN, m_YMin);
	DDX_Text(pDX, IDC_EDIT_YMAX, m_YMax);
	DDX_Text(pDX, IDC_EDIT_ZMIN, m_ZMin);
	DDX_Text(pDX, IDC_EDIT_ZMAX, m_ZMax);
	DDX_Text(pDX, IDC_EDIT_DATA_TOTAL_NUM, DataTotalNum);
	DDX_Text(pDX, IDC_EDIT_DATA_AVERGE_DIS, DataPointAverDis);

	DDX_Text(pDX, IDC_EDIT_BOUNDARY_DMAX, m_BoundaryMaxDist);
	
	DDX_Text(pDX, IDC_EDIT_GLOBAL_MQ_DATA_MAX_NUM, m_MaxPointsNumofMQSurface);
	DDX_Text(pDX, IDC_EDIT_REC_DEPTH, m_RecDepth);

	DDX_Text(pDX, IDC_EDIT_X_ELEMENT_NUM, m_XEleNum);
	DDX_Text(pDX, IDC_EDIT_Y_ELEMENT_NUM, m_YEleNum);

	DDX_Text(pDX, IDC_EDIT_X_ELEMENT_SIZE, m_XEleSize);
	DDX_Text(pDX, IDC_EDIT_Y_ELEMENT_SIZE, m_YEleSize);

	DDX_Text(pDX, IDC_EDIT_OUTPUT_CAL_INFO, m_OutputCalInfo);
	DDX_Check(pDX, IDC_CHECK_MAX_RECT, m_CheckMaxRectBoundary);
}

BEGIN_MESSAGE_MAP(CMQIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATA, &CMQIDlg::OnClickedButtonLoadData)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_BOUNDARY, &CMQIDlg::OnClickedButtonSearchBoundary)
	ON_COMMAND(IDC_RADIO_MQ_METHOD, &CMQIDlg::OnRadioMQInterMethod)
	ON_COMMAND(IDC_RADIO_TRI_LINEAR_METHOD, &CMQIDlg::OnRadioTriLinearInterMethod)
	ON_EN_CHANGE(IDC_EDIT_X_ELEMENT_NUM, &CMQIDlg::OnChangeEditXElementNum)
	ON_EN_CHANGE(IDC_EDIT_Y_ELEMENT_NUM, &CMQIDlg::OnChangeEditYElementNum)
	ON_EN_CHANGE(IDC_EDIT_GLOBAL_MQ_DATA_MAX_NUM, &CMQIDlg::OnChangeEditGlobalMqDataMaxNum)
	ON_BN_CLICKED(IDC_BUTTON_START_GRIDDING, &CMQIDlg::OnClickedButtonStartGridding)
	ON_EN_CHANGE(IDC_EDIT_REC_DEPTH, &CMQIDlg::OnChangeEditRecDepth)
	ON_BN_CLICKED(IDCANCEL, &CMQIDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_MAX_RECT, &CMQIDlg::OnClickedCheckMaxRect)
END_MESSAGE_MAP()


// CMQIDlg 消息处理程序

BOOL CMQIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);		// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	//隐藏进度条
	m_ProgressReadFile.ShowWindow(SW_HIDE);
	
	//绘制画布
	DrawRectCanvas();
	//初始化单选按钮
	((CButton*)GetDlgItem(IDC_RADIO_MQ_METHOD))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_TRI_LINEAR_METHOD))->SetCheck(0);

	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMQIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{		
		//刷新
		if (DrawPointsFlag == false)DrawRectCanvas();
		if (DrawPointsFlag == true)
		{
			RefreshMapArea();
			if (BoundaryFlag == true && m_CheckMaxRectBoundary == false) DrawBoundary();
			if (m_CheckMaxRectBoundary == TRUE && BoundaryFlag == false) DrawMaxRectBoundary();
		}
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMQIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//绘制矩形
void CMQIDlg::DrawRectCanvas()
{	
	//获得静态框在屏幕中的位置;	
	GetDlgItem(IDC_STATIC_MAP_ZONE)->GetWindowRect(rc);
	ScreenToClient(&rc);
	xLeft = rc.left;
	yTop = rc.top;
	xRight = rc.right;
	yBottom = rc.bottom;

	CDC* pDC = GetDC();  //获得设备环境指针
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(0, 200, 200));	pDC->SelectObject(&Pen);
	//绘制矩形框
	pDC->Rectangle(xLeft, yTop, xRight, yBottom);
	ReleaseDC(pDC);
}
//双缓存绘制图形
void CMQIDlg::DrawPoints()
{
	int xl = xLeft + offset;
	int xr = xRight - offset;
	int yu = yTop + offset;
	int yd = yBottom - offset;	

	int xm = (xr + xl) / 2;
	int ym = (yd + yu) / 2;

	REAL XM = (XCMax + XCMin) / 2;
	REAL YM = (YCMax + YCMin) / 2;

	double ZCD = ZCMax - ZCMin;

	double xscale = (xr - xl) / (XCMax - XCMin);
	double yscale = (yd - yu) / (YCMax - YCMin);
	double scale = xscale > yscale ? yscale : xscale;
	//将实测属性值换算到指定范围
	for (int i = 0; i < ColorNum; i++)
	{
		PropertyValue[i] = (REAL)(ZCMin + Data[i] * (ZCMax - ZCMin) / (Data[ColorNum - 1] - Data[0]));
	}
	double area = fabs((xr - xl) * (yd - yu));
	double pn = PropertyData.size() / area;
	int radius = 2;
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	CDC* pDC = GetDC();	
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, xRight, yBottom);	
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(&MemBitmap);	
	//绘制矩形框
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(0, 200, 200));	
	MemDC.SelectObject(&Pen);	
	MemDC.Rectangle(xLeft, yTop, xRight, yBottom);	
	//绘制像素	
	if (pn > 0.2)
	{		
		int step = PropertyData.size() / 50000;
		if (step == 0)step = 1;		

		for (int i = 0; i < PropertyData.size(); i++)
		{		
			if (i % step != 0)continue;//

			int x = int(xm + (PropertyData[i].x - XM) * scale);
			int y = int(ym - (PropertyData[i].y - YM) * scale);

			int r, g, b;
			GetMapColor(PropertyValue, PropertyData[i].z, r, g, b);
			MemDC.SetPixel(x, y, RGB(r, g, b));
		}
	}
	else
	{
		for (int i = 0; i < PropertyData.size(); i++)
		{
			int x = int(xm + (PropertyData[i].x - XM) * scale);
			int y = int(ym - (PropertyData[i].y - YM) * scale);

			int r, g, b;
			GetMapColor(PropertyValue, PropertyData[i].z, r, g, b);

			CPen pPen;
			pPen.CreatePen(PS_SOLID, 1, RGB(r, g, b));
			MemDC.SelectObject(&pPen);
			MemDC.Ellipse(x - radius, y - radius, x + radius, y + radius);		
		}
	}
	pDC->BitBlt(xLeft, yTop, xRight - xLeft, yBottom - yTop, &MemDC, xLeft, yTop, SRCCOPY);
	ReleaseDC(pDC);	
	MemBitmap.DeleteObject();
	DrawPointsFlag = true;
}
//刷新绘图区
void CMQIDlg::RefreshMapArea()
{
	CDC* pDC = GetDC();
	//绘制位图
	pDC->BitBlt(xLeft, yTop, xRight - xLeft, yBottom - yTop, &MemDC, xLeft, yTop, SRCCOPY);
	ReleaseDC(pDC);
}
//获得该属性值对应的颜色
void CMQIDlg::GetMapColor(double* PropertyValue,double& pv, int& rv, int& gv, int& bv)
{		
	//pv <= the minimum of ColorValue
	if (pv <= PropertyValue[0])
	{
		rv = Red[0];
		gv = Green[0];
		bv = Blue[0];
	}
	//pv >= the maximum of ColorValue
	else if (pv >= PropertyValue[ColorNum - 1])
	{
		rv = Red[ColorNum - 1];
		gv = Green[ColorNum - 1];
		bv = Blue[ColorNum - 1];
	}
	//在最小和最大色标值之间
	else
	{
		for (int i = 0; i < ColorNum - 1; i++)
		{
			if (pv >= PropertyValue[i] && pv < PropertyValue[i + 1])
			{
				double f = (pv - PropertyValue[i]) / (PropertyValue[i + 1] - PropertyValue[i]);
				rv = (int)(Red[i] * (1 - f) + Red[i + 1] * f);
				gv = (int)(Green[i] * (1 - f) + Green[i + 1] * f);
				bv = (int)(Blue[i] * (1 - f) + Blue[i + 1] * f);
				break;
			}
		}
	}
}
//=======================================================//
//函数名称：split()                                      //
//函数目的：字符串分割函数                               //
//参数说明：instr：读取的字符串                          //
//         outstr：分割后的字符串                        //
//		  pattern：字符串间的字符                        //
//程序作者：刘海飞，2021.8.20                            //
//=======================================================//
void CMQIDlg::split(string& instr, vector<string>& outstr, const string& pattern)
{
	size_t strSize = instr.size();
	size_t patternSize = pattern.size();

	string strtmp = "";
	for (int i = 0; i < strSize; i++)
	{
		bool flag = true;
		for (int j = 0; j < patternSize; j++)
		{
			if (instr[i] == pattern[j])
			{
				flag = false;
				break;
			}
		}
		if (flag == true)strtmp += instr[i];
		else
		{
			if (strtmp.size()) outstr.push_back(strtmp);
			strtmp = "";
		}
	}
	if (strtmp.size()) outstr.push_back(strtmp);
}

//读取XYZ三列数据，x，y为平面坐标，z为属性值
//程序作者：刘海飞，2021.8.20                            //
void CMQIDlg::OnClickedButtonLoadData()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szFilter("Data Files(*.dat)|*.dat|All Files(*.*)|*.*|");
	CFileDialog openFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	openFile.m_ofn.lpstrTitle = _T(" 导入XYZ数据 ");

	((CButton*)GetDlgItem(IDC_CHECK_MAX_RECT))->SetCheck(FALSE);
	m_CheckMaxRectBoundary = FALSE;
	((CButton*)GetDlgItem(IDC_BUTTON_SEARCH_BOUNDARY))->EnableWindow(TRUE);

	if (openFile.DoModal() == IDOK)
	{
		//将原有数组清空
		PropertyData.clear();PropertyData.shrink_to_fit();
		pp.clear(); pp.shrink_to_fit();
		ANum.clear(); ANum.shrink_to_fit();
		TriFlag.clear(); TriFlag.shrink_to_fit();
	
		BoundaryFlag = FALSE;
		///////////////////////////////////////////////////////////////////////////////////
		CString m_ReadFileName = openFile.GetPathName();		
		FileName = m_ReadFileName.Left(m_ReadFileName.ReverseFind('.'));	
		///////////////////////////////////////////////////////////////////////////////		
		ifstream infile;
		infile.open(m_ReadFileName, ios::in);

		const string pattern = " \t\r,	";
		vector <string> outstr;
		struct PointsXYZ temp {};				
		//////////////////////////////////////////////////////////////////////////////
		m_ProgressReadFile.SetRange(0, 100);
		m_ProgressReadFile.ShowWindow(SW_SHOW);		
		
		//开始读文件
		m_OutputCalInfo = "加载数据...";
		ShowCalInfo();
		SendNewMessage();

		XCMax = -DBL_MAX;
		XCMin = DBL_MAX;
		YCMax = -DBL_MAX;
		YCMin = DBL_MAX;
		ZCMax = -DBL_MAX;
		ZCMin = DBL_MAX;
		int ii = 0;
		while (infile.peek() != EOF)
		{
			string instr = "";
			std::getline(infile, instr);
			//分割字符串
			split(instr, outstr, pattern);
			if (outstr.size() != 3)
			{
				outstr.clear();
				continue;
			}
			//将读取的数据x/y/z压栈
			temp.x = atof(outstr[0].c_str());
			temp.y = atof(outstr[1].c_str());
			temp.z = atof(outstr[2].c_str());
			PropertyData.push_back(temp);
			//寻找坐标区间
			if (temp.x < XCMin)XCMin = temp.x;
			else if (temp.x > XCMax) XCMax = temp.x;
			if (temp.y < YCMin)YCMin = temp.y;
			else if (temp.y > YCMax) YCMax = temp.y;
			if (temp.z < ZCMin)ZCMin = temp.z;
			else if (temp.z > ZCMax) ZCMax = temp.z;
			//清除字符串
			outstr.clear();
			ii += 1;
		}
		m_ProgressReadFile.SetPos(50);
		outstr.shrink_to_fit();
		infile.close();
		UpdateData(FALSE);
		SendNewMessage();
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//判别是否正确读取文件		
		if (PropertyData.size() < 3)
		{
			PropertyDataFlag = FALSE;
			MessageBox(_T("数据个数小于3！"), _T("警告"), MB_OK);
			return;
		}
		else if (XCMax - XCMin < 1e-3 || YCMax - YCMin < 1e-3)
		{
			PropertyDataFlag = FALSE;
			MessageBox(_T("数据在一条直线上！"), _T("警告"), MB_OK);
			return;
		}
		else
		{
			PropertyDataFlag = TRUE;
		}
	}
	DrawPointsFlag = false;	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//三角形剖分
	m_OutputCalInfo = "构建三角网...";
	ShowCalInfo();
	SendNewMessage();

	if (PropertyDataFlag == TRUE)TriangulationOfDiscretePoints(tio_in, tio_out, PropertyData);
	m_ProgressReadFile.SetPos(60);


	//点的平均距离
	m_OutputCalInfo = "计算平均点距...";
	ShowCalInfo();	
	SendNewMessage();	

	REAL PointAverDisTmp = 0;

	int i, i2, i22, i12;
	double x21, y21;
#pragma omp parallel for private(i,i2,i22,i12,x21,y21)  reduction(+:PointAverDisTmp) 
	for (i = 0; i < tio_out.numberofedges; i++)
	{		
		i2 = i * 2;
		i22 = tio_out.edgelist[i2] * 2;
		i12 = tio_out.edgelist[i2 + 1] * 2;
		x21 = tio_out.pointlist[i22] - tio_out.pointlist[i12];
		y21 = tio_out.pointlist[i22 + 1] - tio_out.pointlist[i12 + 1];
		
		PointAverDisTmp += sqrt(x21 * x21 + y21 * y21);	
	}
	PointAverDis = PointAverDisTmp / tio_out.numberofedges;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	m_XMin.Format(_T("%2.5f"), XCMin);
	m_XMax.Format(_T("%2.5f"), XCMax);
	m_YMin.Format(_T("%2.5f"), YCMin);
	m_YMax.Format(_T("%2.5f"), YCMax);
	m_ZMin.Format(_T("%2.5f"), ZCMin);
	m_ZMax.Format(_T("%2.5f"), ZCMax);
	DataPointAverDis.Format(_T("%2.5f"), PointAverDis);
	DataTotalNum.Format(_T("%2d"), tio_out.numberofpoints);

	m_BoundaryMaxDist.Format(_T("%2.5f"), PointAverDis * 10);
	
	m_MaxPointsNumofMQSurface.Format(_T("%2d"), 1000); //小于1000不参与搜索，可以设置
	m_RecDepth.Format(_T("%2d"), 5); //递归深度默认为5，范围为5--10

	m_XEleNum.Format(_T("%2d"), 100);
	m_YEleNum.Format(_T("%2d"), 100);
	m_XEleSize.Format(_T("%2.5f"), (XCMax - XCMin) / 100);
	m_YEleSize.Format(_T("%2.5f"), (YCMax - YCMin) / 100);

	UpdateData(FALSE);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_ProgressReadFile.SetPos(80);
	m_OutputCalInfo = "绘制像素点...";
	ShowCalInfo();	
	SendNewMessage();

	DrawPoints();
	m_ProgressReadFile.SetPos(100);
	m_ProgressReadFile.ShowWindow(SW_HIDE);		
	//提示
	MessageBox(_T("数据加载成功"), _T("警告"), MB_OK);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//算法部分
//=======================================================//
//函数名称：tio_init()                                   //
//函数目的：初始化triang.c的结构体成员                   //
//=======================================================//
void CMQIDlg::tio_init(struct triangulateio* tio)
{
	tio->pointlist = (REAL*)NULL;
	tio->pointattributelist = (REAL*)NULL;
	tio->pointmarkerlist = (int*)NULL;
	tio->numberofpoints = 0;
	tio->numberofpointattributes = 0;
	tio->trianglelist = (int*)NULL;
	tio->triangleattributelist = (REAL*)NULL;
	tio->trianglearealist = (REAL*)NULL;
	tio->neighborlist = (int*)NULL;
	tio->numberoftriangles = 0;
	tio->numberofcorners = 3;
	tio->numberoftriangleattributes = 0;
	tio->segmentlist = (int*)NULL;
	tio->segmentmarkerlist = (int*)NULL;
	tio->numberofsegments = 0;
	tio->holelist = (REAL*)NULL;
	tio->numberofholes = 0;
	tio->regionlist = (REAL*)NULL;
	tio->numberofregions = 0;
	tio->edgelist = (int*)NULL;
	tio->edgemarkerlist = (int*)NULL;
	tio->normlist = (REAL*)NULL;
	tio->numberofedges = 0;
}
//=======================================================//
//函数名称：tio_destroy()                                //
//函数目的：释放triang.c的结构体成员                     //
//=======================================================//
void CMQIDlg::tio_destroy(struct triangulateio* tio)
{
	if (tio->pointlist != NULL)
		delete[]tio->pointlist;

	if (tio->pointattributelist != NULL)
		delete[]tio->pointattributelist;

	if (tio->pointmarkerlist != NULL)
		delete[]tio->pointmarkerlist;

	if (tio->trianglelist != NULL)
		delete[]tio->trianglelist;

	if (tio->triangleattributelist != NULL)
		delete[]tio->triangleattributelist;

	if (tio->trianglearealist != NULL)
		delete[]tio->trianglearealist;

	if (tio->neighborlist != NULL)
		delete[]tio->neighborlist;

	if (tio->segmentlist != NULL)
		delete[]tio->segmentlist;

	if (tio->segmentmarkerlist != NULL)
		delete[]tio->segmentmarkerlist;

	if (tio->holelist != NULL)
		delete[]tio->holelist;

	if (tio->regionlist != NULL)
		delete[]tio->regionlist;

	if (tio->edgelist != NULL)
		delete[]tio->edgelist;

	if (tio->edgemarkerlist != NULL)
		delete[]tio->edgemarkerlist;

	if (tio->normlist != NULL)
		delete[]tio->normlist;
}
//=======================================================//
//函数名称：onrightside()                                //
//函数目的：判别点p是否在向量p12的右侧                   //
//=======================================================//
int CMQIDlg::onrightside(PointsXY* p, PointsXY* p1, PointsXY* p2)
{
	return (p2->x - p->x) * (p1->y - p->y) > (p1->x - p->x) * (p2->y - p->y);
}
//=======================================================//
//函数名称：TriangulationOfDiscretePoints()              //
//函数目的：根据平面离散点数据,生成三角网                //
//参数说明： tio_in：三角网的输入结构体变量              //
//          tio_out：三角网的输出结构体变量              //
//              xyz：平面离散点x,y,z                     //
//程序作者：刘海飞，2021.8.20                            //
//=======================================================//
void CMQIDlg::TriangulationOfDiscretePoints(triangulateio &tio_in, triangulateio &tio_out, vector<PointsXYZ>& xyz)
{
	//初始化结构体成员
	tio_init(&tio_in);
	tio_init(&tio_out);

	//初始化离散点的个数和坐标
	tio_in.numberofpoints = (int)xyz.size();
	if (tio_in.numberofpoints < 3)
	{		
		MessageBox(_T("数据个数小于3！"), _T("警告"), MB_OK);
		return;
	}
	tio_in.pointlist = new REAL[tio_in.numberofpoints * 2];
	//初始化点数组
#pragma omp parallel for
	for (int i = 0; i < tio_in.numberofpoints; i++)
	{
		tio_in.pointlist[i * 2] = xyz[i].x;
		tio_in.pointlist[i * 2 + 1] = xyz[i].y;
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//将离散点剖分成三角形		
	char str[10];
	strcpy_s(str, "pznce");
	triangulate(str, &tio_in, &tio_out, (struct triangulateio*)NULL);
}
//=======================================================//
//函数名称：SearchBoundary()                             //
//函数目的：搜索离散点的凹包边界                         //
//参数说明： tio_in：三角网的输入结构体变量              //
//          tio_out：三角网的输出结构体变量              //
//              xyz：平面离散点x,y,z                     //
//程序作者：刘海飞，2021.8.20                            //
//=======================================================//
void CMQIDlg::SearchBoundary(triangulateio& tio_out, vector<PointsXYZ>& xyz)
{
	/////////////////////////////////////////////////////////////////////////////////////////
	REAL MaxBouDist = (REAL)atof(m_BoundaryMaxDist.GetBuffer());	
	if (MaxBouDist < PointAverDis * 1)
	{
		MessageBox(_T("外边界最小限定边长应大于3陪的平均点距！"), _T("警告"), MB_OK);
		MaxBouDist = PointAverDis * 1;
		m_BoundaryMaxDist.Format(_T("%2.2f"), MaxBouDist);		
		UpdateData(FALSE);	
		return;
	}	
	/////////////////////////////////////////////////////////////////////////////////////////
	//任一三角形的3个相临三角形的编号
	vector< triangle_neighbours > NeighboursArray;
	NeighboursArray.resize(tio_out.numberoftriangles);
	TriFlag.resize(tio_out.numberoftriangles);
	/////////////////////////////////////////////////////////////////////////////////////////
	//将三角形的节点和临三角形编号存到结构体中
	vector<int> PointTri;
	PointTri.resize(tio_out.numberofpoints, 0);	//一个点所共用的三角形个数
	//确定三角网的初始边界
	vector <int> BoundaryTriNumber, boundarymarker;
	for (int i = 0; i < tio_out.numberoftriangles; i++)
	{
		int i3 = i * 3;
		NeighboursArray[i].tids[0] = tio_out.neighborlist[i3];
		NeighboursArray[i].tids[1] = tio_out.neighborlist[i3 + 1];
		NeighboursArray[i].tids[2] = tio_out.neighborlist[i3 + 2];
		TriFlag[i] = i;
		for (int j = 0; j < 3; j++)
		{
			PointTri[tio_out.trianglelist[i3 + j]] += 1;
			if (NeighboursArray[i].tids[j] < 0)
			{
				BoundaryTriNumber.push_back(i);
				boundarymarker.push_back(0);
			}
		}
	}	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//从边界向内部缩放，删除边较大地三角形	
	int M = 5;
	while (true)
	{
		bool flag = false;
		for (unsigned int i = 0; i < BoundaryTriNumber.size(); i++)//边界三角形
		{
			//这个三角形的边界边长小于指定距离
			if (boundarymarker[i] == 1)continue;
			//判别边界三角形的邻居三角形的个数(即三角形的边在边界上的条数)
			int jj = 0;
			for (int j = 0; j < 3; j++)if (NeighboursArray[BoundaryTriNumber[i]].tids[j] < 0)jj++;
			//////////////////////////////////////////////////////////////////////////////////////
			//边界三角形只有一条边在边界上
			if (jj == 1)
			{
				for (int j = 0; j < 3; j++)
				{
					//边界三角形boundary[i]的邻居为-1，即这个边为边界
					if (NeighboursArray[BoundaryTriNumber[i]].tids[j] < 0)
					{
						///////////////////////////////////////////////////////////////////////////
						//boundary[i]临边界的2个点
						int k12 = tio_out.trianglelist[BoundaryTriNumber[i] * 3 + (j + 1) % 3] * 2;
						int k22 = tio_out.trianglelist[BoundaryTriNumber[i] * 3 + (j + 2) % 3] * 2;
					
						//计算三边的边长
						REAL dx = tio_out.pointlist[k12] - tio_out.pointlist[k22];
						REAL dy = tio_out.pointlist[k12 + 1] - tio_out.pointlist[k22 + 1];
						REAL d = sqrt(dx * dx + dy * dy);
						
						if (d > MaxBouDist * M)
						{
							//2个邻居三角形的边将成为边界
							int k1 = NeighboursArray[BoundaryTriNumber[i]].tids[(j + 1) % 3];
							int k2 = NeighboursArray[BoundaryTriNumber[i]].tids[(j + 2) % 3];

							//增加边界三角形k1,k2
							BoundaryTriNumber.push_back(k1);
							boundarymarker.push_back(0);

							BoundaryTriNumber.push_back(k2);
							boundarymarker.push_back(0);
							//
							for (int k = 0; k < 3; k++)
							{
								if (NeighboursArray[k1].tids[k] == BoundaryTriNumber[i])NeighboursArray[k1].tids[k] = -1;
								if (NeighboursArray[k2].tids[k] == BoundaryTriNumber[i])NeighboursArray[k2].tids[k] = -1;

								PointTri[tio_out.trianglelist[BoundaryTriNumber[i] * 3 + k]] -= 1;
							}
							TriFlag[BoundaryTriNumber[i]] = -1;  //三角形在边界外
							TriFlag[k1] = -2;  //k1为边界三角形
							TriFlag[k2] = -2;  //k2为边界三角形
							//删除边界三角形
							BoundaryTriNumber.erase(BoundaryTriNumber.begin() + i);
							boundarymarker.erase(boundarymarker.begin() + i);
						}
						else
						{
							boundarymarker[i] = 1;
						}
						break;
					}
				}
			}
			else if (jj == 2)//该三角形有两个临界三角形，如果比较狭长，则删除该三角形
			{
				///////////////////////////////////////////////////////////////////////////////////
				//确定三角形的最长边和最短边
				REAL dmin = DBL_MAX, dmax = -DBL_MAX;
				int jmin = 0, ik;
				for (int j = 0; j < 3; j++)
				{
					//该三角形非临界边所对的顶点ik
					if (NeighboursArray[BoundaryTriNumber[i]].tids[j] == -1)
					{
						//三角形顶点j对应的边k1k2
						int k1 = tio_out.trianglelist[BoundaryTriNumber[i] * 3 + (j + 1) % 3];
						int k2 = tio_out.trianglelist[BoundaryTriNumber[i] * 3 + (j + 2) % 3];

						REAL dx = tio_out.pointlist[k1 * 2] - tio_out.pointlist[k2 * 2];
						REAL dy = tio_out.pointlist[k1 * 2 + 1] - tio_out.pointlist[k2 * 2 + 1];
						REAL d = sqrt(dx * dx + dy * dy);					
						//记录最长边
						if (d > dmax)dmax = d;
					}
					else
					{
						ik = j;
					}
				}
				//将边长比大于10,三角形顶点ik联系的三角形数大于1，顶点ik正对三角形短边，则删除该三角形						
				if (((dmax > MaxBouDist * M) && PointTri[tio_out.trianglelist[BoundaryTriNumber[i] * 3 + ik]] > 1))
				{
					//三角形顶点ik对应的邻居三角形作为边界三角形
					int k1 = NeighboursArray[BoundaryTriNumber[i]].tids[ik];

					BoundaryTriNumber.push_back(k1);
					boundarymarker.push_back(0);

					for (int k = 0; k < 3; k++)
					{
						if (NeighboursArray[k1].tids[k] == BoundaryTriNumber[i])
						{
							NeighboursArray[k1].tids[k] = -1;
							break;
						}
						PointTri[tio_out.trianglelist[BoundaryTriNumber[i] * 3 + k]] -= 1;
					}
					TriFlag[BoundaryTriNumber[i]] = -1;
					TriFlag[k1] = -2;  //k1为边界三角形

					BoundaryTriNumber.erase(BoundaryTriNumber.begin() + i);
					boundarymarker.erase(boundarymarker.begin() + i);
				}
				else
				{
					boundarymarker[i] = 1;
				}
			}
			else
			{
				boundarymarker[i] = 1;
			}
			flag = true;
		}		
		
		if (flag == false)
		{	
			for (unsigned int i = 0; i < BoundaryTriNumber.size(); i++)boundarymarker[i] = 0;
			M--;
			if (M == 0) break;
		}		
	}	
	//根据搜索的边界三角形，将边界边放入容器pp中		
	for (unsigned int i = 0; i < BoundaryTriNumber.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (NeighboursArray[BoundaryTriNumber[i]].tids[j] < 0)
			{
				int k1 = tio_out.trianglelist[BoundaryTriNumber[i] * 3 + (j + 1) % 3];
				int k2 = tio_out.trianglelist[BoundaryTriNumber[i] * 3 + (j + 2) % 3];
								
				number temp;
				temp.ni = k1;
				temp.nj = k2;
				pp.push_back(temp);
			}
		}
	}
	//删除重复的线段	
	for (unsigned int i = 0; i < pp.size() - 1; i++)
	{
		for (unsigned j = i + 1; j < pp.size(); j++)
		{
			if (pp[i].ni == pp[j].ni && pp[i].nj == pp[j].nj)
			{
				pp.erase(pp.begin() + j);
				j--;
			}
		}
	}	
	/////////////////////////////////////////////////////////////////////////////////////		
	//将线段连接成封闭区域
	int ik = 0;
	ANum.push_back(ik);
	for (unsigned i = 0; i < pp.size() - 1; i++)
	{
		//搜索无序序列最小元素的索引k
		unsigned int k = i + 1;
		bool flag = false;
		for (unsigned j = i + 1; j < pp.size(); j++)
		{
			if (pp[i].nj == pp[j].ni)
			{
				flag = true;
				if (j != k)swap(pp[k], pp[j]);
				break;
			}
		}
		//没找到相邻线段，判别是否形成封闭圈
		if (flag == false)
		{
			if (pp[i].nj == pp[ik].ni)
			{
				if (i - ik + 1 >= 3)//线段数>=3时，为封闭区域
				{
					ik = i + 1;
					ANum.push_back(ik);

				//	outfile4 << ik << endl;
				}
			}
		}
	}
	ANum.push_back(pp.size());
	///////////////////////////////////////////////////////////////////////////////////////////
	//输出边界文件
	CString str = FileName + "_Boundary.bln";
	ofstream outfile;
	outfile.open(str, ios::out);
	for (unsigned i = 0; i < ANum.size() - 1; i++)
	{
		outfile << ANum[i + 1] - ANum[i] + 1 << setw(10) << 0 << endl;
		for (size_t j = ANum[i]; j < ANum[i + 1]; j++)
		{
			outfile << setiosflags(ios::fixed) << setprecision(5);
			outfile << tio_out.pointlist[pp[j].ni * 2] << setw(15) << tio_out.pointlist[pp[j].ni * 2 + 1] << endl;
			if (j == ANum[i + 1] - 1)
			{
				outfile << tio_out.pointlist[pp[ANum[i]].ni * 2] << setw(15) << tio_out.pointlist[pp[ANum[i]].ni * 2 + 1] << endl;
			}				
		}
	}
	MessageBox(_T("离散点边界已保存到文件: \n" + str), _T("提示"), MB_OK);	
	/////////////////////////////////////////////////////////////////////////////////////
	//释放内存
	PointTri.clear(); PointTri.shrink_to_fit();
	BoundaryTriNumber.clear(), BoundaryTriNumber.shrink_to_fit();
	boundarymarker.clear(), boundarymarker.shrink_to_fit();	
	NeighboursArray.clear(), NeighboursArray.shrink_to_fit();
}
//搜索离散点边界
//程序作者：刘海飞，2021.8                            //
void CMQIDlg::OnClickedButtonSearchBoundary()
{
	if (PropertyDataFlag == FALSE)
	{
		MessageBox(_T("请先读取数据文件！"), _T("警告"), MB_OK);
		return;
	}
	// 通知处理程序代码
	UpdateData(TRUE);
	//释放内存
	pp.clear(); pp.shrink_to_fit();
	ANum.clear(); ANum.shrink_to_fit();	
	//开始搜索边界
	SearchBoundary(tio_out, PropertyData);
	//重置底图
	RefreshMapArea();
	//绘制边界
	DrawBoundary();
	//已有边界数据
	BoundaryFlag = TRUE;
}
//绘制边界
//程序作者：刘海飞，2021.8                            //
void CMQIDlg::DrawBoundary()
{
	int xl = xLeft + offset;
	int xr = xRight - offset;
	int yu = yTop + offset;
	int yd = yBottom - offset;

	int xm = (xr + xl) / 2;
	int ym = (yd + yu) / 2;

	REAL XM = (XCMax + XCMin) / 2;
	REAL YM = (YCMax + YCMin) / 2;

	double ZCD = ZCMax - ZCMin;

	double xscale = (xr - xl) / (XCMax - XCMin);
	double yscale = (yd - yu) / (YCMax - YCMin);
	double scale = xscale > yscale ? yscale : xscale;	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	CDC* pDC = GetDC();

	//绘制边界
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pDC->SelectObject(&Pen);	
	
	for (int i = 0; i < pp.size(); i++)
	{
		int x, y;
		x = int(xm + (tio_out.pointlist[pp[i].ni * 2] - XM) * scale);
		y = int(ym - (tio_out.pointlist[pp[i].ni * 2 + 1] - YM) * scale);
		pDC->MoveTo(x, y);
		x = int(xm + (tio_out.pointlist[pp[i].nj * 2] - XM) * scale);
		y = int(ym - (tio_out.pointlist[pp[i].nj * 2 + 1] - YM) * scale);
		pDC->LineTo(x, y);		
	}

	ReleaseDC(pDC);	
}
//程序作者：刘海飞，2021.10                            //
//MQ插值法
void CMQIDlg::OnRadioMQInterMethod()
{
	// TODO: 在此添加命令处理程序代码
	GriddingMethod = false;
	((CButton*)GetDlgItem(IDC_RADIO_MQ_METHOD))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_TRI_LINEAR_METHOD))->SetCheck(0);	
}

//三角形线性插值法
void CMQIDlg::OnRadioTriLinearInterMethod()
{
	// TODO: 在此添加命令处理程序代码
	GriddingMethod = true;
	((CButton*)GetDlgItem(IDC_RADIO_MQ_METHOD))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_TRI_LINEAR_METHOD))->SetCheck(1);
}
//程序作者：刘海飞，2021.10                            //
//改变X方向网格单元数
void CMQIDlg::OnChangeEditXElementNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (PropertyDataFlag == FALSE)return;	
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int XNum = atoi(m_XEleNum.GetBuffer());
	if (XNum <= 0)
	{
		m_XEleSize = "";
		MessageBox(_T("单元数应大于1！"), _T("警告"), MB_OK);
	}
	else
	{
		REAL XSize = (XCMax - XCMin) / atoi(m_XEleNum.GetBuffer());
		if (XSize < 1e-5)
		{
			MessageBox(_T("单元长度应大于1E-4！"), _T("警告"), MB_OK);
			m_XEleNum.Format(_T("%2d"), 100);
			m_XEleSize.Format(_T("%2.5f"), (XCMax - XCMin) / 100);

		}
		else m_XEleSize.Format(_T("%2.5f"), XSize);
	}	
	UpdateData(FALSE);	
}

//改变Y方向网格单元数
void CMQIDlg::OnChangeEditYElementNum()
{	
	if (PropertyDataFlag == FALSE)return;	
	// 通知处理程序代码
	UpdateData(TRUE);
	int YNum = atoi(m_YEleNum.GetBuffer());
	if (YNum <= 0)
	{
		m_YEleSize = "";
		MessageBox(_T("单元数应大于1！"), _T("警告"), MB_OK);
	}
	else
	{
		REAL YSize = (YCMax - YCMin) / atoi(m_YEleNum.GetBuffer());
		if (YSize < 1e-5)
		{
			MessageBox(_T("单元长度应大于1E-4！"), _T("警告"), MB_OK);
			m_YEleNum.Format(_T("%2d"), 100);
			m_YEleSize.Format(_T("%2.5f"), (YCMax - YCMin) / 100);
			
		}
		else  m_YEleSize.Format(_T("%2.5f"), YSize);
	}
	UpdateData(FALSE);
}

//全局MQ曲面的最大数据点的限制数
void CMQIDlg::OnChangeEditGlobalMqDataMaxNum()
{	
	if (PropertyDataFlag == FALSE)return;
	// TODO:  在此添加控件通知处理程序代码
	
	UpdateData(TRUE);
	int Num = atoi(m_MaxPointsNumofMQSurface.GetBuffer());	
	if (Num < 1)
	{		
		MessageBox(_T("全局曲面点个数应在< 100--100000 >范围内！"), _T("警告"), MB_OK);
		m_MaxPointsNumofMQSurface.Format(_T("%2d"), 1000);
	}	
	UpdateData(FALSE);
}
//设置递归搜索深度
void CMQIDlg::OnChangeEditRecDepth()
{	
	if (PropertyDataFlag == FALSE)return;
	UpdateData(TRUE);
	int Num = atoi(m_RecDepth.GetBuffer());
	if (Num < 3 || Num > 9)
	{
		MessageBox(_T("递归搜索深度应在< 3--9 >范围内！"), _T("警告"), MB_OK);
		m_RecDepth.Format(_T("%d"), 5);
	}
	UpdateData(FALSE);
}

//=======================================================//
//函数名称：LSCG()                                       //
//函数目的：利用共轭梯度法解线性方程组                   //
//参数说明： a: 方程组的系数矩阵                         //
//           b: 方程组的右端项                           //
//           x: 解向量                                   //
//           n: 解的个数                                 //
// 程序作者：刘海飞，2021.8                              //
//=======================================================//
void CMQIDlg::LSCG(REAL* A, REAL* x, REAL* B, const int n, REAL damp)
{
	REAL EPS = 1.0e-10 * damp;         //终止条件		
	///////////////////////////////////////////////////////
	//给向量赋初值
	REAL* h = new REAL[n];
	REAL* g = new REAL[n];
	REAL* p = new REAL[n];
	REAL* Ap = new REAL[n];
	///////////////////////////////////////////////////////
	//给定迭代的初始参数		
	REAL g1 = 0;
	for (int i = 0; i < n; i++)
	{
		h[i] = B[i]; //h(0)的初值	
		g[i] = 0;    //g(0)
		for (int j = 0; j < n; j++) g[i] += A[j * n + i] * B[j];  //At*h(0) 		
		p[i] = g[i];                //p(0) 
		x[i] = 0.0;                 //x(0)
		g1 += g[i] * g[i];          //g(0)*g(0) 		
	}
	REAL g0 = g1;	
	///////////////////////////////////////////////////////
	//开始迭代	
	int k = 1;
	while (k <= 500)
	{
		//(p,dp)+(AP,AP)
		REAL sum = 0;
		for (int i = 0; i < n; i++)
		{
			sum += p[i] * p[i] * damp;//(p,d*p)
			Ap[i] = 0;
			for (int j = 0; j < n; j++) Ap[i] += A[i * n + j] * p[j];//A*P
			sum += Ap[i] * Ap[i];
		}
		if (sum <= EPS) break;
		//alpha=(g,g)/[(Ap,Ap)+(p,dp)]
		REAL alpha = g1 / sum;
		//x(j+1)=x(j)+alpha*p(j)	
		//h(j+1)=h(j)-alpha*A*p			
		for (int i = 0; i < n; i++)
		{
			x[i] += alpha * p[i];
			h[i] -= alpha * Ap[i];
		}
		REAL g2 = 0;
		for (int i = 0; i < n; i++)
		{
			//y(j+1)=d*x(j+1)
			g[i] = -damp * x[i];
			//AT*h  //g(j+1)=AT*h-y(j+1)

			for (int j = 0; j < n; j++) g[i] += A[j * n + i] * h[j];
			//[g(j+1),g(j+1)]              
			g2 += g[i] * g[i];
		}		
		if (g2 / g0 <= EPS)break;	//终止条件			
		else
		{
			//Beta=(g(j+1),g(j+1))/(g(j),g(j))		
			REAL Beta = g2 / g1;
			//p(j+1)=g(j+1)+Beta*p(j)
			for (int i = 0; i < n; i++) p[i] = g[i] + Beta * p[i];
			g1 = g2;
		}		
		k += 1;
	}
	delete[]h; delete[]g; delete[]p; delete[]Ap;
}
//=======================================================//
//函数名称：GlobalLSCGLSCG()                             //
//函数目的：利用共轭梯度法解线性方程组-并行              //
//参数说明： a: 方程组的系数矩阵                         //
//           b: 方程组的右端项                           //
//           x: 解向量                                   //
//           n: 解的个数                                 //
// 程序作者：刘海飞，张宇豪，2021-2023.11                //
//=======================================================//
void CMQIDlg::GlobalLSCG(vector<REAL>& A, vector<REAL>& x, vector<REAL>& B, const int n, REAL damp)
{	
	REAL EPS = 1.0e-10 * damp;         //终止条件		
	///////////////////////////////////////////////////////
	//给向量赋初值
	REAL* h = new REAL[n];
	REAL* g = new REAL[n];
	REAL* p = new REAL[n];
	REAL* Ap = new REAL[n];
	///////////////////////////////////////////////////////
	//给定迭代的初始参数
	int coresnum = omp_get_num_procs();
	omp_set_num_threads(coresnum * 2 - 1);

	int i = 0, j;
	REAL g1 = 0;
	REAL sum;
#pragma omp parallel for schedule(dynamic) reduction(+:g1) private(sum)
	for (i = 0; i < n; i++)
	{
		h[i] = B[i]; //h(0)的初值	
		sum = 0;    //g(0)
#pragma omp parallel for schedule(dynamic) reduction(+:sum)
		for (j = 0; j < n; j++) sum += A[j * n + i] * B[j];  //At*h(0) 
		g[i] = sum;
		p[i] = g[i];                //p(0) 
		x[i] = 0.0;                 //x(0)
		g1 += g[i] * g[i];          //g(0)*g(0) 		
	}
	REAL g0 = g1;
	///////////////////////////////////////////////////////
	//开始迭代	
	int k = 1;
	while (k <= 500)
	{
		//(p,dp)+(AP,AP)
		REAL g2 = 0,sum;
#pragma omp parallel for schedule(dynamic) reduction(+:g2) private(sum)
		for (int i = 0; i < n; i++)
		{
			g2 += p[i] * p[i] * damp;//(p,d*p)
			sum = 0;
#pragma omp parallel for schedule(dynamic) reduction(+:sum)
			for (int j = 0; j < n; j++)sum += A[i * n + j] * p[j];//A*P
			
			Ap[i] = sum;
			g2 += Ap[i] * Ap[i];
		}
		if (g2 <= EPS) break;
		//alpha=(g,g)/[(Ap,Ap)+(p,dp)]
		REAL alpha = g1 / g2;
		//x(j+1)=x(j)+alpha*p(j)	
		//h(j+1)=h(j)-alpha*A*p			
#pragma omp parallel for private(i) shared(n,x,p,h,Ap)  
		for (i = 0; i < n; i++)
		{
			x[i] += alpha * p[i];
			h[i] -= alpha * Ap[i];
		}
		g2 = 0;		
#pragma omp parallel for schedule(dynamic) reduction(+:g2) private(sum)
		for (i = 0; i < n; i++)
		{
			//y(j+1)=d*x(j+1)
			//g[i] = -damp * x[i];
			sum = -damp * x[i];
			//AT*h  //g(j+1)=AT*h-y(j+1)			
#pragma omp parallel for schedule(dynamic) reduction(+:sum)
			for (j = 0; j < n; j++) sum += A[j * n + i] * h[j];
			g[i] = sum;
			//[g(j+1),g(j+1)]              
			g2 += g[i] * g[i];
		}
		if (g2 / g0 <= EPS)break;	//终止条件			
		else
		{
			//Beta=(g(j+1),g(j+1))/(g(j),g(j))		
			REAL Beta = g2 / g1;
			//p(j+1)=g(j+1)+Beta*p(j)			
#pragma omp parallel for private(i) shared(n,p,g) 
			for (i = 0; i < n; i++) p[i] = g[i] + Beta * p[i];			
			
			g1 = g2;
		}
		if (k % 10 == 0)
		{
			m_OutputCalInfo.Format("迭代误差: %2.5e / %d", g2 / g0, k);
			ShowCalInfo();
			SendNewMessage();
		}
		k += 1;
	}
	delete[]h; delete[]g; delete[]p; delete[]Ap;
}
//=======================================================//
//函数名称：LocalLSCGLSCG()                              //
//函数目的：利用共轭梯度法解线性方程组-串行              //
//参数说明： a: 方程组的系数矩阵                         //
//           b: 方程组的右端项                           //
//           x: 解向量                                   //
//           n: 解的个数                                 //
// 程序作者：刘海飞，2021.8                              //
//=======================================================//
void CMQIDlg::LocalLSCG(vector<REAL>& A, vector<REAL>& x, vector<REAL>& B, const int n, REAL damp)
{
	REAL EPS = 1.0e-10 * damp;         //终止条件		
	///////////////////////////////////////////////////////
	//给向量赋初值
	vector<REAL>h, g, p, Ap;
	h.resize(n);
	g.resize(n);
	p.resize(n);
	Ap.resize(n);	
	///////////////////////////////////////////////////////
	//给定迭代的初始参数	
	int i, j;
	REAL g1 = 0;
	REAL sum;

	for (i = 0; i < n; i++)
	{
		h[i] = B[i]; //h(0)的初值	
		sum = 0;    //g(0)

		for (j = 0; j < n; j++) sum += A[j * n + i] * B[j];  //At*h(0) 
		g[i] = sum;
		p[i] = g[i];                //p(0) 
		x[i] = 0.0;                 //x(0)
		g1 += g[i] * g[i];          //g(0)*g(0) 		
	}
	REAL g0 = g1;
	///////////////////////////////////////////////////////
	//开始迭代	
	int k = 1;
	while (k <= 500)
	{
		//(p,dp)+(AP,AP)
		REAL g2 = 0, sum;

		for (int i = 0; i < n; i++)
		{
			g2 += p[i] * p[i] * damp;//(p,d*p)
			sum = 0;

			for (int j = 0; j < n; j++)sum += A[i * n + j] * p[j];//A*P

			Ap[i] = sum;
			g2 += Ap[i] * Ap[i];
		}
		if (g2 <= EPS) break;
		//alpha=(g,g)/[(Ap,Ap)+(p,dp)]
		REAL alpha = g1 / g2;
		//x(j+1)=x(j)+alpha*p(j)	
		//h(j+1)=h(j)-alpha*A*p			

		for (i = 0; i < n; i++)
		{
			x[i] += alpha * p[i];
			h[i] -= alpha * Ap[i];
		}
		g2 = 0;

		for (i = 0; i < n; i++)
		{
			//y(j+1)=d*x(j+1)
			//g[i] = -damp * x[i];
			sum = -damp * x[i];
			//AT*h  //g(j+1)=AT*h-y(j+1)			

			for (j = 0; j < n; j++) sum += A[j * n + i] * h[j];
			g[i] = sum;
			//[g(j+1),g(j+1)]              
			g2 += g[i] * g[i];
		}
		if (g2 / g0 <= EPS)break;	//终止条件			
		else
		{
			//Beta=(g(j+1),g(j+1))/(g(j),g(j))		
			REAL Beta = g2 / g1;
			//p(j+1)=g(j+1)+Beta*p(j)			

			for (i = 0; i < n; i++) p[i] = g[i] + Beta * p[i];

			g1 = g2;
		}		
		k += 1;
	}	
	vector<REAL>().swap(h);
	vector<REAL>().swap(g);
	vector<REAL>().swap(p);
	vector<REAL>().swap(Ap);
}
//=======================================================//
//函数名称：CalGlobalMultiQuadricCoef()                  //
//函数目的：计算全局多重二次曲面系数                     //
//参数说明：NeighberingPoints：已知点的x、y、z坐标       //
//                          m：邻近点的个数              //
//                          s：平滑系数                  //
//		                 Coef：多重二次曲面系数          //
// 程序作者：刘海飞，张宇豪，2021-2023.11                //
//=======================================================//
void CMQIDlg::CalGlobalMultiQuadricCoef(vector<PointsXYZ>& NeighberingPoints, size_t m, REAL s, vector<REAL>& Coef)
{
	int m1 = (int)m + 1;
	vector<REAL> a, b;
	a.resize(m1 * m1);
	b.resize(m1);	
	Coef.resize(m1);
	REAL damp = s / 100;
	//生成方程组的系数矩阵	
	int i = 0, j = 0;
	REAL xx = 0, yy = 0, temp = 0;
#pragma omp parallel for private(i,j,xx,yy,temp) 
	for (i = 0; i <= m; i++)
	{
		for (j = i; j <= m; j++)
		{
			if (i == j)
			{
				if (i < m)a[i * m1 + j] = sqrt(s);
				else      a[i * m1 + j] = 0.f;
			}
			else
			{
				if (j < m)
				{
					REAL xx = NeighberingPoints[j].x - NeighberingPoints[i].x;
					REAL yy = NeighberingPoints[j].y - NeighberingPoints[i].y;
					REAL temp = sqrt(xx * xx + yy * yy + s);
					a[i * m1 + j] = temp;
					a[j * m1 + i] = temp;
				}
				else
				{
					a[i * m1 + j] = 1.f;
					a[j * m1 + i] = 1.f;
				}
			}
		}
		if (i < m)b[i] = NeighberingPoints[i].z;
		else      b[i] = 0;
	}
	//解线性方程组，无法求解时返回		
	GlobalLSCG(a, Coef, b, m1, damp);	
	//释放内存
	vector<REAL>().swap(a);
	vector<REAL>().swap(b);
}
//=======================================================//
//函数名称：CalLocalMultiQuadricCoef()                  //
//函数目的：计算局部多重二次曲面系数                    //
//参数说明：NeighberingPoints：已知点的x、y、z坐标      //
//                          m：邻近点的个数             //
//                          s：平滑系数                 //
//		                 Coef：多重二次曲面系数         //
// 程序作者：刘海飞，2021.8                              //
//=======================================================//
void CMQIDlg::CalLocalMultiQuadricCoef(vector<PointsXYZ>& NeighberingPoints, size_t m, REAL s, vector<REAL>& Coef)
{
	int m1 = (int)(m + 1);
	vector<REAL> a, b;
	a.resize(m1 * m1);
	b.resize(m1);
	Coef.resize(m1);
	REAL damp = s / 100;
	//生成方程组的系数矩阵	
	int i, j;	
	for (i = 0; i <= m; i++)
	{
		for (j = i; j <= m; j++)
		{
			if (i == j)
			{
				if (i < m)a[i * m1 + j] = sqrt(s);
				else      a[i * m1 + j] = 0.f;
			}
			else
			{
				if (j < m)
				{
					REAL xx = NeighberingPoints[j].x - NeighberingPoints[i].x;
					REAL yy = NeighberingPoints[j].y - NeighberingPoints[i].y;
					REAL temp = sqrt(xx * xx + yy * yy + s);
					a[i * m1 + j] = temp;
					a[j * m1 + i] = temp;
				}
				else
				{
					a[i * m1 + j] = 1.f;
					a[j * m1 + i] = 1.f;
				}
			}
		}
		if (i < m)b[i] = NeighberingPoints[i].z;
		else      b[i] = 0;
	}
	//解线性方程组，无法求解时返回		
	LocalLSCG(a, Coef, b, m1, damp);
	//释放内存
	vector<REAL>().swap(a);
	vector<REAL>().swap(b);
}
//=======================================================//
//函数名称：GetNeighboringTriangles()                    //
//函数目的：获得以id三角形为中心的邻近三角形             //
//              RecDepth：搜索深度                       //
//参数说明：          id：中心三角形编号                 //
//               tio_out：所有三角形构成的三角网         //
//	NeighboringTriangles：临近三角形的编号               //
// 程序作者：刘海飞，2021.8                              //
//=======================================================//
void CMQIDlg::GetNeighboringTriangles(int RecDepth, int id, triangulateio& tio_out, vector<int>& NeighboringTriangles)
{
	NeighboringTriangles.push_back(id);
	if (RecDepth == 0)return;
	RecSearchNeighboringTriangles(RecDepth, id, tio_out, NeighboringTriangles);
}
//=======================================================//
//函数名称：RecSearchNeighboringTriangles()              //
//函数目的：通过递归向外搜索邻近三角形                   //
//参数说明：    RecDepth：搜索深度                       //
//				      ik：当前三角形编号                 //
//               tio_out：所有三角形构成的三角网         //
//  NeighboringTriangles：临近三角形的编号               //
// 程序作者：刘海飞，2021.8                              //
//=======================================================//
void CMQIDlg::RecSearchNeighboringTriangles(int RecDepth, int ik, triangulateio& tio_out, vector<int>& NeighboringTriangles)
{
	if (RecDepth == 0)return;
	int ik3 = ik * 3;
	for (int i = 0; i < 3; i++)
	{
		int id = tio_out.neighborlist[ik3 + i];		
		if (id >= 0)
		{
			if (find(NeighboringTriangles.begin(), NeighboringTriangles.end(), id) == NeighboringTriangles.end())
				NeighboringTriangles.push_back(id);
		}	
		else continue;
		RecSearchNeighboringTriangles(RecDepth - 1, id, tio_out, NeighboringTriangles);
	}
}
//发送消息
// 程序作者：刘海飞，2021.8      
void CMQIDlg::SendNewMessage()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}
//=======================================================//
//函数名称：OutputGriddingFile()                         //
//函数目的：输出网格化结果文件                           //
//参数说明：       xmin：最小x坐标                       //
//		           xmax：最大x坐标                       //
//                 ymin：最小y坐标                       //
//		           ymax：最大y坐标                       //
//                 zmin：最小z值                         //
//		           zmax：最大z值                         //
//            xNodesNum：x方向结点数                     //
//		      yNodesNum：y方向结点数                     //
//		  GridNodesData：网格化数据                      //
// 程序作者：刘海飞，2021.8                              //
//=======================================================//
void CMQIDlg::OutputGriddingFile(REAL xmin, REAL xmax, REAL ymin, REAL ymax,
	REAL zmin, REAL zmax, int xNodesNum, int yNodesNum, vector <PointsXYZ>& GridNodesData)
{
	m_ProgressReadFile.SetRange(0, 1000);
	m_ProgressReadFile.ShowWindow(SW_SHOW);
	
	int GridNodesNum = xNodesNum * yNodesNum;

	int step = GridNodesNum / 1000;
	if (step < 1)step = 1;
	int ik = 1;

	ofstream outgrid;
	CString str;
	if (GriddingMethod == false)str = FileName + "_MQ_Grid.grd";	
	else                        str = FileName + "_Tri_Grid.grd";
	
	outgrid.open(str, ios::out);
	outgrid << "DSAA" << endl;
	outgrid << xNodesNum << setw(15) << yNodesNum << endl;

	outgrid << xmin << setw(25) << xmax << endl;
	outgrid << ymin << setw(25) << ymax << endl;
	outgrid << zmin << setw(25) << zmax << endl;

	for (int i = 0; i < GridNodesNum; i++)
	{
		outgrid << GridNodesData[i].z << setw(25);
		if ((i + 1) % 10 == 0) outgrid << endl;
		if ((i + 1) % step == 0)
		{
			m_ProgressReadFile.SetPos(ik);
			ik++;	
			SendNewMessage();
		}		
	}
	outgrid.close();
	//隐藏进度条
	m_ProgressReadFile.ShowWindow(SW_HIDE);
	//提示
	MessageBox(_T("网格化数据已保存到文件: \n" + str), _T("提示"), MB_OK);
}
//=========================================================//
//函数名称：OutputTriangles()                              //
//函数目的：输出离散点构成的三角形                         //
//参数说明：tio_out: triangle库三角剖分后输出的结构体变量  //
//              xyz: 输入的离散点                          //
// 程序作者：刘海飞，2021.8                                //
//=========================================================//
void CMQIDlg::OutputTriangles(triangulateio& tio_out, vector<PointsXYZ>& xyz)
{
	/////////////////////////////////////////////////////////////////////////////////////
	//输出三角形的分布
	ofstream output;

	CString str;
	str = FileName + "_Triangle.bln";
	
	output.open(str, ios::out);
	output << setiosflags(ios::fixed) << setprecision(5);
	for (int i = 0; i < tio_out.numberoftriangles; i++)
	{
		//找到*p点所属的三角形id
		triangle_nodes t;
		t.vids[0] = tio_out.trianglelist[i * 3];
		t.vids[1] = tio_out.trianglelist[i * 3 + 1];
		t.vids[2] = tio_out.trianglelist[i * 3 + 2];

		output << 4 << setw(10) << 0 << endl;
		output << xyz[t.vids[0]].x << setw(25) << xyz[t.vids[0]].y << endl;
		output << xyz[t.vids[1]].x << setw(25) << xyz[t.vids[1]].y << endl;
		output << xyz[t.vids[2]].x << setw(25) << xyz[t.vids[2]].y << endl;
		output << xyz[t.vids[0]].x << setw(25) << xyz[t.vids[0]].y << endl;
	}
	output.close();
}
//=======================================================//
//函数名称：GlobalMultiQuadricInter()                    //
//函数目的：全局多重二次曲面插值                         //
//参数说明：      tio_out：三角网的输出结构体变量        //
//          GridNodesData：网格化后的数据                //
//              GridNodes：在三角形内的网格结点          //
///                 zimin：网格化后的最小值              //
//                  zimax：网格化后的最大值              //
//                      s：形状参数                      //
//                   damp：阻尼                          //
// 程序作者：刘海飞，张宇豪，2021--2023.11               //
//=======================================================//
void CMQIDlg::GlobalMultiQuadricInter(triangulateio& tio_out, vector <PointsXYZ>& GridNodesData, 
	grid_nodes_in_triangle* GridNodes, REAL& zimin, REAL& zimax, REAL s)
{	
	////////////////////////////////////////////////////////////////////////////////////////	
	//限定插值结果的上下限
	REAL zmin, zmax;
	zimin = ZCMin, zimax = ZCMax;
	if (ZCMin < 0)zmin = ZCMin * 1.5;
	else          zmin = ZCMin / 5;
	if (ZCMax > 0)zmax = ZCMax * 1.5;
	else          zmax = ZCMax / 5;
	//////////////////////////////////////////////////////////////////////////////////	
	//确定离散点分布范围
	vector < struct PointsXYZ > xyz;
	xyz.resize(tio_out.numberofpoints);

	//REAL xm = (XCMin + XCMax)/2, ym = (YCMin + YCMax) / 2;
	REAL zaver = 0;
#pragma omp parallel for reduction(+:zaver)
	for (int i = 0; i < tio_out.numberofpoints; i++)
	{
		zaver += PropertyData[i].z;
		xyz[i] = PropertyData[i];
	}
	zaver /= tio_out.numberofpoints;	
	/////////////////////////////////////////////////////////////////////////////////////
#pragma omp parallel for
	for (int i = 0; i < tio_out.numberofpoints; i++)xyz[i].z -= zaver;	
	//计算多重二次曲面系数	
	m_OutputCalInfo = "求解线性方程...";
	ShowCalInfo();
	SendNewMessage();

	vector<REAL> Coef;
	CalGlobalMultiQuadricCoef(xyz, tio_out.numberofpoints, s, Coef);
	////////////////////////////////////////////////////////////////////////////////////
	//计算插值结果		
	m_OutputCalInfo = "正在网格化...";
	ShowCalInfo();	
	SendNewMessage();

	if (m_CheckMaxRectBoundary == FALSE)//仅对凹包里的点进行插值
	{		
#pragma omp parallel for  
		for (int i = 0; i < tio_out.numberoftriangles; i++)
		{
			if (GridNodes[i].GridNodesInTriangle.size() == 0 || TriFlag[i] == -1)continue;
			for (unsigned int j = 0; j < GridNodes[i].GridNodesInTriangle.size(); j++)
			{
				REAL zm = Coef[tio_out.numberofpoints] + zaver;
				int ij = GridNodes[i].GridNodesInTriangle[j];

				//#pragma omp parallel for schedule(static) reduction(+:zm) 
				for (int k = 0; k < tio_out.numberofpoints; k++)
				{
					REAL xx = xyz[k].x - GridNodesData[ij].x;
					REAL yy = xyz[k].y - GridNodesData[ij].y;
					zm += Coef[k] * sqrt(xx * xx + yy * yy + s);
				}

				if (zm < zmin) zm = zmin;
				else if (zm > zmax) zm = zmax;
				if (zm < zimin) zimin = zm;
				else if (zm > zimax) zimax = zm;
				GridNodesData[ij].z = zm;
			}
			SendNewMessage();
		}
	}
	else  //对所有网格结点插值
	{
#pragma omp parallel for  
		for (int j = 0; j < GridNodesData.size(); j++)
		{
			REAL zm = Coef[tio_out.numberofpoints] + zaver;
			for (int k = 0; k < tio_out.numberofpoints; k++)
			{
				REAL xx = xyz[k].x - GridNodesData[j].x;
				REAL yy = xyz[k].y - GridNodesData[j].y;
				zm += Coef[k] * sqrt(xx * xx + yy * yy + s);
			}
			if (zm < zmin) zm = zmin;
			else if (zm > zmax) zm = zmax;
			if (zm < zimin) zimin = zm;
			else if (zm > zimax) zimax = zm;
			GridNodesData[j].z = zm;
			SendNewMessage();
		}
	}	
	//释放内存
	Coef.clear(); Coef.shrink_to_fit();
	xyz.clear(); xyz.shrink_to_fit();
}
//////////////////////////////////////////////////////////////////////
//输出计算信息
void CMQIDlg::ShowCalInfo()
{		
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_CAL_INFO);
	VERIFY(pEdit != NULL);
	int nLen = pEdit->GetWindowTextLength();
	pEdit->SetSel(0, nLen);
	pEdit->ReplaceSel(m_OutputCalInfo);     //
}
//=======================================================//
//函数名称：GetTrianglesVertexCoor()                     //
//函数目的：提取子域三角形顶点xyz                         //
//参数说明：      tio_out：三角网的输出结构体变量        //
//   NeighboringTriangles：邻居三角形编号                //
//    NeighboringNodesXYZ：顶点坐标                      //
///                 zaver：z平均值                       //
// 程序作者：刘海飞，2021.8                              //
//=======================================================//
REAL CMQIDlg::GetTrianglesVertexCoor(triangulateio& tio_out, vector<int>& NeighboringTriangles, vector<PointsXYZ>& NeighboringNodesXYZ)
{
	vector<int> NeighboringNodes;      //与中心三角形临近的已知数据点
	//确定邻近三角形的顶点编号	
	for (unsigned int j = 0; j < NeighboringTriangles.size(); j++)
	{
		int id3 = NeighboringTriangles[j] * 3;
		for (int k = 0; k < 3; k++)
		{
			int kk = tio_out.trianglelist[id3 + k];
			bool flag = false;
			for (unsigned int l = 0; l < NeighboringNodes.size(); l++)
			{
				if (NeighboringNodes[l] == kk)
				{
					flag = true;
					break;
				}
			}
			if (flag == false)NeighboringNodes.push_back(kk);
		}
	}
	//将顶点坐标放入容器	
	REAL zaver = 0;
	for (unsigned int j = 0; j < NeighboringNodes.size(); j++)
	{
		NeighboringNodesXYZ.push_back(PropertyData[NeighboringNodes[j]]);
		zaver += NeighboringNodesXYZ[j].z;
	}
	zaver /= NeighboringNodes.size();
	//////////////////////////////////////////////////////////////////////////////////////////////		
	//整体减去平均值，防止子域很小，z值很大的情况，提高稳定性
	for (unsigned int j = 0; j < NeighboringNodesXYZ.size(); j++)NeighboringNodesXYZ[j].z -= zaver;
	//释放内存
	vector<int>().swap(NeighboringNodes);
	//返回平均值	
	return zaver;
}
//=======================================================//
//函数名称：LocalMultiQuadricInter()                     //
//函数目的：局部多重二次曲面插值                         //
//参数说明：      tio_out：三角网的输出结构体变量        //
//          GridNodesData：网格化后的数据                //
//              GridNodes：在三角形内的网格结点          //
///                 zimin：网格化后的最小值              //
//                  zimax：网格化后的最大值              //
//                      s：形状参数                      //
//                   damp：阻尼                          //
// 程序作者：刘海飞，张宇豪，2021--2023.11               //
//=======================================================//
void CMQIDlg::LocalMultiQuadricInter(triangulateio& tio_out, vector <PointsXYZ>& GridNodesData,
	grid_nodes_in_triangle* GridNodes, REAL& zimin, REAL& zimax, REAL s)
{	
	//递归深度
	int RecDepth = atoi(m_RecDepth.GetBuffer());
	////////////////////////////////////////////////////////////////////////////////////////	
	//限定插值结果的上下限
	REAL zmin, zmax;
	REAL zimintmp = ZCMin, zimaxtmp = ZCMax;
	if (ZCMin < 0)zmin = ZCMin * 1.5;
	else          zmin = ZCMin / 5;
	if (ZCMax > 0)zmax = ZCMax * 1.5;
	else          zmax = ZCMax / 5;
	////////////////////////////////////////////////////////////////////////////////////////	
	vector<int> NeighboringTriangles;  //与中心三角形临近的三角形	
	vector<REAL> Coef;
	vector<PointsXYZ> NeighboringNodesXYZ;

	m_OutputCalInfo = "正在网格化...";
	ShowCalInfo();
	SendNewMessage();

	omp_set_num_threads(omp_get_num_procs() * 2 - 1);
	int step = tio_out.numberoftriangles / 10;

#pragma omp parallel for private(NeighboringTriangles, NeighboringNodesXYZ, Coef) schedule(dynamic) 	
	for (int i = 0; i < tio_out.numberoftriangles; i++)
	{
		if (GridNodes[i].GridNodesInTriangle.size() == 0 || TriFlag[i] == -1)continue;
		//搜索i号三角形是否为边界三角形	
		if (TriFlag[i] == -2) GetNeighboringTriangles(RecDepth + 2, i, tio_out, NeighboringTriangles);
		else 			      GetNeighboringTriangles(RecDepth, i, tio_out, NeighboringTriangles);
		///////////////////////////////////////////////////////////////////////////////////////////////
		//确定邻近三角形的顶点//计算局部数据点平均值	
		 REAL zaver = GetTrianglesVertexCoor(tio_out, NeighboringTriangles, NeighboringNodesXYZ);		
		//计算多重二次曲面系数		
		CalLocalMultiQuadricCoef(NeighboringNodesXYZ, NeighboringNodesXYZ.size(), s, Coef);
		///////////////////////////////////////////////////////////////////////////////////////////////////
		for (unsigned int j = 0; j < GridNodes[i].GridNodesInTriangle.size(); j++)
		{
			//计算待插点				
			REAL zm = Coef[NeighboringNodesXYZ.size()] + zaver;
			//计算插值结果				
			for (unsigned int k = 0; k < NeighboringNodesXYZ.size(); k++)
			{
				REAL xx = NeighboringNodesXYZ[k].x - GridNodesData[GridNodes[i].GridNodesInTriangle[j]].x;
				REAL yy = NeighboringNodesXYZ[k].y - GridNodesData[GridNodes[i].GridNodesInTriangle[j]].y;
				zm += Coef[k] * sqrt(xx * xx + yy * yy + s);
			}
			if (zm < zmin) zm = zmin;
			else if (zm > zmax) zm = zmax;
			if (zm < zimintmp) zimintmp = zm;
			else if (zm > zimaxtmp) zimaxtmp = zm;
			GridNodesData[GridNodes[i].GridNodesInTriangle[j]].z = zm;
		}				
		NeighboringTriangles.clear();		
		Coef.clear(); 
		NeighboringNodesXYZ.clear(); 

		if ((i + 1) % step == 0)SendNewMessage();		
	}
	zimin = zimintmp, zimax = zimaxtmp;
	/////////////////////////////////////////////////////////////////////////////////////
	//释放内存 
	NeighboringTriangles.shrink_to_fit();
	Coef.shrink_to_fit();
	NeighboringNodesXYZ.shrink_to_fit();		
}
//=======================================================//
//函数名称：LinearInterWithTriangulation()               //
//函数目的：基于三角网的线性插值                         //
//参数说明：      tio_out：三角网的输出结构体变量        //
//          GridNodesData：网格化后的数据                //
//              GridNodes：在三角形内的网格结点          //
// 程序作者：刘海飞，2021.8                              //
//=======================================================//
void CMQIDlg::LinearInterWithTriangulation(triangulateio& tio_out, vector <PointsXYZ>& GridNodesData, grid_nodes_in_triangle* GridNodes)
{	
	m_OutputCalInfo = "正在网格化...";
	ShowCalInfo();
	SendNewMessage();
	////////////////////////////////////////////////////////////////////////////////////////
	//对每个三角形里的插值点进行线性插值
	int coresnum = omp_get_num_procs();
	omp_set_num_threads(coresnum * 2 - 1);

	for (int i = 0; i < tio_out.numberoftriangles; i++)
	{	
		//三角形内无插值点或三角形在边界外侧不进行插值
		if (GridNodes[i].GridNodesInTriangle.size() == 0 || TriFlag[i] == -1)continue;

		int i3 = i * 3;
		triangle_nodes t;
		t.vids[0] = tio_out.trianglelist[i3];
		t.vids[1] = tio_out.trianglelist[i3 + 1];
		t.vids[2] = tio_out.trianglelist[i3 + 2];
		//三角形的三个顶点坐标
		double x0 = PropertyData[t.vids[0]].x;
		double y0 = PropertyData[t.vids[0]].y;
		double z0 = PropertyData[t.vids[0]].z;
		double x1 = PropertyData[t.vids[1]].x;
		double y1 = PropertyData[t.vids[1]].y;
		double z1 = PropertyData[t.vids[1]].z;
		double x2 = PropertyData[t.vids[2]].x;
		double y2 = PropertyData[t.vids[2]].y;
		double z2 = PropertyData[t.vids[2]].z;
		//构建向量P02,P12
		double x02 = x0 - x2;
		double y02 = y0 - y2;
		double z02 = z0 - z2;
		double x12 = x1 - x2;
		double y12 = y1 - y2;
		double z12 = z1 - z2;
		//计算插值权
		REAL w1, w2, w3;
		if (y12 != 0.0)
		{
			double yy = y02 / y12;
			w1 = (z02 - z12 * yy) / (x02 - x12 * yy);
			w2 = (z12 - w1 * x12) / y12;
			w3 = (z2 - w1 * x2 - w2 * y2);
		}
		else
		{
			double xx = x02 / x12;
			w2 = (z02 - z12 * xx) / (y02 - y12 * xx);
			w1 = (z12 - w2 * y12) / x12;
			w3 = (z2 - w1 * x2 - w2 * y2);
		}
		//线性插值
		for (unsigned int j = 0; j < GridNodes[i].GridNodesInTriangle.size(); j++)
		{			
			REAL xx = GridNodesData[GridNodes[i].GridNodesInTriangle[j]].x;
			REAL yy = GridNodesData[GridNodes[i].GridNodesInTriangle[j]].y;				
			GridNodesData[GridNodes[i].GridNodesInTriangle[j]].z = xx * w1 + yy * w2 + w3;
		}	
	}	
}
// 程序作者：刘海飞，张宇豪，2021--2023.11            
// 开始网格化
void CMQIDlg::OnClickedButtonStartGridding()
{	
	if (PropertyDataFlag == FALSE)
	{
		MessageBox(_T("请先读取数据文件！"), _T("警告"), MB_OK);
		return;
	}
	else if (BoundaryFlag == FALSE && m_CheckMaxRectBoundary == FALSE)
	{		
		MessageBox(_T("请先搜索离散点边界！"), _T("警告"), MB_OK);
		return;		
	}
	//设置开始网格化按钮为禁止状态
	((CButton*)GetDlgItem(IDC_BUTTON_START_GRIDDING))->EnableWindow(FALSE);	
	//////////////////////////////////////////////////////////////////////////
	// 时间
	LARGE_INTEGER st, et, cf;
	QueryPerformanceFrequency(&cf);
	QueryPerformanceCounter(&st);
	////////////////////////////////////////////////////////////////////////////////////////	
	//待插点的网格化结点数据个数
	UpdateData(TRUE);
	int XEleNum = atoi(m_XEleNum.GetBuffer());
	int YEleNum = atoi(m_YEleNum.GetBuffer());

	REAL XEleSize = (XCMax - XCMin) / XEleNum;
	REAL YEleSize = (YCMax - YCMin) / YEleNum;
	int GridNodesNum = (XEleNum + 1) * (YEleNum + 1);

	vector <PointsXYZ> GridNodesData;
	GridNodesData.resize(GridNodesNum);

	int coresnum = omp_get_num_procs();
	omp_set_num_threads(coresnum * 2 - 1);

#pragma omp parallel for 
	for (int i = 0; i <= YEleNum; i++)
	{
		for (int j = 0; j <= XEleNum; j++)
		{
			int ij = i * (XEleNum + 1) + j;
			GridNodesData[ij].x = XCMin + j * XEleSize;
			GridNodesData[ij].y = YCMin + i * YEleSize;
		}
	}	
	/////////////////////////////////////////////////////////////////////////
	//确定插值节点所在的三角形	
	grid_nodes_in_triangle* GridNodes = new grid_nodes_in_triangle[tio_out.numberoftriangles];
	if (m_CheckMaxRectBoundary == FALSE)
	{
		m_OutputCalInfo = "搜索网格结点所在的三角形...";
		ShowCalInfo();
		SendNewMessage();

		int j, it = 0, id = 0;   
		PointsXY p1, p2;

		for (int i = 0; i < GridNodesNum; i++)
		{		
			if (id < 0)id = it;
			//选取插值结点p
			PointsXY p;
			p.x = GridNodesData[i].x;
			p.y = GridNodesData[i].y;
			//找到*p点所属的三角形id
			int id3 = id * 3;
			triangle_nodes t;
			t.vids[0] = tio_out.trianglelist[id3];
			t.vids[1] = tio_out.trianglelist[id3 + 1];
			t.vids[2] = tio_out.trianglelist[id3 + 2];
			do
			{
				for (j = 0; j < 3; j++)
				{
					int j1 = (j + 1) % 3;
					p1.x = tio_out.pointlist[t.vids[j] * 2];
					p1.y = tio_out.pointlist[t.vids[j] * 2 + 1];
					p2.x = tio_out.pointlist[t.vids[j1] * 2];
					p2.y = tio_out.pointlist[t.vids[j1] * 2 + 1];
					if (onrightside(&p, &p1, &p2))
					{
						id = tio_out.neighborlist[id * 3 + (j + 2) % 3];
						if (id < 0)
						{
							j = 3;
							break;
						}
						else
						{
							it = id;
						}
						id3 = id * 3;
						t.vids[0] = tio_out.trianglelist[id3];
						t.vids[1] = tio_out.trianglelist[id3 + 1];
						t.vids[2] = tio_out.trianglelist[id3 + 2];
						break;
					}
				}
			} while (j < 3);
			if (id >= 0 && TriFlag[id] != -1)GridNodes[id].GridNodesInTriangle.push_back(i);
			GridNodesData[i].z = DBL_MAX;
		}
	}	
	////////////////////////////////////////////////////////////////////////////////////////
	//多重二次曲面网格化
	REAL zimin = ZCMin, zimax = ZCMax;
	if (GriddingMethod == false)
	{
		/////////////////////////////////////////////////////////////////////////////////////
		//形状参数
		REAL s = ((XCMax - XCMin) * (XCMax - XCMin) + (YCMax - YCMin) * (YCMax - YCMin)) / (25 * tio_out.numberofpoints);			
		///////////////////////////////////////////////////////////////////////////////////		
		int MaxPointsNumofMQSurface = atoi(m_MaxPointsNumofMQSurface.GetBuffer());
		if (tio_out.numberofpoints <= MaxPointsNumofMQSurface || ( m_CheckMaxRectBoundary == TRUE && tio_out.numberofpoints <= 30000) ) //全局MQ网格化
		{
			GlobalMultiQuadricInter(tio_out, GridNodesData, GridNodes, zimin, zimax, s);
		}
		else  //局部MQ网格化
		{			
			LocalMultiQuadricInter(tio_out, GridNodesData, GridNodes, zimin, zimax, s);
		}
	}
	else   //非结构化三角网线性插值
	{		
		LinearInterWithTriangulation(tio_out, GridNodesData, GridNodes);
	}	
	////////////////////////////////////////////////////////////////////////////////////
	//计算耗费时间
	QueryPerformanceCounter(&et);
	REAL est = ((REAL)et.QuadPart - (REAL)st.QuadPart) / (REAL)cf.QuadPart;
	
	m_OutputCalInfo.Format("耗费时间: %2.5fS", est);
	ShowCalInfo();
	SendNewMessage();
	///////////////////////////////////////////////////////////////////////////////////
	//输出网格化结果文件
	OutputGriddingFile(XCMin, XCMax, YCMin, YCMax, zimin, zimax, XEleNum + 1, YEleNum + 1, GridNodesData);	

	///////////////////////////////////////////////////////////////////////////////////
	//释放内存
	vector<PointsXYZ>().swap(GridNodesData);
	for (int i = 0; i < tio_out.numberoftriangles; i++)
	{
		vector<int>().swap(GridNodes[i].GridNodesInTriangle);		
	}
	delete[]GridNodes;
	//设置开始网格化按钮为激活状态
	((CButton*)GetDlgItem(IDC_BUTTON_START_GRIDDING))->EnableWindow(TRUE);
}
//退出程序
void CMQIDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//释放全局容器的内存
	pp.clear(); pp.shrink_to_fit();
	ANum.clear(); ANum.shrink_to_fit();
	TriFlag.clear(); TriFlag.shrink_to_fit();	
	//退出
	CDialogEx::OnCancel();
}

//绘制最大矩形作为边界
void CMQIDlg::DrawMaxRectBoundary()
{
	// TODO: 在此添加控件通知处理程序代码
	int xl = xLeft + offset;
	int xr = xRight - offset;
	int yu = yTop + offset;
	int yd = yBottom - offset;

	int xm = (xr + xl) / 2;
	int ym = (yd + yu) / 2;

	REAL XM = (XCMax + XCMin) / 2;
	REAL YM = (YCMax + YCMin) / 2;

	double ZCD = ZCMax - ZCMin;

	double xscale = (xr - xl) / (XCMax - XCMin);
	double yscale = (yd - yu) / (YCMax - YCMin);
	double scale = xscale > yscale ? yscale : xscale;

	int xLeft = int(xm + (XCMin - XM) * scale);
	int xRight = int(xm + (XCMax - XM) * scale);
	int yTop = int(ym - (YCMax - YM) * scale);
	int  yBottom = int(ym - (YCMin - YM) * scale);

	CDC* pDC = GetDC();  //获得设备环境指针
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));	
	pDC->SelectObject(&Pen);
	::SelectObject(pDC->m_hDC, GetStockObject(NULL_BRUSH));
	//绘制矩形框
	pDC->Rectangle(xLeft, yTop, xRight, yBottom);
	ReleaseDC(pDC);

}
//以离散点最大矩形作为边界
void CMQIDlg::OnClickedCheckMaxRect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (PropertyDataFlag == FALSE)
	{
		MessageBox(_T("请先读取数据文件！"), _T("警告"), MB_OK);
		((CButton*)GetDlgItem(IDC_CHECK_MAX_RECT))->SetCheck(FALSE);
		return;
	}
	if (tio_out.numberofpoints > 30000) //只能采用局部MQ网格化
	{
		MessageBox(_T("离散点数据个数大于30000，只能采用局部MQ网格化！"), _T("警告"), MB_OK);
		((CButton*)GetDlgItem(IDC_CHECK_MAX_RECT))->SetCheck(FALSE);
		return;
	}
	//重新导入离散点
	RefreshMapArea();
	BoundaryFlag = false;
	if (m_CheckMaxRectBoundary == FALSE)
	{
		((CButton*)GetDlgItem(IDC_CHECK_MAX_RECT))->SetCheck(TRUE);
		m_CheckMaxRectBoundary = TRUE;
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCH_BOUNDARY))->EnableWindow(FALSE);
		//绘制矩形边界
		DrawMaxRectBoundary();	
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_MAX_RECT))->SetCheck(FALSE);
		m_CheckMaxRectBoundary = FALSE;
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCH_BOUNDARY))->EnableWindow(TRUE);				
	}
}
