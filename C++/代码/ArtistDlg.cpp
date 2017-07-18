// ArtistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MediaPlayer.h"
#include "ArtistDlg.h"
#include "afxdialogex.h"
#include <afx.h>//CFile类
extern CString strFileNameView;
extern list<CString> PlayList;
int ArtistNum;

// CArtistDlg 对话框

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


// CArtistDlg 消息处理程序
BOOL CArtistDlg::OnInitDialog()
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
	CRect rect2;

	// 获取列表视图控件的位置和大小
	m_ArtistList.GetClientRect(&rect);
	m_MatchArtistList.GetClientRect(&rect2);

	// 为列表视图控件添加全行选中和栅格风格
	m_ArtistList.SetExtendedStyle(m_ArtistList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_MatchArtistList.SetExtendedStyle(m_ArtistList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ArtistList.InsertColumn(0, _T("艺术家"), LVCFMT_CENTER, rect.Width(), 0);
	// 为列表视图控件添加两列
	m_MatchArtistList.InsertColumn(0, _T("标题"), LVCFMT_CENTER, rect2.Width() / 3, 0);
	m_MatchArtistList.InsertColumn(1, _T("唱片集"), LVCFMT_CENTER, rect2.Width() / 3, 1);
	m_MatchArtistList.InsertColumn(2, _T("文件路径"), LVCFMT_CENTER, rect2.Width() / 3, 1);
	

	OnBnClickedFormmusicdcuments2();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE 
}

CArtistDlg::~CArtistDlg()
{
}



//从文件中读入歌曲
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

	int iPosBegin = -1;   //定义一个游标
	if ((iPosBegin = strTemp.Find('D')) != -1) //找到一个.索引时从0开始的
	{
		strTemp.Delete(0, iPosBegin);    // 删除头部
	}//★由于写入文件时D:\\前面还有字符串，不过没有在文件里显示出来，所以要手动删掉

	int iPos = -1;   //定义一个游标
	while ((iPos = strTemp.Find('\n')) != -1) //找到一个换行，索引时从0开始的
	{
		APath.push_back(strTemp.Left(iPos));//获取一个元素，并插入链表
		strTemp.Delete(0, iPos + 1);    // 删除头部
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


//将艺术家插入到表格中
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

//将艺术家对应歌曲插入到表格中
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

	int iPos = -1;   //定义一个游标
	while ((iPos = strTemp.Find('\n')) != -1) //找到一个，索引时从0开始的
	{
		Path.push_back(strTemp.Left(iPos));//获取一个元素，并插入链表
		strTemp.Delete(0, iPos + 1);    // 删除头部
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


//创建艺术家文件
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
	// TODO: 在此添加控件通知处理程序代码
	m_ArtistList.DeleteAllItems();
	vecArtist.clear();
	DeleteArtists();
	ArtistReadToList();
}


void CArtistDlg::OnClickArtistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	CString theArtistTitle;
	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第5个子项的文本   
		strFileNameView = m_MatchArtistList.GetItemText(pNMListView->iItem, 2);
		SetDlgItemText(IDC_EDIT3, strFileNameView);
	}
}


void CArtistDlg::OnRclickMatchArtist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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
	CFileFind fileFinder;	//实例化一个CFileFind 对象
	CString fp;
	fp = "D:\\PlayerTemp\\Artist";
	CString filePath = fp + _T("\\*.*");	//将路径添加完整
	BOOL bFinished = fileFinder.FindFile(filePath);//变量，判断是否遍历完
	while (bFinished)
	{
		i++;
		bFinished = fileFinder.FindNextFile();
		CString fileName = fileFinder.GetFileName();
		CString thefilePath = fileFinder.GetFilePath();
		int dotPos = fileName.ReverseFind('.');//寻找.所在位置
		CString fileExt = fileName.Right(fileName.GetLength() - dotPos);//提取.及其右侧的字符串
		if (fileExt == _T(".txt"))  //若是txt文件
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
	fileFinder.Close();//关闭文件
}

void CArtistDlg::OnInsertToPlayList()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	CString filePath;
	filePath = m_MatchArtistList.GetItemText(ArtistNum, 2);//获取本行的路径
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
