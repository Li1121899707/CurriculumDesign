#pragma once
#include "afxcmn.h"
#include "MusicDlg.h"
#include "afxwin.h"
#include<vector>
#include<list>
using namespace std;

// CArtistDlg 对话框

class CArtistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CArtistDlg)

public:
	CArtistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArtistDlg();
	virtual BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARTISTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL CArtistDlg::PreTranslateMessage(MSG* pMsg)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		else
			return CDialog::PreTranslateMessage(pMsg);
	}
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ArtistList;
	CListCtrl m_MatchArtistList;
	CEdit m_ArtistFileNameView;
	vector<CString> vecArtist;
	afx_msg void ArtistReadToList();
	afx_msg void ArtistInsrtToListControl(CString, int);
	afx_msg void ArtistInsrtToListControl2(CString, int);
	afx_msg void ReadToArtistTitleList(CString);
	afx_msg void InputArtistToDocuments(CString, CString);
	afx_msg void OnBnClickedFormmusicdcuments2();
	afx_msg void OnClickArtistList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickMatchArtist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickMatchArtist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void DeleteArtists();
	afx_msg void OnInsertToPlayList();
};

class ArtistMP3 : public MP3
{
public:
	ArtistMP3()
	{
	}
	ArtistMP3(CString str, int c)
	{
		getMP3Info(str, c);
	}
};