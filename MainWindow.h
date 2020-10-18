/*************************************************************************
 *
 * This file is part of tgtsml_tools
 *
 * Create Date 2020/10/18
 *
 * Copyright: tgtsml
 *
 * Contact email: time_forget@outlook.com
 *
 * GitHub: www.github.com/tgtsml
 *
 *************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    bool m_isMousePressed;
    QPixmap *m_screenPicture;
    QPixmap *backgroundPicture;
    QPoint m_startPos, m_endPos;
    QMenu *m_screenMenu, *m_trayMenu;

private:
    QRect getCapturedRect(QPoint startpos, QPoint endpos);
    void clearScreenCaptureInfo();
    QString getSaveFileName();
    void activateTrayIcon(QSystemTrayIcon::ActivationReason reason);

private:
    void slot_saveCapturedScreen();
    void slot_saveFullScreen();
};
#endif // MAINWINDOW_H
