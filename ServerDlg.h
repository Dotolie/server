// ServerDlg.h : header file
//
#include "ServerSock.h"
#include "ClientSock.h"
#include "Parser.h"
#include "packet.h"
#include "CSensor.h"
#include "afxcmn.h"

#if !defined(AFX_SERVERDLG_H__98A042E1_27EE_4CCC_AF3A_9E6AE372C409__INCLUDED_)
#define AFX_SERVERDLG_H__98A042E1_27EE_4CCC_AF3A_9E6AE372C409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	SERVER_PORT_NO	15001

#define WM_USER_RECEIVE_CMD			WM_USER+0x3000

#define WM_USER_SR_ACK				WM_USER_RECEIVE_CMD+PKT_ACK
#define WM_USER_SR_NAK				WM_USER_RECEIVE_CMD+PKT_NAK
#define WM_USER_GREETING			WM_USER_RECEIVE_CMD+PKT_GREETING
#define WM_USER_RESET				WM_USER_RECEIVE_CMD+PKT_RESET
#define WM_USER_VERSION_NO			WM_USER_RECEIVE_CMD+PKT_VERSION

#define WM_USER_REG_ALL				WM_USER_RECEIVE_CMD+PKT_REG_ALL
#define WM_USER_REGISTER			WM_USER_RECEIVE_CMD+PKT_REGISTER
#define WM_USER_GLOB_CMD			WM_USER_RECEIVE_CMD+PKT_GLOB_CMD
#define WM_USER_START_STOP			WM_USER_RECEIVE_CMD+PKT_START_STOP
#define WM_USER_SET_MODE			WM_USER_RECEIVE_CMD+PKT_SET_MODE

#define WM_USER_DATA_MANFFT			WM_USER_RECEIVE_CMD+PKT_DATA_MANFFT
#define WM_USER_DATA_AUTOFFT		WM_USER_RECEIVE_CMD+PKT_DATA_AUTOFFT
#define WM_USER_DATA_MANCAP			WM_USER_RECEIVE_CMD+PKT_DATA_MANCAP
#define WM_USER_DATA_REALTIME		WM_USER_RECEIVE_CMD+PKT_DATA_REALTIME

#define	WM_USER_SET_CONFIG_ALARM	WM_USER_RECEIVE_CMD+PKT_SET_CONFIG_ALARM
#define	WM_USER_GET_CONFIG_ALARM	WM_USER_RECEIVE_CMD+PKT_GET_CONFIG_ALARM
#define	WM_USER_CONTROL_ALARM		WM_USER_RECEIVE_CMD+PKT_CONTROL_ALARM
#define	WM_USER_STATUS_ALARM		WM_USER_RECEIVE_CMD+PKT_STATUS_ALARM
#define WM_USER_CLEAR_ALARM			WM_USER_RECEIVE_CMD+PKT_CLEAR_ALARM

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CServerDlg : public CDialog
{
// Construction
public:
	void Send(char* buf, int nLen);
	void OnReceive(CClientSock* pSock);
	void OnAccept();
	void OnClose(CClientSock* pSoc);
	CServerSock* m_pServerSock;
	CTypedPtrList<CObList, CClientSock*> m_clientSocks;
	CServerDlg(CWnd* pParent = NULL);	// standard constructor
	~CServerDlg();

	static unsigned int __stdcall _SensorThread(void *p_this);
	UINT WINAPI SensorThread();

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CListBox	m_listReceive;
	CString	m_strSend;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CParser m_Parser;
	CSensor m_Sensor;
	


protected:

	afx_msg LRESULT OnUserReset(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserRegister(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserGlobCmd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserStartStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserDataManfft(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserDataAutofft(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserDataMancap(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserDataRealtime(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserSetConfigAlarm(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserGetConfigAlarm(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserControlAlarm(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserStatusAlarm(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserClearAlarm(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserSetMode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserRegAll(WPARAM wParam, LPARAM lParam);

private:
	int SendPktGreeting();

	char*	m_pcBuf;
	char	m_cBuf[16][1100];
	bool	m_bRun;
	HANDLE	m_hSensorThread;

public:
	CSliderCtrl m_slidScale;
	afx_msg void OnNMCustomdrawSliderScale(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__98A042E1_27EE_4CCC_AF3A_9E6AE372C409__INCLUDED_)
