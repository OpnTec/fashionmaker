#ifndef DIALOGINCREMENTS_H
#define DIALOGINCREMENTS_H

#include "dialogtool.h"
#include <QPushButton>

#include "../container/vcontainer.h"
#include "../xml/vdomdocument.h"

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
    void AddIncrementToFile(quint64 id, QString name, qint32 base, qreal ksize, qreal kgrowth,
                            QString description);
};

#endif // DIALOGINCREMENTS_H
