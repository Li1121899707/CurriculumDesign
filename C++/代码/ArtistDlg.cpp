// ArtistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MediaPlayer.h"
#include "ArtistDlg.h"
#include "afxdialogex.h"
#include <afx.h>//CFile��
extern CString strFileNameView;
extern list<CString> PlayList;
int ArtistNum;

// CArtistDlg �Ի���

IMPLEMENT_DYNAMIC(CArtistDlg, CDialogEx)

CArtistDlg::CArtistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ARTISTDLG, pParent)
{

}

void CArtistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ARTIST_LIST, m_ArtistList);
	DDX_Control(pDX, IDC_MATCH_ARTIST, m_MatchArtistList);
	DDX_Control(pDX, IDC_EDIT3, m_ArtistFileNameView);
}

BEGIN_MESSAGE_MAP(CArtistDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FORMMUSICDCUMENTS2, &CArtistDlg::OnBnClickedFormmusicdcuments2)
	ON_NOTIFY(NM_CLICK, IDC_ARTIST_LIST, &CArtistDlg::OnClickArtistList)
	ON_NOTIFY(NM_CLICK, IDC_MATCH_ARTIST, &CArtistDlg::OnClickMatchArtist)
	ON_NOTIFY(NM_RCLICK, IDC_MATCH_ARTIST, &CArtistDlg::OnRclickMatchArtist)
	ON_COMMAND(ID_INSERT_32777, &CArtistDlg::OnInsertToPlayList)
END_MESSAGE_MAP()


// CArtistDlg ��Ϣ�������
BOOL CArtistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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


	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect rect;
	CRect rect2;

	// ��ȡ�б���ͼ�ؼ���λ�úʹ�С
	m_ArtistList.GetClientRect(&rect);
	m_MatchArtistList.GetClientRect(&rect2);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	m_ArtistList.SetExtendedStyle(m_ArtistList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_MatchArtistList.SetExtendedStyle(m_ArtistList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ArtistList.InsertColumn(0, _T("������"), LVCFMT_CENTER, rect.Width(), 0);
	// Ϊ�б���ͼ�ؼ��������
	m_MatchArtistList.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect2.Width() / 3, 0);
	m_MatchArtistList.InsertColumn(1, _T("��Ƭ��"), LVCFMT_CENTER, rect2.Width() / 3, 1);
	m_MatchArtistList.InsertColumn(2, _T("�ļ�·��"), LVCFMT_CENTER, rect2.Width() / 3, 1);
	

	OnBnClickedFormmusicdcuments2();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE 
}

CArtistDlg::~CArtistDlg()
{
}



//���ļ��ж������
void CArtistDlg::ArtistReadToList()
{
	list<CString> APath;
	CFile  file(L"D:\\TXL.txt", CFile::modeRead);
	TCHAR  *pBuf;
	int  iLen = file.GetLength();
	pBuf = new  TCHAR[iLen + 1];
	file.Read(pBuf, iLen);
	pBuf[iLen / 2] = 0;
	file.Close();
	CString strTemp;
	strTemp.Format(_T("%s"), pBuf);
	//MessageBox(strTemp);

	int iPosBegin = -1;   //����һ���α�
	if ((iPosBegin = strTemp.Find('D')) != -1) //�ҵ�һ��.����ʱ��0��ʼ��
	{
		strTemp.Delete(0, iPosBegin);    // ɾ��ͷ��
	}//������д���ļ�ʱD:\\ǰ�滹���ַ���������û�����ļ�����ʾ����������Ҫ�ֶ�ɾ��

	int iPos = -1;   //����һ���α�
	while ((iPos = strTemp.Find('\n')) != -1) //�ҵ�һ�����У�����ʱ��0��ʼ��
	{
		APath.push_back(strTemp.Left(iPos));//��ȡһ��Ԫ�أ�����������
		strTemp.Delete(0, iPos + 1);    // ɾ��ͷ��
	}
	APath.push_back(strTemp);
	if (APath.empty())
	{
		CString strs;
		strs = "EMPTY";
		MessageBox(strs);
	}

	list<CString>::iterator PathIterator;
	int i = 0;
	for (PathIterator = APath.begin(); PathIterator != APath.end(); ++PathIterator, ++i)
	{
		ArtistInsrtToListControl(*PathIterator, i);
	}
}


//�������Ҳ��뵽�����
void CArtistDlg::ArtistInsrtToListControl(CString theFileName, int column)
{
	ArtistMP3 t;
	t.getMP3Info(theFileName, column);
	vector<CString>::iterator itArtist;
	int flag = 0;
	if (!vecArtist.empty())
	{
		for (itArtist = vecArtist.begin(); itArtist != vecArtist.end(); itArtist++)
		{
			if ((*itArtist).Compare(t.sArtist) == 0)
			{
				InputArtistToDocuments(t.sArtist, theFileName);
				flag = 1;
				break;
			}
		}
	}
	if (0 == flag)
	{
		InputArtistToDocuments(t.sArtist, theFileName);
		vecArtist.push_back(t.sArtist);
		m_ArtistList.InsertItem(column, t.sArtist);
	}
}

//�������Ҷ�Ӧ�������뵽�����
void CArtistDlg::ArtistInsrtToListControl2(CString theFileName, int column)
{
	ArtistMP3 t;
	t.getMP3Info(theFileName, column);
	m_MatchArtistList.InsertItem(column, t.sTitle);
	m_MatchArtistList.SetItemText(column, 1, t.sAlbum);
	m_MatchArtistList.SetItemText(column, 2, theFileName);
}



void CArtistDlg::ReadToArtistTitleList(CString ArtistPath)
{
	list<CString> Path;
	CFile  file(ArtistPath, CFile::modeRead);
	TCHAR  *pBuf;
	int  iLen = file.GetLength();
	pBuf = new  TCHAR[iLen + 1];
	file.Read(pBuf, iLen);
	pBuf[iLen / 2] = 0;
	file.Close();
	CString strTemp;
	strTemp.Format(_T("%s"), pBuf);
	//MessageBox(strTemp);

	int iPos = -1;   //����һ���α�
	while ((iPos = strTemp.Find('\n')) != -1) //�ҵ�һ��������ʱ��0��ʼ��
	{
		Path.push_back(strTemp.Left(iPos));//��ȡһ��Ԫ�أ�����������
		strTemp.Delete(0, iPos + 1);    // ɾ��ͷ��
	}
	Path.push_back(strTemp);
	if (Path.empty())
	{
		CString strs;
		strs = "EMPTY";
		//MessageBox(strs);
	}

	list<CString>::iterator PathIterator;
	int i = 0;
	for (PathIterator = Path.begin(); PathIterator != Path.end()--; ++PathIterator, ++i)
	{
		ArtistInsrtToListControl2(*PathIterator, i);
	}
}


//�����������ļ�
void CArtistDlg::InputArtistToDocuments(CString theArtistName, CString theFileName)
{
	CFile  cf;
	CString str1, str2, str;
	str1 = "D:\\PlayerTemp\\Artist\\";
	str2 = ".txt";
	str = str1 + theArtistName + str2;
	if (!cf.Open(str, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
	{
		AfxMessageBox(_T("D:\\TXL.TXT \r\n Open failed when write."));
	}

	cf.SeekToEnd();
	CString str3;
	str3 = "\n";
	int len = theFileName.GetLength();
	int len2 = str3.GetLength();
	cf.Write(theFileName.GetBuffer(len), len * 2);
	cf.Write(str3.GetBuffer(len2), len2 * 2);
	cf.Close();
	theFileName.ReleaseBuffer();
}



void CArtistDlg::OnBnClickedFormmusicdcuments2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ArtistList.DeleteAllItems();
	vecArtist.clear();
	DeleteArtists();
	ArtistReadToList();
}


void CArtistDlg::OnClickArtistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	CString theArtistTitle;
	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
	{
		// ��ȡ��ѡ���б����һ��������ı�   
		theArtistTitle = m_ArtistList.GetItemText(pNMListView->iItem, 0);
	}

	CFile  cf;
	CString str1, str2, str;
	str1 = "D:\\PlayerTemp\\Artist\\";
	str2 = ".txt";
	str = str1 + theArtistTitle + str2;
	m_MatchArtistList.DeleteAllItems();
	ReadToArtistTitleList(str);
}


void CArtistDlg::OnClickMatchArtist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
	{
		// ��ȡ��ѡ���б����5��������ı�   
		strFileNameView = m_MatchArtistList.GetItemText(pNMListView->iItem, 2);
		SetDlgItemText(IDC_EDIT3, strFileNameView);
	}
}


void CArtistDlg::OnRclickMatchArtist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU2));

		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		strFileNameView = m_MatchArtistList.GetItemText(pNMListView->iItem, 5);
		ArtistNum = m_MatchArtistList.GetSelectionMark();
	}
	*pResult = 0;

}




void CArtistDlg::DeleteArtists()
{
	int i = -1;
	int j = 0;
	CFileFind fileFinder;	//ʵ����һ��CFileFind ����
	CString fp;
	fp = "D:\\PlayerTemp\\Artist";
	CString filePath = fp + _T("\\*.*");	//��·���������
	BOOL bFinished = fileFinder.FindFile(filePath);//�������ж��Ƿ������
	while (bFinished)
	{
		i++;
		bFinished = fileFinder.FindNextFile();
		CString fileName = fileFinder.GetFileName();
		CString thefilePath = fileFinder.GetFilePath();
		int dotPos = fileName.ReverseFind('.');//Ѱ��.����λ��
		CString fileExt = fileName.Right(fileName.GetLength() - dotPos);//��ȡ.�����Ҳ���ַ���
		if (fileExt == _T(".txt"))  //����txt�ļ�
		{
			if (j == 0)
				j++;
			else
			{
				CFile cf;
				cf.Open(thefilePath, CFile::modeWrite, NULL);
				cf.SetLength(0);
				cf.Close();
			}
		}
	}
	fileFinder.Close();//�ر��ļ�
}

void CArtistDlg::OnInsertToPlayList()
{
	// TODO: �ڴ���������������
	// TODO: �ڴ���������������
	CString filePath;
	filePath = m_MatchArtistList.GetItemText(ArtistNum, 2);//��ȡ���е�·��
	int flag = 0;
	if (!PlayList.empty())
	{
		list<CString>::iterator PlayListIterator;
		int i = 0;
		for (PlayListIterator = PlayList.begin(); PlayListIterator != PlayList.end(); ++PlayListIterator)
		{
			if ((*PlayListIterator).Compare(filePath) == 0)
			{
				flag = 1;
				break;
			}
		}
	}
	if (0 == flag)
	{
		PlayList.push_back(filePath);
		CFile cf;
		if (!cf.Open(_T("D:\\PlayerTemp\\PlayList\\PlayList1.TXT"), CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			AfxMessageBox(_T("D:\\PlayerTemp\\PlayList\\PlayList1.TXT \r\n Open failed when write."));
		}
		else
		{
			CString str;
			str = "\n";
			int len1 = filePath.GetLength();
			int len2 = str.GetLength();
			cf.SeekToEnd();
			cf.Write(filePath.GetBuffer(len1), len1 * 2);
			cf.Write(str.GetBuffer(len2), len2 * 2);
			filePath.ReleaseBuffer();
			str.ReleaseBuffer();
			cf.Close();
		}
	}
	else
	{
		AfxMessageBox(_T("�����ڲ����б����Ѿ����ڣ�"));
	}

}
