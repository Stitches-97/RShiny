#ifndef INFOFORM_H
#define INFOFORM_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class InfoForm;
}

class InfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit InfoForm(QWidget *parent = 0);
    ~InfoForm();

private slots:

protected:
    //overloaded closeEvent function
    void closeEvent(QCloseEvent *event);

private:
    Ui::InfoForm *ui;
};

#endif // INFOFORM_H
