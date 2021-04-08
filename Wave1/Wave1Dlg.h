
// Wave1Dlg.h: 头文件
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
	int   m_nzValues[POINT_COUNT];
	int   test = 1;
	void CWave1Dlg::DrawWave(CDC* pDC, CRect& rectPicture);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedOpen();
	
	int op = 1;
	int cl = 1;


	CEdit m_channelRead;	// 单位 通道读数
	CEdit m_inSig;	// 单位 输入信号
	CEdit m_diff;	// 单位 差值显示
	CEdit m_sinAmp;	// 正弦 幅值
	CEdit m_sinFre;	// 正弦 频率

	double m_valP;	// 比例控制	
	double m_valI;	//积分控制
	double m_valD;	//微分控制
	double Error1 = 0, Error2 = 0; 	//	计算差值和误差
	long ErrorValue = 0;

	CComboBox m_comChannel;	// 通道方式 combo
	CComboBox m_comChannelNum;	//通道数 combo
	CComboBox m_comRange;	// 量程 combo	
	CComboBox m_comGain;	// 增益 combo
	CComboBox m_comSig;	// 信号选择 阶跃/sin
	CComboBox m_comStep;	// 阶跃信号 combo

	double m_valStepAmp;	//阶跃 幅值 combo

	int m_cardN0 = 1;	//模块号
	int m_ChMode = 0;	//通道方式
	int m_ADrange = 1;	//选择对采集到的AD原码值做何种变换，应该与AD采集量程(一般需跳线)相匹配
	int m_ADAmp = 0;	//0 = 1倍增益(无增益)；1 = 10倍增益；2 = 100倍增益
	CTime t;

	afx_msg void OnCbnSelchangeChannel();
	afx_msg void OnCbnSelchangeChannelNum();
	afx_msg void OnCbnSelchangeRange();
	afx_msg void OnCbnSelchangeGain();
	afx_msg void OnCbnSelchangeSig();
	afx_msg void OnCbnSelchangeStep();
	afx_msg void OnCbnSelchangeStepAmp();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedEnd();

	afx_msg void OnEnChangeP();
	afx_msg void OnEnChangeI();
	afx_msg void OnEnChangeD();
	afx_msg void OnEnChangeEdit1();

	afx_msg void OnEnChangeStepAmp();
};
