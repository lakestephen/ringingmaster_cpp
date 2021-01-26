#pragma once

class Tower;

// SimulatorImagesPreviewDlg dialog

class SimulatorImagesPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(SimulatorImagesPreviewDlg)

public:
	SimulatorImagesPreviewDlg(CWnd* pParent, const Tower& tower);   // standard constructor
	virtual ~SimulatorImagesPreviewDlg();

// Dialog Data
	enum { IDD = IDD_PREVIEW_SIMULATOR_IMAGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	const Tower& _internalTower;
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	void loadImage();

	CxImage _image;
	CString _path;
	CBCGPButton	_backLeft;
	CBCGPButton	_handLeft;
	CBCGPButton	_backRight;
	CBCGPButton	_handRight;
	afx_msg void OnBnClickedBackleft();
	afx_msg void OnBnClickedHandleft();
	afx_msg void OnBnClickedBackright();
	afx_msg void OnBnClickedHandright();
};
