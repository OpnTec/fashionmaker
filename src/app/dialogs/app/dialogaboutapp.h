#ifndef DIALOGABOUTAPP_H
#define DIALOGABOUTAPP_H

#include <QDialog>

namespace Ui {
class DialogAboutApp;
}

class DialogAboutApp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAboutApp(QWidget *parent = 0);
    ~DialogAboutApp();

private:
    Ui::DialogAboutApp *ui;
    Q_DISABLE_COPY(DialogAboutApp)

private slots:
    /**
     * @brief Fake button clicked
     */
    void webButtonClicked();
};

#endif // DIALOGABOUTAPP_H
