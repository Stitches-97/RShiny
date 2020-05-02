#ifndef INSTALLER_H
#define INSTALLER_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>

class Installer : public QObject
{
    Q_OBJECT
public:
    explicit Installer(QObject *parent = 0);

    QString m_r_package_resource;   //resource of R package
    QString m_zip_name;             //name of R package zip
    QString m_newpkg_r_home;        //R_HOME of new R package

    //download R package zip
    void download_zip();
    //expand R package zip
    void expand_zip();
    //reset args
    void reset_args();

signals:
    //update bar signal, take a QString, send to installform
    void update_bar_signal(QString out);
    //update expand progress, send to installform
    void update_expand_signal();
    //finish download signal, send to installform
    void finish_download_signal();
    //finish expand signal, take R_HOME, send to installform
    void finish_expand_signal(QString newpkg_r_home);

private slots:
    //monitor the state of the QProcess
    void on_show_state(QProcess::ProcessState);
    //response the finish of download R package zip
    void on_show_finished_download(int, QProcess::ExitStatus);
    //response the finish of expand R package zip
    void on_show_finished_expand(int, QProcess::ExitStatus);
    //response the out of download R package zip
    void on_show_out();
    //response the error of download R package zip
    void on_show_error();

private:
    //get the parent directory of the parent of the current program
    QString get_path();
};

#endif // INSTALLER_H
