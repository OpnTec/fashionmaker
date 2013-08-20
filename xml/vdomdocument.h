#ifndef VDOMDOCUMENT_H
#define VDOMDOCUMENT_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QDomDocument>
#include <QMap>
#include <QObject>
#include <QComboBox>
#include "../container/vcontainer.h"
#include "../widgets/vmaingraphicsscene.h"
#include "../tools/vdatatool.h"
#pragma GCC diagnostic pop
#include "vtoolrecord.h"

namespace Document{
    enum Enum
    {
        LiteParse,
        FullParse
    };
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class VDomDocument : public QObject, public QDomDocument
{
    Q_OBJECT
public:
                VDomDocument(VContainer *data,QComboBox *comboBoxDraws);
                VDomDocument(const QString& name, VContainer *data,QComboBox *comboBoxDraws);
                VDomDocument(const QDomDocumentType& doctype, VContainer *data, QComboBox *comboBoxDraws);
                ~VDomDocument();
    QDomElement elementById(const QString& id);
    void        CreateEmptyFile();
    void        ChangeActivDraw(const QString& name, Document::Enum parse = Document::FullParse);
    QString     GetNameActivDraw() const;
    bool        GetActivDrawElement(QDomElement &element);
    bool        GetActivCalculationElement(QDomElement &element);
    bool        GetActivModelingElement(QDomElement &element);
    bool        GetActivPathsElement(QDomElement &element);
    bool        appendDraw(const QString& name);
    void        SetNameDraw(const QString& name);
    void        Parse(Document::Enum parse, VMainGraphicsScene *scene);
    QMap<qint64, VDataTool*>* getTools();
    QVector<VToolRecord> *getHistory();
    qint64 getCursor() const;
    void setCursor(const qint64 &value);
    void setCurrentData();
signals:
    void        ChangedActivDraw(const QString newName);
    void        ChangedNameDraw(const QString oldName, const QString newName);
    void        FullUpdateFromFile();
    void        haveChange();
    void        ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    void        ChangedCursor(qint64 id);
public slots:
    void        FullUpdateTree();
    void        haveLiteChange();
    void        ShowHistoryTool(qint64 id, Qt::GlobalColor color, bool enable);
private:
    QMap<QString, QDomElement> map;
    QString     nameActivDraw;
    VContainer  *data;
    QMap<qint64, VDataTool*> tools;
    QVector<VToolRecord> history;
    qint64 cursor;
    QComboBox *comboBoxDraws;
                VDomDocument(const VDomDocument & doc);
    const VDomDocument &operator=(const VDomDocument& doc);
    bool        find(QDomElement node, const QString& id);
    bool        CheckNameDraw(const QString& name) const;
    void        SetActivDraw(const QString& name);
    bool        GetActivNodeElement(const QString& name, QDomElement& element);
    void        ParseDrawElement(VMainGraphicsScene  *scene,
                                 const QDomNode& node, Document::Enum parse);
    void        ParseCalculationElement(VMainGraphicsScene  *scene, const QDomNode& node,
                                        Document::Enum parse);
    void        ParsePointElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                  Document::Enum parse, const QString &type);
    void        ParseLineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                 Document::Enum parse);
    void        ParseSplineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                 Document::Enum parse, const QString& type);
    void        ParseArcElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                 Document::Enum parse, const QString& type);
    void        ParseIncrementsElement(const QDomNode& node);
};

#pragma GCC diagnostic pop

#endif // VDOMDOCUMENT_H
