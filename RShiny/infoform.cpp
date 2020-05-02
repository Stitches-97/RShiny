#include "infoform.h"
#include "ui_infoform.h"

InfoForm::InfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoForm)
{
    ui->setupUi(this);
    setWindowTitle("Information Center");
    hide();
}

InfoForm::~InfoForm()
{
    delete ui;
}

void InfoForm::closeEvent(QCloseEvent *event)
{
    if(isVisible())
    {
        hide();
    }
    event->ignore();
}
