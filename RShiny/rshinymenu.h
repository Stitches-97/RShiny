#ifndef RSHINYMENU_H
#define RSHINYMENU_H

#include <QWidget>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QProcess>

#include <QDesktopServices>
#include <QDesktopWidget>
#include <QUrl>
#include <QTextCodec>
#include <QRegExp>
#include <QRegExpValidator>

#include "installform.h"
#include "runninglog.h"
#include "checkform.h"
#include "infoform.h"
#include "licensingform.h"
#include "txter.h"
#include "robject.h"
#include "rmanager.h"

class RShinyMenu : public QObject
{
    Q_OBJECT

public:
    RShinyMenu(QObject *parent = 0);
    ~RShinyMenu();

private slots:
    void do_open_app();
    void do_install_packages();
    void do_running_log();
    void do_check_for_updates();
    void do_licensing();
    void do_information_center();
    void do_options();
    void do_about();
    void do_stop_control_UI();

    //open R url
    void on_open_url();
    //add a new R package and a new RObject
    void on_add_newpkg(QString newpkg_name);
    //update running log
    void on_update_running_log(QString out);

private:
    QSystemTrayIcon* m_tray_icon;
    QMenu* m_tray_menu;
    QMenu* m_open_menu;
    QAction* m_install_packages_action;     //show installform
    QAction* m_running_log_action;          //show runninglog
    QAction* m_check_for_updates_action;    //show checkform
    QAction* m_licensing_action;            //no use
    QAction* m_information_center_action;   //show infoform
    QAction* m_options_action;              //no use
    QAction* m_about_action;                //no use
    QAction* m_stop_control_UI_action;      //stop program

    InstallForm* m_installform;
    RunningLog* m_runninglog;
    CheckForm* m_checkform;
    InfoForm* m_infoform;
    LicensingForm* m_licensingform;
    Txter* m_txter;
    RManager* m_r_manager;

    //create tray menu
    void create_menu();
    //create tray icon
    void create_icon();
    //create connect
    void create_connect();
    //load R package by app.txt when program start
    void load_app();
    //add a new R package
    void add_newpkg(QString app_name, QString version, QString new_version);
    //add a new R package menu
    void add_newpkg_menu(QString app_name);
    //get the parent directory of the parent of the current program
    QString get_path();
};

#endif // RSHINYMENU_H
