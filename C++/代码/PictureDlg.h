#pragma once
#include "afxcmn.h"
#include "afxeditbrowsectrl.h"


// CPictureDlg �Ի���

class CPictureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPictureDlg)

public:
	CPictureDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPictureDlg();
	virtual BOOL OnInitDialog();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICTUREDLG };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL CPictureDlg::PreTranslateMessage(MSG* pMsg)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		else
			return CDialog::PreTranslateMessage(pMsg);
	}
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_pictureList;
	CMFCEditBrowseCtrl m_PictureBrowseCtrl;
	afx_msg void OnEnChangeMfceditbrowse1();
	CString m_AddToPicture;
	afx_msg void search(CString);//�����ļ����е�ÿһ��ͼƬ
	template <class T>
	afx_msg void LongToStr(T, char *);
	afx_msg void OnClickPicturelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedPictureStatic();
	afx_msg void OnRclickPicturelist(NMHDR *pNMHDR, LRESULT *pResult);
	void KeyDownUpdate();//���¼�������Ӧ����
	afx_msg void OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult);//���¼�������Ӧ����
	afx_msg void OnBnClickedPlayButton();
	afx_msg void OnPicturedeleteFromList();
};
