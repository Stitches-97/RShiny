#ifndef INSTALLFORM_H
#define INSTALLFORM_H

#include <QWidget>
#include <QProcess>
#include <QProgressBar>
#include <QCloseEvent>

#include <QApplication>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QUrl>
#include <QtNetwork>
#include <QTextCodec>
#include <QRegExp>
#include <QRegExpValidator>

#include "installer.h"
#include "xmler.h"
#include "txter.h"

namespace Ui {
class InstallForm;
}

class InstallForm : public QWidget
{
    Q_OBJECT

public:
    explicit InstallForm(QWidget *parent = 0);
    ~InstallForm();

    Xmler* m_xmler;        //download and parse xml

private slots:
    //three slots of buttons
    void on_get_button_released();
    void on_cancel_button_released();
    void on_install_button_released();
    //update download_bar by a QString
    void on_update_bar(QString out);
    //update expand progress
    void on_update_expand();
    //update expand_label
    void on_update_expand_label();
    //the treatment of finish download, include hide and reset download_bar
    void on_finish_download();
    //the treatment of finish expand
    void on_finish_expand(QString newpkg_r_home);
    //update detail_text, include name,title,details
    void on_update_detail_text(QString package_name, QString package_title, QString package_details);

signals:
    //new R package installed signal, send to RShinyMenu
    void newpkg_signal(QString newpkg_name);

protected:
    //overloaded closeEvent function
    void closeEvent(QCloseEvent *event);

private:
    Ui::InstallForm *ui;
    Txter* m_txter;             //write and read app.txt
    QTimer* m_timer;            //update expand_label by QTimer
    Installer* m_installer;     //download and install R package

    int m_i;                    //use of update download_bar
    int m_j;                    //use of update expand_label
    QString m_newpkg_name;      //the name of new R package

    //get the parent directory of the parent of the current program
    QString get_path();
};

#endif // INSTALLFORM_H
