// TitleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MediaPlayer.h"
#include "MusicDlg.h"
#include "TitleDlg.h"
#include "afxdialogex.h"
#include <afx.h>
#include <list>
using namespace std;
// CTitleDlg �Ի���
extern CString strFileNameView;
extern list<CString> PlayList;
int num;

IMPLEMENT_DYNAMIC(CTitleDlg, CDialogEx)

CTitleDlg::CTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TITLEDLG, pParent)
{

}

CTitleDlg::~CTitleDlg()
{
}

void CTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_TitleList);
	DDX_Control(pDX, IDC_EDIT1, m_musicFileName);
	DDX_Control(pDX, IDC_FORMMUSICDCUMENTS, m_fromDocumentsToTitleDlg);
	
}


BEGIN_MESSAGE_MAP(CTitleDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTitleDlg::OnClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CTitleDlg::OnRclickList1)
	ON_BN_CLICKED(IDC_FORMMUSICDCUMENTS, &CTitleDlg::OnBnClickedFormmusicdcuments)
	ON_COMMAND(ID_32772, &CTitleDlg::OnDeleteFromList)
	ON_COMMAND(IDD_INSERT_TO_PLAYLIST, &CTitleDlg::OnInsertToPlaylist)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CTitleDlg::OnLvnKeydown)
END_MESSAGE_MAP()

// CTitleDlg ��Ϣ�������
BOOL CTitleDlg::OnInitDialog()
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

	// ��ȡ�б���ͼ�ؼ���λ�úʹ�С
	m_TitleList.GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	m_TitleList.SetExtendedStyle(m_TitleList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������
	m_TitleList.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect.Width() / 3.65, 0);
	m_TitleList.InsertColumn(1, _T("������"), LVCFMT_CENTER, rect.Width() / 3.65, 1);
	m_TitleList.InsertColumn(2, _T("��Ƭ"), LVCFMT_CENTER, rect.Width() / 3.65, 2);
	m_TitleList.InsertColumn(3, _T("�������"), LVCFMT_CENTER, rect.Width() / 7, 3);
	m_TitleList.InsertColumn(4, _T("˵��"), LVCFMT_CENTER, 0, 4);
	m_TitleList.InsertColumn(5, _T("�ļ�·��"), LVCFMT_CENTER, 0, 5);

	OnBnClickedFormmusicdcuments();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE 
}


list<CString> Path;
void CTitleDlg::TitleDlgReadToList()
{
	Path.clear();
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
	}//�������������������������������������������д���ļ�ʱD:\\ǰ�滹���ַ���������û�����ļ�����ʾ����������Ҫ�ֶ�ɾ��

	int iPos = -1;   //����һ���α�
	while ((iPos = strTemp.Find('\n')) != -1) //�ҵ�һ��.����ʱ��0��ʼ��
	{
		Path.push_back(strTemp.Left(iPos));//��ȡһ��Ԫ�أ�����������
		strTemp.Delete(0, iPos + 1);    // ɾ��ͷ��
	}
	Path.push_back(strTemp);
	if (Path.empty())
	{
		CString strs;
		strs = "EMPTY";
		MessageBox(strs);
	}

	list<CString>::iterator PathIterator;
	int i = 0;
	for (PathIterator = Path.begin(); PathIterator != Path.end()--; ++PathIterator, i++)
	{
		InsrtToTitleListControl(*PathIterator, i);
	}
}

void CTitleDlg::InsrtToTitleListControl(CString theFileName, int column)
{
	TitleMp3 t;
	t.getMP3Info(theFileName, column);
	m_TitleList.InsertItem(column, t.sTitle);
	m_TitleList.SetItemText(column, 1, t.sArtist);
	m_TitleList.SetItemText(column, 2, t.sAlbum);
	m_TitleList.SetItemText(column, 3, t.sYear);
	m_TitleList.SetItemText(column, 4, t.sComment);
	m_TitleList.SetItemText(column, 5, theFileName);
}




// CTitleDlg ��Ϣ�������

//��������
void CTitleDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
	{
		// ��ȡ��ѡ���б����һ��������ı�   
		strFileNameView = m_TitleList.GetItemText(pNMListView->iItem, 5);
		// ��ѡ���������ʾ��༭����   
		SetDlgItemText(IDC_EDIT1, strFileNameView);
	}
	*pResult = 0;
}



//�һ�����
void CTitleDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPMENU));

		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		strFileNameView = m_TitleList.GetItemText(pNMListView->iItem, 5);
		num = m_TitleList.GetSelectionMark();
	}
	*pResult = 0;

}

//������밴ť
void CTitleDlg::OnBnClickedFormmusicdcuments()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TitleList.DeleteAllItems();
	TitleDlgReadToList();
}



//��������ɾ������
void CTitleDlg::OnDeleteFromList()
{
	// TODO: �ڴ���������������
	CString str;
	str = m_TitleList.GetItemText(num, 5);
	m_TitleList.DeleteItem(num);
	//iter��ɾ������iter++ָ���Ԫ���Ѿ������ˣ���Ϊiterָ��ָ���Ԫ��ָ�루���������У��Ѿ���ɾ���������Ұָ�룬�޷���ͨ��iter++��ȷ���ҵ���һ��Ԫ�ء�
	//�ٵ���erase����ʱ�᷵�������������Ƿ��ص�ǰɾ������һ����
	list<CString>::iterator PathIterator;
	int i = 0;
	for (PathIterator = Path.begin(); PathIterator != Path.end();)
	{
		if ((*PathIterator).Compare(str) == 0)
		{
			PathIterator = Path.erase(PathIterator);
			//MessageBox(_T("ɾ���ɹ���"));
		}
		else
		{
			PathIterator++;
		}
	}
}


void CTitleDlg::OnInsertToPlaylist()
{
	// TODO: �ڴ���������������
	CString filePath;
	filePath = m_TitleList.GetItemText(num, 5);//��ȡ���е�·��
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

////////////////////////////////////////////�����¼�ѡ��ͼƬ//////////////////////////////////////////////////
UINT DelayLoop2(LPVOID pPatam)//ʹ������ʱ�߳�
{
	CTitleDlg* pDlg = (CTitleDlg*)pPatam;
	Sleep(50);
	pDlg->KeyDownUpdate2();
	return 0;
}

void CTitleDlg::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)//���������ѡ��
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	AfxBeginThread(DelayLoop2, this);
}

void CTitleDlg::KeyDownUpdate2()
{
	POSITION pos = m_TitleList.GetFirstSelectedItemPosition();
	int Item = m_TitleList.GetNextSelectedItem(pos);
	strFileNameView = m_TitleList.GetItemText(Item, 5);
	SetDlgItemText(IDC_EDIT1, strFileNameView);
}



