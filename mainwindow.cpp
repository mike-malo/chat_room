#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "chatlist.h"

#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    ui->password_input->setEchoMode(QLineEdit::EchoMode::Password);

    //绘制阴影
    QGraphicsDropShadowEffect * shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(0, 0);
    QColor color = Qt::black;
    color.setAlpha(64);
    shadowEffect->setColor(color);
    shadowEffect->setBlurRadius(20);
    ui->frame->setGraphicsEffect(shadowEffect);

    // 获取核心实例
    core = &ClientCore::getInstance();

    ui->ConnectButton->setEnabled(false);
    ui->countDown->setText("无法连接到服务器");

    if (core->connectServer()) {
        ui->ConnectButton->setEnabled(true);
        ui->countDown->setText("已连接到服务器");
    }
}

mainwindow::~mainwindow()
{
    delete ui;
}

void mainwindow::on_CloseButton_clicked()
{
    this->close();
}

void mainwindow::on_ConnectButton_clicked()
{
    QString accout = ui->accout_input->text();
    QString password = ui->password_input->text();

    QString errorMsg;

    if (core->loginRequest(accout, password, errorMsg)){
        ChatList * userChatList = new ChatList();
        this->close();
        userChatList->show();
    }
    else
    {
        QMessageBox::critical(this, "错误", errorMsg);
    }
}

void mainwindow::mousePressEvent(QMouseEvent * event)
{
    diff_pos = this->pos()-event->globalPos();
}
void mainwindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+diff_pos);
}
