// MusicDlg.cpp : ʵ���ļ�
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

// CMusicDlg �Ի���

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


// CMusicDlg ��Ϣ�������
BOOL CMusicDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	DeleteArtists();
	CRect tabRect;   // ��ǩ�ؼ��ͻ�����λ�úʹ�С   
	CRect rect;

	m_musicTab.InsertItem(0, _T("���ּ�"));	//tab��Ӧ��һ������      
	m_musicTab.InsertItem(1, _T("������"));  //tab��Ӧ�ڶ�������    
	m_titleDlg.Create(IDD_TITLEDLG, &m_musicTab);    // ������һ����ǩҳ   
	m_artistDlg.Create(IDD_ARTISTDLG, &m_musicTab); // �����ڶ�����ǩҳ   
	
	m_musicTab.GetClientRect(&tabRect);    // ��ȡ��ǩ�ؼ��ͻ���Rect   
									  // ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ   
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	// ���ݵ����õ�tabRect����m_TitleDlg�ӶԻ��򣬲�����Ϊ��ʾ   
	m_titleDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	// ���ݵ����õ�tabRect����m_ArtistDlg�ӶԻ��򣬲�����Ϊ����   
	m_artistDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	
	font.CreatePointFont(240, _T("΢���ź�"));
	GetDlgItem(IDC_MUSICHEAD)->SetFont(&font);

	m_PlayList.GetClientRect(&rect);
	m_PlayList.SetExtendedStyle(m_PlayList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_PlayList.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect.Width(), 0);
	m_PlayList.InsertColumn(1, _T("·��"), LVCFMT_CENTER, 0, 1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
	CFileFind fileFinder;	//ʵ����һ��CFileFind ����
	CString filePath = fp + _T("\\*.*");	//��·���������
	BOOL bFinished = fileFinder.FindFile(filePath);//�������ж��Ƿ������
	while (bFinished)
	{
		i++;
		bFinished = fileFinder.FindNextFile();//FindNextFile�Ǵ�FindFileָ����·����ʼ����
		//IsDirectory�ж��Ƿ����ļ��У�����ǣ�����1
		if (fileFinder.IsDirectory() && !fileFinder.IsDots())  //����Ŀ¼��ݹ���ô˷���
		{
			search(fileFinder.GetFilePath());
		}
		CString fileName = fileFinder.GetFileName();
		int dotPos = fileName.ReverseFind('.');//Ѱ��.����λ��
		CString fileExt = fileName.Right(fileName.GetLength() - dotPos);//��ȡ.�����Ҳ���ַ���
		if (fileExt == _T(".mp3"))  //����mp3�ļ�
		{
			CString fileFullPath = fileFinder.GetFilePath();//��ø��ļ�����·��
			WriteToDocuments(fileFullPath);//��·��д���ļ�
		}
	}
	fileFinder.Close();//�ر��ļ�

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
		//mp3��ĩβ��128�ֽڴ�ţ�ƫ��-128  
		fseek(fp, -128 * sizeof(char), SEEK_END);

		//��ȡ��ǩͷ

		fread(mp3_info.header, sizeof(char), 3, fp);

		//��ǩͷ������"TAG"������Ϊû�б�ǩ
		if (!(mp3_info.header[0] == 'T'
			&& mp3_info.header[1] == 'A'
			&& mp3_info.header[2] == 'G'))
		{
			//str.Format("%s contains no ID3 v1 tag", file_name);
		}
		else
		{
			//��ȡ��Ϣ
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);//����Ϣ����
	//MessageBox(m_AddMusic);
	ClearTheDocuments();
	search(m_AddMusic);//�������ļ����������ļ�
}




void CMusicDlg::OnSelchangeMusicdlgtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CRect tabRect;    // ��ǩ�ؼ��ͻ�����Rect   

					  // ��ȡ��ǩ�ؼ��ͻ���Rect����������������ʺϷ��ñ�ǩҳ   
	m_musicTab.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	switch (m_musicTab.GetCurSel())
	{
	case 0:
		SaveToDocuments();
		//��ѡ�е�һ����ǩҳʱ����ʾ��һ������
		m_titleDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_artistDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;

	case 1:
		SaveToDocuments();
		//��ѡ�еڶ�����ǩҳʱ����ʾ�ڶ�������
		m_titleDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_artistDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
}

void CMusicDlg::OnBnClickedPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	int iPosBegin = -1;   //����һ���α�
	if ((iPosBegin = strTemp.Find('D')) != -1) //�ҵ�һ��.����ʱ��0��ʼ��
	{
		strTemp.Delete(0, iPosBegin);    // ɾ��ͷ��
	}//������д���ļ�ʱD:\\ǰ�滹���ַ���������û�����ļ�����ʾ����������Ҫ�ֶ�ɾ��

	int iPos = -1;   //����һ���α�
	while ((iPos = strTemp.Find('\n')) != -1) //�ҵ�һ��.����ʱ��0��ʼ��
	{
		PlayList.push_back(strTemp.Left(iPos));//��ȡһ��Ԫ�أ�����������
		strTemp.Delete(0, iPos + 1);    // ɾ��ͷ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_PlayList.DeleteAllItems();
	ReadPlayListFromDocuments();
}


void CMusicDlg::OnBnClickedStartplay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


int playListNum;
void CMusicDlg::OnRclickPlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���������������
	CString str;
	str = m_PlayList.GetItemText(playListNum, 1);
	m_PlayList.DeleteItem(playListNum);
	//iter��ɾ������iter++ָ���Ԫ���Ѿ������ˣ���Ϊiterָ��ָ���Ԫ��ָ�루���������У��Ѿ���ɾ���������Ұָ�룬�޷���ͨ��iter++��ȷ���ҵ���һ��Ԫ�ء�
	//�ٵ���erase����ʱ�᷵�ص����������Ƿ��ص�ǰɾ������һ����
	list<CString>::iterator thePlayListIterator;
	int i = 0;
	for (thePlayListIterator = PlayList.begin(); thePlayListIterator != PlayList.end();)
	{
		if ((*thePlayListIterator).Compare(str) == 0)
		{
			thePlayListIterator = PlayList.erase(thePlayListIterator);
			//MessageBox(_T("ɾ���ɹ���"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFile cf;
	cf.Open(_T("D:\\PlayerTemp\\PlayList\\PlayList1.txt"), CFile::modeWrite, NULL);
	cf.SetLength(0);
	cf.Close();
	m_PlayList.DeleteAllItems();
}


void CMusicDlg::OnClickPlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
	{
		// ��ȡ��ѡ���б����һ��������ı�   
		strFileNameView = m_PlayList.GetItemText(pNMListView->iItem, 1);
		// ��ѡ���������ʾ��༭����   
	}
	*pResult = 0;
}
