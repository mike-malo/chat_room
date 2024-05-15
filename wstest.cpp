#include "wstest.h"
#include <QJsonObject>
#include <QJsonDocument>

wstest::wstest()
{
    connectStatus = false;
    m_timer = new QTimer();
}

void wstest::createDataRecvWS() {
    dataRecvWS.open(QUrl("ws://192.168.50.52:8000/"));
    connect(&dataRecvWS, &QWebSocket::disconnected, this, &wstest::onDisConnected);
    connect(&dataRecvWS, &QWebSocket::textMessageReceived, this, &wstest::onTextReceived);
    connect(&dataRecvWS, &QWebSocket::connected, this, &wstest::onConnected);
}

void wstest::onTextReceived(QString msg) {
    qDebug() << "ws接收的信息" << msg;
}

void wstest::onDisConnected() {
    connectStatus = false;
    qDebug() << "未连接";
    m_timer->start(3000);
}

void wstest::reconnect() {
    dataRecvWS.abort();
    dataRecvWS.open(QUrl("ws://10.0.0.178:7999"));
}

bool wstest::sendMessage(QString msg) {

    if (dataRecvWS.sendTextMessage(msg)) {
        qDebug() << "已发送";
        return true;
    }
    else {
        qDebug() << "未发送";
        return false;
    }
}

void wstest::onConnected() {
    connectStatus = true;
    qDebug() << "已连接" << connectStatus;
    m_timer->stop();

    QJsonObject obj;
//    obj["ac"] = "05092";
//    obj["pw"] = "123456";
    obj["task"] = "test";

//    sendMessage(QString(QJsonDocument(obj).toJson(QJsonDocument::Compact)));

    QJsonDocument jsonDoc(obj);
    QString message = jsonDoc.toJson(QJsonDocument::Compact);
    dataRecvWS.sendTextMessage(message.toUtf8());
}
