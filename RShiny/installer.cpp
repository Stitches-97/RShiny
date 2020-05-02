#include "installer.h"

Installer::Installer(QObject *parent) : QObject(parent)
{
    reset_args();
}

void Installer::download_zip()
{
    QProcess* process = new QProcess();
    connect(process,&QProcess::stateChanged,this,&Installer::on_show_state);
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(on_show_finished_download(int,QProcess::ExitStatus)));
    connect(process,&QProcess::readyReadStandardOutput,this,&Installer::on_show_out);
    connect(process,&QProcess::readyReadStandardError,this,&Installer::on_show_error);
    if(m_r_package_resource.isEmpty())
    {
        return;
    }
    QString download_path = QDir::tempPath();
    QString path = get_path();
    QString program;
#ifdef Q_OS_WIN
    program = path + "/wget-1.20.3-win64/wget.exe";
#else
    program = "wget";
#endif
    QStringList args;
    args.append("-P");
    args.append(download_path);
    args.append(m_r_package_resource);
    args.append("--no-check-certificate");
    process->start(program,args);
}

void Installer::expand_zip()
{
    QProcess* process = new QProcess();
    connect(process,&QProcess::stateChanged,this,&Installer::on_show_state);
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(on_show_finished_expand(int,QProcess::ExitStatus)));
    QString download_path = QDir::tempPath() + "/" + m_zip_name;
    QString expand_path = QDir::homePath() + "/" + m_zip_name.mid(0,m_zip_name.lastIndexOf("."));
    m_newpkg_r_home = expand_path + "/R-Portable/bin";

    QString program;
    QStringList args;
#ifdef Q_OS_WIN
    program = "powershell.exe";
    args.append("Expand-Archive");
    args.append("-Path");
    args.append(download_path);
    args.append("-DestinationPath");
    args.append(expand_path);
#else
    program = "unzip";
    args.append("-d");
    args.append(expandPath);
    args.append(downloadPath);
#endif
    process->start(program,args);
    emit update_expand_signal();
}

QString Installer::get_path()
{
    QString project_path = QCoreApplication::applicationDirPath();
    QString path_1 = project_path.mid(0,project_path.lastIndexOf("/"));
    QString path_2 = path_1.mid(0,path_1.lastIndexOf("/"));
    return path_2;
}

void Installer::reset_args()
{
    m_r_package_resource = "";
    m_zip_name = "";
    m_newpkg_r_home = "";
}

void Installer::on_show_state(QProcess::ProcessState state)
{
    QString out = "show_state: ";
    if(state == QProcess::NotRunning)
    {
        out += "Not Running";
    }
    else if(state == QProcess::Starting)
    {
        out += "Starting";
    }
    else
    {
        out += "Running";
    }
    qDebug()<<out;
}

void Installer::on_show_finished_download(int exit_code, QProcess::ExitStatus exit_status)
{
    QString out = "show_finished_download: \n" + QString::number(exit_code) + "\n";
    if(exit_code == 0)
    {
        emit finish_download_signal();
        out += "R package download successful, start installation...";
        expand_zip();
        qDebug()<<out;
    }
}

void Installer::on_show_finished_expand(int exit_code, QProcess::ExitStatus exit_status)
{
    QString out = "show_finished_expand: \n" + QString::number(exit_code) + "\n";
    if(exit_code == 0)
    {
        emit finish_expand_signal(m_newpkg_r_home);
        out += "R package install successful";
        qDebug()<<out;
    }
}

void Installer::on_show_out()
{
    QProcess* process = (QProcess *)sender();
    QString out = QString::fromLocal8Bit(process->readAllStandardOutput().data());
    out = "show_out: " + out;
    qDebug()<<out;
}

void Installer::on_show_error()
{
    QProcess* process = (QProcess *)sender();
    QString error = QString::fromLocal8Bit(process->readAllStandardError().data());
    QString out = "show_error: " + error;
    qDebug()<<out;
    emit update_bar_signal(error);
}
