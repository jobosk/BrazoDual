
// BrazoDualDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include "BrazoDual.h"
#include "BrazoDualDlg.h"
#include "afxdialogex.h"

#include "../../../Includes/VReal/VReal.h"
#include "../../../Includes/VRTransf/VRtransf.h"
#include "../../../Includes/VRStdIO/VRStdIO.h"
#include "Robot Definitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool part[3] = {true, true, true};	// True indica que estan en la izquierda

// Posiciones de origen

double origen_izq[6] = {723,0,712,0,90,0};
double origen_der[6] = {723,0,712,0,90,0};

// Posiciones de intercambio

double intercambio_izq[6] = {620,250,-250,0,180,0};
double intercambio_der[6] = {620,-250,-250,0,180,0};

// Posiciones de las piezas

double pieza_izq[3][6] = {{650,650,350,90,90,0},{500,650,350,90,90,0},{350,650,350,90,90,0}};
double pieza_der[3][6] = {{650,-650,350,-90,90,0},{500,-650,350,-90,90,0},{350,-650,350,-90,90,0}};

int robot_izq, robot_der;
double toolstatusI = 1, toolstatusD = 1;

// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Cuadro de diálogo de CBrazoDualDlg

CBrazoDualDlg::CBrazoDualDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBrazoDualDlg::IDD, pParent)
	, m_robot(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBrazoDualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBrazoDualDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CBrazoDualDlg::ButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON3, &CBrazoDualDlg::ButtonRun)
END_MESSAGE_MAP()


// Controladores de mensaje de CBrazoDualDlg

BOOL CBrazoDualDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Establecer el icono para este cuadro de diálogo. El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	// TODO: agregar aquí inicialización adicional

	if (alInitialize() != RET_OK){
		AfxMessageBox((CString)"Error Initializing VReal");
		exit(-1);
	}

	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
}

void CBrazoDualDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Si agrega un botón Minimizar al cuadro de diálogo, necesitará el siguiente código
//  para dibujar el icono. Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operación la realiza automáticamente el marco de trabajo.

void CBrazoDualDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CBrazoDualDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBrazoDualDlg::ButtonLoad()
{
	// TODO: Agregue aquí su código de controlador de notificación de control

	if(alCloseAll() != RET_OK)
		AfxMessageBox((CString)"Error Closing All");

	if(alLoadRobot("Models/Users/d-roi/ABB IRB 140 - Pinza cilindrica.rkf",&m_robot)!=RET_OK)
		AfxMessageBox((CString)"Error Loading Robot");

	if(alSetActiveToolFrame(m_robot,1)!=RET_OK)
		AfxMessageBox((CString)"Error Setting tool");

	robot_izq = m_robot;

	if(alPlaceRobot(m_robot,300,750,1600,0,90,-90)!=RET_OK)
		AfxMessageBox((CString)"Error Placing Left Robot");

	if(alLoadRobot("Models/Users/d-roi/ABB IRB 140 - Pinza cilindrica.rkf",&m_robot)!=RET_OK)
		AfxMessageBox((CString)"Error Loading Robot");

	if(alSetActiveToolFrame(m_robot,1)!=RET_OK)
		AfxMessageBox((CString)"Error Setting tool");

	robot_der = m_robot;

	if(alPlaceRobot(m_robot,-300,750,1600,0,-90,-90)!=RET_OK)
		AfxMessageBox((CString)"Error Placing Right Robot");

	if(alLoadEnvironment("Models/Users/d-roi/d-roi.enf")!=RET_OK)
		AfxMessageBox((CString)"Error Loading Robot");

	if(alRobotReset(m_robot,RESET) != RET_OK)
		AfxMessageBox((CString)"Error in reset");

}

void CBrazoDualDlg::PickIzq(int part){

	double pos[4][4], aprox[4][4];
	double aprox_intercambio_izq[4][4], aprox_intercambio_der[4][4];

	if(part < 1)
		return;

	alStartCollisionCheck(500);	// Empezamos la comprobacion de colisiones

	// El robot izquierdo agarra la pieza

	LocationEuler2ToTransformation(pieza_izq[part-1], pos);	// Convertimos la localizacion de la pieza de la izquierda en una transformacion hasta dicha posicion
	ApplyUVWTranslation(pos, 0, 0, -100, aprox);	// Aplicamos a dicha transformacion un desplazamiento en el eje Z para definir la transformacion hasta la posicion de aproximacion

	alMoveRobot(robot_izq, aprox, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN); // Realizamos un desplazamiento punto-a-punto con el brazo izquierdo hasta la posicion de aproximacion
	
	alSetRobotSpeed(robot_izq, 0.3);	// Reducimos la velocidad a 0.3
	
	alMoveRobot(robot_izq, pos, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y despues un desplazamiento linear hasta la posicion de la pieza

	alPickPart(robot_izq, toolON);	// Cerramos la herramienta del robot izquierdo

	Sleep(200);	// Hacemos una espera de 200 milisegundos
	
	alMoveRobot(robot_izq, aprox, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Volvemos a la posicion de aproximacion con un desplazamiento linear

	alSetRobotSpeed(robot_izq, 0.5);	// Aumentamos la velocidad a 0.5
	
	// El robot izquierdo se desplaza hasta la posicion de intercambio

	LocationEuler2ToTransformation(intercambio_izq, pos);	// Convertimos la localizacion del punto de intercambio en una transformacion hasta dicha posicion
	ApplyUVWTranslation(pos, 0, 0, -100, aprox_intercambio_izq);	// Aplicamos a dicha transformacion un desplazamiento en el eje Z para definir la transformacion hasta la posicion de aproximacion
	
	alMoveRobot(robot_izq, aprox_intercambio_izq, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Realizamos un desplazamiento punto-a-punto con el brazo izquierdo hasta la posicion de aproximacion
	
	alSetRobotSpeed(robot_izq, 0.3);	// Reducimos la velocidad a 0.3
	
	alMoveRobot(robot_izq, pos, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y despues un desplazamiento linear hasta la posicion del punto de intercambio

	// El robot derecho se desplaza hasta la posicion de intercambio

	LocationEuler2ToTransformation(intercambio_der, pos);	// Convertimos la localizacion del punto de intercambio en una transformacion hasta dicha posicion
	ApplyUVWTranslation(pos, 0, 0, -100, aprox_intercambio_der);	// Aplicamos a dicha transformacion un desplazamiento en el eje Z para definir la transformacion hasta la posicion de aproximacion
	
	alMoveRobot(robot_der, aprox_intercambio_der, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Realizamos un desplazamiento punto-a-punto con el brazo derecho hasta la posicion de aproximacion
	
	alSetRobotSpeed(robot_der, 0.3);	// Reducimos la velocidad a 0.3
	
	alMoveRobot(robot_der, pos, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y despues un desplazamiento linear hasta la posicion del punto de intercambio

	// Se produce el intercambio
	
	alPlacePart(robot_izq, toolOFF);	// Abrimos la herramienta del robot izquierdo
	alPickPart(robot_der, part, 2, toolON, NO_CHECK_COINCIDENCE);	// Cerramos la herramienta del robot derecho

	Sleep(200);	// Hacemos una espera de 200 milisegundos

	alMoveRobot(robot_der, aprox_intercambio_der, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Realizamos un desplazamiento linear con el brazo derecho hasta la posicion de aproximacion del punto de intercambio
	alMoveRobot(robot_izq, aprox_intercambio_izq, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y tambien un desplazamiento linear con el brazo izquierdo hasta la posicion de aproximacion del punto de intercambio

	alSetRobotSpeed(robot_der, 0.5);	// Aumentamos la velocidad a 0.5
	alSetRobotSpeed(robot_izq, 0.5);	// Aumentamos la velocidad a 0.5
	
	LocationEuler2ToTransformation(origen_izq, pos);	// Convertimos la posicion de origen en una transformacion a dicha posicion
	alMoveRobot(robot_izq, pos, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Finalmente regresamos al origen
	
	// El robot derecho suelta la pieza

	LocationEuler2ToTransformation(pieza_der[part-1], pos);	// Convertimos la localizacion de la pieza de la derecha en una transformacion hasta dicha posicion
	ApplyUVWTranslation(pos, 0, 0, -100, aprox);	// Aplicamos a dicha transformacion un desplazamiento en el eje Z para definir la transformacion hasta la posicion de aproximacion
	
	alMoveRobot(robot_der, aprox, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Realizamos un desplazamiento punto-a-punto con el brazo derecho hasta la posicion de aproximacion
	
	alSetRobotSpeed(robot_der, 0.3);	// Reducimos la velocidad a 0.3
	
	alMoveRobot(robot_der, pos, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y despues un desplazamiento linear hasta la posicion de la pieza

	alPlacePart(robot_der, toolOFF);	// Abrimos la herramienta del robot derecho

	Sleep(200);	// Hacemos una espera de 200 milisegundos

	alMoveRobot(robot_der, aprox, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Retrocedemos hasta la posicion de aproximacion con un desplazamiento linear
	
	alSetRobotSpeed(robot_der, 0.5);	// Aumentamos la velocidad a 0.5
	
	LocationEuler2ToTransformation(origen_der, pos);	// Convertimos la posicion de origen en una transformacion a dicha posicion
	alMoveRobot(robot_der, pos, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Finalmente regresamos al origen

	alEndCollisionCheck();	// Terminamos la comprobacion de colisiones

}

void CBrazoDualDlg::PickDer(int part){

	double pos[4][4], aprox[4][4];
	double aprox_intercambio_izq[4][4], aprox_intercambio_der[4][4];

	if(part < 1)
		return;

	alStartCollisionCheck(500);	// Empezamos la comprobacion de colisiones

	// El robot derecho agarra la pieza

	LocationEuler2ToTransformation(pieza_der[part-1], pos);	// Convertimos la localizacion de la pieza de la derecha en una transformacion hasta dicha posicion
	ApplyUVWTranslation(pos, 0, 0, -100, aprox);	// Aplicamos a dicha transformacion un desplazamiento en el eje Z para definir la transformacion hasta la posicion de aproximacion

	alMoveRobot(robot_der, aprox, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Realizamos un desplazamiento punto-a-punto con el brazo derecho hasta la posicion de aproximacion
	
	alSetRobotSpeed(robot_der, 0.3);	// Reducimos la velocidad a 0.3
	
	alMoveRobot(robot_der, pos, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y despues un desplazamiento linear hasta la posicion de la pieza

	alPickPart(robot_der, part, 2, toolON, NO_CHECK_COINCIDENCE);	// Cerramos la herramienta del robot derecho

	Sleep(200);	// Hacemos una espera de 200 milisegundos

	alMoveRobot(robot_der, aprox, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Volvemos a la posicion de aproximacion con un desplazamiento linear

	alSetRobotSpeed(robot_der, 0.5);	// Aumentamos la velocidad a 0.5
	
	// El robot derecho se desplaza hasta la posicion de intercambio

	LocationEuler2ToTransformation(intercambio_der, pos);	// Convertimos la localizacion del punto de intercambio en una transformacion hasta dicha posicion
	ApplyUVWTranslation(pos, 0, 0, -100, aprox_intercambio_der);	// Aplicamos a dicha transformacion un desplazamiento en el eje Z para definir la transformacion hasta la posicion de aproximacion
	
	alMoveRobot(robot_der, aprox_intercambio_der, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Realizamos un desplazamiento punto-a-punto con el brazo derecho hasta la posicion de aproximacion
	
	alSetRobotSpeed(robot_der, 0.3);	// Reducimos la velocidad a 0.3
	
	alMoveRobot(robot_der, pos, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y despues un desplazamiento linear hasta la posicion del punto de intercambio

	// El robot izquierdo se desplaza hasta la posicion de intercambio

	LocationEuler2ToTransformation(intercambio_izq, pos);	// Convertimos la localizacion del punto de intercambio en una transformacion hasta dicha posicion
	ApplyUVWTranslation(pos, 0, 0, -100, aprox_intercambio_izq);	// Aplicamos a dicha transformacion un desplazamiento en el eje Z para definir la transformacion hasta la posicion de aproximacion
	
	alMoveRobot(robot_izq, aprox_intercambio_izq, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Realizamos un desplazamiento punto-a-punto con el brazo izquierdo hasta la posicion de aproximacion
	
	alSetRobotSpeed(robot_izq, 0.3);	// Reducimos la velocidad a 0.3
	
	alMoveRobot(robot_izq, pos, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y despues un desplazamiento linear hasta la posicion del punto de intercambio
	
	// Se produce el intercambio
	
	alPlacePart(robot_der, toolOFF);	// Abrimos la herramienta del robot derecho
	alPickPart(robot_izq, toolON);	// Cerramos la herramienta del robot izquierdo

	Sleep(200);	// Hacemos una espera de 200 milisegundos
	
	alMoveRobot(robot_izq, aprox_intercambio_izq, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Realizamos un desplazamiento linear con el brazo izquierdo hasta la posicion de aproximacion del punto de intercambio
	alMoveRobot(robot_der, aprox_intercambio_der, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y tambien un desplazamiento linear con el brazo derecho hasta la posicion de aproximacion del punto de intercambio

	alSetRobotSpeed(robot_der, 0.5);	// Aumentamos la velocidad a 0.5
	alSetRobotSpeed(robot_izq, 0.5);	// Aumentamos la velocidad a 0.5

	LocationEuler2ToTransformation(origen_der, pos);	// Convertimos la posicion de origen en una transformacion a dicha posicion
	alMoveRobot(robot_der, pos, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Finalmente regresamos al origen
	
	// El robot izquierdo suelta la pieza

	LocationEuler2ToTransformation(pieza_izq[part-1], pos);	// Convertimos la localizacion de la pieza de la izquierda en una transformacion hasta dicha posicion
	ApplyUVWTranslation(pos, 0, 0, -100, aprox);	// Aplicamos a dicha transformacion un desplazamiento en el eje Z para definir la transformacion hasta la posicion de aproximacion
	
	alMoveRobot(robot_izq, aprox, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Realizamos un desplazamiento punto-a-punto con el brazo izquierdo hasta la posicion de aproximacion
	
	alSetRobotSpeed(robot_izq, 0.3);	// Reducimos la velocidad a 0.3
	
	alMoveRobot(robot_izq, pos, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Y despues un desplazamiento linear hasta la posicion de la pieza

	alPlacePart(robot_izq, toolOFF);	// Abrimos la herramienta del robot izquierdo

	Sleep(200);	// Hacemos una espera de 200 milisegundos
	
	alMoveRobot(robot_izq, aprox, LINEAR, ABSOLUTE_MOVEMENT, ORIGIN);	// Retrocedemos hasta la posicion de aproximacion con un desplazamiento linear
	
	alSetRobotSpeed(robot_izq, 0.5);	// Aumentamos la velocidad a 0.5
	
	LocationEuler2ToTransformation(origen_izq, pos);	// Convertimos la posicion de origen en una transformacion a dicha posicion
	alMoveRobot(robot_izq, pos, POINTTOPOINT, ABSOLUTE_MOVEMENT, ORIGIN);	// Finalmente regresamos al origen

	alEndCollisionCheck();	// Terminamos la comprobacion de colisiones

}

int p;

void CBrazoDualDlg::ButtonRun()
{
	p = -1;
	int status;
	for(int i=1;i<=3;i++){	// Para las entradas 1, 2 y 3
		alCheckDigitalOutput(robot_izq, i, &status);	// Comprobamos el estado
		if(status == 1){	// En caso de estar activa
			p = i;	// Actualizamos el identificador
		}
	}

	if(p >= 1 && p <= 3){	// Si el identificador es correcto
		if(part[p-1] == true){
			PickIzq(p);
			part[p-1] = false;
		}else{
			PickDer(p);
			part[p-1] = true;
		}
	}
}
