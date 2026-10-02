#pragma once


// CFingerprintWnd 對話方塊
#include "Fingerprint_Base.h"
class CFingerprintWnd : public CDialog
{
	DECLARE_DYNAMIC(CFingerprintWnd)

public:
	CFingerprintWnd(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CFingerprintWnd();

// 對話方塊資料
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_FINGERPRINT_WND };
//#endif
private:
	CBitmap m_LEDGreen;
	CBitmap m_LEDRed;
	CBitmap m_LEDYellow;
	CBitmap m_LEDGray;

	CStatic m_FingerPrintRegisterImg;
	CStatic m_CommandLabelStatic1;
	CStatic m_StatusLabelStatic1;
	CComboBox m_PortCombox;


	//BYTE m_FingerFeature[768];//指紋特徵大小

	//const int m_FingerFeatureSize = 128*6; //指紋特徵大小
	const int m_FingerFeatureSize = 1024; //指紋特徵大小
	BYTE* m_FingerFeature;

	bool m_CancelEvent;
	bool m_ThreadIsClose;
	bool m_ReturnPasswordMode;

	USER_FINGERPRINT_MODE m_FingerPrintMode;
	CString m_WndText;
	CString m_ErrorString;
	CString m_UserName;
	TUserNode m_User; //用來回傳指紋辨識出的使用者
	std::vector<TUserNode>     m_UserList;//用來傳遞給Fingerprint.cpp判斷是否有重複的指紋
	std::shared_ptr<Fingerprint_Base> m_FPSDevicePtr;
	FPS_DEVICE m_FPSDeviceType;
	//CFingerprint m_Fingerprint;
	int nPort;
	
protected:
	virtual void               DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL               PreTranslateMessage(MSG* pMsg);
	//---------------------------------------------------------------------------------//
	virtual BOOL               OnInitDialog();
	LRESULT		               OnGetMessage(WPARAM wParam, LPARAM lParam);
	virtual void		       OnCancel();
	afx_msg void               OnCbnSelchangePortComboBox();
	//---------------------------------------------------------------------------------//	
	DECLARE_MESSAGE_MAP()

private:
	void                       SwitchMultiLanguage();
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR * pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString & Error);
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadFPSINIFile();
	bool                       SaveFPSINIFile();
	//---------------------------------------------------------------------------------//	
	static UINT AFX_CDECL      ExecuteThread(LPVOID pParam);
	//---------------------------------------------------------------------------------//	


public:
	//---------------------------------------------------------------------------------//	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       SetFingerPrintMode(USER_FINGERPRINT_MODE bMode);
	void                       SetLoginFingerPrint(BYTE* FingerFeature);
	void                       GetEnrollFingerPrint(BYTE* FingerFeature);
	bool                       CheckWndIsCancel();
	//---------------------------------------------------------------------------------//	
	bool                       CheckReturnPassword() { return m_ReturnPasswordMode; };
	//void                       SetFingerPrintUserName(CString Username) { m_UserName = Username; }
	CString                    GetFingerPrintUserName() { return m_User.wUserName; }
	void                       SetFingerPrintUser(TUserNode User) { m_User = User; }
	TUserNode                  GetFingerPrintUser() { return m_User; }
	void                       SetUserList(std::vector<TUserNode> UserList) { m_UserList = UserList; }
	std::vector<TUserNode>     GetUserList() { return m_UserList; }
	bool                       DeleteFingerprintInDB(std::vector<TUserNode> UserList);
	//---------------------------------------------------------------------------------//	
};
