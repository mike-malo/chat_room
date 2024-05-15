#include "clientcore.h"
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include "wstest.h"

bool ClientCore::connectServer()
{
    socket.connectToHost("10.0.0.178", 7999);

    QJsonObject jsonObj;
    jsonObj["task"] = "connecting";

    QJsonObject dataObj;
    dataObj["test"] = "test";

    QString response;

    if (socket.waitForConnected()) {
        if (sendAndWait(response, jsonObj)) {
            qDebug() << response;
            return true;
        }
    }

    return false;
}

bool ClientCore::loginRequest(const QString &accout, const QString &password, QString &errorMsg) {

    QJsonObject jsonObj;
    jsonObj["task"] = "login";
    jsonObj["ac"] = accout;
    jsonObj["pw"] = password;

    QString response;

    if (sendAndWait(response, jsonObj)) {
//        qDebug() << response;
        QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
//        QJsonDocument resJsonDoc = QJsonValue::toObject(resJsonVal);
        qDebug() << typeid(resJsonDoc).name();


        qDebug() << "返回值" << resJsonDoc["success"].toBool();

        // 是否成功登录
        if (!resJsonDoc["success"].toBool())
        {
            errorMsg = resJsonDoc["msg"].toString();
            qDebug() << resJsonDoc["task"];
            qDebug() << resJsonDoc["success"];
            return false;
        }

        QJsonObject uInfo = resJsonDoc["uinfo"].toObject();
        qDebug() << "工号: " << uInfo["staff_code"].toString();
        qDebug() << "姓名: " << uInfo["name"].toString();
        qDebug() << "uid: " << uInfo["uid"].toString();

        currentUserName = uInfo["name"].toString();
        uid = uInfo["uid"].toString();
        account = uInfo["staff_code"].toString();
    }
    return true;
}

bool ClientCore::getChatListRequest(const QString &uid) {
    QJsonObject jsonObj;
    jsonObj["task"] = "chatlist_get";
    jsonObj["chatuid"] = uid;

    QString response;

    if (sendAndWait(response, jsonObj)) {
//        qDebug() << response;

        QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject jsonObj = resJsonDoc.object();
//        QJsonObject resJsonObj = jsonObj["contacts"].toObject();
        QJsonArray resJsonArr = jsonObj["contacts"].toArray();

        chatList = resJsonArr;

        foreach (const QJsonValue &contactValue, resJsonArr) {
            QJsonObject contactObj = contactValue.toObject();

            qDebug() << "消息id: " << contactObj["id"].toString();
            qDebug() << "消息发送人: " << contactObj["showname"].toString();
            qDebug() << "消息: " << contactObj["last_msgstr"].toString();
        }
        return true;
    }
    return false;
}

bool ClientCore::get_message(const QString &type_id) {
    QJsonObject jsonObj;
    jsonObj["task"] = "message_history";
    jsonObj["typeid"] = type_id;
    jsonObj["minid"] = 0;

    QString response;

    if (sendAndWait(response, jsonObj)) {
//        qDebug() << response;

        QJsonDocument resJsonDoc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject jsonObj = resJsonDoc.object();
//        QJsonObject resJsonObj = jsonObj["contacts"].toObject();
        QJsonArray resJsonArr = jsonObj["data"].toArray();
        user_id = jsonObj["typeid"].toString();
        msgList = resJsonArr;
        return true;
    }
    return false;
}

bool ClientCore::send_message(const QString &message) {
    QJsonObject jsonObj;
    jsonObj["task"] = "message_send";
    jsonObj["typeid"] = user_id;
    jsonObj["msgtxt"] = message;

    QString response;

    if (sendAndWait(response, jsonObj)) {
//        qDebug() << response;
    }

    return false;
}

void ClientCore::onReadyRead() {
    // 读取服务端的消息
    QString message = QString::fromUtf8(socket.readAll());

    qDebug() << "接收到的消息是" << message;

    emit readyMessage(message);

    return;

}

ClientCore::ClientCore() {
    connect(&socket, &QTcpSocket::readyRead, this, &ClientCore::onReadyRead);
}

QJsonObject ClientCore::baseJsonObj(const QString &type, const QString &state) {
    // 创建一个 JSON 对象
    QJsonObject jsonObj;
    jsonObj["type"] = type;
    jsonObj["state"] = state;

    // 创建一个嵌套的JSON数据对象
    QJsonObject dataObj;
    jsonObj["data"] = dataObj;

    return jsonObj;
}

void ClientCore::sendJsonObj(const QJsonObject &jsonObj) {
    QJsonDocument jsonDoc(jsonObj);
    QString message = jsonDoc.toJson(QJsonDocument::Compact);
    socket.write(message.toUtf8());
}

bool ClientCore::sendAndWait(QString &response, const QJsonObject &jsonObj) {
    QTimer timer;
    QEventLoop loop;
    bool readFlag = false;

    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(this, &ClientCore::readyMessage, &loop, [&](const QString &message){
        response = message;
//        qDebug() << "数据类型" << typeid (message).name();
        readFlag = true;
        loop.quit();
    });

    timer.start(10000);
    sendJsonObj(jsonObj);
    loop.exec();

    // 判断是否在规定时间内接收到数据
    return readFlag;
}

