#ifndef CHATLIST_H
#define CHATLIST_H

#include <QWidget>
#include <QMouseEvent>
#include "ui_chatlist.h"
#include "clientcore.h"

namespace Ui {
    class ChatList;
}

class ChatList : public QWidget
{
    Q_OBJECT

public:
    explicit ChatList(QWidget *parent = nullptr);
    ~ChatList();

private slots:
    void refreshChatList();
    void on_closeButton_clicked();
    void on_sendButton_clicked();
    void onItemClicked(QListWidgetItem *item);

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    QPoint diff_pos;
    Ui::ChatList *ui;

    ClientCore *core;

    void AddItem(const QString& user_name, const QString& message, const QString& message_time, const QString& type_id);

    QStringList chatHistory;

};

#endif // CHATLIST_H
