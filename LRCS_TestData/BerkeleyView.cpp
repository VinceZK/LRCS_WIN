// BerkeleyView.cpp : implementation file
//

#include "stdafx.h"
#include "LRCS_TestData.h"
#include "BerkeleyView.h"
#include "afxdialogex.h"
#include "../DbLayer_Test/DbLayer_Test.h"

#include <vector>

// CBerkeleyView dialog

IMPLEMENT_DYNAMIC(CBerkeleyView, CDialogEx)

CBerkeleyView::CBerkeleyView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBerkeleyView::IDD, pParent)
{

}

CBerkeleyView::~CBerkeleyView()
{
}

void CBerkeleyView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBerkeleyView, CDialogEx)
	ON_BN_CLICKED(IDC_DB_FILE_BROWSE, &CBerkeleyView::OnBnClickedDbFileBrowse)
	ON_BN_CLICKED(IDC_BUTTON1, &CBerkeleyView::OnBnClickedButton1)
END_MESSAGE_MAP()


// CBerkeleyView message handlers


void CBerkeleyView::OnBnClickedDbFileBrowse()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		::SetWindowText(GetDlgItem(IDC_DB_FILE)->GetSafeHwnd(), dlg.GetPathName());
	}
}


void CBerkeleyView::OnBnClickedButton1()
{
	CString csDbFile;
	GetDlgItem(IDC_DB_FILE)->GetWindowTextW(csDbFile);

	if (!csDbFile.IsEmpty())
	{
		if (m_spDb.get() != NULL)
		{
			m_spDb->DestroyDb();
		}

		m_spDb.reset(new CDbLayer_Test());
		m_spDb->InitDb(csDbFile);

		CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_DB_CONTENT);
		pList->ResetContent();

		std::vector<tstring> data = m_spDb->GetFirstData();
		if (data.size() > 0)
		{
			tstring line = _T("1 ");
			for(int i = 0; i < data.size(); ++i)
			{
				line.append(data[i]);
				line.append(_T(" "));
			}

			pList->AddString(line.c_str());
		}

		int nLine = 1;
		data = m_spDb->GetNextData();
		while(nLine < 1000 && data.size() > 0)
		{
			CString csNum;
			csNum.Format(_T("%d "), nLine+1);
			tstring line = csNum;
			for(int i = 0; i < data.size(); ++i)
			{
				line.append(data[i]);
				line.append(_T(" "));
			}

			pList->AddString(line.c_str());
			data = m_spDb->GetNextData();
			++nLine;
		}
	}
}
