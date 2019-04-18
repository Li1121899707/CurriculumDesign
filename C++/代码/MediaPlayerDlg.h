
// MediaPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include"MusicDlg.h"
#include"PictureDlg.h"
#include "afxcmn.h"

// CMediaPlayerDlg �Ի���
class CMediaPlayerDlg : public CDialogEx
{
// ����
public:
	CMediaPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEDIAPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL CMediaPlayerDlg::PreTranslateMessage(MSG* pMsg)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		else
			return CDialog::PreTranslateMessage(pMsg);
	}
	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_tab;
	afx_msg void OnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult);
};

