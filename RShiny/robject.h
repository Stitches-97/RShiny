#ifndef ROBJECT_H
#define ROBJECT_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QDir>
#include "txter.h"

class RObject : public QObject
{
    Q_OBJECT

public:
    explicit RObject(QString app_name, QObject *parent = 0);

    qint64 m_process_id;    //R process id
    QString m_url;          //R url
    QString m_app_name;     //R name

    //start R
    void start();
    //start R process
    void start_process();
    //kill R process
    void kill_r_object();

private slots:
    //monitor the state of R process
    void on_show_state(QProcess::ProcessState);
    //response the finish of R process
    void on_show_finished(int, QProcess::ExitStatus);
    //response the out of R process
    void on_show_out();
    //response the error of R process
    void on_show_error();

signals:
    //open url signal, send to RShinyMenu
    void open_url_signal();
    //update running log signal
    void running_out_signal(QString out);

private:
    Txter* m_txter;         //write and read app.txt
    QProcess* m_process;    //R process

    //judge validity of url
    bool is_valid_url(QString url);
    //get R_HOME of RObject by app_name
    QString get_r_home();
};

#endif // ROBJECT_H
