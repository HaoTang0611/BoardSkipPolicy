// AOIFrame.h: interface for the CAOIFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIFRAME_H__E51917DE_7526_4618_B0AF_E5A25680973B__INCLUDED_)
#define AFX_AOIFRAME_H__E51917DE_7526_4618_B0AF_E5A25680973B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//影像畫面, 可劃分灰階、原圖、彩色與3D畫面
//-------------------------------------------------------------------------------------//
#include <vector>
#include "AOIObj.h"
#include "AOISlice.h"
//-------------------------------------------------------------------------------------//
enum FRAME_MERGE_STATE   //影像合併狀態
{
	FRAME_MERGE_NONE   = 0,  //尚未合併影像
	FRAME_MERGE_DOING  = 1,  //合併中	
	FRAME_MERGE_DONE   = 2,  //合併完畢	
	FRAME_MERGE_CLEAR  = 9,  //合併清除
	FRAME_MERGE_RETURN
};
//-------------------------------------------------------------------------------------//
enum FRAME_CALC_STATE   //影像計算狀態
{
	FRAME_CALC_NONE   = 0,  //尚未計算
	FRAME_CALC_DOING  = 1,  //計算中
	FRAME_CALC_DONE   = 2,  //計算完畢		
	FRAME_CALC_RETURN
};
//-------------------------------------------------------------------------------------//
class CAOIFov;
class CAOIField;
//-------------------------------------------------------------------------------------//
class CAOIFrame: public CAOIObj  
{	
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIFrame)
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csFrame;//同步機制-關鍵區間
	//---------------------------------------------------------------------------------//
	static size_t              m_FrameShareMaskSize;//Frame共享遮罩大小
	static size_t              m_FrameShareImageSize;//Frame共享影像大小
	static size_t              m_FrameShareSpaceSize;//Frame共享空間大小
	static MASK_PTR            m_FrameShareMaskPtr;//Frame共享空間遮罩
	static IMAGE_PTR           m_FrameShareImagePtr;//Frame共享影像記憶體區塊
	static SPACE_PTR           m_FrameShareSpacePtr;//Frame共享空間記憶體區塊
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	static void                InitialFrameLock(); //初始化影像的關鍵區間
	static void                DeleteFrameLock(); //刪除影像的關鍵區間
	static void                LockFrame();        //進入影像的關鍵區間
	static void                UnlockFrame();      //離開影像的關鍵區間
	//---------------------------------------------------------------------------------//
	static bool                ReleaseFrameShareBuffer();//釋放影像的共享記憶體
	static bool                AllocateFrameShareBuffer();//建立影像的共享記憶體
	static bool                AllocateFrameShareBuffer(size_t szImage, size_t szSpace);//建立影像的共享記憶體	
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//	
	FRAME_TYPE                 m_FrameType;//Frame影像樣式	
	unsigned int               m_FrameIndex;//Frame引數編號
	unsigned int               m_FrameUniqueID;//Frame唯一碼	
	unsigned int               m_FrameHeightID;//Frame高度編號
	unsigned int               m_FrameIndexOffline;//Frame引數編號-Offline		
	SLICE_FUNC_MODE            m_FrameSliceFuncMode;//Frame的影像功能模式-3D
	//---------------------------------------------------------------------------------//	
	unsigned int               m_FrameFovIdx;//Frame所屬的FOV引數編號
	CAOIFov*                   m_FrameFovPtr;//Frame所屬的視野指標
	//---------------------------------------------------------------------------------//
	unsigned int               m_FrameFieldIdx;//Frame所屬的區塊引數編號
	CAOIField*                 m_FrameFieldPtr;//Frame所屬的區塊指標
	//---------------------------------------------------------------------------------//
	CAMERA_ID                  m_FrameCameraID;//Frame的相機編號
	LIGHT_MODE                 m_FrameLightMode;//Frame的燈源模式
	DISTRICT_ID                m_FrameDistrictID;//Frame的多段編號
	bool                       m_FrameLinkPointer;//連結指標, 如果是的話不要刪除
	size_t                     m_FrameRgnCount;//Frame的區域數量
		//---------------------------------------------------------------------------------//
	int                        m_FrameTempInt;//Frame暫存編號
	//---------------------------------------------------------------------------------//
	CString                    m_FrameFileName;//Frame的檔案名稱
	CString                    m_FrameFileFolder;//Frame的檔案資料夾
	CString                    m_FrameFileNameOffline;//Frame的檔案名稱-Offline
	CString                    m_FrameFileFolderOffline;//Frame的檔案資料夾-Offline
	double                     m_FrameMergeTime;//Frame合併花費時間
	FRAME_CALC_STATE           m_FrameCalcState;//影像計算狀態
	FRAME_MERGE_STATE          m_FrameMergeState;//影像合併狀態
	bool                       m_FrameImageSaved;//Frame影像是否存檔過了
	//---------------------------------------------------------------------------------//
	bool                       m_FrameUseShareMaskBuf;//Frame使用共享遮罩記憶體
	bool                       m_FrameUseShareImageBuf;//Frame使用共享影像記憶體
	bool                       m_FrameUseShareSpaceBuf;//Frame使用共享空間記憶體
	//---------------------------------------------------------------------------------//
	bool                       m_FrameImageValid;//Frame影像有效的	
	unsigned int               m_FrameImageIndex;//Frame影像的序號
	IMAGE_SIZE                 m_FrameImageW;//Frame影像寬度
	IMAGE_SIZE                 m_FrameImageH;//Frame影像長度
	IMAGE_SIZE                 m_FrameImageStep;//Frame影像步長
	IMAGE_SIZE                 m_FrameImageBitCount;//Frame影像位元數	
	MASK_PTR                   m_FrameMaskPtr;//Frame空間遮罩
	IMAGE_PTR                  m_FrameImagePtr;//Frame影像記憶體區塊
	SPACE_PTR                  m_FrameSpacePtr;//Frame空間記憶體區塊
	BAYER_PATTERN_MODE         m_FrameBayerPattern;//Frame的Bayer樣板
	float                      m_FraemSpaceOffset;//Frame高度偏差值
	//---------------------------------------------------------------------------------//	
	double                     m_FrameResolutionX;//Frame影像解析度-X
	double                     m_FrameResolutionY;//Frame影像解析度-Y
	//---------------------------------------------------------------------------------//		
	double                     m_FrameSaturationRed;//Frame飽和度調整-紅色
	double                     m_FrameSaturationGreen;//Frame飽和度調整-綠色
	double                     m_FrameSaturationBlue;//Frame飽和度調整-藍色
	//---------------------------------------------------------------------------------//		
	std::vector<CAOISlice*>    m_FrameSlicePtrList;//影像畫面內的區域指標列表		
	//---------------------------------------------------------------------------------//
	bool                       m_FrameSaveRawImageDone;//影像畫面原圖以儲存
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitFrame();
	void                       InitialFrame();
	void                       CloneFrame(const CAOIFrame &frame);
	//---------------------------------------------------------------------------------//	
	bool                       CloneFrameBuffer(const CAOIFrame &frame);	
	//---------------------------------------------------------------------------------//
	bool                       ReleaseCastParam(TCastParam &CastParam);//釋放投光參數
	bool                       BuildCastParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, int ImgPtrCount, int SliceFuncMode, IMAGE_PTR ImgPtr[], LIGHT_3D_CAST_ID CastID, TCastParam &CastParam);//建立投光參數
	bool                       BuildCastParam2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, int ImgPtrCount, int SliceFuncMode, IMAGE_PTR ImgPtr[], LIGHT_3D_CAST_ID CastID, TCastParam &CastParam);//建立投光參數
	//---------------------------------------------------------------------------------//
	bool                       ExecFrameMergeSlice();//影像合併區域影像-灰階
	bool                       ExecFrameMergeSlice_Gray();//影像合併區域影像-灰階
	bool                       ExecFrameMergeSlice_Bayer();//影像合併區域影像-彩色
	bool                       ExecFrameMergeSlice_Color();//影像合併區域影像-彩色	
	bool                       ExecFrameMergeSlice_Color03();//影像合併區域影像-彩色	-RGBx1
	bool                       ExecFrameMergeSlice_Color06();//影像合併區域影像-彩色	-RGBx2
	bool                       ExecFrameMergeSlice_Color09();//影像合併區域影像-彩色-RGBx3
	bool                       ExecFrameMergeSlice_Color12();//影像合併區域影像-彩色	-RGBx4
	bool                       ExecFrameMergeSlice_Space();//影像合併區域影像-3D資料
	bool                       ExecFrameMergeSlice_Space_Combine();//影像合併區域影像-3D資料
	bool                       ExecFrameMergeSlice_Space_Separate();//影像合併區域影像-3D資料
	bool                       ExecFrameMergeSlice_SpaceEnd();//影像合併區域影像-3D資料
	//---------------------------------------------------------------------------------//
	bool                       ExecFrameMergePatch();//影像合併區域影像-灰階
	bool                       ExecFrameMergePatch_Gray();//影像合併區域影像-灰階
	bool                       ExecFrameMergePatch_Bayer();//影像合併區域影像-彩色
	bool                       ExecFrameMergePatch_Color();//影像合併區域影像-彩色	
	bool                       ExecFrameMergePatch_Space();//影像合併區域影像-3D資料
	bool                       ExecFrameMergePatch_SpaceEnd();//影像合併區域影像-3D資料
	//---------------------------------------------------------------------------------//	
	bool                       ReSortFrameSliceList_Kernel(std::vector<CAOISlice*> &List);//重新排序Frame影像列表
	//---------------------------------------------------------------------------------//	
	bool                       ExecFrameSaveRawImage();//影像儲存原始圖檔
	//---------------------------------------------------------------------------------//
public:
	CAOIFrame();
	CAOIFrame(const CAOIFrame &frame);
	virtual ~CAOIFrame();
	CAOIFrame& operator=(const CAOIFrame &frame);
	//---------------------------------------------------------------------------------//
	CAOIFrame*                 CloneFrameObj() const;//建立且複製一個影像
	//---------------------------------------------------------------------------------//	
	bool                       WriteFrameFile(CAOIFileIO &FileIO);//儲存區域影像檔案
	bool                       ReadFrameFile(CAOIFileIO &FileIO);//載入區域影像檔案
	//---------------------------------------------------------------------------------//	
	//Frame影像樣式		
	void                       SetFrameType(FRAME_TYPE value) { m_FrameType = value; }
	FRAME_TYPE                 GetFrameType() const { return m_FrameType; }
	//---------------------------------------------------------------------------------//
	//Frame引數編號
	void                       SetFrameIndex(unsigned int value) { m_FrameIndex = value; }
	unsigned int               GetFrameIndex() const { return m_FrameIndex; }
	//---------------------------------------------------------------------------------//
	//Frame唯一碼
	void                       SetFrameUniqueID(unsigned int value) { m_FrameUniqueID = value; }
	unsigned int               GetFrameUniqueID() const { return m_FrameUniqueID; }
	//---------------------------------------------------------------------------------//	
	//Frame高度編號
	void                       SetFrameHeightID(unsigned int value) { m_FrameHeightID = value; }
	unsigned int               GetFrameHeightID() const { return m_FrameHeightID; }
	//---------------------------------------------------------------------------------//
	//Frame引數編號-Offline
	void                       SetFrameIndexOffline(unsigned int value) { m_FrameIndexOffline = value; }
	unsigned int               GetFrameIndexOffline() const { return m_FrameIndexOffline; }
	//---------------------------------------------------------------------------------//
	//Frame的影像功能模式-3D
	void                       SetFrameSliceFuncMode(SLICE_FUNC_MODE value) { m_FrameSliceFuncMode = value; }
	SLICE_FUNC_MODE            GetFrameSliceFuncMode() const { return m_FrameSliceFuncMode; }
	//---------------------------------------------------------------------------------//
	//Frame所屬的FOV引數編號
	void                       SetFrameFovIndex(unsigned int value) { m_FrameFovIdx = value; }
	unsigned int               GetFrameFovIndex() const { return m_FrameFovIdx; }
	//---------------------------------------------------------------------------------//	
	//Frame所屬的視野指標
	void                       SetFrameFovPtr(CAOIFov* value) { m_FrameFovPtr = value; }
	CAOIFov*                   GetFrameFovPtr() const { return m_FrameFovPtr; }
	//---------------------------------------------------------------------------------//	
	//Frame所屬的區塊引數編號
	void                       SetFrameFieldIndex(unsigned int value) { m_FrameFieldIdx = value; }
	unsigned int               GetFrameFieldIndex() const { return m_FrameFieldIdx; }
	//---------------------------------------------------------------------------------//	
	//Frame所屬的區塊指標
	void                       SetFrameFieldPtr(CAOIField* value) { m_FrameFieldPtr = value; }
	CAOIField*                 GetFrameFieldPtr() const { return m_FrameFieldPtr; }
	//---------------------------------------------------------------------------------//	
	//Frame的相機編號
	void                       SetFrameCameraID(CAMERA_ID value) { m_FrameCameraID = value; }
	CAMERA_ID                  GetFrameCameraID() const { return m_FrameCameraID; }
	//---------------------------------------------------------------------------------//
	//Frame的相機編號
	void                       SetFrameBayerPattern(BAYER_PATTERN_MODE value) { m_FrameBayerPattern = value; }
	BAYER_PATTERN_MODE         GetFrameBayerPattern() const { return m_FrameBayerPattern; }
	//---------------------------------------------------------------------------------//
	//Frame高度偏差值
	void                       SetFraemSpaceOffset(float value) { m_FraemSpaceOffset = value; }
	float                      GetFraemSpaceOffset() const { return m_FraemSpaceOffset; }
	//---------------------------------------------------------------------------------//
	//Frame的燈源模式
	void                       SetFrameLightMode(LIGHT_MODE value) { m_FrameLightMode = value; }
	LIGHT_MODE                 GetFrameLightMode() const { return m_FrameLightMode; }
	//---------------------------------------------------------------------------------//
	//Frame的多段編號
	void                       SetFrameDistrictID(DISTRICT_ID value) { m_FrameDistrictID = value; }
	DISTRICT_ID                GetFrameDistrictID() const { return m_FrameDistrictID; }
	//---------------------------------------------------------------------------------//
	//Frame的指標連動
	void                       SetFrameLinkPointer(bool value) { m_FrameLinkPointer = value; }
	bool                       GetFrameLinkPointer() const { return m_FrameLinkPointer; }
	//---------------------------------------------------------------------------------//
	//Frame暫存編號
	void                       SetFrameTempInt(int value) { m_FrameTempInt = value; }
	int                        GetFrameTempInt() const { return m_FrameTempInt; }
	//---------------------------------------------------------------------------------//	
	//Frame的區域數量
	void                       ResetFrameRgnCount();
	void                       IncreaseFrameRgnCount();
	void                       SetFrameRgnCount(size_t value) { m_FrameRgnCount = value; }
	size_t                     GetFrameRgnCount() const { return m_FrameRgnCount; }	
	//---------------------------------------------------------------------------------//
	//Frame的圖檔名稱
	void                       SetFrameFileName(LPCTSTR  value) { m_FrameFileName = value; }
	LPCTSTR                    GetFrameFileName() const { return m_FrameFileName; }
	//---------------------------------------------------------------------------------//
	//Frame的圖檔資料夾
	void                       SetFrameFileFolder(LPCTSTR  value) { m_FrameFileFolder = value; }
	LPCTSTR                    GetFrameFileFolder() const { return m_FrameFileFolder; }
	//---------------------------------------------------------------------------------//
	//Frame的圖檔名稱-Offline
	void                       SetFrameFileNameOffline(LPCTSTR  value) { m_FrameFileNameOffline = value; }
	LPCTSTR                    GetFrameFileNameOffline() const { return m_FrameFileNameOffline; }
	//---------------------------------------------------------------------------------//
	//Frame的圖檔資料夾-Offline
	void                       SetFrameFileFolderOffline(LPCTSTR  value) { m_FrameFileFolderOffline = value; }
	LPCTSTR                    GetFrameFileFolderOffline() const { return m_FrameFileFolderOffline; }
	//---------------------------------------------------------------------------------//
	//Frame影像解析度-X
	void                       SetFrameResolutionX(double value) { m_FrameResolutionX = value; }
	double                     GetFrameResolutionX() const { return m_FrameResolutionX; }
	//---------------------------------------------------------------------------------//
	//Frame影像解析度-Y
	void                       SetFrameResolutionY(double value) { m_FrameResolutionY = value; }
	double                     GetFrameResolutionY() const { return m_FrameResolutionY; }
	//---------------------------------------------------------------------------------//
	//Frame飽和度調整-紅色
	void                       SetFrameSaturationRed(double value) { m_FrameSaturationRed = value; }
	double                     GetFrameSaturationRed() const { return m_FrameSaturationRed; }
	//---------------------------------------------------------------------------------//
	//Frame飽和度調整-綠色
	void                       SetFrameSaturationGreen(double value) { m_FrameSaturationGreen = value; }
	double                     GetFrameSaturationGreen() const { return m_FrameSaturationGreen; }
	//---------------------------------------------------------------------------------//
	//Frame飽和度調整-藍色
	void                       SetFrameSaturationBlue(double value) { m_FrameSaturationBlue = value; }
	double                     GetFrameSaturationBlue() const { return m_FrameSaturationBlue; }
	//---------------------------------------------------------------------------------//
	//Frame使用共享遮罩記憶體
	void                       SetFrameUseShareMaskBuf(bool value) { m_FrameUseShareMaskBuf = value; }
	bool                       GetFrameUseShareMaskBuf() const { return m_FrameUseShareMaskBuf; }
	//---------------------------------------------------------------------------------//	
	//Frame使用共享影像記憶體
	void                       SetFrameUseShareImageBuf(bool value) { m_FrameUseShareImageBuf = value; }
	bool                       GetFrameUseShareImageBuf() const { return m_FrameUseShareImageBuf; }
	//---------------------------------------------------------------------------------//	
	//Frame使用共享空間記憶體
	void                       SetFrameUseShareSpaceBuf(bool value) { m_FrameUseShareSpaceBuf = value; }
	bool                       GetFrameUseShareSpaceBuf() const { return m_FrameUseShareSpaceBuf; }	
	//---------------------------------------------------------------------------------//
	//Frame影像有效的
	void                       SetFrameImageValid(bool value) { m_FrameImageValid = value; }
	bool                       GetFrameImageValid() const { return m_FrameImageValid; }
	//---------------------------------------------------------------------------------//	
	//Frame影像的序號
	void                       SetFrameImageIndex(unsigned int value) { m_FrameImageIndex = value; }
	unsigned int               GetFrameImageIndex() const { return m_FrameImageIndex; }
	//---------------------------------------------------------------------------------//
	//Frame影像寬度
	void                       SetFrameImageW(IMAGE_SIZE value) { m_FrameImageW = value; }
	IMAGE_SIZE                 GetFrameImageW() const { return m_FrameImageW; }
	//---------------------------------------------------------------------------------//
	//Frame影像長度
	void                       SetFrameImageH(IMAGE_SIZE value) { m_FrameImageH = value; }
	IMAGE_SIZE                 GetFrameImageH() const { return m_FrameImageH; }
	//---------------------------------------------------------------------------------//
	//Frame影像每條的位元組數量
	void                       SetFrameImageStep(IMAGE_SIZE value) { m_FrameImageStep = value; }
	IMAGE_SIZE                 GetFrameImageStep() const { return m_FrameImageStep; }
	//---------------------------------------------------------------------------------//	
	//Frame影像位元數	
	IMAGE_SIZE                 GetFrameImageBitCount() const { return m_FrameImageBitCount; }
	//---------------------------------------------------------------------------------//	
	//Frame影像記憶體區塊	
	bool                       GetFrameUniFrame(TUNI_FRAME &UniFrame) const;
	//---------------------------------------------------------------------------------//	
	//Frame影像記憶體區塊	
	IMAGE_PTR                  GetFrameImagePtr() { return m_FrameImagePtr; }
	void                       SetFrameImagePtr(IMAGE_PTR ImagePtr) { m_FrameImagePtr=ImagePtr; }	
	bool                       SetFrameImagePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool bClone);
	bool                       GetFrameImagePtr(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr) const;	
	//---------------------------------------------------------------------------------//	
	//Frame影像記憶體區塊		
	MASK_PTR                   GetFrameMaskPtr() { return m_FrameMaskPtr; }
	SPACE_PTR                  GetFrameSpacePtr() { return m_FrameSpacePtr; }
	bool                       SetFrameSpacePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, bool bClone);
	bool                       SetFrameSpacePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, bool bClone);
	bool                       GetFrameSpacePtr(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr) const;
	//---------------------------------------------------------------------------------//	
	bool                       ReSortFrameSliceList();//重新排序Frame影像列表
	bool                       AddFrameSlicePtr(CAOISlice *Ptr);//增加影像的區域指標
	size_t                     GetFrameSliceCount() const; //取得影像的區域數量
	CAOISlice*                 GetFrameSlicePtr(size_t index, bool check);//取得影像的區域指標	
	void                       RemoveFrameAllSlices();//移除影像的所有區域
	bool                       CheckFrameSliceFilled();//確認影像畫面內的區域都取到影像
	//---------------------------------------------------------------------------------//			
	void                       SetFrameMergeTime(double value) { m_FrameMergeTime = value; }//設定影像填滿花費時間
	double                     GetFrameMergeTime() const { return m_FrameMergeTime; }//取得影像填滿花費時間
	//---------------------------------------------------------------------------------//	
	void                       SetFrameMergeState(FRAME_MERGE_STATE value) { m_FrameMergeState = value; }//設定影像填滿步驟
	FRAME_MERGE_STATE          GetFrameMergeState() const { return m_FrameMergeState; }//取得影像填滿步驟
	//---------------------------------------------------------------------------------//			
	void                       SetFrameCalcState(FRAME_CALC_STATE value) { m_FrameCalcState = value; }//設定影像計算狀態
	FRAME_CALC_STATE           GetFrameCalcState() const { return m_FrameCalcState; }//取得影像影像計算狀態
	//---------------------------------------------------------------------------------//		
	void                       SetFrameImageSaved(bool value) { m_FrameImageSaved = value; }//Frame影像是否存檔過了
	bool                       GetFrameImageSaved() const { return m_FrameImageSaved; }//Frame影像是否存檔過了
	//---------------------------------------------------------------------------------//
	bool                       CheckFrameFieldFilled();//確認影像畫面內的區域都取到影像
	bool                       ExecFrameMerge();//影像合併區域影像
	bool                       ExecFrameLoad();//影像載入圖檔
	bool                       ExecFrameSave();//影像儲存圖檔
	bool                       ExecFrameSave_Thread();//影像儲存圖檔-執行緒內	
	bool                       ExecFrameLoadImage();//影像載入圖檔
	bool                       SaveFrameImage_Offline();//影像儲存圖檔
	bool                       AllocateFrameBuffer();//建立影像資料
	void                       ClearFrameBuffer();//釋放影像資料
	bool                       CheckFrameNeedToLoad();//確認影像需要載入
	//---------------------------------------------------------------------------------//	
	bool                       ExecFramePhaseToSpace(int CvtMode, LIGHT_3D_CAST_ID CastID, int PhaseMode, int DLPLEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);
	//---------------------------------------------------------------------------------//	
	bool                       DumpFrameSliceImageList(LPCTSTR Folder, bool bUseDlpName);//匯出影像單張影像列表
	//---------------------------------------------------------------------------------//	
	CString                    GetFrameIndexName() const;//取得Frame引數名稱
	//---------------------------------------------------------------------------------//
	bool                       GetFrameSaveRawImageDone() const;//取得影像畫面原圖以儲存
	void                       SetFrameSaveRawImageDone(bool val);//設定影像畫面原圖以儲存	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIFRAME_H__E51917DE_7526_4618_B0AF_E5A25680973B__INCLUDED_)
