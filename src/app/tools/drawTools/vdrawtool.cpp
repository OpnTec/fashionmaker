/************************************************************************
 **
 **  @file   vdrawtool.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
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

#include "vdrawtool.h"

#include <qmuparsererror.h>
#include "../dialogs/tools/dialogeditwrongformula.h"
#include "../dialogs/app/dialogundo.h"
#include "../container/calculator.h"
#include "../../undocommands/addtocalc.h"
#include "../../undocommands/savetooloptions.h"
#include "../../libs/ifc/exception/vexceptionundo.h"

qreal VDrawTool::factor = 1;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDrawTool constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 */
VDrawTool::VDrawTool(VPattern *doc, VContainer *data, quint32 id, QObject *parent)
    :VAbstractTool(doc, data, id, parent), ignoreFullUpdate(false), nameActivDraw(doc->GetNameActivPP()),
      dialog(nullptr), typeLine(TypeLineLine), lineColor(ColorBlack), enabled(true)
{
    connect(this->doc, &VPattern::ChangedActivPP, this, &VDrawTool::ChangedActivDraw);
    connect(this->doc, &VPattern::ChangedNameDraw, this, &VDrawTool::ChangedNameDraw);
    connect(this->doc, &VPattern::ShowTool, this, &VDrawTool::ShowTool);
}

//---------------------------------------------------------------------------------------------------------------------
VDrawTool::~VDrawTool()
{
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool  highlight tool.
 * @param id object id in container.
 * @param color highlight color.
 * @param enable enable or disable highlight.
 */
void VDrawTool::ShowTool(quint32 id, Qt::GlobalColor color, bool enable)
{
    Q_UNUSED(id);
    Q_UNUSED(color);
    Q_UNUSED(enable);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedNameDraw save new name active pattern peace.
 * @param oldName old name.
 * @param newName new name active pattern peace. new name.
 */
void VDrawTool::ChangedNameDraw(const QString &oldName, const QString &newName)
{
    if (nameActivDraw == oldName)
    {
        nameActivDraw = newName;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromGuiOk refresh tool data after change in options.
 * @param result keep result working dialog.
 */
void VDrawTool::FullUpdateFromGuiOk(int result)
{
    if (result == QDialog::Accepted)
    {
        SaveDialogChange();
    }
    delete dialog;
    dialog=nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromGuiApply refresh tool data after change in options but do not delete dialog
 */
void VDrawTool::FullUpdateFromGuiApply()
{
    SaveDialogChange();
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::SaveDialogChange()
{
    qCDebug(vTool)<<"Saving tool options after using dialog";
    QDomElement oldDomElement = doc->elementById(QString().setNum(id));
    if (oldDomElement.isElement())
    {
        QDomElement newDomElement = oldDomElement.cloneNode().toElement();
        SaveDialog(newDomElement);

        SaveToolOptions *saveOptions = new SaveToolOptions(oldDomElement, newDomElement, doc, id);
        connect(saveOptions, &SaveToolOptions::NeedLiteParsing, doc, &VPattern::LiteParseTree);
        qApp->getUndoStack()->push(saveOptions);
    }
    else
    {
        qCDebug(vTool)<<"Can't find tool with id ="<< id << Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VDrawTool::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOptions(domElement, obj);
    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VDrawTool::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOptions(domElement, obj);
    }
    else
    {
        qDebug()<<"Can't find tool with id ="<< id << Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QColor VDrawTool::CorrectColor(const QColor &color) const
{
    if (enabled)
    {
        return color;
    }
    else
    {
        return Qt::gray;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogLinkDestroy removes dialog pointer
 */
void VDrawTool::DialogLinkDestroy()
{
    this->dialog=nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VDrawTool::SetFactor(qreal factor)
{
    VApplication::CheckFactor(this->factor, factor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckFormula check formula.
 *
 * Try calculate formula. If find error show dialog that allow user try fix formula. If user can't throw exception. In
 * successes case return result calculation and fixed formula string. If formula ok don't touch formula.
 *
 * @param toolId [in] tool's id.
 * @param formula [in|out] string with formula.
 * @param data [in] container with variables. Need for math parser.
 * @throw QmuParserError.
 * @return result of calculation formula.
 */
qreal VDrawTool::CheckFormula(const quint32 &toolId, QString &formula, VContainer *data)
{
    SCASSERT(data != nullptr)
    qreal result = 0;
    Calculator *cal = nullptr;
    try
    {
        cal = new Calculator(data);
        result = cal->EvalFormula(formula);
        delete cal;
    }
    catch (qmu::QmuParserError &e)
    {
        //Q_UNUSED(e)
        qDebug() << "\nMath parser error:\n"
                 << "--------------------------------------\n"
                 << "Message:     " << e.GetMsg()  << "\n"
                 << "Expression:  " << e.GetExpr() << "\n"
                 << "--------------------------------------";
        delete cal;

        DialogUndo *dialogUndo = new DialogUndo(qApp->getMainWindow());
        if (dialogUndo->exec() == QDialog::Accepted)
        {
            UndoButton resultUndo = dialogUndo->Result();
            delete dialogUndo;
            if (resultUndo == UndoButton::Fix)
            {
                DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, qApp->getMainWindow());
                dialog->setWindowTitle(tr("Edit wrong formula"));
                dialog->SetFormula(formula);
                if (dialog->exec() == QDialog::Accepted)
                {
                    formula = dialog->GetFormula();
                    /* Need delete dialog here because parser in dialog don't allow use correct separator for parsing
                     * here. */
                    delete dialog;
                    Calculator *cal1 = new Calculator(data);
                    result = cal1->EvalFormula(formula);
                    delete cal1; /* Here can be memory leak, but dialog already check this formula and probability
                                  * very low. */
                }
                else
                {
                    delete dialog;
                    throw;
                }
            }
            else
            {
                QString what = QString("Undo wrong formula %1").arg(formula);
                throw VExceptionUndo(what);
            }
        }
        else
        {
            delete dialogUndo;
            throw;
        }
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToCalculation add tool to calculation tag in pattern file.
 * @param domElement tag in xml tree.
 */
void VDrawTool::AddToCalculation(const QDomElement &domElement)
{
    AddToCalc *addToCal = new AddToCalc(domElement, doc);
    connect(addToCal, &AddToCalc::NeedFullParsing, doc, &VPattern::NeedFullParsing);
    qApp->getUndoStack()->push(addToCal);
}

//---------------------------------------------------------------------------------------------------------------------
QString VDrawTool::getLineType() const
{
    return typeLine;
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::SetTypeLine(const QString &value)
{
    typeLine = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
QString VDrawTool::GetLineColor() const
{
    return lineColor;
}


//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::SetLineColor(const QString &value)
{
    lineColor = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}
