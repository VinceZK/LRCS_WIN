#pragma once

#include <memory>
class CDbLayer_Test;

// CBerkeleyView dialog

class CBerkeleyView : public CDialogEx
{
	DECLARE_DYNAMIC(CBerkeleyView)

public:
	CBerkeleyView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBerkeleyView();

// Dialog Data
	enum { IDD = IDD_BERKELEY_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDbFileBrowse();

private:
	std::shared_ptr<CDbLayer_Test> m_spDb;
public:
	afx_msg void OnBnClickedButton1();
};
