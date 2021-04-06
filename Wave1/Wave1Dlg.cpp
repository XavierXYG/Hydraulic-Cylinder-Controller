
// Wave1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Wave1.h"
#include "Wave1Dlg.h"
#include "afxdialogex.h"
#include "usb7660.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWave1Dlg 对话框



CWave1Dlg::CWave1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WAVE1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 将数组m_nzValues的元素都初始化为0   

	memset(m_nzValues, 0, sizeof(int) * POINT_COUNT);

	// 自定义变量和标志位初始化
	is_open_ = 1;
	is_closed_ = 1;
	dt_ = 200; //200ms
	k_d_ = 0;
	k_i_ = 0;
	k_p_ = 0;

	last_error_ = 0;
	current_error_ = 0;
	error_integral_ = 0;
	control_quantity_ = 0;
	reference_value_ = 0;
}

void CWave1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WAVE_DRAW, m_picDraw);
	DDX_Control(pDX, IDC_P, m_p);
	DDX_Control(pDX, IDC_I, m_I);
	DDX_Control(pDX, IDC_D, m_D);
	DDX_Control(pDX, IDC_CHANNEL_READ, m_channelRead);
	DDX_Control(pDX, IDC_IN_SIG, m_inSig);
	DDX_Control(pDX, IDC_DIFF, m_diff);
	DDX_Control(pDX, IDC_SIN_AMP, m_sinAmp);
	DDX_Control(pDX, IDC_SIN_FRE, m_sinFre);
	DDX_Control(pDX, IDC_CHANNEL, m_comChannel);
	DDX_Control(pDX, IDC_CHANNEL_NUM, m_comChannelNum);
	DDX_Control(pDX, IDC_RANGE, m_comRange);
	DDX_Control(pDX, IDC_GAIN, m_comGain);
	DDX_Control(pDX, IDC_SIG, m_comSig);
	DDX_Control(pDX, IDC_STEP, m_comStep);
	DDX_Control(pDX, IDC_STEP_AMP, m_comStepAmp);
}

BEGIN_MESSAGE_MAP(CWave1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CWave1Dlg::OnBnClickedOk)

	//TODO


	ON_CBN_SELCHANGE(IDC_CHANNEL, &CWave1Dlg::OnCbnSelchangeChannel)
	ON_CBN_SELCHANGE(IDC_CHANNEL_NUM, &CWave1Dlg::OnCbnSelchangeChannelNum)
	ON_CBN_SELCHANGE(IDC_RANGE, &CWave1Dlg::OnCbnSelchangeRange)
	ON_CBN_SELCHANGE(IDC_GAIN, &CWave1Dlg::OnCbnSelchangeGain)
	ON_CBN_SELCHANGE(IDC_SIG, &CWave1Dlg::OnCbnSelchangeSig)
	ON_CBN_SELCHANGE(IDC_STEP, &CWave1Dlg::OnCbnSelchangeStep)
	ON_CBN_SELCHANGE(IDC_STEP_AMP, &CWave1Dlg::OnCbnSelchangeStepAmp)
	ON_BN_CLICKED(IDC_START, &CWave1Dlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_END, &CWave1Dlg::OnBnClickedEnd)
	ON_EN_CHANGE(IDC_CHANNEL_READ, &CWave1Dlg::OnEnChangeChannelRead)
END_MESSAGE_MAP()


// CWave1Dlg 消息处理程序

BOOL CWave1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 以时间为种子来构造随机数生成器   
    //srand((unsigned)time(NULL));   
 
    // 启动定时器，ID为1，定时时间为200ms   
    SetTimer(1, dt_, NULL);   

	// 为“通道方式”combo控件的列表框添加列表项
	m_comChannel.AddString(_T("单端"));
	m_comChannel.AddString(_T("双端"));  
	m_comChannel.SetCurSel(0);	// 默认选择第一项   
	SetDlgItemText(IDC_CHANNEL, _T("单端"));	// 编辑框中默认显示第一项的文字“单端”

	// 为“通道数”combo控件的列表框添加列表项
	m_comChannelNum.AddString(_T("48"));
	m_comChannelNum.AddString(_T("50"));
	m_comChannelNum.SetCurSel(0);	// 默认选择第一项   
	SetDlgItemText(IDC_CHANNEL_NUM, _T("48"));	// 编辑框中默认显示第一项的文字“48”

	// 为“量程”combo控件的列表框添加列表项
	m_comRange.AddString(_T("0-10000mm"));
	m_comRange.AddString(_T("0-5000mm"));
	m_comRange.SetCurSel(0);	// 默认选择第一项   
	SetDlgItemText(IDC_RANGE, _T("0-10000mm"));	// 编辑框中默认显示第一项的文字“0-10000mm”

	// 为“增益”combo控件的列表框添加列表项
	m_comGain.AddString(_T("1倍增益（无增益）"));
	m_comGain.AddString(_T("2倍增益"));
	m_comGain.SetCurSel(0);	// 默认选择第一项   
	SetDlgItemText(IDC_GAIN, _T("1倍增益"));	// 编辑框中默认显示第一项的文字“1倍增益（无增益）”

	// 为“信号选择”combo控件的列表框添加列表项
	m_comSig.AddString(_T("阶跃"));
	m_comSig.AddString(_T("正弦"));
	m_comSig.SetCurSel(0);	// 默认选择第一项   
	SetDlgItemText(IDC_SIG, _T("阶跃"));	// 编辑框中默认显示第一项的文字“阶跃”
  
	// 为“阶跃信号”combo控件的列表框添加列表项
	m_comStep.AddString(_T("开启"));
	m_comStep.AddString(_T("关闭"));
	m_comStep.SetCurSel(0);	// 默认选择第一项   
	SetDlgItemText(IDC_STEP, _T("开启"));	// 编辑框中默认显示第一项的文字“开启”

	// 为“阶跃 幅值”combo控件的列表框添加列表项
	m_comStepAmp.AddString(_T("10"));
	m_comStepAmp.AddString(_T("20"));
	m_comStepAmp.SetCurSel(0);	// 默认选择第一项   
	SetDlgItemText(IDC_STEP_AMP, _T("10"));	// 编辑框中默认显示第一项的文字“10”
    return TRUE;  // return TRUE  unless you set the focus to a control   

	// 文本框设置
	//TODO: 显示状态

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWave1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWave1Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWave1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWave1Dlg::DrawWave(CDC* pDC, CRect& rectPicture)
{
	float fDeltaX;     // x轴相邻两个绘图点的坐标距离   
	float fDeltaY;     // y轴每个逻辑单位对应的坐标值   
	int nX;      // 在连线时用于存储绘图点的横坐标   
	int nY;      // 在连线时用于存储绘图点的纵坐标   
	CPen newPen;       // 用于创建新画笔   
	CPen* pOldPen;     // 用于存放旧画笔   
	CBrush newBrush;   // 用于创建新画刷   
	CBrush* pOldBrush; // 用于存放旧画刷   

	// 计算fDeltaX和fDeltaY   
	fDeltaX = (float)rectPicture.Width() / (POINT_COUNT - 1);
	fDeltaY = (float)rectPicture.Height() / 80;

	// 创建黑色新画刷   
	newBrush.CreateSolidBrush(RGB(0, 0, 0));
	// 选择新画刷，并将旧画刷的指针保存到pOldBrush   
	pOldBrush = pDC->SelectObject(&newBrush);
	// 以黑色画刷为绘图控件填充黑色，形成黑色背景   
	pDC->Rectangle(rectPicture);
	// 恢复旧画刷   
	pDC->SelectObject(pOldBrush);
	// 删除新画刷   
	newBrush.DeleteObject();

	// 创建实心画笔，粗度为1，颜色为绿色   
	newPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	// 选择新画笔，并将旧画笔的指针保存到pOldPen   
	pOldPen = pDC->SelectObject(&newPen);

	// 将当前点移动到绘图控件窗口的左下角，以此为波形的起始点   
	pDC->MoveTo(rectPicture.left, rectPicture.bottom);
	// 计算m_nzValues数组中每个点对应的坐标位置，并依次连接，最终形成曲线   
	for (int i = 0; i < POINT_COUNT; i++)
	{
		nX = rectPicture.left + (int)(i * fDeltaX);
		nY = rectPicture.bottom - (int)(m_nzValues[i] * fDeltaY);
		pDC->LineTo(nX, nY);
	}

	// 恢复旧画笔   
	pDC->SelectObject(pOldPen);
	// 删除新画笔   
	newPen.DeleteObject();
}


void CWave1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rectPicture;

	if (is_open_ == 0)
	{
		int renVal = ZT7660_AIonce(1, 0, 21, 2, 0, 0, 0, 0, 0, 0, 0); //1,0,21,2,0,0,0,0,0,0,0
		
		CString ren_val = _T("");
		ren_val.Format(_T("%d"), renVal);
		AfxMessageBox(ren_val);

		last_error_ = current_error_;
		error_integral_ += last_error_ * dt_ / 1000;
		current_error_ = reference_value_ - double(renVal / 40);
		control_quantity_ = 40 * (k_p_ * current_error_ + k_i_ * error_integral_ + k_d_ * (current_error_ - last_error_) / dt_);
		
		ZT7660_AOonce(1, 1, 16, control_quantity_);

		// TODO: 使数值显示到对话框中
		// UpdateValue否？

	}
	

	// 将数组中的所有元素前移一个单位，第一个元素丢弃   
	for (int i = 0; i < POINT_COUNT - 1; i++)
	{
		m_nzValues[i] = m_nzValues[i + 1];
	}
	// 为最后一个元素赋一个80以内的随机数值（整型）   
	//m_nzValues[POINT_COUNT - 1] = rand() % 80;
	// 获取绘图控件的客户区坐标   
	// （客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标）   
	m_picDraw.GetClientRect(&rectPicture);

	// 绘制波形图   
	DrawWave(m_picDraw.GetDC(), rectPicture);

	CDialogEx::OnTimer(nIDEvent);
}


void CWave1Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	KillTimer(1);
}


void CWave1Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}



void CWave1Dlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");//文件过滤器

	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	if (IDOK == fileDlg.DoModal())//打开对话框关闭成功
	{
		CStdioFile MyFile;//创建文件实体
		MyFile.Open(fileDlg.GetPathName(), CFile::modeRead);//打开文件，只读

		CString srtbuf;//接收数据的缓存
		for (int i = 0; i < POINT_COUNT && MyFile.ReadString(srtbuf); i++)//存储数组用完或者读取到文件尾
		{
			m_nzValues[i] = _ttof(srtbuf)*10+10;
		}//字符串转为double
		//CRect rectPicture;
		//m_picDraw.GetClientRect(&rectPicture);//把picture的控件尺寸付给rectPicture对象，传递给以便DrawWave
		//DrawWave(m_picDraw.GetDC(), rectPicture);

	}
}



//“通道方式”获取combo控件内容
void CWave1Dlg::OnCbnSelchangeChannel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comChannel.GetCurSel();	//获得索引内容
	m_comChannel.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	UpdateData(false);	//更新显示及变量内容

}


void CWave1Dlg::OnCbnSelchangeChannelNum()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comChannelNum.GetCurSel();	//获得索引内容
	m_comChannelNum.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnCbnSelchangeRange()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comRange.GetCurSel();	//获得索引内容
	m_comRange.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnCbnSelchangeGain()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comGain.GetCurSel();	//获得索引内容
	m_comGain.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnCbnSelchangeSig()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comSig.GetCurSel();	//获得索引内容
	m_comSig.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnCbnSelchangeStep()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comStep.GetCurSel();	//获得索引内容
	m_comStep.GetLBText(nSel, strWeb);	//获得数据缓存内容

	//xianshi = strWeb;	//将索引内容传递给另一变量

	//按照加入的顺序改变不同选择的操作
	switch (nSel)
	{
	case 0:
		MessageBox(L"选择的数据为开启");
		break;
	case 1:
		MessageBox(L"选择的数据为关闭");
		break;
	default:
		break;
	}

	UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnCbnSelchangeStepAmp()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comStepAmp.GetCurSel();	//获得索引内容
	m_comStepAmp.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnBnClickedStart()
{
	// TODO: 在此读入用户输入数据
	is_open_ = ZT7660_OpenDevice(1);
	switch (is_open_)
	{
	case 0:
		MessageBox(L"success");
		break;
	case -1:
		MessageBox(L"failed");
		break;
	default:
		MessageBox(L"none");
		break;
	}

	//exit(0);
}


void CWave1Dlg::OnBnClickedEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	is_closed_ = ZT7660_CloseDevice(1);
	switch (is_closed_)
	{
	case 0:
		MessageBox(L"success");
		is_open_ = 1;
		break;
	case -1:
		MessageBox(L"failed");
		break;
	default:
		MessageBox(L"none");
		break;
	}
	//exit(0);
}


void CWave1Dlg::OnEnChangeChannelRead()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
