#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "CWMPPlayer4.h"

// CMusicDlg �Ի���

class CMusicDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMusicDlg)

public:
	CMusicDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMusicDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUSICDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	char title[30];//����
	char artist[31];//������
	char album[31];//��Ƭ
	char year[4];//�������
	char comment[31];
	char genre[2];//a����
public:
	CString sHeader;
	CString sTitle;//����
	CString sArtist;//������
	CString sAlbum;//��Ƭ
	CString sYear;//�������
	CString sComment;
	CString sGenre;//a����
public:
	void getMP3Info(CString, int);
};
