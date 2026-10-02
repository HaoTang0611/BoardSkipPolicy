// OpenGLWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "OpenGLWnd.h"
//-------------------------------------------------------------------------------------//
#include "SortObj.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define   MAX_HEIGHT_AT_LEFT        1
#define   MAX_HEIGHT_AT_TOP         2
#define   MAX_HEIGHT_AT_RIGHT       3
#define   MAX_HEIGHT_AT_BOTTOM      4
//-------------------------------------------------------------------------------------//
#define   MIN_HEIGHT_AT_LEFT        1
#define   MIN_HEIGHT_AT_TOP         2
#define   MIN_HEIGHT_AT_RIGHT       3
#define   MIN_HEIGHT_AT_BOTTOM      4
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd
//-------------------------------------------------------------------------------------//
COpenGLWnd::COpenGLWnd()
{
	PreInitial();	
}
//-------------------------------------------------------------------------------------//
COpenGLWnd::~COpenGLWnd()
{
	Release();
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(COpenGLWnd, CStatic)
	//{{AFX_MSG_MAP(COpenGLWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL COpenGLWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	const bool bReturn=true;
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case VK_RIGHT:
			if (m_IsClipOpen)
			{
				/*
				if ( OPENGL_CLIP_PLANE_HOR == m_ClipPlaneDir )
				{
					m_ClipPlanePosY1 += m_BoundPitch;
					m_ClipPlanePosY2 += m_BoundPitch;										
				}
				if ( OPENGL_CLIP_PLANE_VER == m_ClipPlaneDir )
				{
					m_ClipPlanePosX1 += m_BoundPitch;
					m_ClipPlanePosX2 += m_BoundPitch;					
				}*/
				m_ClipPlanePosX1 += m_BoundPitch;
				m_ClipPlanePosX2 += m_BoundPitch;
				BuildClipPlane();
				InvalidateRect(NULL,FALSE);
				PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			break;
		case VK_ADD:
			if (m_IsClipOpen)
			{
				ChangeClipPlaneSize(true);
				BuildClipPlane();
				InvalidateRect(NULL,FALSE);
				PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
				if ( true == bReturn )
				{	return TRUE; }

			}
			else
			{
				m_ScaleX *= 1.1f;
				m_ScaleY *= 1.1f;
				m_ScaleZ *= 1.1f;
				InvalidateRect(NULL,FALSE);				
			}
			//return TRUE;
			break;
		case VK_LEFT:
			if (m_IsClipOpen)
			{
				/*
				if ( OPENGL_CLIP_PLANE_HOR == m_ClipPlaneDir )
				{
					m_ClipPlanePosY1 -= m_BoundPitch;
					m_ClipPlanePosY2 -= m_BoundPitch;										
				}
				if ( OPENGL_CLIP_PLANE_VER == m_ClipPlaneDir )
				{
					m_ClipPlanePosX1 -= m_BoundPitch;
					m_ClipPlanePosX2 -= m_BoundPitch;					
				}*/
				m_ClipPlanePosX1 -= m_BoundPitch;
				m_ClipPlanePosX2 -= m_BoundPitch;	
				BuildClipPlane();
				InvalidateRect(NULL,FALSE);
				PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			break;
		case VK_SUBTRACT:
			if (m_IsClipOpen)
			{
				ChangeClipPlaneSize(false);
				BuildClipPlane();
				InvalidateRect(NULL,FALSE);
				PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
				if ( true == bReturn )
				{	return TRUE; }		
			}
			else
			{
				m_ScaleX /= 1.1f;
				m_ScaleY /= 1.1f;
				m_ScaleZ /= 1.1f;
				InvalidateRect(NULL,FALSE);
			}
			//return TRUE;
			break;
		case VK_UP:
		case 'W':
			if( m_IsUpperOpen )
			{
				m_UpperBoundHeight += m_BoundPitch;
				BuildUpperBound();
				InvalidateRect(NULL,FALSE);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			else if(m_IsLowerOpen)
			{
				m_LowerBoundHeight += m_BoundPitch;
				BuildLowerBound();
				InvalidateRect(NULL,FALSE);
				if ( true == bReturn )
				{	return TRUE; }				
			}	
			else if (m_IsClipOpen)
			{
				/*
				if ( OPENGL_CLIP_PLANE_HOR == m_ClipPlaneDir )
				{
					m_ClipPlanePosY1 += m_BoundPitch;
					m_ClipPlanePosY2 += m_BoundPitch;
				}
				if ( OPENGL_CLIP_PLANE_VER == m_ClipPlaneDir )
				{
					m_ClipPlanePosX1 += m_BoundPitch;
					m_ClipPlanePosX2 += m_BoundPitch;
				}*/
				m_ClipPlanePosY1 += m_BoundPitch;
				m_ClipPlanePosY2 += m_BoundPitch;
				BuildClipPlane();
				InvalidateRect(NULL,FALSE);
				PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			break;
		case VK_DOWN:
		case 'S':
			if(m_IsUpperOpen)
			{
				m_UpperBoundHeight -= m_BoundPitch;
				BuildUpperBound();
				InvalidateRect(NULL,FALSE);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			else if(m_IsLowerOpen)
			{
				m_LowerBoundHeight -= m_BoundPitch;
				BuildLowerBound();
				InvalidateRect(NULL,FALSE);
				PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
				if ( true == bReturn )
				{	return TRUE; }				
			}	
			else if (m_IsClipOpen)
			{
				/*
				if ( OPENGL_CLIP_PLANE_HOR == m_ClipPlaneDir )
				{
					m_ClipPlanePosY1 -= m_BoundPitch;
					m_ClipPlanePosY2 -= m_BoundPitch;
				}
				if ( OPENGL_CLIP_PLANE_VER == m_ClipPlaneDir )
				{
					m_ClipPlanePosX1 -= m_BoundPitch;
					m_ClipPlanePosX2 -= m_BoundPitch;
				}*/
				m_ClipPlanePosY1 -= m_BoundPitch;
				m_ClipPlanePosY2 -= m_BoundPitch;
				BuildClipPlane();
				InvalidateRect(NULL,FALSE);
				PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			break;
		case VK_PRIOR://Page Up
			if( m_IsUpperOpen )
			{
				m_UpperBoundHeight += (m_BoundPitch*10);
				BuildUpperBound();
				InvalidateRect(NULL,FALSE);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			else if(m_IsLowerOpen)
			{
				m_LowerBoundHeight += (m_BoundPitch*10);
				BuildLowerBound();
				InvalidateRect(NULL,FALSE);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			else if (m_IsClipOpen)
			{
				/*
				if ( OPENGL_CLIP_PLANE_HOR == m_ClipPlaneDir )
				{
					m_ClipPlanePosY1 += (m_BoundPitch*10);
					m_ClipPlanePosY2 += (m_BoundPitch*10);
				}
				if ( OPENGL_CLIP_PLANE_VER == m_ClipPlaneDir )
				{
					m_ClipPlanePosX1 += (m_BoundPitch*10);
					m_ClipPlanePosX2 += (m_BoundPitch*10);
				}*/
				BuildClipPlane();
				InvalidateRect(NULL,FALSE);
				PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			break;
		case VK_NEXT://Page Down
			if(m_IsUpperOpen)
			{
				m_UpperBoundHeight -= (m_BoundPitch*10);
				BuildUpperBound();
				InvalidateRect(NULL,FALSE);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			else if(m_IsLowerOpen)
			{
				m_LowerBoundHeight -= (m_BoundPitch*10);
				BuildLowerBound();
				InvalidateRect(NULL,FALSE);
				if ( true == bReturn )
				{	return TRUE; }				
			}	
			else if (m_IsClipOpen)
			{
				/*
				if ( OPENGL_CLIP_PLANE_HOR == m_ClipPlaneDir )
				{
					m_ClipPlanePosY1 -= (m_BoundPitch*10);
					m_ClipPlanePosY2 -= (m_BoundPitch*10);
				}
				if ( OPENGL_CLIP_PLANE_VER == m_ClipPlaneDir )
				{
					m_ClipPlanePosX1 -= (m_BoundPitch*10);
					m_ClipPlanePosX2 -= (m_BoundPitch*10);
				}*/
				BuildClipPlane();
				InvalidateRect(NULL,FALSE);
				PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
				if ( true == bReturn )
				{	return TRUE; }				
			}
			break;
		}
		break;
	}	
	return CStatic::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	cx = MAX(1, cx);
	cy = MAX(1, cy);
	CreateOpenGLDC();
	wglMakeCurrent(m_hDC, m_hGLContext);

	GLsizei width,height;
	width = cx;
	height = cy;
	glViewport( 0, 0, width, height);            //當視窗長寬改變時，畫面也跟著變 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	float OrRange = 600;
	
	if( width <= height )
	{
		m_Ortho_Left = -OrRange;
		m_Ortho_Right = OrRange;
		m_Ortho_Bot = -OrRange * (float)height/(float)width;
		m_Ortho_Top = OrRange * (float)height/(float)width;
		m_Ortho_Near = OrRange*5;
		m_Ortho_Far = -OrRange*5;
	}
	else
	{
		m_Ortho_Left = -OrRange*(float)width/(float)height;
		m_Ortho_Right = OrRange*(float)width/(float)height;
		m_Ortho_Bot = -OrRange;
		m_Ortho_Top = OrRange;
		m_Ortho_Near = OrRange*5;
		m_Ortho_Far = -OrRange*5;
	}
	//色尺
	m_RulerLB.z = (float)(m_RulerRT.z = -m_Ortho_Near);
	m_RulerLB.x = (float)(m_Ortho_Right - 150);
	m_RulerRT.x = (float)(m_Ortho_Right - 100);
	m_RulerLB.y = (float)(m_Ortho_Bot + 200);
	m_RulerRT.y = (float)(m_Ortho_Top - 100);
	
	glOrtho(m_Ortho_Left,m_Ortho_Right, m_Ortho_Bot, m_Ortho_Top, m_Ortho_Near, m_Ortho_Far);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	glDrawBuffer(GL_BACK);
	
	//環境光         
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightSpecular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightdiffuse);
	
	//點光源
	glLightfv( GL_LIGHT1, GL_AMBIENT, m_ambientProperties);
	glLightfv( GL_LIGHT1, GL_DIFFUSE, m_diffuseProperties);
	glLightfv( GL_LIGHT1, GL_SPECULAR, m_specularProperties);
	glLightfv( GL_LIGHT1, GL_POSITION, m_light_position);
	
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	
	//	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,m_diffuseProperties);
	
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
		
	Build3DObject();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::InitialDisplay()	
{	
	// TODO: Add your message handler code here
	CreateOpenGLDC();
	SwitchMultiLanguage();
	wglMakeCurrent(m_hDC, m_hGLContext);

	RECT Rect={0};
	this->GetClientRect(&Rect);
	int cx=0, cy=0;
	cx = Rect.right-Rect.left;
	cy = Rect.bottom-Rect.top;
	GLsizei width,height;
	width = cx;
	height = cy;
	glViewport( 0, 0, width, height);            //當視窗長寬改變時，畫面也跟著變 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	float OrRange = 600;
	
	if( width <= height )
	{
		m_Ortho_Left = -OrRange;
		m_Ortho_Right = OrRange;
		m_Ortho_Bot = -OrRange * (float)height/(float)width;
		m_Ortho_Top = OrRange * (float)height/(float)width;
		m_Ortho_Near = OrRange*5;
		m_Ortho_Far = -OrRange*5;
	}
	else
	{
		m_Ortho_Left = -OrRange*(float)width/(float)height;
		m_Ortho_Right = OrRange*(float)width/(float)height;
		m_Ortho_Bot = -OrRange;
		m_Ortho_Top = OrRange;
		m_Ortho_Near = OrRange*5;
		m_Ortho_Far = -OrRange*5;
	}
	//色尺
	m_RulerLB.z = (float)(m_RulerRT.z = -m_Ortho_Near);
	m_RulerLB.x = (float)(m_Ortho_Right - 250);
	m_RulerRT.x = (float)(m_Ortho_Right - 150);
	m_RulerLB.y = (float)(m_Ortho_Bot + 200);
	m_RulerRT.y = (float)(m_Ortho_Top - 100);
	
	glOrtho(m_Ortho_Left,m_Ortho_Right, m_Ortho_Bot, m_Ortho_Top, m_Ortho_Near, m_Ortho_Far);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	glDrawBuffer(GL_BACK);
	
	//環境光     
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightSpecular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightdiffuse);
	
	//點光源
	glLightfv( GL_LIGHT1, GL_AMBIENT, m_ambientProperties);
	glLightfv( GL_LIGHT1, GL_DIFFUSE, m_diffuseProperties);
	glLightfv( GL_LIGHT1, GL_SPECULAR, m_specularProperties);
	glLightfv( GL_LIGHT1, GL_POSITION, m_light_position);	

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	
	//	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,m_diffuseProperties);
	
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
		
	Build3DObject();
	InvalidateRect(NULL,FALSE);

}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	wglMakeCurrent(m_hDC, m_hGLContext);
	RenderScene();
	SwapBuffers(dc.m_ps.hdc);
	// Do not call CStatic::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	if ( this->GetCapture() != this ) 
	{
		UpdatePointNow(point.x, point.y);
		CStatic::OnMouseMove(nFlags, point);
		return;
	}
	if((nFlags&MK_LBUTTON))
	{
		CSize rotate = m_LastPos - point;
		m_LastPos = point;
		m_yRotate += rotate.cx;
		m_xRotate += rotate.cy;
		InvalidateRect(NULL,FALSE);
	}
	else if((nFlags&MK_RBUTTON))
	{
		CSize translate = m_LastPos - point;
		m_LastPos = point;
		double TempF1 = (m_Ortho_Right - m_Ortho_Left);
		TempF1 *= 0.001;
		
		m_xTranslate -= translate.cx*TempF1;
		m_yTranslate += translate.cy*TempF1;
		InvalidateRect(NULL,FALSE);
	}
	CStatic::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	this->SetCapture();
	m_LastPos = m_LeftDownPos = point;
	
	CStatic::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	::ReleaseCapture();

	const int dX=m_LeftDownPos.x-point.x;
	const int dY=m_LeftDownPos.y-point.y;
	m_LastPos = point;
	if ( abs(dX)<2 && abs(dY)<2 )	
	{
		double PosX=0, PosY=0, PosZ=0;
		MapWndToOpenGL(point.x, point.y, PosX, PosY, PosZ);				
		if (  OPENGL_CLIP_PLANE_HOR == m_ClipPlaneDir )
		{
			m_ClipPlanePosY1 = (float)(PosY+(m_3DHeight/2));
			m_ClipPlanePosY2 = (float)(PosY+(m_3DHeight/2));
		}
		if (  OPENGL_CLIP_PLANE_VER == m_ClipPlaneDir )
		{
			m_ClipPlanePosX1 = (float)(PosX+(m_3DWidth/2));
			m_ClipPlanePosX2 = (float)(PosX+(m_3DWidth/2));
		}
		if ( OPENGL_CLIP_PLANE_ANY == m_ClipPlaneDir )
		{
			m_ClipPlanePosX1 = (float)(PosX+(m_3DWidth/2));
			m_ClipPlanePosY1 = (float)(PosY+(m_3DHeight/2));
		}

		BuildClipPlane();
		Invalidate();
		UpdatePointA(point.x, point.y);
		PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
	}
	CStatic::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	this->SetCapture();
	m_LastPos = m_RightDownPos = point;
	CStatic::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	//增加回傳函式	
	//::SendMessage(this->GetParent()->GetSafeHwnd(),WM_RBUTTONUP,nFlags,MAKELPARAM(point.x,point.y));
	const int dX=m_RightDownPos.x-point.x;
	const int dY=m_RightDownPos.y-point.y;
	m_LastPos = point;
	if ( abs(dX)<2 && abs(dY)<2 )	
	{
		double PosX=0, PosY=0, PosZ=0;
		MapWndToOpenGL(point.x, point.y, PosX, PosY, PosZ);				
		if (  OPENGL_CLIP_PLANE_HOR == m_ClipPlaneDir )
		{
			m_ClipPlanePosY1 = (float)(PosY+(m_3DHeight/2));
			m_ClipPlanePosY2 = (float)(PosY+(m_3DHeight/2));
		}
		if (  OPENGL_CLIP_PLANE_VER == m_ClipPlaneDir )
		{
			m_ClipPlanePosX1 = (float)(PosX+(m_3DWidth/2));
			m_ClipPlanePosX2 = (float)(PosX+(m_3DWidth/2));
		}
		if ( OPENGL_CLIP_PLANE_ANY == m_ClipPlaneDir )
		{
			m_ClipPlanePosX2 = (float)(PosX+(m_3DWidth/2));
			m_ClipPlanePosY2 = (float)(PosY+(m_3DHeight/2));
		}
		BuildClipPlane();
		Invalidate();
		UpdatePointB(point.x, point.y);
		PostParentWndMsg(MSG_OPEN_GL_WND, WPARAM_UPDATE_CLIP_PLANE, NULL);
	}
	CStatic::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL COpenGLWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	RECT Rect={0};
	this->GetClientRect(&Rect);
	POINT TempPt = pt;
	::ScreenToClient(this->GetSafeHwnd(), &TempPt);
	if( ::PtInRect(&Rect, TempPt) == FALSE ) { return NULL; }
	if(zDelta<0)
	{
		m_ScaleX *= 1.1f;
		m_ScaleY *= 1.1f;
		m_ScaleZ *= 1.1f;
		InvalidateRect(NULL,FALSE);
	}
	else
	{
		m_ScaleX /= 1.1f;
		m_ScaleY /= 1.1f;
		m_ScaleZ /= 1.1f;
		InvalidateRect(NULL,FALSE);
	}	
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
LRESULT COpenGLWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case WM_MOUSEACTIVATE:
		this->SetFocus();
		break;
	case WM_RBUTTONUP:
		SendParentWndMsg(message, wParam, lParam);		
		break;
	case WM_RBUTTONDOWN:
		SendParentWndMsg(message, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		SendParentWndMsg(message, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		SendParentWndMsg(message, wParam, lParam);
		break;
	}
	return CStatic::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	this->ModifyStyle(0, SS_NOTIFY, 0);

	CStatic::PreSubclassWindow();
}
//-------------------------------------------------------------------------------------//
int COpenGLWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here	
	CreateOpenGLDC();	
	return 0;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::PreInitial()
{
	RECT Rect = {0,0,0,0};

	m_2DData = NULL;
	m_3DData = NULL;
	m_Max2DBufferSize = 0;
	m_Max3DBufferSize = 0;	

	m_ShowMode = OPENGL_SHOW_MODEL;
	m_ShowString = _T("");

	m_ErrorString = _T("");
	m_strUpper = _T("Upper");
	m_strLower = _T("Lower");
	m_strHeight = _T("H");
	m_strObject = _T("Object");
	//-------------------------------------//
	//GL Window 參數
	//-------------------------------------//
	m_hDC = NULL;			
	m_hGLContext = NULL;
	m_GLPixelIndex = 0;

	m_Ortho_Left = m_Ortho_Right = m_Ortho_Bot = m_Ortho_Top = m_Ortho_Near = m_Ortho_Far = 0;


	//縮放比例
#ifdef _DEBUG
	m_ScaleX_Fit = 2.0;
	m_ScaleY_Fit = 2.0;
	m_ScaleZ_Fit = 2.0;
#else
	m_ScaleX_Fit = 1.0;
	m_ScaleY_Fit = 1.0;
	m_ScaleZ_Fit = 1.0;
#endif//
	m_ScaleX = m_ScaleX_Fit;
	m_ScaleY = m_ScaleY_Fit;
	m_ScaleZ = m_ScaleZ_Fit;
	//旋轉
	m_xRotate = 0.0;
	m_yRotate = 0.0;	

	m_xRotate = -90;
	m_yRotate = 0;	

	//平移
	m_xTranslate = 0.0;
	m_yTranslate = 0.0;
	m_LastPos.x = m_LastPos.y = 0;
	m_RightDownPos.x = m_RightDownPos.y = 0;
	m_LeftDownPos.x = m_LeftDownPos.y = 0;	

	m_RulerLB.x = m_RulerLB.y = m_RulerLB.z = 0;
	m_RulerRT.x = m_RulerRT.y = m_RulerRT.z = 0;	//高度尺的3D座標

	m_MainObjectMode = OPENGL_OBJECT_TEXTURE;
	m_IsShowColorRuler = false;
	m_IsShowPadHeight = false;
	m_IsShowPadRegion = false;	
	m_IsShowRoiRegion = false;
	m_IsShowGLBoxList = false;
	m_IsShowMainObject = true;
	m_IsBaseOpen = false;
	m_IsUpperOpen = false;
	m_IsLowerOpen = false;
	m_IsClipOpen = false;	
	m_IsShowPoint = true;
	m_IsShowAxis = true;	
	m_AnimationPlay = false;

	//材質 Buffer
	m_TextureNumber = 1;
	m_TextureIndex = NULL;
	m_TextureReducePriorities = NULL;

	m_TextureIndex = new unsigned int[m_TextureNumber];		//用來儲存材質物件的編號 
	m_TextureReducePriorities = new float[m_TextureNumber]; //刪除材質物件的順序 
	m_TextureReducePriorities[0]=1.0f;
	//-------------------------------------//
	//畫圖資料
	m_ScaleW = 1.0f;
	m_ScaleH = 1.0f;

	m_Resolution = (float)(1/10.9);
	m_Resolution = 0.15f;
	m_MaxH = m_MinH = 0;
	m_RulerMinH = m_RulerMaxH = 0;
	m_RulerMinH_GL = m_RulerMaxH_GL = 0;	//3D 世界座標

	m_ShowMinH = m_ShowMaxH = 0;
	m_ShowMinH_GL = m_ShowMaxH_GL = 0;	
	
	m_3DData = NULL;
	m_IsColor = false;
	m_DetailLevel = OPEN_GL_FINENESS_HIGHT;
	m_3DWidth = m_3DHeight = 0;

	//材質縮圖
	m_2DData = NULL;
	m_Max2DBufferSize = 0;
	m_TextureSize = 0;//幾X幾 //256 ,128  
	m_TextureScale = 1.0f;
	m_TextureWidth = 0;
	m_TextureHeight = 0;
	m_TextureRealW = 0;

	//上下邊界目前的實際高度值
	m_BoundPitch = 10.0f;
	m_UpperBoundHeight = -99999.0f;
	m_UpperBoundHeight_GL = 0.0f;
	m_LowerBoundHeight = -99999.0f;
	m_LowerBoundHeight_GL = 0.0f;
	
#ifdef _X64
	#ifdef _DEBUG
		m_Max3DDisplaySize = 512*512;		
	#else
		m_Max3DDisplaySize = 1024*1024;
		m_Max3DDisplaySize = 1024*2048;//Default
		m_Max3DDisplaySize = 2048*2048;
		m_Max3DDisplaySize = 2048*2048*2;
	#ifdef LABORATORY_VERSION
		m_Max3DDisplaySize = 4096*3072;
		m_Max3DDisplaySize = 5124*5124;
	#endif//LABORATORY_VERSION
	#endif//_DEBUG
#else
	m_Max3DDisplaySize = 512*512;
#endif

	m_3DScale = 1;

	m_UserResolution = 0;
	this->m_ambientProperties[0]=0.35f;
	this->m_ambientProperties[1]=0.35f;
	this->m_ambientProperties[2]=0.35f;
	this->m_ambientProperties[3]=1.0f;	

	this->m_diffuseProperties[0]=0.4f;
	this->m_diffuseProperties[1]=0.4f;
	this->m_diffuseProperties[2]=0.4f;
	this->m_diffuseProperties[3]=1.0f;

	this->m_specularProperties[0]=0.7f;
	this->m_specularProperties[1]=0.7f;
	this->m_specularProperties[2]=0.7f;
	this->m_specularProperties[3]=1.0f;

	//0.0表平行光 1.0表點光源
	this->m_light_position[0]=0.0f;
	this->m_light_position[1]=300.0f;
	this->m_light_position[2]=0.0f;
	this->m_light_position[3]=0.0f;

	this->m_light_position[0]= 0.0f;
	this->m_light_position[1]= 0.0f;
	this->m_light_position[2]=-1.0f;
	this->m_light_position[3]= 0.0f;

	
	//環境光
    this->m_lightAmbient[0]=0.05f;
	this->m_lightAmbient[1]=0.05f;
	this->m_lightAmbient[2]=0.05f;
	this->m_lightAmbient[3]=1.0f;

    this->m_lightSpecular[0]=0.3f;
	this->m_lightSpecular[1]=0.3f;
	this->m_lightSpecular[2]=0.3f;
	this->m_lightSpecular[3]=1.0f;

    this->m_lightdiffuse[0]=0.05f;
	this->m_lightdiffuse[1]=0.05f;
	this->m_lightdiffuse[2]=0.05f;
	this->m_lightdiffuse[3]=1.0f;
	
	m_ClipPlanePosX1 = 0;
	m_ClipPlanePosY1 = 0;	
	m_ClipPlanePosX2 = 0;
	m_ClipPlanePosY2 = 0;
	m_ClipPlaneDir = OPENGL_CLIP_PLANE_HOR;

	m_PointPosXA = m_PointPosYA = m_PointPosZA = 0;	
	m_PointPosXB = m_PointPosYB = m_PointPosZB = 0;	
	m_PointPosXNow = m_PointPosYNow = m_PointPosZNow = 0;	
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::Release()
{	
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);
	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}	
	
	//材質
	delete []m_TextureIndex;m_TextureIndex=NULL;
	delete []m_TextureReducePriorities;m_TextureReducePriorities=NULL;

	ReleaseAllBuffer();	
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::CreateOpenGLDC()
{
	if( m_hDC == NULL )
	{
		m_hDC = GetDC()->m_hDC;
		
		if(SetWindowPixelFormat(m_hDC)==FALSE)
		{
			m_hDC= m_hDC;
		}
		
		if(CreateViewGLContext(m_hDC)==FALSE)
		{
			m_hDC= m_hDC;
		}
	}

}
//-------------------------------------------------------------------------------------//
int COpenGLWnd::GetOpenGLTextureSize()//取得OpenGL材質的大小
{
	int Size=256;
	switch ( m_DetailLevel )
	{
	case OPEN_GL_FINENESS_HIGHT_MOST:	Size = 4096;	break;
	case OPEN_GL_FINENESS_HIGHT_MORE:	Size = 2048;	break;
	case OPEN_GL_FINENESS_HIGHT:		Size = 1024;	break;
	case OPEN_GL_FINENESS_MIDDLE:		Size = 512;		break;
	case OPEN_GL_FINENESS_LOW:			Size = 512;		break;
	case OPEN_GL_FINENESS_LOW_MORE:		Size = 256;		break;
	case OPEN_GL_FINENESS_LOW_MOST:		Size = 256;		break;
	default:
		Size = 256;
		break;
	}
	return Size;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::CreateDataBuffer(unsigned int DataW, unsigned int DataH, bool IsColor)
{//fang 1040521
	m_IsColor = IsColor;

//3D
	m_3DScale = 1;
	size_t DataSize = DataW * DataH;
	size_t Max3DDisplaySize = m_Max3DDisplaySize;
	if ( m_DetailLevel > 0 ) 
	{	Max3DDisplaySize = m_Max3DDisplaySize/m_DetailLevel;	}
	if( DataSize > Max3DDisplaySize )
	{
		m_3DScale = (float)Max3DDisplaySize / (float)DataSize;
		m_3DScale = (float)(sqrt(m_3DScale));

		m_3DWidth = (unsigned int)(DataW*m_3DScale);
		m_3DHeight = (unsigned int)(DataH*m_3DScale);
	}
	else
	{//維持原樣
		m_3DScale = 1;	
		m_3DWidth = DataW;
		m_3DHeight = DataH;
	}

	const size_t Size3D = m_3DWidth * m_3DHeight;
	if( Size3D > m_Max3DBufferSize )
	{
		delete[] m_3DData;	m_3DData = NULL;
		m_3DData = new float[Size3D];
		m_Max3DBufferSize = Size3D;
	}	
	::memset(m_3DData, 0x00, sizeof(float)*Size3D);

//2D
	//2D
	//材質縮圖
	m_TextureScale = 1;
	bool bUseNewTextureSize=true;	
	if ( false == bUseNewTextureSize )
	{
		if( (DataW > 256)||(DataH > 256))
		{//TextureSize = 256
			m_TextureSize = 256;
			if( DataW > DataH )
			{//將Width縮到 texturesize
				m_TextureScale = 	(float)m_TextureSize / (float)DataW;
				m_TextureWidth =	(size_t)(DataW*m_TextureScale);
				m_TextureHeight =	(size_t)(DataH*m_TextureScale);
			}
			else
			{//將height縮到 texturesize
				m_TextureScale = (float)m_TextureSize / (float)DataH;
				m_TextureWidth = (size_t)(DataW*m_TextureScale);
				m_TextureHeight = (size_t)(DataH*m_TextureScale);
			}		
		}
		else if( (DataW > 128)||(DataH >128) )
		{//TextureSize = 128
			m_TextureSize = 128;
			if( DataW > DataH )
			{//將Width縮到 texturesize
				m_TextureScale = 	(float)m_TextureSize / (float)DataW;
				m_TextureWidth =	(size_t)(DataW*m_TextureScale);
				m_TextureHeight =	(size_t)(DataH*m_TextureScale);
			}
			else
			{//將height縮到 texturesize
				m_TextureScale = (float)m_TextureSize / (float)DataH;
				m_TextureWidth = (size_t)(DataW*m_TextureScale);
				m_TextureHeight = (size_t)(DataH*m_TextureScale);
			}			
		}
		else
		{//維持原樣
			m_TextureScale = 1;	
			m_TextureWidth = DataW;
			m_TextureHeight = DataH;
		}
	}
	else
	{
		const int TextureSize=GetOpenGLTextureSize();
		if( (DataW > TextureSize)||(DataH > TextureSize))
		{
			m_TextureSize = TextureSize;
			if( DataW > DataH )
			{//將Width縮到 texturesize
				m_TextureScale = 	(float)m_TextureSize / (float)DataW;
				m_TextureWidth =	(size_t)(DataW*m_TextureScale);
				m_TextureHeight =	(size_t)(DataH*m_TextureScale);
			}
			else
			{//將height縮到 texturesize
				m_TextureScale = (float)m_TextureSize / (float)DataH;
				m_TextureWidth = (size_t)(DataW*m_TextureScale);
				m_TextureHeight = (size_t)(DataH*m_TextureScale);
			}		
		}
		else
		{
			m_TextureScale = 1;	
			m_TextureWidth = DataW;
			m_TextureHeight = DataH;
		}
	}
	
	//算出RealW
	if(IsColor == true)
	{	m_TextureRealW = GetNBytesPerLine(m_TextureWidth*3); }
	else
	{	m_TextureRealW = GetNBytesPerLine(m_TextureWidth);	}	
	const size_t Size2D = m_TextureRealW*m_TextureHeight;	
	if( Size2D > m_Max2DBufferSize )
	{
		delete []m_2DData; m_2DData=NULL;
		m_2DData =new unsigned char[Size2D];
		m_Max2DBufferSize = Size2D;
	}
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::ReleaseAllBuffer()
{
	if( m_3DData != NULL )
	{
		delete[] m_3DData;	m_3DData = NULL;
		m_Max3DBufferSize = 0;
	}
	
	if( m_2DData != NULL )
	{
		delete[] m_2DData; m_2DData = NULL;
		m_Max2DBufferSize = 0;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::Build3DObject()
{
	// BackColor
	wglMakeCurrent(m_hDC, m_hGLContext);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	
	// TODO: Add your specialized creation code here
	CString str;
	CString fnName = _T("COpenGLWnd::Build3DObject");
	double fnTime=0.0;
	LARGE_INTEGER fnStart, fnEnd;		

	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
	glShadeModel(GL_SMOOTH);

//	if( m_3DData == NULL ){ return true; }

	//建Model
	JetAPI::SetFuncTimeStart(fnStart);
	SetTextureObject();
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [SetTextureObject] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildColorRuler();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildColorRuler] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildMainObject();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildMainObject] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildPadRegion();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildPadRegion] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildRoiRegion();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildRoiRegion] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildGLBoxList();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildGLBoxList] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildBaseBound();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildBaseBound] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildUpperBound();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildUpperBound] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildLowerBound();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildLowerBound] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildClipPlane();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildClipPlane] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);

	JetAPI::SetFuncTimeStart(fnStart);
	BuildAxis();//++++
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [BuildAxis] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);	

	JetAPI::SetFuncTimeStart(fnStart);
	InvalidateRect(NULL,FALSE);	
	this->OnPaint();
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("%s [OnPaint] fnTime=%.3f ms"), fnName, fnTime);
	//AOIDataCollect.SaveCurrentProcess(str);
	return true;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetTextureObject()
{
	glGenTextures( m_TextureNumber, m_TextureIndex); 
	glPrioritizeTextures( m_TextureNumber, m_TextureIndex, m_TextureReducePriorities); 
	if(m_2DData != NULL)
	{
		glBindTexture(GL_TEXTURE_2D,1); 
		if( m_IsColor )
		{
			glTexImage2D(GL_TEXTURE_2D,0,3,m_TextureWidth,m_TextureHeight,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,m_2DData); 
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,0,GL_INTENSITY8,m_TextureWidth,m_TextureHeight,0,GL_LUMINANCE ,GL_UNSIGNED_BYTE,m_2DData); //亮度
		}  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	}
}	
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildBaseBound()
{	
	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (m_3DWidth*0.5f);
	OffsetH = (m_3DHeight*0.5f);

	TPOINT3F PtLB, PtLT, PtRB, PtRT;

	PtLB.x = 0 - OffsetW;
	PtLT.x = 0 - OffsetW;
	PtRB.x = (m_3DWidth-1) - OffsetW;
	PtRT.x = (m_3DWidth-1) - OffsetW;

	PtLB.z = 0 - OffsetH;
	PtRB.z = 0 - OffsetH;
	PtLT.z = (m_3DHeight-1) - OffsetH;
	PtRT.z = (m_3DHeight-1) - OffsetH;

	float BaseBoundHeight_GL = 0;

	::glNewList(OPENGL_OBJ_BASE_PLANE,GL_COMPILE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); 	
	glBegin(GL_POLYGON);
	glColor3f(0.9f,0.9f,0.9f);//fang 1030124
	glVertex3f( PtLB.x, BaseBoundHeight_GL, PtLB.z);
	glVertex3f( PtRB.x, BaseBoundHeight_GL, PtRB.z);
	glVertex3f( PtRT.x, BaseBoundHeight_GL, PtRT.z);
	glVertex3f( PtLT.x, BaseBoundHeight_GL, PtLT.z);
	glEnd();
	glDisable(GL_BLEND);

	::glEndList();

	GLenum errCode;
	const GLubyte *errString;
	errCode = glGetError();
	if(errCode!=GL_NO_ERROR)
	{
		errString =gluErrorString(errCode);
		this->m_ErrorString=errString;		
	}
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildUpperBound()
{	
	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (m_3DWidth*0.5f);
	OffsetH = (m_3DHeight*0.5f);

	TPOINT3F PtLB, PtLT, PtRB, PtRT;

	PtLB.x = 0 - OffsetW;
	PtLT.x = 0 - OffsetW;
	PtRB.x = (m_3DWidth-1) - OffsetW;
	PtRT.x = (m_3DWidth-1) - OffsetW;

	PtLB.z = 0 - OffsetH;
	PtRB.z = 0 - OffsetH;
	PtLT.z = (m_3DHeight-1) - OffsetH;
	PtRT.z = (m_3DHeight-1) - OffsetH;

	m_UpperBoundHeight_GL = m_UpperBoundHeight*m_Resolution;

	::glNewList(OPENGL_OBJ_UPPER_PLANE,GL_COMPILE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); 	
	glBegin(GL_POLYGON);
	glColor3f(0.9f,0.9f,0.9f);//fang 1030124
	glVertex3f( PtLB.x, m_UpperBoundHeight_GL, PtLB.z);
	glVertex3f( PtRB.x, m_UpperBoundHeight_GL, PtRB.z);
	glVertex3f( PtRT.x, m_UpperBoundHeight_GL, PtRT.z);
	glVertex3f( PtLT.x, m_UpperBoundHeight_GL, PtLT.z);
	glEnd();
	glDisable(GL_BLEND);

	::glEndList();
	
	GLenum errCode;
	const GLubyte *errString;
	errCode = glGetError();
	if(errCode!=GL_NO_ERROR)
	{
		errString =gluErrorString(errCode);
		this->m_ErrorString=errString;		
	}
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildLowerBound()
{
	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (m_3DWidth*0.5f);
	OffsetH = (m_3DHeight*0.5f);

	TPOINT3F PtLB, PtLT, PtRB, PtRT;

	PtLB.x = 0 - OffsetW;
	PtLT.x = 0 - OffsetW;
	PtRB.x = (m_3DWidth-1) - OffsetW;
	PtRT.x = (m_3DWidth-1) - OffsetW;

	PtLB.z = 0 - OffsetH;
	PtRB.z = 0 - OffsetH;
	PtLT.z = (m_3DHeight-1) - OffsetH;
	PtRT.z = (m_3DHeight-1) - OffsetH;

	m_LowerBoundHeight_GL = m_LowerBoundHeight * m_Resolution;

	::glNewList(OPENGL_OBJ_LOWER_PLANE,GL_COMPILE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); 	
	glBegin(GL_POLYGON);
	glColor3f(0.8f,0.8f,0.8f);//fang 1030124
	glVertex3f( PtLB.x, m_LowerBoundHeight_GL, PtLB.z);
	glVertex3f( PtRB.x, m_LowerBoundHeight_GL, PtRB.z);
	glVertex3f( PtRT.x, m_LowerBoundHeight_GL, PtRT.z);
	glVertex3f( PtLT.x, m_LowerBoundHeight_GL, PtLT.z);
	glEnd();	
	glDisable(GL_BLEND);

	::glEndList();

	GLenum errCode;
	const GLubyte *errString;
	errCode = glGetError();
	if(errCode!=GL_NO_ERROR)
	{
		errString =gluErrorString(errCode);
		this->m_ErrorString=errString;		
	}
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildClipPlane()
{
	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (m_3DWidth*0.5f);
	OffsetH = (m_3DHeight*0.5f);

	TPOINT3F PtLB, PtLT, PtRB, PtRT;

	PtLB.x = m_ClipPlanePosX1 - OffsetW;//寬度
	PtLT.x = m_ClipPlanePosX1 - OffsetW;
	PtRB.x = m_ClipPlanePosX2 - OffsetW;
	PtRT.x = m_ClipPlanePosX2 - OffsetW;

	PtLB.z = m_ClipPlanePosY1 - OffsetH;//長度
	PtRB.z = m_ClipPlanePosY2 - OffsetH;
	PtLT.z = m_ClipPlanePosY1 - OffsetH;
	PtRT.z = m_ClipPlanePosY2 - OffsetH;

	PtLB.y = m_ShowMinH*m_Resolution;//高度
	PtRB.y = m_ShowMinH*m_Resolution;
	PtRT.y = m_ShowMaxH*m_Resolution*1.2f;
	PtLT.y = m_ShowMaxH*m_Resolution*1.2f;

	::glNewList(OPENGL_OBJ_CLIP_PLANE,GL_COMPILE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); 	
	glBegin(GL_POLYGON);
	glColor3f(0.8f,0.8f,0.8f);//fang 1030124
	glVertex3f( PtLB.x, PtLB.y, PtLB.z);
	glVertex3f( PtRB.x, PtRB.y, PtRB.z);
	glVertex3f( PtRT.x, PtRT.y, PtRT.z);
	glVertex3f( PtLT.x, PtLT.y, PtLT.z);
	glEnd();	
	glDisable(GL_BLEND);
	::glEndList();

	
	GLenum errCode;
	const GLubyte *errString;
	errCode = glGetError();
	if(errCode!=GL_NO_ERROR)
	{
		errString =gluErrorString(errCode);
		this->m_ErrorString=errString;		
	}
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildPoint(double PosX, double PosY, double Height, int ObjectID)
{
	float OffsetW = 0.0f;
	float OffsetH = 0.0f;	
	OffsetW = (10*m_Resolution);
	OffsetH = (10*m_Resolution);

	GLfloat clrR=1.0f, clrG=1.0f, clrB=1.0f;
	TPOINT3F PtLB, PtLT, PtRB, PtRT;
	const float UpperHeight= (float)(Height+2.0);
	const float LowerHeight= (float)(Height-2.0);

	PtLB.x = (float)(PosX - OffsetW);//寬度
	PtLT.x = (float)(PosX - OffsetW);
	PtRB.x = (float)(PosX + OffsetW);
	PtRT.x = (float)(PosX + OffsetW);

	PtLB.z = (float)(PosY - OffsetH);//長度
	PtRB.z = (float)(PosY - OffsetH);
	PtLT.z = (float)(PosY + OffsetH);
	PtRT.z = (float)(PosY + OffsetH);

	switch ( ObjectID )
	{
	case OPENGL_OBJ_POINT_A:
		clrR = 1.0;	clrG = 0.0f;	clrB = 0.0f;
		break;
	case OPENGL_OBJ_POINT_B:
		clrR = 0.0f;	clrG = 1.0f;	clrB = 0.0f;
		break;
	case OPENGL_OBJ_POINT_NOW:
		clrR = 0.8f;	clrG = 0.8f;	clrB = 0.8f;
		break;
	default:
		break;
	}
	::glNewList(ObjectID,GL_COMPILE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); 	
	/*
	PtLB.y = UpperHeight;//高度
	PtRB.y = UpperHeight;
	PtRT.y = UpperHeight;
	PtLT.y = UpperHeight;
	
	glBegin(GL_POLYGON);
	glColor3f(clrR,clrG,clrB);//fang 1030124
	glVertex3f( PtLB.x, PtLB.y, PtLB.z);
	glVertex3f( PtRB.x, PtRB.y, PtRB.z);
	glVertex3f( PtRT.x, PtRT.y, PtRT.z);
	glVertex3f( PtLT.x, PtLT.y, PtLT.z);
	glEnd();	
	
	PtLB.y = LowerHeight;//高度
	PtRB.y = LowerHeight;
	PtRT.y = LowerHeight;
	PtLT.y = LowerHeight;

	glBegin(GL_POLYGON);
	glColor3f(clrR,clrG,clrB);//fang 1030124
	glVertex3f( PtLB.x, PtLB.y, PtLB.z);
	glVertex3f( PtRB.x, PtRB.y, PtRB.z);
	glVertex3f( PtRT.x, PtRT.y, PtRT.z);
	glVertex3f( PtLT.x, PtLT.y, PtLT.z);
	glEnd();	

	glBegin(GL_LINES);
	//glColor3ub(155,255,255);
	glColor3f(clrR,clrG,clrB);
	glVertex3f(PtLB.x, LowerHeight, PtLB.z);	glVertex3f(PtRB.x, LowerHeight, PtRB.z);
	glVertex3f(PtRB.x, LowerHeight, PtRB.z);	glVertex3f(PtRT.x, LowerHeight, PtRT.z);
	glVertex3f(PtRT.x, LowerHeight, PtRT.z);	glVertex3f(PtLT.x, LowerHeight, PtLT.z);
	glVertex3f(PtLT.x, LowerHeight, PtLT.z);	glVertex3f(PtLB.x, LowerHeight, PtLB.z);

	glVertex3f(PtLB.x, UpperHeight, PtLB.z);	glVertex3f(PtRB.x, UpperHeight, PtRB.z);
	glVertex3f(PtRB.x, UpperHeight, PtRB.z);	glVertex3f(PtRT.x, UpperHeight, PtRT.z);
	glVertex3f(PtRT.x, UpperHeight, PtRT.z);	glVertex3f(PtLT.x, UpperHeight, PtLT.z);
	glVertex3f(PtLT.x, UpperHeight, PtLT.z);	glVertex3f(PtLB.x, UpperHeight, PtLB.z);

	glVertex3f(PtLB.x, LowerHeight, PtLB.z);	glVertex3f(PtLB.x, UpperHeight, PtLB.z);
	glVertex3f(PtRB.x, LowerHeight, PtRB.z);	glVertex3f(PtRB.x, UpperHeight, PtRB.z);
	glVertex3f(PtRT.x, LowerHeight, PtRT.z);	glVertex3f(PtRT.x, UpperHeight, PtRT.z);
	glVertex3f(PtLT.x, LowerHeight, PtLT.z);	glVertex3f(PtLT.x, UpperHeight, PtLT.z);
	glEnd();
	*/
	float fposX = 0.0f;
	float fposY = 0.0f;
	float fposZ = 0.0f;
	float Raduis = 1.0f;
	int   nStep = 10;
	fposX = (float)PosX;
	fposY = (float)Height+2.0f;
	fposZ = (float)PosY;
	Raduis = 2.0f;
	glColor3f(clrR, clrG, clrB);
	RenderSphere(fposX, fposY, fposZ, Raduis, nStep);

	glDisable(GL_BLEND);
	::glEndList();
	
	GLenum errCode;
	const GLubyte *errString;
	errCode = glGetError();
	if(errCode!=GL_NO_ERROR)
	{
		errString =gluErrorString(errCode);
		this->m_ErrorString=errString;		
	}
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::BuildAxis()//繪製三軸
{	
	GLfloat clrR=1.0f, clrG=1.0f, clrB=1.0f;
	GLfloat clrR2=1.0f, clrG2=1.0f, clrB2=1.0f;//line color
	int i=0;
	TPOINT3F a, b, c, d;
	
	float px=0, py=0, pz=0;
	double u=0.0, v=0.0, angle=0;
	const int nStep = 12;	
	const float Extend = 15.0f;
	const float Radius = 5.0f;
	const float Radius2 = 2.0f;
	const double PI = PI_RAD;
	const double PI2 = PI * 2;	
	const double Step = PI2/(double)nStep;
	const float w = m_3DWidth;
	const float h = m_3DHeight;
	const float ox=(-w/2);
	const float oy=0.0f;
	const float oz=(-h/2);

	//ox = -ox;
	//oz = -oz;
	::glNewList(OPENGL_OBJ_AXIS,GL_COMPILE);

	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE); 	
	
	//X-Axis
	clrR=1.0f;
	clrG=0.0f;
	clrB=0.0f;
	glColor3f(clrR, clrG, clrB);
	{		
		float x = (float)(-ox)+Extend;
		float y = 0;
		float z = (float)(oz);
		float r = Radius;
		angle = u = v = 0.0;		 
		for ( i=0; i<nStep; i++ )
		{			
			r = Radius;			
			a.x = 15;	a.y = 0;	a.z = 0;
			b.x = 0;	b.y = r*sin(angle);	b.z = r*cos(angle);
			c.x = 0;	c.y = r*sin(angle+Step);	c.z = r*cos(angle+Step);
			d.x = 0;	d.y = 0;	d.z = 0;

			a.x += x;	a.y += y;	a.z += z;
			b.x += x;	b.y += y;	b.z += z;
			c.x += x;	c.y += y;	c.z += z;
			d.x += x;	d.y += y;	d.z += z;

			//箭頭
			glColor3f(clrR, clrG, clrB);
			if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_TRIANGLES);	}

			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
			glEnd();

			if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_TRIANGLES);	}

			glVertex3f(d.x, d.y, d.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
			glEnd();

			//軸
			r = Radius2;			
			b.x = 0;	b.y = r*sin(angle);	b.z = r*cos(angle);
			c.x = 0;	c.y = r*sin(angle+Step);	c.z = r*cos(angle+Step);

			glColor3f(clrR2, clrG2, clrB2);
			if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_QUADS);	}			
			glVertex3f(ox, b.y+y, b.z+z);
			glVertex3f(b.x+x, b.y+y, b.z+z);
			glVertex3f(c.x+x, c.y+y, c.z+z);			
			glVertex3f(ox, c.y+y, c.z+z);
			glEnd();
			angle += Step;
		}		
	}

	//Y-Axis
	clrR=0.0f;
	clrG=1.0f;
	clrB=0.0f;
	glColor3f(clrR, clrG, clrB);
	{
		float x = (float)(ox);
		float y = 0;//(float)(m_3DHeight/2);;
		float z = (float)(-oz)+Extend;
		float r = Radius;
		angle = u = v = 0.0;
		for ( i=0; i<nStep; i++ )
		{			
			r = Radius;
			a.x = 0;	a.y = 0;	a.z = 15;
			b.x = r*sin(angle);	b.y = r*cos(angle);	b.z = 0;
			c.x = r*sin(angle+Step);	c.y = r*cos(angle+Step);	c.z = 0;
			d.x = 0;	d.y = 0;	d.z = 0;

			a.x += x;	a.y += y;	a.z += z;
			b.x += x;	b.y += y;	b.z += z;
			c.x += x;	c.y += y;	c.z += z;
			d.x += x;	d.y += y;	d.z += z;

			//箭頭
			glColor3f(clrR, clrG, clrB);
			if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_TRIANGLES);	}

			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
			glEnd();

			if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_TRIANGLES);	}

			glVertex3f(d.x, d.y, d.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
			glEnd();

			//軸
			r = Radius2;
			a.x = 0;	a.y = 0;	a.z = 15;
			b.x = r*sin(angle);	b.y = r*cos(angle);	b.z = 0;
			c.x = r*sin(angle+Step);	c.y = r*cos(angle+Step);	c.z = 0;

			glColor3f(clrR2, clrG2, clrB2);
			if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_QUADS);	}			
			glVertex3f(b.x+x, b.y+y, oz);
			glVertex3f(b.x+x, b.y+y, b.z+z);
			glVertex3f(c.x+x, c.y+y, c.z+z);			
			glVertex3f(c.x+x, c.y+y, oz);
			glEnd();
			angle += Step;
		}		
	}

	//Z-Axis
	clrR=0.0f;
	clrG=0.0f;
	clrB=1.0f;
	glColor3f(clrR, clrG, clrB);
	{
		float x = (float)(ox);
		float y = m_ShowMaxH_GL+Extend;
		float z = (float)(oz);
		float r = Radius;
		angle = u = v = 0.0;
		for ( i=0; i<nStep; i++ )
		{			
			r = Radius;
			a.x = 0;	a.y = 15;	a.z = 0;
			b.x = r*sin(angle);	b.y = 0;	b.z = r*cos(angle);
			c.x = r*sin(angle+Step);	c.y = 0;	c.z = r*cos(angle+Step);
			d.x = 0;	d.y = 0;	d.z = 0;

			a.x += x;	a.y += y;	a.z += z;
			b.x += x;	b.y += y;	b.z += z;
			c.x += x;	c.y += y;	c.z += z;
			d.x += x;	d.y += y;	d.z += z;

			//箭頭
			glColor3f(clrR, clrG, clrB);
			if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_TRIANGLES);	}

			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
			glEnd();

			if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_TRIANGLES);	}

			glVertex3f(d.x, d.y, d.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
			glEnd();

			//軸
			r = Radius2;
			a.x = 0;	a.y = 15;	a.z = 0;
			b.x = r*sin(angle);	b.y = 0;	b.z = r*cos(angle);
			c.x = r*sin(angle+Step);	c.y = 0;	c.z = r*cos(angle+Step);

			glColor3f(clrR2, clrG2, clrB2);
			if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_QUADS);	}			
			glVertex3f(b.x+x, oy, b.z+z);
			glVertex3f(b.x+x, b.y+y, b.z+z);
			glVertex3f(c.x+x, c.y+y, c.z+z);			
			glVertex3f(c.x+x, oy, c.z+z);
			glEnd();
			angle += Step;
		}		
	}

	glDisable(GL_BLEND);
	::glEndList();
	
	GLenum errCode;
	const GLubyte *errString;
	errCode = glGetError();
	if(errCode!=GL_NO_ERROR)
	{
		errString =gluErrorString(errCode);
		this->m_ErrorString=errString;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::RenderScene()
{	
	switch ( m_ShowMode )
	{
	case OPENGL_SHOW_MODEL:
		this->RenderModel();
		break;
	case OPENGL_SHOW_STRING:
		this->RenderString();
		break;
	}	
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::SetViewPortMatrix()
{
	glTranslated(m_xTranslate,m_yTranslate,-5.0);
	glRotated(m_xRotate, 1.0, 0.0, 0.0);
	glRotated(m_yRotate, 0.0, 1.0, 0.0);
	glScalef(m_ScaleX,m_ScaleY,m_ScaleZ);
	return true;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::RenderModel()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DoSelectFont(15, ANSI_CHARSET, "新細明體");

	glPushMatrix();
	int temp = 0;
	double X1 = 0;
	double X2 = 100;
	double Bot = 0;
	double Top = 200;
	CString c1, c2;
	float TextZPos = -3000.0f;//fang 1030124	

	//畫色尺上面的刻度
	m_IsShowColorRuler = true;
	if ( true == m_IsShowColorRuler )
	{
		::glCallList(OPENGL_OBJ_RULER);
		//最大值		
		temp = (int)(m_RulerMaxH);
		c1.Format(_T(" %d"), temp);	
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(m_RulerRT.x, m_RulerRT.y+10, TextZPos);
		DrawChineseString(c1);

		//最小值		
		temp= (int)(m_RulerMinH);
		c2.Format(_T(" %d"), temp);
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(m_RulerRT.x, m_RulerLB.y, TextZPos);
		DrawChineseString(c2);
	}

	float r = 0, g = 0, b = 0;
	if ( true==m_IsShowPadRegion && true==m_IsShowPadHeight )
	{
		c1.Format(_T("%s :%f"), m_strObject, m_PadBox.fHeight);
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(m_Ortho_Left + 20, m_Ortho_Bot + 150, TextZPos); 
		DrawChineseString(c1);
	}

	if(this->m_IsUpperOpen==true)
	{	
		c1.Format(_T("%s :%f"), m_strUpper, m_UpperBoundHeight);
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(m_Ortho_Left + 20, m_Ortho_Bot + 90, TextZPos); 
		DrawChineseString(c1);

		//換算色尺的位置		
		float UpperPoint = 0.0f;

		float RulerLength	 = m_RulerRT.y - m_RulerLB.y;
		float RulerLength_GL = m_RulerMaxH_GL - m_RulerMinH_GL;

		if( m_UpperBoundHeight_GL > m_RulerMaxH_GL)
		{//指在最高處
			UpperPoint = m_RulerRT.y;
		}
		else if(m_UpperBoundHeight_GL < m_RulerMinH_GL)
		{//指在最低處
			UpperPoint = m_RulerLB.y;			
		}
		else
		{
			UpperPoint = ( (m_UpperBoundHeight_GL - m_RulerMinH_GL) / RulerLength_GL * RulerLength ) + m_RulerLB.y;
		}
		c2 = _T("->");
		GetColor(m_UpperBoundHeight_GL, r, g, b);
		glColor3f(r,g,b);	
		glRasterPos3f((m_RulerLB.x - 30 ),UpperPoint, TextZPos);
		DrawChineseString(c2);
	}
	if(this->m_IsLowerOpen==true)
	{		
		c1.Format(_T("%s : %f"), m_strLower, m_LowerBoundHeight);
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(m_Ortho_Left + 20 ,m_Ortho_Bot + 30, TextZPos);
		DrawChineseString(c1);

		//換算色尺的位置		
		float LowerPoint = 0.0f;

		float RulerLength	 = m_RulerRT.y - m_RulerLB.y;
		float RulerLength_GL = m_RulerMaxH_GL - m_RulerMinH_GL;

		if( m_LowerBoundHeight_GL > m_RulerMaxH_GL)
		{//指在最高處
			LowerPoint = m_RulerRT.y;
		}
		else if(m_LowerBoundHeight_GL < m_RulerMinH_GL)
		{//指在最低處
			LowerPoint = m_RulerLB.y;			
		}
		else
		{
			LowerPoint = ( (m_LowerBoundHeight_GL - m_RulerMinH_GL) / RulerLength_GL * RulerLength ) + m_RulerLB.y;
		}
		c2 = _T("->");
		GetColor(m_LowerBoundHeight_GL, r, g, b);
		glColor3f(r,g,b);	
		glRasterPos3f((m_RulerLB.x - 30 ),LowerPoint, TextZPos);
		DrawChineseString(c2);

	}
	
	if ( m_IsShowPoint == true )
	{		
		int nOffsetX=300;		
		//c1.Format(_T("H: %.2f"), m_PointPosZNow/m_Resolution);
		c1.Format(_T("%s: %.2f"), m_strHeight, m_PointPosZNow/m_Resolution);
		glColor3f(10.0f, 10.0f, 10.0f);
		glRasterPos3f(m_Ortho_Right-nOffsetX ,m_Ortho_Bot+130, TextZPos);
		DrawChineseString(c1);		

		c1.Format(_T("%s-A: %.2f"), m_strHeight, m_PointPosZA/m_Resolution);
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(m_Ortho_Right-nOffsetX ,m_Ortho_Bot+80, TextZPos);
		DrawChineseString(c1);		

		c1.Format(_T("%s-B: %.2f"), m_strHeight, m_PointPosZB/m_Resolution);
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(m_Ortho_Right-nOffsetX ,m_Ortho_Bot+30, TextZPos);
		DrawChineseString(c1);				
	}

	if ( m_IsClipOpen == true ) 
	{
		//CString str;
		//str = _T("A");
		//glRasterPos3f(PtRT.x,PtRT.y, PtRT.z);
		//DrawChineseString(str);

	}

	//以下會跟著旋轉
	SetViewPortMatrix();  	

	GLfloat light_position[4]={0,1,0,0};	
	glLightfv( GL_LIGHT1, GL_POSITION, light_position);	

	if( this->m_IsShowPadRegion )//PadRegion
	{	::glCallList(OPENGL_OBJ_BOUND_PAD);	}

	if( this->m_IsShowRoiRegion )//RoiRegion
	{	::glCallList(OPENGL_OBJ_BOUND_ROI);	}	
	
	if ( this->m_IsShowGLBoxList )
	{	RenderGLBoxList();	}

	if ( m_IsShowMainObject )//MainObject
	{	::glCallList(OPENGL_OBJ_MODEL);	}

	if ( m_IsBaseOpen ) //基準面
	{	::glCallList(OPENGL_OBJ_BASE_PLANE); }

	if( m_IsUpperOpen )		//上底
	{	::glCallList(OPENGL_OBJ_UPPER_PLANE);	}

	if( m_IsLowerOpen )	//下底
	{	::glCallList(OPENGL_OBJ_LOWER_PLANE);	}

	if ( m_IsClipOpen )//橫切面
	{	::glCallList(OPENGL_OBJ_CLIP_PLANE); }

	if ( m_IsShowPoint )//點-A
	{	
		::glCallList(OPENGL_OBJ_POINT_A);
		::glCallList(OPENGL_OBJ_POINT_B);
		::glCallList(OPENGL_OBJ_POINT_NOW); 
	}

	if ( m_IsShowAxis )//三軸
	{	::glCallList(OPENGL_OBJ_AXIS); }
	glPopMatrix();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::RenderString()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
	GLfloat DrawX, DrawY;
	DrawX = m_Ortho_Left +30;
	DrawY = 0;
	
	DoSelectFont(48, ANSI_CHARSET, "Comic Sans MS");
	glColor3f(0.8f, 0.0f, 0.8f);
	glRasterPos2f(DrawX,DrawY);
	DrawChineseString(m_ShowString);
	
	glPopMatrix();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SwitchMultiLanguage()
{
	int     i = 0;		
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("ML_OPEN_GL_WND");	
	//---------------------------------------------------------------------------------//		
	WndKey = _T("Upper");
	LabelText = m_strUpper;
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	m_strUpper = NewLabelText;
	//---------------------------------------------------------------------------------//
	WndKey = _T("Lower");
	LabelText = m_strLower;
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	m_strLower = NewLabelText;
	//---------------------------------------------------------------------------------//
	WndKey = _T("Height");
	LabelText = m_strHeight;
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	m_strHeight = NewLabelText;
	//---------------------------------------------------------------------------------//
	WndKey = _T("Object");
	LabelText = m_strObject;
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	m_strObject = NewLabelText;
	//---------------------------------------------------------------------------------//	
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::GetColor(float z,float &r,float &g,float &b)
{
	float RulerMinH_GL = m_RulerMinH_GL;//GL顏色最高最低座標
	float RulerMaxH_GL = m_RulerMaxH_GL;

	float LengthH = RulerMaxH_GL - RulerMinH_GL;
	const float Pitch = LengthH / 3.0f;	//3等分, 切割成 白,紅,綠,藍
	const float Pitch2 = Pitch * 0.25f;	//再細分( 紅到綠 , 綠到藍 ) , 以綠色為起點
	GLfloat BotY = 0.0f, TopY = 0.0f;

	//藍到青
	GLfloat BluetoCyan_Bot = RulerMinH_GL;
	GLfloat BluetoCyan_Top = RulerMinH_GL + Pitch - Pitch2;
	//青到綠
	GLfloat CyantoGreen_Bot = BluetoCyan_Top;
	GLfloat CyantoGreen_Top = RulerMinH_GL + Pitch;
	//綠到黃
	GLfloat GreentoYellow_Bot = CyantoGreen_Top;
	GLfloat GreentoYellow_Top = RulerMinH_GL + Pitch + Pitch2;
	//黃到紅
	GLfloat YellowtoRed_Bot = GreentoYellow_Top;
	GLfloat YellowtoRed_Top = RulerMinH_GL + Pitch * 2;
	//紅到白
	GLfloat RedtoWhite_Bot = YellowtoRed_Top;
	GLfloat RedtoWhite_Top = RulerMinH_GL + Pitch * 3;

	//Blue		(藍色)	(0,0,0.8)
	//Cyan		(青色)	(0,0.8,0.8)
	//Green		(綠色)	(0,0.8,0)
	//Yellow	(黃色)	(0.8,0.8,0)
	//Red		(紅色)	(0.8,0,0)
	//White		(白色)	(1,1,1)

	float BotH = 0.0f, TopH = 0.0f;
	float HeightLength = 0.0f;
	float TempF = 0.0f;
	if( z <= BluetoCyan_Top )//藍到青
	{//(0,0,0.8) -> (0,0.8,0.8)
		BotH = BluetoCyan_Bot;
		TopH = BluetoCyan_Top;
		HeightLength = TopH - BotH;
		TempF = z - BotH;
		r = 0;
		g = TempF / HeightLength * 0.8f;
		b = 0.8f;
	}
	else if( z <= CyantoGreen_Top ) //青到綠
	{//(0,0.8,0.8) -> (0,0.8,0)
		BotH = CyantoGreen_Bot;
		TopH = CyantoGreen_Top;
		HeightLength = TopH - BotH;
		TempF = z - BotH;
		r = 0.0f;
		g = 0.8f;
		b = 0.8f - TempF / HeightLength * 0.8f;
	}
	else if( z <= GreentoYellow_Top ) 	//綠到黃
	{//(0,0.8,0) -> (0.8,0.8,0)
		BotH = GreentoYellow_Bot;
		TopH = GreentoYellow_Top;
		HeightLength = TopH - BotH;
		TempF = z - BotH;
		r = TempF / HeightLength * 0.8f;
		g = 0.8f;
		b = 0.0f;
	}
	else if( z <= YellowtoRed_Top )//黃到紅
	{//(0.8,0.8,0) -> (0.8,0,0)
		BotH = YellowtoRed_Bot;
		TopH = YellowtoRed_Top;
		HeightLength = TopH - BotH;
		TempF = z - BotH;
		r = 0.8f;
		g = 0.8f - TempF / HeightLength * 0.8f;
		b = 0.0f;
	}
	else if( z <= (RedtoWhite_Top+0.5) )//紅到白
	{//(0.8,0,0) -> (1,1,1)
		BotH = RedtoWhite_Bot;
		TopH = RedtoWhite_Top;
		HeightLength = TopH - BotH;
		TempF = z - BotH;
		r = 0.8f;
		g = TempF / HeightLength * 0.8f;
		b = TempF / HeightLength * 0.8f;
	}
	else
	{//(1,1,1)
		z = z;
		r = 1;
		g = 1;
		b = 1;
	}
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::GetVector(const TPOINT3F &PtLB, const TPOINT3F &PtLT, const TPOINT3F &PtRB, TPOINT3F &vector)//取得法向量
{
	TPOINT3F light_vector;
	float nr = 0.0f;

	//第一個三角形
	TPOINT3F dc,ac;
	dc.x = PtRB.x - PtLB.x;
	dc.y = PtRB.y - PtLB.y;
	dc.z = PtRB.z - PtLB.z;

	ac.x = PtLT.x - PtLB.x;
	ac.y = PtLT.y - PtLB.y;
	ac.z = PtLT.z - PtLB.z;

	light_vector.x=(dc.y*ac.z-dc.z*ac.y);
	light_vector.y=(dc.z*ac.x-dc.x*ac.z);
	light_vector.z=-(dc.y*ac.x-dc.x*ac.y);
	nr=(float)sqrt(light_vector.x*light_vector.x+light_vector.y*light_vector.y+light_vector.z*light_vector.z);
	light_vector.x=light_vector.x/nr;
	light_vector.y=-light_vector.y/nr;
	light_vector.z=light_vector.z/nr;
	vector=light_vector;	
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildColorRuler()
{
//紅色 刻度放在 m_PadBox_GL.fHeight
	
	TPOINT3F RulerLB = m_RulerLB;
	TPOINT3F RulerRT = m_RulerRT;

	float LengthY = RulerRT.y - RulerLB.y;
	const double Pitch = LengthY / 3;	//3等分, 切割成 白,紅,綠,藍
	const double Pitch2 = Pitch * 0.25;	//再細分( 紅到綠 , 綠到藍 ) , 以綠色為起點
	GLdouble BotY = 0.0, TopY = 0.0;

	//藍到青
	GLdouble ColorRulerBluetoCyan_Bot = RulerLB.y;
	GLdouble ColorRulerBluetoCyan_Top = RulerLB.y + Pitch - Pitch2;
	//青到綠
	GLdouble ColorRulerCyantoGreen_Bot = ColorRulerBluetoCyan_Top;
	GLdouble ColorRulerCyantoGreen_Top = RulerLB.y + Pitch;
	//綠到黃
	GLdouble ColorRulerGreentoYellow_Bot = ColorRulerCyantoGreen_Top;
	GLdouble ColorRulerGreentoYellow_Top = RulerLB.y + Pitch + Pitch2;
	//黃到紅
	GLdouble ColorRulerYellowtoRed_Bot = ColorRulerGreentoYellow_Top;
	GLdouble ColorRulerYellowtoRed_Top = RulerLB.y + Pitch * 2;
	//紅到白
	GLdouble ColorRulerRedtoWhite_Bot = ColorRulerYellowtoRed_Top;
	GLdouble ColorRulerRedtoWhite_Top = RulerLB.y + Pitch * 3;

	//Blue		(藍色)	(0,0,0.8)
	//Cyan		(青色)	(0,0.8,0.8)
	//Green		(綠色)	(0,0.8,0)
	//Yellow	(黃色)	(0.8,0.8,0)
	//Red		(紅色)	(0.8,0,0)
	//White		(白色)	(1,1,1)
	
	//色條
	//藍到青
	BotY = ColorRulerBluetoCyan_Bot;
	TopY = ColorRulerBluetoCyan_Top;
	::glNewList(OPENGL_OBJ_RULER,GL_COMPILE);
	glBegin(GL_POLYGON);
	glColor3f(0.0f,0.0f,0.8f);		glVertex3d(RulerLB.x, BotY , RulerLB.z);
	glColor3f(0.0f,0.8f,0.8f);		glVertex3d(RulerLB.x, TopY , RulerLB.z);
	glColor3f(0.0f,0.8f,0.8f);		glVertex3d(RulerRT.x, TopY , RulerLB.z);
	glColor3f(0.0f,0.0f,0.8f);		glVertex3d(RulerRT.x, BotY , RulerLB.z);	
	glEnd();

	//青到綠
	BotY = ColorRulerCyantoGreen_Bot;
	TopY = ColorRulerCyantoGreen_Top;
	glBegin(GL_POLYGON);
	glColor3f(0.0f,0.8f,0.8f);		glVertex3d(RulerLB.x, BotY, RulerLB.z);
	glColor3f(0.0f,0.8f,0.0f);		glVertex3d(RulerLB.x, TopY, RulerLB.z);
	glColor3f(0.0f,0.8f,0.0f);		glVertex3d(RulerRT.x, TopY, RulerLB.z);
	glColor3f(0.0f,0.8f,0.8f);		glVertex3d(RulerRT.x, BotY, RulerLB.z);	
	glEnd();

	//綠到黃
	BotY = ColorRulerGreentoYellow_Bot;
	TopY = ColorRulerGreentoYellow_Top;
	glBegin(GL_POLYGON);
	glColor3f(0.0f,0.8f,0.0f);		glVertex3d(RulerLB.x, BotY, RulerLB.z);
	glColor3f(0.8f,0.8f,0.0f);		glVertex3d(RulerLB.x, TopY, RulerLB.z);
	glColor3f(0.8f,0.8f,0.0f);		glVertex3d(RulerRT.x, TopY, RulerLB.z);
	glColor3f(0.0f,0.8f,0.0f);		glVertex3d(RulerRT.x, BotY, RulerLB.z);	
	glEnd();

	//黃到紅
	BotY = ColorRulerYellowtoRed_Bot;
	TopY = ColorRulerYellowtoRed_Top;
	glBegin(GL_POLYGON);
	glColor3f(0.8f,0.8f,0.0f);		glVertex3d(RulerLB.x, BotY, RulerLB.z);
	glColor3f(0.8f,0.0f,0.0f);		glVertex3d(RulerLB.x, TopY, RulerLB.z);
	glColor3f(0.8f,0.0f,0.0f);		glVertex3d(RulerRT.x, TopY, RulerLB.z);
	glColor3f(0.8f,0.8f,0.0f);		glVertex3d(RulerRT.x, BotY, RulerLB.z);	
	glEnd();

	//紅到白
	BotY = ColorRulerRedtoWhite_Bot;
	TopY = ColorRulerRedtoWhite_Top;
	glBegin(GL_POLYGON);
	glColor3f(0.8f,0.0f,0.0f);		glVertex3d(RulerLB.x, BotY, RulerLB.z);
	glColor3f(1.0f,1.0f,1.0f);		glVertex3d(RulerLB.x, TopY, RulerLB.z);
	glColor3f(1.0f,1.0f,1.0f);		glVertex3d(RulerRT.x, TopY, RulerLB.z);
	glColor3f(0.8f,0.0f,0.0f);		glVertex3d(RulerRT.x, BotY, RulerLB.z);	
	glEnd();
	::glEndList();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildMainObject()
{
	switch ( this->m_MainObjectMode )
	{
	case OPENGL_OBJECT_COLOR:
		BuildColorObject();
		break;
	case OPENGL_OBJECT_LINE:
		BuildLineObject();
		break;
	case OPENGL_OBJECT_TEXTURE:
		BuildTextureObject();
		break;
	case OPENGL_OBJECT_COLOR_LINE:
		BuildColorLineObject();
		break;
	}
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildLineObject()
{
	::glNewList(OPENGL_OBJ_MODEL,GL_COMPILE);

	GLfloat no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat mat_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_specular[] =  {0.5f, 0.5f, 0.5f, 0.5f};
    GLfloat no_shininess[] = {0.0f};
    GLfloat low_shininess[] = {5.0f};
    GLfloat hig_shininess[] = {100.0f};
    GLfloat mat_emission[] = {0.3f,0.8f, 0.0f, 1.0f};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);


	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (float)((float)m_3DWidth * 0.5);
	OffsetH = (float)((float)m_3DHeight * 0.5);
	float r = 0, g = 0, b = 0;

	TPOINT3F Pt1, Pt2;
	int index = 0;
	int TempX = 0, TempY = 0;
	unsigned int i = 0, j = 0;
	//左右
	if ( NULL!=m_3DData && m_3DWidth>0 && m_3DHeight>0 )
	{
		for( i = 0; i < m_3DHeight; i++ )
		{
			TempY = i;

			glBegin(GL_LINES);
			glNormal3d(0,1,0);
			for( j = 0 ; j<(m_3DWidth-1); j++  )
			{
				TempX = j;
				index = TempY * m_3DWidth + TempX;
				Pt1.x = TempX - OffsetW;
				Pt1.z = (m_3DHeight - TempY) - OffsetH;
				Pt1.y = m_3DData[index];

				TempX = j+1;
				index = TempY * m_3DWidth + TempX;
				Pt2.x = TempX - OffsetW;
				Pt2.z = (m_3DHeight - TempY) - OffsetH;
				Pt2.y = m_3DData[index];
				GetColor(Pt1.y, r, g, b);	glColor3f(r,g,b);	glVertex3f( Pt1.x, Pt1.y, Pt1.z);
				GetColor(Pt2.y, r, g, b);	glColor3f(r,g,b);	glVertex3f( Pt2.x, Pt2.y, Pt2.z);
			}
			glEnd();
		}
		//上下
		for( j = 0; j < m_3DWidth; j++ )
		{
			TempX = j;
			glBegin(GL_LINES);
			glNormal3d(0,1,0);
			for( i = 0 ; i<(m_3DHeight-1); i++  )
			{
				TempY = i;
				index = TempY * m_3DWidth + TempX;
				Pt1.x = TempX - OffsetW;
				Pt1.z = (m_3DHeight - TempY) - OffsetH;
				Pt1.y = m_3DData[index];

				TempY = i+1;
				index = TempY * m_3DWidth + TempX;
				Pt2.x = TempX - OffsetW;
				Pt2.z = (m_3DHeight - TempY) - OffsetH;
				Pt2.y = m_3DData[index];
				GetColor(Pt1.y, r, g, b);	glColor3f(r,g,b);	glVertex3f( Pt1.x, Pt1.y, Pt1.z);
				GetColor(Pt2.y, r, g, b);	glColor3f(r,g,b);	glVertex3f( Pt2.x, Pt2.y, Pt2.z);
			}
			glEnd();
		}
	}

	::glEndList();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::DrawGLColorQuads(int index)
{
	if( m_3DData == NULL ){ return; }
	
	int i=index/m_3DWidth;
	int j=index%m_3DWidth;
	if ( i>=(m_3DHeight-1) || j>=(m_3DWidth-1) ) { return; }

	float r = 0, g = 0, b = 0;
	TPOINT3F PtVector;
	int TempX = 0, TempY = 0;
	TPOINT3F PtLT, PtLB, PtRT, PtRB;

	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (float)((float)m_3DWidth * 0.5);
	OffsetH = (float)((float)m_3DHeight * 0.5);	

	//左下
	TempX = j;
	TempY = i;
	index = TempY * m_3DWidth + TempX;
	PtLB.x = TempX - OffsetW;
	PtLB.z = (m_3DHeight-TempY) - OffsetH;
	PtLB.y = m_3DData[index];
			
	//左上
	TempX = j;
	TempY = i+1;
	index = TempY * m_3DWidth + TempX;
	PtLT.x = TempX - OffsetW;
	PtLT.z = (m_3DHeight-TempY) - OffsetH;
	PtLT.y = m_3DData[index];

	//右下
	TempX = j+1;
	TempY = i;
	index = TempY * m_3DWidth + TempX;
	PtRB.x = TempX - OffsetW;
	PtRB.z =  (m_3DHeight-TempY) - OffsetH;
	PtRB.y = m_3DData[index];

	//右上
	TempX = j+1;
	TempY = i+1;
	index = TempY * m_3DWidth + TempX;
	PtRT.x = TempX - OffsetW;
	PtRT.z =  (m_3DHeight-TempY) - OffsetH;
	PtRT.y = m_3DData[index];

	glBegin(GL_QUADS);
	GetVector(PtLB, PtLT, PtRB, PtVector);
	glNormal3f(PtVector.x,PtVector.y,PtVector.z);
	GetColor(PtLB.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtLB.x, PtLB.y, PtLB.z);
	GetColor(PtRB.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtRB.x, PtRB.y, PtRB.z);
	GetColor(PtRT.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtRT.x, PtRT.y, PtRT.z);
	GetColor(PtLT.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtLT.x, PtLT.y, PtLT.z);
	glEnd();
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildColorObject()
{	
	if( m_3DData == NULL ){ return; }

	::glNewList(OPENGL_OBJ_MODEL,GL_COMPILE);
	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (float)((float)m_3DWidth * 0.5);
	OffsetH = (float)((float)m_3DHeight * 0.5);	

	int index = 0;
	unsigned int i = 0, j = 0;
	float r = 0, g = 0, b = 0;
	int TempX = 0, TempY = 0;
	TPOINT3F PtVector;
	TPOINT3F PtLT, PtLB, PtRT, PtRB;

	GLfloat no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat mat_ambient[] = {1.0f, 1.f, 1.0f, 1.0f};
    GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat mat_specular[] =  {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat no_shininess[] = {0.0f};
    GLfloat low_shininess[] = {70.0f};
    GLfloat hig_shininess[] = {100.0f};
    GLfloat mat_emission[] = {0.3f,0.8f, 0.0f, 1.0f};

	//鏡面反射小塊
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, no_mat);    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, hig_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);		
	
	if ( NULL!=m_3DData && m_3DWidth>0 && m_3DHeight>0 )
	{
		for( i = 0 ; i < (m_3DHeight-1) ; i++ )
		{
			for( j = 0 ; j < (m_3DWidth-1) ; j++ )
			{
				//左下
				TempX = j;
				TempY =i;
				index = TempY * m_3DWidth + TempX;
				PtLB.x = TempX - OffsetW;
				PtLB.z = (m_3DHeight-TempY) - OffsetH;
				PtLB.y = m_3DData[index];
			
				//左上
				TempX = j;
				TempY = i+1;
				index = TempY * m_3DWidth + TempX;
				PtLT.x = TempX - OffsetW;
				PtLT.z = (m_3DHeight-TempY) - OffsetH;
				PtLT.y = m_3DData[index];

				//右下
				TempX = j+1;
				TempY = i;
				index = TempY * m_3DWidth + TempX;
				PtRB.x = TempX - OffsetW;
				PtRB.z =  (m_3DHeight-TempY) - OffsetH;
				PtRB.y = m_3DData[index];

				//右上
				TempX = j+1;
				TempY = i+1;
				index = TempY * m_3DWidth + TempX;
				PtRT.x = TempX - OffsetW;
				PtRT.z =  (m_3DHeight-TempY) - OffsetH;
				PtRT.y = m_3DData[index];

				glBegin(GL_QUADS);
				GetVector(PtLB, PtLT, PtRB, PtVector);
				glNormal3f(PtVector.x,PtVector.y,PtVector.z);
				GetColor(PtLB.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtLB.x, PtLB.y, PtLB.z);
				GetColor(PtRB.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtRB.x, PtRB.y, PtRB.z);
				GetColor(PtRT.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtRT.x, PtRT.y, PtRT.z);
				GetColor(PtLT.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtLT.x, PtLT.y, PtLT.z);
				glEnd();

			}
		}
	}
	::glEndList();

	GLenum errCode;
	const GLubyte *errString;
	errCode = glGetError();
	if(errCode!=GL_NO_ERROR)
	{
		errString =gluErrorString(errCode);
		this->m_ErrorString.Format(_T("%s  %s"), errString,"--(OpenGL)");
	}
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildColorLineObject()
{
	int i = 0, j = 0;
	if( m_3DData == NULL ){ return; }

	::glNewList(OPENGL_OBJ_MODEL,GL_COMPILE);

	GLfloat no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat mat_ambient[] = {1.0f, 1.f, 1.0f, 1.0f};
    GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat mat_specular[] =  {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat no_shininess[] = {0.0f};
    GLfloat low_shininess[] = {70.0f};
    GLfloat hig_shininess[] = {100.0f};
    GLfloat mat_emission[] = {0.3f,0.8f, 0.0f, 1.0f};

	//鏡面反射小塊
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, no_mat);    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, hig_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);


	TPOINT3F PtLT, PtLB, PtRT, PtRB;
	int index = 0;
	int TempX = 0, TempY = 0;

	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (float)((float)m_3DWidth * 0.5);
	OffsetH = (float)((float)m_3DHeight * 0.5);
	float r = 0, g = 0, b = 0;
	TPOINT3F PtVector;
	if ( NULL!=m_3DData && m_3DWidth>0 && m_3DHeight>0 )
	{
		for( i = 0 ; i < (m_3DHeight-1) ; i++ )
		{
			for( j = 0 ; j < (m_3DWidth-1) ; j++ )
			{
				//左下
				TempX = j;
				TempY =i;
				index = TempY * m_3DWidth + TempX;
				PtLB.x = TempX - OffsetW;
				PtLB.z = (m_3DHeight-TempY) - OffsetH;
				PtLB.y = m_3DData[index];
			
				//左上
				TempX = j;
				TempY = i+1;
				index = TempY * m_3DWidth + TempX;
				PtLT.x = TempX - OffsetW;
				PtLT.z = (m_3DHeight-TempY) - OffsetH;
				PtLT.y = m_3DData[index];

				//右下
				TempX = j+1;
				TempY = i;
				index = TempY * m_3DWidth + TempX;
				PtRB.x = TempX - OffsetW;
				PtRB.z =  (m_3DHeight-TempY) - OffsetH;
				PtRB.y = m_3DData[index];

				//右上
				TempX = j+1;
				TempY = i+1;
				index = TempY * m_3DWidth + TempX;
				PtRT.x = TempX - OffsetW;
				PtRT.z =  (m_3DHeight-TempY) - OffsetH;
				PtRT.y = m_3DData[index];

				glBegin(GL_QUADS);
				GetVector(PtLB, PtLT, PtRB, PtVector);
				glNormal3f(PtVector.x,PtVector.y,PtVector.z);
				GetColor(PtLB.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtLB.x, PtLB.y, PtLB.z);
				GetColor(PtRB.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtRB.x, PtRB.y, PtRB.z);
				GetColor(PtRT.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtRT.x, PtRT.y, PtRT.z);
				GetColor(PtLT.y, r, g, b);	glColor3f(r,g,b);	glVertex3f(PtLT.x, PtLT.y, PtLT.z);
				glEnd();

			}
		}	

	//fang 1021226
		TPOINT3F Pt1, Pt2;
		glColor3f(0,0,0);
		for( i = 0; i < m_3DHeight; i++ )
		{
			TempY = i;

			glBegin(GL_LINES);
			glNormal3d(0,1,0);
			for( j = 0 ; j<(m_3DWidth-1); j++  )
			{
				TempX = j;
				index = TempY * m_3DWidth + TempX;
				Pt1.x = TempX - OffsetW;
				Pt1.z = (m_3DHeight - TempY) - OffsetH;
				Pt1.y = m_3DData[index];

				TempX = j+1;
				index = TempY * m_3DWidth + TempX;
				Pt2.x = TempX - OffsetW;
				Pt2.z = (m_3DHeight - TempY) - OffsetH;
				Pt2.y = m_3DData[index];
	//			GetColor(Pt1.y, r, g, b);	glColor3f(r,g,b);
				glVertex3f( Pt1.x, Pt1.y, Pt1.z);
	//			GetColor(Pt2.y, r, g, b);	glColor3f(r,g,b);	
				glVertex3f( Pt2.x, Pt2.y, Pt2.z);
			}
			glEnd();
		}
		//上下
		for( j = 0; j < m_3DWidth; j++ )
		{
			TempX = j;
			glBegin(GL_LINES);
			glNormal3d(0,1,0);
			for( i = 0 ; i<(m_3DHeight-1); i++  )
			{
				TempY = i;
				index = TempY * m_3DWidth + TempX;
				Pt1.x = TempX - OffsetW;
				Pt1.z = (m_3DHeight - TempY) - OffsetH;
				Pt1.y = m_3DData[index];

				TempY = i+1;
				index = TempY * m_3DWidth + TempX;
				Pt2.x = TempX - OffsetW;
				Pt2.z = (m_3DHeight - TempY) - OffsetH;
				Pt2.y = m_3DData[index];
	//			GetColor(Pt1.y, r, g, b);	glColor3f(r,g,b);
				glVertex3f( Pt1.x, Pt1.y, Pt1.z);
	//			GetColor(Pt2.y, r, g, b);	glColor3f(r,g,b);
				glVertex3f( Pt2.x, Pt2.y, Pt2.z);
			}
			glEnd();
		}
	}
//
	::glEndList();

	GLenum errCode;
	const GLubyte *errString;
	errCode = glGetError();
	if(errCode!=GL_NO_ERROR)
	{
		errString =gluErrorString(errCode);
		this->m_ErrorString.Format(_T("%s  %s"), errString,"--(OpenGL)");
	}
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildTextureObject()
{	
	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (float)((float)m_3DWidth * 0.5);
	OffsetH = (float)((float)m_3DHeight * 0.5);
	TPOINT3F PtVector;

	TPOINT3F PtLT, PtLB, PtRT, PtRB;
	int index = 0;
	int TempX = 0, TempY = 0;

	unsigned int i = 0,j = 0;

	float tx=0,ty=0;
	float dtx=1/(float)(m_3DWidth), dty=1/(float)(m_3DHeight);

	::glNewList(OPENGL_OBJ_MODEL,GL_COMPILE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,1); 
	if ( NULL!=m_3DData && m_3DWidth>0 && m_3DHeight>0 )
	{
		for( i = 0 ; i < (m_3DHeight-1) ; i++ )
		{
			tx=0;
			for( j = 0 ; j < (m_3DWidth-1) ; j++ )
			{
				//左下
				TempX = j;
				TempY = i;
				index = TempY * m_3DWidth + TempX;
				PtLB.x = TempX - OffsetW;
				PtLB.z = (m_3DHeight - TempY) - OffsetH;
				PtLB.y = m_3DData[index];
			
				//左上
				TempX = j;
				TempY = i+1;
				index = TempY * m_3DWidth + TempX;
				PtLT.x = TempX - OffsetW;
				PtLT.z = (m_3DHeight - TempY) - OffsetH;
				PtLT.y = m_3DData[index];

				//右下
				TempX = j+1;
				TempY = i;
				index = TempY * m_3DWidth + TempX;
				PtRB.x = TempX - OffsetW;
				PtRB.z = (m_3DHeight - TempY) - OffsetH;
				PtRB.y = m_3DData[index];

				//右上
				TempX = j+1;
				TempY = i+1;
				index = TempY * m_3DWidth + TempX;
				PtRT.x = TempX - OffsetW;
				PtRT.z = (m_3DHeight - TempY) - OffsetH;
				PtRT.y = m_3DData[index];

				glBegin(GL_QUADS);
				glColor3f(1.0f, 1.0f, 1.0f);
				GetVector(PtLB, PtLT, PtRB, PtVector);
				glNormal3f(PtVector.x,PtVector.y,PtVector.z);
				glTexCoord2f(tx,ty);			glVertex3f(PtLB.x, PtLB.y, PtLB.z);
				glTexCoord2f(tx+dtx,ty);		glVertex3f(PtRB.x, PtRB.y, PtRB.z);
				glTexCoord2f(tx+dtx,ty+dty);	glVertex3f(PtRT.x, PtRT.y, PtRT.z);
				glTexCoord2f(tx,ty+dty);		glVertex3f(PtLT.x, PtLT.y, PtLT.z);
				glEnd();
			
				tx=tx+dtx;
			}
			ty=ty+dty;
		}
	}
	glDisable(GL_TEXTURE_2D); 
	::glEndList();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildPadRegion()
{	
	float PadSpecHeight = m_PadBox_GL.fHeight;

	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (m_3DWidth*0.5f);
	OffsetH = (m_3DHeight*0.5f);

	TPOINT3F PtLB, PtLT, PtRB, PtRT;
	float LowerHeight = 0.0f, UpperHeight = PadSpecHeight;

	::glNewList(OPENGL_OBJ_BOUND_PAD,GL_COMPILE);
		
	PtLB.x = m_PadBox_GL.fPtX_1 - OffsetW;			
	PtRB.x = m_PadBox_GL.fPtX_2 - OffsetW;
	PtRT.x = m_PadBox_GL.fPtX_3 - OffsetW;
	PtLT.x = m_PadBox_GL.fPtX_4 - OffsetW;
	PtLB.z = (m_PadBox_GL.fPtY_1) - OffsetH;			
	PtRB.z = (m_PadBox_GL.fPtY_2) - OffsetH;
	PtRT.z = (m_PadBox_GL.fPtY_3) - OffsetH;
	PtLT.z = (m_PadBox_GL.fPtY_4) - OffsetH;	

	glBegin(GL_LINES);
	glColor3ub(255,0,0);
	glVertex3f(PtLB.x, LowerHeight, PtLB.z);	glVertex3f(PtRB.x, LowerHeight, PtRB.z);
	glVertex3f(PtRB.x, LowerHeight, PtRB.z);	glVertex3f(PtRT.x, LowerHeight, PtRT.z);
	glVertex3f(PtRT.x, LowerHeight, PtRT.z);	glVertex3f(PtLT.x, LowerHeight, PtLT.z);
	glVertex3f(PtLT.x, LowerHeight, PtLT.z);	glVertex3f(PtLB.x, LowerHeight, PtLB.z);

	glVertex3f(PtLB.x, UpperHeight, PtLB.z);	glVertex3f(PtRB.x, UpperHeight, PtRB.z);
	glVertex3f(PtRB.x, UpperHeight, PtRB.z);	glVertex3f(PtRT.x, UpperHeight, PtRT.z);
	glVertex3f(PtRT.x, UpperHeight, PtRT.z);	glVertex3f(PtLT.x, UpperHeight, PtLT.z);
	glVertex3f(PtLT.x, UpperHeight, PtLT.z);	glVertex3f(PtLB.x, UpperHeight, PtLB.z);

	glVertex3f(PtLB.x, LowerHeight, PtLB.z);	glVertex3f(PtLB.x, UpperHeight, PtLB.z);
	glVertex3f(PtRB.x, LowerHeight, PtRB.z);	glVertex3f(PtRB.x, UpperHeight, PtRB.z);
	glVertex3f(PtRT.x, LowerHeight, PtRT.z);	glVertex3f(PtRT.x, UpperHeight, PtRT.z);
	glVertex3f(PtLT.x, LowerHeight, PtLT.z);	glVertex3f(PtLT.x, UpperHeight, PtLT.z);
	glEnd();

	::glEndList();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildRoiRegion()
{	
	float PadSpecHeight = m_RoiBox_GL.fHeight;

	float OffsetW = 0.0f;
	float OffsetH = 0.0f;
	OffsetW = (m_3DWidth*0.5f);
	OffsetH = (m_3DHeight*0.5f);

	TPOINT3F PtLB, PtLT, PtRB, PtRT;
	float LowerHeight = 0.0f, UpperHeight = PadSpecHeight;

	::glNewList(OPENGL_OBJ_BOUND_ROI,GL_COMPILE);	

	//PadROIRect
	//PtLB.x = PadROIRect.left - OffsetW;
	//PtLT.x = PadROIRect.left - OffsetW;
	//PtRB.x = PadROIRect.right - OffsetW;
	//PtRT.x = PadROIRect.right - OffsetW;
	//PtLB.z = (m_3DHeight - PadROIRect.bottom) - OffsetH;
	//PtRB.z = (m_3DHeight - PadROIRect.bottom) - OffsetH;
	//PtLT.z = (m_3DHeight - PadROIRect.top) - OffsetH;
	//PtRT.z = (m_3DHeight - PadROIRect.top) - OffsetH;

	PtLB.x = m_RoiBox_GL.fPtX_1 - OffsetW;			
	PtRB.x = m_RoiBox_GL.fPtX_2 - OffsetW;
	PtRT.x = m_RoiBox_GL.fPtX_3 - OffsetW;
	PtLT.x = m_RoiBox_GL.fPtX_4 - OffsetW;
	PtLB.z = (m_RoiBox_GL.fPtY_1) - OffsetH;			
	PtRB.z = (m_RoiBox_GL.fPtY_2) - OffsetH;
	PtRT.z = (m_RoiBox_GL.fPtY_3) - OffsetH;
	PtLT.z = (m_RoiBox_GL.fPtY_4) - OffsetH;	

	glBegin(GL_LINES);
	glColor3ub(155,255,255);
	glVertex3f(PtLB.x, LowerHeight, PtLB.z);	glVertex3f(PtRB.x, LowerHeight, PtRB.z);
	glVertex3f(PtRB.x, LowerHeight, PtRB.z);	glVertex3f(PtRT.x, LowerHeight, PtRT.z);
	glVertex3f(PtRT.x, LowerHeight, PtRT.z);	glVertex3f(PtLT.x, LowerHeight, PtLT.z);
	glVertex3f(PtLT.x, LowerHeight, PtLT.z);	glVertex3f(PtLB.x, LowerHeight, PtLB.z);

	glVertex3f(PtLB.x, UpperHeight, PtLB.z);	glVertex3f(PtRB.x, UpperHeight, PtRB.z);
	glVertex3f(PtRB.x, UpperHeight, PtRB.z);	glVertex3f(PtRT.x, UpperHeight, PtRT.z);
	glVertex3f(PtRT.x, UpperHeight, PtRT.z);	glVertex3f(PtLT.x, UpperHeight, PtLT.z);
	glVertex3f(PtLT.x, UpperHeight, PtLT.z);	glVertex3f(PtLB.x, UpperHeight, PtLB.z);

	glVertex3f(PtLB.x, LowerHeight, PtLB.z);	glVertex3f(PtLB.x, UpperHeight, PtLB.z);
	glVertex3f(PtRB.x, LowerHeight, PtRB.z);	glVertex3f(PtRB.x, UpperHeight, PtRB.z);
	glVertex3f(PtRT.x, LowerHeight, PtRT.z);	glVertex3f(PtRT.x, UpperHeight, PtRT.z);
	glVertex3f(PtLT.x, LowerHeight, PtLT.z);	glVertex3f(PtLT.x, UpperHeight, PtLT.z);
	glEnd();

	::glEndList();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::BuildGLBoxList()
{	
	float ImageW = (float)(m_3DWidth);
	float ImageH = (float)(m_3DHeight);
	float OffsetW = 0.0f;
	float OffsetH = 0.0f;	
	OffsetW = (ImageW*0.5f);
	OffsetH = (ImageH*0.5f);

	TOpenGLBox   glBox;
	unsigned int i=0, ObjID = 0;;
	TPOINT3F PtLB, PtLT, PtRB, PtRT;
	float LowerHeight = 0.0f, UpperHeight = 0.0f;
	const unsigned int GLBoxCount = (unsigned int)(m_GLBoxList.size());
	
	for ( i=0; i<GLBoxCount; i++ )
	{
		glBox = m_GLBoxList[i];
		ObjID = OPENGL_OBJ_BOUND_BOX+i;				

		//PadROIRect
		LowerHeight = 0.0f;
		UpperHeight = glBox.fHeight*m_Resolution;
		PtLB.x = glBox.fPtX_1 - OffsetW;			
		PtRB.x = glBox.fPtX_2 - OffsetW;
		PtRT.x = glBox.fPtX_3 - OffsetW;
		PtLT.x = glBox.fPtX_4 - OffsetW;
		PtLB.z = (glBox.fPtY_1) - OffsetH;			
		PtRB.z = (glBox.fPtY_2) - OffsetH;
		PtRT.z = (glBox.fPtY_3) - OffsetH;
		PtLT.z = (glBox.fPtY_4) - OffsetH;		
		::glNewList(ObjID,GL_COMPILE);	

		glBegin(GL_LINES);
		//glColor3ub(155,255,255);
		glColor3ub(200,200,64);
		glVertex3f(PtLB.x, LowerHeight, PtLB.z);	glVertex3f(PtRB.x, LowerHeight, PtRB.z);
		glVertex3f(PtRB.x, LowerHeight, PtRB.z);	glVertex3f(PtRT.x, LowerHeight, PtRT.z);
		glVertex3f(PtRT.x, LowerHeight, PtRT.z);	glVertex3f(PtLT.x, LowerHeight, PtLT.z);
		glVertex3f(PtLT.x, LowerHeight, PtLT.z);	glVertex3f(PtLB.x, LowerHeight, PtLB.z);

		glVertex3f(PtLB.x, UpperHeight, PtLB.z);	glVertex3f(PtRB.x, UpperHeight, PtRB.z);
		glVertex3f(PtRB.x, UpperHeight, PtRB.z);	glVertex3f(PtRT.x, UpperHeight, PtRT.z);
		glVertex3f(PtRT.x, UpperHeight, PtRT.z);	glVertex3f(PtLT.x, UpperHeight, PtLT.z);
		glVertex3f(PtLT.x, UpperHeight, PtLT.z);	glVertex3f(PtLB.x, UpperHeight, PtLB.z);

		glVertex3f(PtLB.x, LowerHeight, PtLB.z);	glVertex3f(PtLB.x, UpperHeight, PtLB.z);
		glVertex3f(PtRB.x, LowerHeight, PtRB.z);	glVertex3f(PtRB.x, UpperHeight, PtRB.z);
		glVertex3f(PtRT.x, LowerHeight, PtRT.z);	glVertex3f(PtRT.x, UpperHeight, PtRT.z);
		glVertex3f(PtLT.x, LowerHeight, PtLT.z);	glVertex3f(PtLT.x, UpperHeight, PtLT.z);
		glEnd();

		::glEndList();		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::RenderGLBoxList()
{
	TOpenGLBox   glBox;
	unsigned int i=0, ObjID = 0;
	const unsigned int GLBoxCount = (unsigned int)(m_GLBoxList.size());
	for ( i=0; i<GLBoxCount; i++ )
	{
		glBox = m_GLBoxList[i];
		if ( false == glBox.bShow ) { continue; }				
		::glCallList(OPENGL_OBJ_BOUND_BOX+i);

		if ( glBox.strText.GetLength() > 0 )
		{	DrawChineseString(glBox.strText); }
	}
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::DoSelectFont(int size, int charset, const char* face)
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::DrawChineseString(LPCTSTR str) 
{
	int i=0;	
	const int len = (int)(::_tcslen(str));	
	HDC hDC = wglGetCurrentDC(); 
	if ( NULL == hDC ) { return; } 
 // GLuint list = glGenLists(1);    
    for(i=0; i<len; ++i)
    {
	#ifdef _UNICODE		
		wglUseFontBitmapsW(hDC, str[i], 1, OPENGL_OBJ_STRING);
	#else	
		wglUseFontBitmapsA(hDC, str[i], 1, OPENGL_OBJ_STRING);
	#endif
		glCallList(OPENGL_OBJ_STRING);
    }
//  glDeleteLists(list, 1);
}
//-------------------------------------------------------------------------------------//
unsigned int COpenGLWnd::GetNBytesPerLine(const unsigned int ImageW)
{
	unsigned int bytes_per_line = ( ImageW * 8 + 7)/8;
    bytes_per_line = ( bytes_per_line + 3 ) / 4;
    bytes_per_line = bytes_per_line*4;
	return bytes_per_line;
}
//-------------------------------------------------------------------------------------//
BOOL COpenGLWnd::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | 
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_STEREO_DONTCARE;
	
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	
	m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if ( m_GLPixelIndex==0 ) // Choose default
	{	m_GLPixelIndex = 1;	}

	if ( DescribePixelFormat(hDC,m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
	{	return FALSE;	}

	if ( SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc)==FALSE)
	{	return FALSE;	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL COpenGLWnd::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	
	if(m_hGLContext==NULL)
	{ return FALSE; }
	
	if(wglMakeCurrent(hDC,m_hGLContext)==FALSE)
	{ return FALSE; }
	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::ShowMessage(LPCTSTR str)
{
	m_ShowMode = OPENGL_SHOW_STRING;
	m_ShowString = str;
	this->OnPaint();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetUserResolution(float UserResolution)
{
	m_UserResolution = UserResolution; 
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetBountPitch(float Pitch)
{
	m_BoundPitch = Pitch;
}
//-------------------------------------------------------------------------------------//
float COpenGLWnd::GetBoundPitch() const
{
	return m_BoundPitch;
}
//-------------------------------------------------------------------------------------//
float COpenGLWnd::GetBoundHeight() const
{
	double Pos = 0;
	const bool ShowUpper = GetUpperPlaneOpen();
	const bool ShowLower = GetLowerPlaneOpen();
	if ( true==ShowUpper )
	{	Pos = GetUpperBoundHeight();	}
	else
	{
		if ( true == ShowLower )
		{	Pos = GetLowerBoundHeight(); }
		else
		{	Pos = GetUpperBoundHeight();	}
	}
	return Pos;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetBountHeight(float Pos)
{
	const bool ShowUpper = GetUpperPlaneOpen();
	const bool ShowLower = GetLowerPlaneOpen();
	if ( true==ShowUpper )
	{	SetUpperBoundHeight(Pos);	}
	else
	{
		if ( true == ShowLower )
		{	SetLowerBoundHeight(Pos); }
		else
		{	SetUpperBoundHeight(Pos);	}
	}
	return;
}
//-------------------------------------------------------------------------------------//
float COpenGLWnd::GetUpperBoundHeight() const
{	
	return m_UpperBoundHeight;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetUpperBoundHeight(float Pos)
{	
	m_UpperBoundHeight = Pos;
	m_UpperBoundHeight_GL = Pos*m_Resolution;
	BuildUpperBound();
}
//-------------------------------------------------------------------------------------//
float COpenGLWnd::GetLowerBoundHeight() const
{	
	return m_LowerBoundHeight;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetLowerBoundHeight(float Pos)
{	
	m_LowerBoundHeight = Pos;
	m_LowerBoundHeight_GL = Pos*m_Resolution;
	BuildUpperBound();
}
//-------------------------------------------------------------------------------------//
float COpenGLWnd::GetShowMinH() const
{
	return m_ShowMinH;
}
//-------------------------------------------------------------------------------------//
float COpenGLWnd::GetShowMaxH() const
{
	return m_ShowMaxH;
}
//-------------------------------------------------------------------------------------//
float COpenGLWnd::Get3DScale() const
{
	return m_3DScale;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetGLBoxListOpen(bool IsOpen)
{
	m_IsShowGLBoxList = IsOpen;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//	
size_t COpenGLWnd::GetGLBoxListSize() const
{
	return m_GLBoxList.size();
}
//-------------------------------------------------------------------------------------//
TOpenGLBox* COpenGLWnd::GetGLBoxPtr(size_t index, bool bCheck)
{
	if ( true == bCheck )
	{
		const size_t Count = m_GLBoxList.size();
		if ( index >= Count ) 
		{ return NULL; }
	}
	return &(m_GLBoxList[index]);
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::AddGLBoxObj(const TOpenGLBox &Box)
{
	const size_t Count = m_GLBoxList.size();
	//if ( Count >= 8 ) { return false; }
	TOpenGLBox glBox = Box;
	glBox.fPtX_1 = glBox.fPtX_1/m_ScaleW;
	glBox.fPtY_1 = glBox.fPtY_1/m_ScaleH;
	glBox.fPtX_2 = glBox.fPtX_2/m_ScaleW;
	glBox.fPtY_2 = glBox.fPtY_2/m_ScaleH;
	glBox.fPtX_3 = glBox.fPtX_3/m_ScaleW;
	glBox.fPtY_3 = glBox.fPtY_3/m_ScaleH;
	glBox.fPtX_4 = glBox.fPtX_4/m_ScaleW;
	glBox.fPtY_4 = glBox.fPtY_4/m_ScaleH;

	m_GLBoxList.push_back(glBox);
	return true;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::ClearGLBoxList()
{
	m_GLBoxList.clear();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::UpdateGLBoxList()
{
	BuildGLBoxList();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetAnimationPlay(bool val)
{
	if ( CWnd::GetSafeHwnd() == NULL )
	{	return; }
	
	m_AnimationPlay = val;
	if ( true == m_AnimationPlay )
	{	CWnd::SetTimer(TIMER_ID_ANIMATION, 100, NULL); }
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::GetAnimationPlay() const
{
	return m_AnimationPlay;
}
//-------------------------------------------------------------------------------------//
double COpenGLWnd::GetRotateAngleX() const
{
	return m_xRotate;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetRotateAngleX(double val)
{
	m_xRotate = val;
}
//-------------------------------------------------------------------------------------//
double COpenGLWnd::GetRotateAngleY() const
{
	return m_yRotate;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetRotateAngleY(double val)
{
	m_yRotate = val;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::RotateToPadAngle()//轉到Pad視角
{	
	TOpenGLBox PadBox = m_PadBox_GL;
	const double CPX = m_3DWidth*0.5;
	const double CPY = m_3DHeight*0.5;	
	const double PadCpX = (PadBox.fPtX_1+PadBox.fPtX_2+PadBox.fPtX_3+PadBox.fPtX_4)/4;
	const double PadCpY = (PadBox.fPtY_1+PadBox.fPtY_2+PadBox.fPtY_3+PadBox.fPtY_4)/4;
	const double OffsetX = PadCpX-CPX;
	const double OffsetY = PadCpY-CPY;
	const double AngleR = ::atan2(OffsetY, OffsetX);
	const double AngleD = AngleR*RAD_TO_DEG_DBL;
	const double AngleD2 = 360.0-AngleD;
	SetRotateAngleY(AngleD+90);

	/*
	SetRotateAngleX(-45);
	m_xTranslate = 0;
	m_yTranslate = PadCpY;
	//m_ScaleX = 1.0f;
	//m_ScaleY = 1.0f;
	//m_ScaleZ = 1.0f;
	*/
	Invalidate();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::RotateToPadAngleByAroundHeight()//轉到Pad視角-使用四周圍比較
{
	float *Ptr3D = m_3DData;
	if ( NULL == Ptr3D ) { return; }		
	RECT      PadRect={0,0,0,0};
	int       i=0, j=0, k=0;	
	int       CntL=0, CntT=0, CntR=0, CntB=0;
	float     SumL=0.0f, SumT=0.0f, SumR=0.0f, SumB=0.0f;
	float     AveL=0.0f, AveT=0.0f, AveR=0.0f, AveB=0.0f;
	const int Width = m_3DWidth;
	const int Height = m_3DHeight;
	TOpenGLBox PadBox = m_PadBox_GL;
	
	ConvertGLBoxToRect(PadBox, PadRect);
	for ( i=PadRect.top; i<PadRect.bottom; i++ )
	{
		//About Left;
		for ( j=0; j<PadRect.left; j++ )
		{
			k = (i*Width)+j;
			CntL ++;
			SumL += Ptr3D[k];
		}

		//About Right;
		for ( j=PadRect.right; j<Width; j++ )
		{
			k = (i*Width)+j;
			CntR ++;
			SumR += Ptr3D[k];
		}

		if ( CntL > 0 ) 
		{	AveL = SumL/CntL; }
		if ( CntR > 0 ) 
		{	AveR = SumR/CntR; }
	}

	for ( i=PadRect.left; i<PadRect.right; i++ )
	{
		//About Top;
		for ( j=0; j<PadRect.top; j++ )
		{
			k = i+(Width*j);
			CntT ++;
			SumT += Ptr3D[k];
		}

		//About Bottom;
		for ( j=PadRect.bottom; j<Height; j++ )
		{
			k = i+(Width*j);
			CntB ++;
			SumB += Ptr3D[k];
		}

		if ( CntT > 0 ) 
		{	AveT = SumT/CntT; }
		if ( CntB > 0 ) 
		{	AveB = SumB/CntB; }
	}

	double AngleD=0.0;
	int   nVal[4]={0,0,0,0};
	float fVal[4]={0,0,0,0};
	
	GetMaxValueIndex(AveL, AveT, AveR, AveB, nVal, fVal);	
	switch ( nVal[0] )
	{
	case MAX_HEIGHT_AT_LEFT:		
		if ( MAX_HEIGHT_AT_TOP == nVal[1] )
		{	AngleD = 45;	}
		else
		{	
			if ( (fVal[0]*0.25) > fVal[1] )
			{	AngleD = 45;  }
			else
			{	AngleD = 135;	}
		}
		break;
	case MAX_HEIGHT_AT_TOP:		
		if ( MAX_HEIGHT_AT_LEFT == nVal[1] )
		{	AngleD =  45; }
		else
		{
			if ( (fVal[0]*0.25) > fVal[1] )
			{	AngleD =  45; }
			else
			{	AngleD = -45;  }
		}		
		break;
	case MAX_HEIGHT_AT_RIGHT:
		if ( MAX_HEIGHT_AT_TOP == nVal[1] )
		{	AngleD = -45; }
		else
		{
			if ( (fVal[0]*0.25) > fVal[1] )
			{	AngleD = -45;	}
			else
			{	AngleD = 225;  }
		}			
		break;
	case MAX_HEIGHT_AT_BOTTOM:
		if ( MAX_HEIGHT_AT_LEFT == nVal[1] )
		{	AngleD = 135; }
		else
		{
			if ( (fVal[0]*0.25) > fVal[1] )
			{	AngleD = 135; }
			else
			{	AngleD = 225;  }
		}			
		break;
	}
	SetRotateAngleY(AngleD);	
	Invalidate();
	return;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::CreateMaxDataBuffer()//建立最大記憶體緩衝
{	
#ifdef _X64
	//Clear
	ReleaseAllBuffer();	

//3D
	size_t TempSize3D = m_Max3DDisplaySize;
	m_3DData = new float[TempSize3D];
	if ( NULL == m_3DData ) 
	{
		ReleaseAllBuffer();	
		return false; 
	}		
	m_Max3DBufferSize = TempSize3D;
	::memset(m_3DData, 0x00, sizeof(float)*TempSize3D);

//2D
	//2D
	//材質縮圖		
	const int Max2DW = 256;
	const int Max2DH = 256;
	const int Max2DStep = 256*3;
	size_t TempSize2D = Max2DStep*Max2DH;
	m_2DData = new unsigned char[TempSize2D];
	if ( NULL == m_2DData )
	{
		ReleaseAllBuffer();	
		return false; 
	}
	m_Max2DBufferSize = TempSize2D;
	::memset(m_2DData, 0x00, sizeof(unsigned char)*TempSize2D);
#endif//_X64
	return true;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::BuildProfileValue(std::vector<float> &DataList)//計算剖線資料
{
	DataList.clear();
	if ( NULL == m_3DData ) { return false; }	

	int nX=0, nY=0;
	int i=0, j=0, idx=0;
	int nPosX1 = JetAPI::Floor(m_ClipPlanePosX1);
	int nPosY1 = JetAPI::Floor(m_ClipPlanePosY1);
	int nPosX2 = JetAPI::Floor(m_ClipPlanePosX2);
	int nPosY2 = JetAPI::Floor(m_ClipPlanePosY2);
	int nMinX = MIN(nPosX1, nPosX2);
	int nMinY = MIN(nPosY1, nPosY2);
	int nMaxX = MAX(nPosX1, nPosX2);
	int nMaxY = MAX(nPosY1, nPosY2);
	const float Resolution = (float)(1.0/m_Resolution);

	nPosX1 = MAX(0, nMinX);
	nPosY1 = MAX(0, nMinY);
	nPosX2 = MIN(nMaxX, m_3DWidth);
	nPosY2 = MIN(nMaxY, m_3DHeight);

	if ( nPosY1 == nPosY2 )
	{	//水平線
		nY = m_3DHeight-nPosY1-1;
		for ( j=nPosX1; j<nPosX2; j++ )
		{
			idx = (nY*m_3DWidth)+j;
			DataList.push_back(m_3DData[idx]*Resolution);
		}
	}	
	else if ( nPosX1 == nPosX2 )
	{	//垂直線
		for ( j=nPosY1; j<nPosY2; j++ )
		{
			nY = m_3DHeight-j-1;
			idx = (nY*m_3DWidth)+nPosX1;
			DataList.push_back(m_3DData[idx]*Resolution);
		}
	}
	else
	{	//斜線		
		nPosX1 = JetAPI::Floor(m_ClipPlanePosX1);
		nPosY1 = JetAPI::Floor(m_ClipPlanePosY1);
		nPosX2 = JetAPI::Floor(m_ClipPlanePosX2);
		nPosY2 = JetAPI::Floor(m_ClipPlanePosY2);

		double  dL=0, dX=0, dY=0, dRatio=0;
		const double dPX1 = nPosX1;
		const double dPY1 = nPosY1;
		const double dPX2 = nPosX2;
		const double dPY2 = nPosY2;
		const double dX12 = dPX2-dPX1;
		const double dY12 = dPY2-dPY1;
		const double dPXYL = sqrt((dX12*dX12)+(dY12*dY12));
		const int Line = JetAPI::Floor(dPXYL);
		for ( i=0; i<Line; i++ )
		{
			dL = i;
			dRatio = dL/dPXYL;
			dX = dPX1+(dRatio*dX12);
			dY = dPY1+(dRatio*dY12);
			nX = JetAPI::Floor(dX);
			nY = JetAPI::Floor(dY);
			nY = m_3DHeight-nY-1;
			if ( nX<0 || nX>=m_3DWidth ) { continue; }
			if ( nY<0 || nY>=m_3DHeight ) { continue; }
			idx = (nY*m_3DWidth)+nX;
			DataList.push_back(m_3DData[idx]*Resolution);
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
float* COpenGLWnd::Get3DData(IMAGE_SIZE &DataW, IMAGE_SIZE &DataH, IMAGE_SIZE &DataStep)
{
	DataW = m_3DWidth;
	DataH = m_3DHeight;
	DataStep = m_3DWidth;
	return m_3DData;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::Clone3DData(IMAGE_SIZE &DataW, IMAGE_SIZE &DataH, IMAGE_SIZE &DataStep, float*&Ptr)//複製3D資料
{
	const char fnName[]="COpenGLWnd::Clone3DData";
	float *Src=Get3DData(DataW, DataH, DataStep);
	if ( NULL == Src ) { return false; }
	const float Res=m_Resolution;
	const size_t BufferSize=ImageAPI.CalcBufferSize(DataStep, DataH);
	if ( 0 == BufferSize ) { return false; }
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "Ptr") == false )
	{	return false; }

	for ( size_t i=0; i<BufferSize; i++ )
	{
		Ptr[i] = Src[i];
		Ptr[i] /= Res;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::Set3DData(const int* p3D, const unsigned char *p2D, IMAGE_SIZE DataW, IMAGE_SIZE DataH, bool IsColor, float RulerMinH,float RulerMaxH, float ShowMinH, float ShowMaxH, RECT PadRect, RECT ROIRect, float PadSpecHeight)
{
	if( p3D == NULL ) { return; }
	if( p2D == NULL ) {	return; }		

	float Resolution = 0.0f;
	if( DataW > DataH ){ Resolution = (float)(DataW); }
	else{ Resolution = (float)(DataH); }
	Resolution /= 200.0f;	//數值大，z軸小
	m_Resolution = 0.025f * Resolution;
	//Kai-20161012-高度(1um), XY和FOV解析度有關, 所以高度/XY解析度=高度縮放值
	const double ResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const double Res = (ResX+ResY)*0.5;
	m_Resolution = (float)(1.0/Res);

//	m_UpperBoundHeight = -99999.0f;
//	m_UpperBoundHeight_GL = 0.0f;
	m_LowerBoundHeight = -99999.0f;
	m_LowerBoundHeight_GL = 0.0f;

	//fang 1040522
/*	double Rx, Ry;
	SPIDataCollect.GetCameraResolution(Rx, Ry);
	m_Resolution = 1/Rx;*/
	
	if( m_UserResolution > 0 ){ m_Resolution*= m_UserResolution; }

	m_ShowMode = OPENGL_SHOW_MODEL;
	CreateDataBuffer(DataW, DataH, IsColor);
	m_Resolution *= m_3DScale;

	m_RulerMinH = RulerMinH;
	m_RulerMaxH = RulerMaxH;

	m_ShowMinH = ShowMinH;
	m_ShowMaxH = ShowMaxH;	
	
	ConvertRectToGLBox(DataW, DataH, PadRect, PadSpecHeight, m_PadBox);
	ConvertRectToGLBox(DataW, DataH, ROIRect, PadSpecHeight, m_RoiBox);		

	//3D縮圖
	m_ScaleW = m_ScaleH = 1.0f;
	unsigned int i = 0, j = 0;
	const size_t BufferSize = DataH*DataW;
	::memset(m_3DData, 0x00, sizeof(float)*m_3DWidth*m_3DHeight);	
	if( m_3DScale == 1 )
	{
		for ( i=0; i<BufferSize; i++ )
		{
			m_3DData[i] = (float)(p3D[i]);
			m_3DData[i] = m_3DData[i]/INT_TO_FLOAT;
		}
	}
	else
	{
		if ( this->DoDataScaling(p3D, m_3DData, DataW, DataH, m_3DWidth, m_3DHeight) == false )
		{	return;	}		
	}

	ResetPoint();
	UpdateClipPlanePos();	
	//PreCreateModel
	//搜尋最大最小值
	
	const size_t Size3D = m_3DWidth*m_3DHeight;
	float TempMin = 0.0, TempMax = 0.0;
	float TempF = 0.0;

	for( i=0; i < Size3D ; i++ )
	{
		TempF = m_3DData[i];		
		if( i == 0 )
		{ TempMin = TempF;	TempMax = TempF; }
		else
		{
			if( TempF < TempMin ){ TempMin = TempF; }
			if( TempF > TempMax ){ TempMax = TempF; }
		}		
	}
	m_MaxH = TempMax;
	m_MinH = TempMin;
	if( (m_RulerMinH == -1) && (m_RulerMaxH == -1) )
	{
		m_RulerMinH = TempMin;
		m_RulerMaxH = TempMax;
	}
	m_RulerMinH_GL = m_RulerMinH*m_Resolution;
	m_RulerMaxH_GL = m_RulerMaxH*m_Resolution;


	if( (m_ShowMinH == -1) && (m_ShowMaxH == -1) )
	{
		m_ShowMinH = TempMin - 0.5f;
		m_ShowMaxH = TempMax + 0.5f;
	}
	m_ShowMinH_GL = m_ShowMinH*m_Resolution;
	m_ShowMaxH_GL = m_ShowMaxH*m_Resolution;
	
	m_PadBox_GL = m_PadBox;
	m_RoiBox_GL = m_RoiBox;
	ConvertBoxToGLBox(m_PadBox, m_PadBox_GL);
	ConvertBoxToGLBox(m_RoiBox, m_RoiBox_GL);

	if( m_UpperBoundHeight == -99999 )
	{	m_UpperBoundHeight = m_PadBox.fHeight;	}
	m_UpperBoundHeight_GL = m_UpperBoundHeight*m_Resolution;

	if( m_LowerBoundHeight == -99999 )
	{	m_LowerBoundHeight = m_ShowMinH;	}
	m_LowerBoundHeight_GL = m_LowerBoundHeight*m_Resolution;
	//

	//轉換成3D 世界高度 ( 這時會依照輸入的ShowMaxH ,ShowMinH 更改Model的高度值 )
	float *p3DData = m_3DData;
	for( i = 0 ; i< Size3D ; i++ )
	{
		TempF = p3DData[i];
		if( TempF >= UNWRAP_DATA_AVOID )
		{	TempF = 0;	}
		else
		{
			if( TempF < m_ShowMinH )//小於顯示的最小高度
			{
				//TempF = m_ShowMinH_GL;
				TempF = 0;
			}
			else if( TempF > m_ShowMaxH )//大於顯示的最大高度
			{	TempF = m_ShowMaxH_GL;	}
			else
			{	TempF = TempF*m_Resolution;	}
		}
		p3DData[i] = TempF;
	}
	
	//2D縮圖
	::memset(m_2DData, 0, sizeof(unsigned char)*m_TextureRealW*m_TextureHeight);
	if( p2D != NULL )
	{
		int i=0,j=0,Realindex=0;
		int scale_i=0,scale_j = 0;
		int index = 0,scale_index = 0;
		int Temp = 0,Temp2 = 0;
		float TempF = 0.0f,TempF2 = 0.0;

		if( m_IsColor == true )
		{//RGB
			//B G R
			int R= 0 ,G= 0,B = 0;
			for( i = 0;i<DataH;i++ )
			{
				index = i*DataW*3;				
				scale_i = (int)((i * m_TextureScale)+0.5);
				if(scale_i >= m_TextureHeight){scale_i = m_TextureHeight-1;}
				Temp2 = scale_i * m_TextureRealW;
				
				for( j = 0;j<DataW; j++ )
				{
					B = p2D[index++];
					G = p2D[index++];
					R = p2D[index++];
					
					scale_j = (int)((j * m_TextureScale)+0.5);
					if(scale_j >= m_TextureWidth){scale_j = m_TextureWidth-1;}
					scale_index = Temp2 + scale_j * 3;
					
					//B
					Temp = m_2DData[scale_index];
					if(Temp == 0)
					{	m_2DData[scale_index++] = B;	}
					else
					{	m_2DData[scale_index++] = (unsigned char)((Temp + B)*0.5f);	}

					//G
					Temp = m_2DData[scale_index];
					if(Temp == 0)
					{	m_2DData[scale_index++] = G;	}
					else
					{	m_2DData[scale_index++] = (unsigned char)((Temp + G)*0.5f);	}

					//R
					Temp = m_2DData[scale_index];
					if(Temp == 0)
					{	m_2DData[scale_index++] = R;	}
					else
					{	m_2DData[scale_index++] = (unsigned char)((Temp + R)*0.5f);	}
					
				}
			}
		}//RGB 縮圖
		else
		{//縮圖

			for(i = 0;i<DataH;i++)
			{
				for(j=0;j<DataW;j++)
				{
					index = i*DataW +j;
					//Temp = p2D[index] * 2;
					Temp = p2D[index];
					if(Temp > 255){Temp = 255;}
					scale_i = (int)((i * m_TextureScale)+0.5);
					if(scale_i >= m_TextureHeight){scale_i = m_TextureHeight-1;}
					scale_j = (int)((j * m_TextureScale)+0.5);
					if(scale_j >= m_TextureWidth){scale_j = m_TextureWidth-1;}
					scale_index = scale_i * m_TextureRealW + scale_j;

					Temp2 = m_2DData[scale_index];
					if(Temp2 == 0)
					{	m_2DData[scale_index] = Temp;	}
					else
					{	m_2DData[scale_index] = (unsigned char)((Temp2 + Temp)*0.5f);	}
				}
			}			
		}//縮圖 灰階
	}
	//CalcViewZoomValue(DataW, DataH);
	//SetModalCenter();	
	SetAnimationPlay(false);
	SetIsShowMainObject(true);
	this->Build3DObject();
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::Set3DData(const float* p3D, const unsigned char *p2D, IMAGE_SIZE DataW, IMAGE_SIZE DataH, bool IsColor, float RulerMinH,float RulerMaxH, float ShowMinH, float ShowMaxH, RECT PadRect, RECT ROIRect, float PadSpecHeight)
{//fang 1040522
	if( p3D == NULL ) { return; }
	if( p2D == NULL ) {	return; }		

	CString str;
	float Resolution = 0.0f;
	if( DataW > DataH ){ Resolution = (float)(DataW); }
	else{ Resolution = (float)(DataH); }
	Resolution /= 200.0f;	//數值大，z軸小
	m_Resolution = 0.025f * Resolution;
	//Kai-20161012-高度(1um), XY和FOV解析度有關, 所以高度/XY解析度=高度縮放值
	const double ResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const double Res = (ResX+ResY)*0.5;
	m_Resolution = (float)(1.0/Res);

//	m_UpperBoundHeight = -99999.0f;
//	m_UpperBoundHeight_GL = 0.0f;
	m_LowerBoundHeight = -99999.0f;
	m_LowerBoundHeight_GL = 0.0f;

	//fang 1040522
/*	double Rx, Ry;
	SPIDataCollect.GetCameraResolution(Rx, Ry);
	m_Resolution = 1/Rx;*/
	
	if( m_UserResolution > 0 ){ m_Resolution*= m_UserResolution; }

	m_ShowMode = OPENGL_SHOW_MODEL;

	CreateDataBuffer(DataW, DataH, IsColor);	
	m_Resolution *= m_3DScale;

	m_RulerMinH = RulerMinH;
	m_RulerMaxH = RulerMaxH;

	m_ShowMinH = ShowMinH;
	m_ShowMaxH = ShowMaxH;	
	
	ConvertRectToGLBox(DataW, DataH, PadRect, PadSpecHeight, m_PadBox);
	ConvertRectToGLBox(DataW, DataH, ROIRect, PadSpecHeight, m_RoiBox);		

	//3D縮圖
	m_ScaleW = m_ScaleH = 1.0f;
	::memset( m_3DData, 0x00, sizeof(float)*m_3DWidth*m_3DHeight);	
	if( m_3DScale == 1 )
	{	::memcpy( m_3DData, p3D, sizeof(float)*DataW*DataH);	}
	else
	{
		if ( this->DoDataScaling(p3D, m_3DData, DataW, DataH, m_3DWidth, m_3DHeight) == false )
		{	return;	}		
	}

	ResetPoint();
	UpdateClipPlanePos();
	//PreCreateModel
	//搜尋最大最小值
	unsigned int i = 0, j = 0;
	const size_t Size3D = m_3DWidth*m_3DHeight;
	float TempMin = 0.0, TempMax = 0.0;
	float TempF = 0.0;

	for( i=0; i < Size3D ; i++ )
	{
		TempF = m_3DData[i];		
		if( i == 0 )
		{ TempMin = TempF;	TempMax = TempF; }
		else
		{
			if( TempF < TempMin ){ TempMin = TempF; }
			if( TempF > TempMax ){ TempMax = TempF; }
		}		
	}

	m_MaxH = TempMax;
	m_MinH = TempMin;
	if( (m_RulerMinH == -1) && (m_RulerMaxH == -1) )
	{
		m_RulerMinH = TempMin;
		m_RulerMaxH = TempMax;
	}
	m_RulerMinH_GL = m_RulerMinH * m_Resolution;
	m_RulerMaxH_GL = m_RulerMaxH * m_Resolution;


	if( (m_ShowMinH == -1) && (m_ShowMaxH == -1) )
	{
		m_ShowMinH = TempMin - 0.5f;
		m_ShowMaxH = TempMax + 0.5f;
	}
	m_ShowMinH_GL = m_ShowMinH * m_Resolution;
	m_ShowMaxH_GL = m_ShowMaxH * m_Resolution;


	m_PadBox_GL = m_PadBox;	
	m_RoiBox_GL = m_RoiBox;
	ConvertBoxToGLBox(m_PadBox, m_PadBox_GL);
	ConvertBoxToGLBox(m_RoiBox, m_RoiBox_GL);

	if( m_UpperBoundHeight == -99999 )
	{	m_UpperBoundHeight = m_PadBox.fHeight;	} 
	m_UpperBoundHeight_GL = m_UpperBoundHeight*m_Resolution;

	if( m_LowerBoundHeight == -99999 )
	{	m_LowerBoundHeight = m_ShowMinH;	}
	m_LowerBoundHeight_GL = m_LowerBoundHeight*m_Resolution;
	//

	//轉換成3D 世界高度 ( 這時會依照輸入的ShowMaxH ,ShowMinH 更改Model的高度值 )
	float *p3DData = m_3DData;
	for( i = 0 ; i< Size3D ; i++ )
	{
		TempF = p3DData[i];
		if( TempF >= UNWRAP_DATA_AVOID )
		{	TempF = 0;	}
		else
		{
			if( TempF < m_ShowMinH )//小於顯示的最小高度
			{
				//TempF = m_ShowMinH_GL;
				TempF = 0;
			}
			else if( TempF > m_ShowMaxH )//大於顯示的最大高度
			{	TempF = m_ShowMaxH_GL;	}
			else
			{	TempF = TempF*m_Resolution;	}
		}
		p3DData[i] = TempF;
	}
	
	//2D縮圖
	::memset(m_2DData, 0, sizeof(unsigned char)*m_TextureRealW*m_TextureHeight);
	if( p2D != NULL )
	{
		int i=0,j=0,Realindex=0;
		int scale_i=0,scale_j = 0;
		int index = 0,scale_index = 0;
		int Temp = 0,Temp2 = 0;
		float TempF = 0.0f,TempF2 = 0.0;

		if( m_IsColor == true )
		{//RGB
			//B G R
			int R= 0 ,G= 0,B = 0;
			for( i = 0;i<DataH;i++ )
			{
				index = i * DataW * 3;
				scale_i = (int)((i * m_TextureScale)+0.5);
				if(scale_i >= m_TextureHeight){scale_i = m_TextureHeight-1;}
				Temp2 = scale_i * m_TextureRealW;
				
				for( j = 0;j<DataW; j++ )
				{
					B = p2D[index++];
					G = p2D[index++];
					R = p2D[index++];
					
					scale_j = (int)((j * m_TextureScale)+0.5);
					if(scale_j >= m_TextureWidth){scale_j = m_TextureWidth-1;}
					scale_index = Temp2 + scale_j * 3;
					
					//B
					Temp = m_2DData[scale_index];
					if(Temp == 0)
					{	m_2DData[scale_index++] = B;	}
					else
					{	m_2DData[scale_index++] = (unsigned char)((Temp + B)*0.5f); }
					//G
					Temp = m_2DData[scale_index];
					if(Temp == 0)
					{	m_2DData[scale_index++] = G;	}
					else
					{	m_2DData[scale_index++] = (unsigned char)((Temp + G)*0.5f); }
					//R
					Temp = m_2DData[scale_index];
					if(Temp == 0)
					{	m_2DData[scale_index++] = R;	}
					else
					{	m_2DData[scale_index++] = (unsigned char)((Temp + R)*0.5f);	}
					
				}
			}
		}//RGB 縮圖
		else
		{//縮圖

			for(i = 0;i<DataH;i++)
			{
				for(j=0;j<DataW;j++)
				{
					index = i*DataW +j;
					//Temp = p2D[index] * 2;
					Temp = p2D[index];
					if(Temp > 255){Temp = 255;}
					scale_i = (int)((i * m_TextureScale)+0.5);
					if(scale_i >= m_TextureHeight){scale_i = m_TextureHeight-1;}
					scale_j = (int)((j * m_TextureScale)+0.5);
					if(scale_j >= m_TextureWidth){scale_j = m_TextureWidth-1;}
					scale_index = scale_i * m_TextureRealW + scale_j;

					Temp2 = m_2DData[scale_index];
					if(Temp2 == 0)
					{	m_2DData[scale_index] = Temp;	}
					else
					{	m_2DData[scale_index] = (unsigned char)((Temp2 + Temp)*0.5f);	}
				}
			}			
		}//縮圖 灰階
	}

	//CalcViewZoomValue(DataW, DataH);
	//SetModalCenter();	
	SetAnimationPlay(false);
	SetIsShowMainObject(true);
	Build3DObject();
	InvalidateRect(NULL,FALSE);
	
	str.Format(_T("3D-Width:%d, 3D-Height:%d"), m_3DWidth, m_3DHeight);
	//AOIDataCollect.SaveLogMessage(str);
	str.Format(_T("Translate(%.0f, %.0f)"), m_xTranslate, m_yTranslate);
	//AOIDataCollect.SaveLogMessage(str);
	str.Format(_T("Rotate(%.0f, %.0f)"), m_xRotate, m_xRotate);
	//AOIDataCollect.SaveLogMessage(str);
	str.Format(_T("Scale(%.0f, %.0f, %.0f)"), m_ScaleX, m_ScaleY, m_ScaleZ);
	//AOIDataCollect.SaveLogMessage(str);	
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::CalcViewZoomValue(int DataW, int DataH)//計算顯示縮放比例
{
	if ( DataW < 1 || DataH < 1 ) { return false; }

	RECT  WndRect={0,0,0,0};
	CWnd::GetClientRect(&WndRect);	

	double Ratio = 0.5;
	double Zoom = 1.0;
	double WndW = WndRect.right-WndRect.left;
	double WndH = WndRect.bottom-WndRect.top;
	double ScaleX = WndW/DataW;
	double ScaleY = WndH/DataH;
	int    NewWndW = (int)(DataW*ScaleY);
	int    NewWndH = (int)(DataH*ScaleX);

#ifdef _DEBUG
	Ratio = 0.5;
#else
	Ratio = 0.5;
#endif//_DEBUG

	if ( NewWndW > WndW )
	{	Zoom = 1/ScaleX;	}
	else
	{	Zoom = 1/ScaleY;	}
	Zoom = Zoom*Ratio;

	float ViewScale = (float)(1.0/Zoom);
	float ViewScaleX = ViewScale*1.0f;//m_ScaleW;
	float ViewScaleY = ViewScale*1.0f;//m_ScaleH;
	ViewScale = MAX(ViewScaleX, ViewScaleY);
	//if ( ViewScale < 1.0f ) 
	//{	ViewScale = 1.0f; }
	m_ScaleX_Fit = ViewScale;//5.0f
	m_ScaleY_Fit = ViewScale;//5.0f
	m_ScaleZ_Fit = ViewScale;//5.0f

	m_ScaleX = m_ScaleX_Fit;
	m_ScaleY = m_ScaleY_Fit;
	m_ScaleZ = m_ScaleZ_Fit;
	return true;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::GetShowImageSize(int &ImageW, int &ImageH)
{
	ImageW = 0;
	ImageH = 0;
	
	RECT WinRect = {0};
	this->GetClientRect(&WinRect);
	ImageW = WinRect.right;
	ImageH = WinRect.bottom;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::ExtractShowImage(unsigned char *pImage, const int ImageW, const int ImageH)
{
	if( pImage == NULL )
	{
		m_ErrorString.Format(_T("pImage == NULL!"));
		return false;
	}
	
	RECT WinRect = {0};
	this->GetClientRect(&WinRect);
	const int WindowWidth = WinRect.right;
	const int WindowHeight = WinRect.bottom;
	
	if( ImageW != WindowWidth )
	{
		m_ErrorString.Format(_T("ImageW (%d) != WindowWidth (%d)"), ImageW, WindowWidth);
		return false;
	}
	
	if( ImageH != WindowHeight )
	{
		m_ErrorString.Format(_T("ImageH (%d) != WindowHeight (%d)"), ImageH, WindowHeight);
		return false;
	}
	
	const int ImageSize = ImageW * ImageH * 3;
	::memset(pImage, 0 , sizeof(unsigned char)*ImageSize);
	
	//配置四的倍數的空間
	int RealW = GetNBytesPerLine(WindowWidth * 3);
	GLint    PixelDataLength =  RealW * WindowHeight;
    GLubyte* pPixelData = new GLubyte[PixelDataLength];
	
	glReadBuffer(GL_FRONT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, WindowWidth, WindowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);
	
	//轉成不是四的倍數
	int i = 0;
	int PerLineByte = ImageW*3;
	int Offset=0;
	int ROffset=0;
	for( i = 0 ; i< ImageH ; i++ )
	{
		memcpy(pImage + Offset, pPixelData + ROffset , PerLineByte);
		Offset += ImageW*3;
		ROffset += RealW;
	}
	
	delete[] pPixelData; pPixelData = NULL;
	
	return true;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetMainObjectMode(OPENGL_OBJECT_MODE Mode)
{
	this->m_MainObjectMode = Mode;
	BuildMainObject();
	InvalidateRect(NULL,FALSE);	
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetIsShowMainObject(bool bShow)
{
	m_IsShowMainObject = bShow;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::GetIsShowMainObject() const
{
	return m_IsShowMainObject;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetZoomScale(float ScaleX, float ScaleY, float ScaleZ)
{
	m_ScaleX = ScaleX;
	m_ScaleY = ScaleY;
	m_ScaleZ = ScaleZ;
	Invalidate();
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::GetZoomScale(float &ScaleX, float &ScaleY, float &ScaleZ) const
{
	ScaleX = m_ScaleX;
	ScaleY = m_ScaleY;
	ScaleZ = m_ScaleZ;	
}
//-------------------------------------------------------------------------------------//
float COpenGLWnd::GetRuleMaxH() const
{
	return m_RulerMaxH;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetRuleMaxH(float val)
{
	m_RulerMaxH = val;
	m_RulerMaxH_GL = m_RulerMaxH*m_Resolution;
	m_ShowMaxH = val+0.5;
	m_ShowMaxH_GL = m_ShowMaxH * m_Resolution;

	BuildMainObject();
	InvalidateRect(NULL,FALSE);	
	return;
}
//-------------------------------------------------------------------------------------//
float COpenGLWnd::GetRuleMinH() const
{
	return m_RulerMinH;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetRuleMinH(float val)
{
	m_RulerMinH = val;
	m_RulerMinH_GL = m_RulerMinH*m_Resolution;

	m_ShowMinH = val-0.5f;
	m_ShowMinH_GL = m_ShowMinH * m_Resolution;
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetModalCenter(bool ResetRotated)
{
	float ScaleX=1.0f, ScaleY=1.0f, ScaleZ=1.0f;
#ifdef _DEBUG
	ScaleX = 2.0f;//5.0f
	ScaleY = 2.0f;//5.0f
	ScaleZ = 2.0f;//5.0f
#else
	ScaleX = 1.0f;//5.0f
	ScaleY = 1.0f;//5.0f
	ScaleZ = 1.0f;//5.0f
#endif//_DEBUG
	CalcViewZoomValue(m_3DWidth, m_3DHeight);
	//m_ScaleX = m_ScaleX_Fit;
	//m_ScaleY = m_ScaleY_Fit;
	//m_ScaleZ = m_ScaleZ_Fit;
	ScaleX = m_ScaleX;
	ScaleY = m_ScaleY;
	ScaleZ = m_ScaleZ;
	SetModalCenterScale(ResetRotated, ScaleX, ScaleY, ScaleZ);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetModalCenterScale(bool ResetRotated, float sX, float sY, float sZ)
{
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	sX *= (float)(SystemParam.m_3DObjectDrawScaleX);
	sY *= (float)(SystemParam.m_3DObjectDrawScaleY);
	sZ *= (float)(SystemParam.m_3DObjectDrawScaleZ);

	if ( true == ResetRotated )
	{
		m_xRotate = -90;
		m_yRotate = 0;	
	}
	m_xTranslate= 0.0f;
	m_yTranslate= 0.0f;	
	
	m_ScaleX = sX;
	m_ScaleY = sY;
	m_ScaleZ = sZ;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetShowColorRuler(bool bShow)
{
	m_IsShowColorRuler = bShow;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetDetailLevel(int nLv)
{
	m_DetailLevel = nLv;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetBoundPadOpen(bool IsOpen)
{
	m_IsShowPadRegion = IsOpen;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetPadRegion(const RECT &BoxRect, float Height)
{
	const float Cpx = (float)((BoxRect.left+BoxRect.right)*0.5f);
	const float Cpy = (float)((BoxRect.top+BoxRect.bottom)*0.5f);
	const float RectW = (float)(BoxRect.right-BoxRect.left);
	const float RectH = (float)(BoxRect.bottom-BoxRect.top);
	m_PadBox.fPtX_1 = Cpx-(RectW*0.5f);
	m_PadBox.fPtY_1 = Cpy-(RectH*0.5f);
	m_PadBox.fPtX_2 = Cpx+(RectW*0.5f);
	m_PadBox.fPtY_2 = Cpy-(RectH*0.5f);
	m_PadBox.fPtX_3 = Cpx+(RectW*0.5f);
	m_PadBox.fPtY_3 = Cpy+(RectH*0.5f);
	m_PadBox.fPtX_4 = Cpx-(RectW*0.5f);
	m_PadBox.fPtY_4 = Cpy+(RectH*0.5f);
	m_PadBox_GL.fPtX_1 = m_PadBox.fPtX_1/m_ScaleW;
	m_PadBox_GL.fPtY_1 = m_PadBox.fPtY_1/m_ScaleH;
	m_PadBox_GL.fPtX_2 = m_PadBox.fPtX_2/m_ScaleW;
	m_PadBox_GL.fPtY_2 = m_PadBox.fPtY_2/m_ScaleH;
	m_PadBox_GL.fPtX_3 = m_PadBox.fPtX_3/m_ScaleW;
	m_PadBox_GL.fPtY_3 = m_PadBox.fPtY_3/m_ScaleH;
	m_PadBox_GL.fPtX_4 = m_PadBox.fPtX_4/m_ScaleW;
	m_PadBox_GL.fPtY_4 = m_PadBox.fPtY_4/m_ScaleH;	
	if ( Height > 0 ) 
	{
		m_PadBox.fHeight = Height;
		m_PadBox_GL.fHeight = m_PadBox.fHeight*m_Resolution;
	}
	BuildPadRegion();
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetPadCornerPts(const TPOINT2D BoxCornerPts[], float Height)
{
	m_PadBox.fPtX_1 = (float)(BoxCornerPts[0].x);
	m_PadBox.fPtY_1 = (float)(BoxCornerPts[0].y);
	m_PadBox.fPtX_2 = (float)(BoxCornerPts[1].x);
	m_PadBox.fPtY_2 = (float)(BoxCornerPts[1].y);
	m_PadBox.fPtX_3 = (float)(BoxCornerPts[2].x);
	m_PadBox.fPtY_3 = (float)(BoxCornerPts[2].y);
	m_PadBox.fPtX_4 = (float)(BoxCornerPts[3].x);
	m_PadBox.fPtY_4 = (float)(BoxCornerPts[3].y);
	m_PadBox_GL.fPtX_1 = m_PadBox.fPtX_1/m_ScaleW;
	m_PadBox_GL.fPtY_1 = m_PadBox.fPtY_1/m_ScaleH;
	m_PadBox_GL.fPtX_2 = m_PadBox.fPtX_2/m_ScaleW;
	m_PadBox_GL.fPtY_2 = m_PadBox.fPtY_2/m_ScaleH;
	m_PadBox_GL.fPtX_3 = m_PadBox.fPtX_3/m_ScaleW;
	m_PadBox_GL.fPtY_3 = m_PadBox.fPtY_3/m_ScaleH;
	m_PadBox_GL.fPtX_4 = m_PadBox.fPtX_4/m_ScaleW;
	m_PadBox_GL.fPtY_4 = m_PadBox.fPtY_4/m_ScaleH;	
	if ( Height > 0 ) 
	{
		m_PadBox.fHeight = Height;
		m_PadBox_GL.fHeight = m_PadBox.fHeight*m_Resolution;
	}
	BuildPadRegion();
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetBoundRoiOpen(bool IsOpen)
{
	m_IsShowRoiRegion = IsOpen;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetRoiRegion(const RECT &BoxRect, float Height)
{	
	const float Cpx = (float)((BoxRect.left+BoxRect.right)*0.5f);
	const float Cpy = (float)((BoxRect.top+BoxRect.bottom)*0.5f);
	const float RectW = (float)(BoxRect.right-BoxRect.left);
	const float RectH = (float)(BoxRect.bottom-BoxRect.top);
	m_RoiBox.fPtX_1 = Cpx-(RectW*0.5f);
	m_RoiBox.fPtY_1 = Cpy-(RectH*0.5f);
	m_RoiBox.fPtX_2 = Cpx+(RectW*0.5f);
	m_RoiBox.fPtY_2 = Cpy-(RectH*0.5f);
	m_RoiBox.fPtX_3 = Cpx+(RectW*0.5f);
	m_RoiBox.fPtY_3 = Cpy+(RectH*0.5f);
	m_RoiBox.fPtX_4 = Cpx-(RectW*0.5f);
	m_RoiBox.fPtY_4 = Cpy+(RectH*0.5f);
	m_RoiBox_GL.fPtX_1 = m_RoiBox.fPtX_1/m_ScaleW;
	m_RoiBox_GL.fPtY_1 = m_RoiBox.fPtY_1/m_ScaleH;
	m_RoiBox_GL.fPtX_2 = m_RoiBox.fPtX_2/m_ScaleW;
	m_RoiBox_GL.fPtY_2 = m_RoiBox.fPtY_2/m_ScaleH;
	m_RoiBox_GL.fPtX_3 = m_RoiBox.fPtX_3/m_ScaleW;
	m_RoiBox_GL.fPtY_3 = m_RoiBox.fPtY_3/m_ScaleH;
	m_RoiBox_GL.fPtX_4 = m_RoiBox.fPtX_4/m_ScaleW;
	m_RoiBox_GL.fPtY_4 = m_RoiBox.fPtY_4/m_ScaleH;	

	if ( Height > 0 ) 
	{
		m_RoiBox.fHeight = Height;
		m_RoiBox_GL.fHeight = m_RoiBox.fHeight*m_Resolution;
	}
	BuildRoiRegion();
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetRoiCornerPts(const TPOINT2D BoxCornerPts[], float Height)
{
	m_RoiBox.fPtX_1 = (float)(BoxCornerPts[0].x);
	m_RoiBox.fPtY_1 = (float)(BoxCornerPts[0].y);
	m_RoiBox.fPtX_2 = (float)(BoxCornerPts[1].x);
	m_RoiBox.fPtY_2 = (float)(BoxCornerPts[1].y);
	m_RoiBox.fPtX_3 = (float)(BoxCornerPts[2].x);
	m_RoiBox.fPtY_3 = (float)(BoxCornerPts[2].y);
	m_RoiBox.fPtX_4 = (float)(BoxCornerPts[3].x);
	m_RoiBox.fPtY_4 = (float)(BoxCornerPts[3].y);
	m_RoiBox_GL.fPtX_1 = m_RoiBox.fPtX_1/m_ScaleW;
	m_RoiBox_GL.fPtY_1 = m_RoiBox.fPtY_1/m_ScaleH;
	m_RoiBox_GL.fPtX_2 = m_RoiBox.fPtX_2/m_ScaleW;
	m_RoiBox_GL.fPtY_2 = m_RoiBox.fPtY_2/m_ScaleH;
	m_RoiBox_GL.fPtX_3 = m_RoiBox.fPtX_3/m_ScaleW;
	m_RoiBox_GL.fPtY_3 = m_RoiBox.fPtY_3/m_ScaleH;
	m_RoiBox_GL.fPtX_4 = m_RoiBox.fPtX_4/m_ScaleW;
	m_RoiBox_GL.fPtY_4 = m_RoiBox.fPtY_4/m_ScaleH;	
	if ( Height > 0 ) 
	{
		m_RoiBox.fHeight = Height;
		m_RoiBox_GL.fHeight = m_RoiBox.fHeight*m_Resolution;
	}
	BuildRoiRegion();
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetBasePlaneOpen(bool IsOpen)
{
	m_IsBaseOpen = IsOpen;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetUpperPlaneOpen(bool IsOpen)
{
	m_IsUpperOpen = IsOpen;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetLowerPlaneOpen(bool IsOpen)
{
	m_IsLowerOpen = IsOpen;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetClipPlaneOpen(bool IsOpen)
{
	m_IsClipOpen = IsOpen;	
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetClipPlaneDir(int Dir)
{
	m_ClipPlaneDir = Dir;
	UpdateClipPlanePos();
	BuildClipPlane();
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetIsShowPoint(bool IsOpen)
{
	m_IsShowPoint = IsOpen;	
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetIsShowAxis(bool Is)
{
	m_IsShowAxis = Is;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::UpdateClipPlanePos()
{
	if ( OPENGL_CLIP_PLANE_VER == m_ClipPlaneDir )
	{
		m_ClipPlanePosX1 = (float)(m_3DWidth/2);
		m_ClipPlanePosY1 = 0.0f;	
		m_ClipPlanePosX2 = (float)(m_3DWidth/2);
		m_ClipPlanePosY2 = (float)(m_3DHeight);
	}
	if ( OPENGL_CLIP_PLANE_HOR == m_ClipPlaneDir )
	{
		m_ClipPlanePosX1 = 0.0f;
		m_ClipPlanePosY1 = (float)(m_3DHeight/2);
		m_ClipPlanePosX2 = (float)(m_3DWidth);
		m_ClipPlanePosY2 = (float)(m_3DHeight/2);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::ChangeClipPlaneSize(bool bBigger)
{
	float dX = m_ClipPlanePosX2-m_ClipPlanePosX1;
	float dY = m_ClipPlanePosY2-m_ClipPlanePosY1;
	float dCX = (m_ClipPlanePosX1+m_ClipPlanePosX2)/2;
	float dCY = (m_ClipPlanePosY1+m_ClipPlanePosY2)/2;
	if ( true == bBigger )
	{
		dX = (dX*1.1f*0.5f);
		dY = (dY*1.1f*0.5f);
	}
	else
	{
		dX = (dX*0.9f*0.5f);
		dY = (dY*0.9f*0.5f);
	}
	if ( m_ClipPlanePosX1 > m_ClipPlanePosX2 ) 
	{	
		m_ClipPlanePosX1 = dCX+dX; 
		m_ClipPlanePosX2 = dCX-dX; 
	}
	else
	{
		m_ClipPlanePosX1 = dCX-dX; 
		m_ClipPlanePosX2 = dCX+dX; 
	}
	if ( m_ClipPlanePosY1 > m_ClipPlanePosY2 ) 
	{	
		m_ClipPlanePosY1 = dCY+dY; 
		m_ClipPlanePosY2 = dCY-dY; 
	}
	else
	{
		m_ClipPlanePosY1 = dCY-dY; 
		m_ClipPlanePosY2 = dCY+dY; 
	}
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SetMouseDisplayScale(float Scale)
{
	m_ScaleX = Scale;
	m_ScaleY = Scale;
	m_ScaleZ = Scale;
	InvalidateRect(NULL,FALSE);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::GetMouseDisplayScale(float &Scale)
{
	Scale = m_ScaleZ;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::DoDataScaling(const int *pSrc3D, float *pDest3D, int SrcW, int SrcH, int DestW, int DestH)
{
	if ( NULL == pSrc3D ) { return false; }
	if ( NULL == pDest3D ) { return false; }

	int i=0, j=0, k=0;
	int u=0, v=0, w=0;
	double ScaleX=0, ScaleY=0;
	ScaleX = SrcW;
	ScaleX = ScaleX/DestW;
	ScaleY = SrcH;
	ScaleY = ScaleY/DestH;

	for ( i=0; i<DestH; i++ )
	{
		u = (int)(i*ScaleY);

		k = i*DestW;
		w = u*SrcW;
		for ( j=0; j<DestW; j++ )
		{
			v = (int)(j*ScaleX);
			pDest3D[k+j] = (float)(pSrc3D[w+v]);
			pDest3D[k+j] = pDest3D[k+j]/INT_TO_FLOAT;
		}
	}

	m_ScaleW = (float)(ScaleX);
	m_ScaleH = (float)(ScaleY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::DoDataScaling(const float *pSrc3D, float *pDest3D, int SrcW, int SrcH, int DestW, int DestH)
{
	if ( NULL == pSrc3D ) { return false; }
	if ( NULL == pDest3D ) { return false; }

	int i=0, j=0, k=0;
	int u=0, v=0, w=0;
	double ScaleX=0, ScaleY=0;
	ScaleX = SrcW;
	ScaleX = ScaleX/DestW;
	ScaleY = SrcH;
	ScaleY = ScaleY/DestH;

	for ( i=0; i<DestH; i++ )
	{
		u = (int)(i*ScaleY);

		k = i*DestW;
		w = u*SrcW;
		for ( j=0; j<DestW; j++ )
		{
			v = (int)(j*ScaleX);
			pDest3D[k+j] = pSrc3D[w+v];
		}
	}
	m_ScaleW = (float)(ScaleX);
	m_ScaleH = (float)(ScaleY);
	return true;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch ( nIDEvent )
	{
	case TIMER_ID_ANIMATION:
		if ( false == m_AnimationPlay )
		{	CWnd::KillTimer(nIDEvent); }

		m_yRotate += 1.0;
		m_xRotate += 0.0;
		InvalidateRect(NULL,FALSE);
		break;
	}
	CStatic::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::ConvertRectToGLBox(int W, int H, const RECT &Rect, float Height, TOpenGLBox &glBox)//將Rect轉成TOpenGLBox
{	
	const float CpX = (float)((Rect.left+Rect.right)*0.5f);
	//const float CpY = (float)((Rect.top+Rect.bottom)*0.5f);
	const float CpYTmp = (float)((Rect.top+Rect.bottom)*0.5f);
	const float CpY = (float)(H-CpYTmp);
	const float RectW = (float)(Rect.right-Rect.left);
	const float RectH = (float)(Rect.bottom-Rect.top);
	glBox.fPtX_1 = CpX-(RectW*0.5f);
	glBox.fPtY_1 = CpY-(RectH*0.5f);
	glBox.fPtX_2 = CpX+(RectW*0.5f);
	glBox.fPtY_2 = CpY-(RectH*0.5f);
	glBox.fPtX_3 = CpX+(RectW*0.5f);
	glBox.fPtY_3 = CpY+(RectH*0.5f);
	glBox.fPtX_4 = CpX-(RectW*0.5f);
	glBox.fPtY_4 = CpY+(RectH*0.5f);
	glBox.fHeight = Height;

#ifdef _DEBUG
	RECT       RectTmp;
	TOpenGLBox glBoxTmp=glBox;
	ConvertGLBoxToRect(glBoxTmp, RectTmp);
#endif//_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::ConvertCornerPtToGLBox(const TPOINT2D BoxCornerPts[], float Height, TOpenGLBox &glBox)//將四角端點轉成TOpenGLBox
{
	glBox.fPtX_1 = (float)(BoxCornerPts[0].x);
	glBox.fPtY_1 = (float)(BoxCornerPts[0].y);
	glBox.fPtX_2 = (float)(BoxCornerPts[1].x);
	glBox.fPtY_2 = (float)(BoxCornerPts[1].y);
	glBox.fPtX_3 = (float)(BoxCornerPts[2].x);
	glBox.fPtY_3 = (float)(BoxCornerPts[2].y);
	glBox.fPtX_4 = (float)(BoxCornerPts[3].x);
	glBox.fPtY_4 = (float)(BoxCornerPts[3].y);
	glBox.fHeight = Height;
	return true;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::ConvertGLBoxToRect(const TOpenGLBox &glBox, RECT &Rect)
{
	Rect.left = (int)(MIN(glBox.fPtX_1, glBox.fPtX_2));
	Rect.top = (int)(MIN(glBox.fPtY_1, glBox.fPtY_2));
	Rect.right = (int)(MAX(glBox.fPtX_1, glBox.fPtX_2));
	Rect.bottom = (int)(MAX(glBox.fPtY_1, glBox.fPtY_2));

	Rect.left = (int)(MIN(Rect.left, glBox.fPtX_3));
	Rect.top = (int)(MIN(Rect.top, glBox.fPtY_3));
	Rect.right = (int)(MAX(Rect.right, glBox.fPtX_3));
	Rect.bottom = (int)(MAX(Rect.bottom, glBox.fPtY_3));

	Rect.left = (int)(MIN(Rect.left, glBox.fPtX_4));
	Rect.top = (int)(MIN(Rect.top, glBox.fPtY_4));
	Rect.right = (int)(MAX(Rect.right, glBox.fPtX_4));
	Rect.bottom = (int)(MAX(Rect.bottom, glBox.fPtY_4));

	int v1 = m_3DHeight-Rect.top;
	int v2 = m_3DHeight-Rect.bottom;
	Rect.top = MIN(v1, v2);
	Rect.bottom = MAX(v1, v2);

	Rect.left = MAX(Rect.left, 0);
	Rect.top = MAX(Rect.top, 0);	
	Rect.right = MAX(Rect.right, 0);
	Rect.bottom = MAX(Rect.bottom, 0);

	Rect.left = MIN(Rect.left, m_3DWidth);
	Rect.top = MIN(Rect.top, m_3DHeight);
	Rect.right = MIN(Rect.right, m_3DWidth);
	Rect.bottom = MIN(Rect.bottom, m_3DHeight);
	return true;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::ConvertBoxToGLBox(const TOpenGLBox &Box, TOpenGLBox &Box_GL)//將Box轉成3D下的GL-Box
{
	Box_GL = Box;
	Box_GL.fPtX_1 = Box.fPtX_1/m_ScaleW;
	Box_GL.fPtY_1 = Box.fPtY_1/m_ScaleH;
	Box_GL.fPtX_2 = Box.fPtX_2/m_ScaleW;
	Box_GL.fPtY_2 = Box.fPtY_2/m_ScaleH;
	Box_GL.fPtX_3 = Box.fPtX_3/m_ScaleW;
	Box_GL.fPtY_3 = Box.fPtY_3/m_ScaleH;
	Box_GL.fPtX_4 = Box.fPtX_4/m_ScaleW;
	Box_GL.fPtY_4 = Box.fPtY_4/m_ScaleH;	
	Box_GL.fHeight = Box.fHeight*m_Resolution;
	return true;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::GetMinValueIndex(float valL, float valT, float valR, float valB, int RankIdx[], float RankVal[])//取得最低值引數(1:L, 2:T, 3:R, 4:B);
{
	CSortObj  SortObj;
	std::vector<CSortObj> SortList;
	SortObj.SetSortMode(SORT_BY_DBL);
	SortObj.SetValueDbl(valL);
	SortObj.SetValueInt(MIN_HEIGHT_AT_LEFT);
	SortList.push_back(SortObj);

	SortObj.SetValueDbl(valT);
	SortObj.SetValueInt(MIN_HEIGHT_AT_TOP);
	SortList.push_back(SortObj);

	SortObj.SetValueDbl(valR);
	SortObj.SetValueInt(MIN_HEIGHT_AT_RIGHT);
	SortList.push_back(SortObj);

	SortObj.SetValueDbl(valB);
	SortObj.SetValueInt(MIN_HEIGHT_AT_BOTTOM);
	SortList.push_back(SortObj);

	std::sort(SortList.begin(), SortList.end());
	
	RankIdx[0] = SortList[0].GetValueInt();
	RankVal[0] = (float)(SortList[0].GetValueDbl());

	RankIdx[1] = SortList[1].GetValueInt();
	RankVal[1] = (float)(SortList[1].GetValueDbl());

	RankIdx[2] = SortList[2].GetValueInt();
	RankVal[2] = (float)(SortList[2].GetValueDbl());

	RankIdx[3] = SortList[3].GetValueInt();
	RankVal[3] = (float)(SortList[3].GetValueDbl());
	return true;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::GetMaxValueIndex(float valL, float valT, float valR, float valB, int RankIdx[], float RankVal[])//取得最高值引數(1:L, 2:T, 3:R, 4:B);
{	
	CSortObj  SortObj;
	std::vector<CSortObj> SortList;
	SortObj.SetSortMode(SORT_BY_DBL);
	SortObj.SetValueDbl(valL);
	SortObj.SetValueInt(MAX_HEIGHT_AT_LEFT);
	SortList.push_back(SortObj);

	SortObj.SetValueDbl(valT);
	SortObj.SetValueInt(MAX_HEIGHT_AT_TOP);
	SortList.push_back(SortObj);

	SortObj.SetValueDbl(valR);
	SortObj.SetValueInt(MAX_HEIGHT_AT_RIGHT);
	SortList.push_back(SortObj);

	SortObj.SetValueDbl(valB);
	SortObj.SetValueInt(MAX_HEIGHT_AT_BOTTOM);
	SortList.push_back(SortObj);

	std::sort(SortList.begin(), SortList.end());
	
	RankIdx[0] = SortList[3].GetValueInt();
	RankVal[0] = (float)(SortList[3].GetValueDbl());

	RankIdx[1] = SortList[2].GetValueInt();
	RankVal[1] = (float)(SortList[2].GetValueDbl());

	RankIdx[2] = SortList[1].GetValueInt();
	RankVal[2] = (float)(SortList[1].GetValueDbl());

	RankIdx[3] = SortList[0].GetValueInt();	
	RankVal[3] = (float)(SortList[0].GetValueDbl());
	return 0;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::PostParentWndMsg(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	CWnd *ParentWnd = GetParent();
	if ( NULL == ParentWnd ) { return ; }
	HWND hWnd = ParentWnd->GetSafeHwnd();
	if ( NULL == hWnd ) { return ; }
	::PostMessage(hWnd, Msg, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::SendParentWndMsg(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	CWnd *ParentWnd = GetParent();
	if ( NULL == ParentWnd ) { return ; }
	HWND hWnd = ParentWnd->GetSafeHwnd();
	if ( NULL == hWnd ) { return ; }
	::SendMessage(hWnd, Msg, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::MapWndToOpenGL(int WndX, int WndY, double &PosX, double &PosY, double &PosZ)
{
	GLint viewport[4]={0};
	GLdouble modelview[16]={0};
	GLdouble projection[16]={0};
	GLfloat winX=0, winY=0, winZ=0;
	GLdouble posX=0, posY=0, posZ=0;

	glPushMatrix();
	//.....
	SetViewPortMatrix();

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	glPopMatrix();

	winX = WndX;
	winY = viewport[3]-WndY;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	PosX =  posX;	
	PosY =  posZ;
	PosZ =  posY;
	return true;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::ResetPoint()//復歸點參數
{
	m_PointPosXA = m_PointPosYA = m_PointPosZA = 0.f;
	m_PointPosXB = m_PointPosYB = m_PointPosZB = 0.f;
	m_PointPosXNow = m_PointPosYNow = m_PointPosZNow = 0.f;
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::UpdatePointA(int x, int y)//更新點A參數
{
	if ( NULL == m_3DData )	{	return; }
	if ( false == m_IsShowPoint ) { return; }	

	double Height=0.0;
	double PosX=0, PosY=0, PosZ=0;
	MapWndToOpenGL(x, y, PosX, PosY, PosZ);

	const int nX = (int)(PosX)+(m_3DWidth/2);	
	const int nY = (m_3DHeight/2)-(int)(PosY);
	if ( nX<0 || nX>=m_3DWidth ) { return; }
	if ( nY<0 || nY>=m_3DHeight ) { return; }

	const size_t index=(nY*m_3DWidth)+(nX);		
	if ( index < m_Max3DBufferSize )
	{	Height = m_3DData[index];	}

	m_PointPosXA = (float)(PosX);
	m_PointPosYA = (float)(PosY);
	m_PointPosZA = (float)(Height);
	BuildPoint(PosX, PosY, Height, OPENGL_OBJ_POINT_A);
	Invalidate();
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::UpdatePointB(int x, int y)//更新點B參數
{
	if ( NULL == m_3DData )	{	return; }
	if ( false == m_IsShowPoint ) { return; }	

	double Height=0.0;
	double PosX=0, PosY=0, PosZ=0;
	MapWndToOpenGL(x, y, PosX, PosY, PosZ);

	const int nX = (int)(PosX)+(m_3DWidth/2);	
	const int nY = (m_3DHeight/2)-(int)(PosY);
	if ( nX<0 || nX>=m_3DWidth ) { return; }
	if ( nY<0 || nY>=m_3DHeight ) { return; }

	const size_t index=(nY*m_3DWidth)+(nX);		
	if ( index < m_Max3DBufferSize )
	{	Height = m_3DData[index];	}

	m_PointPosXB = (float)(PosX);
	m_PointPosYB = (float)(PosY);
	m_PointPosZB = (float)(Height);
	BuildPoint(PosX, PosY, Height, OPENGL_OBJ_POINT_B);
	Invalidate();
	return;
}
//-------------------------------------------------------------------------------------//
void COpenGLWnd::UpdatePointNow(int x, int y)//更新第1點參數
{
	if ( NULL == m_3DData )	{	return; }
	if ( false == m_IsShowPoint ) { return; }	

	double Height=0.0;
	double PosX=0, PosY=0, PosZ=0;
	MapWndToOpenGL(x, y, PosX, PosY, PosZ);

	const int nX = (int)(PosX)+(m_3DWidth/2);	
	const int nY = (m_3DHeight/2)-(int)(PosY);
	if ( nX<0 || nX>=m_3DWidth ) { return; }
	if ( nY<0 || nY>=m_3DHeight ) { return; }

	const size_t index=(nY*m_3DWidth)+(nX);		
	if ( index < m_Max3DBufferSize )
	{	Height = m_3DData[index];	}

	m_PointPosXNow = (float)(PosX);
	m_PointPosYNow = (float)(PosY);
	m_PointPosZNow = (float)(Height);
	BuildPoint(PosX, PosY, Height, OPENGL_OBJ_POINT_NOW);
	Invalidate();
	return;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::RenderSphere(float x, float y, float z, double r, int nStep)//繪製球體
{
	int i=0, j=0;
	const int uStepNum = nStep;
	const int vStepNum = nStep;
	const float fR = (float)(r);
	const float ustep = 1 / (float)uStepNum;
	const float vstep = 1 / (float)vStepNum;

	float u = 0, v = 0;
	TPOINT3F a, b, c, d;

	//r = 100;
	//繪製下端三角形組
	for (i = 0; i<uStepNum; i++)
	{
		if (OPENGL_OBJECT_LINE == m_MainObjectMode)
		{	glBegin(GL_LINE_LOOP);	}
		else
		{	glBegin(GL_TRIANGLES);	}
		CalcSpherePt(0, 0, a.x, a.y, a.z);
		CalcSpherePt(u, vstep, b.x, b.y, b.z);
		CalcSpherePt(u+ustep, vstep, c.x, c.y, c.z);

		a.x *= fR;	a.y *= fR;	a.z *= fR;
		b.x *= fR;	b.y *= fR;	b.z *= fR;
		c.x *= fR;	c.y *= fR;	c.z *= fR;

		a.x += x;	a.y += y;	a.z += z;
		b.x += x;	b.y += y;	b.z += z;
		c.x += x;	c.y += y;	c.z += z;

		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);		
		glVertex3f(c.x, c.y, c.z);
		u += ustep;
		glEnd();
	}


	//繪製中間四邊形	
	u = 0, v = vstep;
	for (i = 1; i<vStepNum - 1; i++)
	{
		for (j = 0; j<uStepNum; j++)
		{
			if (OPENGL_OBJECT_LINE == m_MainObjectMode)
			{	glBegin(GL_LINE_LOOP);	}
			else
			{	glBegin(GL_QUADS);	}
			CalcSpherePt(u, v, a.x, a.y, a.z);			
			CalcSpherePt(u+ustep, v, b.x, b.y, b.z);
			CalcSpherePt(u+ustep, v+vstep, c.x, c.y, c.z);
			CalcSpherePt(u, v+vstep, d.x, d.y, d.z);
			
			a.x *= fR;	a.y *= fR;	a.z *= fR;
			b.x *= fR;	b.y *= fR;	b.z *= fR;
			c.x *= fR;	c.y *= fR;	c.z *= fR;
			d.x *= fR;	d.y *= fR;	d.z *= fR;

			a.x += x;	a.y += y;	a.z += z;
			b.x += x;	b.y += y;	b.z += z;
			c.x += x;	c.y += y;	c.z += z;
			d.x += x;	d.y += y;	d.z += z;

			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
			glVertex3f(d.x, d.y, d.z);
			u += ustep;
			glEnd();
		}
		v += vstep;
	}
	
	//繪製上端三角形組
	u = 0;	
	for (i = 0; i<uStepNum; i++)
	{
		if ( OPENGL_OBJECT_LINE == m_MainObjectMode )
		{	glBegin(GL_LINE_LOOP);	}
		else
		{	glBegin(GL_TRIANGLES);	}
		CalcSpherePt(0, 1, a.x, a.y, a.z);
		CalcSpherePt(u, 1-vstep, b.x, b.y, b.z);
		CalcSpherePt(u+ustep, 1-vstep, c.x, c.y, c.z);

		a.x *= fR;	a.y *= fR;	a.z *= fR;
		b.x *= fR;	b.y *= fR;	b.z *= fR;
		c.x *= fR;	c.y *= fR;	c.z *= fR;

		a.x += x;	a.y += y;	a.z += z;
		b.x += x;	b.y += y;	b.z += z;
		c.x += x;	c.y += y;	c.z += z;

		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(c.x, c.y, c.z);
		glEnd();

		u += ustep;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool COpenGLWnd::CalcSpherePt(float u, float v, float &x, float &y, float &z)//計算球體上的點
{
	const double PI = PI_RAD;
	const double PI2 = PI * 2;
	x = (float)(sin(PI*v)*cos(PI2*u));
	y = (float)(sin(PI*v)*sin(PI2*u));
	z = (float)(cos(PI*v));
	return true;
}
//-------------------------------------------------------------------------------------//