// DAUDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HDLCSend.h"
#include "DAUDlg.h"
#include "HDLCSendDlg.h"
#include ".\daudlg.h"

#include <math.h>

#define _PI 3.1415f
#define ToMRad(a) (a*_PI/180.0f*1000.0f)
#define ToRad(a) (a*_PI/180.0f)

ULONG_PTR           gdiplusToken;



// DAUDlg dialog

IMPLEMENT_DYNAMIC(DAUDlg, CDialog)
DAUDlg::DAUDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DAUDlg::IDD, pParent)    
    ,m_pParent(NULL)  
    ,m_rollAng(0.0f)
    ,m_pitchAng(0.0f)
    , m_protRoll(0)
    , m_protPitch(0)
    , m_autoRoll(FALSE)
    , m_autoPitch(FALSE)
    , m_timer(0)    
    , m_maxRollAngStr(_T(""))
    , m_rollAngStepStr(_T(""))
    , m_rollAngStep(6.0f)
    , m_maxRollAng(0.5f)
    , m_maxPitchAngStr(_T(""))
    , m_pitchAngStepStr(_T(""))
    , m_pitchAngStep(6.0f)
    , m_maxPitchAng(0.5f)
    , m_rollCenterStr(_T(""))
    , m_pitchCenterStr(_T(""))
    , m_rollCenter(0.0f)
    , m_pitchCenter(0.0f)
    , m_animate(TRUE)
    , m_rollNoiceStr(_T(""))
    , m_pitchNoiceStr(_T(""))
    , m_pitchNoice(0.0f)    
    , m_rollNoice(0.0f)    
    , m_bInduceCRCErr(FALSE)
{
    m_frequence.QuadPart = 0;
    ::QueryPerformanceFrequency(&m_frequence);
    ::QueryPerformanceCounter(&m_lastTime);
}

DAUDlg::~DAUDlg()
{
}

void DAUDlg::DoDataExchange(CDataExchange* pDX)
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
    DDX_Control(pDX, IDC_COMBO1, m_typeCombo);
    DDX_Text(pDX, IDC_ROLL_CENTER, m_rollCenterStr);
    DDX_Text(pDX, IDC_PITCH_CENTER, m_pitchCenterStr);
    DDX_Check(pDX, IDC_ANIM, m_animate);
    DDX_Text(pDX, IDC_NOICE_ROLL, m_rollNoiceStr);
    DDX_Text(pDX, IDC_NOICE_PITCH, m_pitchNoiceStr);
}


BEGIN_MESSAGE_MAP(DAUDlg, CDialog)
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
    ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
    
    ON_EN_KILLFOCUS(IDC_ROLL_CENTER, OnEnKillfocusRollCenter)
    ON_EN_CHANGE(IDC_ROLL_CENTER, OnEnChangeRollCenter)
    ON_EN_KILLFOCUS(IDC_PITCH_CENTER, OnEnKillfocusPitchCenter)
    ON_EN_CHANGE(IDC_PITCH_CENTER, OnEnChangePitchCenter)
    ON_EN_CHANGE(IDC_NOICE_PITCH, OnEnChangeNoicePitch)
    ON_EN_KILLFOCUS(IDC_NOICE_PITCH, OnEnKillfocusNoicePitch)
    ON_EN_CHANGE(IDC_NOICE_ROLL, OnEnChangeNoiceRoll)
    ON_EN_KILLFOCUS(IDC_NOICE_ROLL, OnEnKillfocusNoiceRoll)
    ON_BN_CLICKED(IDC_ANIM, OnBnClickedAnim)
END_MESSAGE_MAP()


void swap(unsigned short& data)
{
    short low = data & 0x00ff;
    short hi = data >> 8 & 0x00ff;
    data = MAKEWORD(hi, low);
}

// DAUDlg message handlers

BOOL DAUDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    int startY=10,ctrlHeight=120, margin=10;    
    CRect rect;
    GetClientRect(&rect);
    //rect.bottom = rect.bottom - 40;
    

    m_rollRect = CRect(margin, startY, rect.right-margin, startY+ctrlHeight);
    m_pitchRect = m_rollRect;    
    m_pitchRect.OffsetRect(0, ctrlHeight+1*margin);    
    
    m_typeCombo.AddString("Mins");    

    m_type = TYPE_MINS;
    m_typeCombo.SelectString(0, "Mins");

     
    m_maxRollAngStr.Format("%.2f",m_maxRollAng);
    m_rollAngStepStr.Format("%.2f",m_rollAngStep);
    m_maxPitchAngStr.Format("%.2f",m_maxPitchAng);
    m_pitchAngStepStr.Format("%.2f",m_pitchAngStep);

    m_rollCenterStr.Format("%.2f",m_rollCenter);
    m_pitchCenterStr.Format("%.2f",m_pitchCenter);

    m_rollNoiceStr.Format("%.2f",m_rollNoice);
    m_pitchNoiceStr.Format("%.2f",m_pitchNoice);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void DAUDlg::SendDauData()
{
    
    unsigned short roll = m_protRoll;
    swap(roll);
    unsigned short pitch = m_protPitch;
    swap(pitch);

    m_pParent->TransmitData(m_rollAng, m_pitchAng);
  
}


void DAUDlg::OnBnClickedSendButton()
{
    SendDauData();
}

unsigned short DAUDlg::ToProtocolFormat(double deg)
{
    double r = (deg / 180.0f) * 65536 ;
    unsigned short ang = (short)(r+0.5f);
  //  swap(ang);    
    return ang;
}

double DAUDlg::CalcAngle(CRect& rect, CPoint& point)
{
//     double x = abs(rect.CenterPoint().x-point.x);
//     double y = abs(rect.CenterPoint().y-point.y);
    double x = point.x-rect.CenterPoint().x;
    double y = point.y-rect.CenterPoint().y;


    double ang = atan(y/x)*180/_PI;

    return ang;
}

void DAUDlg::UpdateValues()
{    
    LARGE_INTEGER currTime;
    ::QueryPerformanceCounter(&currTime);        
    double elapsedTime = ((double) (currTime.QuadPart - m_lastTime.QuadPart))  /(double) m_frequence.QuadPart;	// ms

    m_lastTime = currTime;

    double rollPeriodTime = atof(m_rollAngStepStr);    
    double rollStep = 360.0*(elapsedTime/rollPeriodTime);

    double pitchPeriodTime = atof(m_pitchAngStepStr);
    double pitchStep = 360.0*(elapsedTime/pitchPeriodTime);

    static int c=0;
    //TRACE("Update:%.4f\n",elapsedTime);
    //     trace.Format("t:%.5f",elapsedTime*1000.0f);
    //     sw.TraceTime(TRUE, TRUE, trace);

    if(m_autoRoll)
    {        
        static double moveRAng = 90.0f;
        double rollNoice = ((double)(rand() % 65000)-32500.0f) / 32500.0f * m_rollNoice;    
        moveRAng += rollStep;
        m_rollAng = m_maxRollAng*cos(ToRad(moveRAng))+m_rollCenter;
        m_protRoll = ToProtocolFormat(m_rollAng+rollNoice);
        // TRACE("ang:%.2f, roll:%.2f\n", moveRAng,m_rollAng);
//         if(m_animate)
//             InvalidateRect(m_rollRect, FALSE);  
    }
    if(m_autoPitch)
    {     
        static double movePAng = 90.0f;
        double pitchNoice = ((double)(rand() % 65000)-32500.0f) / 32500.0f * m_pitchNoice;    
        movePAng += pitchStep;
        m_pitchAng = m_maxPitchAng*cos(ToRad(movePAng))+m_pitchCenter;
        m_protPitch = ToProtocolFormat(m_pitchAng+pitchNoice);
    }


}

void DAUDlg::CheckPoint(CPoint& point)
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

void DAUDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CheckPoint(point);

    CDialog::OnLButtonDown(nFlags, point);
}


void DAUDlg::Draw(Graphics &gr)
{     
//     unsigned char gray = 200;
//     SolidBrush brush(Color(255,gray,gray,gray));    
//     Pen pen(Color(255,0,0,0), 1);
//     Pen gridPen(Color(30,0,0,0), 1);
//     int lenght = 200,x,y;
// 
//     USES_CONVERSION;
//     
//     Gdiplus::Font myFont(L"Georgia", 10);
//     PointF origin(m_rollRect.left, m_rollRect.top+5);
//     SolidBrush blackBrush(Color(255, 0, 0, 0));
//     CString str;
// 
//     gr.FillRectangle(&brush, m_rollRect.left, m_rollRect.top, m_rollRect.Width(), m_rollRect.Height());
//     gr.DrawLine(&gridPen, m_rollRect.left, m_rollRect.CenterPoint().y, m_rollRect.right, m_rollRect.CenterPoint().y);
//     x = lenght*cos(m_rollAng*_PI/180.0f);
//     y = lenght*sin(m_rollAng*_PI/180.0f);
//     gr.DrawLine(&pen, m_rollRect.CenterPoint().x, m_rollRect.CenterPoint().y, m_rollRect.CenterPoint().x+x, m_rollRect.CenterPoint().y+y);
//     x = lenght*cos((m_rollAng+180.0)*_PI/180.0f);
//     y = lenght*sin((m_rollAng+180.0)*_PI/180.0f);
//     gr.DrawLine(&pen, m_rollRect.CenterPoint().x, m_rollRect.CenterPoint().y, m_rollRect.CenterPoint().x+x, m_rollRect.CenterPoint().y+y);    
//     
// 
//    
//     origin = PointF(m_rollRect.left, m_rollRect.top+5);
//     str.Format("Roll: %.2f (%.1fmrad)",m_rollAng, ToMRad(m_rollAng));
//     gr.DrawString(T2W((LPCTSTR)str), str.GetLength(), &myFont, origin, &blackBrush);
// 
//     
//     gr.FillRectangle(&brush, m_pitchRect.left, m_pitchRect.top, m_pitchRect.Width(), m_pitchRect.Height());
//     gr.DrawLine(&gridPen, m_pitchRect.left, m_pitchRect.CenterPoint().y, m_pitchRect.right, m_pitchRect.CenterPoint().y);
//     x = lenght*cos(m_pitchAng*_PI/180.0f);
//     y = lenght*sin(m_pitchAng*_PI/180.0f);
//     gr.DrawLine(&pen, m_pitchRect.CenterPoint().x, m_pitchRect.CenterPoint().y, m_pitchRect.CenterPoint().x+x, m_pitchRect.CenterPoint().y+y);
//     x = lenght*cos((m_pitchAng+180.0)*_PI/180.0f);
//     y = lenght*sin((m_pitchAng+180.0)*_PI/180.0f);
//     gr.DrawLine(&pen, m_pitchRect.CenterPoint().x, m_pitchRect.CenterPoint().y, m_pitchRect.CenterPoint().x+x, m_pitchRect.CenterPoint().y+y);    
// 
//     origin = PointF(m_pitchRect.left, m_pitchRect.top+5);
//     str.Format("Pitch: %.2f (%.1fmrad)",m_pitchAng, ToMRad(m_pitchAng));
//     gr.DrawString(T2W((LPCTSTR)str), str.GetLength(), &myFont, origin, &blackBrush);
    unsigned char gray = 200;
    SolidBrush brush(Color(255,gray,gray,gray));    
    Pen pen(Color(255,0,0,0), 2);
    Pen gridPen(Color(30,0,0,0), 1);
    int lenght = 200,x,y;

    USES_CONVERSION;

    Gdiplus::Font myFont(L"Georgia", 10);
    PointF origin(m_rollRect.left, m_rollRect.top+5);
    SolidBrush blackBrush(Color(255, 0, 0, 0));
    CString str;

    gr.FillRectangle(&brush, m_rollRect.left, m_rollRect.top, m_rollRect.Width(), m_rollRect.Height());
    gr.DrawLine(&gridPen, m_rollRect.left, m_rollRect.CenterPoint().y, m_rollRect.right, m_rollRect.CenterPoint().y);
    x = lenght*cos(m_rollAng*_PI/180.0f);
    y = lenght*sin(m_rollAng*_PI/180.0f);
    gr.DrawLine(&pen, m_rollRect.CenterPoint().x, m_rollRect.CenterPoint().y, m_rollRect.CenterPoint().x+x, m_rollRect.CenterPoint().y+y);
    x = lenght*cos((m_rollAng+180.0)*_PI/180.0f);
    y = lenght*sin((m_rollAng+180.0)*_PI/180.0f);
    gr.DrawLine(&pen, m_rollRect.CenterPoint().x, m_rollRect.CenterPoint().y, m_rollRect.CenterPoint().x+x, m_rollRect.CenterPoint().y+y);    



    origin = PointF(m_rollRect.left, m_rollRect.top+5);
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

    origin = PointF(m_pitchRect.left, m_pitchRect.top+5);
    str.Format("Pitch: %.2f (%.1fmrad)",m_pitchAng, ToMRad(m_pitchAng));
    gr.DrawString(T2W((LPCTSTR)str), str.GetLength(), &myFont, origin, &blackBrush);
}

void DAUDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    
    Graphics gr(m_hWnd);

    CRect rect;
    GetClientRect(&rect);
    rect.bottom = rect.bottom - 175;

    Bitmap bm(rect.Width(), rect.Height(), PixelFormat32bppARGB);
    Graphics bmGr(&bm);
    bmGr.SetSmoothingMode(SmoothingModeAntiAlias);


    DWORD col = GetSysColor(COLOR_BTNFACE);   
    SolidBrush brush(Color(255,255,255,255));    
    //SolidBrush brush(Color(255,0xec,0xe9,0xd8));    
    bmGr.FillRectangle(&brush, 0,0,rect.Width(),rect.Height());


    Draw(bmGr);


    BitmapData bitmapData;
    Rect r(0, 0, bm.GetWidth(), bm.GetHeight());

    bm.LockBits(&r, ImageLockModeRead, PixelFormat32bppARGB,
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

void DAUDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    
    if(nFlags & 0x1)
    {
        CheckPoint(point);
    }


    CDialog::OnMouseMove(nFlags, point);
}

void DAUDlg::Send()
{
    SendDauData();
}

void DAUDlg::OnBnClickedPitchAuto()
{
    UpdateData(TRUE);

    if(m_autoRoll || m_autoPitch)
    {
        if(m_timer == 0)
            m_timer= SetTimer(1, 100, NULL);
    }
    else
    {
        if(m_timer != 0)
        {
            KillTimer(m_timer);
            m_timer = 0;
        }
    }
}

void DAUDlg::OnBnClickedRollAuto()
{   
    UpdateData(TRUE);
    if(m_autoRoll || m_autoPitch)
    {
        if(m_timer == 0)
            m_timer= SetTimer(1, 100, NULL);
    }
    else
    {
        if(m_timer != 0)
        {
            KillTimer(m_timer);
            m_timer = 0;
        }
    }

}

void DAUDlg::OnTimer(UINT nIDEvent)
{
    if(m_autoRoll)
    {        
//         static double moveRAng = 90.0f;
//         moveRAng += m_rollAngStep;
//         m_rollAng = m_maxRollAng*cos(ToRad(moveRAng));
//         m_protRoll = ToProtocolFormat(m_rollAng);
        // TRACE("ang:%.2f, roll:%.2f\n", moveRAng,m_rollAng);
        if(m_animate)
            InvalidateRect(m_rollRect, FALSE);  
    }
    if(m_autoPitch)
    {     
//         static double movePAng = 90.0f;
//         movePAng += m_pitchAngStep;
//         m_pitchAng = m_maxPitchAng*cos(ToRad(movePAng));
//         m_protPitch = ToProtocolFormat(m_pitchAng);
        //TRACE("ang:%.2f, roll:%.2f\n", movePAng,m_pitchAng);
        if(m_animate)
            InvalidateRect(m_pitchRect, FALSE);  
    }


//    InvalidateRect(FALSE);

    UpdateData(FALSE);
    CDialog::OnTimer(nIDEvent);
}

void DAUDlg::OnEnKillfocusMaxRollAng()
{
    UpdateData(TRUE);
    m_maxRollAng = atof(m_maxRollAngStr);
}

void DAUDlg::OnEnKillfocusRollStep()
{
    UpdateData(TRUE);
    m_rollAngStep = atof(m_rollAngStepStr);    
}

void DAUDlg::OnEnKillfocusMaxPitchAng()
{
    UpdateData(TRUE);
    m_maxPitchAng = atof(m_maxPitchAngStr);
}

void DAUDlg::OnEnKillfocusPitchStep()
{
    UpdateData(TRUE);
    m_pitchAngStep = atof(m_pitchAngStepStr);    
}

void DAUDlg::OnEnChangeMaxRollAng()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    UpdateData(TRUE);
}

void DAUDlg::OnEnChangeRollStep()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    UpdateData(TRUE);
}

void DAUDlg::OnEnChangeMaxPitchAng()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    UpdateData(TRUE);
}

void DAUDlg::OnEnChangePitchStep()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    UpdateData(TRUE);
}

void DAUDlg::OnCbnSelchangeCombo1()
{
    CString type;
    m_typeCombo.GetLBText(m_typeCombo.GetCurSel(), type);
}

void DAUDlg::OnEnKillfocusRollCenter()
{
    UpdateData(TRUE);
    m_rollCenter = atof(m_rollCenterStr);    
}

void DAUDlg::OnEnChangeRollCenter()
{
    UpdateData(TRUE);
}

void DAUDlg::OnEnKillfocusPitchCenter()
{
    m_pitchCenter = atof(m_pitchCenterStr);    
}

void DAUDlg::OnEnChangePitchCenter()
{
    UpdateData(TRUE);
}

void DAUDlg::OnEnChangeNoicePitch()
{
    UpdateData(TRUE);
}

void DAUDlg::OnEnKillfocusNoicePitch()
{
    m_pitchNoice = atof(m_pitchNoiceStr);    
}

void DAUDlg::OnEnChangeNoiceRoll()
{
    UpdateData(TRUE);
}

void DAUDlg::OnEnKillfocusNoiceRoll()
{
    m_rollNoice = atof(m_rollNoiceStr);    
}

void DAUDlg::OnBnClickedAnim()
{
    UpdateData(TRUE);
}

