// TitleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MediaPlayer.h"
#include "MusicDlg.h"
#include "TitleDlg.h"
#include "afxdialogex.h"
#include <afx.h>
#include <list>
using namespace std;
// CTitleDlg 对话框
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

// CTitleDlg 消息处理程序
BOOL CTitleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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


	// TODO: 在此添加额外的初始化代码
	CRect rect;

	// 获取列表视图控件的位置和大小
	m_TitleList.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格
	m_TitleList.SetExtendedStyle(m_TitleList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加六列
	m_TitleList.InsertColumn(0, _T("标题"), LVCFMT_CENTER, rect.Width() / 3.65, 0);
	m_TitleList.InsertColumn(1, _T("艺术家"), LVCFMT_CENTER, rect.Width() / 3.65, 1);
	m_TitleList.InsertColumn(2, _T("唱片"), LVCFMT_CENTER, rect.Width() / 3.65, 2);
	m_TitleList.InsertColumn(3, _T("发行年份"), LVCFMT_CENTER, rect.Width() / 7, 3);
	m_TitleList.InsertColumn(4, _T("说明"), LVCFMT_CENTER, 0, 4);
	m_TitleList.InsertColumn(5, _T("文件路径"), LVCFMT_CENTER, 0, 5);

	OnBnClickedFormmusicdcuments();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE 
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

	int iPosBegin = -1;   //定义一个游标
	if ((iPosBegin = strTemp.Find('D')) != -1) //找到一个.索引时从0开始的
	{
		strTemp.Delete(0, iPosBegin);    // 删除头部
	}//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★由于写入文件时D:\\前面还有字符串，不过没有在文件里显示出来，所以要手动删掉

	int iPos = -1;   //定义一个游标
	while ((iPos = strTemp.Find('\n')) != -1) //找到一个.索引时从0开始的
	{
		Path.push_back(strTemp.Left(iPos));//获取一个元素，并插入链表
		strTemp.Delete(0, iPos + 1);    // 删除头部
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




// CTitleDlg 消息处理程序

//单击链表
void CTitleDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
		strFileNameView = m_TitleList.GetItemText(pNMListView->iItem, 5);
		// 将选择的语言显示与编辑框中   
		SetDlgItemText(IDC_EDIT1, strFileNameView);
	}
	*pResult = 0;
}



//右击链表
void CTitleDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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

//点击键入按钮
void CTitleDlg::OnBnClickedFormmusicdcuments()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TitleList.DeleteAllItems();
	TitleDlgReadToList();
}



//从链表中删除歌曲
void CTitleDlg::OnDeleteFromList()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	str = m_TitleList.GetItemText(num, 5);
	m_TitleList.DeleteItem(num);
	//iter被删除后，再iter++指向的元素已经不对了，因为iter指针指向的元素指针（暂且这样叫）已经被删除，变成了野指针，无法再通过iter++正确的找到下一个元素。
	//再调用erase方法时会返回适配器，就是返回当前删除的下一个。
	list<CString>::iterator PathIterator;
	int i = 0;
	for (PathIterator = Path.begin(); PathIterator != Path.end();)
	{
		if ((*PathIterator).Compare(str) == 0)
		{
			PathIterator = Path.erase(PathIterator);
			//MessageBox(_T("删除成功！"));
		}
		else
		{
			PathIterator++;
		}
	}
}


void CTitleDlg::OnInsertToPlaylist()
{
	// TODO: 在此添加命令处理程序代码
	CString filePath;
	filePath = m_TitleList.GetItemText(num, 5);//获取本行的路径
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
		AfxMessageBox(_T("歌曲在播放列表中已经存在！"));
	}
}

////////////////////////////////////////////按上下键选中图片//////////////////////////////////////////////////
UINT DelayLoop2(LPVOID pPatam)//使用了延时线程
{
	CTitleDlg* pDlg = (CTitleDlg*)pPatam;
	Sleep(50);
	pDlg->KeyDownUpdate2();
	return 0;
}

void CTitleDlg::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)//方向键控制选项
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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



