// SensorSimulDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aligner.h"
#include "SensorSimulDlg.h"

#include "SerialPort.h"
#include ".\SensorSimulDlg.h"

#include <math.h>


// SensorSimulDlg dialog

IMPLEMENT_DYNAMIC(SensorSimulDlg, CDialog)
SensorSimulDlg::SensorSimulDlg(CWnd* pParent /*=NULL*/)
: CDialog(SensorSimulDlg::IDD, pParent)    
,m_rollAng(0.0f)
,m_pitchAng(0.0f)
, m_protRoll(0)
, m_protPitch(0)
, m_autoRoll(FALSE)
, m_autoPitch(FALSE)
, m_timer(0)    
, m_maxRollAngStr(_T(""))
, m_rollAngStepStr(_T(""))
, m_maxRollAng(4.1f)
, m_maxPitchAngStr(_T(""))
, m_pitchAngStepStr(_T(""))
, m_maxPitchAng(4.1f)
, m_rollCenterStr(_T(""))
, m_pitchCenterStr(_T(""))
, m_animate(TRUE)
, m_rollNoiceStr(_T(""))
, m_pitchNoiceStr(_T(""))
, m_pitchNoice(0.0f)    
, m_rollNoice(0.0f)    

{
//     m_rollCenter = (double)(rand()%60)/10.0f-3.0;
//     m_pitchCenter = (double)(rand()%60)/10.0f-3.0;
//     m_rollAngStep = rand()%10+1;
//     m_pitchAngStep = rand()%10+1;

    m_rollCenter = 0.0;
    m_pitchCenter = 0.0;
    m_rollAngStep = 4;
    m_pitchAngStep = 4;
}

SensorSimulDlg::~SensorSimulDlg()
{
}

void SensorSimulDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);    
    DDX_Text(pDX, IDC_PROT_ROLL, m_protRoll);
    DDX_Text(pDX, IDC_PROT_PITCH, m_protPitch);
    DDX_Check(pDX, IDC_ROLL_AUTO, m_autoRoll);
    DDX_Check(pDX, IDC_PITCH_AUTO, m_autoPitch);
    DDX_Text(pDX, IDC_MAX_ROLL_ANG, m_maxRollAngStr);
    DDX_Text(pDX, IDC_ROLL_STEP, m_rollAngStepStr);
    DDX_Text(pDX, IDC_MAX_PITCH_ANG, m_maxPitchAngStr);
    DDX_Text(pDX, IDC_PITCH_STEP, m_pitchAngStepStr);
    DDX_Text(pDX, IDC_ROLL_CENTER, m_rollCenterStr);
    DDX_Text(pDX, IDC_PITCH_CENTER, m_pitchCenterStr);
    DDX_Check(pDX, IDC_ANIM, m_animate);
    DDX_Text(pDX, IDC_NOICE_ROLL, m_rollNoiceStr);
    DDX_Text(pDX, IDC_NOICE_PITCH, m_pitchNoiceStr);
    DDX_Control(pDX, IDC_TYPE_COMBO, m_typeCombo);
}


BEGIN_MESSAGE_MAP(SensorSimulDlg, CDialog)
    ON_BN_CLICKED(IDC_SEND_BUTTON, OnBnClickedSendButton)
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_PITCH_AUTO, OnBnClickedPitchAuto)
    ON_BN_CLICKED(IDC_ROLL_AUTO, OnBnClickedRollAuto)
    ON_WM_TIMER()
    ON_EN_KILLFOCUS(IDC_MAX_ROLL_ANG, OnEnKillfocusMaxRollAng)
    ON_EN_KILLFOCUS(IDC_ROLL_STEP, OnEnKillfocusRollStep)
    ON_EN_KILLFOCUS(IDC_MAX_PITCH_ANG, OnEnKillfocusMaxPitchAng)
    ON_EN_KILLFOCUS(IDC_PITCH_STEP, OnEnKillfocusPitchStep)
    ON_EN_CHANGE(IDC_MAX_ROLL_ANG, OnEnChangeMaxRollAng)
    ON_EN_CHANGE(IDC_ROLL_STEP, OnEnChangeRollStep)
    ON_EN_CHANGE(IDC_MAX_PITCH_ANG, OnEnChangeMaxPitchAng)
    ON_EN_CHANGE(IDC_PITCH_STEP, OnEnChangePitchStep)    
    ON_EN_KILLFOCUS(IDC_ROLL_CENTER, OnEnKillfocusRollCenter)
    ON_EN_CHANGE(IDC_ROLL_CENTER, OnEnChangeRollCenter)
    ON_EN_KILLFOCUS(IDC_PITCH_CENTER, OnEnKillfocusPitchCenter)
    ON_EN_CHANGE(IDC_PITCH_CENTER, OnEnChangePitchCenter)
    ON_EN_CHANGE(IDC_NOICE_PITCH, OnEnChangeNoicePitch)
    ON_EN_KILLFOCUS(IDC_NOICE_PITCH, OnEnKillfocusNoicePitch)
    ON_EN_CHANGE(IDC_NOICE_ROLL, OnEnChangeNoiceRoll)
    ON_EN_KILLFOCUS(IDC_NOICE_ROLL, OnEnKillfocusNoiceRoll)
    ON_BN_CLICKED(IDC_ANIM, OnBnClickedAnim)
    ON_CBN_SELCHANGE(IDC_TYPE_COMBO, OnCbnSelchangeTypeCombo)
END_MESSAGE_MAP()



// SensorSimulDlg message handlers

BOOL SensorSimulDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    int startY=10,ctrlHeight=120, margin=10;    
    CRect rect;
    GetClientRect(&rect);
    //rect.bottom = rect.bottom - 40;

    m_type = TYPE_SENSORX;
    m_typeCombo.AddString("SensorX");
    m_typeCombo.SetItemData(0, TYPE_SENSORX);
    m_typeCombo.AddString("Lucas");
    m_typeCombo.SetItemData(1, TYPE_LUCAS);
    m_typeCombo.SetCurSel(0);

    m_rollRect = CRect(margin, startY, rect.right-margin, startY+ctrlHeight);
    m_pitchRect = m_rollRect;    
    m_pitchRect.OffsetRect(0, ctrlHeight+1*margin);    

    m_sigma40id01Data.header = 0xa55a;
    m_sigma40id01Data.numData = 0x1a;
    m_sigma40id01Data.Id = 01;
    m_sigma40id01Data.status1 = 0;
    m_sigma40id01Data.status2 = 0;
    m_sigma40id01Data.heading = 0;
    m_sigma40id01Data.roll = 1;
    m_sigma40id01Data.pitch = 2;

    m_maxRollAngStr.Format("%.2f",m_maxRollAng);
    m_rollAngStepStr.Format("%.2f",m_rollAngStep);
    m_maxPitchAngStr.Format("%.2f",m_maxPitchAng);
    m_pitchAngStepStr.Format("%.2f",m_pitchAngStep);

    m_rollCenterStr.Format("%.2f",m_rollCenter);
    m_pitchCenterStr.Format("%.2f",m_pitchCenter);

    m_rollNoiceStr.Format("%.2f",m_rollNoice);
    m_pitchNoiceStr.Format("%.2f",m_pitchNoice);

    SetWindowText(m_id);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void SensorSimulDlg::SendDauData()
{

//     unsigned short roll = m_protRoll;
//     Swap(roll);
//     unsigned short pitch = m_protPitch;
//     Swap(pitch);
// 
//     TRACE("send(01):%d, %x\n",m_protRoll, roll);
//     m_sigma40id01Data.roll = roll;
//     m_sigma40id01Data.pitch = pitch;
//     int length = sizeof(Sigma40Id01Data);
    //m_pSerialPort->Write( reinterpret_cast<const char *>(&m_sigma40id01Data), length ) ;
   
}


void SensorSimulDlg::OnBnClickedSendButton()
{
    SendDauData();
}

unsigned short SensorSimulDlg::ToProtocolFormat(double deg)
{
    if(m_type == TYPE_SENSORX || m_type == TYPE_LUCAS)
    {
        return ToSensorXFormat(deg);
    }
    else
    {
        double r = (deg / 180.0f) * 65536 ;
        unsigned short ang = (short)(r+0.5f);
        //  swap(ang);    
        return ang;
    }
    
}

unsigned short SensorSimulDlg::ToSensorXFormat(double deg)
{
    double mrad = ToMRad(deg);
    double max = (m_type == TYPE_SENSORX) ? 100.0f : 104.7f;    
    unsigned short ang = mrad/max*32768;

    return ang;
}


double SensorSimulDlg::CalcAngle(CRect& rect, CPoint& point)
{
    //     double x = abs(rect.CenterPoint().x-point.x);
    //     double y = abs(rect.CenterPoint().y-point.y);
    double x = point.x-rect.CenterPoint().x;
    double y = point.y-rect.CenterPoint().y;


    double ang = atan(y/x)*180/_PI;

    return ang;
}

void SensorSimulDlg::CheckPoint(CPoint& point)
{
    if(m_rollRect.PtInRect(point))
    {        
        m_rollAng = CalcAngle(m_rollRect, point);    
        m_protRoll = ToProtocolFormat(m_rollAng);
        //        TRACE("roll: %.2f\n", m_rollAng);
    }

    if(m_pitchRect.PtInRect(point))
    {
        m_pitchAng = CalcAngle(m_pitchRect, point);
        m_protPitch = ToProtocolFormat(m_pitchAng);

    }

    Invalidate(FALSE);
    UpdateData(FALSE);
}

void SensorSimulDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CheckPoint(point);

    CDialog::OnLButtonDown(nFlags, point);
}


void SensorSimulDlg::Draw(Gdiplus::Graphics &gr)
{     
    unsigned char gray = 200;
    Gdiplus::SolidBrush brush(Gdiplus::Color(255,gray,gray,gray));    
    Gdiplus::Pen pen(Gdiplus::Color(255,0,0,0), 2);
    Gdiplus::Pen gridPen(Gdiplus::Color(30,0,0,0), 1);
    int lenght = 200,x,y;

    USES_CONVERSION;

    Gdiplus::Font myFont(L"Georgia", 10);
    Gdiplus::PointF origin(m_rollRect.left, m_rollRect.top+5);
    Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));
    CString str;

    gr.FillRectangle(&brush, m_rollRect.left, m_rollRect.top, m_rollRect.Width(), m_rollRect.Height());
    gr.DrawLine(&gridPen, m_rollRect.left, m_rollRect.CenterPoint().y, m_rollRect.right, m_rollRect.CenterPoint().y);
    x = lenght*cos(m_rollAng*_PI/180.0f);
    y = lenght*sin(m_rollAng*_PI/180.0f);
    gr.DrawLine(&pen, m_rollRect.CenterPoint().x, m_rollRect.CenterPoint().y, m_rollRect.CenterPoint().x+x, m_rollRect.CenterPoint().y+y);
    x = lenght*cos((m_rollAng+180.0)*_PI/180.0f);
    y = lenght*sin((m_rollAng+180.0)*_PI/180.0f);
    gr.DrawLine(&pen, m_rollRect.CenterPoint().x, m_rollRect.CenterPoint().y, m_rollRect.CenterPoint().x+x, m_rollRect.CenterPoint().y+y);    



    origin = Gdiplus::PointF(m_rollRect.left, m_rollRect.top+5);
    str.Format("Roll: %.2f (%.1fmrad)",m_rollAng, ToMRad(m_rollAng));
    gr.DrawString(T2W((LPCTSTR)str), str.GetLength(), &myFont, origin, &blackBrush);


    gr.FillRectangle(&brush, m_pitchRect.left, m_pitchRect.top, m_pitchRect.Width(), m_pitchRect.Height());
    gr.DrawLine(&gridPen, m_pitchRect.left, m_pitchRect.CenterPoint().y, m_pitchRect.right, m_pitchRect.CenterPoint().y);
    x = lenght*cos(m_pitchAng*_PI/180.0f);
    y = lenght*sin(m_pitchAng*_PI/180.0f);
    gr.DrawLine(&pen, m_pitchRect.CenterPoint().x, m_pitchRect.CenterPoint().y, m_pitchRect.CenterPoint().x+x, m_pitchRect.CenterPoint().y+y);
    x = lenght*cos((m_pitchAng+180.0)*_PI/180.0f);
    y = lenght*sin((m_pitchAng+180.0)*_PI/180.0f);
    gr.DrawLine(&pen, m_pitchRect.CenterPoint().x, m_pitchRect.CenterPoint().y, m_pitchRect.CenterPoint().x+x, m_pitchRect.CenterPoint().y+y);    

    origin = Gdiplus::PointF(m_pitchRect.left, m_pitchRect.top+5);
    str.Format("Pitch: %.2f (%.1fmrad)",m_pitchAng, ToMRad(m_pitchAng));
    gr.DrawString(T2W((LPCTSTR)str), str.GetLength(), &myFont, origin, &blackBrush);
}

void SensorSimulDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    Gdiplus::Graphics gr(m_hWnd);

    CRect rect;
    GetClientRect(&rect);
    rect.bottom = rect.bottom - 190;

    Gdiplus::Bitmap bm(rect.Width(), rect.Height(), PixelFormat32bppARGB);
    Gdiplus::Graphics bmGr(&bm);
    bmGr.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);


    DWORD col = GetSysColor(COLOR_BTNFACE);   
    Gdiplus::SolidBrush brush(Gdiplus::Color(255,255,255,255));    
    //SolidBrush brush(Color(255,0xec,0xe9,0xd8));    
    bmGr.FillRectangle(&brush, 0,0,rect.Width(),rect.Height());


    Draw(bmGr);


    Gdiplus::BitmapData bitmapData;
    Gdiplus::Rect r(0, 0, bm.GetWidth(), bm.GetHeight());

    bm.LockBits(&r, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB,
        &bitmapData);

    CDC* pDc = GetDC();
    CDC memDc;
    memDc.CreateCompatibleDC(pDc);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDc, bm.GetWidth(), bm.GetHeight());
    bitmap.SetBitmapBits(bm.GetWidth()*bm.GetHeight()*4, bitmapData.Scan0);

    memDc.SelectObject(&bitmap);

    pDc->BitBlt(0,0,bm.GetWidth(),bm.GetHeight(),&memDc,0,0,SRCCOPY);
}

void SensorSimulDlg::OnMouseMove(UINT nFlags, CPoint point)
{

    if(nFlags & 0x1)
    {
        CheckPoint(point);
    }


    CDialog::OnMouseMove(nFlags, point);
}

void SensorSimulDlg::Send()
{
    SendDauData();
}

void SensorSimulDlg::OnBnClickedPitchAuto()
{
    UpdateData(TRUE);

//     if(m_autoRoll || m_autoPitch)
//     {
//         if(m_timer == 0)
//             m_timer= SetTimer(1, 100, NULL);
//     }
//     else
//     {
//         if(m_timer != 0)
//         {
//             KillTimer(m_timer);
//             m_timer = 0;
//         }
//     }
}

void SensorSimulDlg::OnBnClickedRollAuto()
{   
    UpdateData(TRUE);
//     if(m_autoRoll || m_autoPitch)
//     {
//         if(m_timer == 0)
//             m_timer= SetTimer(1, 100, NULL);
//     }
//     else
//     {
//         if(m_timer != 0)
//         {
//             KillTimer(m_timer);
//             m_timer = 0;
//         }
//     }

}

void SensorSimulDlg::Update()
{
    if(m_autoRoll)
    {        
        static double moveRAng = 90.0f;
        double rollNoice = ((double)(rand() % 65000)-32500.0f) / 32500.0f * m_rollNoice;    
        moveRAng += m_rollAngStep;
        m_rollAng = m_maxRollAng*cos(ToRad(moveRAng))+m_rollCenter;
        m_protRoll = ToProtocolFormat(m_rollAng+rollNoice);
        // TRACE("ang:%.2f, roll:%.2f\n", moveRAng,m_rollAng);
        if(m_animate)
            InvalidateRect(m_rollRect, FALSE);  
    }
    if(m_autoPitch)
    {     
        static double movePAng = 90.0f;
        double pitchNoice = ((double)(rand() % 65000)-32500.0f) / 32500.0f * m_pitchNoice;    
        movePAng += m_pitchAngStep;
        m_pitchAng = m_maxPitchAng*cos(ToRad(movePAng))+m_pitchCenter;
        m_protPitch = ToProtocolFormat(m_pitchAng+pitchNoice);
        //TRACE("ang:%.2f, roll:%.2f\n", movePAng,m_pitchAng);
        if(m_animate)
            InvalidateRect(m_pitchRect, FALSE);  
    }
    
}

void SensorSimulDlg::OnTimer(UINT nIDEvent)
{


    //Update();
    UpdateData(FALSE);

    //    InvalidateRect(FALSE);

    
    CDialog::OnTimer(nIDEvent);
}

void SensorSimulDlg::OnEnKillfocusMaxRollAng()
{
    UpdateData(TRUE);
    m_maxRollAng = atof(m_maxRollAngStr);
}

void SensorSimulDlg::OnEnKillfocusRollStep()
{
    UpdateData(TRUE);
    double periodTime = atof(m_rollAngStepStr);    
    m_rollAngStep = 360.0/(10*periodTime);    
}

void SensorSimulDlg::OnEnKillfocusMaxPitchAng()
{
    UpdateData(TRUE);
    m_maxPitchAng = atof(m_maxPitchAngStr);
}

void SensorSimulDlg::OnEnKillfocusPitchStep()
{
    UpdateData(TRUE);
    double periodTime = atof(m_pitchAngStepStr);
    m_pitchAngStep = 360.0/(10*periodTime);       
}

void SensorSimulDlg::OnEnChangeMaxRollAng()
{

    UpdateData(TRUE);
}

void SensorSimulDlg::OnEnChangeRollStep()
{

    UpdateData(TRUE);
}

void SensorSimulDlg::OnEnChangeMaxPitchAng()
{
    UpdateData(TRUE);
}

void SensorSimulDlg::OnEnChangePitchStep()
{   
    UpdateData(TRUE);
}

void SensorSimulDlg::OnEnKillfocusRollCenter()
{
    UpdateData(TRUE);
    m_rollCenter = atof(m_rollCenterStr);    
}

void SensorSimulDlg::OnEnChangeRollCenter()
{
    UpdateData(TRUE);
}

void SensorSimulDlg::OnEnKillfocusPitchCenter()
{
    m_pitchCenter = atof(m_pitchCenterStr);    
}

void SensorSimulDlg::OnEnChangePitchCenter()
{
    UpdateData(TRUE);
}


void SensorSimulDlg::OnEnChangeNoicePitch()
{
    UpdateData(TRUE);
}

void SensorSimulDlg::OnEnKillfocusNoicePitch()
{
    m_pitchNoice = atof(m_pitchNoiceStr);    
}

void SensorSimulDlg::OnEnChangeNoiceRoll()
{
    UpdateData(TRUE);
}

void SensorSimulDlg::OnEnKillfocusNoiceRoll()
{
    m_rollNoice = atof(m_rollNoiceStr);    
}

void SensorSimulDlg::OnBnClickedAnim()
{
    UpdateData(TRUE);
}

void SensorSimulDlg::OnCbnSelchangeTypeCombo()
{
    int index;
    if((index = m_typeCombo.GetCurSel()) == CB_ERR)
        return;

    m_type = m_typeCombo.GetItemData(index);
    TRACE("Type:%d\n",m_type);
}
