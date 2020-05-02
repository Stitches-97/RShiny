#ifndef RUNNINGLOG_H
#define RUNNINGLOG_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class RunningLog;
}

class RunningLog : public QWidget
{
    Q_OBJECT

public:
    explicit RunningLog(QWidget *parent = 0);
    ~RunningLog();

    void append_out(QString out);

private slots:

protected:
    //overloaded closeEvent function
    void closeEvent(QCloseEvent *event);

private:
    Ui::RunningLog *ui;
};

#endif // RUNNINGLOG_H
