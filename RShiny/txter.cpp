#include "txter.h"

Txter::Txter(QObject *parent) : QObject(parent)
{

}

QStringList Txter::read_line(QString tag, bool flag)
{
    QString txt_path = get_path() + "/app.txt";
    QFile* txt = new QFile(txt_path);
    QStringList info;
    if(!txt->open(QFile::ReadWrite))
    {
        qDebug()<<"open txt failed!";
        return info;
    }
    while(!txt->atEnd())
    {
        QByteArray line = txt->readLine();
        QString str(line);
        str.replace("\r","").replace("\n","");
        QStringList info_test = str.split("\t");
        if(flag)
        {
            if(info_test[0] == tag)
            {
                info = info_test;
                break;
            }
        }
        else
        {
            if(info_test[3] == tag)
            {
                info = info_test;
                break;
            }
        }
    }
    txt->close();
    return info;
}

QStringList Txter::read_txt()
{
    QString txt_path = get_path() + "/app.txt";
    QFile* txt = new QFile(txt_path);
    QStringList result;
    if(!txt->open(QFile::ReadWrite))
    {
        qDebug()<<"open txt failed!";
        return result;
    }
    while(!txt->atEnd())
    {
        QByteArray line = txt->readLine();
        QString str(line);
        result.append(str);
    }
    txt->close();
    return result;
}

void Txter::write_line(QString app_name, QString version, QString new_version, QString r_home)
{
    QString txt_path = get_path() + "/app.txt";
    QFile* txt = new QFile(txt_path);
    if(!txt->open(QFile::ReadWrite | QFile::Append))
    {
        return;
    }
    QString info = app_name+"\t"+version+"\t"+new_version+"\t"+r_home+"\r\n";
    QByteArray line = info.toUtf8();
    txt->write(line);
    txt->close();
}

void Txter::change_line(QString app_name, QString version, QString new_version, QString r_home)
{
    QStringList txt_content = read_txt();
    QString str;
    QString txt_path = get_path() + "/app.txt";
    QFile* txt = new QFile(txt_path);
    if(!txt->open(QFile::ReadWrite | QFile::Truncate))
    {
        return;
    }
    for(int i=0;i<txt_content.length();i++)
    {
        str = txt_content[i];
        str.replace("\r","").replace("\n","");
        QStringList info = str.split("\t");
        if(info[0] == app_name)
        {
            str = app_name+"\t"+version+"\t"+new_version+"\t"+r_home+"\r\n";
            txt_content[i]=str;
        }
        QByteArray line = txt_content[i].toUtf8();
        txt->write(line);
    }
    txt->close();
}

QString Txter::get_path()
{
    QString project_path = QCoreApplication::applicationDirPath();
    QString path_1 = project_path.mid(0,project_path.lastIndexOf("/"));
    QString path_2 = path_1.mid(0,path_1.lastIndexOf("/"));
    return path_2;
}
