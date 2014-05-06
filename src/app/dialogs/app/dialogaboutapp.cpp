#include "dialogaboutapp.h"
#include "ui_dialogaboutapp.h"
#include "../../version.h"

DialogAboutApp::DialogAboutApp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutApp)
{
    ui->setupUi(this);

    ui->label_Valentina_Version->setText(QString("Valentina %1").arg(APP_VERSION));
    ui->label_QT_Version->setText(tr("Based on Qt %2 (32 bit)").arg(QT_VERSION_STR));

    QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
    ui->label_Valentina_Built->setText(tr("Built on %3 at %4").arg(date.toString()).arg(__TIME__));

    ui->label_Legal_Stuff->setText(WARRANTY);

    ui->pushButton_Web_Site->setText(tr("Web site : %1").arg(VER_COMPANYDOMAIN_STR));
    connect(ui->pushButton_Web_Site, &QPushButton::clicked,
            this, &DialogAboutApp::webButtonClicked );

}

DialogAboutApp::~DialogAboutApp()
{
    delete ui;
}

void DialogAboutApp::webButtonClicked() {
    if ( ! QDesktopServices::openUrl(QUrl(VER_COMPANYDOMAIN_STR))) {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Cannot open your default browser"));
    }

}
