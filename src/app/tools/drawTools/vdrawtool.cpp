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
#include "dialogs/tools/dialogeditwrongformula.h"
#include "container/calculator.h"
#include "../../undocommands/addtocalc.h"

qreal VDrawTool::factor = 1;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDrawTool constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 */
VDrawTool::VDrawTool(VPattern *doc, VContainer *data, quint32 id)
    :VAbstractTool(doc, data, id), ignoreContextMenuEvent(false), ignoreFullUpdate(false),
      nameActivDraw(doc->GetNameActivDraw()), dialog(nullptr)
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
 * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
 * @param newName new name active pattern peace. name new active pattern peace.
 */
void VDrawTool::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        ignoreContextMenuEvent = false;
    }
    else
    {
        ignoreContextMenuEvent = true;
    }
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
 * @brief FullUpdateFromGui refresh tool data after change in options.
 * @param result keep result working dialog.
 */
void VDrawTool::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            SaveDialog(domElement);

            emit LiteUpdateTree();
            emit toolhaveChange();
        }
    }
    delete dialog;
    dialog = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::FullUpdateFromGuiApply()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SaveDialog(domElement);

        emit LiteUpdateTree();
        emit toolhaveChange();
    }
}

//---------------------------------------------------------------------------------------------------------------------
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
    if (factor <= 2 && factor >= 0.5)
    {
        this->factor = factor;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckFormula check formula.
 *
 * Try calculate formula. If find error show dialog that allow user try fix formula. If user can't throw exception. In
 * successes case return result calculation and fixed formula string. If formula ok don't touch formula.
 * @param formula [in|out] string with formula.
 * @param data [in] container with variables. Need for math parser.
 * @throw QmuParserError.
 * @return result of calculation formula.
 */
qreal VDrawTool::CheckFormula(QString &formula, VContainer *data)
{
    qreal result = 0;
    Calculator *cal = nullptr;
    try
    {
        cal = new Calculator(data);
        result = cal->EvalFormula(formula);
        delete cal;
    }
    catch(qmu::QmuParserError &e)
    {
        Q_UNUSED(e)
        delete cal;
        DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data);
        dialog->setFormula(formula);
        if (dialog->exec() == QDialog::Accepted)
        {
            formula = dialog->getFormula();
            //Need delete dialog here because parser in dialog don't allow use correct separator for parsing here.
            //Don't know why.
            delete dialog;
            Calculator *cal1 = new Calculator(data);
            result = cal1->EvalFormula(formula);
            delete cal1;//Here can be memory leak, but dialog already check this formula and probability very low.
        }
        else
        {
            delete dialog;
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
