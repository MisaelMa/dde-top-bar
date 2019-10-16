#include "mainpanel.h"
#include "utils/global.h"
#include <QScreen>
#include <QApplication>
#include <QApplication>
#include <QRect>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <QtX11Extras/QX11Info>
#include <DPlatformWindowHandle>
#include <DForeignWindow>
#include <QTimer>
#include <DWindowManagerHelper>
#include <QScreen>
#include <QMessageBox>

DWIDGET_USE_NAMESPACE

#define DOCK_POS_TOP 0
#define DOCK_POS_RIGHT 1
#define DOCK_POS_BOTTOM 2
#define DOCK_POS_LEFT 3


MainPanel::MainPanel(QWidget *parent)
    :  DBlurEffectWidget(parent)
    , m_backgroundAni(new QVariantAnimation(this))
    , m_defaultBgColor(maskColor())
{
    init();
    screenChanged();
}

void MainPanel::init()
{
    m_structWidget = new QWidget;
    m_structWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
    m_structWidget->setAttribute(Qt::WA_TranslucentBackground);

    m_desktopWidget = QApplication::desktop();

    setWindowFlags(Qt::X11BypassWindowManagerHint);
    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setAttribute(Qt::WA_TranslucentBackground);
    setMaskColor(DBlurEffectWidget::DarkColor);
    setFull(true);

    DPlatformWindowHandle handle(this);
    handle.setShadowColor(QColor(0, 0, 0, 0));

    m_defaultBgColor.setAlpha(maskAlpha());

   // m_mainPanel = new dtb::MainPanel(this);

    m_dockInter = new DockInter("com.deepin.dde.daemon.Dock",
                                "/com/deepin/dde/daemon/Dock",
                                QDBusConnection::sessionBus(),
                                this);
}
void MainPanel::initAnimation()
{
    m_launchAni = new QPropertyAnimation(this, "pos", this);
    m_launchAni->setDuration(1000);
    m_launchAni->setEasingCurve(QEasingCurve::OutBounce);
    m_backgroundAni->setDuration(500);

}



void MainPanel::screenChanged()
{
    xcb_ewmh_connection_t m_ewmh_connection;
    xcb_intern_atom_cookie_t *cookie = xcb_ewmh_init_atoms(QX11Info::connection(), &m_ewmh_connection);
    xcb_ewmh_init_atoms_replies(&m_ewmh_connection, cookie, nullptr);

    xcb_atom_t atoms[1];
    atoms[0] = m_ewmh_connection._NET_WM_WINDOW_TYPE_DOCK;
    xcb_ewmh_set_wm_window_type(&m_ewmh_connection, m_structWidget->winId(), 1, atoms);

    xcb_ewmh_wm_strut_partial_t strutPartial;
    memset(&strutPartial, 0, sizeof(xcb_ewmh_wm_strut_partial_t));

    // clear strut partial
    xcb_ewmh_set_wm_strut_partial(&m_ewmh_connection,  m_structWidget->winId(), strutPartial);

#ifdef ENABLE_RATOTE
    if (!isRotated()) {
        m_launchAni->stop();
        m_structWidget->hide();
        return hide();
    }
#endif

    const int frameHeight = DWindowManagerHelper::instance()->hasComposite() ? FRAMEHEIGHT : TOPHEIGHT;

    // set strct partial
    xcb_ewmh_wm_strut_partial_t strut_partial;
    memset(&strut_partial, 0, sizeof(xcb_ewmh_wm_strut_partial_t));

    const QRect dockRect = m_dockInter->frontendWindowRect();
    const QRect primaryRect = QApplication::primaryScreen()->geometry();
/*
   QMessageBox *a  = new QMessageBox(this);
   a->setText(QString::number(m_dockInter->position()));
   a->show();*/

    switch (m_dockInter->position()) {
    case DOCK_POS_BOTTOM:
        //space que ocupa la barra
        strut_partial.top = TOPHEIGHT * devicePixelRatioF()+5;
        strut_partial.top_start_x = primaryRect.x();
        strut_partial.top_end_x = primaryRect.x() + primaryRect.width();

        //tamnaño width an height
        setFixedSize(primaryRect.width(), frameHeight-4);

        //poscion left anf right
        move(primaryRect.x(), primaryRect.y());
       // m_mainPanel->resize(primaryRect.width(), TOPHEIGHT);
       // m_mainPanel->move(0, 0);
        break;
    case DOCK_POS_LEFT:
        strut_partial.top = TOPHEIGHT * devicePixelRatioF();
        strut_partial.top_start_x = primaryRect.x();
        strut_partial.top_end_x = primaryRect.x() + primaryRect.width();

        if (dockRect.bottomRight().y() / devicePixelRatioF() + TOPHEIGHT >= primaryRect.height()) {
         //   m_mainPanel->resize(primaryRect.width() - dockRect.topRight().x() / devicePixelRatioF(), TOPHEIGHT);
           // m_mainPanel->move(0, 0);
            setFixedSize(primaryRect.width() - dockRect.topRight().x() / devicePixelRatioF(), frameHeight);
            move(primaryRect.x() + dockRect.topRight().x() / devicePixelRatioF(), primaryRect.y());
        }
        else {
            //m_mainPanel->resize(primaryRect.width(), TOPHEIGHT);
            //m_mainPanel->move(0, 0);
            setFixedSize(primaryRect.width(), frameHeight);
            move(primaryRect.x(), primaryRect.y());
        }
        break;
    case DOCK_POS_RIGHT:
        strut_partial.top = TOPHEIGHT * devicePixelRatioF();
        strut_partial.top_start_x = primaryRect.x();
        strut_partial.top_end_x = primaryRect.x() + primaryRect.width();

        if (dockRect.height() / devicePixelRatioF() + TOPHEIGHT >= primaryRect.height()) {
            setFixedSize((dockRect.topLeft().x() - primaryRect.x()) / devicePixelRatioF(), frameHeight);
         //   m_mainPanel->resize((dockRect.topLeft().x() - primaryRect.x()) / devicePixelRatioF(), TOPHEIGHT);
        }
        else {
            setFixedSize(primaryRect.width(), frameHeight);
           // m_mainPanel->resize(primaryRect.width(), TOPHEIGHT);
        }

        move(primaryRect.x(), primaryRect.y());
        //m_mainPanel->move(0, 0);
        break;
    case DOCK_POS_TOP:


        strut_partial.top = TOPHEIGHT * devicePixelRatioF() + dockRect.height() / devicePixelRatioF();
        strut_partial.top_start_x = primaryRect.x();
        strut_partial.top_end_x = primaryRect.x() + primaryRect.width();

        move(primaryRect.x(), primaryRect.y() + dockRect.bottomRight().y() / devicePixelRatioF());
        setFixedSize(primaryRect.width(), frameHeight);
       // m_mainPanel->resize(primaryRect.width(), TOPHEIGHT);
        //m_mainPanel->move(0, 0);
        break;
    default:
        break;
    }

    m_structWidget->move(geometry().topLeft());
    //m_structWidget->setFixedSize(m_mainPanel->size());

    xcb_ewmh_set_wm_strut_partial(&m_ewmh_connection, m_structWidget->winId(), strut_partial);
    m_structWidget->show();

    //updateBorderPath();
}

MainPanel::~MainPanel()
{
    m_dockInter->deleteLater();
    m_desktopWidget->deleteLater();
}

