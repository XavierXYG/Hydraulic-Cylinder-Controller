// Wave1Dlg.cpp: 实现文件

#include "pch.h"
#include "framework.h"
#include "Wave1.h"
#include "Wave1Dlg.h"
#include "afxdialogex.h"
#include "usb7660.H"

#include <math.h>

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
	, m_valP(5)
	, m_valI(0.3)
	, m_valD(0)
	, m_xAmp(1)
	, m_valStepmV(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 将数组m_nzValues的元素都初始化为0   

	memset(m_nzValues, 0, sizeof(int) * POINT_COUNT);
}

void CWave1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WAVE_DRAW, m_picDraw);
	DDX_Control(pDX, IDC_ERROR_PLOT, m_ErrorDraw);
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
	DDX_Text(pDX, IDC_STEP_AMP, m_valStepAmp);
	DDV_MinMaxDouble(pDX, m_valStepAmp, 0, 100000);
	DDX_Text(pDX, IDC_P, m_valP);
	DDV_MinMaxDouble(pDX, m_valP, 0, 100000);
	DDX_Text(pDX, IDC_I, m_valI);
	DDV_MinMaxDouble(pDX, m_valI, 0, 100000);
	DDX_Text(pDX, IDC_D, m_valD);
	DDV_MinMaxDouble(pDX, m_valD, 0, 100000);
	DDX_Control(pDX, IDC_DEADZONE, m_DeadZone);
	DDX_Text(pDX, IDC_EDIT_XAUG, m_xAmp);
	DDX_Control(pDX, IDC_STEP_SPEED, m_ValStepSpeed);
	DDX_Control(pDX, IDC_OVERSHOOT, m_ValOvershoot);
	DDX_Control(pDX, IDC_RISETIME, m_ValRiseTime);
	DDX_Control(pDX, IDC_SETTLING_TIME, m_ValSettlingTime);
	// DDX_Control(pDX, IDC_COMMUTATION, m_ValCommutationError);
	DDX_Control(pDX, IDC_ERROR_CODE, m_ValErrorCode);

	DDX_Text(pDX, IDC_STEP_AMP_MV, m_valStepmV);
	DDX_Control(pDX, IDC_START_VALUE, m_valSatrtPos);
	DDX_Control(pDX, IDC_DIRE, m_valDirection);
	DDX_Control(pDX, IDC_SET_DIR, m_comDir);
	DDX_Control(pDX, IDC_PEAK_TIME, m_valPeak);
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
	ON_CBN_SELCHANGE(IDC_STEP_AMP, &CWave1Dlg::OnCbnSelchangeStepAmp)
	ON_BN_CLICKED(IDC_START, &CWave1Dlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_END, &CWave1Dlg::OnBnClickedEnd)
	ON_EN_CHANGE(IDC_P, &CWave1Dlg::OnEnChangeP)
	ON_EN_CHANGE(IDC_I, &CWave1Dlg::OnEnChangeI)
	ON_EN_CHANGE(IDC_D, &CWave1Dlg::OnEnChangeD)

	ON_EN_CHANGE(IDC_STEP_AMP, &CWave1Dlg::OnEnChangeStepAmp)
	ON_BN_CLICKED(IDC_UPDATE, &CWave1Dlg::OnBnClickedUpdate)
	ON_CBN_SELCHANGE(IDC_SET_DIR, &CWave1Dlg::OnCbnSelchangeSetDir)
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
	m_comRange.AddString(_T("0 -- 5000mV"));	//1
	m_comRange.AddString(_T("0 -- 10000mV"));	//2
	m_comRange.AddString(_T("-5000 -- 5000mV"));	//5
	m_comRange.AddString(_T("原码值"));	//0
	m_comRange.SetCurSel(1);	// 默认选择第一项   
	SetDlgItemText(IDC_RANGE, _T("0 -- 10000mV"));	// 编辑框中默认显示第一项的文字“0-10000mm”

	// 为“增益”combo控件的列表框添加列表项
	m_comGain.AddString(_T("1倍增益（无增益）"));
	m_comGain.AddString(_T("10倍增益"));
	m_comGain.AddString(_T("100倍增益"));
	m_comGain.SetCurSel(0);	// 默认选择第一项   
	SetDlgItemText(IDC_GAIN, _T("1倍增益（无增益）"));	// 编辑框中默认显示第一项的文字“1倍增益（无增益）”

	// 为“信号选择”combo控件的列表框添加列表项
	m_comSig.AddString(_T("阶跃"));
	m_comSig.AddString(_T("分段阶跃"));
	m_comSig.AddString(_T("正弦"));
	m_comSig.AddString(_T("测死区"));
	m_comSig.SetCurSel(1);	// 默认选择第一项   
	SetDlgItemText(IDC_SIG, _T("请选择"));	// 编辑框中默认显示第一项的文字“阶跃”
	//m_xAmp = 1;

	m_comDir.AddString(_T("正向"));
	m_comDir.AddString(_T("反向"));
	m_comDir.SetCurSel(0);	// 默认选择第一项   
	SetDlgItemText(IDC_SET_DIR, _T("正向"));	// 编辑框中默认显示第一项的文字“正向”

	SetWindowText(_T("Tiny Cylinder Controller"));

	m_DeadZone.SetWindowTextW(_T("2150"));

	//PID参数初值
	forward_P = 5;
	forward_I = 0.3;
	forward_D = 0;
	backward_P = 20;
	backward_I = 0.3;
	backward_D = 0;

	// 波形图框地址指针初始化
	//获取Picture Control控件的大小，ID为IDC_ShowPic  
	GetDlgItem(IDC_WAVE_DRAW)->GetWindowRect(&rectWave);
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rectWave);
	//窗口移动到控件表示的区域
	GetDlgItem(IDC_WAVE_DRAW)->MoveWindow(rectWave.left, rectWave.top, rectWave.Width(), rectWave.Height(), TRUE);
	pDCWave = GetDlgItem(IDC_WAVE_DRAW);
	pDCWave->GetClientRect(&rectWave);      //获取句柄指向控件区域的大小 

	//误差图框地址指针初始化
	//获取Picture Control控件的大小，ID为IDC  
	GetDlgItem(IDC_ERROR_PLOT)->GetWindowRect(&rectError);
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rectError);
	//窗口移动到控件表示的区域
	GetDlgItem(IDC_ERROR_PLOT)->MoveWindow(rectError.left, rectError.top, rectError.Width(), rectError.Height(), TRUE);
	pDCError = GetDlgItem(IDC_ERROR_PLOT);
	pDCError->GetClientRect(&rectError);      //获取句柄指向控件区域的大小 

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

void CWave1Dlg::DrawWave(CWnd* pWnd, CRect& rectPicture)
{
	float fDeltaX;     // x轴相邻两个绘图点的坐标距离   
	float fDeltaY;     // y轴每个逻辑单位对应的坐标值   
	int nX;      // 在连线时用于存储绘图点的横坐标   
	int nY;      // 在连线时用于存储绘图点的纵坐标   
	CPen newPen1;       // 用于创建新画笔-real   
	CPen* pOldPen1;     // 用于存放旧画笔-ref   
	CPen newPen2;       // 用于创建新画笔-real   
	CPen* pOldPen2;     // 用于存放旧画笔-ref  
	CBrush newBrush;   // 用于创建新画刷   
	CBrush* pOldBrush; // 用于存放旧画刷   

	//GetClientRect(rect);

	CClientDC DC(pWnd);
	// 计算fDeltaX和fDeltaY  
	//rect.MoveToX(rectPicture.right);
	//rect.MoveToY(-(float)rect.Height()+rect.bottom);
	//rect.MoveToY(rect.bottom);
	fDeltaX = (float)rectPicture.Width() / (POINT_COUNT - 1);
	fDeltaY = (float)rectPicture.Height() / 275;
	// 创建黑色新画刷   
	newBrush.CreateSolidBrush(RGB(0, 0, 0));
	// 选择新画刷，并将旧画刷的指针保存到pOldBrush   
	pOldBrush = DC.SelectObject(&newBrush);
	// 以黑色画刷为绘图控件填充黑色，形成黑色背景   
	DC.Rectangle(rectWave);
	// 恢复旧画刷   
	DC.SelectObject(pOldBrush);
	// 删除新画刷   
	newBrush.DeleteObject();

	// 创建实心画笔，粗度为2，颜色为*色   
	newPen1.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	// 选择新画笔，并将旧画笔的指针保存到pOldPen   
	pOldPen1 = DC.SelectObject(&newPen1);
	// 将当前点移动到绘图控件窗口的左下角，以此为波形的起始点   
	DC.MoveTo(rectPicture.left, rectPicture.bottom);
	// 计算m_nzValues数组中每个点对应的坐标位置，并依次连接，最终形成曲线 

	float upperLimit = rectPicture.bottom - (int)(275 * fDeltaY);
	float lowerLimit = rectPicture.bottom;

	for (int i = 0; i < POINT_COUNT; i++)
	{
		//计算坐标
		nX = rectPicture.left + (float)rectPicture.Width() - m_xAmp * ((float)rectPicture.Width() - (int)(i * fDeltaX));
		//nX = rectPicture.left + (int)(i * fDeltaX) * m_xAmp;
		nY = rectPicture.bottom - (int)(m_nzValues[i] * fDeltaY);
		//判断会不会画出画布
		if (m_nzValues[i] > 275)//上界
		{
			//左界
			if (nX < rectPicture.left + fDeltaX / 20) DC.MoveTo(rectPicture.left + fDeltaX / 20, upperLimit);
			else DC.MoveTo(nX, upperLimit);
		}
		else if (m_nzValues[i] < 0)//下界
		{
			//左界
			if (nX < rectPicture.left + fDeltaX / 20) DC.MoveTo(rectPicture.left + fDeltaX / 20, lowerLimit);
			else DC.MoveTo(nX, lowerLimit);
		}
		else if (nX < rectPicture.left + fDeltaX / 20) DC.MoveTo(rectPicture.left + fDeltaX / 20, nY);//
		else if (i == 0) DC.MoveTo(nX, nY);//第一个点先不画
		else DC.LineTo(nX, nY);//连线
	}

	// 恢复旧画笔   
	DC.SelectObject(pOldPen1);
	// 删除新画笔   
	newPen1.DeleteObject();

	newPen2.CreatePen(PS_SOLID, 2, RGB(255, 225, 0));
	pOldPen2 = DC.SelectObject(&newPen2);
	for (int i = 0; i < POINT_COUNT; i++)
	{
		nX = rectPicture.left + (float)rectPicture.Width() - m_xAmp * ((float)rectPicture.Width() - (int)(i * fDeltaX));
		//nX = rectPicture.left + (int)(i * fDeltaX) * m_xAmp ;
		nY = rectPicture.bottom - (int)(m_nzValues2[i] * fDeltaY);

		if (m_nzValues2[i] > 275)
		{
			if (nX < rectPicture.left + fDeltaX / 20) DC.MoveTo(rectPicture.left + fDeltaX / 20, upperLimit);
			else DC.MoveTo(nX, upperLimit);
		}
		else if (m_nzValues2[i] < 0)
		{
			if (nX < rectPicture.left + fDeltaX / 20) DC.MoveTo(rectPicture.left + fDeltaX / 20, lowerLimit);
			else DC.MoveTo(nX, lowerLimit);
		}
		else if (nX < rectPicture.left + fDeltaX / 20) DC.MoveTo(rectPicture.left + fDeltaX / 20, nY);
		else if (i == 0) DC.MoveTo(nX, nY);
		else DC.LineTo(nX, nY);

	}

	DC.SelectObject(pOldPen2);
	// 删除新画笔   
	newPen2.DeleteObject();
	//ReleaseDC(pDC);
}

void CWave1Dlg::DrawError(CWnd* pWnd, CRect& rectPicture)
{
	float fDeltaX;     // x轴相邻两个绘图点的坐标距离   
	float fDeltaY;     // y轴每个逻辑单位对应的坐标值   
	int nX;      // 在连线时用于存储绘图点的横坐标   
	int nY;      // 在连线时用于存储绘图点的纵坐标   
	CPen newPen;       // 用于创建新画笔   
	CPen* pOldPen;     // 用于存放旧画笔   
	CBrush newBrush;   // 用于创建新画刷   
	CBrush* pOldBrush; // 用于存放旧画刷   

	//GetClientRect(rect);

	CClientDC DC(pWnd);

	// 计算fDeltaX和fDeltaY   
	fDeltaX = (float)rectPicture.Width() / (POINT_COUNT - 1);
	fDeltaY = (float)rectPicture.Height() / 10;

	// 创建黑色新画刷   
	newBrush.CreateSolidBrush(RGB(0, 0, 0));
	// 选择新画刷，并将旧画刷的指针保存到pOldBrush   
	pOldBrush = DC.SelectObject(&newBrush);
	// 以黑色画刷为绘图控件填充黑色，形成黑色背景   
	DC.Rectangle(rectPicture);
	// 恢复旧画刷   
	DC.SelectObject(pOldBrush);
	// 删除新画刷   
	newBrush.DeleteObject();

	// 创建实心画笔，粗度为2，颜色为red guys   
	newPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	// 选择新画笔，并将旧画笔的指针保存到pOldPen   
	pOldPen = DC.SelectObject(&newPen);

	// 将当前点移动到绘图控件窗口的左下角，以此为波形的起始点   
	DC.MoveTo(rectPicture.left, rectPicture.bottom);
	// 计算m_nzValues数组中每个点对应的坐标位置，并依次连接，最终形成曲线 

	for (int i = 0; i < POINT_COUNT; i++)
	{
		//nX = rectPicture.left + (int)(i * fDeltaX) * m_xAmp;
		nX = rectPicture.left + (float)rectPicture.Width() - m_xAmp * ((float)rectPicture.Width() - (int)(i * fDeltaX));
		nY = rectPicture.bottom - (int)(m_nzValues3[i] * fDeltaY);
		float upperLimit = rectPicture.bottom - (int)(10 * fDeltaY);
		float lowerLimit = rectPicture.bottom;
		if (m_nzValues3[i] > 10)
		{
			if (nX < rectPicture.left + fDeltaX / 20) DC.MoveTo(rectPicture.left + fDeltaX / 20, upperLimit);
			else DC.MoveTo(nX, upperLimit);
		}
		else if (m_nzValues3[i] < 0)
		{
			if (nX < rectPicture.left + fDeltaX / 20) DC.MoveTo(rectPicture.left + fDeltaX / 20, lowerLimit);
			else DC.MoveTo(nX, lowerLimit);
		}
		else if (nX < rectPicture.left + fDeltaX / 20) DC.MoveTo(rectPicture.left + fDeltaX / 20, nY);
		else if (i == 0) DC.MoveTo(nX, nY);
		else DC.LineTo(nX, nY);

	}

	// 恢复旧画笔   
	DC.SelectObject(pOldPen);
	// 删除新画笔   
	newPen.DeleteObject();
}


void CWave1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rectPicture_wave;
	CRect rectPicture_error;
	double ref = 0;
	double renVal = 0;	//单次数据采集
	double renVolt = 0;  //单词电压返回值，用于测试
	double dead_zone = 0;
	int direction = 0;
	double overshoot = 0;
	int count_period = 2;

	if (op == 0)	//保证是“开始”后再计算; if debug 改成-1
	{

		tt = tt + 0.01;
		t_sin = t_sin + 0.01;
		GetLocalTime(&t2);//获取当前系统时间

		//阶跃
		if (m_comSig.GetCurSel() == 0 && nIDEvent == 1)
		{
			//定义变量
			ref = m_valStepAmp;
			//double integral = 0;	//算真正的积分项
			CString tempStr, strd;	//用于输出一些值

			//检测面板输入
			//tempStr.Format(_T("%d,%d,%d,%d"), m_cardN0, m_ChMode, m_ADrange, m_ADAmp);
			//MessageBox(tempStr);	//检测是否进入这个函数
			m_DeadZone.GetWindowText(strd);
			dead_zone = _tstof(strd);

			//指定通道单次采集
			renVolt = ZT7660_AIonce(m_cardN0, m_ChMode, 21, m_ADrange, m_ADAmp, 0, 0, 0, 0, 0, 0);
			renVal = double(renVolt / input_denominator);

			//判断方向
			if (ref >= start_pos) direction = 1;
			else if (ref < start_pos) direction = -1;

			//更新面板输出
			tempStr.Format(_T("%.5f"), renVolt);
			SetDlgItemText(IDC_CHANNEL_READ, tempStr);	//输出通道读数
			tempStr.Format(_T("%.5f"), ref);
			SetDlgItemText(IDC_IN_SIG, tempStr); //参考值

			if (direction == 1 && ref != start_pos)  //判断最大超调
			{
				if (renVal > maximum_step_value)
				{
					maximum_step_value = renVal;
					tempStr.Format(_T("%.5f"), tt);
					SetDlgItemText(IDC_PEAK_TIME, tempStr);
				}
				//overshoot = (maximum_step_value - ref) / (ref - start_pos);
				overshoot = fabs(maximum_step_value - ref) / (ref);
				tempStr.Format(_T("%.5f"), 100.0 * overshoot);
				SetDlgItemText(IDC_OVERSHOOT, tempStr); 
			}
			else if (direction == -1 && ref != start_pos)
			{
				if (renVal < maximum_step_value)
				{
					maximum_step_value = renVal;
					tempStr.Format(_T("%.5f"), tt);
					SetDlgItemText(IDC_PEAK_TIME, tempStr);
				}
				//overshoot = (maximum_step_value - ref) / (ref - start_pos);
				overshoot = fabs(maximum_step_value - ref) / (ref);
				tempStr.Format(_T("%.5f"), 100.0 * overshoot);
				SetDlgItemText(IDC_OVERSHOOT, tempStr); 
			}

			if (renVal >= ref && !is_rised)  //计算上升时间，注意>=
			{
				is_rised = !is_rised;
				tempStr.Format(_T("%.5f"), tt);
				SetDlgItemText(IDC_RISETIME, tempStr);
			}

			//if (fabs(renVal - ref) <= 0.03 * fabs(ref - start_pos) && is_rised)  settle_count++;  //计算稳定时间
			//else settle_count = 0;
			//if (settle_count == 100 * count_period)  //1s = 100 * 10ms
			//{
			//	tempStr.Format(_T("%.5f"), tt - count_period);
			//	SetDlgItemText(IDC_SETTLING_TIME, tempStr);
			//}

			if (!is_settled && fabs(renVal - ref) <= 0.03 * fabs(ref - start_pos))  //计算稳定时间
			{
				tempStr.Format(_T("%.5f"), tt);
				SetDlgItemText(IDC_SETTLING_TIME, tempStr);
				is_settled = 1;
			}
			else if (is_settled && fabs(renVal - ref) > 0.03 * fabs(ref - start_pos)) is_settled = 0;


			//更新控制参数并输出误差
			integral = Error1 + integral;
			Error2 = Error1;
			Error1 = double(ref - renVal);
			tempStr.Format(_T("%.5f"), Error1);
			SetDlgItemText(IDC_DIFF, tempStr);	//把误差输出
			//UpdateData(FALSE);

			//根据方向调整PID, 控制量计算
			if(direction==1)
			{
				Actuating_signal = output_factor * (forward_P * Error1 + forward_I * integral + forward_D * (Error1 - Error2));
			}
			else if (direction == -1)
			{
				Actuating_signal = output_factor * (backward_P * Error1 + backward_I * integral + backward_D * (Error1 - Error2));
			}
			else  //不会进入
			{
				exit(0);
			}

			//控制量计算
			//TODO: 优化算法，分段控制
			//Actuating_signal = output_factor * (m_valP * Error1 + m_valI * integral + m_valD * (Error1 - Error2));

			//控制量输出
			if(Actuating_signal > 0)  ZT7660_AOonce(1, 1, 6, Actuating_signal + dead_zone);
			else if(Actuating_signal < 0)  ZT7660_AOonce(1, 1, 6, Actuating_signal - dead_zone);

			//板卡状态更新
			tempStr.Format(_T("%d"), ZT7660_GetLastErr());
			m_ValErrorCode.SetWindowText(tempStr);

			//UpdateData(TRUE);

		}
		//分段阶跃
		else if (m_comSig.GetCurSel() == 1 && nIDEvent == 1)
		{
			//定义变量
			ref = m_valStepAmp;
			//double integral = 0;	//算真正的积分项
			CString tempStr, strd;	//用于输出一些值

			//检测面板输入
			//tempStr.Format(_T("%d,%d,%d,%d"), m_cardN0, m_ChMode, m_ADrange, m_ADAmp);
			//MessageBox(tempStr);	//检测是否进入这个函数
			m_DeadZone.GetWindowText(strd);
			dead_zone = _tstof(strd);

			//指定通道单次采集
			renVolt = ZT7660_AIonce(m_cardN0, m_ChMode, 21, m_ADrange, m_ADAmp, 0, 0, 0, 0, 0, 0);
			renVal = double(renVolt / input_denominator);

			//判断方向
			if (ref >= start_pos) direction = 1;
			else if (ref < start_pos) direction = -1;

			//更新面板输出
			tempStr.Format(_T("%.5f"), renVolt);
			SetDlgItemText(IDC_CHANNEL_READ, tempStr);	//输出通道读数
			tempStr.Format(_T("%.5f"), ref);
			SetDlgItemText(IDC_IN_SIG, tempStr); //参考值

			if (tt < 0.5 && direction == 1 && ref != start_pos)  //判断最大超调
			{
				if (renVal > maximum_step_value)
				{
					maximum_step_value = renVal;
					tempStr.Format(_T("%.5f"), tt);
					SetDlgItemText(IDC_PEAK_TIME, tempStr);
				}

				//overshoot = (maximum_step_value - ref) / (ref - start_pos);
				overshoot = fabs(maximum_step_value - ref) / (ref);
				tempStr.Format(_T("%.5f"), 100.0 * overshoot);
				SetDlgItemText(IDC_OVERSHOOT, tempStr);


			}
			else if (tt < 0.5 && direction == -1 && ref != start_pos)
			{
				if (renVal < maximum_step_value)
				{
					maximum_step_value = renVal;
					tempStr.Format(_T("%.5f"), tt);
					SetDlgItemText(IDC_PEAK_TIME, tempStr);
				}
				//overshoot = (maximum_step_value - ref) / (ref - start_pos);
				overshoot = fabs(maximum_step_value - ref) / (ref);
				tempStr.Format(_T("%.5f"), 100.0 * overshoot);
				SetDlgItemText(IDC_OVERSHOOT, tempStr);



			}

			if (direction == 1 && renVal >= 0.95 * ref && !is_rised)  //计算上升时间正向
			{
				is_rised = !is_rised;
				tempStr.Format(_T("%.5f"), tt);
				SetDlgItemText(IDC_RISETIME, tempStr);
			}
			else if (direction == -1 && renVal <= 1.05 * ref && !is_rised)  //计算上升时间反向
			{
				is_rised = !is_rised;
				tempStr.Format(_T("%.5f"), tt);
				SetDlgItemText(IDC_RISETIME, tempStr);
			}

			//if (fabs(renVal - ref) <= 0.03 * fabs(ref - start_pos) && is_rised)  settle_count++;  //计算稳定时间
			//else settle_count = 0;
			//if (settle_count == 100 * count_period)  //1s = 100 * 10ms
			//{
			//	tempStr.Format(_T("%.5f"), tt - count_period);
			//	SetDlgItemText(IDC_SETTLING_TIME, tempStr);
			//}

			if (!is_settled && fabs(renVal - ref) <= 0.03 * fabs(ref - start_pos))  //计算稳定时间
			{
				tempStr.Format(_T("%.5f"), tt);
				SetDlgItemText(IDC_SETTLING_TIME, tempStr);
				is_settled = 1;
			}
			else if (is_settled && fabs(renVal - ref) > 0.03 * fabs(ref - start_pos)) is_settled = 0;


			//更新控制参数并输出误差
			integral = Error1 + integral;
			Error2 = Error1;
			Error1 = double(ref - renVal);
			tempStr.Format(_T("%.5f"), Error1);
			SetDlgItemText(IDC_DIFF, tempStr);	//把误差输出
			//UpdateData(FALSE);

			//根据实时位置调整PID, 控制量计算
			if (direction == 1)
			{
				if (fabs(renVal - start_pos) / fabs(ref - start_pos) <= 0.75 && direction == 1)  //正向前半部分
				{
					integral = 0; //前面75%不积分
					Actuating_signal = output_factor * (front_P_forward * Error1 + 0 * integral + 0 * (Error1 - Error2));
				}
				else if (fabs(renVal - start_pos) / fabs(ref - start_pos) > 0.75 && direction == 1)  //正向后半部分
				{
					Actuating_signal = output_factor * (rear_P_forward * Error1 + rear_I_forward * integral + rear_D_forward * (Error1 - Error2));
				}
			}
			else if (direction == -1)
			{
				if (fabs(renVal - start_pos) / fabs(ref - start_pos) <= 0.75 && direction == -1)  //反向前半部分
				{
					integral = 0; //前面75%不积分
					Actuating_signal = output_factor * (front_P_backward * Error1 + 0 * integral + 0 * (Error1 - Error2));
				}
				else if (fabs(renVal - start_pos) / fabs(ref - start_pos) > 0.75 && direction == -1)  //反向后半部分
				{
					Actuating_signal = output_factor * (rear_P_backward * Error1 + rear_I_backward * integral + rear_D_backward * (Error1 - Error2));
				}
			}	
			else  //不会进入
			{
				exit(0);
			}

			//控制量计算
			//TODO: 优化算法，分段控制
			//Actuating_signal = output_factor * (m_valP * Error1 + m_valI * integral + m_valD * (Error1 - Error2));

			//控制量输出
			if (Actuating_signal > 0)  ZT7660_AOonce(1, 1, 6, Actuating_signal + dead_zone);
			else if (Actuating_signal < 0)  ZT7660_AOonce(1, 1, 6, Actuating_signal - dead_zone);

			//板卡状态更新
			tempStr.Format(_T("%d"), ZT7660_GetLastErr());
			m_ValErrorCode.SetWindowText(tempStr);

			//UpdateData(TRUE);

		}

		//正弦
		else if (m_comSig.GetCurSel() == 2 && nIDEvent == 1)
		{
			//定义变量
			//double integral = 0;
			double fre, peak = 0;	//计算积分项、频率、幅值和返回函数
			CString tempStr, strf, strd;	//用于输出一些值

			//检测面板输入, 生成参考位置
			m_DeadZone.GetWindowText(strd);
			dead_zone = _tstof(strd);

			m_sinFre.GetWindowText(strf);
			fre = _tstof(strf);
			peak = GetDlgItemInt(IDC_SIN_AMP);

			if (t_sin < 2.5) ref = 50 * t_sin;
			else  ref = 125 + peak * sin((t_sin-2.5) * fre * 2 * acos(-1)); //不考虑死区
			
			//指定通道单次采集
			renVal = ZT7660_AIonce(m_cardN0, m_ChMode, 21, m_ADrange, m_ADAmp, 0, 0, 0, 0, 0, 0) / input_denominator;

			//判断方向
			if (ref >= renVal) direction = 1;
			else if (ref < renVal) direction = -1;

			//更新面板输出
			tempStr.Format(_T("%.5f"), renVal);
			SetDlgItemText(IDC_CHANNEL_READ, tempStr);

			tempStr.Format(_T("%.5f"), ref);
			SetDlgItemText(IDC_IN_SIG, tempStr); //参考值

			//更新控制参数并输出误差

			integral = Error1 + integral;
			double tempp = Error1 - Error2; //为什么要上上次？
			Error2 = Error1;
			Error1 = ref - renVal;
			tempStr.Format(_T("%.5f"), Error1);
			SetDlgItemText(IDC_DIFF, tempStr);

			//控制量计算
			//TODO: 优化算法，分段控制
			//Actuating_signal = output_factor * (m_valP * Error1 + m_valI * integral + m_valD * tempp);
			//根据方向调整PID, 控制量计算
			if (direction == 1)
			{
				Actuating_signal = output_factor * (forward_P * Error1 + forward_I * integral + forward_D * (Error1 - Error2));
			}
			else if (direction == -1)
			{
				Actuating_signal = output_factor * (backward_P * Error1 + backward_I * integral + backward_D * (Error1 - Error2));
			}
			else  //不会进入
			{
				exit(0);
			}

			//控制量输出
			if (Actuating_signal > 0)  ZT7660_AOonce(1, 1, 6, Actuating_signal + dead_zone);
			else if (Actuating_signal < 0)  ZT7660_AOonce(1, 1, 6, Actuating_signal - dead_zone);

			//板卡状态更新
			tempStr.Format(_T("%d"), ZT7660_GetLastErr());
			m_ValErrorCode.SetWindowText(tempStr);

			//UpdateData(TRUE);
		}
		else if (m_comSig.GetCurSel() == 3 && nIDEvent == 1) //测死区
		{
			//定义变量
			ref = m_valStepmV;
			//double integral = 0;	//算真正的积分项
			CString tempStr, strd;	//用于输出一些值

			//检测面板输入
			//tempStr.Format(_T("%d,%d,%d,%d"), m_cardN0, m_ChMode, m_ADrange, m_ADAmp);
			//MessageBox(tempStr);	//检测是否进入这个函数
			m_DeadZone.GetWindowText(strd);
			dead_zone = _tstof(strd);

			//指定通道单次采集
			renVolt = ZT7660_AIonce(m_cardN0, m_ChMode, 21, m_ADrange, m_ADAmp, 0, 0, 0, 0, 0, 0);
			renVal = renVolt / input_denominator;

			//更新面板输出
			tempStr.Format(_T("%.5f"), renVolt);
			SetDlgItemText(IDC_CHANNEL_READ, tempStr);	//输出通道读数
			tempStr.Format(_T("%.5f"), ref);
			SetDlgItemText(IDC_IN_SIG, tempStr); //电压参考值

			//更新控制参数并输出误差
			integral = Error1 + integral;
			Error2 = Error1;
			Error1 = ref - renVolt;
			tempStr.Format(_T("%.5f"), Error1);
			SetDlgItemText(IDC_DIFF, tempStr);	//把误差输出
			//UpdateData(FALSE);

			//控制量计算
			//TODO: 优化算法，分段控制
			//Actuating_signal = (m_valP * Error1 + m_valI * integral + m_valD * (Error1 - Error2));
			
			//控制量输出
			if (ref > 0)  ZT7660_AOonce(1, 1, 6, ref + dead_zone);
			else if (ref < 0)  ZT7660_AOonce(1, 1, 6, ref - dead_zone);

			//板卡状态更新
			tempStr.Format(_T("%d"), ZT7660_GetLastErr());
			m_ValErrorCode.SetWindowText(tempStr);

			//UpdateData(TRUE);

		}
	}

	// 将数组中的所有元素前移一个单位，第一个元素丢弃   
	for (int i = 0; i < POINT_COUNT - 1; i++)
	{
		m_nzValues[i] = m_nzValues[i + 1];
		m_nzValues2[i] = m_nzValues2[i + 1];
		m_nzValues3[i] = m_nzValues3[i + 1];
	}

	//为画图函数赋值
	m_nzValues[POINT_COUNT - 1] = renVal;
	m_nzValues2[POINT_COUNT - 1] = ref;
	m_nzValues3[POINT_COUNT - 1] = Error1 + 5;

	// 获取绘图控件的客户区坐标   
	// （客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标）   
	m_picDraw.GetClientRect(&rectPicture_wave);
	m_ErrorDraw.GetClientRect(&rectPicture_error);

	// 绘制波形图   
	DrawWave(pDCWave, rectPicture_wave);
	DrawError(pDCError, rectPicture_error);
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
	CString FileName = _T("E:\\自控\\data.txt");
	CStdioFile FileWrite;
	//char string[10];
	CString strt;
	if (!FileWrite.Open(FileName, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
	{
		AfxMessageBox(_T("打开文件资源失败"));
		return;
	}

	for (int i = 0; i < POINT_COUNT - 1; i++)
	{
		strt.Format(_T("%d\n"),m_nzValues[i]);
		FileWrite.WriteString(strt);
	}
	

	//CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	//if (IDOK == fileDlg.DoModal())//打开对话框关闭成功
	//{
	//	CStdioFile MyFile;//创建文件实体
	//	MyFile.Open(fileDlg.GetPathName(), CFile::modeRead);//打开文件，只读

	//	CString srtbuf;//接收数据的缓存
	//	for (int i = 0; i < POINT_COUNT && MyFile.ReadString(srtbuf); i++)//存储数组用完或者读取到文件尾
	//	{
	//		m_nzValues[i] = _ttof(srtbuf)*10+10;
	//	}//字符串转为double
	//	//CRect rectPicture;
	//	//m_picDraw.GetClientRect(&rectPicture);//把picture的控件尺寸付给rectPicture对象，传递给以便DrawWave
	//	//DrawWave(m_picDraw.GetDC(), rectPicture);

	//}
}



//“通道方式”获取combo控件内容
void CWave1Dlg::OnCbnSelchangeChannel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comChannel.GetCurSel();	//获得索引内容
	if (nSel == 0) {
		m_ChMode = 0;//单端
	}
	else if (nSel == 1)
	{
		m_ChMode = 2;//双端
	}
	m_comChannel.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	//UpdateData(TRUE);
	//UpdateData(false);	//更新显示及变量内容

}


void CWave1Dlg::OnCbnSelchangeChannelNum()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comChannelNum.GetCurSel();	//获得索引内容
	
	m_comChannelNum.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	//UpdateData(TRUE);
	//UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnCbnSelchangeRange()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comRange.GetCurSel();	//获得索引内容
	switch (nSel) {
	case(0):
		m_ADrange = 1;	//0 -- 5000mV
		break;
	case(1):
		m_ADrange = 2;	// 0 -- 10000mV 
		break;
	case(2):
		m_ADrange = 5;	//-5000 -- 5000mV
		break;
	case(3):
		m_ADrange = 0;	//原码值
		break;
	default:
		break;
	}
	//m_comRange.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	//UpdateData(true);	//更新显示及变量内容
	//UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnCbnSelchangeGain()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	m_ADAmp = m_comGain.GetCurSel();	//获得索引内容
	//m_comSig.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	//UpdateData(TRUE);	//更新显示及变量内容
	//UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnCbnSelchangeSig()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comSig.GetCurSel();	//获得索引内容
	m_comSig.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	//UpdateData(false);	//更新显示及变量内容

	if(m_comSig.GetCurSel() == 0)  //阶跃
	{
		//forward_P = 5;
		//forward_I = 0.3;
		//forward_D = 0;
		//backward_P = 20;
		//backward_I = 0.3;
		//backward_D = 0;
		forward_P = 4.5;
		forward_I = 0.003;
		forward_D = 0.001;
		backward_P = 20;
		backward_I = 0.3;
		backward_D = 0;
		if (para_flag == 1)
		{
			
			m_valP = forward_P;
			m_valI = forward_I;
			m_valD = forward_D;
		}
		else if (para_flag == -1)
		{
			
			m_valP = backward_P;
			m_valI = backward_I;
			m_valD = backward_D;
		}
		
		m_DeadZone.SetWindowTextW(_T("2150"));
		UpdateData(false);
	}
	else if (m_comSig.GetCurSel() == 1)  //分段阶跃
	{
		/*空载*/
		front_P_forward = 100;
		rear_P_forward = 3.3;
		rear_I_forward = 0.003;
		rear_D_forward = 0.003;
		front_P_backward = 100;
		rear_P_backward = 3.5;
		rear_I_backward = 0.018;
		rear_D_backward = 0.00;


		///*3负载*/
		/*front_P_forward = 50;
		rear_P_forward = 3.5;
		rear_I_forward = 0.015;
		rear_D_forward = 0.01;
		front_P_backward = 50;
		rear_P_backward = 3.5;
		rear_I_backward = 0.015;
		rear_D_backward = 0.01;*/
		if (para_flag == 1)
		{
			m_valP = rear_P_forward;
			m_valI = rear_I_forward;
			m_valD = rear_D_forward;
		}
		else if (para_flag)
		{
			m_valP = rear_P_backward;
			m_valI = rear_I_backward;
			m_valD = rear_D_backward;
		}

		m_DeadZone.SetWindowTextW(_T("2150"));
		UpdateData(false);
	}
	else if (m_comSig.GetCurSel() == 2)  //正弦
	{
		//m_valP = 8;
		//m_valI = 0.4;
		//m_valD = 0.10;

		/*无负载*/
		//t_sin = 0;
		forward_P = 10;
		forward_I = 0.4;
		forward_D = 0.15;
		backward_P = 12;
		backward_I = 0.46;
		backward_D = 0.13;

		/**3负载*/ 

		/*forward_P = 4.6;
		forward_I = 0.4;
		forward_D = 0.15;
		backward_P = 15.8;
		backward_I = 0.36;
		backward_D = 0.11;*/
		if (para_flag == 1)
		{

			m_valP = forward_P;
			m_valI = forward_I;
			m_valD = forward_D;
		}
		else if (para_flag == -1)
		{

			m_valP = backward_P;
			m_valI = backward_I;
			m_valD = backward_D;
		}
		m_DeadZone.SetWindowTextW(_T("2150"));
		m_sinAmp.SetWindowTextW(_T("60"));
		m_sinFre.SetWindowTextW(_T("0.25"));
		UpdateData(false);
	}
	else if (m_comSig.GetCurSel() == 3)  //死区
	{
		m_valP = 0;
		m_valI = 0;
		m_valD = 0;
		m_DeadZone.SetWindowTextW(_T("0"));
		UpdateData(false);
	}
}

void CWave1Dlg::OnCbnSelchangeSetDir()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;	//保存数据缓存
	int nSel;	//获得索引变量
	nSel = m_comDir.GetCurSel();	//获得索引内容
	m_comDir.GetLBText(nSel, strWeb);	//获得数据缓存内容
	//xianshi = strWeb;	//将索引内容传递给另一变量
	//UpdateData(false);	//更新显示及变量内容
	if (m_comSig.GetCurSel() != 1)
	{
		if (m_comDir.GetCurSel() == 0)  //正向
		{
			para_flag = 1;
			m_valP = forward_P;
			m_valI = forward_I;
			m_valD = forward_D;
			UpdateData(false);
		}
		else if (m_comDir.GetCurSel() == 1)  //反向
		{
			para_flag = -1;
			m_valP = backward_P;
			m_valI = backward_I;
			m_valD = backward_D;
			UpdateData(false);
		}
	}
	else
	{
		if (m_comDir.GetCurSel() == 0)  //正向
		{
			para_flag = 1;
			m_valP = rear_P_forward;
			m_valI = rear_I_forward;
			m_valD = rear_D_forward;
			UpdateData(false);
		}
		else if (m_comDir.GetCurSel() == 1)  //反向
		{
			para_flag = -1;
			m_valP = rear_P_backward;
			m_valI = rear_I_backward;
			m_valD = rear_D_backward;
			UpdateData(false);
		}
	}
}

void CWave1Dlg::OnCbnSelchangeStepAmp()
{
	// TODO: 在此添加控件通知处理程序代码

	//UpdateData(false);	//更新显示及变量内容
}


void CWave1Dlg::OnBnClickedStart()
{
	tt = 0;
	t_sin = 0;
	is_rised = 0;
	is_settled = 0;
	//记录初始位置
	CString tempStr;
	double renVal = ZT7660_AIonce(m_cardN0, m_ChMode, 21, m_ADrange, m_ADAmp, 0, 0, 0, 0, 0, 0) / input_denominator;
	tempStr.Format(_T("%.5f"), renVal);
	SetDlgItemText(IDC_START_VALUE, tempStr);	//输出通道读数
	start_pos = renVal;
	maximum_step_value = start_pos;
	////记录初始位置
	//CString tempStr;
	//double renVal = ZT7660_AIonce(m_cardN0, m_ChMode, 21, m_ADrange, m_ADAmp, 0, 0, 0, 0, 0, 0) / input_denominator;
	//tempStr.Format(_T("%.5f"), renVal);
	//SetDlgItemText(IDC_START_VALUE, tempStr);	//输出通道读数
	//start_pos = renVal;

	// TODO: 在此添加控件通知处理程序代码
	int err;
	CString string;
	op = ZT7660_OpenDevice(1);
	cl = 1;
	//tt = 0;
	switch (op)
	{
	case 0:
		MessageBox(L"Open Device success");
		break;
	case -1:
		err = ZT7660_GetLastErr();
		string.Format(_T("Open Device failed\nError code: %d"), err); //将变量组装到字符串中
		MessageBox(string);
		break;
	default:
		MessageBox(L"Open Device none operation");
		break;
	}
	// 启动定时器，ID为1，定时时间为200ms   
	SetTimer(1, 10, NULL);

	//exit(0);
}


void CWave1Dlg::OnBnClickedEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	int err;
	CString string;
	ZT7660_AOonce(1, 1, 6, 0);  //避免直接关闭板卡造成刹不住车
	cl = ZT7660_CloseDevice(1);
	op = 1;
	tt = 0;
	t_sin = 0;
	KillTimer(1);
	switch (cl)
	{
	case 0:
		MessageBox(L"Close Device success");
		break;
	case -1:
		err = ZT7660_GetLastErr();
		string.Format(_T("Close Device failed\nError code: %d"), err); //将变量组装到字符串中
		MessageBox(string);
		break;
	default:
		MessageBox(L"Close Device none operation");
		break;
	}

	//exit(0);
}


void CWave1Dlg::OnEnChangeP()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	//UpdateData(FALSE);//本来就没有
}


void CWave1Dlg::OnEnChangeI()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	//UpdateData(FALSE);
}


void CWave1Dlg::OnEnChangeD()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//CString str;
	//GetDlgItemText(IDC_D, str);

	//m_valD = _ttof(str);
	//m_valD= _ttof(GetDlgItemText(IDC_D, str));
	//UpdateData(TRUE);
	//UpdateData(FALSE);//本来就没有
}





void CWave1Dlg::OnEnChangeStepAmp()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	//UpdateData(TRUE);
}





void CWave1Dlg::OnBnClickedUpdate()
{
	GetLocalTime(&t1);//获取当前系统时间
	tt = 0;
	is_rised = 0;
	integral = 0;
	//记录初始位置
	CString tempStr;
	double renVal = ZT7660_AIonce(m_cardN0, m_ChMode, 21, m_ADrange, m_ADAmp, 0, 0, 0, 0, 0, 0) / input_denominator;
	tempStr.Format(_T("%.5f"), renVal);
	SetDlgItemText(IDC_START_VALUE, tempStr);	//输出通道读数
	start_pos = renVal;
	maximum_step_value = start_pos;

	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	//根据选择的方向判断更新PID参数
	if (m_comSig.GetCurSel() == 1)  //分段阶跃
	{
		if (para_flag == 1)
		{
			rear_P_forward = m_valP;
			rear_I_forward = m_valI;
			rear_D_forward = m_valD;
		}
		else if (para_flag == -1)
		{
			rear_P_backward = m_valP;
			rear_I_backward = m_valI;
			rear_D_backward = m_valD;
		}

	}
	else if (para_flag == 1)
	{
		forward_P = m_valP;
		forward_I = m_valI;
		forward_D = m_valD;
	}
	else if (para_flag == -1)
	{
		backward_P = m_valP;
		backward_I = m_valI;
		backward_D = m_valD;
	}

	//test
	//CString str(_T("12"));
	//m_DeadZone.SetWindowText(str);
	UpdateData(false);
}




