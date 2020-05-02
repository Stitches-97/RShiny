#include "licensingform.h"
#include "ui_licensingform.h"

LicensingForm::LicensingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LicensingForm)
{
    ui->setupUi(this);
    setWindowTitle("Licensing");
    hide();
}

LicensingForm::~LicensingForm()
{
    delete ui;
}

void LicensingForm::closeEvent(QCloseEvent *event)
{
    if(isVisible())
    {
        hide();
    }
    event->ignore();
}
