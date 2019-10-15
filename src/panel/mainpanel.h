#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <DBlurEffectWidget>
#include <QPropertyAnimation>
#include <DPlatformWindowHandle>
#include <QPropertyAnimation>
#include <DForeignWindow>
#include <com_deepin_dde_daemon_dock.h>
#include <memory>

using DockInter = com::deepin::dde::daemon::Dock;

DWIDGET_USE_NAMESPACE

class MainPanel : public DBlurEffectWidget
{
    Q_OBJECT

public:
    MainPanel(QWidget *parent = nullptr);
    ~MainPanel();

private slots:
    void screenChanged();
private:
    void init();
    void initConnect();
    void initAnimation();
    void onWindowPosChanged(DForeignWindow *window);
    void updateBackground();
    void updateBorderPath();
    void setBackground(const QColor &color);
    void onBackgroundChanged(const QColor &color);
    QPainterPath pathHandle() const;

private:
    QDesktopWidget *m_desktopWidget;
    DPlatformWindowHandle *m_handle;
    QPropertyAnimation *m_launchAni;
    DockInter *m_dockInter;
    QWidget *m_structWidget;

    QMap<WId,DForeignWindow*> m_windowList;
    QList<WId> m_windowIdList;
    QList<WId> m_maxWindowList;
    QList<WId> m_overlapping;
    QList<WId> m_fullWindowList;
    QVariantAnimation *m_backgroundAni;
    QColor m_defaultBgColor;
    QColor m_backgroundColor;
};
#endif // MAINWINDOW_H
