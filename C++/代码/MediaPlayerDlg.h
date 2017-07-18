
// MediaPlayerDlg.h : 头文件
//

#pragma once
#include"MusicDlg.h"
#include"PictureDlg.h"
#include "afxcmn.h"

// CMediaPlayerDlg 对话框
class CMediaPlayerDlg : public CDialogEx
{
// 构造
public:
	CMediaPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEDIAPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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

