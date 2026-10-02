// AOISlice.h: interface for the CAOISlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOISLICE_H__FED483CF_3E3B_4568_82A8_A957AE78A73E__INCLUDED_)
#define AFX_AOISLICE_H__FED483CF_3E3B_4568_82A8_A957AE78A73E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//相機的片段影像
//走停式: 每個FOV會有1個片段影像-等於相機影像
//掃描式: 每個FOV會有多個片段影像-將相機影像畫分個小部位
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
//-------------------------------------------------------------------------------------//
class CAOIFov;
//-------------------------------------------------------------------------------------//
enum SLICE_IMAGE_TYPE
{
	SLICE_IMAGE_GRAY  = 0,
	SLICE_IMAGE_BAYER = 1,
	SLICE_IMAGE_COLOR = 2
};
//-------------------------------------------------------------------------------------//
enum SLICE_FILL_STATE
{
	SLICE_FILL_NONE  = 0,//尚未填滿
	SLICE_FILL_DOING = 1,//填滿中	
	SLICE_FILL_DONE  = 2,//填滿完畢	
	SLICE_FILL_CLEAR = 9 //清除填滿
};
//-------------------------------------------------------------------------------------//
class CAOISlice : public CAOIObj  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOISlice)
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csSlice;//同步機制-關鍵區間
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	static void                InitialSliceLock();//初始化相機內區域的關鍵區間
	static void                DeleteSliceLock(); //刪除相機內區域的關鍵區間
	static void                LockSlice();        //進入相機內區域的關鍵區間
	static void                UnlockSlice();      //離開相機內區域的關鍵區間
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	unsigned int               m_SliceIndex;//相機內區域的引數編號
	//---------------------------------------------------------------------------------//
	unsigned int               m_SliceFovIdx;//相機內區域的視野編號
	CAOIFov*                   m_SliceFovPtr;//相機內區域的視野指標
	//---------------------------------------------------------------------------------//
	unsigned int               m_SliceFrameIdx;//相機內區域的Frame編號
	//---------------------------------------------------------------------------------//
	CAMERA_ID                  m_SliceCameraID;//相機內區域的相機編號
	LIGHT_MODE                 m_SliceLightMode;//相機內區域的燈源編號
	LIGHT_3D_CAST_ID           m_SliceLight3DCastID;//相機內區域的樣板投光編號
	double                     m_SliceGainValue;//相機內區域的亮度增益比例
	//---------------------------------------------------------------------------------//
	unsigned int               m_SliceCameraFrameIdx;//相機內區域的相機圖像引數編號		
	RECT                       m_SliceCameraFrameRect;//相機內區域在相機圖像的區域	
	//---------------------------------------------------------------------------------//
	TPOINT2D                   m_SliceCadPos;//相機內區域在Cad的位置	
	TPOINT3D                   m_SliceStagePos;//相機內區域在Stage的位置	
	//---------------------------------------------------------------------------------//
	//Field Image
	SLICE_IMAGE_TYPE           m_SliceImageType;//相機內區域的影像格式
	IMAGE_SIZE                 m_SliceImageW;//相機內區域的影像寬度
	IMAGE_SIZE                 m_SliceImageH;//相機內區域的影像高度
	IMAGE_SIZE                 m_SliceImageStep;//相機內區域的影像步長
	IMAGE_SIZE                 m_SliceImageBitCount;//相機內區域的影像位元數
	IMAGE_PTR                  m_SliceImagePtr;//相機內區域的影像指標
	//---------------------------------------------------------------------------------//
	SLICE_FILL_STATE           m_SliceFillState;//相機內區域填圖狀態
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitSlice();
	void                       InitialSlice();
	void                       CloneSlice(const CAOISlice &slice);
	//---------------------------------------------------------------------------------//
	void                       CloneSliceImage(const CAOISlice &slice);//複製相機內區域影像記憶體區塊	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOISlice();
	CAOISlice(const CAOISlice &slice);
	virtual ~CAOISlice();
	CAOISlice& operator=(const CAOISlice &slice);
	//---------------------------------------------------------------------------------//
	CAOISlice*                 CloneSliceObj() const;//建立且複製一個相機內區域
	//---------------------------------------------------------------------------------//	
	void                       SetSliceIndex(unsigned int value) { m_SliceIndex = value; }
	unsigned int               GetSliceIndex() const { return m_SliceIndex; }
	//---------------------------------------------------------------------------------//	
	void                       SetSliceFovIndex(unsigned int value) { m_SliceFovIdx = value; }
	unsigned int               GetSliceFovIndex() const { return m_SliceFovIdx; }
	//---------------------------------------------------------------------------------//	
	void                       SetSliceFovPtr(CAOIFov* value) { m_SliceFovPtr = value; }
	CAOIFov*                   GetSliceFovPtr() const { return m_SliceFovPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetSliceFrameIdx(unsigned int value) { m_SliceFrameIdx = value; }
	unsigned int               GetSliceFrameIdx() const { return m_SliceFrameIdx; }
	//---------------------------------------------------------------------------------//	
	void                       SetSliceCameraID(CAMERA_ID value) { m_SliceCameraID = value; }
	CAMERA_ID                  GetSliceCameraID() const { return m_SliceCameraID; }
	//---------------------------------------------------------------------------------//
	void                       SetSliceLightMode(LIGHT_MODE value) { m_SliceLightMode = value; }
	LIGHT_MODE                 GetSliceLightMode() const { return m_SliceLightMode; }	
	//---------------------------------------------------------------------------------//
	void                       SetSliceLight3DCastID(LIGHT_3D_CAST_ID value) { m_SliceLight3DCastID = value; }
	LIGHT_3D_CAST_ID           GetSliceLight3DCastID() const { return m_SliceLight3DCastID; }	
	//---------------------------------------------------------------------------------//
	//相機內區域的亮度增益比例
	void                       SetSliceGainValue(double value) { m_SliceGainValue = value; }
	double                     GetSliceGainValue() const { return m_SliceGainValue; }	
	//---------------------------------------------------------------------------------//
	void                       SetSliceCameraFrameRect(RECT value) { m_SliceCameraFrameRect = value; }
	RECT                       GetSliceCameraFrameRect() const { return m_SliceCameraFrameRect; }

	void                       SetSliceCameraFrameIndex(unsigned int value) { m_SliceCameraFrameIdx = value; }
	unsigned int               GetSliceCameraFrameIndex() const { return m_SliceCameraFrameIdx; }
	//---------------------------------------------------------------------------------//
	void                       SetSliceCadPos(const TPOINT2D &value) { m_SliceCadPos = value; }
	TPOINT2D                   GetSliceCadPos() const { return m_SliceCadPos; }

	void                       SetSliceCadPosX(double value) { m_SliceCadPos.x = value; }
	double                     GetSliceCadPosX() const { return m_SliceCadPos.x; }

	void                       SetSliceCadPosY(double value) { m_SliceCadPos.y = value; }
	double                     GetSliceCadPosY() const { return m_SliceCadPos.y; }
	//---------------------------------------------------------------------------------//	
	void                       SetSliceStagePos(const TPOINT3D &value) { m_SliceStagePos = value; }
	TPOINT3D                   GetSliceStagePos() const { return m_SliceStagePos; }

	void                       SetSliceStagePosX(double value) { m_SliceStagePos.x = value; }
	double                     GetSliceStagePosX() const { return m_SliceStagePos.x; }

	void                       SetSliceStagePosY(double value) { m_SliceStagePos.y = value; }
	double                     GetSliceStagePosY() const { return m_SliceStagePos.y; }

	void                       SetSliceStagePosZ(double value) { m_SliceStagePos.z = value; }
	double                     GetSliceStagePosZ() const { return m_SliceStagePos.z; }
	//---------------------------------------------------------------------------------//	    
	void                       SetSliceImageType(SLICE_IMAGE_TYPE value) { m_SliceImageType = value; }
	SLICE_IMAGE_TYPE           GetSliceImageType() const { return m_SliceImageType; }
	//---------------------------------------------------------------------------------//
	void                       ClearSliceImageBuffer();//清除相機內區域影像記憶體區塊
	void                       ReleaseSliceImageBuffer();//釋放相機內區域影像記憶體區塊
	bool                       SetSliceImageBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool Clone);//設定區塊圖的影像資料
	bool                       GetSliceImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);//取得區塊圖的影像資料
	bool                       CloneSliceImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);//取得區塊圖的影像資料
	//---------------------------------------------------------------------------------//
	void                       SetSliceFillState(SLICE_FILL_STATE value) { m_SliceFillState = value; }
	SLICE_FILL_STATE           GetSliceFillState() const { return m_SliceFillState; }
	//---------------------------------------------------------------------------------//	
	bool                       CheckSliceCameaGrabbed();//確認畫面的相機有取到影像
	bool                       ExecSliceFill();//填滿畫面影像
	bool                       ExecSliceGainImage();//增益畫面影像
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOISLICE_H__FED483CF_3E3B_4568_82A8_A957AE78A73E__INCLUDED_)
