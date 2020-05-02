#include "runninglog.h"
#include "ui_runninglog.h"

RunningLog::RunningLog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunningLog)
{
    ui->setupUi(this);
    setWindowTitle("Running log");
    hide();
}

RunningLog::~RunningLog()
{
    delete ui;
}

void RunningLog::closeEvent(QCloseEvent *event)
{
    if(isVisible())
    {
        hide();
    }
    event->ignore();
}

void RunningLog::append_out(QString out)
{
    ui->log_text_edit->append(out);
}
