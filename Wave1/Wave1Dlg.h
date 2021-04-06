
// Wave1Dlg.h: 头文件
//

#pragma once
#define POINT_COUNT 100  

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
	
	// Flags
	int is_open_;  //initial = 1, success = 0; failed = -1;
	int is_closed_;  //initial = 1, success = 0; failed = -1;

	// User defined value
	int dt_; //default = 200ms
	double k_p_;
	double k_i_;
	double k_d_;

	// Signals
	double last_error_;
	double current_error_;
	double error_integral_;
	long control_quantity_;
	double reference_value_;


	CEdit m_p;	// PID 比例系数P
	CEdit m_I;	// PID 积分系数I
	CEdit m_D;	// PID 微分系数 D
	CEdit m_channelRead;	// 单位 通道读数
	CEdit m_inSig;	// 单位 输入信号
	CEdit m_diff;	// 单位 差值显示
	CEdit m_sinAmp;	// 正弦 幅值
	CEdit m_sinFre;	// 正弦 频率

	CComboBox m_comChannel;	// 通道方式 combo
	CComboBox m_comChannelNum;	//通道数 combo
	CComboBox m_comRange;	// 量程 combo
	CComboBox m_comGain;	// 增益 combo
	CComboBox m_comSig;	// 信号选择 阶跃/sin
	CComboBox m_comStep;	// 阶跃信号 combo
	CComboBox m_comStepAmp;	//阶跃 幅值 combo

	afx_msg void OnCbnSelchangeChannel();
	afx_msg void OnCbnSelchangeChannelNum();
	afx_msg void OnCbnSelchangeRange();
	afx_msg void OnCbnSelchangeGain();
	afx_msg void OnCbnSelchangeSig();
	afx_msg void OnCbnSelchangeStep();
	afx_msg void OnCbnSelchangeStepAmp();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedEnd();
	afx_msg void OnEnChangeChannelRead();
};
