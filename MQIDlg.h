
// MQIDlg.h: 头文件
//

#pragma once
#include <vector>
#include<iostream>
#include<vector>
#include<string>
#include "triangle.h"
#include "omp.h"

using namespace std;

typedef double REAL;
//xyz分量
struct PointsXYZ
{
	double x, y, z;
};
struct number
{
	int ni, nj;
};

struct PointsXY
{
	REAL x;
	REAL y;
};

struct triangle_nodes
{
	int vids[3];
};

struct triangle_neighbours
{
	int tids[3];
};

struct triangle_weights
{
	REAL wts[3];
};

struct grid_nodes_in_triangle
{
	vector<int>GridNodesInTriangle;
};

class CRectOfControl
{
public:
	int nId;
	int width;
	int height;
	CRect rect;
};
// CMQIDlg 对话框
class CMQIDlg : public CDialogEx
{
// 构造
public:
	CMQIDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MQI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	int ColorNum;
	double Data[21], PropertyValue[21];
	int Red[21], Green[21], Blue[21];	

public:

	CRect rc;
	unsigned int xLeft, yTop, xRight, yBottom, offset;
	

	bool DrawPointsFlag;	
	CDC MemDC; //设备指针

public:
	//保存边界线段
	vector < number >pp;
	//封闭区域的首尾结点序号
	vector< size_t > ANum;
	//标识三角形是否在边界以内，在边界外其值为-1；
	vector< size_t > TriFlag;	

	//绘制边界
	void DrawBoundary();

	//在屏幕上绘制像素
	void DrawPixel();
	//获得该属性值对应的颜色
	void GetMapColor(double* PropertyValue, double& pv, int& rv, int& gv, int& bv);

public:
	//高程数据
	vector < struct PointsXYZ > PropertyData;
	double XCMax, XCMin, YCMax, YCMin, ZCMax, ZCMin, PointAverDis;
		
	bool PropertyDataFlag;
	bool BoundaryFlag;

	CString FileName;
	bool GriddingMethod;
public:
	/////////////////////////////////////////////////////////////////////////////////////
	//非结构三角化对象
	triangulateio tio_in;
	triangulateio tio_out;
	//初始化triang.c的结构体成员
	void tio_init(struct triangulateio* tio);
	//释放triang.c的结构体成员
	void tio_destroy(struct triangulateio* tio);
	//根据平面离散点数据,生成三角网
	void TriangulationOfDiscretePoints(triangulateio& tio_in, triangulateio& tio_out, vector<PointsXYZ>& xyz);
	//搜索离散点的凹包边界
	void SearchBoundary(triangulateio& tio_out, vector<PointsXYZ>& xyz);	
	//判别点p是否在向量p12的右侧
	int onrightside(PointsXY* p, PointsXY* p1, PointsXY* p2);
	//利用共轭梯度法解线性方程组-串行 
	void LSCG(REAL* A, REAL* x, REAL* B, const int n, REAL damp);
	void LocalLSCG(vector<REAL>& A, vector<REAL>& x, vector<REAL>& B, const int n, REAL damp);
	//利用共轭梯度法解线性方程组-并行 
	void GlobalLSCG(vector<REAL>& A, vector<REAL>& x, vector<REAL>& B, const int n, REAL damp);	
	//计算全局多重二次曲面系数
	void CalGlobalMultiQuadricCoef(vector<PointsXYZ>& NeighberingPoints, size_t m, REAL s, vector<REAL>& Coef);
	//计算局部多重二次曲面系数
	void CalLocalMultiQuadricCoef(vector<PointsXYZ>& NeighberingPoints, size_t m, REAL s, vector<REAL>& Coef);
	//获得以id三角形为中心的邻近三角形
	void GetNeighboringTriangles(int RecDepth, int id, triangulateio& tio_out, vector<int>& NeighboringTriangles);
	//通过递归向外搜索邻近三角形
	void RecSearchNeighboringTriangles(int RecDepth, int ik, triangulateio& tio_out, vector<int>& NeighboringTriangles);
	//全局多重二次曲面插值
	void GlobalMultiQuadricInter(triangulateio& tio_out, vector <PointsXYZ>& GridNodesData, grid_nodes_in_triangle* GridNodes, REAL& zimin, REAL& zimax, REAL s);
	//局部多重二次曲面插值
	void LocalMultiQuadricInter(triangulateio& tio_out, vector <PointsXYZ>& GridNodesData, grid_nodes_in_triangle* GridNodes, REAL& zimin, REAL& zimax, REAL s);
	//基于三角网的线性插值
	void LinearInterWithTriangulation(triangulateio& tio_out, vector <PointsXYZ>& GridNodesData, grid_nodes_in_triangle* GridNodes);
	//输出网格化结果文件
	void OutputGriddingFile(REAL xmin, REAL xmax, REAL ymin, REAL ymax,	REAL zmin, REAL zmax, int xNodesNum, int yNodesNum, vector <PointsXYZ>& GridNodesData);
	//输出离散点构成的三角形  
	void CMQIDlg::OutputTriangles(triangulateio& tio_out, vector<PointsXYZ>& xyz);
	//输出计算信息
	void ShowCalInfo();
	//发送消息
	void SendNewMessage();
	//提取子域三角形顶点xyz
    REAL GetTrianglesVertexCoor(triangulateio& tio_out, vector<int>& NeighboringTriangles, vector<PointsXYZ>& NeighboringNodesXYZ);
	
public:

	//绘制矩形
	void DrawRectCanvas();	
	//双缓存绘制图形
	void DrawPoints();
	//刷新绘图区
	void RefreshMapArea();
	//字符串分割函数 
	void split(string& instr, vector<string>& outstr, const string& pattern);
	//绘制最大矩形作为边界
	void DrawMaxRectBoundary();
		
	afx_msg void OnClickedButtonLoadData();
	CProgressCtrl m_ProgressReadFile;

	//X,Y范围
	CString m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax;
	//数据点数
	CString  DataTotalNum;
	//点位平均距离
	CString  DataPointAverDis;
	//最大边界距离
	CString m_BoundaryMaxDist;

	//全局趋势面的最大点数
	CString m_MaxPointsNumofMQSurface;

	//搜索邻居的递归深度
	CString m_RecDepth;

	//X，Y方向剖分的单元数
	CString m_XEleNum, m_YEleNum;
	//X，Y方向剖分的单元大小
	CString m_XEleSize, m_YEleSize;

	//输出当前计算信息
	CString m_OutputCalInfo;
	
	afx_msg void OnClickedButtonSearchBoundary();
	afx_msg void OnRadioMQInterMethod();
	afx_msg void OnRadioTriLinearInterMethod();
	afx_msg void OnChangeEditXElementNum();
	afx_msg void OnChangeEditYElementNum();
	afx_msg void OnChangeEditGlobalMqDataMaxNum();
	afx_msg void OnClickedButtonStartGridding();
	afx_msg void OnChangeEditRecDepth();
	afx_msg void OnBnClickedCancel();
	BOOL m_CheckMaxRectBoundary;
	afx_msg void OnClickedCheckMaxRect();
};
