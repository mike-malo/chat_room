#include "chatlist.h"
#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include "itemwidget.h"
#include "clientcore.h"

ChatList::ChatList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatList) {
    ui->setupUi(this);

    core = &ClientCore::getInstance();

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //绘制阴影
    QGraphicsDropShadowEffect * shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(0, 0);
    QColor color = Qt::black;
    color.setAlpha(64);
    shadowEffect->setColor(color);
    shadowEffect->setBlurRadius(20);
    ui->frame->setGraphicsEffect(shadowEffect);
    //设置关闭按钮
    ui->closeButton->setIcon(QPixmap(":/icon/icon/close.png"));

    ui->label->setText("当前用户: " + core->currentUserName + " 工号: " + core->account);

    refreshChatList();

}

ChatList::~ChatList()
{
    delete ui;
}

void ChatList::on_closeButton_clicked() {
    this->close();
}

void ChatList::on_sendButton_clicked() {
    QString message = ui -> MessageInput -> toPlainText();
    qDebug() << message;
    qDebug() << core->user_id;

    if (core->send_message(message)) {

    }

    ui->MessageInput->clear();
}

void ChatList::refreshChatList() {

    if (core->getChatListRequest(core->uid)) {
        //    ui->listWidget->addItems(core->chatList);

        ui->listWidget->clear();
        foreach (const QJsonValue &contactValue, core->chatList) {
            QJsonObject contactObj = contactValue.toObject();

            QString username = contactObj["showname"].toString();
            QString last_msgstr = contactObj["last_msgstr"].toString();
            QString cdatestr = contactObj["cdatestr"].toString();
            QString type_id = contactObj["typeid"].toString();

//            qDebug() << "消息id: " << contactObj["id"].toString();
//            qDebug() << "消息发送人: " << contactObj["showname"].toString();
//            qDebug() << "消息: " << contactObj["last_msgstr"].toString();
//            qDebug() << "时间: " << contactObj["cdatestr"].toString();
//            qDebug() << "typeid: " << contactObj["typeid"].toString();
            AddItem(username, last_msgstr, cdatestr, type_id);
        }

    }
    connect(ui->listWidget, &QListWidget::itemClicked, this, &ChatList::onItemClicked);
}

void ChatList::AddItem(const QString &user_name, const QString &message, const QString &message_time, const QString &type_id) {
    ItemWidget* pItemWidget = new ItemWidget(this);
    pItemWidget->SetData(user_name, message, message_time, type_id);
    QListWidgetItem* pItem = new QListWidgetItem();

    pItem->setSizeHint(QSize(256, 60));

    ui->listWidget->addItem(pItem);
    ui->listWidget->setItemWidget(pItem, pItemWidget);

    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ChatList::onItemClicked(QListWidgetItem *item) {

    ItemWidget *itemWidget = qobject_cast<ItemWidget*>(ui->listWidget->itemWidget(item));

    // 获取参数
    QString typeId = itemWidget->getTypeId();
    qDebug() << "点击了" << typeId;
    chatHistory.clear();
    if (core->get_message(typeId)) {
        foreach (const QJsonValue &contactValue, core->msgList) {
            QJsonObject contactObj = contactValue.toObject();

            QString fromname = contactObj["fromname"].toString();
            QString msgtxt = contactObj["msgtxt"].toString();
            QString ctime = contactObj["ctime"].toString();

//            qDebug() << "发送人: " << contactObj["fromname"].toString();
//            qDebug() << "发送日期: " << contactObj["ctime"].toString();
//            qDebug() << "内容: " << contactObj["msgtxt"].toString();
            if (contactObj["withdraw"].toString() == "1") {
                chatHistory.append(fromname + "撤回了一条消息");
            }
            chatHistory.append(fromname + ": " + msgtxt + " " + ctime);
        }
        ui->textBrowser->setPlainText(chatHistory.join("\n"));
        ui->textBrowser->moveCursor(QTextCursor::End);
    }

}

// 鼠标事件
void ChatList::mousePressEvent(QMouseEvent * event) {
    diff_pos = this->pos()-event->globalPos();
}
void ChatList::mouseMoveEvent(QMouseEvent *event) {
    this->move(event->globalPos()+diff_pos);
}

