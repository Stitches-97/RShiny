#include "rshinymenu.h"

#include <QFile>

RShinyMenu::RShinyMenu(QObject *parent)
    :QObject(parent)
{
    create_menu();
    if(!QSystemTrayIcon::isSystemTrayAvailable())
    {
        return;
    }
    create_icon();
    create_connect();
    load_app();
}

RShinyMenu::~RShinyMenu()
{
    do_stop_control_UI();
}

void RShinyMenu::create_menu()
{
    m_install_packages_action = new QAction("Install packages...");
    m_running_log_action = new QAction("Running log...");
    m_check_for_updates_action = new QAction("Check for updates...");
    m_licensing_action = new QAction("Licensing...");
    m_information_center_action = new QAction("Information Center...");
    m_options_action = new QAction("Options...");
    m_about_action = new QAction("About...");
    m_stop_control_UI_action = new QAction("Stop Control UI");

    m_tray_menu = new QMenu((QWidget*)QApplication::desktop());
    m_open_menu = new QMenu("Open(start browser)");

    m_installform = new InstallForm();
    m_runninglog = new RunningLog();
    m_checkform = new CheckForm();
    m_infoform = new InfoForm();
    m_licensingform = new LicensingForm();
    m_txter = new Txter();
    m_r_manager = new RManager();

    m_tray_menu->addMenu(m_open_menu);

    m_tray_menu->addSeparator();

    m_tray_menu->addAction(m_install_packages_action);
    m_tray_menu->addAction(m_running_log_action);
    m_tray_menu->addAction(m_check_for_updates_action);

    m_tray_menu->addSeparator();

    m_tray_menu->addAction(m_licensing_action);
    m_tray_menu->addAction(m_information_center_action);
    m_tray_menu->addAction(m_options_action);
    m_tray_menu->addAction(m_about_action);

    m_tray_menu->addSeparator();

    m_tray_menu->addAction(m_stop_control_UI_action);

    m_options_action->setEnabled(false);
    m_about_action->setEnabled(false);
}

void RShinyMenu::create_icon()
{
    m_tray_icon = new QSystemTrayIcon();
    m_tray_icon->setIcon(QIcon(":/image/logo.jpg"));
    m_tray_icon->setToolTip(QString::fromLocal8Bit("RShiny"));
    m_tray_icon->showMessage("tray","tray manager",QSystemTrayIcon::Information,10000);
    m_tray_icon->setContextMenu(m_tray_menu);
    m_tray_icon->show();
}

void RShinyMenu::create_connect()
{
    connect(m_installform,SIGNAL(newpkg_signal(QString)),this,SLOT(on_add_newpkg(QString)));
    connect(m_install_packages_action,SIGNAL(triggered(bool)),this,SLOT(do_install_packages()));
    connect(m_running_log_action,SIGNAL(triggered(bool)),this,SLOT(do_running_log()));
    connect(m_check_for_updates_action,SIGNAL(triggered(bool)),this,SLOT(do_check_for_updates()));
    connect(m_licensing_action,SIGNAL(triggered(bool)),this,SLOT(do_licensing()));
    connect(m_information_center_action,SIGNAL(triggered(bool)),this,SLOT(do_information_center()));
    connect(m_options_action,SIGNAL(triggered(bool)),this,SLOT(do_options()));
    connect(m_about_action,SIGNAL(triggered(bool)),this,SLOT(do_about()));
    connect(m_stop_control_UI_action,SIGNAL(triggered(bool)),this,SLOT(do_stop_control_UI()));
}

void RShinyMenu::load_app()
{
    QStringList txt_content = m_txter->read_txt();
    QString str;
    for(int i=0;i<txt_content.length();i++)
    {
        str = txt_content[i];
        str.replace("\r","").replace("\n","");
        QStringList info = str.split("\t");
        QString app_name = info[i];
        RObject* r_object = new RObject(app_name);
        connect(r_object,&RObject::open_url_signal,this,&RShinyMenu::on_open_url);
        connect(r_object,SIGNAL(running_out_signal(QString)),this,SLOT(on_update_running_log(QString)));
        m_r_manager->add_r(app_name,r_object);
        add_newpkg(info[0],info[1],info[2]);
    }
}

void RShinyMenu::add_newpkg(QString app_name, QString version, QString new_version)
{
    add_newpkg_menu(app_name);
    m_checkform->add_line(app_name,version,new_version);
}

void RShinyMenu::add_newpkg_menu(QString app_name)
{
    QAction* app_action = new QAction(app_name,m_open_menu);
    connect(app_action,SIGNAL(triggered(bool)),this,SLOT(do_open_app()));
    m_open_menu->addAction(app_action);
}

QString RShinyMenu::get_path()
{
    QString project_path = QCoreApplication::applicationDirPath();
    QString path_1 = project_path.mid(0,project_path.lastIndexOf("/"));
    QString path_2 = path_1.mid(0,path_1.lastIndexOf("/"));
    return path_2;
}

void RShinyMenu::do_open_app()
{
    QAction* app_action = (QAction*)sender();
    QString app_name = app_action->text();
    RObject* r_object = m_r_manager->find(app_name);
    r_object->start();
}

void RShinyMenu::do_install_packages()
{
    m_installform->show();
}

void RShinyMenu::do_running_log()
{
    m_runninglog->show();
}

void RShinyMenu::do_check_for_updates()
{
    m_checkform->show();
}

void RShinyMenu::do_licensing()
{
    m_licensingform->show();
}

void RShinyMenu::do_information_center()
{
    m_infoform->show();
}

void RShinyMenu::do_options()
{

}

void RShinyMenu::do_about()
{
    //https://wzhy2000.mynetgear.com/shares/USB_Storage/idem-app.xml
}

void RShinyMenu::do_stop_control_UI()
{
    m_r_manager->kill_all_r_object();
    qApp->quit();
}

void RShinyMenu::on_open_url()
{
    RObject* r_object = (RObject*)sender();
    QString url = r_object->m_url;
    QString out = "R App URL: " + url;
    qDebug()<<out;
    QDesktopServices::openUrl(QUrl(url));
}

void RShinyMenu::on_add_newpkg(QString newpkg_name)
{
    QString app_name = newpkg_name;
    QStringList info = m_txter->read_line(app_name,true);
    add_newpkg(info[0],info[1],info[2]);
    RObject* r_object = new RObject(app_name);
    connect(r_object,&RObject::open_url_signal,this,&RShinyMenu::on_open_url);
    m_r_manager->add_r(app_name,r_object);
    m_installform->m_xmler->clear_xml_reader();
}

void RShinyMenu::on_update_running_log(QString out)
{
    m_runninglog->append_out(out);
}
