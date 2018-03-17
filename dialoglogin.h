#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>

namespace Ui {
class DialogLogIn;
}

class DialogLogIn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogIn(QWidget *parent = 0);
    ~DialogLogIn();

private:
    Ui::DialogLogIn *ui;
};

#endif // DIALOGLOGIN_H
