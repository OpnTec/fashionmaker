#ifndef DIALOGINCREMENTS_H
#define DIALOGINCREMENTS_H

#include <QDialog>
#include <QPushButton>

#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"

namespace Ui {
class DialogIncrements;
}

class DialogIncrements : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogIncrements(VContainer *data, VDomDocument *doc, QWidget *parent = 0);
    ~DialogIncrements();
public slots:
    void clickedToolButtonAdd();
    void clickedToolButtonRemove();
    void cellChanged ( qint32 row, qint32 column );
    void FullUpdateFromFile();
    void clickedButtonOk();
signals:
    void FullUpdateTree();
    void haveLiteChange();
    void closedActionTable();
protected:
    void closeEvent ( QCloseEvent * event );
private:
    Ui::DialogIncrements *ui;
    VContainer *data;
    VDomDocument *doc;
    void InitialStandartTable();
    void InitialIncrementTable();
    void InitialLinesTable();
    void FillStandartTable();
    void FillIncrementTable();
    void FillLengthLines();
    void AddIncrementToFile(quint64 id, QString name, qint32 base, qreal ksize, qreal kgrowth,
                            QString description);
};

#endif // DIALOGINCREMENTS_H
