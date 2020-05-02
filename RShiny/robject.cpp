#include "robject.h"

RObject::RObject(QString app_name, QObject *parent) : QObject(parent)
{
    m_process_id = 0;
    m_url = "";
    m_app_name = app_name;
    m_txter = new Txter();
    m_process = new QProcess();
    connect(m_process,&QProcess::stateChanged,this,&RObject::on_show_state);
    connect(m_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(on_show_finished(int,QProcess::ExitStatus)));
    connect(m_process,&QProcess::readyReadStandardOutput,this,&RObject::on_show_out);
    connect(m_process,&QProcess::readyReadStandardError,this,&RObject::on_show_error);
}

void RObject::start()
{
    if(m_process_id <= 0)
    {
        start_process();
    }
    else
    {
        if(m_url != "")
        {
            emit open_url_signal();
        }
    }
}

void RObject::start_process()
{
    QString r_home = get_r_home();
    QString program;
    Q_OS_WIN32
#ifdef Q_OS_WIN32
    program = r_home + "/i386/Rscript.exe";
#elif Q_OS_WIN64
    program = r_home + "/x64/Rscript.exe"
#else
    program = r_home + "/Rscript";
#endif

    QStringList args_environment;
    args_environment.append("R_HOME=" + r_home);
    args_environment.append("TMPDIR=" + QDir::tempPath());
    m_process->setEnvironment(args_environment);
    QStringList args;
    args.append("-e");
    args.append("library(idem);imShiny();");
    m_process->start(program,args);
    m_process->waitForStarted();
}

bool RObject::is_valid_url(QString url)
{
    if(url.length() <= 17)
    {
        return false;
    }
    if(url.mid(0,17) == "http://127.0.0.1:")
    {
        url = url.mid(17,url.length()-17);
        if(url.toInt() > 0)
        {
            return true;
        }
    }
    return false;
}

void RObject::kill_r_object()
{
    m_process->kill();
    m_process->waitForFinished();
}

QString RObject::get_r_home()
{
    QStringList info = m_txter->read_line(m_app_name,true);
    QString r_home = info[3];
    return r_home;
}

void RObject::on_show_state(QProcess::ProcessState state)
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
    emit running_out_signal(out);
}

void RObject::on_show_finished(int exit_code, QProcess::ExitStatus exit_status)
{
    QString out = "show_finished: \n" + QString::number(exit_code) + "\n";
    out += "R program has ended !";
    qDebug()<<out;
    m_process_id = 0;
    emit running_out_signal(out);
}

void RObject::on_show_out()
{
    QString out = QString::fromLocal8Bit(m_process->readAllStandardOutput().data());
    qDebug() << out;
    emit running_out_signal(out);
}

void RObject::on_show_error()
{
    QString error = QString::fromLocal8Bit(m_process->readAllStandardError().data());
    QString out = error;
    qDebug() << out;
    emit running_out_signal(out);
    if(error.length() <= 15)
    {
        return;
    }
    QString url = error.mid(15,error.length()-15);
    url = url.replace("\r","").replace("\n","");
    if(is_valid_url(url))
    {
        m_process_id = m_process->processId();
        m_url = url;
        emit open_url_signal();
    }
}
