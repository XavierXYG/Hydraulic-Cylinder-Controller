
// Wave1Dlg.h: 头文件


#include <windows.h>
//

#pragma once
#define POINT_COUNT 200 

// CWave1Dlg 对话框
class CWave1Dlg : public CDialogEx
{
// 构造
public:
	CWave1Dlg(CWnd* pParent = nullptr);	// 标准构造函数
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVE1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//	afx_msg void OnStnClickedWaveDraw();
	CStatic m_picDraw;
	CStatic m_ErrorDraw;
	double m_nzValues[POINT_COUNT];
	int test = 1;
	void CWave1Dlg::DrawWave(CDC* pDC, CRect& rectPicture, int x);
	void CWave1Dlg::DrawError(CDC* pDC, CRect& rectPicture);

	double m_nzValues2[POINT_COUNT];
	double m_nzValues3[POINT_COUNT];


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedOpen();

	int op = 1;
	int cl = 1;
	float tt = 0;   //主计时器
	float t_sin = 0;  //用于发正弦波
	int x;	//判断是画reference还是真实值，1为真实值；2为reference

	int input_denominator = 17.8;
	int output_factor = 40;

	CEdit m_channelRead;	// 单位 通道读数
	CEdit m_inSig;	// 单位 输入信号
	CEdit m_diff;	// 单位 差值显示
	CEdit m_sinAmp;	// 正弦 幅值
	CEdit m_sinFre;	// 正弦 频率
	CEdit m_DeadZone; // 死区
	CEdit m_ValStepSpeed;  // 阶跃目标速度
	CEdit m_ValOvershoot;  // 最大超调量(百分比)
	CEdit m_ValRiseTime;  //阶跃上升时间	  
	CEdit m_ValSettlingTime;  // 阶跃稳定时间
	// CEdit m_ValCommutationError;  // 正弦换向误差
	CEdit m_ValErrorCode;  // 板卡接口返回的错误代码
	CEdit m_valSatrtPos;  // 每次更新参数后的初始位置
	CEdit m_valDirection;  // 期望运动方向
	CEdit m_valPeak;  // 最大超调的峰值时间

	double m_valP;	// 比例控制
	double m_valI;	//积分控制
	double m_valD;	//微分控制

	int para_flag = 1;  //记录当前输入参数是正向的还是反向的
	double forward_P;
	double backward_P;
	double forward_I;
	double backward_I;
	double forward_D;
	double backward_D;

	double Error1 = 0, Error2 = 0; 	//	计算差值和误差, Error1 = 这次的；Error2 = 上次的
	double Actuating_signal = 0;
	double m_xAmp = 1;	//横轴放大倍数
	double start_pos = -1;
	double maximum_step_value = 0;

	//double starting_time; //恒等于0
	bool is_rised = 0;  //判断是否达到过100%的ref，达到过就不再更新tr
	int settle_count = 0;  //计数，上下不超过3%，否则清零，到100个（1s）计算ts
	bool is_settled = 0;  //判断是否稳定

	CComboBox m_comChannel;	// 通道方式 combo
	CComboBox m_comChannelNum;	//通道数 combo
	CComboBox m_comRange;	// 量程 combo	
	CComboBox m_comGain;	// 增益 combo
	CComboBox m_comSig;	// 信号选择 阶跃/sin
	CComboBox m_comDir;  // 调参时不同方向参数不同


	double m_valStepAmp;	//阶跃 幅值 combo
	double m_valStepmV;  // 测死区电压阶跃

	int m_cardN0 = 1;	//模块号
	int m_ChMode = 0;	//通道方式
	int m_ADrange = 1;	//选择对采集到的AD原码值做何种变换，应该与AD采集量程(一般需跳线)相匹配
	int m_ADAmp = 0;	//0 = 1倍增益(无增益)；1 = 10倍增益；2 = 100倍增益
	CTime t;
	SYSTEMTIME t1;  //Update参数按下时的系统时间
	SYSTEMTIME t2;  //不断更新的系统时间

	afx_msg void OnCbnSelchangeChannel();
	afx_msg void OnCbnSelchangeChannelNum();
	afx_msg void OnCbnSelchangeRange();
	afx_msg void OnCbnSelchangeGain();
	afx_msg void OnCbnSelchangeSig();
	afx_msg void OnCbnSelchangeStepAmp();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedEnd();

	afx_msg void OnEnChangeP();
	afx_msg void OnEnChangeI();
	afx_msg void OnEnChangeD();

	afx_msg void OnEnChangeStepAmp();


	

	

	afx_msg void OnBnClickedUpdate();
	

	
	
	afx_msg void OnCbnSelchangeSetDir();
	
	
};
