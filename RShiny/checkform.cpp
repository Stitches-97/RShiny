#include "checkform.h"
#include "ui_checkform.h"

#include <QDebug>

CheckForm::CheckForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckForm)
{
    ui->setupUi(this);
    setWindowTitle("Check for updates");
    hide();
    m_index = 0;
    ui->check_table->setColumnWidth(0,150);
    ui->check_table->setColumnWidth(1,100);
    ui->check_table->setColumnWidth(2,100);
    ui->check_table->setColumnWidth(3,231);
    ui->check_table->verticalHeader()->setVisible(false);
}

CheckForm::~CheckForm()
{
    delete ui;
}

void CheckForm::add_line(QString app_name, QString version, QString new_version)
{
    ui->check_table->setItem(m_index,0,new QTableWidgetItem(app_name));
    ui->check_table->item(m_index,0)->setTextAlignment(Qt::AlignCenter);
    ui->check_table->setItem(m_index,1,new QTableWidgetItem(version));
    ui->check_table->item(m_index,1)->setTextAlignment(Qt::AlignCenter);
    ui->check_table->setItem(m_index,2,new QTableWidgetItem(new_version));
    ui->check_table->item(m_index,2)->setTextAlignment(Qt::AlignCenter);

    QPushButton* update_button = new QPushButton("update");
    connect(update_button,SIGNAL(released()),this,SLOT(on_updates()));
    ui->check_table->setCellWidget(m_index,3,update_button);

    if(version != new_version)
    {
        update_button->setEnabled(true);
    }
    else
    {
        update_button->setEnabled(false);
    }
    m_index++;
}

void CheckForm::on_updates()
{
    qDebug()<<"start updates...";
}

void CheckForm::closeEvent(QCloseEvent *event)
{
    if(isVisible())
    {
        hide();
    }
    event->ignore();
}
