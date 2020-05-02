#ifndef CHECKFORM_H
#define CHECKFORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QPushButton>

namespace Ui {
class CheckForm;
}

class CheckForm : public QWidget
{
    Q_OBJECT

public:
    explicit CheckForm(QWidget *parent = 0);
    ~CheckForm();

    //add a line of info, including app_name,version,new_version
    void add_line(QString app_name, QString version, QString new_version);

private slots:
    //update old app
    void on_updates();

protected:
    //overloaded closeEvent function
    void closeEvent(QCloseEvent *event);

private:
    Ui::CheckForm *ui;
    int m_index;    //the number of apps in the list
};

#endif // CHECKFORM_H
