#ifndef LICENSINGFORM_H
#define LICENSINGFORM_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class LicensingForm;
}

class LicensingForm : public QWidget
{
    Q_OBJECT

public:
    explicit LicensingForm(QWidget *parent = 0);
    ~LicensingForm();

protected:
    //overloaded closeEvent function
    void closeEvent(QCloseEvent *event);

private:
    Ui::LicensingForm *ui;
};

#endif // LICENSINGFORM_H
