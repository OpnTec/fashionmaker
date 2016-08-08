/***************************************************************************************************
 **
 **  Copyright (c) 2012 Linas Valiukas and others.
 **
 **  Permission is hereby granted, free of charge, to any person obtaining a copy of this
 **  software and associated documentation files (the "Software"), to deal in the Software
 **  without restriction, including without limitation the rights to use, copy, modify,
 **  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 **  permit persons to whom the Software is furnished to do so, subject to the following conditions:
 **
 **  The above copyright notice and this permission notice shall be included in all copies or
 **  substantial portions of the Software.
 **
 **  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 **  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 **  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 **  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 **  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **
 ******************************************************************************************************/

#ifndef FVUPDATEWINDOW_H
#define FVUPDATEWINDOW_H

#include <QDialog>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

class QGraphicsScene;
class QWidget;

namespace Ui
{
    class FvUpdateWindow;
}

class FvUpdateWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FvUpdateWindow(QWidget *parent = nullptr);
    virtual ~FvUpdateWindow();

    // Update the current update proposal from FvUpdater
    bool UpdateWindowWithCurrentProposedUpdate();

private:
    Q_DISABLE_COPY(FvUpdateWindow)

    Ui::FvUpdateWindow*	m_ui;
    QGraphicsScene* m_appIconScene;

};

#endif // FVUPDATEWINDOW_H
