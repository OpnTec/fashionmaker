/************************************************************************
 **
 **  @file   vabstractpattern.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef VABSTRACTPATTERN_H
#define VABSTRACTPATTERN_H

#include <qcompilerdetection.h>
#include <QHash>
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "vdomdocument.h"
#include "vtoolrecord.h"
#include "../vlayout/vtextmanager.h"

class QDomElement;
class VPiecePath;
class VPieceNode;

enum class Document : qint8 { FullLiteParse, LiteParse, LitePPParse, FullParse };
enum class LabelType : qint8 {NewPatternPiece, NewLabel};

// Don't touch values!!!. Same values stored in xml.
enum class CrossCirclesPoint : qint8 {FirstPoint = 1, SecondPoint = 2};
enum class VCrossCurvesPoint : qint8 {HighestPoint = 1, LowestPoint = 2};
enum class HCrossCurvesPoint : qint8 {LeftmostPoint = 1, RightmostPoint = 2};
enum class AxisType : qint8 {VerticalAxis = 1, HorizontalAxis = 2};

class VContainer;
class VDataTool;

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

struct VFormulaField
{
    QString     expression;
    QDomElement element;
    QString     attribute;
};

struct VFinalMeasurement
{
    QString name;
    QString formula;
    QString description;
};

QT_WARNING_POP

class VAbstractPattern : public VDomDocument
{
    Q_OBJECT
public:
    explicit VAbstractPattern(QObject *parent = nullptr);
    virtual ~VAbstractPattern();

    bool           RequiresMeasurements() const;
    QStringList    ListMeasurements() const;
    QVector<VFormulaField> ListExpressions() const;
    QVector<VFormulaField> ListIncrementExpressions() const;

    virtual void   CreateEmptyFile()=0;

    void           ChangeActivPP(const QString& name, const Document &parse = Document::FullParse);
    QString        GetNameActivPP() const;
    bool           CheckExistNamePP(const QString& name) const;
    int            CountPP() const;
    QDomElement    GetPPElement(const QString &name);
    bool           ChangeNamePP(const QString& oldName, const QString &newName);
    bool           appendPP(const QString& name);

    bool           GetActivNodeElement(const QString& name, QDomElement& element) const;

    quint32        getCursor() const;
    void           setCursor(const quint32 &value);

    virtual void   setXMLContent(const QString &fileName) override;

    virtual void   IncrementReferens(quint32 id) const=0;
    virtual void   DecrementReferens(quint32 id) const=0;

    virtual QString GenerateLabel(const LabelType &type, const QString &reservedName = QString())const=0;
    virtual QString GenerateSuffix() const=0;

    virtual void   UpdateToolData(const quint32 &id, VContainer *data)=0;

    virtual void Clear();

    static VDataTool* getTool(quint32 id);
    static void       AddTool(quint32 id, VDataTool *tool);
    static void       RemoveTool(quint32 id);

    static VPiecePath              ParsePieceNodes(const QDomElement &domElement);
    static QVector<CustomSARecord> ParsePieceCSARecords(const QDomElement &domElement);
    static QVector<quint32>        ParsePieceInternalPaths(const QDomElement &domElement);
    static QVector<quint32>        ParsePiecePointRecords(const QDomElement &domElement);

    void           AddToolOnRemove(VDataTool *tool);

    QVector<VToolRecord> *getHistory();
    QVector<VToolRecord> getLocalHistory() const;

    QString        MPath() const;
    void           SetMPath(const QString &path);

    quint32        SiblingNodeId(const quint32 &nodeId) const;

    QStringList    getPatternPieces() const;

    QMap<GHeights, bool> GetGradationHeights() const;
    void                 SetGradationHeights(const QMap<GHeights, bool> &options);

    QMap<GSizes, bool>   GetGradationSizes() const;
    void                 SetGradationSizes(const QMap<GSizes, bool> &options);

    QString        GetDescription() const;
    void           SetDescription(const QString &text);

    QString        GetNotes() const;
    void           SetNotes(const QString &text);

    QString        GetPatternName() const;
    void           SetPatternName(const QString& qsName);

    QString        GetCompanyName() const;
    void           SetCompanyName(const QString& qsName);

    QString        GetPatternNumber() const;
    void           SetPatternNumber(const QString &qsNum);

    QString        GetCustomerName() const;
    void           SetCustomerName(const QString& qsName);

    QString        GetLabelDateFormat() const;
    void           SetLabelDateFormat(const QString &format);

    QString        GetLabelTimeFormat() const;
    void           SetLabelTimeFormat(const QString &format);

    void                        SetPatternLabelTemplate(const QVector<VLabelTemplateLine> &lines);
    QVector<VLabelTemplateLine> GetPatternLabelTemplate() const;

    bool SetWatermarkPath(const QString &path);
    QString GetWatermarkPath() const;

    void               SetPatternMaterials(const QMap<int, QString> &materials);
    QMap<int, QString> GetPatternMaterials() const;

    QVector<VFinalMeasurement> GetFinalMeasurements() const;
    void                       SetFinalMeasurements(const QVector<VFinalMeasurement> &measurements);

    void SetPatternWasChanged(bool changed);
    bool GetPatternWasChanged() const;

    QString        GetImage() const;
    QString        GetImageExtension() const;
    void           SetImage(const QString &text, const QString &extension);
    void           DeleteImage();

    QString        GetVersion() const;
    void           SetVersion();

    bool           IsModified() const;
    void           SetModified(bool modified);

    QDomElement    GetDraw(const QString &name) const;

    void           ParseGroups(const QDomElement &domElement);
    QDomElement    CreateGroups();
    QDomElement    CreateGroup(quint32 id, const QString &name, const QMap<quint32, quint32> &groupData,
                            vidtype tool=null_id);
    vidtype        GroupLinkedToTool(vidtype toolId) const;
    QString        GetGroupName(quint32 id);
    void           SetGroupName(quint32 id, const QString &name);
    QMap<quint32, QPair<QString, bool> > GetGroups();
    QMap<quint32, QString> GetGroupsContainingItem(quint32 toolId, quint32 objectId, bool containItem);
    QDomElement           AddItemToGroup(quint32 toolId, quint32 objectId, quint32 groupId);
    QDomElement           RemoveItemFromGroup(quint32 toolId, quint32 objectId, quint32 groupId);
    bool           GroupIsEmpty(quint32 id);
    bool           GetGroupVisibility(quint32 id);

    QString PieceDrawName(quint32 id);

    static const QString TagPattern;
    static const QString TagCalculation;
    static const QString TagModeling;
    static const QString TagDetails;
    static const QString TagDetail;
    static const QString TagDescription;
    static const QString TagImage;
    static const QString TagNotes;
    static const QString TagMeasurements;
    static const QString TagIncrements;
    static const QString TagPreviewCalculations;
    static const QString TagIncrement;
    static const QString TagDraw;
    static const QString TagGroups;
    static const QString TagGroup;
    static const QString TagGroupItem;
    static const QString TagPoint;
    static const QString TagSpline;
    static const QString TagArc;
    static const QString TagElArc;
    static const QString TagTools;
    static const QString TagOperation;
    static const QString TagGradation;
    static const QString TagHeights;
    static const QString TagSizes;
    static const QString TagData;
    static const QString TagPatternInfo;
    static const QString TagPatternName;
    static const QString TagPatternNum;
    static const QString TagCompanyName;
    static const QString TagCustomerName;
    static const QString TagPatternLabel;
    static const QString TagWatermark;
    static const QString TagPatternMaterials;
    static const QString TagFinalMeasurements;
    static const QString TagMaterial;
    static const QString TagFMeasurement;
    static const QString TagGrainline;
    static const QString TagPath;
    static const QString TagNodes;
    static const QString TagNode;

    static const QString AttrName;
    static const QString AttrVisible;
    static const QString AttrObject;
    static const QString AttrTool;
    static const QString AttrType;
    static const QString AttrLetter;
    static const QString AttrAnnotation;
    static const QString AttrOrientation;
    static const QString AttrRotationWay;
    static const QString AttrTilt;
    static const QString AttrFoldPosition;
    static const QString AttrQuantity;
    static const QString AttrOnFold;
    static const QString AttrDateFormat;
    static const QString AttrTimeFormat;
    static const QString AttrArrows;
    static const QString AttrNodeReverse;
    static const QString AttrNodeExcluded;
    static const QString AttrNodePassmark;
    static const QString AttrNodePassmarkLine;
    static const QString AttrNodePassmarkAngle;
    static const QString AttrNodeShowSecondPassmark;
    static const QString AttrSABefore;
    static const QString AttrSAAfter;
    static const QString AttrStart;
    static const QString AttrPath;
    static const QString AttrEnd;
    static const QString AttrIncludeAs;
    static const QString AttrRotation;
    static const QString AttrNumber;
    static const QString AttrCheckUniqueness;
    static const QString AttrManualPassmarkLength;
    static const QString AttrPassmarkLength;
    static const QString AttrOpacity;

    static const QString AttrAll;

    static const QString AttrH50;
    static const QString AttrH56;
    static const QString AttrH62;
    static const QString AttrH68;
    static const QString AttrH74;
    static const QString AttrH80;
    static const QString AttrH86;
    static const QString AttrH92;
    static const QString AttrH98;
    static const QString AttrH104;
    static const QString AttrH110;
    static const QString AttrH116;
    static const QString AttrH122;
    static const QString AttrH128;
    static const QString AttrH134;
    static const QString AttrH140;
    static const QString AttrH146;
    static const QString AttrH152;
    static const QString AttrH158;
    static const QString AttrH164;
    static const QString AttrH170;
    static const QString AttrH176;
    static const QString AttrH182;
    static const QString AttrH188;
    static const QString AttrH194;
    static const QString AttrH200;

    static const QString AttrS22;
    static const QString AttrS24;
    static const QString AttrS26;
    static const QString AttrS28;
    static const QString AttrS30;
    static const QString AttrS32;
    static const QString AttrS34;
    static const QString AttrS36;
    static const QString AttrS38;
    static const QString AttrS40;
    static const QString AttrS42;
    static const QString AttrS44;
    static const QString AttrS46;
    static const QString AttrS48;
    static const QString AttrS50;
    static const QString AttrS52;
    static const QString AttrS54;
    static const QString AttrS56;
    static const QString AttrS58;
    static const QString AttrS60;
    static const QString AttrS62;
    static const QString AttrS64;
    static const QString AttrS66;
    static const QString AttrS68;
    static const QString AttrS70;
    static const QString AttrS72;

    static const QString AttrCustom;
    static const QString AttrDefHeight;
    static const QString AttrDefSize;
    static const QString AttrExtension;

    static const QString AttrFormula;
    static const QString AttrDescription;

    static const QString NodeArc;
    static const QString NodeElArc;
    static const QString NodePoint;
    static const QString NodeSpline;
    static const QString NodeSplinePath;

signals:
    /**
     * @brief ChangedActivPP change active pattern peace.
     * @param newName new pattern peace name.
     */
    void           ChangedActivPP(const QString &newName);

    /**
     * @brief ChangedCursor change cursor position.
     * @param id tool id.
     */
    void           ChangedCursor(quint32 id);

    /**
     * @brief ChangedNameDraw save new name pattern peace.
     * @param oldName old name.
     * @param newName new name.
     */
    void           ChangedNameDraw(const QString &oldName, const QString &newName);
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    void           FullUpdateFromFile();
    /**
     * @brief patternChanged emit if we have unsaved change.
     */
    void           patternChanged(bool saved);
    void           UpdatePatternLabel();
    /**
     * @brief ShowTool highlight tool.
     * @param id tool id.
     * @param enable enable or disable highlight.
     */
    void           ShowTool(quint32 id, bool enable);
    void           ClearMainWindow();
    void           UndoCommand();
    void           SetEnabledGUI(bool enabled);
    void           CheckLayout();
    void           UpdateInLayoutList();
    void           ShowDetail(quint32 id);
    void           SetCurrentPP(const QString &patterPiece);
    void           MadeProgress();
    /**
     * @brief UpdateGroups emit if the groups have been updated
     */
    void           UpdateGroups();
    void           UpdateToolTip();

public slots:
    virtual void   LiteParseTree(const Document &parse)=0;
    void           haveLiteChange();
    void           NeedFullParsing();
    void           ClearScene();
    void           CheckInLayoutList();
    void           SelectedDetail(quint32 id);
    void           UpdateVisiblityGroups();

protected:
    /** @brief nameActivDraw name current pattern peace. */
    QString        nameActivPP;

    /** @brief cursor cursor keep id tool after which we will add new tool in file. */
    quint32        cursor;

    QVector<VDataTool*> toolsOnRemove;

    /** @brief history history records. */
    QVector<VToolRecord> history;

    /** @brief patternPieces list of patern pieces names for combobox*/
    QStringList    patternPieces;

    /** @brief modified keep state of the document for cases that do not cover QUndoStack*/
    mutable bool   modified;

    /** @brief tools list with pointer on tools. */
    static QHash<quint32, VDataTool*> tools;
    /** @brief patternLabelLines list to speed up reading a template by many pieces. */
    static QVector<VLabelTemplateLine> patternLabelLines;
    /** @brief patternMaterials list to speed up reading materials by many pieces. */
    static QMap<int, QString> patternMaterials;
    static bool patternLabelWasChanged;

    static void       ToolExists(const quint32 &id);
    static VPiecePath ParsePathNodes(const QDomElement &domElement);
    static VPieceNode ParseSANode(const QDomElement &domElement);

    void           SetActivPP(const QString& name);

    QDomElement    CheckTagExists(const QString &tag);
    void           InsertTag(const QStringList &tags, const QDomElement &element);

    void           SetChildTag(const QString& qsParent, const QString& qsChild, const QString& qsValue);

    int  GetIndexActivPP() const;
    bool GetActivDrawElement(QDomElement &element) const;

    QVector<VToolRecord> getLocalHistory(const QString &draw) const;

   bool GroupHasItem(const QDomElement &groupDomElement, quint32 toolId, quint32 objectId);
private:
    Q_DISABLE_COPY(VAbstractPattern)

    QStringList ListIncrements() const;
    QVector<VFormulaField> ListPointExpressions() const;
    QVector<VFormulaField> ListArcExpressions() const;
    QVector<VFormulaField> ListElArcExpressions() const;
    QVector<VFormulaField> ListSplineExpressions() const;
    QVector<VFormulaField> ListPathPointExpressions() const;
    QVector<VFormulaField> ListOperationExpressions() const;
    QVector<VFormulaField> ListNodesExpressions(const QDomElement &nodes) const;
    QVector<VFormulaField> ListPathExpressions() const;
    QVector<VFormulaField> ListGrainlineExpressions(const QDomElement &element) const;
    QVector<VFormulaField> ListPieceExpressions() const;
    QVector<VFormulaField> ListFinalMeasurementsExpressions() const;

    bool IsVariable(const QString& token) const;
    bool IsFunction(const QString& token) const;

    QPair<bool, QMap<quint32, quint32> > ParseItemElement(const QDomElement &domElement);

    QMap<int, QString> GetMaterials(const QDomElement &element) const;
    void               SetMaterials(QDomElement &element, const QMap<int, QString> &materials);

    QVector<VFinalMeasurement> GetFMeasurements(const QDomElement &element) const;
    void                       SetFMeasurements(QDomElement &element, const QVector<VFinalMeasurement> &measurements);
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetNameActivPP return current pattern piece name.
 * @return pattern piece name.
 */
inline QString VAbstractPattern::GetNameActivPP() const
{
    return nameActivPP;
}

#endif // VABSTRACTPATTERN_H
