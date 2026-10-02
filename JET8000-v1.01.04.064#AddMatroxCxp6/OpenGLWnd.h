#if !defined(AFX_OPENGLWND_H__B4AF22F7_7CD0_475E_807D_472BB143D71C__INCLUDED_)
#define AFX_OPENGLWND_H__B4AF22F7_7CD0_475E_807D_472BB143D71C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGLWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include <float.h>
#include <gl\gl.h>
#include <gl\glu.h>
#pragma comment(lib, "OPENGL32.LIB")
#pragma comment(lib, "GLU32.LIB ")
//-------------------------------------------------------------------------------------//
//#define UNWRAP_DATA_AVOID           FLT_MAX-10.0f
//-------------------------------------------------------------------------------------//
#define TIMER_ID_ANIMATION      100//Timer->動畫
//-------------------------------------------------------------------------------------//
#define OPEN_GL_FINENESS_HIGHT_MOST     1
#define OPEN_GL_FINENESS_HIGHT_MORE     2
#define OPEN_GL_FINENESS_HIGHT          4//Target
#define OPEN_GL_FINENESS_MIDDLE         8
#define OPEN_GL_FINENESS_LOW           16
#define OPEN_GL_FINENESS_LOW_MORE      32
#define OPEN_GL_FINENESS_LOW_MOST      64
//-------------------------------------------------------------------------------------//
#define CALC_OBJEC_MAX_IMAGE_W       2048//確認要計算物件的影像上限
#define CALC_OBJEC_MAX_IMAGE_H       2048//確認要計算物件的影像上限
//-------------------------------------------------------------------------------------//
enum OPENGL_SHOW_MODE
{
	OPENGL_SHOW_MODEL  = 1,
	OPENGL_SHOW_STRING = 2
};
//-------------------------------------------------------------------------------------//
enum OPENGL_OBJECT_MODE
{
	OPENGL_OBJECT_COLOR      = 1,
	OPENGL_OBJECT_LINE       = 2,
	OPENGL_OBJECT_TEXTURE    = 3,
	OPENGL_OBJECT_COLOR_LINE = 4
};
//-------------------------------------------------------------------------------------//
#ifndef _Jet3DAOIDefine_H_	
	typedef struct _POINT3F
	{
		float x,y,z;	
		_POINT3F():x(0.0f), y(0.0f), z(0.0f)
		{}
	} TPOINT3F, *PPOINT3F;
#endif//_Jet3DAOIDefine_H_	
//-------------------------------------------------------------------------------------//
const float INT_TO_FLOAT           = 1000.0f;
//-------------------------------------------------------------------------------------//
typedef struct _OpenGLBox
{
	float   fPtX_1;
	float   fPtY_1;
	float   fPtX_2;
	float   fPtY_2;
	float   fPtX_3;
	float   fPtY_3;
	float   fPtX_4;
	float   fPtY_4;
	float   fHeight;
	bool    bShow;
	CString strText;
	_OpenGLBox()
	{	
		fPtX_1 = 0;
		fPtY_1 = 0;
		fPtX_2 = 0;
		fPtY_2 = 0;
		fPtX_3 = 0;
		fPtY_3 = 0;
		fPtX_4 = 0;
		fPtY_4 = 0;
		fHeight = 0;
		bShow = true;		
	}
} TOpenGLBox, *POpenGLBox;
//-------------------------------------------------------------------------------------//
#define  OPENGL_OBJ_RULER         1//m_GLModel_ColorRuler
#define  OPENGL_OBJ_MODEL         2//m_GLModel_MainObject
#define  OPENGL_OBJ_BOUND_PAD     3//m_GLModel_BoxRegion
#define  OPENGL_OBJ_BOUND_ROI     4//m_GLModel_RoiRegion
#define  OPENGL_OBJ_BASE_PLANE    5//m_GLModel_BaseBound
#define  OPENGL_OBJ_UPPER_PLANE   6//m_GLModel_UpperBound
#define  OPENGL_OBJ_LOWER_PLANE   7//m_GLModel_LowerBound
#define  OPENGL_OBJ_STRING        8//m_GLModel_DrawText
#define  OPENGL_OBJ_CLIP_PLANE    9//m_GLModel_ClipPlane

#define  OPENGL_OBJ_POINT_A      10//m_GLModel_Point-A
#define  OPENGL_OBJ_POINT_B      11//m_GLModel_Point-B
#define  OPENGL_OBJ_POINT_NOW    12//m_GLModel_Point-Now
#define  OPENGL_OBJ_AXIS         13//m_GLModel_Axis(X-Y-Z)

#define  OPENGL_OBJ_BOUND_BOX    20//m_GLModel_RoiRegio-會往後加
//-------------------------------------------------------------------------------------//
#define  OPENGL_CLIP_PLANE_HOR    1//Horizontal Line
#define  OPENGL_CLIP_PLANE_VER    2//Vertical Line
#define  OPENGL_CLIP_PLANE_ANY    3//Any Line
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd window
//-------------------------------------------------------------------------------------//
class COpenGLWnd : public CStatic
{
// Construction
public:
	COpenGLWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
// Implementation
public:
	virtual ~COpenGLWnd();

public:
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetErrorString()	{ return m_ErrorString; }
	//---------------------------------------------------------------------------------//	
	void                       InitialDisplay();
	void                       ShowMessage(LPCTSTR str);
	void                       ReleaseAllBuffer();
	bool                       CreateMaxDataBuffer();//建立最大記憶體緩衝
	bool                       BuildProfileValue(std::vector<float> &DataList);//計算剖線資料
	float*                     Get3DData(IMAGE_SIZE &DataW, IMAGE_SIZE &DataH, IMAGE_SIZE &DataStep);
	bool                       Clone3DData(IMAGE_SIZE &DataW, IMAGE_SIZE &DataH, IMAGE_SIZE &DataStep, float*&Ptr);//複製3D資料
	void Set3DData(const int* p3D, const unsigned char *p2D, IMAGE_SIZE DataW, IMAGE_SIZE DataH, bool IsColor, float RulerMinH,float RulerMaxH, float ShowMinH, float ShowMaxH, RECT PadRect, RECT ROIRect, float PadSpecHeight);
	void Set3DData(const float* p3D, const unsigned char *p2D, IMAGE_SIZE DataW, IMAGE_SIZE DataH, bool IsColor, float RulerMinH,float RulerMaxH, float ShowMinH, float ShowMaxH, RECT PadRect, RECT ROIRect, float PadSpecHeight);
	//---------------------------------------------------------------------------------//	
	//選單參數
	//---------------------------------------------------------------------------------//	
	float                      GetDataMaxH() const { return m_MaxH; }
	float                      GetDataMinH() const { return m_MinH; }
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 GetDataW() const { return m_3DWidth; }
	IMAGE_SIZE                 GetDataH() const { return m_3DHeight; }
	//---------------------------------------------------------------------------------//	
	void                       SetMainObjectMode(OPENGL_OBJECT_MODE Mode);
	OPENGL_OBJECT_MODE         GetMainObjectMode() const { return m_MainObjectMode; }
	//---------------------------------------------------------------------------------//	
	void                       SetIsShowMainObject(bool bShow);
	bool                       GetIsShowMainObject() const;
	//---------------------------------------------------------------------------------//
	void                       SetZoomScale(float ScaleX, float ScaleY, float ScaleZ);
	void                       GetZoomScale(float &ScaleX, float &ScaleY, float &ScaleZ) const;	
	//---------------------------------------------------------------------------------//
	float                      GetRuleMaxH() const;
	void                       SetRuleMaxH(float val);	
	float                      GetRuleMinH() const;
	void                       SetRuleMinH(float val);

	void                       SetModalCenter(bool ResetRotated);	
	void                       SetModalCenterScale(bool ResetRotated, float sX, float sY, float sZ);	

	float                      GetPadSpecHeight() const { return m_PadBox.fHeight; }

	void                       SetShowColorRuler(bool bShow);
	bool                       GetShowColorRuler() const { return m_IsShowColorRuler; }

	void                       SetDetailLevel(int nLv);
	int                        GetDetailLevel() const { return m_DetailLevel; }	

	void                       SetShowPadHeight(bool IsShow) { m_IsShowPadHeight=IsShow; }
	bool                       GetShowPadHeight() const { return m_IsShowPadHeight; }	

	void                       SetBoundPadOpen(bool IsOpen);
	bool                       GetBoundPadOpen() const { return m_IsShowPadRegion; }		
	void                       SetPadRegion(const RECT &BoxRect, float Height=-1);	
	void                       SetPadCornerPts(const TPOINT2D BoxCornerPts[], float Height=-1);		

	void                       SetBoundRoiOpen(bool IsOpen);
	bool                       GetBoundRoiOpen() const { return m_IsShowRoiRegion; }	
	void                       SetRoiRegion(const RECT &BoxRect, float Height=-1);		
	void                       SetRoiCornerPts(const TPOINT2D BoxCornerPts[], float Height=-1);		

	void                       SetBasePlaneOpen(bool IsOpen);
	bool                       GetBasePlaneOpen() const { return m_IsBaseOpen; }

	void                       SetUpperPlaneOpen(bool IsOpen);
	bool                       GetUpperPlaneOpen() const { return m_IsUpperOpen; }

	void                       SetLowerPlaneOpen(bool IsOpen);
	bool                       GetLowerPlaneOpen() const { return m_IsLowerOpen; }

	void                       SetClipPlaneOpen(bool IsOpen);
	bool                       GetClipPlaneOpen() const { return m_IsClipOpen; }		

	void                       SetIsShowPoint(bool IsOpen);
	bool                       GetIsShowPoint() const { return m_IsShowPoint; }	

	void                       SetIsShowAxis(bool Is);
	bool                       GetIsShowAxis() const { return m_IsShowAxis; }	

	std::vector<float>         GetClipPlanePos() { return {m_ClipPlanePosX1, m_ClipPlanePosY1, m_ClipPlanePosX2, m_ClipPlanePosY2}; }

	void                       SetClipPlaneDir(int Dir);
	int                        GetClipPlaneDir() const { return m_ClipPlaneDir; }
	void                       UpdateClipPlanePos();
	void                       ChangeClipPlaneSize(bool bBigger);

	void                       GetShowImageSize(int &ImageW, int &ImageH);
	bool                       ExtractShowImage(unsigned char *pImage, const int ImageW, const int ImageH);

	void                       SetMouseDisplayScale(float Scale);
	void                       GetMouseDisplayScale(float &Scale);
	void                       SetUserResolution(float UserResolution);

	void                       SetBountPitch(float Pitch);
	float                      GetBoundPitch() const;//m_BoundPitch

	float                      GetBoundHeight() const;
	void                       SetBountHeight(float Pos);	
	
	float                      GetUpperBoundHeight() const;
	void                       SetUpperBoundHeight(float Pos);

	float                      GetLowerBoundHeight() const;
	void                       SetLowerBoundHeight(float Pos);

	float                      GetShowMinH() const;
	float                      GetShowMaxH() const;	

	float                      Get3DScale() const;
	
	void                       SetGLBoxListOpen(bool IsOpen);
	bool                       GetGLBoxListOpen() const { return m_IsShowGLBoxList; }
	size_t                     GetGLBoxListSize() const;	
	TOpenGLBox*                GetGLBoxPtr(size_t index, bool bCheck);
	bool                       AddGLBoxObj(const TOpenGLBox &Box);
	void                       ClearGLBoxList();
	void                       UpdateGLBoxList();
	//---------------------------------------------------------------------------------//
	void                       SetAnimationPlay(bool val);
	bool                       GetAnimationPlay() const;
	//---------------------------------------------------------------------------------//	
	double                     GetRotateAngleX() const;
	void                       SetRotateAngleX(double val);
	double                     GetRotateAngleY() const;
	void                       SetRotateAngleY(double val);
	void                       RotateToPadAngle();//轉到Pad視角
	void                       RotateToPadAngleByAroundHeight();//轉到Pad視角-使用四周圍比較
	//---------------------------------------------------------------------------------//	
	bool                       ConvertRectToGLBox(int W, int H, const RECT &Rect, float Height, TOpenGLBox &glBox);//將Rect轉成TOpenGLBox
	bool                       ConvertCornerPtToGLBox(const TPOINT2D BoxCornerPts[], float Height, TOpenGLBox &glBox);//將四角端點轉成TOpenGLBox
	bool                       ConvertGLBoxToRect(const TOpenGLBox &glBox, RECT &Rect);//將TOpenGLBox轉成Rect
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitial();
	void                       Release();	
	void                       CreateOpenGLDC();
	int                        GetOpenGLTextureSize();//取得OpenGL材質的大小
	void                       CreateDataBuffer(unsigned int DataW, unsigned int DataH, bool IsColor);
	bool                       Build3DObject();
	void                       RenderScene();
	void                       RenderModel();
	void                       RenderString();
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//	
	void                       SetTextureObject();	//創材質buffer
	void                       GetColor(float z,float &r,float &g,float &b);	//取得點顏色
	void                       GetVector(const TPOINT3F &PtLB, const TPOINT3F &PtLT, const TPOINT3F &PtRB, TPOINT3F &vector);//取得法向量
	//---------------------------------------------------------------------------------//	
	//創model	
	void                       BuildColorRuler();
	void                       BuildMainObject();
	void                       BuildLineObject();
	void                       BuildColorObject();
	void                       BuildColorLineObject();
	void                       BuildTextureObject();
	void                       BuildPadRegion();
	void                       BuildRoiRegion();
	void                       BuildGLBoxList();
	void                       BuildBaseBound();
	void                       BuildUpperBound();
	void                       BuildLowerBound();
	void                       BuildClipPlane();		
	void                       BuildPoint(double PosX, double PosY, double Height, int ObjectID);
	bool                       BuildAxis();//繪製三軸
	
	void                       RenderGLBoxList();
	void                       DrawGLColorQuads(int index);
	//---------------------------------------------------------------------------------//	
	//畫字
	void                       DoSelectFont(int size, int charset, const char* face);
	void                       DrawChineseString(LPCTSTR str) ;
	unsigned int               GetNBytesPerLine(const unsigned int ImageW);
	//---------------------------------------------------------------------------------//		
	//GL windows function
	BOOL                       SetWindowPixelFormat(HDC hDC);
	BOOL                       CreateViewGLContext(HDC hDC);
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//	
	OPENGL_SHOW_MODE           m_ShowMode;
	CString                    m_ShowString;
	CString                    m_ErrorString;
	//---------------------------------------------------------------------------------//	
	CString                    m_strUpper;
	CString                    m_strLower;
	CString                    m_strHeight;
	CString                    m_strObject;
	//---------------------------------------------------------------------------------//	
	//GL Window 參數
	//---------------------------------------------------------------------------------//	
	HDC                        m_hDC;
	HGLRC                      m_hGLContext;
	int                        m_GLPixelIndex;

	float m_Ortho_Left, m_Ortho_Right, m_Ortho_Bot, m_Ortho_Top, m_Ortho_Near, m_Ortho_Far;

	//縮放比例
	GLfloat                    m_ScaleX;
	GLfloat                    m_ScaleY;
	GLfloat                    m_ScaleZ;

	GLfloat                    m_ScaleX_Fit;
	GLfloat                    m_ScaleY_Fit;
	GLfloat                    m_ScaleZ_Fit;
	//旋轉
	GLdouble                   m_xRotate;
	GLdouble                   m_yRotate;
	//平移
	GLdouble                   m_xTranslate;
	GLdouble                   m_yTranslate;
	CPoint                     m_LastPos;
	CPoint                     m_RightDownPos;
	CPoint                     m_LeftDownPos;

	TPOINT3F                    m_RulerLB, m_RulerRT;	//高度尺的3D座標

	OPENGL_OBJECT_MODE         m_MainObjectMode;
	bool                       m_IsShowColorRuler;
	bool                       m_IsShowPadHeight;
	bool                       m_IsShowPadRegion;	
	bool                       m_IsShowRoiRegion;
	bool                       m_IsShowGLBoxList;
	bool                       m_IsShowMainObject;
	bool                       m_IsBaseOpen;
	bool                       m_IsUpperOpen;
	bool                       m_IsLowerOpen;
	bool                       m_IsClipOpen;
	bool                       m_IsShowPoint;	
	bool                       m_IsShowAxis;	
	bool                       m_AnimationPlay;

	//材質 Buffer
	unsigned int               m_TextureNumber;
	unsigned int*              m_TextureIndex;
	float*                     m_TextureReducePriorities;

	//畫圖資料
	float                      m_Resolution;
	float                      m_UserResolution;
	float                      m_ScaleW, m_ScaleH;	
	float                      m_MaxH, m_MinH;

	float                      m_RulerMinH, m_RulerMaxH;
	float                      m_RulerMinH_GL, m_RulerMaxH_GL;	//3D 世界座標

	float                      m_ShowMinH, m_ShowMaxH;
	float                      m_ShowMinH_GL, m_ShowMaxH_GL;

	std::vector<TOpenGLBox>    m_GLBoxList;
	TOpenGLBox                 m_PadBox, m_RoiBox;
	TOpenGLBox                 m_PadBox_GL, m_RoiBox_GL;	
	
	bool                       m_IsColor;
	float*                     m_3DData;
	int                        m_DetailLevel;//精細度
	unsigned int               m_3DWidth, m_3DHeight;
	size_t                     m_Max3DBufferSize;

	//材質縮圖
	size_t                     m_TextureSize;//幾X幾 //256 ,128  
	float                      m_TextureScale;

	unsigned char*             m_2DData;
	size_t                     m_Max2DBufferSize;
	unsigned int               m_TextureWidth;
	unsigned int               m_TextureHeight;
	unsigned int               m_TextureRealW;

	//上下邊界目前的實際高度值
	float                      m_BoundPitch;	
	float                      m_UpperBoundHeight;
	float                      m_UpperBoundHeight_GL;
	float                      m_LowerBoundHeight;
	float                      m_LowerBoundHeight_GL;

	//橫切面的參數
	int                        m_ClipPlaneDir;
	float                      m_ClipPlanePosX1;
	float                      m_ClipPlanePosY1;	
	float                      m_ClipPlanePosX2;
	float                      m_ClipPlanePosY2;	

	//點A的參數	
	float                      m_PointPosXA;	
	float                      m_PointPosYA;	
	float                      m_PointPosZA;	
	//點B的參數	
	float                      m_PointPosXB;	
	float                      m_PointPosYB;	
	float                      m_PointPosZB;	
	//點Now的參數	
	float                      m_PointPosXNow;	
	float                      m_PointPosYNow;	
	float                      m_PointPosZNow;	

	//fang 1040522
	size_t                     m_Max3DDisplaySize;
	float                      m_3DScale;

	//Lights, material properties	
	GLfloat	                   m_ambientProperties[4];	
	GLfloat	                   m_diffuseProperties[4];
	GLfloat	                   m_specularProperties[4];
	//---------------------------------------------------------------------------------//	
	//0.0表平行光 1.0表點光源
	GLfloat                    m_light_position[4];
	//---------------------------------------------------------------------------------//	
	//環境光
    GLfloat                    m_lightAmbient[4];
    GLfloat                    m_lightSpecular[4];
    GLfloat                    m_lightdiffuse[4];
	//---------------------------------------------------------------------------------//		
	bool                       CalcViewZoomValue(int DataW, int DataH);//計算顯示縮放比例
	//---------------------------------------------------------------------------------//	
	bool                       DoDataScaling(const int *pSrc3D, float *pDest3D, int SrcW, int SrcH, int DestW, int DestH);
	bool                       DoDataScaling(const float *pSrc3D, float *pDest3D, int SrcW, int SrcH, int DestW, int DestH);
	//---------------------------------------------------------------------------------//
	bool                       SetViewPortMatrix();
	//---------------------------------------------------------------------------------//
	void                       ResetPoint();//復歸點參數
	void                       UpdatePointA(int x, int y);//更新點A參數
	void                       UpdatePointB(int x, int y);//更新點B參數
	void                       UpdatePointNow(int x, int y);//更新點Now參數
	//---------------------------------------------------------------------------------//	
	bool                       ConvertBoxToGLBox(const TOpenGLBox &Box, TOpenGLBox &Box_GL);//將Box轉成3D下的GL-Box
	//---------------------------------------------------------------------------------//
	bool                       GetMinValueIndex(float valL, float valT, float valR, float valB, int RankIdx[], float RankVal[]);//取得最低值引數(1:L, 2:T, 3:R, 4:B);
	bool                       GetMaxValueIndex(float valL, float valT, float valR, float valB, int RankIdx[], float RankVal[]);//取得最高值引數(1:L, 2:T, 3:R, 4:B);
	//---------------------------------------------------------------------------------//
	void                       PostParentWndMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
	void                       SendParentWndMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//
	bool                       MapWndToOpenGL(int WndX, int WndY, double &PosX, double &PosY, double &PosZ);
	//---------------------------------------------------------------------------------//
	bool                       RenderSphere(float x, float y, float z, double r, int nStep =10);//繪製球體
	bool                       CalcSpherePt(float u, float v, float &x, float &y, float &z);//計算球體上的點
	//---------------------------------------------------------------------------------//
	
	// Generated message map functions
protected:
	//{{AFX_MSG(COpenGLWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLWND_H__B4AF22F7_7CD0_475E_807D_472BB143D71C__INCLUDED_)
