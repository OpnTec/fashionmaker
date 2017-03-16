/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef FANCYTAB_H
#define FANCYTAB_H

#include <QIcon>
#include <QPropertyAnimation>

class FancyTab : public QObject
{
    friend class FancyTabBar;
    Q_OBJECT

    Q_PROPERTY(float fader READ fader WRITE setFader)
public:
    explicit FancyTab(QWidget *tabbar);

    float fader();
    void  setFader(float value);

    void fadeIn();
    void fadeOut();

private:
    Q_DISABLE_COPY(FancyTab)
    QIcon              m_icon;
    QString            m_text;
    QString            m_toolTip;
    bool               m_enabled;
    QPropertyAnimation m_Animator;
    QWidget           *m_TabBar;
    float              m_Fader;
};

#endif // FANCYTAB_H
