#ifndef VDOMDOCUMENT_H
#define VDOMDOCUMENT_H

#pragma GCC diagnostic ignored "-Weffc++"
#include <QDomDocument>
#include <QMap>
#include <QObject>
#include <QComboBox>
#include "../container/vcontainer.h"
#include "../widgets/vmaingraphicsscene.h"
#include "../tools/vdatatool.h"
#pragma GCC diagnostic warning "-Weffc++"

namespace Document{
    enum Enum
    {
        LiteParse,
        FullParse
    };
}

class VDomDocument : public QObject, public QDomDocument
{
    Q_OBJECT
public:
                VDomDocument(VContainer *data);
                VDomDocument(const QString& name, VContainer *data);
                VDomDocument(const QDomDocumentType& doctype, VContainer *data);
                ~VDomDocument();
    QDomElement elementById(const QString& id);
    void        CreateEmptyFile();
    void        ChangeActivDraw(const QString& name);
    QString     GetNameActivDraw() const;
    bool        GetActivDrawElement(QDomElement &element);
    bool        GetActivCalculationElement(QDomElement &element);
    bool        GetActivModelingElement(QDomElement &element);
    bool        GetActivPathsElement(QDomElement &element);
    bool        appendDraw(const QString& name);
    void        SetNameDraw(const QString& name);
    void        Parse(Document::Enum parse, VMainGraphicsScene *scene, QComboBox *comboBoxDraws);
    QMap<qint64, VDataTool*>* getTools();
signals:
    void        ChangedActivDraw(const QString newName);
    void        ChangedNameDraw(const QString oldName, const QString newName);
    void        FullUpdateFromFile();
    void        haveChange();
public slots:
    void        FullUpdateTree();
    void        haveLiteChange();
private:
    QMap<QString, QDomElement> map;
    QString     nameActivDraw;
    VContainer  *data;
    QMap<qint64, VDataTool*> tools;
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
    void        AddNewDraw(const QDomElement &node, QComboBox *comboBoxDraws)const;
};

#endif // VDOMDOCUMENT_H
