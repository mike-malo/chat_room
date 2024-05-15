#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>

namespace Ui {
    class ItemWidget;
}

class ItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget(QWidget *parent = 0);
    ~ItemWidget();

    void SetData(const QString& user_name, const QString& message, const QString& message_time, const QString& type_id);

    QString getTypeId() const;

private:
    Ui::ItemWidget *ui;

    QString m_typeId;

};

#endif // ITEMWIDGET_H
