#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "CWMPPlayer4.h"

// CMusicDlg 对话框

class CMusicDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMusicDlg)

public:
	CMusicDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMusicDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUSICDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_musicTab;
	
	afx_msg void ClearTheDocuments();
	afx_msg void search(CString);
	afx_msg void WriteToDocuments(CString);
	afx_msg void PassString();
	CMFCEditBrowseCtrl m_AddToBrowse;
	afx_msg void OnEnChangeMfceditbrowse1();
	CString m_AddMusic;
	afx_msg void OnSelchangeMusicdlgtab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedPlay();
	CWMPPlayer4 m_MediaPlayer;
	afx_msg void SaveToDocuments();
	afx_msg void ReadPlayListFromDocuments();
	afx_msg void InsrtToPlayListControl(CString, int);
	CListCtrl m_PlayList;
	afx_msg void OnBnClickedUpdatePlaylist();
	afx_msg void OnBnClickedStartplay();
	afx_msg void DeleteArtists();
	afx_msg void OnRclickPlaylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeleteFromPlayList();
	afx_msg void OnBnClickedClearfull();
	afx_msg void OnClickPlaylist(NMHDR *pNMHDR, LRESULT *pResult);
};


class MP3
{
public:
	char header[4];
	char title[30];//标题
	char artist[31];//艺术家
	char album[31];//唱片
	char year[4];//发行年份
	char comment[31];
	char genre[2];//a流派
public:
	CString sHeader;
	CString sTitle;//标题
	CString sArtist;//艺术家
	CString sAlbum;//唱片
	CString sYear;//发行年份
	CString sComment;
	CString sGenre;//a流派
public:
	void getMP3Info(CString, int);
};
