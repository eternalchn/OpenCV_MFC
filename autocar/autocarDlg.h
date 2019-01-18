// autocarDlg.h : 头文件
//
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "zbar.h"            

#include "./serial/ComPort.h"
#include "resource.h"

using namespace zbar;  //添加zbar名称空间     
                       // CautocarDlg 对话框
class CautocarDlg : virtual DebugLabComm::CComPort, public CDialog
{
  // 构造
public:
  CautocarDlg(CWnd* pParent = NULL);	// 标准构造函数

  IplImage* image;
  IplImage* m_Frame;
  CvvImage m_CvvImage;

  int m_modeall;
  int m_modeevery;
  int m_modeeverytime;
  int m_modeeveryold;
  int m_modeeverynumber;

  CvCapture* capture;
  CvCapture* capture1;

  CRect rect;
  CDC *pDC;
  HDC hDC;
  CWnd *pWnd;

  CRect rect1;
  HDC hDC1;
  CDC *pDC1;
  CWnd *pwnd1;

  CRect rect2;
  CDC *pDC2;
  HDC hDC2;
  CWnd *pwnd2;

  CRect rect3;
  CDC *pDC3;
  HDC hDC3;
  CWnd *pwnd3;

  // 串口接收数据处理函数
  afx_msg LONG OnRecvSerialData(WPARAM wParam, LPARAM lParam);
  unsigned char _sendData[18];

  // 对话框数据
  enum { IDD = IDD_AUTOCAR_DIALOG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
                                                    // 实现
  HICON m_hIcon;

  // 生成的消息映射函数
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedBtopen();
  afx_msg void OnBnClickedBtclose();
  CString _textBox_serialSend;
  CString _textBox_serialReceive;
  afx_msg void OnBnClickedBtsend();

  CString m_cgrgb1;
  CString m_cgrgb2;

  int m_clockbuffleft;
  int m_clockbufffright;

  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnClose();
  afx_msg void OnBnClickedBtopenvideo();
  afx_msg void OnBnClickedBtclosevideo();
  afx_msg void OnBnClickedBtautodrive();
  afx_msg void OnBnClickedBtscannumber();

};

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

  // 对话框数据
  enum { IDD = IDD_ABOUTBOX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
                                                      // 实现
  DECLARE_MESSAGE_MAP()
};

/*Deal Receive Data Function / 接收数据处理函数
*/
void OnReceiveData(LPVOID pSender, void* pBuf, DWORD InBufferCount);
/*Deal with the break of com /处理串口中断
*/
void OnComBreak(LPVOID pSender, DWORD dwMask, COMSTAT stat);