/************************************************************************
 **
 **  @file   vundocommands.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vundocommands.h"
#include <QDebug>
#include "../widgets/vapplication.h"
#include "../tools/vabstracttool.h"

//---------------------------------------------------------------------------------------------------------------------
AddToCal::AddToCal(const QDomElement &xml, VPattern *doc, QUndoCommand *parent)
    : QObject(), QUndoCommand(parent), xml(xml), doc(doc), nameActivDraw(doc->GetNameActivDraw()),
      cursor(doc->getCursor()), redoFlag(false)
{
    setText(tr("Add object"));
}

//---------------------------------------------------------------------------------------------------------------------
AddToCal::~AddToCal()
{}

//---------------------------------------------------------------------------------------------------------------------
void AddToCal::undo()
{
    doc->ChangeActivPP(nameActivDraw);
    doc->setCursor(cursor);

    QDomElement calcElement;
    if (doc->GetActivNodeElement(VPattern::TagCalculation, calcElement))
    {
        calcElement.removeChild(xml);
        if (cursor > 0)
        {
            doc->setCursor(0);
        }
    }
    else
    {
        qDebug()<<"Can't find tag Calculation"<< Q_FUNC_INFO;
    }
    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
void AddToCal::redo()
{
    doc->ChangeActivPP(nameActivDraw);
    doc->setCursor(cursor);

    QDomElement calcElement;
    if (doc->GetActivNodeElement(VPattern::TagCalculation, calcElement))
    {
        if (cursor <= 0)
        {
            calcElement.appendChild(xml);
        }
        else
        {
            QDomElement refElement = doc->elementById(QString().setNum(cursor));
            if (refElement.isElement())
            {
                calcElement.insertAfter(xml, refElement);
                doc->setCursor(0);
            }
            else
            {
                qDebug()<<"Can not find the element after which you want to insert."<< Q_FUNC_INFO;
            }
        }
    }
    else
    {
        qDebug()<<"Can't find tag Calculation"<< Q_FUNC_INFO;
    }
    if (redoFlag)
    {
        emit NeedFullParsing();
    }
    redoFlag = true;
}

//--------------------------------------------AddPatternPiece----------------------------------------------------------

int AddPatternPiece::countPP = 0;

AddPatternPiece::AddPatternPiece(const QDomElement &xml, VPattern *doc, const QString &namePP, const QString &mPath,
                                 QUndoCommand *parent)
    : QObject(), QUndoCommand(parent), xml(xml), doc(doc), namePP(namePP), redoFlag(false), mPath(mPath)
{
    setText(tr("Add pattern piece %1").arg(namePP));
}

//---------------------------------------------------------------------------------------------------------------------
AddPatternPiece::~AddPatternPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
void AddPatternPiece::undo()
{
    if (countPP <= 1)
    {
        emit ClearScene();
    }
    else
    {
        QDomElement rootElement = doc->documentElement();
        QDomElement patternPiece = doc->GetPPElement(namePP);
        rootElement.removeChild(patternPiece);
        emit NeedFullParsing();
    }
    --countPP;
}

//---------------------------------------------------------------------------------------------------------------------
void AddPatternPiece::redo()
{
    ++countPP;
    if (countPP == 1 && mPath.isEmpty() == false)
    {
        doc->CreateEmptyFile(mPath);
    }

    QDomElement rootElement = doc->documentElement();

    rootElement.appendChild(xml);
    doc->haveLiteChange();

    if (redoFlag)
    {
        emit NeedFullParsing();
    }
    redoFlag = true;
}

//--------------------------------------------MoveSPoint---------------------------------------------------------------
MoveSPoint::MoveSPoint(VPattern *doc, const double &x, const double &y, const quint32 &id, QGraphicsScene *scene,
                       QUndoCommand *parent)
    : QObject(), QUndoCommand(parent), doc(doc), oldX(10.0), oldY(10.0), newX(x), newY(y), sPointId(id), scene(scene)
{
    setText(tr("Move single point"));

    SCASSERT(scene != nullptr);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        oldX = qApp->toPixel(doc->GetParametrDouble(domElement, VAbstractTool::AttrX, "10.0"));
        oldY = qApp->toPixel(doc->GetParametrDouble(domElement, VAbstractTool::AttrY, "10.0"));
    }
    else
    {
        qDebug()<<"Can't find spoint with id ="<< sPointId << Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
MoveSPoint::~MoveSPoint()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveSPoint::undo()
{
    QDomElement domElement = doc->elementById(QString().setNum(sPointId));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, VAbstractTool::AttrX, QString().setNum(qApp->fromPixel(oldX)));
        doc->SetAttribute(domElement, VAbstractTool::AttrY, QString().setNum(qApp->fromPixel(oldY)));

        emit NeedLiteParsing();

        QList<QGraphicsView*> list = scene->views();
        VAbstractTool::NewSceneRect(scene, list[0]);
    }
    else
    {
        qDebug()<<"Can't find spoint with id ="<< sPointId << Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSPoint::redo()
{
    QDomElement domElement = doc->elementById(QString().setNum(sPointId));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, VAbstractTool::AttrX, QString().setNum(qApp->fromPixel(newX)));
        doc->SetAttribute(domElement, VAbstractTool::AttrY, QString().setNum(qApp->fromPixel(newY)));

        emit NeedLiteParsing();

        QList<QGraphicsView*> list = scene->views();
        VAbstractTool::NewSceneRect(scene, list[0]);
    }
    else
    {
        qDebug()<<"Can't find spoint with id ="<< sPointId << Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveSPoint::mergeWith(const QUndoCommand *command)
{
    const MoveSPoint *moveCommand = static_cast<const MoveSPoint *>(command);
    SCASSERT(moveCommand != nullptr);
    const quint32 id = moveCommand->getSPointId();

    if (id != sPointId)
    {
        return false;
    }

    newX = moveCommand->getNewX();
    newY = moveCommand->getNewY();
    return true;
}
