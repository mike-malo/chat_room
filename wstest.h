#ifndef WSTEST_H
#define WSTEST_H

#include <QWidget>
#include <QWebSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

class wstest : public QObject
{
    Q_OBJECT
public:
    wstest();

    bool sendMessage(QString msg);
    bool connectStatus;

public slots:
    void createDataRecvWS();

private:
    QWebSocket dataRecvWS;
    void reconnect();
    QTimer *m_timer;

private slots:
    void onConnected();
    void onTextReceived(QString msg);
    void onDisConnected();

};

#endif // WSTEST_H
