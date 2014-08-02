
// BrazoDualDlg.h: archivo de encabezado
//

#pragma once


// Cuadro de di�logo de CBrazoDualDlg
class CBrazoDualDlg : public CDialogEx
{
// Construcci�n
public:
	CBrazoDualDlg(CWnd* pParent = NULL);	// Constructor est�ndar

// Datos del cuadro de di�logo
	enum { IDD = IDD_BRAZODUAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementaci�n
protected:
	HICON m_hIcon;

	// Funciones de asignaci�n de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void ButtonLoad();
	int m_robot;
	afx_msg void ButtonRun();
	void PickIzq(double *posIzq, double *posDer);
	void PickDer(double *posDer, double *posIzq);

	void PickIzq(int part);
	void PickDer(int part);

	void Pick(int robot, double *pos);
};
