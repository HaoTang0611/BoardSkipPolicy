// AOIBox.cpp: implementation of the CAOIBox class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIBox.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIBox, CAOIObj)
//IMPLEMENT_DYNAMIC(CAOIBox, CAOIRgn)	
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxShapeUseParam1(BOX_SHAPE_MODE ShapeMode)//確認框外型是否使用參數1
{
	if ( BOX_SHAPE_ROUND_RECT==ShapeMode || BOX_SHAPE_HALF_ROUND_RECT==ShapeMode || BOX_SHAPE_T_SHAPE==ShapeMode )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxShapeUseParam2(BOX_SHAPE_MODE ShapeMode)//確認框外型是否使用參數2
{
	if ( BOX_SHAPE_T_SHAPE == ShapeMode )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
int CAOIBox::CalcRoundRectRectRadius(const RECT &Rect, double Param)//計算矩形圓角半徑
{
	int   d=0, r=0;
	const int w = Rect.right-Rect.left;
	const int h = Rect.bottom-Rect.top;
	if ( Param > 100 ) { Param = 100.0; }
	if ( Param < 0 ) { Param = 0.0; }
	d = MIN(w, h);
	d = d*Param/100;
	r = d/2;
	if ( r < 0 ) { r = 1; }
	return r;
}
//-----------------------------------------------------------------------------//
int CAOIBox::CalcHalfRoundRectRectRadius(const RECT &Rect, double Param, BOX_TOWARD Toward)//計算矩形半圓角半徑
{
	int   d=0, r=0;
	const int w = Rect.right-Rect.left;
	const int h = Rect.bottom-Rect.top;	
	if ( Param > 100 ) { Param = 100.0; }
	if ( Param < 0 ) { Param = 0.0; }
	d = MIN(w, h);
	d = d*Param/100;
	r = d;
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		r = MIN(r, w/2);
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		r = MIN(r, h/2);
		break;
	default:
		r = r/2;
		break;
	}
	if ( r < 0 ) { r = 1; }	
	return r;
}
//-----------------------------------------------------------------------------//
int CAOIBox::CalcBoxTowardAngle(BOX_TOWARD TowardFrom, BOX_TOWARD TowardTo)//取得兩個朝向的角度
{
	int Angle = 0;
	switch ( TowardFrom )
	{
	case BOX_TOWARD_UP:
		switch ( TowardTo )
		{		
		case BOX_TOWARD_LEFT:
			Angle = 90;
			break;
		case BOX_TOWARD_DOWN:
			Angle = 180;
			break;
		case BOX_TOWARD_RIGHT:
			Angle = 270;
			break;
		}
		break;
	case BOX_TOWARD_LEFT:
		switch ( TowardTo )
		{
		case BOX_TOWARD_UP:
			Angle = 270;
			break;		
		case BOX_TOWARD_DOWN:
			Angle = 90;
			break;
		case BOX_TOWARD_RIGHT:
			Angle = 180;
			break;
		}
		break;
	case BOX_TOWARD_DOWN:
		switch ( TowardTo )
		{
		case BOX_TOWARD_UP:
			Angle = 180;
			break;
		case BOX_TOWARD_LEFT:
			Angle = 270;
			break;		
		case BOX_TOWARD_RIGHT:
			Angle = 90;
			break;
		}
		break;
	case BOX_TOWARD_RIGHT:
		switch ( TowardTo )
		{
		case BOX_TOWARD_UP:
			Angle = 90;
			break;
		case BOX_TOWARD_LEFT:
			Angle = 180;
			break;
		case BOX_TOWARD_DOWN:
			Angle = 270;
			break;		
		}
		break;
	}
	return Angle;
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawRectangleRect(HDC hDC, const RECT &Rect, HPEN hPen, HBRUSH hBrush)//繪製矩形	
{
	RECT Rect2=Rect;	
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	::InflateRect(&Rect2, -1, -1);
	::Rectangle(hDC, Rect2.left, Rect2.top, Rect2.right, Rect2.bottom);
	::SelectObject(hDC, hOldBrush);	
	::SelectObject(hDC, hOldPen);	
	return;
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawRoundRectRect(HDC hDC, const RECT &Rect, double Param, HPEN hPen, HBRUSH hBrush)//繪製矩形	
{	
	RECT Rect2=Rect;
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	::InflateRect(&Rect2, -1, -1);
	const int R = CAOIBox::CalcRoundRectRectRadius(Rect2, Param);
	const int D = R*2;
	::RoundRect(hDC, Rect2.left, Rect2.top, Rect2.right, Rect2.bottom, D, D);
	::SelectObject(hDC, hOldBrush);	
	::SelectObject(hDC, hOldPen);	
	return;
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawHalfRoundRectRect(HDC hDC, const RECT &Rect, double Param, BOX_TOWARD Toward, HPEN hPen, HBRUSH hBrush)//繪製半圓矩形
{
	RECT Rect2=Rect;
	const int W = (Rect.right-Rect.left);
	const int H = (Rect.bottom-Rect.top);
	const int HalfW=W/2;
	const int HalfH=H/2;
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	::InflateRect(&Rect2, -1, -1);	
	const int R = CAOIBox::CalcHalfRoundRectRectRadius(Rect2, Param, Toward);
	const int D = 2*R;
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		::Pie(hDC, Rect2.left, Rect2.top, Rect2.left+D, Rect2.top+D, Rect2.left+D, Rect2.top+R, Rect2.left, Rect2.top+R);
		if ( R < HalfW )
		{	
			::Pie(hDC, Rect2.right-D, Rect2.top, Rect2.right, Rect2.top+D, Rect2.right, Rect2.top+R, Rect2.right-D, Rect2.top+R);	
			::Rectangle(hDC, Rect2.left+R, Rect2.top, Rect2.right-R, Rect2.bottom);
		}		
		if ( R < H )
		{	::Rectangle(hDC, Rect2.left, Rect2.top+R-1, Rect2.right, Rect2.bottom);	}		
		break;
	case BOX_TOWARD_LEFT:
		::Pie(hDC, Rect2.left, Rect2.top, Rect2.left+D, Rect2.top+D, Rect2.left+R, Rect2.top, Rect2.left+R, Rect2.top+D);
		if ( R < HalfH )
		{
			::Pie(hDC, Rect2.left, Rect2.bottom-D, Rect2.left+D, Rect2.bottom, Rect2.left+R, Rect2.bottom-D, Rect2.left+R, Rect2.bottom);
			::Rectangle(hDC, Rect2.left, Rect2.top+R, Rect2.right, Rect2.bottom-R);
		}
		if ( R < W )
		{	::Rectangle(hDC, Rect2.left+R-1, Rect2.top, Rect2.right, Rect2.bottom);	}	
		break;
	case BOX_TOWARD_DOWN:
		::Pie(hDC, Rect2.left, Rect2.bottom-D, Rect2.left+D, Rect2.bottom, Rect2.left, Rect2.bottom-R, Rect2.left+D, Rect2.bottom-R);		
		if ( R < HalfW )
		{	
			::Pie(hDC, Rect2.right-D, Rect2.bottom-D, Rect2.right, Rect2.bottom, Rect2.right-D, Rect2.bottom-R, Rect2.right, Rect2.bottom-R);	
			::Rectangle(hDC, Rect2.left+R, Rect2.top, Rect2.right-R, Rect2.bottom);
		}		
		if ( R < H )
		{	::Rectangle(hDC, Rect2.left, Rect2.top, Rect2.right, Rect2.bottom-R+1);	}		
		
		break;
	case BOX_TOWARD_RIGHT:
		::Pie(hDC, Rect2.right-D, Rect2.top, Rect2.right, Rect2.top+D, Rect2.right-R, Rect2.top+D, Rect2.right-R, Rect2.top);
		if ( R < HalfH )
		{
			::Pie(hDC, Rect2.right-D, Rect2.bottom-D, Rect2.right, Rect2.bottom, Rect2.right-R, Rect2.bottom, Rect2.right-R, Rect2.bottom-D);
			::Rectangle(hDC, Rect2.left, Rect2.top+R, Rect2.right, Rect2.bottom-R);
		}
		if ( R < W )
		{	::Rectangle(hDC, Rect2.left, Rect2.top, Rect2.right-R+1, Rect2.bottom);	}	
		break;
	default:
		::RoundRect(hDC, Rect2.left, Rect2.top, Rect2.right, Rect2.bottom, D, D);
		break;
	}
	::SelectObject(hDC, hOldBrush);	
	::SelectObject(hDC, hOldPen);	
	return;
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawEllipseRect(HDC hDC, const RECT &Rect, HPEN hPen, HBRUSH hBrush)//繪製橢圓形
{	
	RECT Rect2=Rect;	
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	::InflateRect(&Rect2, -1, -1);
	::Pie(hDC, Rect2.left, Rect2.top, Rect2.right, Rect2.bottom, Rect2.left, Rect2.top, Rect2.left, Rect2.top);
	::SelectObject(hDC, hOldBrush);	
	::SelectObject(hDC, hOldPen);	
	return;
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawCapsuleRect(HDC hDC, const RECT &Rect, HPEN hPen, HBRUSH hBrush)//繪製膠囊型
{
	RECT Rect2=Rect;
	::InflateRect(&Rect2, -1, -1);
	int width = Rect2.right-Rect2.left;
	int height = Rect2.bottom-Rect2.top;	
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	if ( width < height )//上下鵝卵形
	{
		int CircleTy = Rect2.top+(width/2);
		int CircleBy = Rect2.bottom-(width/2);
		::Pie(hDC, Rect2.left, Rect2.top, Rect2.right, Rect2.top+width, Rect2.right, CircleTy, Rect2.left, CircleTy);		
		::Pie(hDC, Rect2.left, Rect2.bottom-width, Rect2.right, Rect2.bottom, Rect2.left, CircleBy, Rect2.right, CircleBy);

		::Rectangle(hDC, Rect2.left, CircleTy-1, Rect2.right, CircleBy+1);
		//::MoveToEx(hDC, Rect.left, CircleTy, NULL);
		//::LineTo(hDC, Rect.left, CircleBy);
		//::MoveToEx(hDC, Rect.right, CircleBy, NULL);
		//::LineTo(hDC, Rect.right, CircleTy);
	}
	else//左右鵝卵形
	{
		int CircleLx = Rect2.left+(height/2);
		int CircleRx = Rect2.right-(height/2);
		::Pie(hDC, Rect2.left, Rect2.top, Rect2.left+height, Rect2.bottom, CircleLx, Rect2.top, CircleLx, Rect2.bottom);				
		::Pie(hDC, Rect2.right, Rect2.top, Rect2.right-height, Rect2.bottom, CircleRx, Rect2.bottom, CircleRx, Rect2.top);

		::Rectangle(hDC, CircleLx-1, Rect2.top, CircleRx+1, Rect2.bottom);
		//::MoveToEx(hDC, CircleLx, Rect.bottom, NULL);
		//::LineTo(hDC, CircleRx, Rect.bottom);
		//::MoveToEx(hDC, CircleRx, Rect.top, NULL);
		//::LineTo(hDC, CircleLx, Rect.top);
	}
	::SelectObject(hDC, hOldBrush);	
	::SelectObject(hDC, hOldPen);	
	return ;
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawBulletRect(HDC hDC, const RECT &Rect, BOX_TOWARD Toward, HPEN hPen, HBRUSH hBrush)//繪製子彈形
{
	RECT Rect2=Rect;
	::InflateRect(&Rect2, -1, -1);
	const int width = Rect2.right-Rect2.left;
	const int height = Rect2.bottom-Rect2.top;

	int CircleTy = 0;
	int CircleBy = 0;
	int CircleLx = 0;
	int CircleRx = 0;
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);	
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		CircleTy = Rect2.top+(width/2);		
		if ( CircleTy < Rect2.bottom )
		{	
			::Pie(hDC, Rect2.left, Rect2.top, Rect2.right, Rect2.top+width, Rect2.right, CircleTy, Rect2.left, CircleTy); 
			::Rectangle(hDC, Rect2.left, CircleTy, Rect2.right, Rect2.bottom);		
		}
		else
		{	::Chord(hDC, Rect2.left, Rect2.top, Rect2.right, Rect2.top+width, Rect2.right, Rect2.bottom, Rect2.left, Rect2.bottom);  }		
		break;
	case BOX_TOWARD_LEFT:
		CircleLx = Rect2.left+(height/2);		
		if ( CircleLx < Rect2.right )
		{	
			::Pie(hDC, Rect2.left, Rect2.top, Rect2.left+height, Rect2.bottom, CircleLx, Rect2.top, CircleLx, Rect2.bottom); 
			::Rectangle(hDC, CircleLx, Rect2.top, Rect2.right, Rect2.bottom);		
		}
		else
		{	::Chord(hDC, Rect2.left, Rect2.top, Rect2.left+height, Rect2.bottom, Rect2.right , Rect2.top, Rect2.right , Rect2.bottom); }		
		break;
	case BOX_TOWARD_DOWN:		
		CircleBy = Rect2.bottom-(width/2);		
		if ( CircleBy > Rect2.top )
		{	
			::Pie(hDC, Rect2.left, Rect2.bottom-width, Rect2.right, Rect2.bottom, Rect2.left, CircleBy, Rect2.right, CircleBy); 
			::Rectangle(hDC, Rect2.left, Rect2.top, Rect2.right, CircleBy);
		}
		else
		{	::Chord(hDC, Rect2.left, Rect2.bottom-width, Rect2.right, Rect2.bottom, Rect2.left, Rect2.top, Rect2.right, Rect2.top); }		
		break;
	case BOX_TOWARD_RIGHT:		
		CircleRx = Rect2.right-(height/2);	
		if ( CircleRx > Rect2.left )
		{	
			::Pie(hDC, Rect2.right, Rect2.top, Rect2.right-height, Rect2.bottom, CircleRx, Rect2.bottom, CircleRx, Rect2.top); 
			::Rectangle(hDC, Rect2.left, Rect2.top, CircleRx, Rect2.bottom);
		}
		else
		{	::Chord(hDC, Rect2.right, Rect2.top, Rect2.right-height, Rect2.bottom, Rect2.left, Rect2.bottom, Rect2.left, Rect2.top); }				
		break;
	}	
	::SelectObject(hDC, hOldBrush);	
	::SelectObject(hDC, hOldPen);	
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawTShapeRect(HDC hDC, const RECT &Rect, double Param, double Param2, BOX_TOWARD Toward, HPEN hPen, HBRUSH hBrush)//繪製T形
{	
	int RectW2_S=0;
	int RectH2_S=0;	
	RECT RectB=Rect;//Bigger
	RECT RectS=Rect;//Smaller
	const int RectW_B=RectB.right-RectB.left;
	const int RectH_B=RectB.bottom-RectB.top;
	const int RectW2_B=RectW_B/2;
	const int RectH2_B=RectH_B/2;
	Param = 2*Param;
	if ( Param < 0 ) { Param = 0; }
	else if ( Param > 100 ) { Param = 100; }
	if ( Param2 < 0 ) { Param2 = 0; }
	else if ( Param2 > 100 ) { Param2 = 100; }
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		RectW2_S=(int)(RectW2_B*Param/100.0);
		RectH2_S=(int)(RectH2_B*Param2/100.0);	
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		RectW2_S=(int)(RectW2_B*Param2/100.0);
		RectH2_S=(int)(RectH2_B*Param/100.0);	
		break;
	default:
		break;
	}
	const int RectW_S=RectW2_S*2;
	const int RectH_S=RectH2_S*2;
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);	
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		RectS.left  += RectW2_S;
		RectS.right -= RectW2_S;
		RectS.top    = RectB.bottom-RectH_S;
		RectB.bottom = RectS.top;
		break;
	case BOX_TOWARD_LEFT:
		RectS.top    += RectH2_S;
		RectS.bottom -= RectH2_S;
		RectS.left   = RectB.right-RectW_S;
		RectB.right  = RectS.left;
		break;
	case BOX_TOWARD_DOWN:	
		RectS.left  += RectW2_S;
		RectS.right -= RectW2_S;
		RectS.bottom = RectB.top+RectH_S;
		RectB.top    = RectS.bottom;		
		break;
	case BOX_TOWARD_RIGHT:
		RectS.top    += RectH2_S;
		RectS.bottom -= RectH2_S;
		RectS.right   = RectB.left+RectW_S;
		RectB.left    = RectS.right;
		break;
	default:
		break;
	}	
	::FillRect(hDC, &RectS, hBrush);
	::FillRect(hDC, &RectB, hBrush);
	::SelectObject(hDC, hOldBrush);	
	::SelectObject(hDC, hOldPen);	
	return;
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawRectangleLine(HDC hDC, const RECT &Rect, BOX_TOWARD Toward, int TowardSize)//繪製矩形	
{
	::MoveToEx(hDC, Rect.left, Rect.top, NULL);
	::LineTo(hDC, Rect.right, Rect.top);
	::LineTo(hDC, Rect.right, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.top);
	
	if ( BOX_TOWARD_NULL != Toward )
	{	CAOIBox::DrawTowardFeature(hDC, Rect, Toward, TowardSize);	}
}
//-------------------------------------------------------------------------------------//
void CAOIBox::DrawRoundRectLine(HDC hDC, const RECT &Rect, double Param, BOX_TOWARD Toward, int TowardSize)//繪製矩形
{
	RECT RectR={0};
	int Cx=0, Cy=0;
	const int R = CAOIBox::CalcRoundRectRectRadius(Rect, Param);

	//左側至下圓角
	::MoveToEx(hDC, Rect.left, Rect.top+R, NULL);
	::LineTo(hDC, Rect.left, Rect.bottom-R);
	Cx = Rect.left+R;
	Cy = Rect.bottom-R;
	RectR.left = Cx-R;	RectR.top = Cy-R;
	RectR.right = Cx+R;	RectR.bottom = Cy+R;
	::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, RectR.left, Cy, Cx, RectR.bottom);
	
	//下側至右圓角
	::MoveToEx(hDC, Rect.left+R, Rect.bottom, NULL);
	::LineTo(hDC, Rect.right-R, Rect.bottom);
	Cx = Rect.right-R;
	Cy = Rect.bottom-R;
	RectR.left = Cx-R;	RectR.top = Cy-R;
	RectR.right = Cx+R;	RectR.bottom = Cy+R;
	::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, Cx, RectR.bottom, RectR.right, Cy);

	//右側至上圓角
	::MoveToEx(hDC, Rect.right, Rect.bottom-R, NULL);
	::LineTo(hDC, Rect.right, Rect.top+R);
	Cx = Rect.right-R;
	Cy = Rect.top+R;
	RectR.left = Cx-R;	RectR.top = Cy-R;
	RectR.right = Cx+R;	RectR.bottom = Cy+R;
	::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, RectR.right, Cy, Cx, RectR.top);

	//上側至左圓角
	::MoveToEx(hDC, Rect.right-R, Rect.top, NULL);
	::LineTo(hDC, Rect.left+R, Rect.top);
	Cx = Rect.left+R;
	Cy = Rect.top+R;
	RectR.left = Cx-R;	RectR.top = Cy-R;
	RectR.right = Cx+R;	RectR.bottom = Cy+R;
	::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, Cx, RectR.top, RectR.left, Cy);

	if ( BOX_TOWARD_NULL != Toward )
	{	CAOIBox::DrawTowardFeature(hDC, Rect, Toward, TowardSize);	}
}
//-------------------------------------------------------------------------------------//
void CAOIBox::DrawHalfRoundRectLine(HDC hDC, const RECT &Rect, double Param, BOX_TOWARD Toward, int TowardSize)//繪製矩形
{
	RECT RectR={0};
	int Cx=0, Cy=0;
	const int R = CAOIBox::CalcHalfRoundRectRectRadius(Rect, Param, Toward);

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		//左側至下圓角
		::MoveToEx(hDC, Rect.left, Rect.top+R, NULL);
		::LineTo(hDC, Rect.left, Rect.bottom);
		::LineTo(hDC, Rect.right, Rect.bottom);		
		::LineTo(hDC, Rect.right, Rect.top+R);

		//右側至上圓角		
		Cx = Rect.right-R;
		Cy = Rect.top+R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, RectR.right, Cy, Cx, RectR.top);

		//上側至左圓角
		::MoveToEx(hDC, Rect.right-R, Rect.top, NULL);
		::LineTo(hDC, Rect.left+R, Rect.top);
		Cx = Rect.left+R;
		Cy = Rect.top+R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, Cx, RectR.top, RectR.left, Cy);
		break;
	case BOX_TOWARD_LEFT:
		//左側至下圓角
		::MoveToEx(hDC, Rect.left, Rect.top+R, NULL);
		::LineTo(hDC, Rect.left, Rect.bottom-R);
		Cx = Rect.left+R;
		Cy = Rect.bottom-R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, RectR.left, Cy, Cx, RectR.bottom);
	
		//下側至右圓角
		::MoveToEx(hDC, Rect.left+R, Rect.bottom, NULL);
		::LineTo(hDC, Rect.right, Rect.bottom);				
		::LineTo(hDC, Rect.right, Rect.top);
		::LineTo(hDC, Rect.left+R, Rect.top);

		Cx = Rect.left+R;
		Cy = Rect.top+R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, Cx, RectR.top, RectR.left, Cy);
		break;
	case BOX_TOWARD_DOWN:
		//左側至下圓角
		::MoveToEx(hDC, Rect.left, Rect.top, NULL);
		::LineTo(hDC, Rect.left, Rect.bottom-R);
		Cx = Rect.left+R;
		Cy = Rect.bottom-R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, RectR.left, Cy, Cx, RectR.bottom);
	
		//下側至右圓角
		::MoveToEx(hDC, Rect.left+R, Rect.bottom, NULL);
		::LineTo(hDC, Rect.right-R, Rect.bottom);
		Cx = Rect.right-R;
		Cy = Rect.bottom-R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, Cx, RectR.bottom, RectR.right, Cy);

		//右側至上圓角
		::MoveToEx(hDC, Rect.right, Rect.bottom-R, NULL);
		::LineTo(hDC, Rect.right, Rect.top);		
		::LineTo(hDC, Rect.left, Rect.top);		
		break;
	case BOX_TOWARD_RIGHT:		
		//左側至下圓角
		::MoveToEx(hDC, Rect.left, Rect.top, NULL);
		::LineTo(hDC, Rect.left, Rect.bottom);		
		::LineTo(hDC, Rect.right-R, Rect.bottom);

		//下側至右圓角
		Cx = Rect.right-R;
		Cy = Rect.bottom-R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, Cx, RectR.bottom, RectR.right, Cy);

		//右側至上圓角
		::MoveToEx(hDC, Rect.right, Rect.bottom-R, NULL);
		::LineTo(hDC, Rect.right, Rect.top+R);
		Cx = Rect.right-R;
		Cy = Rect.top+R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, RectR.right, Cy, Cx, RectR.top);

		//上側至左圓角
		::MoveToEx(hDC, Rect.right-R, Rect.top, NULL);
		::LineTo(hDC, Rect.left, Rect.top);		
		break;
	default:		
		//左側至下圓角
		::MoveToEx(hDC, Rect.left, Rect.top+R, NULL);
		::LineTo(hDC, Rect.left, Rect.bottom-R);
		Cx = Rect.left+R;
		Cy = Rect.bottom-R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, RectR.left, Cy, Cx, RectR.bottom);
	
		//下側至右圓角
		::MoveToEx(hDC, Rect.left+R, Rect.bottom, NULL);
		::LineTo(hDC, Rect.right-R, Rect.bottom);
		Cx = Rect.right-R;
		Cy = Rect.bottom-R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, Cx, RectR.bottom, RectR.right, Cy);

		//右側至上圓角
		::MoveToEx(hDC, Rect.right, Rect.bottom-R, NULL);
		::LineTo(hDC, Rect.right, Rect.top+R);
		Cx = Rect.right-R;
		Cy = Rect.top+R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, RectR.right, Cy, Cx, RectR.top);

		//上側至左圓角
		::MoveToEx(hDC, Rect.right-R, Rect.top, NULL);
		::LineTo(hDC, Rect.left+R, Rect.top);
		Cx = Rect.left+R;
		Cy = Rect.top+R;
		RectR.left = Cx-R;	RectR.top = Cy-R;
		RectR.right = Cx+R;	RectR.bottom = Cy+R;
		::Arc(hDC, RectR.left, RectR.top, RectR.right, RectR.bottom, Cx, RectR.top, RectR.left, Cy);
		break;
	}
	if ( BOX_TOWARD_NULL != Toward )
	{	CAOIBox::DrawTowardFeature(hDC, Rect, Toward, TowardSize);	}
}
//-------------------------------------------------------------------------------------//
void CAOIBox::DrawEllipseLine(HDC hDC, const RECT &Rect, BOX_TOWARD Toward, int TowardSize)//繪製橢圓形
{
	::Arc(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom, Rect.left, Rect.top, Rect.left, Rect.top);
	if ( BOX_TOWARD_NULL==Toward || 0==TowardSize ) { return; }	
	CAOIBox::DrawTowardFeature(hDC, Rect, Toward, TowardSize);
}
//----------------------------------------------------------------------------//
void CAOIBox::DrawCapsuleLine(HDC hDC, const RECT &Rect, BOX_TOWARD Toward, int TowardSize)//繪製膠囊型
{
	int width = Rect.right-Rect.left;
	int height = Rect.bottom-Rect.top;
	if ( width < height )//上下鵝卵形
	{
		int CircleTy = Rect.top+(width/2);
		int CircleBy = Rect.bottom-(width/2);
		::Arc(hDC, Rect.left, Rect.top, Rect.right, Rect.top+width, Rect.right, CircleTy, Rect.left, CircleTy);
		::MoveToEx(hDC, Rect.left, CircleTy, NULL);
		::LineTo(hDC, Rect.left, CircleBy);
		::Arc(hDC, Rect.left, Rect.bottom-width, Rect.right, Rect.bottom, Rect.left, CircleBy, Rect.right, CircleBy);
		::MoveToEx(hDC, Rect.right, CircleBy, NULL);
		::LineTo(hDC, Rect.right, CircleTy);
	}
	else//左右鵝卵形
	{
		int CircleLx = Rect.left+(height/2);
		int CircleRx = Rect.right-(height/2);
		::Arc(hDC, Rect.left, Rect.top, Rect.left+height, Rect.bottom, CircleLx, Rect.top, CircleLx, Rect.bottom);		
		::MoveToEx(hDC, CircleLx, Rect.bottom, NULL);
		::LineTo(hDC, CircleRx, Rect.bottom);
		::Arc(hDC, Rect.right, Rect.top, Rect.right-height, Rect.bottom, CircleRx, Rect.bottom, CircleRx, Rect.top);
		::MoveToEx(hDC, CircleRx, Rect.top, NULL);
		::LineTo(hDC, CircleLx, Rect.top);
	}

//	::Arc(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom, Rect.left, Rect.top, Rect.left, Rect.top);
	if ( BOX_TOWARD_NULL==Toward || 0==TowardSize ) { return; }	
	CAOIBox::DrawTowardFeature(hDC, Rect, Toward, TowardSize);
}
//----------------------------------------------------------------------------//
void CAOIBox::DrawBulletLine(HDC hDC, const RECT &Rect, BOX_TOWARD Toward, int TowardSize)//繪製子彈形
{
	const int width = Rect.right-Rect.left;
	const int height = Rect.bottom-Rect.top;

	//int Radius = 0;
	//int Diameter = 0;
	int CircleTy = 0;
	int CircleBy = 0;
	int CircleLx = 0;
	int CircleRx = 0;	

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		CircleTy = Rect.top+(width/2);		
		if ( CircleTy < Rect.bottom )
		{	
			::Arc(hDC, Rect.left, Rect.top, Rect.right, Rect.top+width, Rect.right, CircleTy, Rect.left, CircleTy); 
			::MoveToEx(hDC, Rect.left, CircleTy, NULL);
			::LineTo(hDC, Rect.left, Rect.bottom);
			::LineTo(hDC, Rect.right, Rect.bottom);
			::LineTo(hDC, Rect.right, CircleTy);
		}
		else
		{	
			::Arc(hDC, Rect.left, Rect.top, Rect.right, Rect.top+width, Rect.right, Rect.bottom, Rect.left, Rect.bottom); 
			::MoveToEx(hDC, Rect.left, Rect.bottom, NULL);
			::LineTo(hDC, Rect.right, Rect.bottom);
		}
		
		break;
	case BOX_TOWARD_LEFT:
		CircleLx = Rect.left+(height/2);		
		if ( CircleLx < Rect.right )
		{	
			::Arc(hDC, Rect.left, Rect.top, Rect.left+height, Rect.bottom, CircleLx, Rect.top, CircleLx, Rect.bottom); 
			::MoveToEx(hDC, CircleLx, Rect.bottom, NULL);
			::LineTo(hDC, Rect.right, Rect.bottom);		
			::LineTo(hDC, Rect.right, Rect.top);		
			::LineTo(hDC, CircleLx, Rect.top);
		}
		else
		{	
			::Arc(hDC, Rect.left, Rect.top, Rect.left+height, Rect.bottom, Rect.right , Rect.top, Rect.right , Rect.bottom); 
			::MoveToEx(hDC, Rect.right, Rect.top, NULL);
			::LineTo(hDC, Rect.right, Rect.bottom);
		}
		
		break;
	case BOX_TOWARD_DOWN:		
		CircleBy = Rect.bottom-(width/2);		
		if ( CircleBy > Rect.top )
		{	
			::Arc(hDC, Rect.left, Rect.bottom-width, Rect.right, Rect.bottom, Rect.left, CircleBy, Rect.right, CircleBy); 
			::MoveToEx(hDC, Rect.right, CircleBy, NULL);
			::LineTo(hDC, Rect.right, Rect.top);
			::LineTo(hDC, Rect.left, Rect.top);
			::LineTo(hDC, Rect.left, CircleBy);
		}
		else
		{	
			::Arc(hDC, Rect.left, Rect.bottom-width, Rect.right, Rect.bottom, Rect.left, Rect.top, Rect.right, Rect.top); 
			::MoveToEx(hDC, Rect.left, Rect.top, NULL);
			::LineTo(hDC, Rect.right, Rect.top);
		}		
		break;
	case BOX_TOWARD_RIGHT:		
		CircleRx = Rect.right-(height/2);	
		if ( CircleRx > Rect.left )
		{	
			::Arc(hDC, Rect.right, Rect.top, Rect.right-height, Rect.bottom, CircleRx, Rect.bottom, CircleRx, Rect.top); 
			::MoveToEx(hDC, CircleRx, Rect.top, NULL);
			::LineTo(hDC, Rect.left, Rect.top);
			::LineTo(hDC, Rect.left, Rect.bottom);
			::LineTo(hDC, CircleRx, Rect.bottom);
		}
		else
		{			
			::Arc(hDC, Rect.right, Rect.top, Rect.right-height, Rect.bottom, Rect.left, Rect.bottom, Rect.left, Rect.top); 			
			::MoveToEx(hDC, Rect.left, Rect.top, NULL);
			::LineTo(hDC, Rect.left, Rect.bottom);
		}		
		break;
	}
//	::Arc(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom, Rect.left, Rect.top, Rect.left, Rect.top);
	if ( BOX_TOWARD_NULL==Toward || 0==TowardSize ) { return; }	
	CAOIBox::DrawTowardFeature(hDC, Rect, Toward, TowardSize);
}
//----------------------------------------------------------------------------//
void CAOIBox::DrawTShapeRectLine(HDC hDC, const RECT &Rect, double Param, double Param2, BOX_TOWARD Toward, int TowardSize)//繪製半圓矩形
{	
	int RectW2_S=0;
	int RectH2_S=0;
	RECT Rect1={0};	
	RECT Rect2={0};	
	RECT RectB=Rect;//Bigger
	RECT RectS=Rect;//Smaller
	const int RectW_B=RectB.right-RectB.left;
	const int RectH_B=RectB.bottom-RectB.top;
	const int RectW2_B=RectW_B/2;
	const int RectH2_B=RectH_B/2;
	Param = 2*Param;
	if ( Param < 0 ) { Param = 0; }
	else if ( Param > 100 ) { Param = 100; }
	if ( Param2 < 0 ) { Param2 = 0; }
	else if ( Param2 > 100 ) { Param2 = 100; }
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		RectW2_S=(int)(RectW2_B*Param/100.0);
		RectH2_S=(int)(RectH2_B*Param2/100.0);	
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		RectW2_S=(int)(RectW2_B*Param2/100.0);
		RectH2_S=(int)(RectH2_B*Param/100.0);	
		break;
	default:
		break;
	}
	const int RectW_S=RectW2_S*2;
	const int RectH_S=RectH2_S*2;
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		RectS.left  += RectW2_S;
		RectS.right -= RectW2_S;
		RectS.top    = RectB.bottom-RectH_S;
		RectB.bottom = RectS.top;

		Rect1 = RectB;//Top
		Rect2 = RectS;//Bottom
		::MoveToEx(hDC, Rect1.left, Rect1.top, NULL);
		::LineTo(hDC, Rect1.left, Rect2.top);
		::LineTo(hDC, Rect2.left, Rect2.top);
		::LineTo(hDC, Rect2.left, Rect2.bottom);
		::LineTo(hDC, Rect2.right, Rect2.bottom);
		::LineTo(hDC, Rect2.right, Rect2.top);
		::LineTo(hDC, Rect1.right, Rect2.top);
		::LineTo(hDC, Rect1.right, Rect1.top);
		::LineTo(hDC, Rect1.left, Rect1.top);
		break;
	case BOX_TOWARD_LEFT:
		RectS.top    += RectH2_S;
		RectS.bottom -= RectH2_S;
		RectS.left   = RectB.right-RectW_S;
		RectB.right  = RectS.left;

		Rect1 = RectB;//Left
		Rect2 = RectS;//Right
		::MoveToEx(hDC, Rect1.left, Rect1.top, NULL);
		::LineTo(hDC, Rect1.left, Rect1.bottom);
		::LineTo(hDC, Rect1.right, Rect1.bottom);
		::LineTo(hDC, Rect1.right, Rect2.bottom);
		::LineTo(hDC, Rect2.right, Rect2.bottom);
		::LineTo(hDC, Rect2.right, Rect2.top);
		::LineTo(hDC, Rect2.left, Rect2.top);
		::LineTo(hDC, Rect2.left, Rect1.top);
		::LineTo(hDC, Rect1.left, Rect1.top);
		break;
	case BOX_TOWARD_DOWN:	
		RectS.left  += RectW2_S;
		RectS.right -= RectW2_S;
		RectS.bottom = RectB.top+RectH_S;
		RectB.top    = RectS.bottom;

		Rect1 = RectS;//Top
		Rect2 = RectB;//Bottom
		::MoveToEx(hDC, Rect1.left, Rect1.top, NULL);
		::LineTo(hDC, Rect1.left, Rect2.top);
		::LineTo(hDC, Rect2.left, Rect2.top);
		::LineTo(hDC, Rect2.left, Rect2.bottom);
		::LineTo(hDC, Rect2.right, Rect2.bottom);
		::LineTo(hDC, Rect2.right, Rect2.top);
		::LineTo(hDC, Rect1.right, Rect2.top);
		::LineTo(hDC, Rect1.right, Rect1.top);
		::LineTo(hDC, Rect1.left, Rect1.top);
		break;
	case BOX_TOWARD_RIGHT:
		RectS.top    += RectH2_S;
		RectS.bottom -= RectH2_S;
		RectS.right   = RectB.left+RectW_S;
		RectB.left    = RectS.right;

		Rect1 = RectS;//Left
		Rect2 = RectB;//Right
		::MoveToEx(hDC, Rect1.left, Rect1.top, NULL);
		::LineTo(hDC, Rect1.left, Rect1.bottom);
		::LineTo(hDC, Rect1.right, Rect1.bottom);
		::LineTo(hDC, Rect1.right, Rect2.bottom);
		::LineTo(hDC, Rect2.right, Rect2.bottom);
		::LineTo(hDC, Rect2.right, Rect2.top);
		::LineTo(hDC, Rect2.left, Rect2.top);
		::LineTo(hDC, Rect2.left, Rect1.top);
		::LineTo(hDC, Rect1.left, Rect1.top);
		break;
	}	

	if ( BOX_TOWARD_NULL != Toward )
	{	CAOIBox::DrawTowardFeature(hDC, Rect, Toward, TowardSize);	}
}
//----------------------------------------------------------------------------//
void CAOIBox::DrawTowardFeature(HDC hDC, const RECT &Rect, BOX_TOWARD Toward, int Size)//繪製朝向特徵
{
	if ( 0 == Size ) { return; }
	const int Margin = 8;
	int Offset = Size;
	POINT Pts[3];	
	switch ( Toward )
	{
	case BOX_TOWARD_LEFT://Left
		if ( Offset > ((Rect.bottom-Rect.top-Margin)/2) )
		{
			Offset = (Rect.bottom-Rect.top-Margin)-1;
			Offset = Offset/2;
			if ( Offset < 0 ) { return; }
		}
		Pts[0].x = Rect.left-Offset;
		Pts[0].y = (int)((Rect.top+Rect.bottom)/2);

		Pts[1].x = Pts[2].x = Rect.left;			
		Pts[1].y = Pts[0].y + Offset;
		Pts[2].y = Pts[0].y - Offset;

		CAOIBox::DrawTriangleLine(hDC, Pts);
		break;
	case BOX_TOWARD_RIGHT://Right
		if ( Offset > ((Rect.bottom-Rect.top-Margin)/2) )
		{
			Offset = (Rect.bottom-Rect.top-Margin)-1;
			Offset = Offset/2;
			if ( Offset < 0 ) { return; }
		}
		Pts[0].x = Rect.right+Offset;
		Pts[0].y = (int)((Rect.top+Rect.bottom)/2);

		Pts[1].x = Pts[2].x = Rect.right;			
		Pts[1].y = Pts[0].y + Offset;
		Pts[2].y = Pts[0].y - Offset;

		CAOIBox::DrawTriangleLine(hDC, Pts);

		break;
	case BOX_TOWARD_UP://Up
		if ( Offset > ((Rect.right-Rect.left-Margin)/2) )
		{
			Offset = (Rect.right-Rect.left-Margin)-1;
			Offset = Offset/2;
			if ( Offset < 0 ) { return; }
		}
		Pts[0].y = Rect.top-Offset;
		Pts[0].x = (int)((Rect.left+Rect.right)/2);

		Pts[1].y = Pts[2].y = Rect.top;			
		Pts[1].x = Pts[0].x + Offset;
		Pts[2].x = Pts[0].x - Offset;

		CAOIBox::DrawTriangleLine(hDC, Pts);

		break;
	case BOX_TOWARD_DOWN://Down
		if ( Offset > ((Rect.right-Rect.left-Margin)/2) )
		{
			Offset = (Rect.right-Rect.left-Margin)-1;
			Offset = Offset/2;
			if ( Offset < 0 ) { return; }
		}

		Pts[0].y = Rect.bottom+Offset;
		Pts[0].x = (int)((Rect.left+Rect.right)/2);

		Pts[1].y = Pts[2].y = Rect.bottom;			
		Pts[1].x = Pts[0].x + Offset;
		Pts[2].x = Pts[0].x - Offset;

		CAOIBox::DrawTriangleLine(hDC, Pts);

		break;		
	}	
}
//----------------------------------------------------------------------------//
void CAOIBox::DrawTriangleLine(HDC hDC, const POINT PTList[])//繪製三角形
{
	::MoveToEx(hDC, PTList[0].x, PTList[0].y, NULL);
	::LineTo(hDC, PTList[1].x, PTList[1].y);
	::LineTo(hDC, PTList[2].x, PTList[2].y);
	::LineTo(hDC, PTList[0].x, PTList[0].y);
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawEditRect(HDC hDC, const RECT &EditRect, int HalfSize)//繪製選取編輯框	
{
	::MoveToEx(hDC, EditRect.left, EditRect.top, NULL);
	::LineTo(hDC, EditRect.right, EditRect.top);
	::LineTo(hDC, EditRect.right, EditRect.bottom);
	::LineTo(hDC, EditRect.left, EditRect.bottom);
	::LineTo(hDC, EditRect.left, EditRect.top);

	if ( HalfSize <= 0 ) { return; }
	
	POINT CP, Pt;
	HPEN  hPen = NULL;
	HPEN  hOldPen = NULL;
	bool  LineMode = false;	
	CP.x = (EditRect.left+EditRect.right)/2;
	CP.y = (EditRect.top+EditRect.bottom)/2;	
	
	if ( false == LineMode )
	{
		HalfSize += 1;
		//HalfSize = HalfSize*2;
		hPen = ::CreatePen(PS_NULL, 1, 0xFFFFFF);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
	}
	//Left Top
	Pt.x = EditRect.left;
	Pt.y = EditRect.top;
	//::MoveToEx(hDC, Pt.x, Pt.y, NULL);
	//::AngleArc(hDC, Pt.x, Pt.y, HalfSize, 0.0f, 360.0f);
	
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }

	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);		
	/*
	::MoveToEx(hDC, Pt.x-HalfSize, Pt.y-HalfSize, NULL);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y-HalfSize);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y-HalfSize);
	*/

	//Top
	Pt.x = CP.x;
	Pt.y = EditRect.top;
	//::MoveToEx(hDC, Pt.x, Pt.y, NULL);
	//::AngleArc(hDC, Pt.x, Pt.y, HalfSize, 0.0f, 360.0f);
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);	
	/*
	::MoveToEx(hDC, Pt.x-HalfSize, Pt.y-HalfSize, NULL);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y-HalfSize);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y-HalfSize);	
	*/

	//Right Top
	Pt.x = EditRect.right;
	Pt.y = EditRect.top;
	//::MoveToEx(hDC, Pt.x, Pt.y, NULL);
	//::AngleArc(hDC, Pt.x, Pt.y, HalfSize, 0.0f, 360.0f);
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);	
	/*
	::MoveToEx(hDC, Pt.x-HalfSize, Pt.y-HalfSize, NULL);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y-HalfSize);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y-HalfSize);
	*/

	//Left
	Pt.x = EditRect.left;	
	Pt.y = CP.y;
	//::MoveToEx(hDC, Pt.x, Pt.y, NULL);
	//::AngleArc(hDC, Pt.x, Pt.y, HalfSize, 0.0f, 360.0f);
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);	
	/*
	::MoveToEx(hDC, Pt.x-HalfSize, Pt.y-HalfSize, NULL);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y-HalfSize);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y-HalfSize);	
	*/

	//Right
	Pt.x = EditRect.right;	
	Pt.y = CP.y;
	//::MoveToEx(hDC, Pt.x, Pt.y, NULL);
	//::AngleArc(hDC, Pt.x, Pt.y, HalfSize, 0.0f, 360.0f);
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);	
	/*
	::MoveToEx(hDC, Pt.x-HalfSize, Pt.y-HalfSize, NULL);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y-HalfSize);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y-HalfSize);
	*/
	
	//Left Bottom
	Pt.x = EditRect.left;
	Pt.y = EditRect.bottom;
	//::MoveToEx(hDC, Pt.x, Pt.y, NULL);
	//::AngleArc(hDC, Pt.x, Pt.y, HalfSize, 0.0f, 360.0f);
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);	
	/*
	::MoveToEx(hDC, Pt.x-HalfSize, Pt.y-HalfSize, NULL);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y-HalfSize);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y-HalfSize);
	*/

	//Bottom
	Pt.x = CP.x;
	Pt.y = EditRect.bottom;
	//::MoveToEx(hDC, Pt.x, Pt.y, NULL);
	//::AngleArc(hDC, Pt.x, Pt.y, HalfSize, 0.0f, 360.0f);
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);	
	/*
	::MoveToEx(hDC, Pt.x-HalfSize, Pt.y-HalfSize, NULL);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y-HalfSize);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y-HalfSize);	
	*/

	//Right Bottom
	Pt.x = EditRect.right;
	Pt.y = EditRect.bottom;
	//::MoveToEx(hDC, Pt.x, Pt.y, NULL);
	//::AngleArc(hDC, Pt.x, Pt.y, HalfSize, 0.0f, 360.0f);
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);	
	/*
	::MoveToEx(hDC, Pt.x-HalfSize, Pt.y-HalfSize, NULL);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y-HalfSize);
	::LineTo(hDC, Pt.x+HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y+HalfSize);
	::LineTo(hDC, Pt.x-HalfSize, Pt.y-HalfSize);
	*/

	if ( false == LineMode )
	{
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen); hPen = NULL;		
	}
}
//-----------------------------------------------------------------------------//
void CAOIBox::DrawEditRect(HDC hDC, const POINT CornPoint[], int HalfSize)//繪製選取編輯框	
{
	::MoveToEx(hDC, CornPoint[0].x, CornPoint[0].y, NULL);
	::LineTo(hDC, CornPoint[1].x, CornPoint[1].y);
	::LineTo(hDC, CornPoint[2].x, CornPoint[2].y);
	::LineTo(hDC, CornPoint[3].x, CornPoint[3].y);
	::LineTo(hDC, CornPoint[0].x, CornPoint[0].y);
	if ( HalfSize <= 0 ) { return; }	
	
	POINT CP, Pt;
	HPEN  hPen = NULL;
	HPEN  hOldPen = NULL;
	bool  LineMode = false;
	CP.x = (CornPoint[0].x+CornPoint[1].x+CornPoint[2].x+CornPoint[3].x)/2;
	CP.y = (CornPoint[0].y+CornPoint[1].y+CornPoint[2].y+CornPoint[3].y)/2;	
	
	if ( false == LineMode )
	{
		//HalfSize = HalfSize+1;
		HalfSize = HalfSize*2;
		hPen = ::CreatePen(PS_NULL, 1, 0xFFFFFF);
		hOldPen = (HPEN)::SelectObject(hDC, hPen);		
	}
	//Left Top
	Pt.x = CornPoint[0].x;
	Pt.y = CornPoint[0].y;

	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);	

	//Top
	Pt.x = (CornPoint[0].x+CornPoint[1].x)/2;
	Pt.y = (CornPoint[0].y+CornPoint[1].y)/2;
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);

	//Right Top
	Pt.x = CornPoint[1].x;
	Pt.y = CornPoint[1].y;
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);

	//Left
	Pt.x = (CornPoint[1].x+CornPoint[2].x)/2;	
	Pt.y = (CornPoint[1].y+CornPoint[2].y)/2;
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);

	//Right
	Pt.x = CornPoint[2].x;	
	Pt.y = CornPoint[2].y;
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);
	
	//Left Bottom
	Pt.x = (CornPoint[2].x+CornPoint[3].x)/2;
	Pt.y = (CornPoint[2].y+CornPoint[3].y)/2;
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);

	//Bottom
	Pt.x = CornPoint[3].x;
	Pt.y = CornPoint[3].y;
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);

	//Right Bottom
	Pt.x = (CornPoint[3].x+CornPoint[0].x)/2;
	Pt.y = (CornPoint[3].y+CornPoint[0].y)/2;
	if ( false == LineMode )
	{	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	else
	{	::Arc(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize); }
	//::Ellipse(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize);

	if ( false == LineMode )
	{
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen); hPen = NULL;		
	}
}
//-----------------------------------------------------------------------------//
CAOIBox::CAOIBox():CAOIObj(AOI_OBJ_BOX)
{
	PreInitBox();
	InitialBox();
}
//-------------------------------------------------------------------------------------//
CAOIBox::CAOIBox(const CAOIBox &Box):CAOIObj(Box)
{
	PreInitBox();
	CloneBox(Box);
}
//-------------------------------------------------------------------------------------//
CAOIBox::~CAOIBox()
{

}
//-------------------------------------------------------------------------------------//
CAOIBox& CAOIBox::operator=(const CAOIBox &Box)
{
	if ( &Box == this ) { return *this; }
	CAOIObj::operator=(Box);
	CloneBox(Box);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOIBox::PreInitBox()
{	
}
//-------------------------------------------------------------------------------------//
inline void CAOIBox::InitialBox()
{
	m_BoxShapeParam = 30.0;
	m_BoxShapeParam2 = 50.0;
	m_BoxMaskEraseMode = false;
	m_BoxToward = BOX_TOWARD_RIGHT;//框朝向	
	m_BoxShapeMode = BOX_SHAPE_RECTANGLE;//框形狀

	m_BoxIndex = -1;
	m_BoxEnabled = true;//是否啟用
	m_BoxActived = false;//是否焦點中
	m_BoxSelected = false;//是否選取中
	m_BoxVisibled = true;//是否顯示中
	m_BoxEditabled = true;//是否可編輯

	m_BoxAngle = 0;
	m_BoxAngleRes = 0;
	m_BoxAngleSkew = 0;

	m_BoxAttachedAngle = 0;
	m_BoxAttachedPosCad = TPOINT2D();	
	m_BoxAttachedPosStage = TPOINT2D();	
	
	m_BoxSize = TSIZE2D();
	m_BoxSizeRes = TSIZE2D();	

	m_BoxPos = TPOINT2D();	
	m_BoxPosRes = TPOINT2D();	
	m_BoxCornerPos[0] = TPOINT2D();	
	m_BoxCornerPos[1] = TPOINT2D();	
	m_BoxCornerPos[2] = TPOINT2D();	
	m_BoxCornerPos[3] = TPOINT2D();	
	m_BoxCornerPosRes[0] = TPOINT2D();	
	m_BoxCornerPosRes[1] = TPOINT2D();	
	m_BoxCornerPosRes[2] = TPOINT2D();	
	m_BoxCornerPosRes[3] = TPOINT2D();	
	
	m_BoxPosCad = TPOINT2D();	
	m_BoxPosCadRes = TPOINT2D();	
	m_BoxCornerPosCad[0] = TPOINT2D();	
	m_BoxCornerPosCad[1] = TPOINT2D();	
	m_BoxCornerPosCad[2] = TPOINT2D();	
	m_BoxCornerPosCad[3] = TPOINT2D();	
	m_BoxCornerPosCadRes[0] = TPOINT2D();	
	m_BoxCornerPosCadRes[1] = TPOINT2D();	
	m_BoxCornerPosCadRes[2] = TPOINT2D();	
	m_BoxCornerPosCadRes[3] = TPOINT2D();	

	m_BoxPosStage = TPOINT2D();	
	m_BoxPosStageRes = TPOINT2D();	
	m_BoxCornerPosStage[0] = TPOINT2D();	
	m_BoxCornerPosStage[1] = TPOINT2D();	
	m_BoxCornerPosStage[2] = TPOINT2D();	
	m_BoxCornerPosStage[3] = TPOINT2D();	
	m_BoxCornerPosStageRes[0] = TPOINT2D();	
	m_BoxCornerPosStageRes[1] = TPOINT2D();	
	m_BoxCornerPosStageRes[2] = TPOINT2D();	
	m_BoxCornerPosStageRes[3] = TPOINT2D();

	m_BoxResultID = RESULT_ID_NONE;
	m_BoxResultText = _T("");
	m_BoxResultValue = 0.0;
	m_BoxResultTextVisibled = false;

	m_BoxPolygon.clear();
	m_BoxPolygonVisibled = false;	
}
//-------------------------------------------------------------------------------------//
inline void CAOIBox::CloneBox(const CAOIBox &Box)
{	
	m_BoxToward = Box.m_BoxToward;//框朝向
	m_BoxShapeMode = Box.m_BoxShapeMode;//框形狀
	m_BoxShapeParam = Box.m_BoxShapeParam;
	m_BoxShapeParam2 = Box.m_BoxShapeParam2;
	m_BoxMaskEraseMode = Box.m_BoxMaskEraseMode;

	m_BoxIndex = Box.m_BoxIndex;//佇列引數
	m_BoxEnabled = Box.m_BoxEnabled;//是否啟用
	m_BoxActived = Box.m_BoxActived;//是否焦點中
	m_BoxSelected = Box.m_BoxSelected;//是否選取中
	m_BoxVisibled = Box.m_BoxVisibled;//是否顯示中
	m_BoxEditabled = Box.m_BoxEditabled;//是否可編輯

	m_BoxAngle = Box.m_BoxAngle;	
	m_BoxAngleRes = Box.m_BoxAngleRes;
	m_BoxAngleSkew = Box.m_BoxAngleSkew;

	m_BoxAttachedAngle = Box.m_BoxAttachedAngle;
	m_BoxAttachedPosCad = Box.m_BoxAttachedPosCad;
	m_BoxAttachedPosStage = Box.m_BoxAttachedPosStage;	

	m_BoxSize = Box.m_BoxSize;
	m_BoxSizeRes = Box.m_BoxSizeRes;	

	m_BoxPos = Box.m_BoxPos;	
	m_BoxPosRes = Box.m_BoxPosRes;	
	m_BoxCornerPos[0] = Box.m_BoxCornerPos[0];	
	m_BoxCornerPos[1] = Box.m_BoxCornerPos[1];	
	m_BoxCornerPos[2] = Box.m_BoxCornerPos[2];	
	m_BoxCornerPos[3] = Box.m_BoxCornerPos[3];	
	m_BoxCornerPosRes[0] = Box.m_BoxCornerPosRes[0];	
	m_BoxCornerPosRes[1] = Box.m_BoxCornerPosRes[1];	
	m_BoxCornerPosRes[2] = Box.m_BoxCornerPosRes[2];	
	m_BoxCornerPosRes[3] = Box.m_BoxCornerPosRes[3];	
	
	m_BoxPosCad = Box.m_BoxPosCad;	
	m_BoxPosCadRes = Box.m_BoxPosCadRes;	
	m_BoxCornerPosCad[0] = Box.m_BoxCornerPosCad[0];	
	m_BoxCornerPosCad[1] = Box.m_BoxCornerPosCad[1];	
	m_BoxCornerPosCad[2] = Box.m_BoxCornerPosCad[2];	
	m_BoxCornerPosCad[3] = Box.m_BoxCornerPosCad[3];	
	m_BoxCornerPosCadRes[0] = Box.m_BoxCornerPosCadRes[0];	
	m_BoxCornerPosCadRes[1] = Box.m_BoxCornerPosCadRes[1];	
	m_BoxCornerPosCadRes[2] = Box.m_BoxCornerPosCadRes[2];	
	m_BoxCornerPosCadRes[3] = Box.m_BoxCornerPosCadRes[3];	

	m_BoxPosStage = Box.m_BoxPosStage;	
	m_BoxPosStageRes = Box.m_BoxPosStageRes;	
	m_BoxCornerPosStage[0] = Box.m_BoxCornerPosStage[0];	
	m_BoxCornerPosStage[1] = Box.m_BoxCornerPosStage[1];	
	m_BoxCornerPosStage[2] = Box.m_BoxCornerPosStage[2];	
	m_BoxCornerPosStage[3] = Box.m_BoxCornerPosStage[3];	
	m_BoxCornerPosStageRes[0] = Box.m_BoxCornerPosStageRes[0];	
	m_BoxCornerPosStageRes[1] = Box.m_BoxCornerPosStageRes[1];	
	m_BoxCornerPosStageRes[2] = Box.m_BoxCornerPosStageRes[2];	
	m_BoxCornerPosStageRes[3] = Box.m_BoxCornerPosStageRes[3];	

	m_BoxResultID = Box.m_BoxResultID;	;
	m_BoxResultText = Box.m_BoxResultText;	
	m_BoxResultValue = Box.m_BoxResultValue;	
	m_BoxResultTextVisibled = Box.m_BoxResultTextVisibled;

	m_BoxPolygon = Box.m_BoxPolygon;
	m_BoxPolygonVisibled = Box.m_BoxPolygonVisibled;
}
//-------------------------------------------------------------------------------------//
CAOIBox* CAOIBox::CloneBoxObj() const//建立且複製一個框
{
	CAOIBox *ObjPtr = AOIObjManager.CreateBoxObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);	
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::WriteBoxFile(CAOIFileIO &FileIO)//儲存框檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	CAOIBox *BoxPtr = this;	
	char     uuidStr[MAX_JET_PATH]="";	
	wchar_t  uuidWStr[MAX_JET_PATH]=L"";	
	UUID     uuid = BoxPtr->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIBox::WriteBoxFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	if ( FileIO.SaveChunk_INT(FILE_IO_BOX_START, 0) == false ) { return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_BOX_TOWARD, BoxPtr->GetBoxToward()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BOX_SHAPE_MODE, BoxPtr->GetBoxShapeMode()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_BOX_POS_X, BoxPtr->GetBoxPosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BOX_POS_Y, BoxPtr->GetBoxPosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BOX_SIZE_X, BoxPtr->GetBoxSizeX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BOX_SIZE_Y, BoxPtr->GetBoxSizeY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BOX_ANGLE, BoxPtr->GetBoxAngle()) == false ) { return false; }	
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_BOX_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_BOX_OBJ_UUID, uuidStr) == false ) { return false; } }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BOX_SHAPE_PARAM_1, BoxPtr->GetBoxShapeParam()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_BOX_SHAPE_PARAM_2, BoxPtr->GetBoxShapeParam2()) == false ) { return false; }	
	
	if ( FileIO.SaveChunk_INT(FILE_IO_BOX_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::ReadBoxFile(CAOIFileIO &FileIO)//載入框檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int      index = 0;
	double   dValue = 0.0;
	UUID     uuid;
	unsigned char *unStr=NULL;
	CAOIBox *BoxPtr = this;	
	FileIO.SetFnName(_T("CAOIBox::ReadBoxFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_BOX_START://基本框參數-起點
			break;
		case FILE_IO_BOX_END://基本框參數-終點
			BoxPtr->LayoutBoxCornerPos(true);
			return true;
			break;
		case FILE_IO_BOX_TOWARD://基本框參數-框朝向
			BoxPtr->SetBoxToward((BOX_TOWARD)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BOX_SHAPE_MODE://基本框參數-框形狀
			BoxPtr->SetBoxShapeMode((BOX_SHAPE_MODE)(FileIO.GetData_INT()));
			break;		
		case FILE_IO_BOX_POS_X://基本框參數-框位置-X
			dValue = FileIO.GetData_DBL();
			BoxPtr->SetBoxPosX(dValue);
			BoxPtr->SetBoxPosResX(dValue);
			break;
		case FILE_IO_BOX_POS_Y://基本框參數-框位置-Y
			dValue = FileIO.GetData_DBL();
			BoxPtr->SetBoxPosY(dValue);
			BoxPtr->SetBoxPosResY(dValue);
			break;
		case FILE_IO_BOX_SIZE_X://基本框參數-框尺寸-X
			dValue = FileIO.GetData_DBL();
			BoxPtr->SetBoxSizeX(dValue);
			BoxPtr->SetBoxSizeResX(dValue);
			break;
		case FILE_IO_BOX_SIZE_Y://基本框參數-框尺寸-Y
			dValue = FileIO.GetData_DBL();
			BoxPtr->SetBoxSizeY(dValue);
			BoxPtr->SetBoxSizeResY(dValue);
			break;
		case FILE_IO_BOX_ANGLE://基本框參數-框角度
			BoxPtr->SetBoxAngle(FileIO.GetData_DBL());
			break;
		case FILE_IO_BOX_OBJ_UUID://基本框參數-UUID
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	BoxPtr->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	BoxPtr->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_BOX_SHAPE_PARAM_1://基本框參數-框形狀參數-1
			BoxPtr->SetBoxShapeParam(FileIO.GetData_DBL());
			break;
		case FILE_IO_BOX_SHAPE_PARAM_2://基本框參數-框形狀參數-2
			BoxPtr->SetBoxShapeParam2(FileIO.GetData_DBL());
			break;
		default:
			break;
		}
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxPos(const TPOINT2D &Pos, bool bIncludeRes)
{
	CAOIBox::m_BoxPos = Pos;
	CAOIBox::UpdateBoxCornerPos();
	if ( true == bIncludeRes )
	{
		CAOIBox::m_BoxPosRes = CAOIBox::m_BoxPos;
		CAOIBox::UpdateBoxCornerPosRes();
	}
	CAOIBox::UpdateBoxAttachedPosCad(bIncludeRes);
	CAOIBox::UpdateBoxAttachedPosStage(bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxPos(double px, double py, bool bIncludeRes)
{
	CAOIBox::m_BoxPos.x = px;
	CAOIBox::m_BoxPos.y = py;
	CAOIBox::UpdateBoxCornerPos();
	if ( true == bIncludeRes )
	{
		CAOIBox::m_BoxPosRes = CAOIBox::m_BoxPos;
		CAOIBox::UpdateBoxCornerPosRes();
	}
	CAOIBox::UpdateBoxAttachedPosCad(bIncludeRes);
	CAOIBox::UpdateBoxAttachedPosStage(bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxPosRes(const TPOINT2D &Pos)
{
	CAOIBox::m_BoxPosRes = Pos;
	CAOIBox::UpdateBoxCornerPosRes();
	CAOIBox::UpdateBoxAttachedPosCadRes();
	CAOIBox::UpdateBoxAttachedPosStageRes();	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxPosRes(double px, double py)
{
	CAOIBox::m_BoxPosRes.x = px;
	CAOIBox::m_BoxPosRes.y = py;
	CAOIBox::UpdateBoxCornerPosRes();
	CAOIBox::UpdateBoxAttachedPosCadRes();
	CAOIBox::UpdateBoxAttachedPosStageRes();	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxSize(const TSIZE2D &Sz, bool bIncludeRes)
{
	CAOIBox::m_BoxSize = Sz;
	CAOIBox::UpdateBoxCornerPos();
	if ( true == bIncludeRes )
	{
		CAOIBox::m_BoxSizeRes = m_BoxSize;
		CAOIBox::UpdateBoxCornerPosRes();
	}
	CAOIBox::UpdateBoxAttachedPosCad(bIncludeRes);
	CAOIBox::UpdateBoxAttachedPosStage(bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxSize(double cx, double cy, bool bIncludeRes)
{
	CAOIBox::m_BoxSize.cx = cx;
	CAOIBox::m_BoxSize.cy = cy;
	CAOIBox::UpdateBoxCornerPos();
	if ( true == bIncludeRes )
	{
		CAOIBox::m_BoxSizeRes = m_BoxSize;
		CAOIBox::UpdateBoxCornerPosRes();
	}
	CAOIBox::UpdateBoxAttachedPosCad(bIncludeRes);
	CAOIBox::UpdateBoxAttachedPosStage(bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
inline void CAOIBox::UpdateBoxCornerPos()//更新框的角落座標
{
	const double SizeW2 = m_BoxSize.cx*0.5;
	const double SizeH2 = m_BoxSize.cy*0.5;
	CAOIBox::m_BoxCornerPos[0].x = m_BoxPos.x-(SizeW2);
	CAOIBox::m_BoxCornerPos[0].y = m_BoxPos.y-(SizeH2);
	CAOIBox::m_BoxCornerPos[1].x = m_BoxPos.x+(SizeW2);
	CAOIBox::m_BoxCornerPos[1].y = m_BoxPos.y-(SizeH2);
	CAOIBox::m_BoxCornerPos[2].x = m_BoxPos.x+(SizeW2);
	CAOIBox::m_BoxCornerPos[2].y = m_BoxPos.y+(SizeH2);
	CAOIBox::m_BoxCornerPos[3].x = m_BoxPos.x-(SizeW2);
	CAOIBox::m_BoxCornerPos[3].y = m_BoxPos.y+(SizeH2);
}
//-------------------------------------------------------------------------------------//
inline void CAOIBox::UpdateBoxCornerPosRes()//更新框的角落座標-結果
{
	const double SizeW2 = m_BoxSizeRes.cx*0.5;
	const double SizeH2 = m_BoxSizeRes.cy*0.5;
	CAOIBox::m_BoxCornerPosRes[0].x = m_BoxPosRes.x-(SizeW2);
	CAOIBox::m_BoxCornerPosRes[0].y = m_BoxPosRes.y-(SizeH2);
	CAOIBox::m_BoxCornerPosRes[1].x = m_BoxPosRes.x+(SizeW2);
	CAOIBox::m_BoxCornerPosRes[1].y = m_BoxPosRes.y-(SizeH2);
	CAOIBox::m_BoxCornerPosRes[2].x = m_BoxPosRes.x+(SizeW2);
	CAOIBox::m_BoxCornerPosRes[2].y = m_BoxPosRes.y+(SizeH2);
	CAOIBox::m_BoxCornerPosRes[3].x = m_BoxPosRes.x-(SizeW2);
	CAOIBox::m_BoxCornerPosRes[3].y = m_BoxPosRes.y+(SizeH2);
}
//-------------------------------------------------------------------------------------//
inline void CAOIBox::UpdateBoxAttachedPosCadRes()//更新框的零件座標-CAD
{
	const double dx = m_BoxAttachedPosCad.x;
	const double dy = m_BoxAttachedPosCad.y;	

	CAOIBox::m_BoxPosCadRes.x = CAOIBox::m_BoxPosRes.x + dx;
	CAOIBox::m_BoxPosCadRes.y = CAOIBox::m_BoxPosRes.y + dy;
	CAOIBox::m_BoxCornerPosCadRes[0].x = CAOIBox::m_BoxCornerPosRes[0].x + dx;
	CAOIBox::m_BoxCornerPosCadRes[0].y = CAOIBox::m_BoxCornerPosRes[0].y + dy;
	CAOIBox::m_BoxCornerPosCadRes[1].x = CAOIBox::m_BoxCornerPosRes[1].x + dx;
	CAOIBox::m_BoxCornerPosCadRes[1].y = CAOIBox::m_BoxCornerPosRes[1].y + dy;
	CAOIBox::m_BoxCornerPosCadRes[2].x = CAOIBox::m_BoxCornerPosRes[2].x + dx;
	CAOIBox::m_BoxCornerPosCadRes[2].y = CAOIBox::m_BoxCornerPosRes[2].y + dy;
	CAOIBox::m_BoxCornerPosCadRes[3].x = CAOIBox::m_BoxCornerPosRes[3].x + dx;
	CAOIBox::m_BoxCornerPosCadRes[3].y = CAOIBox::m_BoxCornerPosRes[3].y + dy;
	return;
}
//-------------------------------------------------------------------------------------//
inline void CAOIBox::UpdateBoxAttachedPosCad(bool bIncludeRes)//更新框的座標-CAD
{
	const double dx = m_BoxAttachedPosCad.x;
	const double dy = m_BoxAttachedPosCad.y;	

	CAOIBox::m_BoxPosCad.x = CAOIBox::m_BoxPos.x + dx;
	CAOIBox::m_BoxPosCad.y = CAOIBox::m_BoxPos.y + dy;
	CAOIBox::m_BoxCornerPosCad[0].x = CAOIBox::m_BoxCornerPos[0].x + dx;
	CAOIBox::m_BoxCornerPosCad[0].y = CAOIBox::m_BoxCornerPos[0].y + dy;
	CAOIBox::m_BoxCornerPosCad[1].x = CAOIBox::m_BoxCornerPos[1].x + dx;
	CAOIBox::m_BoxCornerPosCad[1].y = CAOIBox::m_BoxCornerPos[1].y + dy;
	CAOIBox::m_BoxCornerPosCad[2].x = CAOIBox::m_BoxCornerPos[2].x + dx;
	CAOIBox::m_BoxCornerPosCad[2].y = CAOIBox::m_BoxCornerPos[2].y + dy;
	CAOIBox::m_BoxCornerPosCad[3].x = CAOIBox::m_BoxCornerPos[3].x + dx;
	CAOIBox::m_BoxCornerPosCad[3].y = CAOIBox::m_BoxCornerPos[3].y + dy;
	
	if ( true == bIncludeRes )
	{	UpdateBoxAttachedPosCadRes(); }
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::UpdateBoxAttachedPosStageRes()//更新框的零件座標-機台
{
	//注意機台座標與CAD座標的極性
	const double dx = m_BoxAttachedPosStage.x;
	const double dy = m_BoxAttachedPosStage.y;		
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();	
	if ( false == SignX ) 
	{		
		CAOIBox::m_BoxPosStageRes.x = dx - CAOIBox::m_BoxPosRes.x;
		CAOIBox::m_BoxCornerPosStageRes[0].x = dx - CAOIBox::m_BoxCornerPosRes[0].x;
		CAOIBox::m_BoxCornerPosStageRes[1].x = dx - CAOIBox::m_BoxCornerPosRes[1].x;
		CAOIBox::m_BoxCornerPosStageRes[2].x = dx - CAOIBox::m_BoxCornerPosRes[2].x;
		CAOIBox::m_BoxCornerPosStageRes[3].x = dx - CAOIBox::m_BoxCornerPosRes[3].x;
	}		
	else
	{		
		CAOIBox::m_BoxPosStageRes.x = CAOIBox::m_BoxPosRes.x + dx;
		CAOIBox::m_BoxCornerPosStageRes[0].x = CAOIBox::m_BoxCornerPosRes[0].x + dx;
		CAOIBox::m_BoxCornerPosStageRes[1].x = CAOIBox::m_BoxCornerPosRes[1].x + dx;
		CAOIBox::m_BoxCornerPosStageRes[2].x = CAOIBox::m_BoxCornerPosRes[2].x + dx;
		CAOIBox::m_BoxCornerPosStageRes[3].x = CAOIBox::m_BoxCornerPosRes[3].x + dx;		
	}
	
	if ( false == SignY ) 	
	{	
		CAOIBox::m_BoxPosStageRes.y = dy - CAOIBox::m_BoxPosRes.y;
		CAOIBox::m_BoxCornerPosStageRes[0].y = dy - CAOIBox::m_BoxCornerPosRes[0].y;
		CAOIBox::m_BoxCornerPosStageRes[1].y = dy - CAOIBox::m_BoxCornerPosRes[1].y;
		CAOIBox::m_BoxCornerPosStageRes[2].y = dy - CAOIBox::m_BoxCornerPosRes[2].y;
		CAOIBox::m_BoxCornerPosStageRes[3].y = dy - CAOIBox::m_BoxCornerPosRes[3].y;
	}
	else
	{	
		CAOIBox::m_BoxPosStageRes.y = CAOIBox::m_BoxPosRes.y + dy;
		CAOIBox::m_BoxCornerPosStageRes[0].y = CAOIBox::m_BoxCornerPosRes[0].y + dy;
		CAOIBox::m_BoxCornerPosStageRes[1].y = CAOIBox::m_BoxCornerPosRes[1].y + dy;
		CAOIBox::m_BoxCornerPosStageRes[2].y = CAOIBox::m_BoxCornerPosRes[2].y + dy;
		CAOIBox::m_BoxCornerPosStageRes[3].y = CAOIBox::m_BoxCornerPosRes[3].y + dy;
	}	
	return;
}
//-------------------------------------------------------------------------------------//
inline void CAOIBox::UpdateBoxAttachedPosStage(bool bIncludeRes)//更新框的座標-機台
{
	//注意機台座標與CAD座標的極性
	const double dx = m_BoxAttachedPosStage.x;
	const double dy = m_BoxAttachedPosStage.y;		
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();	
	if ( false == SignX ) 
	{
		CAOIBox::m_BoxPosStage.x = dx - CAOIBox::m_BoxPos.x;
		CAOIBox::m_BoxCornerPosStage[0].x = dx - CAOIBox::m_BoxCornerPos[0].x;
		CAOIBox::m_BoxCornerPosStage[1].x = dx - CAOIBox::m_BoxCornerPos[1].x;
		CAOIBox::m_BoxCornerPosStage[2].x = dx - CAOIBox::m_BoxCornerPos[2].x;
		CAOIBox::m_BoxCornerPosStage[3].x = dx - CAOIBox::m_BoxCornerPos[3].x;		
	}		
	else
	{
		CAOIBox::m_BoxPosStage.x = CAOIBox::m_BoxPos.x + dx;
		CAOIBox::m_BoxCornerPosStage[0].x = CAOIBox::m_BoxCornerPos[0].x + dx;
		CAOIBox::m_BoxCornerPosStage[1].x = CAOIBox::m_BoxCornerPos[1].x + dx;
		CAOIBox::m_BoxCornerPosStage[2].x = CAOIBox::m_BoxCornerPos[2].x + dx;
		CAOIBox::m_BoxCornerPosStage[3].x = CAOIBox::m_BoxCornerPos[3].x + dx;
	}
	
	if ( false == SignY ) 	
	{		
		CAOIBox::m_BoxPosStage.y = dy - CAOIBox::m_BoxPos.y;
		CAOIBox::m_BoxCornerPosStage[0].y = dy - CAOIBox::m_BoxCornerPos[0].y;
		CAOIBox::m_BoxCornerPosStage[1].y = dy - CAOIBox::m_BoxCornerPos[1].y;
		CAOIBox::m_BoxCornerPosStage[2].y = dy - CAOIBox::m_BoxCornerPos[2].y;
		CAOIBox::m_BoxCornerPosStage[3].y = dy - CAOIBox::m_BoxCornerPos[3].y;	
	}
	else
	{
		CAOIBox::m_BoxPosStage.y = CAOIBox::m_BoxPos.y + dy;
		CAOIBox::m_BoxCornerPosStage[0].y = CAOIBox::m_BoxCornerPos[0].y + dy;
		CAOIBox::m_BoxCornerPosStage[1].y = CAOIBox::m_BoxCornerPos[1].y + dy;
		CAOIBox::m_BoxCornerPosStage[2].y = CAOIBox::m_BoxCornerPos[2].y + dy;
		CAOIBox::m_BoxCornerPosStage[3].y = CAOIBox::m_BoxCornerPos[3].y + dy;
	}	

	if ( true == bIncludeRes )
	{	UpdateBoxAttachedPosStageRes(); }
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxRegion(const TREGION4D &Rgn, bool bIncludeRes)
{
	CAOIBox::m_BoxPos.x = (Rgn.maxX+Rgn.minX)*0.5;
	CAOIBox::m_BoxPos.y = (Rgn.maxY+Rgn.minY)*0.5;
	CAOIBox::m_BoxSize.cx = (Rgn.maxX-Rgn.minX);
	CAOIBox::m_BoxSize.cy = (Rgn.maxY-Rgn.minY);
	CAOIBox::UpdateBoxCornerPos();
	if ( true == bIncludeRes )
	{
		CAOIBox::m_BoxPosRes = CAOIBox::m_BoxPos;
		CAOIBox::m_BoxSizeRes = CAOIBox::m_BoxSize;		
		CAOIBox::UpdateBoxCornerPosRes();
	}
	CAOIBox::UpdateBoxAttachedPosCad(bIncludeRes);
	CAOIBox::UpdateBoxAttachedPosStage(bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxRegion(double MinX, double MinY, double MaxX, double MaxY, bool bIncludeRes)
{
	CAOIBox::m_BoxPos.x = (MaxX+MinX)*0.5;
	CAOIBox::m_BoxPos.y = (MaxY+MinY)*0.5;
	CAOIBox::m_BoxSize.cx = (MaxX-MinX);
	CAOIBox::m_BoxSize.cy = (MaxY-MinY);
	CAOIBox::UpdateBoxCornerPos();
	if ( true == bIncludeRes )
	{
		CAOIBox::m_BoxPosRes = CAOIBox::m_BoxPos;
		CAOIBox::m_BoxSizeRes = CAOIBox::m_BoxSize;		
		CAOIBox::UpdateBoxCornerPosRes();
	}
	CAOIBox::UpdateBoxAttachedPosCad(bIncludeRes);
	CAOIBox::UpdateBoxAttachedPosStage(bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxRegionRes(const TREGION4D &Rgn)
{
	CAOIBox::m_BoxPosRes.x = (Rgn.maxX+Rgn.minX)*0.5;
	CAOIBox::m_BoxPosRes.y = (Rgn.maxY+Rgn.minY)*0.5;
	CAOIBox::m_BoxSizeRes.cx = (Rgn.maxX-Rgn.minX);
	CAOIBox::m_BoxSizeRes.cy = (Rgn.maxY-Rgn.minY);
	CAOIBox::UpdateBoxCornerPosRes();
	CAOIBox::UpdateBoxAttachedPosCadRes();
	CAOIBox::UpdateBoxAttachedPosStageRes();	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxRegionRes(double MinX, double MinY, double MaxX, double MaxY)
{
	CAOIBox::m_BoxPosRes.x = (MaxX+MinX)*0.5;
	CAOIBox::m_BoxPosRes.y = (MaxY+MinY)*0.5;
	CAOIBox::m_BoxSizeRes.cx = (MaxX-MinX);
	CAOIBox::m_BoxSizeRes.cy = (MaxY-MinY);
	CAOIBox::UpdateBoxCornerPosRes();
	CAOIBox::UpdateBoxAttachedPosCadRes();
	CAOIBox::UpdateBoxAttachedPosStageRes();	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxSizeRes(double szX, double szY, bool UpdateCorner)
{
	m_BoxSizeRes.cx = szX; 
	m_BoxSizeRes.cy = szY; 
	if ( true == UpdateCorner )
	{	UpdateBoxCornerPosRes();	}
	UpdateBoxAttachedPosCadRes();
	UpdateBoxAttachedPosStageRes();	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::ResetBoxRegionRes()
{
	m_BoxAngleSkew = 0;
	m_BoxAngleRes = m_BoxAngle;

	m_BoxSizeRes = m_BoxSize;

	m_BoxPosRes = m_BoxPos;	
	m_BoxCornerPosRes[0] = m_BoxCornerPos[0];
	m_BoxCornerPosRes[1] = m_BoxCornerPos[1];
	m_BoxCornerPosRes[2] = m_BoxCornerPos[2];
	m_BoxCornerPosRes[3] = m_BoxCornerPos[3];

	m_BoxPosCadRes = m_BoxPosCad;	
	m_BoxCornerPosCadRes[0] = m_BoxCornerPosCad[0];
	m_BoxCornerPosCadRes[1] = m_BoxCornerPosCad[1];
	m_BoxCornerPosCadRes[2] = m_BoxCornerPosCad[2];
	m_BoxCornerPosCadRes[3] = m_BoxCornerPosCad[3];

	m_BoxPosStageRes = m_BoxPosStage;	
	m_BoxCornerPosStageRes[0] = m_BoxCornerPosStage[0];
	m_BoxCornerPosStageRes[1] = m_BoxCornerPosStage[1];
	m_BoxCornerPosStageRes[2] = m_BoxCornerPosStage[2];
	m_BoxCornerPosStageRes[3] = m_BoxCornerPosStage[3];
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::LayoutBoxCornerPos(bool bIncludeRes)
{
	CAOIBox::UpdateBoxCornerPos();
	if ( true == bIncludeRes )
	{	CAOIBox::UpdateBoxCornerPosRes();	}
}
//-------------------------------------------------------------------------------------//
void CAOIBox::ModifyBoxRegion(const TREGION4D &dRgn, bool bIncludeRes)
{
	double W2=0, H2=0;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;

	W2 = CAOIBox::m_BoxSize.cx*0.5;
	H2 = CAOIBox::m_BoxSize.cy*0.5;
	MinX = CAOIBox::m_BoxPos.x-W2+dRgn.minX;
	MaxX = CAOIBox::m_BoxPos.x+W2+dRgn.maxX;
	MinY = CAOIBox::m_BoxPos.y-H2+dRgn.minY;
	MaxY = CAOIBox::m_BoxPos.y+H2+dRgn.maxY;

	CAOIBox::m_BoxPos.x = (MinX+MaxX)/2.0f;
	CAOIBox::m_BoxPos.y = (MinY+MaxY)/2.0f;
	CAOIBox::m_BoxSize.cx = (MaxX-MinX);
	CAOIBox::m_BoxSize.cy = (MaxY-MinY);
	if ( CAOIBox::m_BoxSize.cx < 0 ) { CAOIBox::m_BoxSize.cx = -CAOIBox::m_BoxSize.cx; }
	if ( CAOIBox::m_BoxSize.cy < 0 ) { CAOIBox::m_BoxSize.cy = -CAOIBox::m_BoxSize.cy; }
	CAOIBox::UpdateBoxCornerPos();

	if ( true == bIncludeRes ) 
	{	
		CAOIBox::m_BoxPosRes = CAOIBox::m_BoxPos;
		CAOIBox::m_BoxSizeRes = CAOIBox::m_BoxSize;	
		CAOIBox::UpdateBoxCornerPosRes();	
	}
	CAOIBox::UpdateBoxAttachedPosCad(bIncludeRes);
	CAOIBox::UpdateBoxAttachedPosStage(bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::ModifyBoxRegion(double dMinX, double dMinY, double dMaxX, double dMaxY, bool bIncludeRes)
{
	double W2=0, H2=0;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;

	W2 = CAOIBox::m_BoxSize.cx*0.5;
	H2 = CAOIBox::m_BoxSize.cy*0.5;
	MinX = CAOIBox::m_BoxPos.x-W2+dMinX;
	MaxX = CAOIBox::m_BoxPos.x+W2+dMaxX;
	MinY = CAOIBox::m_BoxPos.y-H2+dMinY;
	MaxY = CAOIBox::m_BoxPos.y+H2+dMaxY;

	CAOIBox::m_BoxPos.x = (MinX+MaxX)/2.0f;
	CAOIBox::m_BoxPos.y = (MinY+MaxY)/2.0f;
	CAOIBox::m_BoxSize.cx = (MaxX-MinX);
	CAOIBox::m_BoxSize.cy = (MaxY-MinY);
	if ( CAOIBox::m_BoxSize.cx < 0 ) { CAOIBox::m_BoxSize.cx = -CAOIBox::m_BoxSize.cx; }
	if ( CAOIBox::m_BoxSize.cy < 0 ) { CAOIBox::m_BoxSize.cy = -CAOIBox::m_BoxSize.cy; }
	CAOIBox::UpdateBoxCornerPos();	

	if ( true == bIncludeRes ) 
	{	
		CAOIBox::m_BoxPosRes = CAOIBox::m_BoxPos;
		CAOIBox::m_BoxSizeRes = CAOIBox::m_BoxSize;	
		CAOIBox::UpdateBoxCornerPosRes();			
	}	
	CAOIBox::UpdateBoxAttachedPosCad(bIncludeRes);
	CAOIBox::UpdateBoxAttachedPosStage(bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::ModifyBoxRegionRes(const TREGION4D &dRgn)
{
	double W2=0, H2=0;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;

	W2 = CAOIBox::m_BoxSizeRes.cx*0.5;
	H2 = CAOIBox::m_BoxSizeRes.cy*0.5;
	MinX = CAOIBox::m_BoxPosRes.x-W2+dRgn.minX;
	MaxX = CAOIBox::m_BoxPosRes.x+W2+dRgn.maxX;
	MinY = CAOIBox::m_BoxPosRes.y-H2+dRgn.minY;
	MaxY = CAOIBox::m_BoxPosRes.y+H2+dRgn.maxY;

	CAOIBox::m_BoxPosRes.x = (MinX+MaxX)/2.0f;
	CAOIBox::m_BoxPosRes.y = (MinY+MaxY)/2.0f;
	CAOIBox::m_BoxSizeRes.cx = (MaxX-MinX);
	CAOIBox::m_BoxSizeRes.cy = (MaxY-MinY);
	CAOIBox::UpdateBoxCornerPosRes();
	CAOIBox::UpdateBoxAttachedPosCadRes();
	CAOIBox::UpdateBoxAttachedPosStageRes();
}
//-------------------------------------------------------------------------------------//
void CAOIBox::ModifyBoxRegionRes(double dMinX, double dMinY, double dMaxX, double dMaxY)
{
	double W2=0, H2=0;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;

	W2 = CAOIBox::m_BoxSizeRes.cx*0.5;
	H2 = CAOIBox::m_BoxSizeRes.cy*0.5;
	MinX = CAOIBox::m_BoxPosRes.x-W2+dMinX;
	MaxX = CAOIBox::m_BoxPosRes.x+W2+dMaxX;
	MinY = CAOIBox::m_BoxPosRes.y-H2+dMinY;
	MaxY = CAOIBox::m_BoxPosRes.y+H2+dMaxY;

	CAOIBox::m_BoxPosRes.x = (MinX+MaxX)/2.0f;
	CAOIBox::m_BoxPosRes.y = (MinY+MaxY)/2.0f;
	CAOIBox::m_BoxSizeRes.cx = (MaxX-MinX);
	CAOIBox::m_BoxSizeRes.cy = (MaxY-MinY);
	CAOIBox::UpdateBoxCornerPosRes();
	CAOIBox::UpdateBoxAttachedPosCadRes();
	CAOIBox::UpdateBoxAttachedPosStageRes();
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxRegion(TREGION4D &Region) const
{
	Region.minX = CAOIBox::m_BoxPos.x - (CAOIBox::m_BoxSize.cx*0.5);
	Region.minY = CAOIBox::m_BoxPos.y - (CAOIBox::m_BoxSize.cy*0.5);
	Region.maxX = CAOIBox::m_BoxPos.x + (CAOIBox::m_BoxSize.cx*0.5);
	Region.maxY = CAOIBox::m_BoxPos.y + (CAOIBox::m_BoxSize.cy*0.5);
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxRegion(double &MinX, double &MinY, double &MaxX, double &MaxY) const
{
	MinX = CAOIBox::m_BoxPos.x - (CAOIBox::m_BoxSize.cx*0.5);
	MinY = CAOIBox::m_BoxPos.y - (CAOIBox::m_BoxSize.cy*0.5);
	MaxX = CAOIBox::m_BoxPos.x + (CAOIBox::m_BoxSize.cx*0.5);
	MaxY = CAOIBox::m_BoxPos.y + (CAOIBox::m_BoxSize.cy*0.5);
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxRegionRes(TREGION4D &Region) const
{
	Region.minX = CAOIBox::m_BoxPosRes.x - (CAOIBox::m_BoxSizeRes.cx*0.5);
	Region.minY = CAOIBox::m_BoxPosRes.y - (CAOIBox::m_BoxSizeRes.cy*0.5);
	Region.maxX = CAOIBox::m_BoxPosRes.x + (CAOIBox::m_BoxSizeRes.cx*0.5);
	Region.maxY = CAOIBox::m_BoxPosRes.y + (CAOIBox::m_BoxSizeRes.cy*0.5);
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxRegionRes(double &MinX, double &MinY, double &MaxX, double &MaxY) const
{
	MinX = CAOIBox::m_BoxPosRes.x - (CAOIBox::m_BoxSizeRes.cx*0.5);
	MinY = CAOIBox::m_BoxPosRes.y - (CAOIBox::m_BoxSizeRes.cy*0.5);
	MaxX = CAOIBox::m_BoxPosRes.x + (CAOIBox::m_BoxSizeRes.cx*0.5);
	MaxY = CAOIBox::m_BoxPosRes.y + (CAOIBox::m_BoxSizeRes.cy*0.5);
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxCornerPos(TPOINT2D CornerPos[]) const
{
	CornerPos[0] = m_BoxCornerPos[0];
	CornerPos[1] = m_BoxCornerPos[1];
	CornerPos[2] = m_BoxCornerPos[2];
	CornerPos[3] = m_BoxCornerPos[3];
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxCornerPosRes(TPOINT2D CornerPos[]) const
{
	CornerPos[0] = m_BoxCornerPosRes[0];
	CornerPos[1] = m_BoxCornerPosRes[1];
	CornerPos[2] = m_BoxCornerPosRes[2];
	CornerPos[3] = m_BoxCornerPosRes[3];
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxRegionCad(TREGION4D &Region) const
{
	Region.minX = CAOIBox::m_BoxPosCad.x - (CAOIBox::m_BoxSize.cx*0.5);
	Region.minY = CAOIBox::m_BoxPosCad.y - (CAOIBox::m_BoxSize.cy*0.5);
	Region.maxX = CAOIBox::m_BoxPosCad.x + (CAOIBox::m_BoxSize.cx*0.5);
	Region.maxY = CAOIBox::m_BoxPosCad.y + (CAOIBox::m_BoxSize.cy*0.5);
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxRegionCadRes(TREGION4D &Region) const
{
	Region.minX = CAOIBox::m_BoxPosCadRes.x - (CAOIBox::m_BoxSizeRes.cx*0.5);
	Region.minY = CAOIBox::m_BoxPosCadRes.y - (CAOIBox::m_BoxSizeRes.cy*0.5);
	Region.maxX = CAOIBox::m_BoxPosCadRes.x + (CAOIBox::m_BoxSizeRes.cx*0.5);
	Region.maxY = CAOIBox::m_BoxPosCadRes.y + (CAOIBox::m_BoxSizeRes.cy*0.5);
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxCornerPosCad(TPOINT2D CornerPos[]) const
{
	CornerPos[0] = m_BoxCornerPosCad[0];
	CornerPos[1] = m_BoxCornerPosCad[1];
	CornerPos[2] = m_BoxCornerPosCad[2];
	CornerPos[3] = m_BoxCornerPosCad[3];
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxCornerPosCadRes(TPOINT2D CornerPos[]) const
{
	CornerPos[0] = m_BoxCornerPosCadRes[0];
	CornerPos[1] = m_BoxCornerPosCadRes[1];
	CornerPos[2] = m_BoxCornerPosCadRes[2];
	CornerPos[3] = m_BoxCornerPosCadRes[3];
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxRegionStage(TREGION4D &Region) const
{
	Region.minX = CAOIBox::m_BoxPosStage.x - (CAOIBox::m_BoxSize.cx*0.5);
	Region.minY = CAOIBox::m_BoxPosStage.y - (CAOIBox::m_BoxSize.cy*0.5);
	Region.maxX = CAOIBox::m_BoxPosStage.x + (CAOIBox::m_BoxSize.cx*0.5);
	Region.maxY = CAOIBox::m_BoxPosStage.y + (CAOIBox::m_BoxSize.cy*0.5);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxRegionStageRes(TREGION4D &Region) const
{
	Region.minX = CAOIBox::m_BoxPosStageRes.x - (CAOIBox::m_BoxSizeRes.cx*0.5);
	Region.minY = CAOIBox::m_BoxPosStageRes.y - (CAOIBox::m_BoxSizeRes.cy*0.5);
	Region.maxX = CAOIBox::m_BoxPosStageRes.x + (CAOIBox::m_BoxSizeRes.cx*0.5);
	Region.maxY = CAOIBox::m_BoxPosStageRes.y + (CAOIBox::m_BoxSizeRes.cy*0.5);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxCornerPosStage(TPOINT2D CornerPos[]) const
{
	CornerPos[0] = m_BoxCornerPosStage[0];
	CornerPos[1] = m_BoxCornerPosStage[1];
	CornerPos[2] = m_BoxCornerPosStage[2];
	CornerPos[3] = m_BoxCornerPosStage[3];
}
//-------------------------------------------------------------------------------------//
void CAOIBox::GetBoxCornerPosStageRes(TPOINT2D CornerPos[])const
{
	CornerPos[0] = m_BoxCornerPosStageRes[0];
	CornerPos[1] = m_BoxCornerPosStageRes[1];
	CornerPos[2] = m_BoxCornerPosStageRes[2];
	CornerPos[3] = m_BoxCornerPosStageRes[3];
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::RotateBoxPolygon(double Angle, double CPX, double CPY)
{
	if ( fabs(Angle) < 0.00001 ) { return true; }

	TPOINT2D *BoxPt=NULL;	
	const size_t PolygonPtCount=GetBoxPolygonPtCount();
	for ( size_t i=0; i<PolygonPtCount; i++ )
	{
		BoxPt=(TPOINT2D *)GetBoxPolygonPt(i, false);
		if ( NULL == BoxPt ) { continue; }
		JetAPI::RotatePos(Angle, CPX, CPY, *BoxPt);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxBePickByCad(const TPOINT2D &pt, bool IsExceptionAngle) const
{
	const double szX = CAOIBox::m_BoxSize.cx*0.5;
	if ( pt.x < (CAOIBox::m_BoxPosCad.x-szX) ) { return false; }
	if ( pt.x > (CAOIBox::m_BoxPosCad.x+szX) ) { return false; }
	const double szY = CAOIBox::m_BoxSize.cy*0.5;
	if ( pt.y < (CAOIBox::m_BoxPosCad.y-szY) ) { return false; }
	if ( pt.y > (CAOIBox::m_BoxPosCad.y+szY) ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxBePickByCadRes(const TPOINT2D &pt, bool IsExceptionAngle) const
{
	const double szX = CAOIBox::m_BoxSizeRes.cx*0.5;
	if ( pt.x < (CAOIBox::m_BoxPosCadRes.x-szX) ) { return false; }
	if ( pt.x > (CAOIBox::m_BoxPosCadRes.x+szX) ) { return false; }
	const double szY = CAOIBox::m_BoxSizeRes.cy*0.5;
	if ( pt.y < (CAOIBox::m_BoxPosCadRes.y-szY) ) { return false; }
	if ( pt.y > (CAOIBox::m_BoxPosCadRes.y+szY) ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxBePickByStage(const TPOINT2D &pt, bool IsExceptionAngle) const
{
	const double szX = CAOIBox::m_BoxSize.cx*0.5;
	if ( pt.x < (CAOIBox::m_BoxPosStage.x-szX) ) { return false; }
	if ( pt.x > (CAOIBox::m_BoxPosStage.x+szX) ) { return false; }
	const double szY = CAOIBox::m_BoxSize.cy*0.5;
	if ( pt.y < (CAOIBox::m_BoxPosStage.y-szY) ) { return false; }
	if ( pt.y > (CAOIBox::m_BoxPosStage.y+szY) ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxBePickByStageRes(const TPOINT2D &pt, bool IsExceptionAngle) const
{
	const double szX = CAOIBox::m_BoxSizeRes.cx*0.5;
	if ( pt.x < (CAOIBox::m_BoxPosStageRes.x-szX) ) { return false; }
	if ( pt.x > (CAOIBox::m_BoxPosStageRes.x+szX) ) { return false; }
	const double szY = CAOIBox::m_BoxSizeRes.cy*0.5;
	if ( pt.y < (CAOIBox::m_BoxPosStageRes.y-szY) ) { return false; }
	if ( pt.y > (CAOIBox::m_BoxPosStageRes.y+szY) ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxInRegionByCad(const TREGION4D &SelRgn, bool IsExceptionAngle, bool bEntireIn) const
{
	TREGION4D    Region;
	CAOIBox::GetBoxRegionCad(Region);
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxInRegionByCadRes(const TREGION4D &SelRgn, bool IsExceptionAngle, bool bEntireIn) const
{
	TREGION4D    Region;
	CAOIBox::GetBoxRegionCadRes(Region);
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxInRegionByStage(const TREGION4D &SelRgn, bool IsExceptionAngle, bool bEntireIn) const
{
	TREGION4D    Region;
	CAOIBox::GetBoxRegionStage(Region);
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);	
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckBoxInRegionByStageRes(const TREGION4D &SelRgn, bool IsExceptionAngle, bool bEntireIn) const
{
	TREGION4D    Region;
	CAOIBox::GetBoxRegionStageRes(Region);
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
void CAOIBox::MoveBox(double x, double y, bool bIncludeRes)
{
	CAOIBox::m_BoxPos.x += x;
	CAOIBox::m_BoxPos.y += y;
	
	CAOIBox::m_BoxCornerPos[0].x += x;
	CAOIBox::m_BoxCornerPos[0].y += y;
	CAOIBox::m_BoxCornerPos[1].x += x;
	CAOIBox::m_BoxCornerPos[1].y += y;
	CAOIBox::m_BoxCornerPos[2].x += x;
	CAOIBox::m_BoxCornerPos[2].y += y;
	CAOIBox::m_BoxCornerPos[3].x += x;
	CAOIBox::m_BoxCornerPos[3].y += y;
	CAOIBox::UpdateBoxAttachedPosCad(false);
	CAOIBox::UpdateBoxAttachedPosStage(false);

	if ( true == bIncludeRes )
	{	CAOIBox::MoveBoxRes(x, y);	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::MoveBox(const TPOINT2D &Pos, bool bIncludeRes)
{
	CAOIBox::m_BoxPos.x += Pos.x;
	CAOIBox::m_BoxPos.y += Pos.y;
	
	CAOIBox::m_BoxCornerPos[0].x += Pos.x;
	CAOIBox::m_BoxCornerPos[0].y += Pos.y;
	CAOIBox::m_BoxCornerPos[1].x += Pos.x;
	CAOIBox::m_BoxCornerPos[1].y += Pos.y;
	CAOIBox::m_BoxCornerPos[2].x += Pos.x;
	CAOIBox::m_BoxCornerPos[2].y += Pos.y;
	CAOIBox::m_BoxCornerPos[3].x += Pos.x;
	CAOIBox::m_BoxCornerPos[3].y += Pos.y;
	CAOIBox::UpdateBoxAttachedPosCad(false);
	CAOIBox::UpdateBoxAttachedPosStage(false);

	if ( true == bIncludeRes )
	{	CAOIBox::MoveBoxRes(Pos);	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SkewBoxAngle(double Skew)
{
	m_BoxAngleSkew += Skew;
	m_BoxAngleRes = m_BoxAngle+m_BoxAngleSkew;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::MoveBoxRes(double x, double y)
{
	m_BoxPosRes.x += x;
	m_BoxPosRes.y += y;
	
	m_BoxCornerPosRes[0].x += x;
	m_BoxCornerPosRes[0].y += y;
	m_BoxCornerPosRes[1].x += x;
	m_BoxCornerPosRes[1].y += y;
	m_BoxCornerPosRes[2].x += x;
	m_BoxCornerPosRes[2].y += y;
	m_BoxCornerPosRes[3].x += x;
	m_BoxCornerPosRes[3].y += y;

	UpdateBoxAttachedPosCadRes();
	UpdateBoxAttachedPosStageRes();
}
//-------------------------------------------------------------------------------------//
void CAOIBox::MoveBoxRes(const TPOINT2D &Pos)
{
	CAOIBox::m_BoxPosRes.x += Pos.x;
	CAOIBox::m_BoxPosRes.y += Pos.y;
	
	CAOIBox::m_BoxCornerPosRes[0].x += Pos.x;
	CAOIBox::m_BoxCornerPosRes[0].y += Pos.y;
	CAOIBox::m_BoxCornerPosRes[1].x += Pos.x;
	CAOIBox::m_BoxCornerPosRes[1].y += Pos.y;
	CAOIBox::m_BoxCornerPosRes[2].x += Pos.x;
	CAOIBox::m_BoxCornerPosRes[2].y += Pos.y;
	CAOIBox::m_BoxCornerPosRes[3].x += Pos.x;
	CAOIBox::m_BoxCornerPosRes[3].y += Pos.y;

	CAOIBox::UpdateBoxAttachedPosCadRes();
	CAOIBox::UpdateBoxAttachedPosStageRes();
}
//-------------------------------------------------------------------------------------//
void CAOIBox::ScaleBox(double sx, double sy, bool bIncludeRes)
{	
	CAOIBox::m_BoxPos.x *= sx;
	CAOIBox::m_BoxPos.y *= sy;
	CAOIBox::m_BoxSize.cx *= sx;
	CAOIBox::m_BoxSize.cy *= sy;	
	CAOIBox::m_BoxCornerPos[0].x *= sx;
	CAOIBox::m_BoxCornerPos[0].y *= sy;
	CAOIBox::m_BoxCornerPos[1].x *= sx;
	CAOIBox::m_BoxCornerPos[1].y *= sy;
	CAOIBox::m_BoxCornerPos[2].x *= sx;
	CAOIBox::m_BoxCornerPos[2].y *= sy;
	CAOIBox::m_BoxCornerPos[3].x *= sx;
	CAOIBox::m_BoxCornerPos[3].y *= sy;
	CAOIBox::UpdateBoxAttachedPosCad(false);
	CAOIBox::UpdateBoxAttachedPosStage(false);

	if ( true == bIncludeRes )
	{	ScaleBoxRes(sx, sy);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::ScaleBoxRes(double sx, double sy)
{
	CAOIBox::m_BoxPosRes.x *= sx;
	CAOIBox::m_BoxPosRes.y *= sy;
	CAOIBox::m_BoxSizeRes.cx *= sx;
	CAOIBox::m_BoxSizeRes.cy *= sy;		
	CAOIBox::m_BoxCornerPosRes[0].x *= sx;
	CAOIBox::m_BoxCornerPosRes[0].y *= sy;
	CAOIBox::m_BoxCornerPosRes[1].x *= sx;
	CAOIBox::m_BoxCornerPosRes[1].y *= sy;
	CAOIBox::m_BoxCornerPosRes[2].x *= sx;
	CAOIBox::m_BoxCornerPosRes[2].y *= sy;
	CAOIBox::m_BoxCornerPosRes[3].x *= sx;
	CAOIBox::m_BoxCornerPosRes[3].y *= sy;
	CAOIBox::UpdateBoxAttachedPosCadRes();
	CAOIBox::UpdateBoxAttachedPosStageRes();
}
//-------------------------------------------------------------------------------------//
void CAOIBox::ScaleBoxSize(double sx, double sy, bool bIncludeRes)
{
	CAOIBox::m_BoxSize.cx *= sx;
	CAOIBox::m_BoxSize.cy *= sy;
	JetAPI::ScaleCornerPosSize(sx, sy, m_BoxCornerPos);
	if ( true == bIncludeRes )
	{	ScaleBoxSizeRes(sx, sy);	}
}
//-------------------------------------------------------------------------------------//
void CAOIBox::ScaleBoxSizeRes(double sx, double sy)
{
	CAOIBox::m_BoxSizeRes.cx *= sx;
	CAOIBox::m_BoxSizeRes.cy *= sy;		
	JetAPI::ScaleCornerPosSize(sx, sy, m_BoxCornerPosRes);
}
//-------------------------------------------------------------------------------------//
void CAOIBox::RotateBox(double Angle, double CPX, double CPY, bool bIncludeRes)
{
	TPOINT2D *BoxPt=NULL;	
	BOX_TOWARD Toward = BOX_TOWARD_NULL;
	const double NowBoxAngle = GetBoxAngle();
	const size_t PolygonPtCount=GetBoxPolygonPtCount();
	const double NextBoxAngle = JetAPI::RotateAngle(Angle, NowBoxAngle);
	const double NowAttachedAngle = GetBoxAttachedAngle();
	const double NextAttachedAngle = JetAPI::RotateAngle(Angle, NowAttachedAngle);

	bool IsNowExceptionAngle = JetAPI::CheckIsExceptionAngle(NowAttachedAngle);
	bool IsNextExceptionAngle = JetAPI::CheckIsExceptionAngle(NextAttachedAngle);
	
	if ( IsNowExceptionAngle==true || IsNextExceptionAngle==true )//旋轉前後是特殊角度
	{
		//先返轉回去0度
		const double InversAngle = JetAPI::RotateAngle(-NowAttachedAngle, 0);
		if ( fabs(InversAngle) > 0.0001 )
		{
			Toward = GetBoxToward();
			Toward = JetAPI::RotateToward(InversAngle, Toward);
			SetBoxToward(Toward);

			JetAPI::RotateSize(InversAngle, m_BoxSize);	
			JetAPI::RotatePos(InversAngle, CPX, CPY, m_BoxPos);
			UpdateBoxCornerPos();	

			JetAPI::RotateSize(InversAngle, m_BoxSizeRes);	
			JetAPI::RotatePos(InversAngle, CPX, CPY, m_BoxPosRes);
			UpdateBoxCornerPosRes();	

			RotateBoxPolygon(InversAngle, CPX, CPY);
		}
		
		//再轉回去
		if ( fabs(NextAttachedAngle) > 0.0001 )
		{
			Toward = GetBoxToward();
			Toward = JetAPI::RotateToward(NextAttachedAngle, Toward);
			SetBoxToward(Toward);

			JetAPI::RotateSize(NextAttachedAngle, m_BoxSize);	
			JetAPI::RotatePos(NextAttachedAngle, CPX, CPY, m_BoxPos);	
			JetAPI::RotateCornerPos(NextAttachedAngle, CPX, CPY, m_BoxCornerPos);

			JetAPI::RotateSize(NextAttachedAngle, m_BoxSizeRes);	
			JetAPI::RotatePos(NextAttachedAngle, CPX, CPY, m_BoxPosRes);
			JetAPI::RotateCornerPos(NextAttachedAngle, CPX, CPY, m_BoxCornerPosRes);

			RotateBoxPolygon(NextAttachedAngle, CPX, CPY);
		}
	}
	else
	{
		double AngelAdjust = JetAPI::RotateAngle(Angle, 0);		
		Toward = GetBoxToward();
		Toward = JetAPI::RotateToward(AngelAdjust, Toward);
		SetBoxToward(Toward);

		JetAPI::RotateSize(AngelAdjust, m_BoxSize);	
		JetAPI::RotatePos(AngelAdjust, CPX, CPY, m_BoxPos);			
		UpdateBoxCornerPos();

		JetAPI::RotateSize(AngelAdjust, m_BoxSizeRes);	
		JetAPI::RotatePos(AngelAdjust, CPX, CPY, m_BoxPosRes);
		UpdateBoxCornerPosRes();

		RotateBoxPolygon(AngelAdjust, CPX, CPY);
	}
	SetBoxAttachedAngle(NextAttachedAngle);
	SetBoxAngle(NextBoxAngle);
	SetBoxAngleRes(NextBoxAngle);	

	UpdateBoxAttachedPosCad(true);
	UpdateBoxAttachedPosStage(true);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::MirrorBoxXAxis(double CPY)
{	
	BOX_TOWARD Toward = this->GetBoxToward();
	JetAPI::MirrorXAxisToward(Toward);
	SetBoxToward(Toward);
	
	const double BoxAngle = GetBoxAngle();
	const double NewBoxAngle = JetAPI::MirrorXAxisAngle(BoxAngle);
	SetBoxAngle(NewBoxAngle);
	SetBoxAngleRes(NewBoxAngle);
	
	const double BoxAttachedAngle = m_BoxAttachedAngle;
	m_BoxAttachedAngle = JetAPI::MirrorXAxisAngle(BoxAttachedAngle);

	TPOINT2D  Pos, PosRes;
	GetBoxPos(Pos);	
	GetBoxPosRes(PosRes);	
	
	JetAPI::MirrorXAxisPos(CPY, Pos);
	JetAPI::MirrorXAxisPos(CPY, PosRes);
	SetBoxPos(Pos, false);
	SetBoxPosRes(PosRes);

	UpdateBoxAttachedPosCad(true);
	UpdateBoxAttachedPosStage(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::MirrorBoxYAxis(double CPX)
{
	BOX_TOWARD Toward = this->GetBoxToward();
	JetAPI::MirrorYAxisToward(Toward);
	SetBoxToward(Toward);	
	const double BoxAngle = GetBoxAngle();
	const double NewBoxAngle = JetAPI::MirrorYAxisAngle(BoxAngle);
	SetBoxAngle(NewBoxAngle);
	SetBoxAngleRes(NewBoxAngle);
	
	const double BoxAttachedAngle = m_BoxAttachedAngle;
	m_BoxAttachedAngle = JetAPI::MirrorYAxisAngle(BoxAttachedAngle);

	TPOINT2D  Pos, PosRes;
	GetBoxPos(Pos);	
	GetBoxPosRes(PosRes);	

	JetAPI::MirrorYAxisPos(CPX, Pos);
	JetAPI::MirrorYAxisPos(CPX, PosRes);
	
	SetBoxPos(Pos, false);
	SetBoxPosRes(PosRes);

	UpdateBoxAttachedPosCad(true);
	UpdateBoxAttachedPosStage(true);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxAttachedPosCad(const TPOINT2D &Pos)//設定所屬零件的座標-Cad
{
	CAOIBox::m_BoxAttachedPosCad = Pos;
	CAOIBox::UpdateBoxAttachedPosCad(true);
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxAttachedPosCad(double PosX, double PosY)//設定所屬零件的座標-Cad
{
	CAOIBox::m_BoxAttachedPosCad.x = PosX;
	CAOIBox::m_BoxAttachedPosCad.y = PosY;
	CAOIBox::UpdateBoxAttachedPosCad(true);
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxAttachedPosStage(const TPOINT2D &Pos)//設定所屬零件的座標-Stage	
{		
	CAOIBox::m_BoxAttachedPosStage = Pos;	
	CAOIBox::UpdateBoxAttachedPosStage(true);
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::SetBoxAttachedPosStage(double PosX, double PosY)//設定所屬零件的座標-Stage	
{
	CAOIBox::m_BoxAttachedPosStage.x = PosX;
	CAOIBox::m_BoxAttachedPosStage.y = PosY;
	CAOIBox::UpdateBoxAttachedPosStage(true);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::CheckDrawBoxText(const RECT &Rect) const
{
	const int W=Rect.right-Rect.left;
	if ( W < 64 ) { return false; }
	const int H=Rect.bottom-Rect.top;
	if ( H < 32 ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIBox::DrawBoxEditKernel_1(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	RECT  Rect;			
	TREGION4D Region;	
	BOX_SHAPE_MODE BoxShapeMode = GetBoxShapeMode();
	const double   BoxShapeParam = GetBoxShapeParam();
	const double   BoxShapeParam2 = GetBoxShapeParam2();
	double BoxMinX=0, BoxMinY=0, BoxMaxX=0, BoxMaxY=0;			

	const int LineWidth = 1;
	const double ComAngle = CAOIBox::GetBoxAttachedAngle();	                                  
	const double ImageAngle= JetAPI::MirrorXAxisAngle(ComAngle);	
	if ( false == DrawParam.IsExceptionAngle )
	{		
		CAOIBox::GetBoxRegion(Region);	

		Region.minX -= DrawParam.Extend.x;
		Region.maxX += DrawParam.Extend.x;
		Region.minY -= DrawParam.Extend.y;
		Region.maxY += DrawParam.Extend.y;
		Rect.left    = (int)((Region.minX-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		Rect.right   = (int)((Region.maxX-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		Rect.top     = DrawParam.WndRect.bottom-((int)((Region.maxY-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
		Rect.bottom  = DrawParam.WndRect.bottom-((int)((Region.minY-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;

		//是否在視窗畫面內
		if ( Rect.left > DrawParam.WndRect.right ) { return; }
		if ( Rect.top  > DrawParam.WndRect.bottom ) { return; }
		if ( Rect.right < DrawParam.WndRect.left ) { return; }
		if ( Rect.bottom < DrawParam.WndRect.top ) { return; }

		if ( DrawParam.WndRect.left > Rect.right ) { return; }
		if ( DrawParam.WndRect.top  > Rect.bottom ) { return; }
		if ( DrawParam.WndRect.right < Rect.left ) { return; }
		if ( DrawParam.WndRect.bottom < Rect.top ) { return; }

		switch ( DrawParam.ShapeMode )
		{
		case BOX_SHAPE_ROUND_RECT:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawRoundRectRect(hDC, Rect, BoxShapeParam, DrawParam.hPenNull, DrawParam.hBrushMask);	}
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawRoundRectLine(hDC, Rect, BoxShapeParam, DrawParam.Toward, 0);	 }
			else
			{	CAOIBox::DrawRoundRectLine(hDC, Rect, BoxShapeParam, DrawParam.Toward, DrawParam.TowardSize);	 }
			break;
		case BOX_SHAPE_ELLIPSE:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawEllipseRect(hDC, Rect, DrawParam.hPenNull, DrawParam.hBrushMask);	}
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawEllipseLine(hDC, Rect, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawEllipseLine(hDC, Rect, DrawParam.Toward, DrawParam.TowardSize); }
			break;		
		case BOX_SHAPE_CAPSULE:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawCapsuleRect(hDC, Rect, DrawParam.hPenNull, DrawParam.hBrushMask); }
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawCapsuleLine(hDC, Rect, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawCapsuleLine(hDC, Rect, DrawParam.Toward, DrawParam.TowardSize); }
			break;
		case BOX_SHAPE_BULLET:			
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawBulletRect(hDC, Rect, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask); }
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawBulletLine(hDC, Rect, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawBulletLine(hDC, Rect, DrawParam.Toward, DrawParam.TowardSize); }
			break;
		case BOX_SHAPE_HALF_ROUND_RECT:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawHalfRoundRectRect(hDC, Rect, BoxShapeParam, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask);	}
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawHalfRoundRectLine(hDC, Rect, BoxShapeParam, DrawParam.Toward, 0);	 }
			else
			{	CAOIBox::DrawHalfRoundRectLine(hDC, Rect, BoxShapeParam, DrawParam.Toward, DrawParam.TowardSize);	 }
			break;
		case BOX_SHAPE_T_SHAPE:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawTShapeRect(hDC, Rect, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask);	}
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawTShapeRectLine(hDC, Rect, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, 0);	 }
			else
			{	CAOIBox::DrawTShapeRectLine(hDC, Rect, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, DrawParam.TowardSize);	 }
			break;
		default:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawRectangleRect(hDC, Rect, DrawParam.hPenNull, DrawParam.hBrushMask); }
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawRectangleLine(hDC, Rect, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawRectangleLine(hDC, Rect, DrawParam.Toward, DrawParam.TowardSize); }
			break;
		}
		
		if ( DrawParam.DrawEditLine == true )
		{	
			if ( CAOIBox::GetBoxSelected() == true )//繪製選取編輯框
			{	CAOIBox::DrawEditRect(hDC, Rect, DrawParam.EditLineSize);	}
		}
		if ( true == DrawParam.DrawBoxIndex )
		{	
			int TextLen = DrawParam.strBoxIndex.GetLength();
			int RectCpX = (Rect.left+Rect.right)/2;
			int RectCpY = (Rect.top+Rect.bottom)/2;
			if ( TextLen > 0 ) 
			{	::TextOut(hDC, Rect.left+4, Rect.top+4, DrawParam.strBoxIndex, TextLen); }
		}
	}
	else
	{	
		double CornerCPX=0, CornerCPY=0;
		double CornerPTX[4]={0}, CornerPTY[4]={0};
		POINT CornPoint[4]={0}, CornPoint2[4]={0};
		BOX_TOWARD Toward = GetBoxToward();
		const int ImageToward = JetAPI::RotateToward(-ComAngle, Toward);

		CornerPTX[0] = CornPoint2[0].x = CornPoint[0].x = (int)((m_BoxCornerPos[0].x-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		CornerPTY[0] = CornPoint2[0].y = CornPoint[0].y = DrawParam.WndRect.bottom-((int)((m_BoxCornerPos[0].y-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
		CornerPTX[1] = CornPoint2[1].x = CornPoint[1].x = (int)((m_BoxCornerPos[1].x-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		CornerPTY[1] = CornPoint2[1].y = CornPoint[1].y = DrawParam.WndRect.bottom-((int)((m_BoxCornerPos[1].y-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
		CornerPTX[2] = CornPoint2[2].x = CornPoint[2].x = (int)((m_BoxCornerPos[2].x-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		CornerPTY[2] = CornPoint2[2].y = CornPoint[2].y = DrawParam.WndRect.bottom-((int)((m_BoxCornerPos[2].y-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
		CornerPTX[3] = CornPoint2[3].x = CornPoint[3].x = (int)((m_BoxCornerPos[3].x-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		CornerPTY[3] = CornPoint2[3].y = CornPoint[3].y = DrawParam.WndRect.bottom-((int)((m_BoxCornerPos[3].y-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;		

		//是否在視窗畫面內
		RECT CorRect;
		JetAPI::PointsToRect(CornPoint, 4, CorRect);		
		if ( CorRect.left > DrawParam.WndRect.right ) { return; }
		if ( CorRect.top  > DrawParam.WndRect.bottom ) { return; }
		if ( CorRect.right < DrawParam.WndRect.left ) { return; }
		if ( CorRect.bottom < DrawParam.WndRect.top ) { return; }

		if ( DrawParam.WndRect.left > CorRect.right ) { return; }
		if ( DrawParam.WndRect.top  > CorRect.bottom ) { return; }
		if ( DrawParam.WndRect.right < CorRect.left ) { return; }
		if ( DrawParam.WndRect.bottom < CorRect.top ) { return; }

		::MoveToEx(hDC, CornPoint[0].x, CornPoint[0].y, NULL);
		::LineTo(hDC, CornPoint[1].x, CornPoint[1].y);
		::LineTo(hDC, CornPoint[2].x, CornPoint[2].y);
		::LineTo(hDC, CornPoint[3].x, CornPoint[3].y);
		::LineTo(hDC, CornPoint[0].x, CornPoint[0].y);

		JetAPI::PointsCenter(CornerPTX, CornerPTY, 4, CornerCPX, CornerCPY);
		JetAPI::RotateCornerPos(-ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

		Region.maxX = Region.minX = CornerPTX[0];	Region.maxY = Region.minY = CornerPTY[0];	
		if ( Region.minX > CornerPTX[1] ) { Region.minX = CornerPTX[1]; }
		if ( Region.minY > CornerPTY[1] ) { Region.minY = CornerPTY[1]; }
		if ( Region.maxX < CornerPTX[1] ) { Region.maxX = CornerPTX[1]; }
		if ( Region.maxY < CornerPTY[1] ) { Region.maxY = CornerPTY[1]; }

		if ( Region.minX > CornerPTX[2] ) { Region.minX = CornerPTX[2]; }
		if ( Region.minY > CornerPTY[2] ) { Region.minY = CornerPTY[2]; }
		if ( Region.maxX < CornerPTX[2] ) { Region.maxX = CornerPTX[2]; }
		if ( Region.maxY < CornerPTY[2] ) { Region.maxY = CornerPTY[2]; }

		if ( Region.minX > CornerPTX[3] ) { Region.minX = CornerPTX[3]; }
		if ( Region.minY > CornerPTY[3] ) { Region.minY = CornerPTY[3]; }
		if ( Region.maxX < CornerPTX[3] ) { Region.maxX = CornerPTX[3]; }
		if ( Region.maxY < CornerPTY[3] ) { Region.maxY = CornerPTY[3]; }
		
		switch ( ImageToward )
		{
		case BOX_TOWARD_UP:
			CornerPTX[0] = (Region.minX+Region.maxX)/2.0;
			CornerPTY[0] = Region.minY-DrawParam.TowardSize;
			CornerPTX[1] = CornerPTX[0]-DrawParam.TowardSize; 
			CornerPTX[2] = CornerPTX[0]+DrawParam.TowardSize; 
			CornerPTY[2] = CornerPTY[1] = Region.minY;
			CornerPTY[3] = CornerPTX[3] = 0.0;
			JetAPI::RotateCornerPos(ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

			if ( BOX_TOWARD_NULL != DrawParam.Toward )
			{
				CornPoint2[0].x = (int)(CornerPTX[0]+0.5);	CornPoint2[0].y = (int)(CornerPTY[0]+0.5);
				CornPoint2[1].x = (int)(CornerPTX[1]+0.5);	CornPoint2[1].y = (int)(CornerPTY[1]+0.5);
				CornPoint2[2].x = (int)(CornerPTX[2]+0.5);	CornPoint2[2].y = (int)(CornerPTY[2]+0.5);			
				CAOIBox::DrawTriangleLine(hDC, CornPoint2);
			}
			break;
		case BOX_TOWARD_LEFT:			
			CornerPTX[0] = Region.minX-DrawParam.TowardSize;
			CornerPTY[0] = (Region.minY+Region.maxY)/2.0;			
			CornerPTX[2] = CornerPTX[1] = Region.minX;
			CornerPTY[1] = CornerPTY[0]-DrawParam.TowardSize; 
			CornerPTY[2] = CornerPTY[0]+DrawParam.TowardSize; 
			CornerPTY[3] = CornerPTX[3] = 0.0;
			JetAPI::RotateCornerPos(ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

			if ( BOX_TOWARD_NULL != DrawParam.Toward )
			{
				CornPoint2[0].x = (int)(CornerPTX[0]+0.5);	CornPoint2[0].y = (int)(CornerPTY[0]+0.5);
				CornPoint2[1].x = (int)(CornerPTX[1]+0.5);	CornPoint2[1].y = (int)(CornerPTY[1]+0.5);
				CornPoint2[2].x = (int)(CornerPTX[2]+0.5);	CornPoint2[2].y = (int)(CornerPTY[2]+0.5);			
				CAOIBox::DrawTriangleLine(hDC, CornPoint2);
			}
			break;
		case BOX_TOWARD_DOWN:
			CornerPTX[0] = (Region.minX+Region.maxX)/2.0;
			CornerPTY[0] = Region.maxY+DrawParam.TowardSize;
			CornerPTX[1] = CornerPTX[0]-DrawParam.TowardSize; 
			CornerPTX[2] = CornerPTX[0]+DrawParam.TowardSize; 
			CornerPTY[2] = CornerPTY[1] = Region.maxY;
			CornerPTY[3] = CornerPTX[3] = 0.0;
			JetAPI::RotateCornerPos(ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

			if ( BOX_TOWARD_NULL != DrawParam.Toward )
			{
				CornPoint2[0].x = (int)(CornerPTX[0]+0.5);	CornPoint2[0].y = (int)(CornerPTY[0]+0.5);
				CornPoint2[1].x = (int)(CornerPTX[1]+0.5);	CornPoint2[1].y = (int)(CornerPTY[1]+0.5);
				CornPoint2[2].x = (int)(CornerPTX[2]+0.5);	CornPoint2[2].y = (int)(CornerPTY[2]+0.5);			
				CAOIBox::DrawTriangleLine(hDC, CornPoint2);
			}
			break;
		case BOX_TOWARD_RIGHT:
			CornerPTX[0] = Region.maxX+DrawParam.TowardSize;
			CornerPTY[0] = (Region.minY+Region.maxY)/2.0;			
			CornerPTX[2] = CornerPTX[1] = Region.maxX;
			CornerPTY[1] = CornerPTY[0]-DrawParam.TowardSize; 
			CornerPTY[2] = CornerPTY[0]+DrawParam.TowardSize; 
			CornerPTY[3] = CornerPTX[3] = 0.0;
			JetAPI::RotateCornerPos(ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

			if ( BOX_TOWARD_NULL != DrawParam.Toward )
			{
				CornPoint2[0].x = (int)(CornerPTX[0]+0.5);	CornPoint2[0].y = (int)(CornerPTY[0]+0.5);
				CornPoint2[1].x = (int)(CornerPTX[1]+0.5);	CornPoint2[1].y = (int)(CornerPTY[1]+0.5);
				CornPoint2[2].x = (int)(CornerPTX[2]+0.5);	CornPoint2[2].y = (int)(CornerPTY[2]+0.5);			
				CAOIBox::DrawTriangleLine(hDC, CornPoint2);
			}
			break;
		}
		if ( DrawParam.DrawEditLine == true )
		{
			if ( CAOIBox::GetBoxSelected() == true )//繪製選取編輯框	
			{	CAOIBox::DrawEditRect(hDC, CornPoint, DrawParam.EditLineSize);	}
		}		

		if ( true == DrawParam.DrawBoxIndex )
		{	
			JetAPI::CornerPtToRect(CornPoint2, Rect);
			int TextLen = DrawParam.strBoxIndex.GetLength();
			int RectCpX = (Rect.left+Rect.right)/2;
			int RectCpY = (Rect.top+Rect.bottom)/2;
			if ( TextLen > 0 ) 
			{	::TextOut(hDC, Rect.left+4, Rect.top+4, DrawParam.strBoxIndex, TextLen); }
		}
	}	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::DrawBoxEditKernel_2(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	RECT  Rect;			
	TREGION4D Region;	
	BOX_SHAPE_MODE BoxShapeMode = GetBoxShapeMode();
	const double   BoxShapeParam = GetBoxShapeParam();
	const double   BoxShapeParam2 = GetBoxShapeParam2();
	const int LineWidth = 1;	
	bool  IsExceptionAngle=DrawParam.IsExceptionAngle;
	double BoxMinX=0, BoxMinY=0, BoxMaxX=0, BoxMaxY=0;				
	const double ComAngle = DrawParam.ComponentAngle;//CAOIBox::GetBoxAttachedAngle();	
	const double ImageAngle = JetAPI::MirrorXAxisAngle(ComAngle);
	const double AngleLabel = JetAPI::GetAngleLabel(ComAngle);
	const double DCAngle = ImageAngle+AngleLabel;//要轉DC的部分要注意	

	CAOIBox::GetBoxRegion(Region);	

	Region.minX -= DrawParam.Extend.x;
	Region.maxX += DrawParam.Extend.x;
	Region.minY -= DrawParam.Extend.y;
	Region.maxY += DrawParam.Extend.y;
	//Rect.left    = (int)((Region.minX-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
	//Rect.right   = (int)((Region.maxX-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
	//Rect.top     = DrawParam.WndRect.bottom-((int)((Region.maxY-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
	//Rect.bottom  = DrawParam.WndRect.bottom-((int)((Region.minY-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;

	double L = DrawParam.RealToImageX(Region.minX);
	double R = DrawParam.RealToImageX(Region.maxX);
	double T = DrawParam.RealToImageY(Region.maxY);
	double B = DrawParam.RealToImageY(Region.minY);
	Rect.left   = (int)(L+0.5);
	Rect.right  = (int)(R+0.5);
	Rect.top    = (int)(T+0.5);
	Rect.bottom = (int)(B+0.5);

	//是否在視窗畫面內
	if ( Rect.left > DrawParam.WndRect.right ) { return; }
	if ( Rect.top  > DrawParam.WndRect.bottom ) { return; }
	if ( Rect.right < DrawParam.WndRect.left ) { return; }
	if ( Rect.bottom < DrawParam.WndRect.top ) { return; }

	if ( DrawParam.WndRect.left > Rect.right ) { return; }
	if ( DrawParam.WndRect.top  > Rect.bottom ) { return; }
	if ( DrawParam.WndRect.right < Rect.left ) { return; }
	if ( DrawParam.WndRect.bottom < Rect.top ) { return; }

	XFORM xFormNew;
	XFORM xFormOld;
	RECT  Rect2=Rect;
	int OldGraphicsMode = GM_COMPATIBLE;
	if ( true == IsExceptionAngle )
	{
		const double CpX = (Rect.left+Rect.right)*0.5;
		const double CpY = (Rect.top+Rect.bottom)*0.5;	
		const int nCpX = (int)(CpX+0.5);
		const int nCpY = (int)(CpY+0.5);
		::OffsetRect(&Rect2, -nCpX, -nCpY);
		JetAPI::RotateDCTransform(DCAngle, CpX, CpY, xFormNew);		

		OldGraphicsMode = SetGraphicsMode(hDC, GM_ADVANCED);
		//int OldMapMode = SetMapMode(hDC, MM_LOENGLISH);//將DC的左上方起點改成左下方為起點
		GetWorldTransform(hDC, &xFormOld);
		SetWorldTransform(hDC, &xFormNew); 
	}

	switch ( DrawParam.ShapeMode )
	{
	case BOX_SHAPE_ROUND_RECT:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawRoundRectRect(hDC, Rect2, BoxShapeParam, DrawParam.hPenNull, DrawParam.hBrushMask);	}
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawRoundRectLine(hDC, Rect2, BoxShapeParam, DrawParam.Toward, 0);	 }
		else
		{	CAOIBox::DrawRoundRectLine(hDC, Rect2, BoxShapeParam, DrawParam.Toward, DrawParam.TowardSize);	 }
		break;
	case BOX_SHAPE_ELLIPSE:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawEllipseRect(hDC, Rect2, DrawParam.hPenNull, DrawParam.hBrushMask);	}
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawEllipseLine(hDC, Rect2, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawEllipseLine(hDC, Rect2, DrawParam.Toward, DrawParam.TowardSize); }
		break;		
	case BOX_SHAPE_CAPSULE:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawCapsuleRect(hDC, Rect2, DrawParam.hPenNull, DrawParam.hBrushMask); }
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawCapsuleLine(hDC, Rect2, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawCapsuleLine(hDC, Rect2, DrawParam.Toward, DrawParam.TowardSize); }
		break;
	case BOX_SHAPE_BULLET:			
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawBulletRect(hDC, Rect2, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask); }
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawBulletLine(hDC, Rect2, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawBulletLine(hDC, Rect2, DrawParam.Toward, DrawParam.TowardSize); }
		break;
	case BOX_SHAPE_HALF_ROUND_RECT:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawHalfRoundRectRect(hDC, Rect2, BoxShapeParam, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask);	}
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawHalfRoundRectLine(hDC, Rect2, BoxShapeParam, DrawParam.Toward, 0);	 }
		else
		{	CAOIBox::DrawHalfRoundRectLine(hDC, Rect2, BoxShapeParam, DrawParam.Toward, DrawParam.TowardSize);	 }
		break;
	case BOX_SHAPE_T_SHAPE:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawTShapeRect(hDC, Rect2, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask);	}
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawTShapeRectLine(hDC, Rect2, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, 0);	 }
		else
		{	CAOIBox::DrawTShapeRectLine(hDC, Rect2, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, DrawParam.TowardSize);	 }		
		break;
	default:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawRectangleRect(hDC, Rect2, DrawParam.hPenNull, DrawParam.hBrushMask); }
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawRectangleLine(hDC, Rect2, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawRectangleLine(hDC, Rect2, DrawParam.Toward, DrawParam.TowardSize); }
		break;
	}		
	if ( true == DrawParam.DrawEditLine )
	{	
		if ( CAOIBox::GetBoxSelected() == true )//繪製選取編輯框
		{	CAOIBox::DrawEditRect(hDC, Rect2, DrawParam.EditLineSize);	}
	}

	if ( true == DrawParam.DrawBoxIndex )
	{	
		int TextLen = DrawParam.strBoxIndex.GetLength();
		int RectCpX = (Rect.left+Rect.right)/2;
		int RectCpY = (Rect.top+Rect.bottom)/2;
		if ( TextLen > 0 ) 
		{	::TextOut(hDC, Rect.left+4, Rect.top+4, DrawParam.strBoxIndex, TextLen); }
	}
	if ( true == IsExceptionAngle )
	{
		//復歸座標系統
		SetWorldTransform(hDC, &xFormOld); 
		//::SetMapMode(hDC, OldMapMode); 
		::SetGraphicsMode(hDC, OldGraphicsMode);	
	}	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::DrawBoxEdit(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	//return DrawBoxEditKernel_1(hDC, DrawParam);
	return DrawBoxEditKernel_2(hDC, DrawParam);	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::DrawBoxResultKernel_1(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	RECT  Rect;		
	TREGION4D Region;	
	BOX_SHAPE_MODE BoxShapeMode = GetBoxShapeMode();
	const double   BoxShapeParam = GetBoxShapeParam();
	const double   BoxShapeParam2 = GetBoxShapeParam2();
	double BoxMinX=0, BoxMinY=0, BoxMaxX=0, BoxMaxY=0;		

	const int LineWidth = 1;
	const double ComAngle = CAOIBox::GetBoxAttachedAngle();
	const double ImageAngle= JetAPI::MirrorXAxisAngle(ComAngle);	
	const bool   ResultTextVisibled = GetBoxResultTextVisibled();	
	const bool   PolygonVisibled = GetBoxPolygonVisibled();
	if ( false == DrawParam.IsExceptionAngle )
	{		
		CAOIBox::GetBoxRegionRes(Region);	

		Region.minX -= DrawParam.Extend.x;
		Region.maxX += DrawParam.Extend.x;
		Region.minY -= DrawParam.Extend.y;
		Region.maxY += DrawParam.Extend.y;
		Rect.left    = (int)((Region.minX-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		Rect.right   = (int)((Region.maxX-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		Rect.top     = DrawParam.WndRect.bottom-((int)((Region.maxY-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
		Rect.bottom  = DrawParam.WndRect.bottom-((int)((Region.minY-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;

		//是否在視窗畫面內
		if ( Rect.left > DrawParam.WndRect.right ) { return; }
		if ( Rect.top  > DrawParam.WndRect.bottom ) { return; }
		if ( Rect.right < DrawParam.WndRect.left ) { return; }
		if ( Rect.bottom < DrawParam.WndRect.top ) { return; }

		if ( DrawParam.WndRect.left > Rect.right ) { return; }
		if ( DrawParam.WndRect.top  > Rect.bottom ) { return; }
		if ( DrawParam.WndRect.right < Rect.left ) { return; }
		if ( DrawParam.WndRect.bottom < Rect.top ) { return; }

		switch ( DrawParam.ShapeMode )
		{
		case BOX_SHAPE_ROUND_RECT:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawRoundRectRect(hDC, Rect, BoxShapeParam, DrawParam.hPenNull, DrawParam.hBrushMask);	}
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawRoundRectLine(hDC, Rect, BoxShapeParam, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawRoundRectLine(hDC, Rect, BoxShapeParam, DrawParam.Toward, DrawParam.TowardSize); }
			break;
		case BOX_SHAPE_ELLIPSE:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawEllipseRect(hDC, Rect, DrawParam.hPenNull, DrawParam.hBrushMask); }
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawEllipseLine(hDC, Rect, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawEllipseLine(hDC, Rect, DrawParam.Toward, 0); }
			break;
		case BOX_SHAPE_CAPSULE:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawCapsuleRect(hDC, Rect, DrawParam.hPenNull, DrawParam.hBrushMask); }
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawCapsuleLine(hDC, Rect, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawCapsuleLine(hDC, Rect, DrawParam.Toward, 0); }
			break;
		case BOX_SHAPE_BULLET:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawBulletRect(hDC, Rect, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask); }
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawBulletLine(hDC, Rect, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawBulletLine(hDC, Rect, DrawParam.Toward, 0); }
			break;
		case BOX_SHAPE_HALF_ROUND_RECT:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawHalfRoundRectRect(hDC, Rect, BoxShapeParam, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask);	}
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawHalfRoundRectLine(hDC, Rect, BoxShapeParam, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawHalfRoundRectLine(hDC, Rect, BoxShapeParam, DrawParam.Toward, DrawParam.TowardSize); }
			break;
		case BOX_SHAPE_T_SHAPE:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawTShapeRect(hDC, Rect, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask);	}
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawTShapeRectLine(hDC, Rect, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawTShapeRectLine(hDC, Rect, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, DrawParam.TowardSize); }			
			break;
		default:
			if ( true == DrawParam.FillRegion )
			{	CAOIBox::DrawRectangleRect(hDC, Rect, DrawParam.hPenNull, DrawParam.hBrushMask); }
			if ( false == DrawParam.DrawTowardFeature )
			{	CAOIBox::DrawRectangleLine(hDC, Rect, DrawParam.Toward, 0); }
			else
			{	CAOIBox::DrawRectangleLine(hDC, Rect, DrawParam.Toward, DrawParam.TowardSize); }
			break;
		}

		if ( true == DrawParam.DrawResultText )
		{
			if ( true == PolygonVisibled )
			{
			}
			if ( true == ResultTextVisibled ) 
			{					
				if ( CheckDrawBoxText(Rect) == true )
				{
					int TextLen = m_BoxResultText.GetLength();
					int RectCpX = (Rect.left+Rect.right)/2;
					int RectCpY = (Rect.top+Rect.bottom)/2;
					int OffsetX = TextLen*2;
					::TextOut(hDC, RectCpX-OffsetX, RectCpY, m_BoxResultText, TextLen);
				}
			}			
		}
		if ( true == DrawParam.DrawBoxIndex )
		{
			int TextLen = DrawParam.strBoxIndex.GetLength();
			int RectCpX = (Rect.left+Rect.right)/2;
			int RectCpY = (Rect.top+Rect.bottom)/2;
			if ( TextLen > 0 ) 
			{	::TextOut(hDC, Rect.left+4, Rect.top+4, DrawParam.strBoxIndex, TextLen); }
		}		
		//if ( DrawParam.DrawEditLine == true )
		//{	
		//	if ( CAOIBox::GetBoxSelected() == true )//繪製選取編輯框
		//	{	CAOIBox::DrawEditRect(hDC, Rect, DrawParam.EditLineSize);	}
		//}
	}
	else
	{		
		
		double CornerCPX=0, CornerCPY=0;
		double CornerPTX[4]={0}, CornerPTY[4]={0};
		POINT CornPoint[4]={0}, CornPoint2[4]={0};
		BOX_TOWARD Toward = GetBoxToward();
		const int ImageToward = JetAPI::RotateToward(-ComAngle, Toward);

		CornerPTX[0] = CornPoint2[0].x = CornPoint[0].x = (int)((m_BoxCornerPosRes[0].x-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		CornerPTY[0] = CornPoint2[0].y = CornPoint[0].y = DrawParam.WndRect.bottom-((int)((m_BoxCornerPosRes[0].y-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
		CornerPTX[1] = CornPoint2[1].x = CornPoint[1].x = (int)((m_BoxCornerPosRes[1].x-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		CornerPTY[1] = CornPoint2[1].y = CornPoint[1].y = DrawParam.WndRect.bottom-((int)((m_BoxCornerPosRes[1].y-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
		CornerPTX[2] = CornPoint2[2].x = CornPoint[2].x = (int)((m_BoxCornerPosRes[2].x-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		CornerPTY[2] = CornPoint2[2].y = CornPoint[2].y = DrawParam.WndRect.bottom-((int)((m_BoxCornerPosRes[2].y-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
		CornerPTX[3] = CornPoint2[3].x = CornPoint[3].x = (int)((m_BoxCornerPosRes[3].x-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
		CornerPTY[3] = CornPoint2[3].y = CornPoint[3].y = DrawParam.WndRect.bottom-((int)((m_BoxCornerPosRes[3].y-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;		

		//是否在視窗畫面內
		RECT CorRect;
		JetAPI::PointsToRect(CornPoint, 4, CorRect);		
		if ( CorRect.left > DrawParam.WndRect.right ) { return; }
		if ( CorRect.top  > DrawParam.WndRect.bottom ) { return; }
		if ( CorRect.right < DrawParam.WndRect.left ) { return; }
		if ( CorRect.bottom < DrawParam.WndRect.top ) { return; }

		if ( DrawParam.WndRect.left > CorRect.right ) { return; }
		if ( DrawParam.WndRect.top  > CorRect.bottom ) { return; }
		if ( DrawParam.WndRect.right < CorRect.left ) { return; }
		if ( DrawParam.WndRect.bottom < CorRect.top ) { return; }

		::MoveToEx(hDC, CornPoint[0].x, CornPoint[0].y, NULL);
		::LineTo(hDC, CornPoint[1].x, CornPoint[1].y);
		::LineTo(hDC, CornPoint[2].x, CornPoint[2].y);
		::LineTo(hDC, CornPoint[3].x, CornPoint[3].y);
		::LineTo(hDC, CornPoint[0].x, CornPoint[0].y);

		JetAPI::PointsCenter(CornerPTX, CornerPTY, 4, CornerCPX, CornerCPY);
		JetAPI::RotateCornerPos(-ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

		Region.maxX = Region.minX = CornerPTX[0];	Region.maxY = Region.minY = CornerPTY[0];	
		if ( Region.minX > CornerPTX[1] ) { Region.minX = CornerPTX[1]; }
		if ( Region.minY > CornerPTY[1] ) { Region.minY = CornerPTY[1]; }
		if ( Region.maxX < CornerPTX[1] ) { Region.maxX = CornerPTX[1]; }
		if ( Region.maxY < CornerPTY[1] ) { Region.maxY = CornerPTY[1]; }

		if ( Region.minX > CornerPTX[2] ) { Region.minX = CornerPTX[2]; }
		if ( Region.minY > CornerPTY[2] ) { Region.minY = CornerPTY[2]; }
		if ( Region.maxX < CornerPTX[2] ) { Region.maxX = CornerPTX[2]; }
		if ( Region.maxY < CornerPTY[2] ) { Region.maxY = CornerPTY[2]; }

		if ( Region.minX > CornerPTX[3] ) { Region.minX = CornerPTX[3]; }
		if ( Region.minY > CornerPTY[3] ) { Region.minY = CornerPTY[3]; }
		if ( Region.maxX < CornerPTX[3] ) { Region.maxX = CornerPTX[3]; }
		if ( Region.maxY < CornerPTY[3] ) { Region.maxY = CornerPTY[3]; }
		
		switch ( ImageToward )
		{
		case BOX_TOWARD_UP:
			CornerPTX[0] = (Region.minX+Region.maxX)/2.0;
			CornerPTY[0] = Region.minY-DrawParam.TowardSize;
			CornerPTX[1] = CornerPTX[0]-DrawParam.TowardSize; 
			CornerPTX[2] = CornerPTX[0]+DrawParam.TowardSize; 
			CornerPTY[2] = CornerPTY[1] = Region.minY;
			CornerPTY[3] = CornerPTX[3] = 0.0;
			JetAPI::RotateCornerPos(ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

			if ( BOX_TOWARD_NULL != DrawParam.Toward )
			{
				CornPoint2[0].x = (int)(CornerPTX[0]+0.5);	CornPoint2[0].y = (int)(CornerPTY[0]+0.5);
				CornPoint2[1].x = (int)(CornerPTX[1]+0.5);	CornPoint2[1].y = (int)(CornerPTY[1]+0.5);
				CornPoint2[2].x = (int)(CornerPTX[2]+0.5);	CornPoint2[2].y = (int)(CornerPTY[2]+0.5);			
				CAOIBox::DrawTriangleLine(hDC, CornPoint2);
			}
			break;
		case BOX_TOWARD_LEFT:			
			CornerPTX[0] = Region.minX-DrawParam.TowardSize;
			CornerPTY[0] = (Region.minY+Region.maxY)/2.0;			
			CornerPTX[2] = CornerPTX[1] = Region.minX;
			CornerPTY[1] = CornerPTY[0]-DrawParam.TowardSize; 
			CornerPTY[2] = CornerPTY[0]+DrawParam.TowardSize; 
			CornerPTY[3] = CornerPTX[3] = 0.0;
			JetAPI::RotateCornerPos(ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

			if ( BOX_TOWARD_NULL != DrawParam.Toward )
			{
				CornPoint2[0].x = (int)(CornerPTX[0]+0.5);	CornPoint2[0].y = (int)(CornerPTY[0]+0.5);
				CornPoint2[1].x = (int)(CornerPTX[1]+0.5);	CornPoint2[1].y = (int)(CornerPTY[1]+0.5);
				CornPoint2[2].x = (int)(CornerPTX[2]+0.5);	CornPoint2[2].y = (int)(CornerPTY[2]+0.5);			
				CAOIBox::DrawTriangleLine(hDC, CornPoint2);
			}
			break;
		case BOX_TOWARD_DOWN:
			CornerPTX[0] = (Region.minX+Region.maxX)/2.0;
			CornerPTY[0] = Region.maxY+DrawParam.TowardSize;
			CornerPTX[1] = CornerPTX[0]-DrawParam.TowardSize; 
			CornerPTX[2] = CornerPTX[0]+DrawParam.TowardSize; 
			CornerPTY[2] = CornerPTY[1] = Region.maxY;
			CornerPTY[3] = CornerPTX[3] = 0.0;
			JetAPI::RotateCornerPos(ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

			if ( BOX_TOWARD_NULL != DrawParam.Toward )
			{
				CornPoint2[0].x = (int)(CornerPTX[0]+0.5);	CornPoint2[0].y = (int)(CornerPTY[0]+0.5);
				CornPoint2[1].x = (int)(CornerPTX[1]+0.5);	CornPoint2[1].y = (int)(CornerPTY[1]+0.5);
				CornPoint2[2].x = (int)(CornerPTX[2]+0.5);	CornPoint2[2].y = (int)(CornerPTY[2]+0.5);			
				CAOIBox::DrawTriangleLine(hDC, CornPoint2);
			}
			break;
		case BOX_TOWARD_RIGHT:
			CornerPTX[0] = Region.maxX+DrawParam.TowardSize;
			CornerPTY[0] = (Region.minY+Region.maxY)/2.0;			
			CornerPTX[2] = CornerPTX[1] = Region.maxX;
			CornerPTY[1] = CornerPTY[0]-DrawParam.TowardSize; 
			CornerPTY[2] = CornerPTY[0]+DrawParam.TowardSize; 
			CornerPTY[3] = CornerPTX[3] = 0.0;
			JetAPI::RotateCornerPos(ImageAngle, CornerCPX, CornerCPY, CornerPTX, CornerPTY);

			if ( BOX_TOWARD_NULL != DrawParam.Toward )
			{
				CornPoint2[0].x = (int)(CornerPTX[0]+0.5);	CornPoint2[0].y = (int)(CornerPTY[0]+0.5);
				CornPoint2[1].x = (int)(CornerPTX[1]+0.5);	CornPoint2[1].y = (int)(CornerPTY[1]+0.5);
				CornPoint2[2].x = (int)(CornerPTX[2]+0.5);	CornPoint2[2].y = (int)(CornerPTY[2]+0.5);			
				CAOIBox::DrawTriangleLine(hDC, CornPoint2);
			}
			break;
		}		

		if ( true == DrawParam.DrawResultText )
		{	
			if ( true == PolygonVisibled )
			{
			}
			if ( true == ResultTextVisibled ) 
			{
				if ( CheckDrawBoxText(Rect) == true )
				{
					int TextLen = m_BoxResultText.GetLength();
					JetAPI::CornerPtToRect(CornPoint2, Rect);
					int RectCpX = (Rect.left+Rect.right)/2;
					int RectCpY = (Rect.top+Rect.bottom)/2;
					int OffsetX = TextLen*2;
					::TextOut(hDC, RectCpX-OffsetX, RectCpY, m_BoxResultText, TextLen); 
				}
			}			
		}

		if ( true == DrawParam.DrawBoxIndex )
		{
			JetAPI::CornerPtToRect(CornPoint2, Rect);
			int TextLen = DrawParam.strBoxIndex.GetLength();
			int RectCpX = (Rect.left+Rect.right)/2;
			int RectCpY = (Rect.top+Rect.bottom)/2;
			if ( TextLen > 0 ) 
			{	::TextOut(hDC, Rect.left+4, Rect.top+4, DrawParam.strBoxIndex, TextLen); }
		}	

		//if ( DrawParam.DrawEditLine == true )
		//{
		//	if ( CAOIBox::GetBoxSelected() == true )//繪製選取編輯框	
		//	{	CAOIBox::DrawEditRect(hDC, CornPoint, DrawParam.EditLineSize);	}
		//}
	}	
}
//-------------------------------------------------------------------------------------//
void CAOIBox::DrawBoxResultKernel_2(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	RECT  Rect;		
	TREGION4D Region;	
	BOX_SHAPE_MODE BoxShapeMode = GetBoxShapeMode();
	const double   BoxShapeParam = GetBoxShapeParam();
	const double   BoxShapeParam2 = GetBoxShapeParam2();
	double BoxMinX=0, BoxMinY=0, BoxMaxX=0, BoxMaxY=0;		

	const int LineWidth = 1;
	bool  IsExceptionAngle = DrawParam.IsExceptionAngle;
	const double BoxSkew = GetBoxAngleSkew();
	const double ComAngle = DrawParam.ComponentAngle;//CAOIBox::GetBoxAttachedAngle();
	const double ImageAngle= JetAPI::MirrorXAxisAngle(ComAngle+BoxSkew);	
	const double AngleLabel = JetAPI::GetAngleLabel(ComAngle);
	const double DCAngle = ImageAngle+AngleLabel;//要轉DC的部分要注意
	const bool   ResultTextVisibled = GetBoxResultTextVisibled();	
	const bool   PolygonVisibled = GetBoxPolygonVisibled();

	if ( JetAPI::CheckIsExceptionAngle(BoxSkew) == true ) 
	{	IsExceptionAngle = true;	}

	CAOIBox::GetBoxRegionRes(Region);	
	const double RegionCpX=Region.GetCpX();
	const double RegionCpY=Region.GetCpY();

	Region.minX -= DrawParam.Extend.x;
	Region.maxX += DrawParam.Extend.x;
	Region.minY -= DrawParam.Extend.y;
	Region.maxY += DrawParam.Extend.y;
	//Rect.left    = (int)((Region.minX-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
	//Rect.right   = (int)((Region.maxX-DrawParam.ModelCP.x)/DrawParam.Zoom.x + DrawParam.ViewOffset.x) + DrawParam.WndCP.x + DrawParam.ViewCP.x;
	//Rect.top     = DrawParam.WndRect.bottom-((int)((Region.maxY-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
	//Rect.bottom  = DrawParam.WndRect.bottom-((int)((Region.minY-DrawParam.ModelCP.y)/DrawParam.Zoom.y + DrawParam.ViewOffset.y) + DrawParam.WndCP.y) + DrawParam.ViewCP.y;
	
	double L = DrawParam.RealToImageX(Region.minX);
	double R = DrawParam.RealToImageX(Region.maxX);
	double T = DrawParam.RealToImageY(Region.maxY);
	double B = DrawParam.RealToImageY(Region.minY);
	Rect.left   = (int)(L+0.5);
	Rect.right  = (int)(R+0.5);
	Rect.top    = (int)(T+0.5);
	Rect.bottom = (int)(B+0.5);

	//是否在視窗畫面內
	if ( Rect.left > DrawParam.WndRect.right ) { return; }
	if ( Rect.top  > DrawParam.WndRect.bottom ) { return; }
	if ( Rect.right < DrawParam.WndRect.left ) { return; }
	if ( Rect.bottom < DrawParam.WndRect.top ) { return; }

	if ( DrawParam.WndRect.left > Rect.right ) { return; }
	if ( DrawParam.WndRect.top  > Rect.bottom ) { return; }
	if ( DrawParam.WndRect.right < Rect.left ) { return; }
	if ( DrawParam.WndRect.bottom < Rect.top ) { return; }

	XFORM xFormNew;
	XFORM xFormOld;
	RECT  Rect2=Rect;
	int OldGraphicsMode = GM_COMPATIBLE;
	const double CpX = (Rect.left+Rect.right)*0.5;
	const double CpY = (Rect.top+Rect.bottom)*0.5;	
	const int nCpX = (int)(CpX+0.5);
	const int nCpY = (int)(CpY+0.5);
	if ( true == IsExceptionAngle )
	{	
		::OffsetRect(&Rect2, -nCpX, -nCpY);
		JetAPI::RotateDCTransform(DCAngle, CpX, CpY, xFormNew);		

		OldGraphicsMode = SetGraphicsMode(hDC, GM_ADVANCED);
		//int OldMapMode = SetMapMode(hDC, MM_LOENGLISH);//將DC的左上方起點改成左下方為起點
		GetWorldTransform(hDC, &xFormOld);
		SetWorldTransform(hDC, &xFormNew); 
	}
	switch ( DrawParam.ShapeMode )
	{
	case BOX_SHAPE_ROUND_RECT:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawRoundRectRect(hDC, Rect2, BoxShapeParam, DrawParam.hPenNull, DrawParam.hBrushMask);	}
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawRoundRectLine(hDC, Rect2, BoxShapeParam, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawRoundRectLine(hDC, Rect2, BoxShapeParam, DrawParam.Toward, DrawParam.TowardSize); }
		break;
	case BOX_SHAPE_ELLIPSE:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawEllipseRect(hDC, Rect2, DrawParam.hPenNull, DrawParam.hBrushMask); }
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawEllipseLine(hDC, Rect2, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawEllipseLine(hDC, Rect2, DrawParam.Toward, 0); }
		break;
	case BOX_SHAPE_CAPSULE:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawCapsuleRect(hDC, Rect2, DrawParam.hPenNull, DrawParam.hBrushMask); }
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawCapsuleLine(hDC, Rect2, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawCapsuleLine(hDC, Rect2, DrawParam.Toward, 0); }
		break;
	case BOX_SHAPE_BULLET:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawBulletRect(hDC, Rect2, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask); }
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawBulletLine(hDC, Rect2, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawBulletLine(hDC, Rect2, DrawParam.Toward, 0); }
		break;
	case BOX_SHAPE_HALF_ROUND_RECT:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawHalfRoundRectRect(hDC, Rect2, BoxShapeParam, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask);	}
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawHalfRoundRectLine(hDC, Rect2, BoxShapeParam, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawHalfRoundRectLine(hDC, Rect2, BoxShapeParam, DrawParam.Toward, DrawParam.TowardSize); }
		break;
	case BOX_SHAPE_T_SHAPE:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawTShapeRect(hDC, Rect2, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, DrawParam.hPenNull, DrawParam.hBrushMask);	}
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawTShapeRectLine(hDC, Rect2, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawTShapeRectLine(hDC, Rect2, BoxShapeParam, BoxShapeParam2, DrawParam.Toward, DrawParam.TowardSize); }		
		break;
	default:
		if ( true == DrawParam.FillRegion )
		{	CAOIBox::DrawRectangleRect(hDC, Rect2, DrawParam.hPenNull, DrawParam.hBrushMask); }
		if ( false == DrawParam.DrawTowardFeature )
		{	CAOIBox::DrawRectangleLine(hDC, Rect2, DrawParam.Toward, 0); }
		else
		{	CAOIBox::DrawRectangleLine(hDC, Rect2, DrawParam.Toward, DrawParam.TowardSize); }
		break;
	}

	if ( true == DrawParam.DrawResultText )
	{	
		if ( true == PolygonVisibled )
		{
			POINT ImgPt;			
			const size_t PtCnt=GetBoxPolygonPtCount();
			for ( size_t ii=0; ii<PtCnt; ii++ )
			{
				const TPOINT2D *BoxPt=GetBoxPolygonPt(ii, false);
				if ( NULL == BoxPt ) { continue; }
				if ( false == IsExceptionAngle )
				{
					ImgPt.x = DrawParam.RealToImageX(BoxPt->x);
					ImgPt.y = DrawParam.RealToImageY(BoxPt->y);
				}
				else
				{
					TPOINT2D TemPt=*BoxPt;					
					JetAPI::RotatePos(-ComAngle, RegionCpX, RegionCpY, TemPt);
					ImgPt.x = DrawParam.RealToImageX(TemPt.x);
					ImgPt.y = DrawParam.RealToImageY(TemPt.y);

					ImgPt.x -= nCpX; 
					ImgPt.y -= nCpY; 
				}	
				if ( 0 == ii )
				{	::MoveToEx(hDC, ImgPt.x, ImgPt.y, NULL);	}
				else
				{	::LineTo(hDC, ImgPt.x, ImgPt.y);	}
			}			
		}
		if ( true == ResultTextVisibled ) 
		{	
			if ( CheckDrawBoxText(Rect2) == true )
			{
				int TextLen = m_BoxResultText.GetLength();
				int RectCpX = (Rect2.left+Rect2.right)/2;
				int RectCpY = (Rect2.top+Rect2.bottom)/2;
				int OffsetX = TextLen*2;
				::TextOut(hDC, RectCpX-OffsetX, RectCpY, m_BoxResultText, TextLen);
			}
		}
	}		
	
	if ( true == DrawParam.DrawBoxIndex )
	{	
		int TextLen = DrawParam.strBoxIndex.GetLength();
		int RectCpX = (Rect.left+Rect.right)/2;
		int RectCpY = (Rect.top+Rect.bottom)/2;
		if ( TextLen > 0 ) 
		{	::TextOut(hDC, Rect.left+4, Rect.top+4, DrawParam.strBoxIndex, TextLen); }
	}

	//if ( true == DrawParam.DrawEditLine )
	//{	
	//	if ( CAOIBox::GetBoxSelected() == true )//繪製選取編輯框
	//	{	CAOIBox::DrawEditRect(hDC, Rect2, DrawParam.EditLineSize);	}
	//}
	
	if ( true == IsExceptionAngle )
	{
		//復歸座標系統
		SetWorldTransform(hDC, &xFormOld); 
		//::SetMapMode(hDC, OldMapMode); 
		::SetGraphicsMode(hDC, OldGraphicsMode);	
	}
}
//-------------------------------------------------------------------------------------//
void CAOIBox::DrawBoxResult(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	//return DrawBoxResultKernel_1(hDC, DrawParam);
	return DrawBoxResultKernel_2(hDC, DrawParam);	
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::ApplyBox(const CAOIBox *RefBoxPtr)
{
	if ( NULL == RefBoxPtr ) { return false; }

	//m_BoxToward = RefBoxPtr->m_BoxToward;
	//m_BoxIndex = RefBoxPtr->m_BoxIndex;
	m_BoxShapeMode = RefBoxPtr->m_BoxShapeMode;
	m_BoxShapeParam = RefBoxPtr->m_BoxShapeParam;
	m_BoxShapeParam2 = RefBoxPtr->m_BoxShapeParam2;
	m_BoxMaskEraseMode = RefBoxPtr->m_BoxMaskEraseMode;

	//m_BoxEnabled = RefBoxPtr->m_BoxEnabled;
	//m_BoxActived = = RefBoxPtr->m_BoxActived;
	//m_BoxSelected = RefBoxPtr->m_BoxSelected;
	m_BoxVisibled = RefBoxPtr->m_BoxVisibled;
	m_BoxEditabled = RefBoxPtr->m_BoxEditabled;
	/*
	m_BoxAngle = RefBoxPtr->m_BoxAngle;
	m_BoxAngleRes = RefBoxPtr->m_BoxAngleRes;
	m_BoxAngleSkew = RefBoxPtr->m_BoxAngleSkew;
	m_BoxPos = RefBoxPtr->m_BoxPos;	
	m_BoxSize = RefBoxPtr->m_BoxSize;

	m_BoxPosRes = RefBoxPtr->m_BoxPosRes;	
	m_BoxSizeRes = RefBoxPtr->m_BoxSizeRes;

	m_BoxCornerPos[0] = RefBoxPtr->m_BoxCornerPos[0];	
	m_BoxCornerPos[1] = RefBoxPtr->m_BoxCornerPos[1];	
	m_BoxCornerPos[2] = RefBoxPtr->m_BoxCornerPos[2];	
	m_BoxCornerPos[3] = RefBoxPtr->m_BoxCornerPos[3];	
	m_BoxCornerPosRes[0] = RefBoxPtr->m_BoxCornerPosRes[0];
	m_BoxCornerPosRes[1] = RefBoxPtr->m_BoxCornerPosRes[1];
	m_BoxCornerPosRes[2] = RefBoxPtr->m_BoxCornerPosRes[2];
	m_BoxCornerPosRes[3] = RefBoxPtr->m_BoxCornerPosRes[3];
	UpdateBoxAttachedPosCad();//更新框的零件座標-CAD
	UpdateBoxAttachedPosStage();//更新框的零件座標-機台
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBox::SynchronousBox(const CAOIBox *RefBoxPtr)
{
	if ( NULL == RefBoxPtr ) { return false; }

	m_BoxToward = RefBoxPtr->m_BoxToward;
	//m_BoxIndex = RefBoxPtr->m_BoxIndex;
	m_BoxShapeMode = RefBoxPtr->m_BoxShapeMode;
	m_BoxShapeParam = RefBoxPtr->m_BoxShapeParam;
	m_BoxShapeParam2 = RefBoxPtr->m_BoxShapeParam2;
	m_BoxMaskEraseMode = RefBoxPtr->m_BoxMaskEraseMode;

	m_BoxEnabled = RefBoxPtr->m_BoxEnabled;
	m_BoxActived = RefBoxPtr->m_BoxActived;
	m_BoxSelected = RefBoxPtr->m_BoxSelected;
	m_BoxVisibled = RefBoxPtr->m_BoxVisibled;
	m_BoxEditabled = RefBoxPtr->m_BoxEditabled;
	/*
	m_BoxAngle = RefBoxPtr->m_BoxAngle;
	m_BoxAngleRes = RefBoxPtr->m_BoxAngleRes;
	m_BoxAngleSkew = RefBoxPtr->m_BoxAngleSkew;

	m_BoxPos = RefBoxPtr->m_BoxPos;	
	m_BoxSize = RefBoxPtr->m_BoxSize;

	m_BoxPosRes = RefBoxPtr->m_BoxPosRes;	
	m_BoxSizeRes = RefBoxPtr->m_BoxSizeRes;

	m_BoxCornerPos[0] = RefBoxPtr->m_BoxCornerPos[0];	
	m_BoxCornerPos[1] = RefBoxPtr->m_BoxCornerPos[1];	
	m_BoxCornerPos[2] = RefBoxPtr->m_BoxCornerPos[2];	
	m_BoxCornerPos[3] = RefBoxPtr->m_BoxCornerPos[3];	
	m_BoxCornerPosRes[0] = RefBoxPtr->m_BoxCornerPosRes[0];
	m_BoxCornerPosRes[1] = RefBoxPtr->m_BoxCornerPosRes[1];
	m_BoxCornerPosRes[2] = RefBoxPtr->m_BoxCornerPosRes[2];
	m_BoxCornerPosRes[3] = RefBoxPtr->m_BoxCornerPosRes[3];
	UpdateBoxAttachedPosCad();//更新框的零件座標-CAD
	UpdateBoxAttachedPosStage();//更新框的零件座標-機台
	*/
	return true;
}
//-------------------------------------------------------------------------------------//