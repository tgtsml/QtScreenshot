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

#include "MainWindow.h"
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QDateTime>
#include <QDir>

#define CURRENT_DATETIME QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_isMousePressed(false), m_startPos(1,1), m_endPos(1,1)
{
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    m_screenPicture = new QPixmap();
    m_screenMenu = new QMenu(this);
    m_screenMenu->addAction("保存截图", this, &MainWindow::slot_saveCapturedScreen);
    m_screenMenu->addAction("保存全屏截图", this, &MainWindow::slot_saveFullScreen);
    m_screenMenu->addAction("退出", this, &MainWindow::close);

    QIcon appIcon(":/app.png");
    this->setWindowIcon(appIcon);

    m_trayMenu = new QMenu(this);
    m_trayMenu->addAction("新建截图", this, &MainWindow::showFullScreen);
    m_trayMenu->addAction("退出", this, &MainWindow::close);
    QSystemTrayIcon *m_trayIcon = new QSystemTrayIcon(appIcon, this);
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->show();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::activateTrayIcon);
}

void MainWindow::activateTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick){
        this->showFullScreen();
    }
}

MainWindow::~MainWindow()
{
    if(m_screenPicture){
        delete m_screenPicture;
    }
}

void MainWindow::showEvent(QShowEvent *)
{
    QSize desktopSize = QApplication::desktop()->size();
    QScreen *pscreen = QApplication::primaryScreen();
    *m_screenPicture = pscreen->grabWindow(QApplication::desktop()->winId(), 0, 0, desktopSize.width(), desktopSize.height());

    QPixmap pix(desktopSize.width(), desktopSize.height());
    pix.fill((QColor(0, 0, 0, 150)));
    backgroundPicture = new QPixmap(*m_screenPicture);
    QPainter painter(backgroundPicture);
    painter.drawPixmap(0, 0, pix);
}

QRect MainWindow::getCapturedRect(QPoint startpos, QPoint endpos)
{
    if(startpos == endpos){
        return QRect();
    }
    QPoint tmpTopLeftPos = startpos, tmpBottomRightPos = endpos;
    if(endpos.x() < startpos.x()){
        tmpBottomRightPos.setX(startpos.x());
        tmpTopLeftPos.setX(endpos.x());
    }
    if(endpos.y() < startpos.y()){
        tmpBottomRightPos.setY(startpos.y());
        tmpTopLeftPos.setY(endpos.y());
    }
    return QRect(tmpTopLeftPos, tmpBottomRightPos);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, *backgroundPicture);

    QRect rect(getCapturedRect(m_startPos, m_endPos));
    if (rect.isValid()) {
        painter.drawPixmap(rect.x(), rect.y(), m_screenPicture->copy(rect));
    }
    pen.setColor(Qt::green);
    painter.setPen(pen);
    painter.drawText(rect.x(), rect.y() - 8, tr("截图范围：(%1 x %2) - (%3 x %4)  图片大小：(%5 x %6)")
                     .arg(rect.x())
                     .arg(rect.y())
                     .arg(rect.bottomRight().x())
                     .arg(rect.bottomRight().y())
                     .arg(rect.width())
                     .arg(rect.height()));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isMousePressed){
        QPoint tmpPos = event->pos();
        m_endPos = tmpPos;
        this->update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_isMousePressed = true;
        m_startPos = event->pos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_isMousePressed = false;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *)
{
    this->setCursor(Qt::ArrowCursor);
    m_screenMenu->exec(cursor().pos());
}

void MainWindow::clearScreenCaptureInfo()
{
    m_startPos = m_endPos;
}

QString MainWindow::getSaveFileName()
{
    QString saveDirPath = qApp->applicationDirPath();
    QString picutreSaveDir("screenshot");
    QDir saveDir(saveDirPath);
    if(!saveDir.exists(picutreSaveDir)){
        saveDir.mkdir(picutreSaveDir);
    }
    return saveDirPath + "/" + picutreSaveDir + "/screenshot_" + CURRENT_DATETIME + ".png";
}

void MainWindow::slot_saveCapturedScreen()
{
    QRect rect(getCapturedRect(m_startPos, m_endPos));
    m_screenPicture->copy(rect).save(getSaveFileName(), "png");
    this->hide();
    clearScreenCaptureInfo();
}

void MainWindow::slot_saveFullScreen()
{
    m_screenPicture->save(getSaveFileName(), "png");
    this->hide();
    clearScreenCaptureInfo();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        this->hide();
        clearScreenCaptureInfo();
    }
}
