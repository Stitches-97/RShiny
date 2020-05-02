#ifndef TXTER_H
#define TXTER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QApplication>

class Txter : public QObject
{
    Q_OBJECT
public:
    explicit Txter(QObject *parent = 0);

    //read line by tag, if flag == true, tag = R package name, else, tag = R_HOME
    QStringList read_line(QString tag, bool flag);
    //read app.txt
    QStringList read_txt();
    //write line to app.txt
    void write_line(QString app_name, QString version, QString new_version, QString r_home);
    //change line to app.txt
    void change_line(QString app_name, QString version, QString new_version, QString r_home);

private:
    //get the parent directory of the parent of the current program
    QString get_path();
};

#endif // TXTER_H
