#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QEventLoop>
#include <QJsonArray>
#include <QTimer>

class Chat;

class ClientCore : public QObject
{
    Q_OBJECT

public:
    static ClientCore& getInstance() {
        static ClientCore instance; // 单例对象
        return instance;
    }

    bool connectServer();
    bool loginRequest(const QString &accout, const QString &password, QString &errorMsg);
    bool getChatListRequest(const QString &uid);

    bool get_message(const QString &type_id);
    bool send_message(const QString &message);

signals:
    void readyMessage(const QString &message);

private slots:
    void onReadyRead();

private:
    ClientCore(); // 私有构造函数，确保单例

    QJsonObject baseJsonObj(const QString &type, const QString &state);

    void sendJsonObj(const QJsonObject &jsonObj);

    bool sendAndWait(QString &response, const QJsonObject &jsonObj);

public:
    QTcpSocket socket;

    QString currentUserName; //当前登录的用户
    QString uid; //当前登录的uid
    QString account;

    QJsonArray chatList;
    QJsonArray msgList;
    QString type_id;
    QString user_id;
};


#endif // CLIENTCORE_H
