#ifndef DIALOGEDITLABEL_H
#define DIALOGEDITLABEL_H

#include <QDialog>

namespace Ui {
class DialogEditLabel;
}

class DialogEditLabel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditLabel(QWidget *parent = 0);
    ~DialogEditLabel();

private:
    Ui::DialogEditLabel *ui;
};

#endif // DIALOGEDITLABEL_H
