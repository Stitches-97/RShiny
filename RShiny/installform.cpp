#include "installform.h"
#include "ui_installform.h"

InstallForm::InstallForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstallForm)
{
    ui->setupUi(this);
    setWindowTitle("Install packages");
    hide();
    m_newpkg_name = "";
    m_i = 0;
    m_j = 0;
    m_xmler = new Xmler();
    m_txter = new Txter();
    m_installer = new Installer();
    m_timer = new QTimer(this);
    ui->download_bar->hide();
    ui->expand_label->hide();
    ui->install_button->setEnabled(false);

    connect(m_timer, &QTimer::timeout, this, &InstallForm::on_update_expand_label);
    connect(m_xmler,SIGNAL(update_detail_text_signal(QString,QString,QString)),this,SLOT(on_update_detail_text(QString,QString,QString)));
    connect(m_installer,SIGNAL(update_bar_signal(QString)),this,SLOT(on_update_bar(QString)));
    connect(m_installer,&Installer::update_expand_signal,this,&InstallForm::on_update_expand);
    connect(m_installer,&Installer::finish_download_signal,this,&InstallForm::on_finish_download);
    connect(m_installer,SIGNAL(finish_expand_signal(QString)),this,SLOT(on_finish_expand(QString)));
}

InstallForm::~InstallForm()
{
    delete ui;
}

QString InstallForm::get_path()
{
    QString project_path = QCoreApplication::applicationDirPath();
    QString path_1 = project_path.mid(0,project_path.lastIndexOf("/"));
    QString path_2 = path_1.mid(0,path_1.lastIndexOf("/"));
    return path_2;
}

void InstallForm::on_get_button_released()
{
    ui->detail_text_edit->clear();
    QString xml_url = ui->url_line_edit->text();
    if(xml_url.isEmpty())
    {
        return;
    }
    QString xml_name = xml_url.mid(xml_url.lastIndexOf("/")+1,(xml_url.length() - xml_url.lastIndexOf("/") - 1));
    m_xmler->m_xml_name = xml_name;
    m_xmler->m_xml_url = xml_url;
    ui->get_button->setEnabled(false);
    m_xmler->download_xml();
}

void InstallForm::on_install_button_released()
{
    QString perferred = m_xmler->get_xml_map_element("app_install_perferred");
    QString app_name = m_newpkg_name;
    QString appver = perferred + "_windows_" + app_name;
    QString new_version = m_xmler->get_xml_map_element(appver);
    QStringList info = m_txter->read_line(app_name,true);
    if(!info.isEmpty())
    {
        QString old_version = info[1];
        if(old_version == new_version)
        {
            qDebug()<<"app: "<<app_name<<" already install";
            m_xmler->clear_xml_reader();
            return;
        }
    }
    QString source_tag_name = perferred + "_windows_url";
    QString r_package_resource = m_xmler->get_xml_map_element(source_tag_name);
    QString zip_name = r_package_resource.mid(r_package_resource.lastIndexOf("/")+1,(r_package_resource.length() - r_package_resource.lastIndexOf("/") - 1));
    m_installer->m_r_package_resource = r_package_resource;
    m_installer->m_zip_name = zip_name;
    ui->install_button->setEnabled(false);
    ui->get_button->setEnabled(false);
    ui->cancel_button->setEnabled(false);
    ui->download_bar->show();
    m_installer->download_zip();
}

void InstallForm::on_cancel_button_released()
{
    hide();
    ui->url_line_edit->clear();
    ui->detail_text_edit->clear();
    ui->install_button->setEnabled(false);
    ui->get_button->setEnabled(true);
    ui->download_bar->hide();
    ui->expand_label->hide();
    m_newpkg_name = "";
    m_installer->reset_args();
    m_xmler->reset_args();
    m_xmler->clear_xml_reader();
}

void InstallForm::on_update_bar(QString out)
{
    QRegExp regexp("\\d+");
    int pos = regexp.indexIn(out);
    if(pos != -1)
    {
        m_i++;
        if(m_i > 8)
        {
            m_i--;
            int value = regexp.cap(0).toInt();
            if((value >= 0) && (value <= 100))
            {
                if(ui->download_bar->value() < value)
                {
                    QString info = "update bar's value to " + QString::number(value);
                    qDebug()<<info;
                    ui->download_bar->setValue(value);
                    ui->download_bar->setFormat(tr("downloading : %1%").arg(QString::number(ui->download_bar->value())));
                }
            }
        }
    }
}

void InstallForm::on_update_expand()
{
    ui->expand_label->show();
    m_timer->start(1000);
}

void InstallForm::on_update_expand_label()
{
    QString text = "expanding";
    if(m_j > 3)
    {
        m_j = 0;
    }
    for(int i = 0; i < m_j ; i++)
    {
        text+=".";
    }
    m_j++;
    ui->expand_label->setText(text);
}

void InstallForm::on_finish_download()
{
    ui->download_bar->hide();
    ui->download_bar->reset();
}

void InstallForm::on_finish_expand(QString newpkg_r_home)
{
    m_j = 0;
    ui->expand_label->hide();
    m_timer->stop();
    ui->install_button->setEnabled(true);
    ui->get_button->setEnabled(true);
    ui->cancel_button->setEnabled(true);

    QString app_name = m_newpkg_name;
    QString perferred = m_xmler->get_xml_map_element("app_install_perferred");
    QString appver = perferred + "_windows_" + app_name;
    QString version = m_xmler->get_xml_map_element(appver);
    QString new_version = version;
    QString r_home = newpkg_r_home;
    QStringList info = m_txter->read_line(app_name,true);
    if(info.isEmpty())
    {
        m_txter->write_line(app_name,version,new_version,r_home);
    }
    else
    {
        m_txter->change_line(app_name,version,new_version,r_home);
    }
    emit newpkg_signal(m_newpkg_name);
}

void InstallForm::on_update_detail_text(QString package_name, QString package_title, QString package_details)
{
    m_newpkg_name = package_name;

    ui->detail_text_edit->append("Package name: ");
    ui->detail_text_edit->append(package_name);
    ui->detail_text_edit->append("");
    ui->detail_text_edit->append("Package title: ");
    ui->detail_text_edit->append(package_title);
    ui->detail_text_edit->append("");
    ui->detail_text_edit->append("Package details: ");
    ui->detail_text_edit->append(package_details);

    ui->get_button->setEnabled(true);
    ui->install_button->setEnabled(true);
}

void InstallForm::closeEvent(QCloseEvent *event)
{
    if(isVisible())
    {
        on_cancel_button_released();
    }
    event->ignore();
}
