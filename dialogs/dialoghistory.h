#ifndef DIALOGHISTORY_H
#define DIALOGHISTORY_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "dialogtool.h"
#include "../xml/vdomdocument.h"
#pragma GCC diagnostic pop

namespace Ui {
class DialogHistory;
}

class DialogHistory : public DialogTool
{
    Q_OBJECT
    
public:
    explicit DialogHistory(VContainer *data, VDomDocument *doc, QWidget *parent = 0);
    virtual ~DialogHistory();
public slots:
    virtual void DialogAccepted();
    void cellClicked(int row, int column);
    void ChangedCursor(qint64 id);
    void UpdateHistory();
signals:
    void ShowHistoryTool(qint64 id, Qt::GlobalColor color, bool enable);
protected:
    virtual void     closeEvent ( QCloseEvent * event );
private:
    Ui::DialogHistory *ui;
    VDomDocument *doc;
    qint32 cursorRow;
    qint32 cursorToolRecordRow;
    void FillTable();
    QString Record(const VToolRecord &tool);
    void InitialTable();
    void ShowPoint();
};

#endif // DIALOGHISTORY_H
