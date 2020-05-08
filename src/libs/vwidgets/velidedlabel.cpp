/************************************************************************
 **
 **  @file   velidedlabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 5, 2020
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2020 Valentina project
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
#include "velidedlabel.h"

#include <QPainter>
#include <QTextLayout>

//---------------------------------------------------------------------------------------------------------------------
VElidedLabel::VElidedLabel(QWidget *parent)
    : QFrame(parent)
{
    // reserve size for one line of text
    setMinimumHeight(fontMetrics().lineSpacing());
}

//---------------------------------------------------------------------------------------------------------------------
VElidedLabel::VElidedLabel(const QString &text, QWidget *parent)
    : QFrame(parent),
      m_content(text)
{
    // reserve size for one line of text
    setMinimumHeight(fontMetrics().lineSpacing());
}

//---------------------------------------------------------------------------------------------------------------------
void VElidedLabel::setText(const QString &text)
{
    m_content = text;
    update();
}

//---------------------------------------------------------------------------------------------------------------------
void VElidedLabel::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    bool didElide = false;
    int y = 0;

    // QTextLayout works with paragraphs
    QStringList paragraphs = m_content.split('\n');
    for (int i = 0; i < paragraphs.size(); ++i)
    {
        if (not DrawParagraph(painter, paragraphs.at(i), y, didElide))
        {
            // It text has empty strings a user will not see elided string, but we still want to show a tooltip.
            if (i != paragraphs.size()-1 && !didElide)
            {
                didElide = true;
            }
            break;
        }
    }

    if (didElide != m_elided)
    {
        m_elided = didElide;

        if (m_showElided)
        {
            if (m_elided)
            {
                // Show a tooltip in case we cannot show a whole text in the widget
                // Use dumb html tag to separate paragraphs.
                QString toolTip;
                for(int i = 0; i < paragraphs.size(); ++i)
                {
                    if (i != 0)
                    {
                        toolTip.append(QString("<br>"));
                    }
                    toolTip.append(QString("<font>%1</font>").arg(paragraphs.at(i)));
                }
                setToolTip(toolTip);
            }
            else
            {
                setToolTip(QString());
            }
        }
        emit ElisionChanged(didElide);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VElidedLabel::DrawParagraph(QPainter &painter, const QString& paragraph, int &y, bool &didElide)
{
    QFontMetrics fontMetrics = painter.fontMetrics();

    int lineSpacing = fontMetrics.lineSpacing();
    bool ok = true;

    QTextLayout textLayout(paragraph, painter.font());
    textLayout.beginLayout();
    forever
    {
        QTextLine line = textLayout.createLine();

        if (!line.isValid())
        {
            break;
        }

        line.setLineWidth(width());
        int nextLineY = y + lineSpacing;

        if (height() >= nextLineY + lineSpacing)
        {
            line.draw(&painter, QPoint(0, y));
            y = nextLineY;
        }
        else
        {
            QString lastLine = paragraph.mid(line.textStart());
            QString elidedLastLine = fontMetrics.elidedText(lastLine, Qt::ElideRight, width());
            painter.drawText(QPoint(0, y + fontMetrics.ascent()), elidedLastLine);
            y = nextLineY;
            ok = false;
            didElide = lastLine != elidedLastLine;
            break;
        }
    }
    textLayout.endLayout();
    return ok;
}
