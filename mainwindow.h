#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "clientcore.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void on_CloseButton_clicked();
    void on_ConnectButton_clicked();

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::mainwindow *ui;

    QPoint diff_pos;
    ClientCore *core;
};
#endif // MAINWINDOW_H
