#pragma once
#include "afxcmn.h"
#include "MusicDlg.h"
#include "afxwin.h"
#include"MediaPlayerDlg.h"

// CTitleDlg 对话框

class CTitleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTitleDlg)

public:
	CTitleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTitleDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TITLEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	BOOL CTitleDlg::PreTranslateMessage(MSG* pMsg)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		else
			return CDialog::PreTranslateMessage(pMsg);
	}
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_TitleList;
	CEdit m_musicFileName;
	CButton m_fromDocumentsToTitleDlg;

	afx_msg void TitleDlgReadToList();
	afx_msg void InsrtToTitleListControl(CString, int);
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFormmusicdcuments();
	afx_msg void OnDeleteFromList();
	afx_msg void OnInsertToPlaylist();
	void KeyDownUpdate2();//上下键控制响应内容
	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);//上下键控制响应函数
};

class TitleMp3 : public MP3
{
public:
	TitleMp3()
	{
	}
	TitleMp3(CString str, int c)
	{
		getMP3Info(str, c);
	}
};