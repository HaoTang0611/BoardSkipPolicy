// AOILogic.h: interface for the CAOILogic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOILOGIC_H__EF370D6A_D13A_4704_B714_F1F5CCA35BEF__INCLUDED_)
#define AFX_AOILOGIC_H__EF370D6A_D13A_4704_B714_F1F5CCA35BEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
#include "AOIModelDef.h"
//-------------------------------------------------------------------------------------//
enum LOGIC_MODE//邏輯閘模式
{
	//LOGIC_MODE_OR         =  0,//邏輯閘模式-或
	LOGIC_MODE_AND        =  1,//邏輯閘模式-且
	LOGIC_MODE_XOR        =  2,//邏輯閘模式-XOR

	LOGIC_MODE_BIG        =  3,//邏輯閘模式-大於
	LOGIC_MODE_SMALL      =  4,//邏輯閘模式-小於

	LOGIC_MODE_EQU_BIG    =  5,//邏輯閘模式-大於等於
	LOGIC_MODE_EQU_SMALL  =  6,//邏輯閘模式-小於等於
	
	LOGIC_MODE_GAP_BIG    =  7,//邏輯閘模式-差距大於
	LOGIC_MODE_GAP_SMALL  =  8,//邏輯閘模式-差距小於		

	LOGIC_MODE_GAP_RATIO_BIG  = 9,//邏輯閘模式-差距比率大於
	LOGIC_MODE_GAP_RATIO_SMALL  = 10//邏輯閘模式-差距比率小於
};
//-------------------------------------------------------------------------------------//
class CAOIWnd;
class CAOILand;
class CAOIModel;
//-------------------------------------------------------------------------------------//
class CAOILogic : public CAOIObj  
{
	DECLARE_DYNAMIC(CAOILogic)
	//---------------------------------------------------------------------------------//	
	static  CString            GetLogicModeText(LOGIC_MODE LogicMode);
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//	
	unsigned int               m_LogicIndex;	
	//---------------------------------------------------------------------------------//	
	CAOIModel                 *m_LogicModelPtr;
	//---------------------------------------------------------------------------------//	
	CAOILand                  *m_LogicLandPtr;
	unsigned int               m_LogicLandIndex;
	//---------------------------------------------------------------------------------//
	int                        m_LogicGroupID;
	bool                       m_LogicModified;
//	RESULT_ID                  m_LogicResultID;//
//	WND_DEFECT_ID              m_LogicDefectID;	
	//---------------------------------------------------------------------------------//
	std::vector<CAOIWnd*>      m_LogicWndPtrList;
	std::vector<unsigned int>  m_LogicWndIndexList;
	//---------------------------------------------------------------------------------//	
	bool                       m_LogicEnabled;
	bool                       m_LogicSelected;
	bool                       m_LogicIsolated;
	//---------------------------------------------------------------------------------//
	LOGIC_MODE                 m_LogicLogicMode;	
	double                     m_LogicParamDBL1;	
	CString                    m_LogicResultString;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitLogic();
	void                       InitialLogic();
	void                       CloneLogic(const CAOILogic &Logic);
	//---------------------------------------------------------------------------------//
	size_t                     GetLogicWndPtrCount_Inline() const;
	void                       RemoveLogicWndList_Inline();
	void                       AddLogicWndPtr_Inline(CAOIWnd* Ptr);
	CAOIWnd*                   GetLogicWndPtr_Inline(size_t index, bool Check) const;	
	//---------------------------------------------------------------------------------//
	size_t                     GetLogicWndIndexCount_Inline() const;	
	void                       RemoveLogicWndIndexList_Inline();
	void                       AddLogicWndIndex_Inline(unsigned int WndIndex);
	unsigned int               GetLogicWndIndex_Inline(size_t index, bool Check) const;	
	//---------------------------------------------------------------------------------//
	bool                       DoLogicAnalysis_AND();//執行邏輯分析-且
	bool                       DoLogicAnalysis_XOR();//執行邏輯分析-XOR
	bool                       DoLogicAnalysis_BIG();//執行邏輯分析-大於
	bool                       DoLogicAnalysis_SMALL();//執行邏輯分析-小於
	bool                       DoLogicAnalysis_EQU_BIG();//執行邏輯分析-大於等於
	bool                       DoLogicAnalysis_EQU_SMALL();//執行邏輯分析-小於等於
	bool                       DoLogicAnalysis_GAP_BIG();//執行邏輯分析-差距大於
	bool                       DoLogicAnalysis_GAP_SMALL();//執行邏輯分析-差距小於
	bool                       DoLogicAnalysis_GAP_Ratio_BIG();//執行邏輯分析-差距比率大於
	bool                       DoLogicAnalysis_GAP_Ratio_SMALL();//執行邏輯分析-差距比率小於
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOILogic();
	CAOILogic(const CAOILogic &Logic);
	virtual ~CAOILogic();
	//---------------------------------------------------------------------------------//
	CAOILogic& operator=(const CAOILogic &Logic);
	//---------------------------------------------------------------------------------//
	CAOILogic*                 CloneLogicObj() const;//複製出實體的CAOILogic指標
	//---------------------------------------------------------------------------------//	
	void                       SetLogicIndex(int value) { CAOILogic::m_LogicIndex=value;}
	int                        GetLogicIndex() const { return CAOILogic::m_LogicIndex; }
	//---------------------------------------------------------------------------------//	
	void                       SetLogicModelPtr(CAOIModel *Ptr) { CAOILogic::m_LogicModelPtr=Ptr;}
	CAOIModel*                 GetLogicModelPtr() const { return CAOILogic::m_LogicModelPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetLogicLandPtr(CAOILand *Ptr) { CAOILogic::m_LogicLandPtr=Ptr;}
	CAOILand*                  GetLogicLandPtr() const { return CAOILogic::m_LogicLandPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetLogicLandIndex(unsigned int value) { CAOILogic::m_LogicLandIndex=value;}
	unsigned int               GetLogicLandIndex() const { return CAOILogic::m_LogicLandIndex; }
	//---------------------------------------------------------------------------------//
	void                       SetLogicGroupID(int value) { CAOILogic::m_LogicGroupID=value;}
	int                        GetLogicGroupID() const { return CAOILogic::m_LogicGroupID; }
	//---------------------------------------------------------------------------------//
	void                       SetLogicModified(bool value) { CAOILogic::m_LogicModified=value;}
	bool                       GetLogicModified() const { return CAOILogic::m_LogicModified; }
	//---------------------------------------------------------------------------------//
	//void                       SetLogicResultID(RESULT_ID value) { CAOILogic::m_LogicResultID=value;}
	//RESULT_ID                  GetLogicResultID() const { return CAOILogic::m_LogicResultID; }
	//---------------------------------------------------------------------------------//
	//void                       SetLogicDefectID(WND_DEFECT_ID value) { CAOILogic::m_LogicDefectID=value;}
	//WND_DEFECT_ID              GetLogicDefectID() const { return CAOILogic::m_LogicDefectID; }
	//---------------------------------------------------------------------------------//
	void                       SetLogicSelected(bool value) { CAOILogic::m_LogicSelected=value;}
	bool                       GetLogicSelected() const { return CAOILogic::m_LogicSelected; }
	//---------------------------------------------------------------------------------//
	void                       SetLogicEnabled(bool value) { CAOILogic::m_LogicEnabled=value;}
	bool                       GetLogicEnabled() const { return CAOILogic::m_LogicEnabled; }
	//---------------------------------------------------------------------------------//
	void                       SetLogicIsolated(bool value) { CAOILogic::m_LogicIsolated=value;}
	bool                       GetLogicIsolated() const { return CAOILogic::m_LogicIsolated; }
	//---------------------------------------------------------------------------------//
	size_t                     GetLogicWndPtrCount() const { return (CAOILogic::m_LogicWndPtrList.size()); }		
	size_t                     GetLogicWndIndexCount() const { return (CAOILogic::m_LogicWndIndexList.size()); }		
	void                       AddLogicWndPtr(CAOIWnd* Ptr);
	void                       AddLogicWndIndex(unsigned int WndIndex);
	unsigned int               GetLogicWndIndex(size_t index, bool Check) const;
	CAOIWnd*                   GetLogicWndPtr(size_t index, bool Check) const;	
	void                       SetLogicWndSelected(bool val); 
	CAOIWnd*                   GetLogicWndSelected() const; 
	void                       RemoveLogicWndPtrSelected();
	void                       RemoveLogicWndPtrList();	
	void                       UpdateLogicWndIndex();
	//---------------------------------------------------------------------------------//
	void                       SetLogicLogicMode(LOGIC_MODE value) { CAOILogic::m_LogicLogicMode=value;}
	LOGIC_MODE                 GetLogicLogicMode() const { return CAOILogic::m_LogicLogicMode; }
	//---------------------------------------------------------------------------------//	
	void                       SetLogicResultString(LPCTSTR value) { CAOILogic::m_LogicResultString=value;}
	LPCTSTR                    GetLogicResultString() const { return CAOILogic::m_LogicResultString; }
	//---------------------------------------------------------------------------------//		
	bool                       GetLogicHaveParamDBL(LOGIC_MODE LogicMode, int Index);
	//---------------------------------------------------------------------------------//
	void                       SetLogicParamDBL1(double value) { CAOILogic::m_LogicParamDBL1=value;}
	double                     GetLogicParamDBL1() const { return CAOILogic::m_LogicParamDBL1; }
	//---------------------------------------------------------------------------------//
	bool                       DoLogicAnalysis();//執行邏輯分析
	//---------------------------------------------------------------------------------//
	bool                       ApplyLogic(const CAOILogic *RefLogicPtr);//套用相同的邏輯閘
	bool                       SynchronousLogic(const CAOILogic *RefLogicPtr);//同步化同一個邏輯閘
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOILOGIC_H__EF370D6A_D13A_4704_B714_F1F5CCA35BEF__INCLUDED_)
