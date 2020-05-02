#include "xmler.h"

Xmler::Xmler(QObject *parent) : QObject(parent)
{
    reset_args();
}

QString Xmler::get_path()
{
    QString project_path = QCoreApplication::applicationDirPath();
    QString path_1 = project_path.mid(0,project_path.lastIndexOf("/"));
    QString path_2 = path_1.mid(0,path_1.lastIndexOf("/"));
    return path_2;
}

void Xmler::read_xml(QString xml_name)
{
    QFile* xml_file = new QFile(xml_name);
    if(!xml_file->open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"Error: open failed!";
        return;
    }

    QDomDocument doc;
    QString error_string;
    int error_line;
    int error_column;

    if(!doc.setContent(xml_file,&error_string,&error_line,&error_column))
    {
        xml_file->close();
        qDebug()<<"Error: doc setContent error at line "<<error_line<<", "
               <<"column "<<error_column<<": "<<error_string;
        return;
    }
    else
    {
        QDomElement root_element = doc.documentElement();
        QDomNode child_node = root_element.firstChild();
        while(!child_node.isNull())
        {
            if(child_node.isElement())
            {
                m_xml_map.insert(child_node.toElement().tagName(),child_node.toElement().text());
                if(child_node.toElement().hasAttributes())
                {
                    QDomNamedNodeMap node_map =  child_node.toElement().attributes();
                    for(int i =0 ; i < node_map.count(); i++)
                    {
                        QDomNode n = node_map.item(i);
                        QString n_name = child_node.parentNode().toElement().tagName() + "_" + child_node.toElement().tagName() + "_" + n.nodeName();
                        m_xml_map.insert(n_name,n.nodeValue());
                    }
                }
                read_by_res(child_node);
            }
            child_node = child_node.nextSibling();
        }
    }
    xml_file->close();
}

void Xmler::read_xml_map()
{
    QMap<QString,QString>::Iterator it = m_xml_map.begin();
    while(it != m_xml_map.end())
    {
        qDebug()<<"Key:    "<<it.key();
        qDebug()<<"Value:  "<<it.value()<<endl;
        it++;
    }
}

QString Xmler::get_xml_map_element(QString element_name)
{
    return m_xml_map[element_name];
}

void Xmler::read_by_res(QDomNode child_node)
{
    QDomNodeList node_list = child_node.childNodes();
    if(node_list.isEmpty())
    {
        return;
    }
    for (int i = 0; i< node_list.count();i++)
    {
        QDomNode node = node_list.at(i);
        if(node.isElement())
        {
            QString node_name = node.parentNode().parentNode().toElement().tagName() + "_" + node.parentNode().toElement().tagName() + "_" + node.toElement().tagName();
            m_xml_map.insert(node_name,node.toElement().text());
            if(node.toElement().hasAttributes())
            {
                QDomNamedNodeMap node_map = node.toElement().attributes();
                for(int i =0 ; i < node_map.count(); i++)
                {
                    QDomNode n = node_map.item(i);
                    QString n_name = node.parentNode().toElement().tagName() + "_" + node.toElement().tagName() + "_" + n.nodeName();
                    m_xml_map.insert(n_name,n.nodeValue());
                }
            }
            read_by_res(node);
        }
    }
}

void Xmler::clear_xml_reader()
{
    m_xml_map.clear();
}

void Xmler::reset_args()
{
    m_xml_url = "";
    m_xml_name = "";
}

void Xmler::download_xml()
{
    QProcess* process = new QProcess();
    connect(process,&QProcess::stateChanged,this,&Xmler::on_show_state);
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(on_show_finished_download_xml(int,QProcess::ExitStatus)));
    connect(process,&QProcess::readyReadStandardOutput,this,&Xmler::on_show_out);
    connect(process,&QProcess::readyReadStandardError,this,&Xmler::on_show_error);
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
    args.append(m_xml_url);
    args.append("--no-check-certificate");
    process->start(program,args);
}

void Xmler::parse_xml()
{
    m_xml_name = QDir::tempPath() + "/" + m_xml_name;
    read_xml(m_xml_name);
    QString package_name = get_xml_map_element("app_head_pkgname");
    QString package_title = get_xml_map_element("app_head_title");
    QString package_details = get_xml_map_element("app_head_desicription");
    emit update_detail_text_signal(package_name,package_title,package_details);
}

void Xmler::on_show_state(QProcess::ProcessState state)
{
    qDebug() << "show_state: ";
    if(state == QProcess::NotRunning)
    {
        qDebug()<<"Not Running";
    }
    else if(state == QProcess::Starting)
    {
        qDebug()<<"Starting";
    }
    else
    {
        qDebug()<<"Running";
    }
}

void Xmler::on_show_finished_download_xml(int exit_code, QProcess::ExitStatus exit_status)
{
    qDebug() << "show_finished_download_xml: " << endl << exit_code << exit_status;
    if(exit_code == 0)
    {
        qDebug() << QString::fromLocal8Bit("xml download successful, start parse xml...");
        parse_xml();
    }
}

void Xmler::on_show_out()
{
    QProcess* process = (QProcess *)sender();
    QString out = QString::fromLocal8Bit(process->readAllStandardOutput().data());
    qDebug() << out;
}

void Xmler::on_show_error()
{
    QProcess* process = (QProcess *)sender();
    QString error = QString::fromLocal8Bit(process->readAllStandardError().data());
    qDebug() << error;
}
