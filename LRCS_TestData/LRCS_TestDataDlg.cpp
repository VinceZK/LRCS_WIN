
// LRCS_TestDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LRCS_TestData.h"
#include "LRCS_TestDataDlg.h"
#include "afxdialogex.h"
#include "BerkeleyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLRCS_TestDataDlg dialog



CLRCS_TestDataDlg::CLRCS_TestDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLRCS_TestDataDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLRCS_TestDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLRCS_TestDataDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLRCS_TestDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BERKELEY_VIEW, &CLRCS_TestDataDlg::OnBnClickedBerkeleyView)
END_MESSAGE_MAP()


// CLRCS_TestDataDlg message handlers

BOOL CLRCS_TestDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	GetDlgItem(IDC_DATA_FOLDER)->SetWindowTextW(_T("D:\\Data"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLRCS_TestDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLRCS_TestDataDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLRCS_TestDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLRCS_TestDataDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csColumnNum;
	GetDlgItem(IDC_COLUMN_NUM)->GetWindowTextW(csColumnNum);
	CString priKeyLen;
	GetDlgItem(IDC_PRIKEY_LENGTH)->GetWindowTextW(priKeyLen);

	int nColumnCount = _ttoi(csColumnNum);
	CString priKeyPrefix; 
	GetDlgItem(IDC_PRIMARY_KEY)->GetWindowTextW(priKeyPrefix);
	int nPriKeyLen = _ttoi(priKeyLen);

	for(int i = 0; i < nColumnCount; ++i)
	{

	}
}


void CLRCS_TestDataDlg::OnBnClickedBerkeleyView()
{
	CBerkeleyView().DoModal();
}
