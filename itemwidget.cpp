#include "itemwidget.h"
#include "ui_itemwidget.h"

ItemWidget::ItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemWidget)
{
    ui->setupUi(this);
}

ItemWidget::~ItemWidget()
{
    delete ui;
}

void ItemWidget::SetData(const QString& user_name, const QString& message, const QString& message_time, const QString& type_id)
{
    ui->user_name->setText(user_name);
    ui->message->setText(message);
    ui->message_time->setText(message_time);
    m_typeId = type_id;
}

QString ItemWidget::getTypeId() const {
    return m_typeId;
}
