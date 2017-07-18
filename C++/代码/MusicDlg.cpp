// MusicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MediaPlayer.h"
#include "MusicDlg.h"
#include "ArtistDlg.h"
#include "TitleDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include "CWMPPlayer4.h"
#include <list>

#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib,"WINMM.LIB")

CString strFileNameView;
CFont font;
CArtistDlg m_artistDlg;
CTitleDlg m_titleDlg;

using namespace std;

extern list<CString> Path;
list<CString> PlayList;

// CMusicDlg 对话框

IMPLEMENT_DYNAMIC(CMusicDlg, CDialogEx)

CMusicDlg::CMusicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MUSICDLG, pParent)
	, m_AddMusic(_T(""))
{

}

CMusicDlg::~CMusicDlg()
{
}

void CMusicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MUSICDLGTAB, m_musicTab);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_AddToBrowse);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_AddMusic);
	DDX_Control(pDX, IDC_OCX1, m_MediaPlayer);
	DDX_Control(pDX, IDC_PLAYLIST, m_PlayList);
}


BEGIN_MESSAGE_MAP(CMusicDlg, CDialogEx)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CMusicDlg::OnEnChangeMfceditbrowse1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MUSICDLGTAB, &CMusicDlg::OnSelchangeMusicdlgtab)
	ON_BN_CLICKED(IDC_PLAY, &CMusicDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_UPDATE_PLAYLIST, &CMusicDlg::OnBnClickedUpdatePlaylist)
	ON_BN_CLICKED(IDC_STARTPLAY, &CMusicDlg::OnBnClickedStartplay)
	ON_NOTIFY(NM_RCLICK, IDC_PLAYLIST, &CMusicDlg::OnRclickPlaylist)
	ON_COMMAND(ID_DELETE_32776, &CMusicDlg::OnDeleteFromPlayList)
	ON_BN_CLICKED(IDC_CLEARFULL, &CMusicDlg::OnBnClickedClearfull)
	ON_NOTIFY(NM_CLICK, IDC_PLAYLIST, &CMusicDlg::OnClickPlaylist)
END_MESSAGE_MAP()


// CMusicDlg 消息处理程序
BOOL CMusicDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	//SetIcon(m_hIcon, TRUE);			// 设置大图标
	//SetIcon(m_hIcon, FALSE);		// 设置小图标

	DeleteArtists();
	CRect tabRect;   // 标签控件客户区的位置和大小   
	CRect rect;

	m_musicTab.InsertItem(0, _T("音乐集"));	//tab对应第一个窗口      
	m_musicTab.InsertItem(1, _T("艺术家"));  //tab对应第二个窗口    
	m_titleDlg.Create(IDD_TITLEDLG, &m_musicTab);    // 创建第一个标签页   
	m_artistDlg.Create(IDD_ARTISTDLG, &m_musicTab); // 创建第二个标签页   
	
	m_musicTab.GetClientRect(&tabRect);    // 获取标签控件客户区Rect   
									  // 调整tabRect，使其覆盖范围适合放置标签页   
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	// 根据调整好的tabRect放置m_TitleDlg子对话框，并设置为显示   
	m_titleDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	// 根据调整好的tabRect放置m_ArtistDlg子对话框，并设置为隐藏   
	m_artistDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	
	font.CreatePointFont(240, _T("微软雅黑"));
	GetDlgItem(IDC_MUSICHEAD)->SetFont(&font);

	m_PlayList.GetClientRect(&rect);
	m_PlayList.SetExtendedStyle(m_PlayList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_PlayList.InsertColumn(0, _T("标题"), LVCFMT_CENTER, rect.Width(), 0);
	m_PlayList.InsertColumn(1, _T("路径"), LVCFMT_CENTER, 0, 1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMusicDlg::ClearTheDocuments()
{
	CFile cf;
	cf.Open(_T("D:\\TXL.TXT"), CFile::modeCreate | CFile::modeWrite, NULL);
	cf.Close();
}

void CMusicDlg::search(CString fp)
{
	int i = -1;
	CFileFind fileFinder;	//实例化一个CFileFind 对象
	CString filePath = fp + _T("\\*.*");	//将路径添加完整
	BOOL bFinished = fileFinder.FindFile(filePath);//变量，判断是否遍历完
	while (bFinished)
	{
		i++;
		bFinished = fileFinder.FindNextFile();//FindNextFile是从FindFile指定的路径开始查找
		//IsDirectory判断是否是文件夹，如果是，则是1
		if (fileFinder.IsDirectory() && !fileFinder.IsDots())  //若是目录则递归调用此方法
		{
			search(fileFinder.GetFilePath());
		}
		CString fileName = fileFinder.GetFileName();
		int dotPos = fileName.ReverseFind('.');//寻找.所在位置
		CString fileExt = fileName.Right(fileName.GetLength() - dotPos);//提取.及其右侧的字符串
		if (fileExt == _T(".mp3"))  //若是mp3文件
		{
			CString fileFullPath = fileFinder.GetFilePath();//获得该文件完整路径
			WriteToDocuments(fileFullPath);//将路径写入文件
		}
	}
	fileFinder.Close();//关闭文件

}

void CMusicDlg::WriteToDocuments(CString strSum)
{
	CFile cf;
	//if (!cf.Open(_T("D:\\TXL.TXT"), CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
	if (!cf.Open(_T("D:\\TXL.TXT"), CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
	{
		AfxMessageBox(_T("D:\\TXL.TXT \r\n Open failed when write."));
	}
	cf.SeekToEnd();
	CString str;

	str = "\n";
	int len = strSum.GetLength();
	int len2 = str.GetLength();
	cf.Write(strSum.GetBuffer(len), len * 2);
	cf.Write(str.GetBuffer(len2), len2 * 2);
	cf.Close();
	strSum.ReleaseBuffer();
}

void CMusicDlg::PassString()
{
	//m_MediaPlayer.put_URL(strFileNameView);
}

void MP3::getMP3Info(CString theFileName, int column)
{
	MP3 mp3_info;
	CString strTemp;
	strTemp = theFileName;
	FILE *fp;
	char file_name[200] = { '\n' };
	for (int i = 0; i<strTemp.GetLength(); i++)
	{
		file_name[i] = strTemp.GetAt(i);
	}

	fp = fopen(file_name, "r");
	if (fp == NULL)
	{
		CString str;
		str = "can't open file";

	}
	else
	{
		//mp3以末尾的128字节存放，偏移-128  
		fseek(fp, -128 * sizeof(char), SEEK_END);

		//读取标签头

		fread(mp3_info.header, sizeof(char), 3, fp);

		//标签头必须是"TAG"否则认为没有标签
		if (!(mp3_info.header[0] == 'T'
			&& mp3_info.header[1] == 'A'
			&& mp3_info.header[2] == 'G'))
		{
			//str.Format("%s contains no ID3 v1 tag", file_name);
		}
		else
		{
			//读取信息
			fread(mp3_info.title, sizeof(char), 30, fp);
			fread(mp3_info.artist, sizeof(char), 30, fp);
			fread(mp3_info.album, sizeof(char), 30, fp);
			fread(mp3_info.year, sizeof(char), 4, fp);
			fread(mp3_info.comment, sizeof(char), 30, fp);

			sTitle = mp3_info.title;
			sArtist = mp3_info.artist;
			sAlbum = mp3_info.album;
			CString s1, s2, s3, s4;
			s1 = mp3_info.year[0]; s2 = mp3_info.year[1]; s3 = mp3_info.year[2]; s4 = mp3_info.year[3];
			sYear = s1 + s2 + s3 + s4;
			sComment = mp3_info.comment;
		}
		fclose(fp);
	}
}


void CMusicDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);//将信息更新
	//MessageBox(m_AddMusic);
	ClearTheDocuments();
	search(m_AddMusic);//遍历该文件夹下所有文件
}




void CMusicDlg::OnSelchangeMusicdlgtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CRect tabRect;    // 标签控件客户区的Rect   

					  // 获取标签控件客户区Rect，并对其调整，以适合放置标签页   
	m_musicTab.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	switch (m_musicTab.GetCurSel())
	{
	case 0:
		SaveToDocuments();
		//当选中第一个标签页时，显示第一个窗口
		m_titleDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_artistDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;

	case 1:
		SaveToDocuments();
		//当选中第二个标签页时，显示第二个窗口
		m_titleDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_artistDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
}

void CMusicDlg::OnBnClickedPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MediaPlayer.put_URL(strFileNameView);
}

void CMusicDlg::SaveToDocuments()
{
	CFile cf;
	//if (!cf.Open(_T("D:\\TXL.TXT"), CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
	if (!cf.Open(_T("D:\\TXL.TXT"), CFile::modeReadWrite | CFile::modeCreate))
	{
		AfxMessageBox(_T("D:\\TXL.TXT \r\n Open failed when write."));
	}
	else
	{
		CString str1;
		CString str2;
		str2 = "\n";
		int len2 = str2.GetLength();
		list<CString>::iterator PathIterator;
		int i = 0;
		for (PathIterator = Path.begin(); PathIterator != Path.end(); ++PathIterator)
		{
			cf.SeekToEnd();
			str1 = *PathIterator;
			int len = str1.GetLength();
			cf.Write(str1.GetBuffer(len), len * 2);
			cf.Write(str2.GetBuffer(len2), len2 * 2);
			str1.ReleaseBuffer();
			str2.ReleaseBuffer();
			str1 = "";
		}
		cf.Close();
	}
}

void CMusicDlg::ReadPlayListFromDocuments()
{
	PlayList.clear();
	CFile  file(L"D:\\PlayerTemp\\PlayList\\PlayList1.TXT", CFile::modeRead);
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
	while ((iPos = strTemp.Find('\n')) != -1) //找到一个.索引时从0开始的
	{
		PlayList.push_back(strTemp.Left(iPos));//获取一个元素，并插入链表
		strTemp.Delete(0, iPos + 1);    // 删除头部
	}
	PlayList.push_back(strTemp);
	if (PlayList.empty())
	{
		CString strs;
		strs = "EMPTY";
		MessageBox(strs);
	}

	list<CString>::iterator PlayListIterator;
	int i = 0;
	for (PlayListIterator = PlayList.begin(); PlayListIterator != PlayList.end(); ++PlayListIterator, i++)
	{
		InsrtToPlayListControl(*PlayListIterator, i);
	}
}

void CMusicDlg::InsrtToPlayListControl(CString theFilePath, int column)
{
	MP3 t;
	t.getMP3Info(theFilePath, column);
	m_PlayList.InsertItem(column, t.sTitle);
	m_PlayList.SetItemText(column, 1, theFilePath);
}


void CMusicDlg::OnBnClickedUpdatePlaylist()
{
	// TODO: 在此添加控件通知处理程序代码
	m_PlayList.DeleteAllItems();
	ReadPlayListFromDocuments();
}


void CMusicDlg::OnBnClickedStartplay()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MediaPlayer.put_URL(strFileNameView);
}


void CMusicDlg::DeleteArtists()
{
	CFile cf1;
	cf1.Open(_T("D:\\PlayerTemp\\PlayList\\PlayList1.txt"), CFile::modeWrite, NULL);
	cf1.SetLength(0);
	cf1.Close();

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


int playListNum;
void CMusicDlg::OnRclickPlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU1));

		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);

		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		strFileNameView = m_PlayList.GetItemText(pNMListView->iItem, 2);
		playListNum = m_PlayList.GetSelectionMark();
	}
	*pResult = 0;
}


void CMusicDlg::OnDeleteFromPlayList()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	str = m_PlayList.GetItemText(playListNum, 1);
	m_PlayList.DeleteItem(playListNum);
	//iter被删除后，再iter++指向的元素已经不对了，因为iter指针指向的元素指针（暂且这样叫）已经被删除，变成了野指针，无法再通过iter++正确的找到下一个元素。
	//再调用erase方法时会返回迭代器，就是返回当前删除的下一个。
	list<CString>::iterator thePlayListIterator;
	int i = 0;
	for (thePlayListIterator = PlayList.begin(); thePlayListIterator != PlayList.end();)
	{
		if ((*thePlayListIterator).Compare(str) == 0)
		{
			thePlayListIterator = PlayList.erase(thePlayListIterator);
			//MessageBox(_T("删除成功！"));
		}
		else
		{
			thePlayListIterator++;
		}
	}


	CFile cf;
	if (!cf.Open(_T("D:\\PlayerTemp\\PlayList\\PlayList1.TXT"), CFile::modeWrite | CFile::modeCreate))
	{
		AfxMessageBox(_T("D:\\PlayerTemp\\PlayList\\PlayList1.TXT \r\n Open failed when write."));
	}
	else
	{
		for (thePlayListIterator = PlayList.begin(); thePlayListIterator != PlayList.end(); thePlayListIterator++)
		{
			CString str;
			str = "\n";
			int len1 = (*thePlayListIterator).GetLength();
			int len2 = str.GetLength();
			if (thePlayListIterator == PlayList.begin());
			else cf.SeekToEnd();
			cf.Write((*thePlayListIterator).GetBuffer(len1), len1 * 2);
			cf.Write(str.GetBuffer(len2), len2 * 2);
			(*thePlayListIterator).ReleaseBuffer();
			str.ReleaseBuffer();
		}
	}
	cf.Close();
}


void CMusicDlg::OnBnClickedClearfull()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile cf;
	cf.Open(_T("D:\\PlayerTemp\\PlayList\\PlayList1.txt"), CFile::modeWrite, NULL);
	cf.SetLength(0);
	cf.Close();
	m_PlayList.DeleteAllItems();
}


void CMusicDlg::OnClickPlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
		strFileNameView = m_PlayList.GetItemText(pNMListView->iItem, 1);
		// 将选择的语言显示与编辑框中   
	}
	*pResult = 0;
}
