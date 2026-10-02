// AOIObjManager.h: interface for the CAOIObjManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIOBJMANAGER_H__4CDA2C6A_F2F4_4B4C_8D63_410932776DB7__INCLUDED_)
#define AFX_AOIOBJMANAGER_H__4CDA2C6A_F2F4_4B4C_8D63_410932776DB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "AOIFd.h"
#include "AOIFov.h"
#include "AOIObj.h"
#include "AOIRgn.h"
#include "AOIBox.h"
#include "AOIWnd.h"
#include "AOILand.h"
#include "AOILogic.h"
#include "AOIModel.h"
#include "AOISlice.h"
#include "AOIFrame.h"
#include "AOIBoard.h"
#include "AOIPanel.h"
#include "AOIField.h"
#include "AOIWndRoi.h"
#include "AOIWndMask.h"
#include "AOIProject.h"
#include "AOIWindow.h"
#include "AOIBarcode.h"
#include "AOIComponent.h"
#include "AOIPartGroup.h"
//-------------------------------------------------------------------------------------//
#define OBJ_MGR_MAX_MP_COUNT   16
//-------------------------------------------------------------------------------------//
typedef struct tagObjRecycleNode//碻吏ㄏノ竊翴
{
	std::vector<CAOIObj*>       ObjList;//ン	
	size_t                      StartIndex;//癬﹍腹

	tagObjRecycleNode()
	{	StartIndex = 0;	}

	void Clear()
	{
		StartIndex = 0;
		ObjList.clear();
	}
} TObjRecycleNode, *PObjRecycleNode;
//-------------------------------------------------------------------------------------//
class CAOIObjManager  
{
private:
	//---------------------------------------------------------------------------------//	
	CRITICAL_SECTION            m_csManager;//˙て
	//---------------------------------------------------------------------------------//	
	size_t                      m_glbObjDelCount;//竒埃计秖
	//---------------------------------------------------------------------------------//
	std::vector<CAOIObj*>       m_glbObjList;
	//---------------------------------------------------------------------------------//
	TObjRecycleNode             m_RecycleWnd;//碻吏ㄏノ浪代
	TObjRecycleNode             m_RecycleLand;//碻吏ㄏノ疭紉
	//---------------------------------------------------------------------------------//
#ifdef OPEN_MP_USE
	bool                        m_RecycleMP;
	CRITICAL_SECTION            m_csManager_MP[OBJ_MGR_MAX_MP_COUNT];//˙て
	TObjRecycleNode             m_RecycleWnd_MP[OBJ_MGR_MAX_MP_COUNT];//碻吏ㄏノ浪代		
	TObjRecycleNode             m_RecycleLand_MP[OBJ_MGR_MAX_MP_COUNT];//碻吏ㄏノ浪代
#endif//OPEN_MP_USE
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CString                     m_ErrorString;	
	//---------------------------------------------------------------------------------//
	CAOIObjManager(const CAOIObjManager &manager);
	CAOIObjManager& operator=(const CAOIObjManager &manager);
	//---------------------------------------------------------------------------------//
	bool                       AddObjPtr(CAOIObj *ObjPtr);
	//---------------------------------------------------------------------------------//
	bool                       DestroyObject(CAOIObj *ObjPtr);
	//---------------------------------------------------------------------------------//
	void                       ShowMessageBox(LPCTSTR str);
	//---------------------------------------------------------------------------------//
	bool                       LockManagerFn(CRITICAL_SECTION &cs);
	bool                       UnlockManagerFn(CRITICAL_SECTION &cs);
	//---------------------------------------------------------------------------------//
	void                       CheckGlobalObjList();	
	void                       DestroyRecycleObjList();	
	void                       RearrangeGlobalObjList();	
	//---------------------------------------------------------------------------------//
	void                       ClearRecycleWndList();
	CAOIWnd*                   GetFreeRecycleWndPtr();
	void                       ClearRecycleLandList();
	CAOILand*                  GetFreeRecycleLandPtr();	
	//---------------------------------------------------------------------------------//
	CAOIObj*                   GetFreeRecycleObjPtr(CRITICAL_SECTION &cs, TObjRecycleNode &RecycleNode);
	bool                       AddFreeRecycleObjPtr(CRITICAL_SECTION &cs, TObjRecycleNode &RecycleNode, CAOIObj *ObjPtr, bool bMP);
	bool                       ClearRecycleObjList_MP();//睲埃碻吏ㄏノ-MP
	bool                       BuildRecycleObjList_MP(const TObjRecycleNode &RecycleNode, TObjRecycleNode Node_MP[], int MPCount);//俱碻吏ㄏノ
	//---------------------------------------------------------------------------------//			
public:
	//---------------------------------------------------------------------------------//
	CAOIObjManager();
	virtual ~CAOIObjManager();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() const;
	//---------------------------------------------------------------------------------//
	void                       SetObjExceptionCode_Create(LPCTSTR Err=NULL);
	void                       SetObjExceptionCode_Delete(LPCTSTR Err=NULL);	
	//---------------------------------------------------------------------------------//	
	bool                       LockManager();
	bool                       UnlockManager();
	//---------------------------------------------------------------------------------//
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	CAOIFd*                    CreateFdObj();//承 CAOIFd ン
	bool                       DestroyFdObj(CAOIFd *&FdPtr);//篟反 CAOIFd ン
	bool                       DestroyFdList(std::vector<CAOIFd*> &List);//篟反 CAOIFd
	//---------------------------------------------------------------------------------//
	CAOIObj*                   CreateObject();//承 CAOIObj ン
	bool                       DestroyAOIObj(CAOIObj *&ObjPtr);//篟反 CAOIObj ン	
	bool                       DestroyAOIList(std::vector<CAOIObj*> &List);//篟反 CAOIObj
	//---------------------------------------------------------------------------------//
	CAOIRgn*                   CreateRgnObj();//承 CAOIRgn ン
	bool                       DestroyRgnObj(CAOIRgn *&RgnPtr);//篟反 CAOIRgn ン
	bool                       DestroyRgnList(std::vector<CAOIRgn*> &List);//篟反 CAOIRgn
	//---------------------------------------------------------------------------------//
	CAOIFov*                   CreateFovObj();//承 CAOIFov ン
	bool                       DestroyFovObj(CAOIFov *&FovPtr);//篟反 CAOIFov ン
	bool                       DestroyFovList(std::vector<CAOIFov*> &List);//篟反 CAOIFov
	//---------------------------------------------------------------------------------//
	CAOIBox*                   CreateBoxObj();//承 CAOIBox ン
	bool                       DestroyBoxObj(CAOIBox *&BoxPtr);//篟反 CAOIBox ン
	bool                       DestroyBoxList(std::vector<CAOIBox*> &List);//篟反 CAOIBox
	//---------------------------------------------------------------------------------//
	CAOIWnd*                   CreateWndObj();//承 CAOIWnd ン	
	bool                       DestroyWndObj(CAOIWnd *&WndPtr);//篟反 CAOIWnd ン
	bool                       DestroyWndList(std::vector<CAOIWnd*> &List);//篟反 CAOIWnd
	//---------------------------------------------------------------------------------//
	CAOILand*                  CreateLandObj();//承 CAOILand ン
	bool                       DestroyLandObj(CAOILand *&LandPtr);//篟反 CAOILand ン
	bool                       DestroyLandList(std::vector<CAOILand*> &List);//篟反 CAOILand
	//---------------------------------------------------------------------------------//
	CAOILogic*                 CreateLogicObj();//承 CAOILogic ン 
	bool                       DestroyLogicObj(CAOILogic *&LogicPtr);//篟反 CAOILogic ン
	bool                       DestroyLogicList(std::vector<CAOILogic*> &List);//篟反 CAOILogic
	//---------------------------------------------------------------------------------//
	CAOIModel*                 CreateModelObj();//承 CAOIModel ン
	bool                       DestroyModelObj(CAOIModel *&ModelPtr);//篟反 CAOIModel ン
	bool                       DestroyModelList(std::vector<CAOIModel*> &List);//篟反 CAOIModel
	//---------------------------------------------------------------------------------//
	CAOISlice*                 CreateSliceObj();//承 CAOISlice ン
	bool                       DestroySliceObj(CAOISlice *&SlicePtr);//篟反 CAOISlice ン	
	bool                       DestroySliceList(std::vector<CAOISlice*> &List);//篟反 CAOISlice
	//---------------------------------------------------------------------------------//
	CAOIField*                 CreateFieldObj();//承 CAOIField ン	
	bool                       DestroyFieldObj(CAOIField *&FieldPtr);//篟反 CAOIField ン
	bool                       DestroyFieldList(std::vector<CAOIField*> &List);//篟反 CAOIField
	//---------------------------------------------------------------------------------//
	CAOIFrame*                 CreateFrameObj();//承 CAOIFrame ン
	bool                       DestroyFrameObj(CAOIFrame *&FramePtr);//篟反 CAOIFrame ン
	bool                       DestroyFrameList(std::vector<CAOIFrame*> &List);//篟反 CAOIFrame
	//---------------------------------------------------------------------------------//
	CAOIBoard*                 CreateBoardObj();//承 CAOIBoard ン
	bool                       DestroyBoardObj(CAOIBoard *&BoardPtr);//篟反 CAOIBoard ン
	bool                       DestroyBoardList(std::vector<CAOIBoard*> &List);//篟反 CAOIBoard
	//---------------------------------------------------------------------------------//
	CAOIPanel*                 CreatePanelObj();//承 CAOIPanel ン	
	bool                       DestroyPanelObj(CAOIPanel *&PanelPtr);//篟反 CAOIPanel ン	
	bool                       DestroyPanelList(std::vector<CAOIPanel*> &List);//篟反 CAOIPanel
	//---------------------------------------------------------------------------------//
	CAOIWindow*                CreateWindowObj();//承 CAOIWindow ン
	bool                       DestroyWindowObj(CAOIWindow *&WindowPtr);//篟反 CAOIWindow ン
	bool                       DestroyWindowList(std::vector<CAOIWindow*> &List);//篟反 CAOIWindow
	//---------------------------------------------------------------------------------//	
	CAOIWndRoi*                CreateWndRoiObj();//承ン CAOIWndRoi ン
	bool                       DestroyWndRoiObj(CAOIWndRoi *&WndRoiPtr);//篟反 CAOIWndRoi ン
	bool                       DestroyWndRoiList(std::vector<CAOIWndRoi*> &List);//篟反 CAOIWndRoi
	//---------------------------------------------------------------------------------//
	CAOIWndMask*               CreateWndMaskObj();//承ン CAOIWndMask ン
	bool                       DestroyWndMaskObj(CAOIWndMask *&WndMaskPtr);//篟反 CAOIWndMask ン
	bool                       DestroyWndMaskList(std::vector<CAOIWndMask*> &List);//篟反 CAOIWndMask
	//---------------------------------------------------------------------------------//
	CAOIProject*               CreateProjectObj();//承 CAOIProject ン
	bool                       DestroyProjectObj(CAOIProject *&ProjectPtr);//篟反 CAOIProject ン
	bool                       DestroyProjectList(std::vector<CAOIProject*> &List);//篟反 CAOIProject
	//---------------------------------------------------------------------------------//
	CAOIBarcode*               CreateBarcodeObj();//承 CAOIBarcode ン
	bool                       DestroyBarcodeObj(CAOIBarcode *&BarcodePtr);//篟反 CAOIBarcode ン
	bool                       DestroyBarcodeList(std::vector<CAOIBarcode*> &List);//篟反 CAOIBarcode
	//---------------------------------------------------------------------------------//
	CAOIComponent*             CreateComponentObj();//承 CAOIComponent ン
	bool                       DestroyComponentObj(CAOIComponent *&CmpPtr);	//篟反 CAOIComponent ン
	bool                       DestroyComponentList(std::vector<CAOIComponent*> &List);	//篟反 CAOIComponent
	//---------------------------------------------------------------------------------//	
	CAOIMark*                  CreateMarkObj();//承 CAOIMark ン
	bool                       DestroyMarkObj(CAOIMark *&MarkPtr);	//篟反 CAOIMark ン
	bool                       DestroyMarkList(std::vector<CAOIMark*> &List);	//篟反 CAOIMark
	//---------------------------------------------------------------------------------//	
	CAOIPartGroup*             CreatePartGroupObj();//承 CAOIPartGroup ン
	bool                       DestroyPartGroupObj(CAOIPartGroup *&PartGroupPtr);	//篟反 CAOIPartGroup ン
	bool                       DestroyPartGroupList(std::vector<CAOIPartGroup*> &List);	//篟反 CAOIPartGroup
	//---------------------------------------------------------------------------------//	
	void                       LayoutRecycleObjList(int MPCount=0);//俱碻吏ㄏノ	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
extern CAOIObjManager  AOIObjManager;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIOBJMANAGER_H__4CDA2C6A_F2F4_4B4C_8D63_410932776DB7__INCLUDED_)
