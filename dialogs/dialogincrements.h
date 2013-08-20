#ifndef DIALOGINCREMENTS_H
#define DIALOGINCREMENTS_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "dialogtool.h"
#include <QPushButton>
#include "../xml/vdomdocument.h"
#pragma GCC diagnostic pop
#include "../container/vcontainer.h"


namespace Ui {
class DialogIncrements;
}

class DialogIncrements : public DialogTool
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
    virtual void DialogAccepted();
signals:
    void FullUpdateTree();
    void haveLiteChange();
private:
    Ui::DialogIncrements *ui;
    VContainer *data; // need because we must change data
    VDomDocument *doc;
    void InitialStandartTable();
    void InitialIncrementTable();
    void InitialLinesTable();
    void InitialSplinesTable();
    void InitialArcsTable();
    void FillStandartTable();
    void FillIncrementTable();
    void FillLengthLines();
    void FillLengthSplines();
    void FillLengthArcs();
    void AddIncrementToFile(qint64 id, QString name, qint32 base, qreal ksize, qreal kgrowth,
                            QString description);
    DialogIncrements(const DialogIncrements &dialog);
    const DialogIncrements &operator=(const DialogIncrements& dialog);
};

#endif // DIALOGINCREMENTS_H
