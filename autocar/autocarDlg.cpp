#include "stdafx.h"
#include "autocarDlg.h"
#include "strngs.h"

#include <math.h>
#include <stdio.h>
#include "resource.h"

#pragma comment(lib,"libtesseract302.lib")

//定义串口数据接收消息常量
#define WM_RECV_SERIAL_DATA WM_USER + 101
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CautocarDlg 对话框

CautocarDlg::CautocarDlg(CWnd* pParent /*=NULL*/)
  : CDialog(IDD, pParent)
  , _textBox_serialSend(_T(""))
  , _textBox_serialReceive(_T(""))
  , m_cgrgb1(_T(""))
  , m_cgrgb2(_T(""))
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_modeall = 0;
  m_modeevery = 0;
  m_modeeveryold = 0;
}

// CautocarDlg 消息处理程序
//初始化
BOOL CautocarDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // 将“关于...”菜单项添加到系统菜单中。

  // IDM_ABOUTBOX 必须在系统命令范围内。
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
  {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
  // 执行此操作
  SetIcon(m_hIcon, TRUE);			// 设置大图标
  SetIcon(m_hIcon, FALSE);		// 设置小图标

                              // TODO: 在此添加额外的初始化代码
                              // TODO: Add extra initialization here
  pWnd = GetDlgItem(IDC_ShowImage);
  pDC = pWnd->GetDC();
  hDC = pDC->GetSafeHdc();
  pWnd->GetClientRect(&rect);

  pwnd1 = GetDlgItem(IDC_ShowImage1);
  pDC1 = pwnd1->GetDC();
  hDC1 = pDC1->GetSafeHdc();
  pwnd1->GetClientRect(&rect1);

  pwnd2 = GetDlgItem(IDC_ShowImage3);
  pDC2 = pwnd2->GetDC();
  hDC2 = pDC2->GetSafeHdc();
  pwnd2->GetClientRect(&rect2);

  pwnd3 = GetDlgItem(IDC_ShowImage5);
  pDC3 = pwnd3->GetDC();
  hDC3 = pDC3->GetSafeHdc();
  pwnd3->GetClientRect(&rect3);

  //启动串口
  OnBnClickedBtopen();
  //启动摄像头
  OnBnClickedBtopenvideo();
  //启动自动发车
  SetTimer(3, 2000, NULL);

  return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CautocarDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDITSEND, _textBox_serialSend);
  DDX_Text(pDX, IDC_EDITREV, _textBox_serialReceive);
}

BEGIN_MESSAGE_MAP(CautocarDlg, CDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_TIMER()
  ON_WM_CLOSE()

  ON_MESSAGE(WM_RECV_SERIAL_DATA, OnRecvSerialData)

  ON_BN_CLICKED(IDC_BTOPEN, &CautocarDlg::OnBnClickedBtopen)
  ON_BN_CLICKED(IDC_BTCLOSE, &CautocarDlg::OnBnClickedBtclose)
  ON_BN_CLICKED(IDC_BTSEND, &CautocarDlg::OnBnClickedBtsend)
  ON_BN_CLICKED(IDC_BTAUTODRIVE, &CautocarDlg::OnBnClickedBtautodrive)
  ON_BN_CLICKED(IDC_BTOPENVIDEO, &CautocarDlg::OnBnClickedBtopenvideo)
  ON_BN_CLICKED(IDC_BTCLOSEVIDEO, &CautocarDlg::OnBnClickedBtclosevideo)
  ON_BN_CLICKED(IDC_BTSCANNUMBER, &CautocarDlg::OnBnClickedBtscannumber)
END_MESSAGE_MAP()

// 串口接收数据处理函数
LONG CautocarDlg::OnRecvSerialData(WPARAM wParam, LPARAM lParam)
{
  UpdateData(true);
  CString strOldRecv = _T("");
  CString strRecv = _T("");
  //串口接收到的BUF
  CHAR *pBuf = (CHAR*)wParam;
  //串口接收到的BUF长度
  DWORD dwBufLen = lParam;

  //长度判断，太长丢弃
  if (dwBufLen == 30)
  {
    CString c;
    byte revbuf[30] = "\0";
    memcpy(revbuf, pBuf, dwBufLen);
    int mybuff;
    if (revbuf[0] == 0xff && revbuf[1] == 0xff && revbuf[2] == 0x10 && revbuf[3] == 0x01 && revbuf[27] == 0x0d && revbuf[29] == 0x0a)
    {
      mybuff = revbuf[4] * 256 + revbuf[5];
      mybuff = revbuf[6] * 256 + revbuf[7];
      mybuff = revbuf[8] * 256 + revbuf[9];

      //显示颜色255的值
      //m_cgrgb1.Format(_T("%d %d %d %d"),revbuf[10]*256+revbuf[11],revbuf[12]*256+revbuf[13],revbuf[14]*256+revbuf[15],revbuf[16]*256+revbuf[17]);
      //m_cgrgb2.Format(_T("%d %d %d %d"),revbuf[18]*256+revbuf[19],revbuf[20]*256+revbuf[21],revbuf[22]*256+revbuf[23],revbuf[24]*256+revbuf[25]);

      //显示颜色255的值
      m_cgrgb1.Format(_T("%d"), revbuf[10] * 256 + revbuf[11] + revbuf[12] * 256 + revbuf[13] + revbuf[14] * 256 + revbuf[15] + revbuf[16] * 256 + revbuf[17]);
      m_cgrgb2.Format(_T("%d"), revbuf[18] * 256 + revbuf[19] + revbuf[20] * 256 + revbuf[21] + revbuf[22] * 256 + revbuf[23] + revbuf[24] * 256 + revbuf[25]);

      _textBox_serialReceive.Format(_T("%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x"), revbuf[0], revbuf[1], revbuf[2], revbuf[3], revbuf[4], revbuf[5], revbuf[6], revbuf[7], revbuf[8], revbuf[9], revbuf[10], revbuf[11], revbuf[12], revbuf[13], revbuf[14], revbuf[15], revbuf[16], revbuf[17], revbuf[18], revbuf[19], revbuf[20], revbuf[21], revbuf[22], revbuf[23], revbuf[24], revbuf[25], revbuf[26], revbuf[27], revbuf[28], revbuf[29]);

      UpdateData(false);

      OnBnClickedBtsend();
    }
  }

  //释放内存
  delete[] pBuf;
  pBuf = NULL;
  return 0;
}

//打开串口
void CautocarDlg::OnBnClickedBtopen()
{
  //串口初始化
  DCB dcb;
  int nPort = 7;
  //如果打开过串口啥都不做
  if (!this->IsOpen())
  {
    if (!this->Open(nPort, DebugLabComm::CComPort::AutoReceiveBySignal))
    {
      CString sMsg;
      sMsg.Format("提示:不能打开串口%d!", nPort);
      AfxMessageBox(sMsg, MB_ICONINFORMATION | MB_OK);
    }
    this->GetSerialPort()->GetState(dcb);
    dcb.BaudRate = 115200;

    this->GetSerialPort()->SetState(dcb);
    this->SetReceiveFunc((FOnReceiveData)OnReceiveData, this);
    this->SetBreakHandleFunc(OnComBreak);
  }
}

void CautocarDlg::OnBnClickedBtclose()
{
  Close();
}

//发送数据
void CautocarDlg::OnBnClickedBtsend()
{
  // TODO: 在此添加控件通知处理程序代码
  UpdateData(true);
  // TODO: 在此添加控件通知处理程序代码

  if (this->IsOpen())
  {

  }

  UpdateData(false);
}

void CautocarDlg::OnBnClickedBtautodrive()
{
  UpdateData(true);
  m_modeall = 10;//大模式自动开机
  SetTimer(2, 50, NULL);
  UpdateData(false);
}

//打开摄像头
void CautocarDlg::OnBnClickedBtopenvideo()
{
  //摄像头1
  if (!capture)
  {
    capture = cvCaptureFromCAM(0);
  }
  if (!capture)
  {
    AfxMessageBox("无法打开摄像头");
    return;
  }
  m_Frame = cvQueryFrame(capture);
  m_CvvImage.CopyOf(m_Frame, 1);
  m_CvvImage.DrawToHDC(hDC, &rect);

  /*
  //摄像头2
  if(!capture1)
  {
  capture1 = cvCaptureFromCAM(1);
  }
  if (!capture1)
  {
  AfxMessageBox("无法打开摄像头");
  return;
  }
  m_Frame=cvQueryFrame(capture1);
  m_CvvImage.CopyOf(m_Frame,1);
  m_CvvImage.DrawToHDC(hDC3, &rect3);
  */
  // 设置计时器,每10ms触发一次事件
  SetTimer(1, 50, NULL);

}

//关闭摄像头
void CautocarDlg::OnBnClickedBtclosevideo()
{
  cvReleaseCapture(&capture);
  cvReleaseCapture(&capture1);
  CDC MemDC;
  CBitmap m_Bitmap1;
  m_Bitmap1.LoadBitmap(IDB_BITMAP1);
  MemDC.CreateCompatibleDC(NULL);
  MemDC.SelectObject(&m_Bitmap1);
  pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
  pDC1->StretchBlt(rect1.left, rect1.top, rect1.Width(), rect1.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
  pDC2->StretchBlt(rect2.left, rect2.top, rect2.Width(), rect2.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
  pDC3->StretchBlt(rect3.left, rect3.top, rect3.Width(), rect3.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
  KillTimer(1);
}

void CautocarDlg::OnBnClickedBtscannumber()
{
  if (!capture1)
  {
    capture1 = cvCaptureFromCAM(0);
  }
  if (!capture1)
  {
    AfxMessageBox("无法打开摄像头");
    return;
  }

  //显示到第2个图像
  ::Sleep(20);
  m_Frame = cvQueryFrame(capture1);
  ::Sleep(20);
  m_Frame = cvQueryFrame(capture1);
  ::Sleep(20);
  m_Frame = cvQueryFrame(capture1);
  ::Sleep(20);
  m_Frame = cvQueryFrame(capture1);
  ::Sleep(20);
  m_Frame = cvQueryFrame(capture1);
  ::Sleep(20);
  m_Frame = cvQueryFrame(capture1);
  ::Sleep(20);
  m_Frame = cvQueryFrame(capture1);
  ::Sleep(20);
  m_Frame = cvQueryFrame(capture1);
  ::Sleep(20);
  m_Frame = cvQueryFrame(capture1);

  m_CvvImage.CopyOf(m_Frame, 1);
  m_CvvImage.DrawToHDC(hDC3, &rect3);


  cv::Mat src = m_Frame;
  // resize(src,src,Size(800,600));//标准大小
  cv::Mat src_gray;
  cv::Mat src_all = src.clone();
  cv::Mat threshold_output;
  std::vector<std::vector<cv::Point> > contours, contours2, contours_out, contours_all;
  std::vector<cv::Vec4i> hierarchy, hierarchy_out, hierarchy_all;
  //预处理
  cvtColor(src, src_gray, CV_BGR2GRAY);
  blur(src_gray, src_gray, cv::Size(3, 3)); //模糊，去除毛刺
  cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
  //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
  //腐蚀操作
  dilate(src_gray, src, element);
  threshold(src, threshold_output, 100, 255, cv::THRESH_OTSU);

  m_Frame = &IplImage(threshold_output);
  m_CvvImage.CopyOf(m_Frame, 1);
  m_CvvImage.DrawToHDC(hDC2, &rect2);

  //寻找轮廓 
  //第一个参数是输入图像 2值化的
  //第二个参数是内存存储器，FindContours找到的轮廓放到内存里面。
  //第三个参数是层级，**[Next, Previous, First_Child, Parent]** 的vector
  //第四个参数是类型，采用树结构
  //第五个参数是节点拟合模式，这里是全部寻找 findContours(image_contour_outside, contours_out, hierarchy_out, RETR_EXTERNAL, CHAIN_APPROX_NONE);  


  findContours(threshold_output, contours_out, hierarchy_out, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

  findContours(threshold_output, contours_all, hierarchy_all, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
  cv::Mat Drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);

  if (contours_all.size() == contours_out.size()) return;//没有内轮廓，则提前返回  

  for (int i = 0; i < contours_out.size(); i++)
  {
    int conloursize = contours_out[i].size();
    for (int j = 0; j < contours_all.size(); j++)
    {
      int tem_size = contours_all[j].size();
      if (conloursize == tem_size)
      {
        swap(contours_all[j], contours_all[contours_all.size() - 1]);
        contours_all.pop_back();
        break;
      }
    }
  }

  //contours_all中只剩下内轮廓  
  //查找最大轮廓  
  double maxarea = 0;
  int maxAreaIdx = 0;
  for (int index = contours_all.size() - 1; index >= 0; index--)
  {
    double tmparea = fabs(contourArea(contours_all[index]));
    if (tmparea > maxarea)
    {
      maxarea = tmparea;
      maxAreaIdx = index;//记录最大轮廓的索引号  
    }
  }
  cv::Scalar color = cv::Scalar(0, 0, 255);
  //绘制最大内轮廓
  drawContours(Drawing, contours_all, maxAreaIdx, color, 2, 8, hierarchy_all, 0, cv::Point());
  //轮廓长度
  double g_dConLength = arcLength(contours_all[maxAreaIdx], true);
  //轮廓面积
  double g_dConArea = contourArea(contours_all[maxAreaIdx], true);

  m_Frame = &IplImage(Drawing);
  m_CvvImage.CopyOf(m_Frame, 1);
  m_CvvImage.DrawToHDC(hDC1, &rect1);

  //关闭摄像头
  cvReleaseCapture(&capture1);

  // OnBnClickedBtopenvideo();

  UpdateData(false);
}

void CautocarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else
  {
    CDialog::OnSysCommand(nID, lParam);
  }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CautocarDlg::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // 用于绘制的设备上下文

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    // 使图标在工作区矩形中居中
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // 绘制图标
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CautocarDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

//回调函数
void OnReceiveData(LPVOID pSender, void* pBuf, DWORD InBufferCount)
{
  BYTE *pRecvBuf = NULL; //接收缓冲区
                         //得到父对象指针
  CautocarDlg* pThis = (CautocarDlg*)pSender;
  //将接收的缓冲区拷贝到pRecvBuf种
  pRecvBuf = new BYTE[InBufferCount];
  CopyMemory(pRecvBuf, pBuf, InBufferCount);

  //发送异步消息，表示收到串口数据，消息处理完，应释放内存
  pThis->PostMessage(WM_RECV_SERIAL_DATA, WPARAM(pRecvBuf), InBufferCount);
}

// CheatingfactoryDlg 对话框
void OnComBreak(LPVOID pSender, DWORD dwMask, COMSTAT stat)
{
  //deal with the break of com here
  switch (dwMask)
  {
  case  EV_BREAK:
  {
    break;
  }
  case EV_CTS:
  {
    break;
  }
  case EV_DSR:
  {
    break;
  }
  case EV_ERR:
  {
    break;
  }
  case EV_RING:
  {
    break;
  }
  case EV_RLSD:
  {
    break;
  }
  case EV_RXCHAR:
  {
    break;
  }
  case EV_RXFLAG:
  {
    break;
  }
  case EV_TXEMPTY:
  {
    break;
  }
  default:
  {
  }
  }
}

//定时获取摄像头信息，同时对图像做处理
void CautocarDlg::OnTimer(UINT_PTR nIDEvent)
{
  if (nIDEvent == 1)
  {
    UpdateData(true);
    // TODO: Add your message handler code here and/or call default
    /************************************************************************/
    /* 显示摄像头                                                           */
    /************************************************************************/
    m_Frame = cvQueryFrame(capture);
    //显示到第一个图像
    m_CvvImage.CopyOf(m_Frame, 1);
    m_CvvImage.DrawToHDC(hDC, &rect);
    // 获取当前帧的灰度图

    IplImage* frame_1;
    IplImage* frame_2;
    IplImage* frame_3;
    IplImage* frame_4;
    IplImage* frame_color;

    frame_1 = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 1);
    frame_2 = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 1);
    frame_3 = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 1);
    frame_4 = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 1);
    frame_color = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, 3);
    //cvSplit(m_Frame, frame_1, frame_2, frame_4, 0);//分离RGB通道，分别得到BGR的单通道图像

    cvCvtColor(m_Frame, frame_color, CV_BGR2HSV);//得到HSV图
    cvSplit(frame_color, frame_3, frame_1, frame_2, 0);//分离hsv通道，分别得到hsv的单通道图像

    if (m_modeall == 10)
    { //判断开机
      CvScalar myaverage = cvAvg(m_Frame);

    }

    //灰度处理
    //		cvCvtColor(frame_1,frame_color,CV_GRAY2BGR);
    //平滑处理
    //		cvSmooth(frame_2, frame_3,CV_GAUSSIAN, 5, 0); 
    //二值化处理
    cvThreshold(frame_2, frame_1, 230, 255, CV_THRESH_BINARY);


    int i = 0, j = 0;
    int m_jucenter = 0;
    CvSize size = cvGetSize(frame_1);
    int m_crossy = 480;
    int m_linecenternumber = 0;
    for (i = size.height - 10; i >= 0; i--)
    {
      int m_writenumber = 0;

      int m_start = 640;
      int m_end = 0;
      for (j = 0; j<size.width; j++)
      {
        if (CV_IMAGE_ELEM(frame_1, uchar, i, j) == 255)
        {
          m_writenumber++;
          CV_IMAGE_ELEM(frame_1, uchar, i, j) = 0;
          if (m_start>j)
            m_start = j;
          if (m_end<j)
            m_end = j;
        }
      }
      if (m_crossy == 480 && m_writenumber>88)  //  原始值200  
      {
        m_crossy = 480 - i;
      }

      //间距很宽
      if (m_writenumber>150 && abs(m_end - m_start)>150 && i<300)
      {
        m_jucenter++;
      }
      else if (m_writenumber<100 && abs(m_end - m_start)<100)
      {
        //中点累加值
        CV_IMAGE_ELEM(frame_1, uchar, i, (m_start + m_end) / 2) = 255;
        if (m_linecenternumber == 20)
        {
          
        }
        m_linecenternumber++;
      }
     
    }

    //显示到第3个图像
    m_CvvImage.CopyOf(frame_1, 1);
    m_CvvImage.DrawToHDC(hDC2, &rect2);

    //显示到第2个图像
    //m_Frame=cvQueryFrame(capture1);

    //cvSmooth(frame_4, frame_3,CV_GAUSSIAN, 5, 0); 
    //二值化处理
    //cvThreshold(frame_3, frame_1,240, 255, CV_THRESH_BINARY);

    //m_CvvImage.CopyOf(m_Frame,1);	
    //m_CvvImage.DrawToHDC(hDC1, &rect1);

    cvReleaseImage(&frame_1);
    cvReleaseImage(&frame_2);
    cvReleaseImage(&frame_3);
    cvReleaseImage(&frame_4);
    cvReleaseImage(&frame_color);
    UpdateData(false);
  }
  if (nIDEvent == 2)
  {

  }

  if (nIDEvent == 3)
  {
    OnBnClickedBtautodrive();
    KillTimer(3);
  }

  CDialog::OnTimer(nIDEvent);
}

void CautocarDlg::OnClose()
{
  // TODO: Add your message handler code here and/or call default
  OnBnClickedBtclose();
  OnBnClickedBtclosevideo();
  CDialog::OnClose();
}

