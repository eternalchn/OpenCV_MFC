#include <string>

#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "zbar.h"            

#include "./serial/ComPort.h"
#include "resource.h"

using namespace zbar;
using namespace std;

// 自定义MFC消息常量 串口数据接收消息 @PostMessageFunc OnReceiveData()
#define WM_RECV_SERIAL_DATA WM_USER + 101

class CautocarDlg : virtual DebugLabComm::CComPort, public CDialog
{
public:
  explicit CautocarDlg(CWnd* pParent = NULL);
  virtual BOOL OnInitDialog();

  /**
   * @func: DoDataExchange - DDX/DDV支持
   *      : OnPaint - 向对话框添加最小化按钮
   *      : OnQueryDragIcon - 当用户拖动最小化窗口时系统调用此函数取得光标显示
   * @Message function mapping
   *      DECLARE_MESSAGE_MAP() 
   *      @see autocarDlg.cpp BEGIN_MESSAGE_MAP
   */
  virtual void DoDataExchange(CDataExchange* pDX);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()

  /* 串口相关函数 ********************************************/
  afx_msg void OnBnClickedBt_OpenSerial();
  afx_msg void OnBnClickedBt_CloseSerial();
  afx_msg void OnBnClickedBt_SendToSerial();
  void PrintlnToSerial(const string& message);
  void PrintToSerial(const string& message);
  friend void OnCommReceive(LPVOID pSender, void* pBuf, DWORD InBufferCount);
  afx_msg LONG OnRecvSerialData(WPARAM wParam, LPARAM lParam);
  friend void OnCommBreak(LPVOID pSender, DWORD dwMask, COMSTAT stat);

  /* OpenCV相关函数 *****************************************/
  afx_msg void OnBnClickedBtopenvideo();
  afx_msg void OnBnClickedBtclosevideo();
  afx_msg void OnBnClickedBtautodrive();
  afx_msg void OnBnClickedBtscannumber();

  int m_clockbuffleft;
  int m_clockbufffright;

  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnClose();

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

protected:
  HICON appIcon_;

private:
  void _SerialOpen(int commNum = 2, int baudRate = 115200);

  CString _msgSerialSend;
  CString _msgSerialReceive;
  
  IplImage* _cameraFrame;
  CvvImage _cvvImage;

  //TAG: _mode的类型应该设置为一个 枚举类
  int _mode;
};
