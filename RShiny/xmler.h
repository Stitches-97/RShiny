#ifndef XMLER_H
#define XMLER_H

#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QMap>
#include <QtCore>
#include <QFile>
#include <QProcess>

class Xmler : public QObject
{
    Q_OBJECT
public:
    explicit Xmler(QObject *parent = 0);

    QString m_xml_url;
    QString m_xml_name;

    //download xml
    void download_xml();
    //get xml_map element by element_name
    QString get_xml_map_element(QString element_name);
    //clear xml map
    void clear_xml_reader();
    //reset args
    void reset_args();

signals:
    //update detail_text signal, take name,title,details
    void update_detail_text_signal(QString package_name, QString package_title, QString package_details);

public slots:
    //monitor the state of the QProcess
    void on_show_state(QProcess::ProcessState);
    //response the finish of download xml
    void on_show_finished_download_xml(int, QProcess::ExitStatus);
    //response the out of download xml
    void on_show_out();
    //response the error of download xml
    void on_show_error();

private:
    QMap<QString,QString> m_xml_map;

    //read xml by xml_name
    void read_xml(QString xml_name);
    //read xml_map
    void read_xml_map();
    //read xml by recursion
    void read_by_res(QDomNode child_node);
    //parse xml
    void parse_xml();
    //get the parent directory of the parent of the current program
    QString get_path();
};

#endif // XMLER_H
