// PictureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MediaPlayer.h"
#include "PictureDlg.h"
#include "afxdialogex.h"
#include "Picture.h"
CFont font3;
CPicture *picture = new CPicture[1000];
// CPictureDlg 对话框

IMPLEMENT_DYNAMIC(CPictureDlg, CDialogEx)

CPictureDlg::CPictureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PICTUREDLG, pParent)
	, m_AddToPicture(_T(""))
{

}

CPictureDlg::~CPictureDlg()
{
}

void CPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURELIST, m_pictureList);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_PictureBrowseCtrl);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_AddToPicture);
}


BEGIN_MESSAGE_MAP(CPictureDlg, CDialogEx)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CPictureDlg::OnEnChangeMfceditbrowse1)
	ON_NOTIFY(NM_CLICK, IDC_PICTURELIST, &CPictureDlg::OnClickPicturelist)
	ON_STN_CLICKED(IDC_PICTURE_STATIC, &CPictureDlg::OnStnClickedPictureStatic)
	ON_NOTIFY(NM_RCLICK, IDC_PICTURELIST, &CPictureDlg::OnRclickPicturelist)
	ON_BN_CLICKED(IDC_PLAY_BUTTON, &CPictureDlg::OnBnClickedPlayButton)
	ON_NOTIFY(LVN_KEYDOWN, IDC_PICTURELIST, &CPictureDlg::OnLvnKeydownList1)
	ON_COMMAND(ID_PICTUREDELETE_32778, &CPictureDlg::OnPicturedeleteFromList)
END_MESSAGE_MAP()


// CPictureDlg 消息处理程序
BOOL CPictureDlg::OnInitDialog()//初始化菜单窗口
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

									// TODO: 在此添加额外的初始化代码
	CRect rect;

	// 获取列表视图控件的位置和大小
	m_pictureList.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格
	m_pictureList.SetExtendedStyle(m_pictureList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加六列
	m_pictureList.InsertColumn(0, _T("标题"), LVCFMT_CENTER, rect.Width() / 4, 0);
	m_pictureList.InsertColumn(1, _T("宽度"), LVCFMT_CENTER, rect.Width() / 4, 1);
	m_pictureList.InsertColumn(2, _T("高度"), LVCFMT_CENTER, rect.Width() / 4, 2);
	m_pictureList.InsertColumn(3, _T("大小"), LVCFMT_CENTER, rect.Width() / 4, 3);


	font3.CreatePointFont(240, _T("微软雅黑"));
	GetDlgItem(IDC_PICTUREHEAD)->SetFont(&font3);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CPictureDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	search(m_AddToPicture);
}

int col = -1;
void CPictureDlg::search(CString tp)//遍历选定文件夹tp
{
	CFileFind fileFinder;//文件查找者
	CString FolderPath = tp + _T("\\*.*");//一个文件夹的路径
	BOOL bFinished = fileFinder.FindFile(FolderPath);

                             //行数

	while (bFinished)
	{
		bFinished = fileFinder.FindNextFile();

		if (fileFinder.IsDirectory() && !fileFinder.IsDots())
			search(fileFinder.GetFilePath());//如果文件夹中还有一个文件夹，就递归地查看

		CString fileName = fileFinder.GetFileName();//文件名

		int dotPos = fileName.ReverseFind('.');//“点”的位置

		CString fileExt = fileName.Right(fileName.GetLength() - dotPos);//切割出点及其右面的字符串

		if (fileExt == _T(".bmp")) //筛选出bmp文件执行以下操作
		{

			col++;//表格行标下移一次
			picture[col].Name = fileName;//将文件名和文件地址建立联系
			picture[col].Path = fileFinder.GetFilePath();//将文件名和文件地址建立联系

			BITMAPFILEHEADER bmpHeader;                    //文件头
			BITMAPINFOHEADER bmpInfo;                      //信息头
			CFile bmpFile;                                 //记录打开文件
			CString strFileName = picture[col].Path;               //记录选择文件路径
			char *p = new char[12];                          //用于将长整型转换为字符串
			CString fff;                                   //用于将长整型转换为字符串


			if (!bmpFile.Open(strFileName, CFile::modeRead | CFile::typeBinary))
				return;//以只读的方式打开文件

			if (bmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
			{
				AfxMessageBox(_T("read bmp header failed!"));
				return;
			}
			if (bmpHeader.bfType != 0x4d42)
			{
				AfxMessageBox(_T("invalid file type!"));
				return;
			}
			if (bmpFile.Read(&bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
			{
				AfxMessageBox(_T("read bmp infor header failed!"));
				return;
			}

			m_pictureList.InsertItem(col, fileName);                               //填表第一列
			LongToStr(bmpInfo.biWidth, p);
			CString cstr1(p);
			m_pictureList.SetItemText(col, 1, cstr1);                               //填表第二列
			LongToStr(bmpInfo.biHeight, p);
			CString cstr2(p);
			m_pictureList.SetItemText(col, 2, cstr2);                                //填表第三列
			LongToStr(bmpInfo.biSize, p);
			CString cstr3(p);
			m_pictureList.SetItemText(col, 3, cstr3);                               //填表第四列           
		}
	}
	fileFinder.Close();
}

template <class T>
void CPictureDlg::LongToStr(T a, char *p)
{
	T value = a;
	int i = 0;
	int j = 0;
	char tmp;
	do
	{
		p[i++] = value % 10 + 48;
		value = value / 10;
	} while (value > 0);
	p[i--] = 0;
	for (; j<i; i--, j++)
	{
		tmp = p[i];
		p[i] = p[j];
		p[j] = tmp;
	}
}




void CPictureDlg::OnClickPicturelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString FileName;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)// 如果iItem不是-1，就说明有列表项被选择   
		FileName = m_pictureList.GetItemText(pNMListView->iItem, 0);// 获取被选择列表项第一个子项的文本

	BITMAPFILEHEADER bmpHeader;                    //文件头
	BITMAPINFOHEADER bmpInfo;                      //信息头
	CFile bmpFile;                                 //记录打开文件

	int i = 0;//计数变量
	for (; i < 1000; i++)//遍历数组，找到选中文件
		if (picture[i].Name == FileName)
			break;
	CString FilePath(picture[i].Path);//记录选中的文件路径

									  //以只读的方式打开文件
	if (!bmpFile.Open(FilePath, CFile::modeRead | CFile::typeBinary))
		return;
	if (bmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
	{
		AfxMessageBox(_T("read bmp header failed!"));
		return;
	}
	if (bmpHeader.bfType != 0x4d42)
	{
		AfxMessageBox(_T("invalid file type!"));
		return;
	}
	if (bmpFile.Read(&bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
	{
		AfxMessageBox(_T("read bmp infor header failed!"));
		return;
	}
	if (bmpInfo.biBitCount != 24)
	{
		AfxMessageBox(_T("File is not 24 bit.Application doesn't support this kind of file!"));
		return;
	}
	BITMAPINFO * pBmpInfo = (BITMAPINFO *)new char[sizeof(BITMAPINFOHEADER)];
	if (!pBmpInfo)
	{
		AfxMessageBox(_T("memory error!"));
		return;
	}
	//为图像数据申请空间
	memcpy(pBmpInfo, &bmpInfo, sizeof(BITMAPINFOHEADER));
	DWORD dataBytes = bmpHeader.bfSize - bmpHeader.bfOffBits;
	BYTE* pBmpData = (BYTE*)new char[dataBytes];
	if (!pBmpData)
	{
		AfxMessageBox(_T("memory error!"));
		delete pBmpData;
		return;
	}
	if (bmpFile.Read(pBmpData, dataBytes) != dataBytes)
	{
		AfxMessageBox(_T("Read bmp data failed!"));
		delete pBmpInfo;
		delete pBmpData;
		return;
	}
	bmpFile.Close();

	CWnd *pWnd = GetDlgItem(IDC_PICTURE_STATIC);//获得pictrue控件窗口的句柄
	CRect rect;
	pWnd->GetClientRect(&rect);//获得pictrue控件所在的矩形区域
	CDC *pDC = pWnd->GetDC();//获得pictrue控件的DC
							 //显示图片
	pDC->SetStretchBltMode(COLORONCOLOR);

	StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpInfo.biWidth, bmpInfo.biHeight, pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	/*	iBmpWidth = bmpInfo.biWidth;
	iBmpHeight = bmpInfo.biHeight;*/
}


void CPictureDlg::OnStnClickedPictureStatic()
{
	// TODO: 在此添加控件通知处理程序代码
}





int num2;
void CPictureDlg::OnRclickPicturelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU3));

		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		//strFileNameView = m_TitleList.GetItemText(pNMListView->iItem, 5);
		num2 = m_pictureList.GetSelectionMark();
	}
	*pResult = 0;
}



void CPictureDlg::OnBnClickedPlayButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pictureList.GetFirstSelectedItemPosition() == NULL)//从未选择过第几行
	{

		MessageBox(_T("将从第一张图片开始放映。"));
		CString FileName;
		BITMAPFILEHEADER bmpHeader;                    //文件头
		BITMAPINFOHEADER bmpInfo;                      //信息头
		CFile bmpFile;                                 //记录打开文件
		int j, i;                                      //计数变量
		for (i = 0; i < m_pictureList.GetItemCount(); i++)
		{
			FileName = m_pictureList.GetItemText(i, 0);// 获取被选择列表项第一个子项的文本
			for (j = 0; j < 1000; j++)               //遍历数组，找到选中文件
				if (picture[j].Name == FileName)
					break;

			//以只读的方式打开文件								
			if (!bmpFile.Open(picture[j].Path, CFile::modeRead | CFile::typeBinary))
				return;
			if (bmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
			{
				AfxMessageBox(_T("read bmp header failed!"));
				return;
			}
			if (bmpFile.Read(&bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
			{
				AfxMessageBox(_T("read bmp infor header failed!"));
				return;
			}
			BITMAPINFO * pBmpInfo = (BITMAPINFO *)new char[sizeof(BITMAPINFOHEADER)];
			//为图像数据申请空间
			memcpy(pBmpInfo, &bmpInfo, sizeof(BITMAPINFOHEADER));
			DWORD dataBytes = bmpHeader.bfSize - bmpHeader.bfOffBits;
			BYTE* pBmpData = (BYTE*)new char[dataBytes];
			if (!pBmpData)
			{
				AfxMessageBox(_T("memory error!"));
				delete pBmpData;
				return;
			}
			if (bmpFile.Read(pBmpData, dataBytes) != dataBytes)
			{
				AfxMessageBox(_T("Read bmp data failed!"));
				delete pBmpInfo;
				delete pBmpData;
				return;
			}
			bmpFile.Close();

			CWnd *pWnd = GetDlgItem(IDC_PICTURE_STATIC);//获得pictrue控件窗口的句柄
			CRect rect;
			pWnd->GetClientRect(&rect);//获得pictrue控件所在的矩形区域
			CDC *pDC = pWnd->GetDC();//获得pictrue控件的DC
									 //显示图片
			pDC->SetStretchBltMode(COLORONCOLOR);
			StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpInfo.biWidth, bmpInfo.biHeight, pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);
			Sleep(888);//时间延迟函数
		}
	}
	else
	{
		POSITION tmpPOSITION;
		POSITION & pos = tmpPOSITION;
		CString FileName;
		BITMAPFILEHEADER bmpHeader;                    //文件头
		BITMAPINFOHEADER bmpInfo;                      //信息头
		CFile bmpFile;                                 //记录打开文件
		int i, j;
		pos = m_pictureList.GetFirstSelectedItemPosition();
		for (i = m_pictureList.GetNextSelectedItem(pos); i< m_pictureList.GetItemCount(); i++)
		{
			FileName = m_pictureList.GetItemText(i, 0);
			for (j = 0; j < 1000; j++)//遍历数组，找到选中文件
				if (picture[j].Name == FileName)
					break;
			CString FilePath(picture[j].Path);//记录选中的文件路径

											  //以只读的方式打开文件								
			if (!bmpFile.Open(FilePath, CFile::modeRead | CFile::typeBinary))
				return;
			if (bmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
			{
				AfxMessageBox(_T("read bmp header failed!"));
				return;
			}
			if (bmpFile.Read(&bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
			{
				AfxMessageBox(_T("read bmp infor header failed!"));
				return;
			}
			BITMAPINFO * pBmpInfo = (BITMAPINFO *)new char[sizeof(BITMAPINFOHEADER)];
			if (!pBmpInfo)
			{
				AfxMessageBox(_T("memory error!"));
				return;
			}
			//为图像数据申请空间
			memcpy(pBmpInfo, &bmpInfo, sizeof(BITMAPINFOHEADER));
			DWORD dataBytes = bmpHeader.bfSize - bmpHeader.bfOffBits;
			BYTE* pBmpData = (BYTE*)new char[dataBytes];
			if (bmpFile.Read(pBmpData, dataBytes) != dataBytes)
			{
				AfxMessageBox(_T("Read bmp data failed!"));
				delete pBmpInfo;
				delete pBmpData;
				return;
			}
			bmpFile.Close();

			CWnd *pWnd = GetDlgItem(IDC_PICTURE_STATIC);//获得pictrue控件窗口的句柄
			CRect rect;
			pWnd->GetClientRect(&rect);//获得pictrue控件所在的矩形区域
			CDC *pDC = pWnd->GetDC();//获得pictrue控件的DC
									 //显示图片
			pDC->SetStretchBltMode(COLORONCOLOR);
			StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpInfo.biWidth, bmpInfo.biHeight, pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);
			Sleep(888);//时间延迟函数
		}
	}
}


////////////////////////////////////////////按上下键选中图片//////////////////////////////////////////////////
UINT DelayLoop(LPVOID pPatam)//使用了延时线程
{
	CPictureDlg* pDlg = (CPictureDlg*)pPatam;
	Sleep(100);
	pDlg->KeyDownUpdate();
	return 0;
}
void CPictureDlg::OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)//方向键控制选项
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	AfxBeginThread(DelayLoop, this);
}

void CPictureDlg::KeyDownUpdate()
{
	POSITION pos = m_pictureList.GetFirstSelectedItemPosition();

	int Item = m_pictureList.GetNextSelectedItem(pos);

	CString FileName = m_pictureList.GetItemText(Item, 0);// 获取被选择列表项第一个子项的文本	
														  
/////////////////////////////////////下面是图片显示/////////////////////////////////////////////////////////
	BITMAPFILEHEADER bmpHeader;                    //文件头
	BITMAPINFOHEADER bmpInfo;                      //信息头
	CFile bmpFile;                                 //记录打开文件

	int i = 0;//计数变量
	for (; i < 1000; i++)//遍历数组，找到选中文件
		if (picture[i].Name == FileName)
			break;
	CString FilePath(picture[i].Path);//记录选中的文件路径

									  //以只读的方式打开文件
	if (!bmpFile.Open(FilePath, CFile::modeRead | CFile::typeBinary))
		return;
	if (bmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
	{
		AfxMessageBox(_T("read bmp header failed!"));
		return;
	}
	if (bmpHeader.bfType != 0x4d42)
	{
		AfxMessageBox(_T("invalid file type!"));
		return;
	}
	if (bmpFile.Read(&bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
	{
		AfxMessageBox(_T("read bmp infor header failed!"));
		return;
	}
	if (bmpInfo.biBitCount != 24)
	{
		AfxMessageBox(_T("File is not 24 bit.Application doesn't support this kind of file!"));
		return;
	}
	BITMAPINFO * pBmpInfo = (BITMAPINFO *)new char[sizeof(BITMAPINFOHEADER)];
	if (!pBmpInfo)
	{
		AfxMessageBox(_T("memory error!"));
		return;
	}
	//为图像数据申请空间
	memcpy(pBmpInfo, &bmpInfo, sizeof(BITMAPINFOHEADER));
	DWORD dataBytes = bmpHeader.bfSize - bmpHeader.bfOffBits;
	BYTE* pBmpData = (BYTE*)new char[dataBytes];
	if (!pBmpData)
	{
		AfxMessageBox(_T("memory error!"));
		delete pBmpData;
		return;
	}
	if (bmpFile.Read(pBmpData, dataBytes) != dataBytes)
	{
		AfxMessageBox(_T("Read bmp data failed!"));
		delete pBmpInfo;
		delete pBmpData;
		return;
	}
	bmpFile.Close();

	CWnd *pWnd = GetDlgItem(IDC_PICTURE_STATIC);//获得pictrue控件窗口的句柄
	CRect rect;
	pWnd->GetClientRect(&rect);//获得pictrue控件所在的矩形区域
	CDC *pDC = pWnd->GetDC();//获得pictrue控件的DC
	//显示图片
	pDC->SetStretchBltMode(COLORONCOLOR);
	StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpInfo.biWidth, bmpInfo.biHeight, pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);	
	delete pBmpInfo;
	delete pBmpData;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void CPictureDlg::OnPicturedeleteFromList()
{
	// TODO: 在此添加命令处理程序代码
	m_pictureList.DeleteItem(num2);
}

