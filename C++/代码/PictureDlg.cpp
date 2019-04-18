// PictureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MediaPlayer.h"
#include "PictureDlg.h"
#include "afxdialogex.h"
#include "Picture.h"
CFont font3;
CPicture *picture = new CPicture[1000];
// CPictureDlg �Ի���

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


// CPictureDlg ��Ϣ�������
BOOL CPictureDlg::OnInitDialog()//��ʼ���˵�����
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

									// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect rect;

	// ��ȡ�б���ͼ�ؼ���λ�úʹ�С
	m_pictureList.GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	m_pictureList.SetExtendedStyle(m_pictureList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������
	m_pictureList.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect.Width() / 4, 0);
	m_pictureList.InsertColumn(1, _T("���"), LVCFMT_CENTER, rect.Width() / 4, 1);
	m_pictureList.InsertColumn(2, _T("�߶�"), LVCFMT_CENTER, rect.Width() / 4, 2);
	m_pictureList.InsertColumn(3, _T("��С"), LVCFMT_CENTER, rect.Width() / 4, 3);


	font3.CreatePointFont(240, _T("΢���ź�"));
	GetDlgItem(IDC_PICTUREHEAD)->SetFont(&font3);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CPictureDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	search(m_AddToPicture);
}

int col = -1;
void CPictureDlg::search(CString tp)//����ѡ���ļ���tp
{
	CFileFind fileFinder;//�ļ�������
	CString FolderPath = tp + _T("\\*.*");//һ���ļ��е�·��
	BOOL bFinished = fileFinder.FindFile(FolderPath);

                             //����

	while (bFinished)
	{
		bFinished = fileFinder.FindNextFile();

		if (fileFinder.IsDirectory() && !fileFinder.IsDots())
			search(fileFinder.GetFilePath());//����ļ����л���һ���ļ��У��͵ݹ�ز鿴

		CString fileName = fileFinder.GetFileName();//�ļ���

		int dotPos = fileName.ReverseFind('.');//���㡱��λ��

		CString fileExt = fileName.Right(fileName.GetLength() - dotPos);//�и���㼰��������ַ���

		if (fileExt == _T(".bmp")) //ɸѡ��bmp�ļ�ִ�����²���
		{

			col++;//����б�����һ��
			picture[col].Name = fileName;//���ļ������ļ���ַ������ϵ
			picture[col].Path = fileFinder.GetFilePath();//���ļ������ļ���ַ������ϵ

			BITMAPFILEHEADER bmpHeader;                    //�ļ�ͷ
			BITMAPINFOHEADER bmpInfo;                      //��Ϣͷ
			CFile bmpFile;                                 //��¼���ļ�
			CString strFileName = picture[col].Path;               //��¼ѡ���ļ�·��
			char *p = new char[12];                          //���ڽ�������ת��Ϊ�ַ���
			CString fff;                                   //���ڽ�������ת��Ϊ�ַ���


			if (!bmpFile.Open(strFileName, CFile::modeRead | CFile::typeBinary))
				return;//��ֻ���ķ�ʽ���ļ�

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

			m_pictureList.InsertItem(col, fileName);                               //����һ��
			LongToStr(bmpInfo.biWidth, p);
			CString cstr1(p);
			m_pictureList.SetItemText(col, 1, cstr1);                               //���ڶ���
			LongToStr(bmpInfo.biHeight, p);
			CString cstr2(p);
			m_pictureList.SetItemText(col, 2, cstr2);                                //��������
			LongToStr(bmpInfo.biSize, p);
			CString cstr3(p);
			m_pictureList.SetItemText(col, 3, cstr3);                               //��������           
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CString FileName;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)// ���iItem����-1����˵�����б��ѡ��   
		FileName = m_pictureList.GetItemText(pNMListView->iItem, 0);// ��ȡ��ѡ���б����һ��������ı�

	BITMAPFILEHEADER bmpHeader;                    //�ļ�ͷ
	BITMAPINFOHEADER bmpInfo;                      //��Ϣͷ
	CFile bmpFile;                                 //��¼���ļ�

	int i = 0;//��������
	for (; i < 1000; i++)//�������飬�ҵ�ѡ���ļ�
		if (picture[i].Name == FileName)
			break;
	CString FilePath(picture[i].Path);//��¼ѡ�е��ļ�·��

									  //��ֻ���ķ�ʽ���ļ�
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
	//Ϊͼ����������ռ�
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

	CWnd *pWnd = GetDlgItem(IDC_PICTURE_STATIC);//���pictrue�ؼ����ڵľ��
	CRect rect;
	pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������
	CDC *pDC = pWnd->GetDC();//���pictrue�ؼ���DC
							 //��ʾͼƬ
	pDC->SetStretchBltMode(COLORONCOLOR);

	StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpInfo.biWidth, bmpInfo.biHeight, pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	/*	iBmpWidth = bmpInfo.biWidth;
	iBmpHeight = bmpInfo.biHeight;*/
}


void CPictureDlg::OnStnClickedPictureStatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}





int num2;
void CPictureDlg::OnRclickPicturelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pictureList.GetFirstSelectedItemPosition() == NULL)//��δѡ����ڼ���
	{

		MessageBox(_T("���ӵ�һ��ͼƬ��ʼ��ӳ��"));
		CString FileName;
		BITMAPFILEHEADER bmpHeader;                    //�ļ�ͷ
		BITMAPINFOHEADER bmpInfo;                      //��Ϣͷ
		CFile bmpFile;                                 //��¼���ļ�
		int j, i;                                      //��������
		for (i = 0; i < m_pictureList.GetItemCount(); i++)
		{
			FileName = m_pictureList.GetItemText(i, 0);// ��ȡ��ѡ���б����һ��������ı�
			for (j = 0; j < 1000; j++)               //�������飬�ҵ�ѡ���ļ�
				if (picture[j].Name == FileName)
					break;

			//��ֻ���ķ�ʽ���ļ�								
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
			//Ϊͼ����������ռ�
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

			CWnd *pWnd = GetDlgItem(IDC_PICTURE_STATIC);//���pictrue�ؼ����ڵľ��
			CRect rect;
			pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������
			CDC *pDC = pWnd->GetDC();//���pictrue�ؼ���DC
									 //��ʾͼƬ
			pDC->SetStretchBltMode(COLORONCOLOR);
			StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpInfo.biWidth, bmpInfo.biHeight, pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);
			Sleep(888);//ʱ���ӳٺ���
		}
	}
	else
	{
		POSITION tmpPOSITION;
		POSITION & pos = tmpPOSITION;
		CString FileName;
		BITMAPFILEHEADER bmpHeader;                    //�ļ�ͷ
		BITMAPINFOHEADER bmpInfo;                      //��Ϣͷ
		CFile bmpFile;                                 //��¼���ļ�
		int i, j;
		pos = m_pictureList.GetFirstSelectedItemPosition();
		for (i = m_pictureList.GetNextSelectedItem(pos); i< m_pictureList.GetItemCount(); i++)
		{
			FileName = m_pictureList.GetItemText(i, 0);
			for (j = 0; j < 1000; j++)//�������飬�ҵ�ѡ���ļ�
				if (picture[j].Name == FileName)
					break;
			CString FilePath(picture[j].Path);//��¼ѡ�е��ļ�·��

											  //��ֻ���ķ�ʽ���ļ�								
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
			//Ϊͼ����������ռ�
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

			CWnd *pWnd = GetDlgItem(IDC_PICTURE_STATIC);//���pictrue�ؼ����ڵľ��
			CRect rect;
			pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������
			CDC *pDC = pWnd->GetDC();//���pictrue�ؼ���DC
									 //��ʾͼƬ
			pDC->SetStretchBltMode(COLORONCOLOR);
			StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpInfo.biWidth, bmpInfo.biHeight, pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);
			Sleep(888);//ʱ���ӳٺ���
		}
	}
}


////////////////////////////////////////////�����¼�ѡ��ͼƬ//////////////////////////////////////////////////
UINT DelayLoop(LPVOID pPatam)//ʹ������ʱ�߳�
{
	CPictureDlg* pDlg = (CPictureDlg*)pPatam;
	Sleep(100);
	pDlg->KeyDownUpdate();
	return 0;
}
void CPictureDlg::OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)//���������ѡ��
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	AfxBeginThread(DelayLoop, this);
}

void CPictureDlg::KeyDownUpdate()
{
	POSITION pos = m_pictureList.GetFirstSelectedItemPosition();

	int Item = m_pictureList.GetNextSelectedItem(pos);

	CString FileName = m_pictureList.GetItemText(Item, 0);// ��ȡ��ѡ���б����һ��������ı�	
														  
/////////////////////////////////////������ͼƬ��ʾ/////////////////////////////////////////////////////////
	BITMAPFILEHEADER bmpHeader;                    //�ļ�ͷ
	BITMAPINFOHEADER bmpInfo;                      //��Ϣͷ
	CFile bmpFile;                                 //��¼���ļ�

	int i = 0;//��������
	for (; i < 1000; i++)//�������飬�ҵ�ѡ���ļ�
		if (picture[i].Name == FileName)
			break;
	CString FilePath(picture[i].Path);//��¼ѡ�е��ļ�·��

									  //��ֻ���ķ�ʽ���ļ�
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
	//Ϊͼ����������ռ�
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

	CWnd *pWnd = GetDlgItem(IDC_PICTURE_STATIC);//���pictrue�ؼ����ڵľ��
	CRect rect;
	pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������
	CDC *pDC = pWnd->GetDC();//���pictrue�ؼ���DC
	//��ʾͼƬ
	pDC->SetStretchBltMode(COLORONCOLOR);
	StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, bmpInfo.biWidth, bmpInfo.biHeight, pBmpData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);	
	delete pBmpInfo;
	delete pBmpData;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void CPictureDlg::OnPicturedeleteFromList()
{
	// TODO: �ڴ���������������
	m_pictureList.DeleteItem(num2);
}

