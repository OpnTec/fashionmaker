/************************************************************************
 **
 **  @file   dialoglayoutprogress.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef DIALOGLAYOUTPROGRESS_H
#define DIALOGLAYOUTPROGRESS_H

#include <QDialog>
#include <QElapsedTimer>
#include <QTimer>

#include "../vlayout/vlayoutdef.h"

namespace Ui
{
    class DialogLayoutProgress;
}

class DialogLayoutProgress : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLayoutProgress(QElapsedTimer timer, qint64 timeout, QWidget *parent = nullptr);
    ~DialogLayoutProgress();

signals:
    void Abort();
    void Timeout();

public slots:
    void Start();
    void Finished();
    void Efficiency(qreal value);

protected:
    virtual void showEvent(QShowEvent *event) override;

private:
    Q_DISABLE_COPY(DialogLayoutProgress)
    Ui::DialogLayoutProgress *ui;
    QMovie *m_movie;
    QElapsedTimer m_timer;
    qint64 m_timeout;
    bool isInitialized{false};
    QTimer *progressTimer;
};

#endif // DIALOGLAYOUTPROGRESS_H
