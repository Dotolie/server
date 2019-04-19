// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_strSend = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hSensorThread = (HANDLE)_beginthreadex(NULL, 0, _SensorThread, (LPVOID)this, CREATE_SUSPENDED , NULL);
}

CServerDlg::~CServerDlg()
{
	TerminateThread(m_hSensorThread, 0);

	delete m_pServerSock;
	POSITION pos = m_clientSocks.GetHeadPosition();
	while (pos != NULL)
		delete m_clientSocks.GetNext(pos);

}


void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_LIST_RECEIVE, m_listReceive);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SLIDER_SCALE, m_slidScale);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_USER_RESET, &CServerDlg::OnUserReset)

	ON_MESSAGE(WM_USER_REG_ALL, &CServerDlg::OnUserRegAll)
	ON_MESSAGE(WM_USER_REGISTER, &CServerDlg::OnUserRegister)
	ON_MESSAGE(WM_USER_GLOB_CMD, &CServerDlg::OnUserGlobCmd)
	ON_MESSAGE(WM_USER_SET_MODE, &CServerDlg::OnUserSetMode)
	ON_MESSAGE(WM_USER_START_STOP, &CServerDlg::OnUserStartStop)

	ON_MESSAGE(WM_USER_DATA_MANFFT, &CServerDlg::OnUserDataManfft)
	ON_MESSAGE(WM_USER_DATA_AUTOFFT, &CServerDlg::OnUserDataAutofft)
	ON_MESSAGE(WM_USER_DATA_MANCAP, &CServerDlg::OnUserDataMancap)
	ON_MESSAGE(WM_USER_DATA_REALTIME, &CServerDlg::OnUserDataRealtime)

	ON_MESSAGE(WM_USER_SET_CONFIG_ALARM, &CServerDlg::OnUserSetConfigAlarm)
	ON_MESSAGE(WM_USER_GET_CONFIG_ALARM, &CServerDlg::OnUserGetConfigAlarm)
	ON_MESSAGE(WM_USER_CONTROL_ALARM, &CServerDlg::OnUserControlAlarm)
	ON_MESSAGE(WM_USER_STATUS_ALARM, &CServerDlg::OnUserStatusAlarm)
	ON_MESSAGE(WM_USER_CLEAR_ALARM, &CServerDlg::OnUserClearAlarm)
	
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCALE, &CServerDlg::OnNMCustomdrawSliderScale)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_slidScale.SetRange(5,15);
	m_slidScale.SetRangeMin(5);
	m_slidScale.SetRangeMax(15);

	m_slidScale.SetPos(10);
	m_slidScale.SetTicFreq(1);

	m_pServerSock = new CServerSock();
	m_pServerSock->Create(SERVER_PORT_NO);
	m_pServerSock->Listen();



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CServerDlg::OnAccept()
{
	CClientSock *p = new CClientSock;
	m_pServerSock->Accept(*p);
//	p->Send("연결되었습니다.", 16);

	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);

	m_clientSocks.AddTail(p);

	SendPktGreeting();

}

void CServerDlg::OnReceive(CClientSock* pSock)
{
	int i = 0;
	int nLen = 0;
	int nSize = 0;
	char cCmd;
	char *pcData = NULL;
	char szBuf[200];
	UINT msgId = 0;

	nLen = pSock->Receive(szBuf,200);
	m_listReceive.InsertString(0, szBuf);
	
//	TRACE("[%s:%d] nLen=%d\n", __FUNCTION__, __LINE__, nLen);

	for (i = 0; i<nLen; i++) 
	{
		if (m_Parser.Decode(szBuf[i])) 
		{
			nSize = m_Parser.GetSize();
			cCmd = m_Parser.GetCmd();
			pcData = m_Parser.GetData();

			msgId = WM_USER_RECEIVE_CMD + cCmd;

			TRACE("[%s:%d] size=%d, cmd=0x%02x \n", __FUNCTION__, __LINE__, nSize, cCmd);

			PostMessage(msgId, nSize, (long long)pcData);
		}
	}

	//Send(buf);
}

void CServerDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char buf[200];
	lstrcpy(buf, "Server:");
	lstrcat(buf, (const char*)m_strSend);
	Send(buf, strlen(buf)+1 );
}

void CServerDlg::Send(char *buf, int nLen)
{
	POSITION pos = m_clientSocks.GetHeadPosition();
	while (pos != NULL)
	{
		CClientSock *p = m_clientSocks.GetNext(pos);
		p->Send(buf, nLen);
	}
}


void CServerDlg::OnClose(CClientSock* pSoc)
{
	// TODO: 여기에 구현 코드 추가.
	POSITION pos = m_clientSocks.GetHeadPosition();
	POSITION pos_saved;

	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);

	while (pos != NULL)
	{
		pos_saved = pos;
		CClientSock *p = m_clientSocks.GetNext(pos);
		if (p == pSoc)
		{
			m_clientSocks.RemoveAt(pos_saved);
			delete p;

			break;
		}
	}

}




afx_msg LRESULT CServerDlg::OnUserReset(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserRegister(WPARAM wParam, LPARAM lParam)
{
	int nSize = (int)wParam;
	int nValue = 0;
	int nSelect = 0;
	char cData[10] = { 0, };
	char *pParam;

	pParam = (char*)lParam;

	memcpy(cData, pParam, nSize);

	nSelect = cData[1];
	nValue = cData[3];

	TRACE("[%s:%d] nSize=%d, cData[]=0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", __FUNCTION__, __LINE__, 
		nSize, cData[0], cData[1], cData[2], cData[3], cData[4] );

	switch( nSelect )
	{
	case 0x1c:		//Select Scale
		m_Sensor.SetScale(nValue);
		break;
	case 0x3a:		//select bandwidth
		m_Sensor.SetBandwidth(nValue);
		break;
	default:
		break;
	}
	
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserGlobCmd(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserStartStop(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] IsRun=%d\n", __FUNCTION__, __LINE__, m_Sensor.IsRun());

	if (m_Sensor.IsRun() )
	{
		m_Sensor.SetStop();
		SuspendThread(m_hSensorThread);
	}
	else
	{
		m_Sensor.SetStart();
		ResumeThread(m_hSensorThread);
	}

	

	return 0;
}


afx_msg LRESULT CServerDlg::OnUserDataManfft(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserDataAutofft(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserDataMancap(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserDataRealtime(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserSetConfigAlarm(WPARAM wParam, LPARAM lParam)
{
	int nSize = (int)wParam;
	char *pAlarms = (char*)lParam;
	STATUS_PTR_t pStatus = m_Sensor.GetStatus();

	memcpy((char*)m_Sensor.m_Alarms, pAlarms, nSize );
	memcpy((char*)pStatus->alarms, pAlarms, nSize);

	TRACE("[%s:%d] nSize=%d\n", __FUNCTION__, __LINE__, nSize);

	return 0;
}


afx_msg LRESULT CServerDlg::OnUserGetConfigAlarm(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserControlAlarm(WPARAM wParam, LPARAM lParam)
{
	int nSize = (int)wParam;
	char cVal = *(char*)lParam;

	m_Sensor.SetAlarmCtrl( cVal&0x0f );

	TRACE("[%s:%d] nSize=%d, cVal=0x%02x\n", __FUNCTION__, __LINE__, nSize, cVal&0x0f );
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserStatusAlarm(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);
	return 0;
}


afx_msg LRESULT CServerDlg::OnUserClearAlarm(WPARAM wParam, LPARAM lParam)
{
	TRACE("[%s:%d] \n", __FUNCTION__, __LINE__);
	return 0;
}


int CServerDlg::SendPktGreeting()
{
	// TODO: 여기에 구현 코드 추가.
	char data[200] = { 0, };
	int len = 5;
	int nSize = sizeof(STATUS_t);
	STATUS_PTR_t pStatus = m_Sensor.GetStatus();

	MAKE_NETHDR(data, (len + nSize), PKT_GREETING);
	memcpy(data + len, (char*)pStatus, nSize);
	Send(data, (len + nSize));

	return 0;
}


afx_msg LRESULT CServerDlg::OnUserSetMode(WPARAM wParam, LPARAM lParam)
{
	int nSize = (int)wParam;
	char cData[10] = { 0, };
	char *pParam;
	
	pParam = (char*)lParam;

	memcpy(cData, pParam, nSize);

	m_Sensor.SetMode(cData[0]);

	TRACE("[%s:%d] nSize=%d, cData[0]=%d\n", __FUNCTION__, __LINE__, nSize, cData[0]);

	return 0;
}


afx_msg LRESULT CServerDlg::OnUserRegAll(WPARAM wParam, LPARAM lParam)
{
	int nSize = (int)wParam;
	char cData[10] = { 0, };

	TRACE("[%s:%d] nSize=%d\n", __FUNCTION__, __LINE__, nSize);

	return 0;
}


unsigned int __stdcall CServerDlg::_SensorThread(void *p_this)
{
	CServerDlg* p_ServerDlg = static_cast<CServerDlg*>(p_this);
	p_ServerDlg->SensorThread(); // Non-static member function! 

	return 0;
}

UINT WINAPI CServerDlg::SensorThread()
{
	int len = 5;
	int nRecord = 256;
	int nMode = 0;
	int nIndex = 0;
	char cCmd = 0;
	short sDiag = 0;
	short sAlm[3] = { 0, };
	
	


	m_bRun = true;

	while (m_bRun)
	{
		nMode = m_Sensor.GetMode();
		switch (nMode)
		{
		case 0:	// manual fft
			cCmd = PKT_DATA_MANFFT;
			break;
		case 1:
			cCmd = PKT_DATA_AUTOFFT;
			break;
		default:
			break;
		}

		

		m_Sensor.GenRandom();
		m_Sensor.ChkAlarm();

		sDiag = m_Sensor.m_sDiag;
		memcpy(sAlm, m_Sensor.m_sAlm, sizeof(sAlm));

		m_pcBuf = m_cBuf[nIndex];
		MAKE_NETHDR(m_pcBuf, (len + 4 + nRecord * 2), cCmd );
		m_pcBuf[len] = 'x';
		m_pcBuf[len+1] = ((sDiag >> 8) & 0xff);
		m_pcBuf[len+2] = (sAlm[X_AXIS] & 0xff);
		m_pcBuf[len+3] = ((sAlm[X_AXIS] >> 8) & 0xff);
		memcpy(m_pcBuf + 9, (char*)m_Sensor.m_sData[X_AXIS], (nRecord * 2));
		Send(m_pcBuf, (len + 4+ nRecord*2 ));
		(++nIndex) %= 16;

		m_pcBuf = m_cBuf[nIndex];
		MAKE_NETHDR(m_pcBuf, (len + 4 + nRecord * 2), cCmd);
		m_pcBuf[len] = 'y';
		m_pcBuf[len + 1] = ((sDiag >> 8) & 0xff);
		m_pcBuf[len + 2] = (sAlm[Y_AXIS] & 0xff);
		m_pcBuf[len + 3] = ((sAlm[Y_AXIS] >> 8) & 0xff);
		memcpy(m_pcBuf + 9, (char*)m_Sensor.m_sData[Y_AXIS], (nRecord * 2));
		Send(m_pcBuf, (len + 4 + nRecord * 2));
		(++nIndex) %= 16;

		m_pcBuf = m_cBuf[nIndex];
		MAKE_NETHDR(m_pcBuf, (len + 4 + nRecord * 2), cCmd);
		m_pcBuf[len] = 'z';
		m_pcBuf[len + 1] = ((sDiag >> 8) & 0xff);
		m_pcBuf[len + 2] = (sAlm[Z_AXIS] & 0xff);
		m_pcBuf[len + 3] = ((sAlm[Z_AXIS] >> 8) & 0xff);
		memcpy(m_pcBuf + 9, (char*)m_Sensor.m_sData[Z_AXIS], (nRecord * 2));
		Send(m_pcBuf, (len + 4 + nRecord * 2));
		(++nIndex) %= 16;


		TRACE("[%s:%d] nMode=%d\n", __FUNCTION__, __LINE__, m_Sensor.GetMode() );

		if (m_Sensor.GetMode() == 0)
		{
			m_Sensor.SetStop();
			SuspendThread(m_hSensorThread);
		}
		Sleep(1000);
	}

	return 0;
}


void CServerDlg::OnNMCustomdrawSliderScale(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nPos = m_slidScale.GetPos();
	float fPos = (20 - nPos)/10.0f;

	m_Sensor.SetScale( fPos );

	TRACE("[%s:%d] pos=%f\n", __FUNCTION__, __LINE__, fPos);

	*pResult = 0;
}
