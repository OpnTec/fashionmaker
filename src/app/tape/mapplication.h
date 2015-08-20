/************************************************************************
 **
 **  @file   mapplication.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef MAPPLICATION_H
#define MAPPLICATION_H

#include "../vpatterndb/vtranslatevars.h"
#include "../vmisc/def.h"
#include "../vmisc/vtapesettings.h"
#include "../vmisc/vabstractapplication.h"
#include "dialogs/dialogmdatabase.h"

class MApplication;// use in define
class TMainWindow;
class QLocalServer;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<MApplication*>(VAbstractApplication::instance()))

class MApplication : public VAbstractApplication
{
    Q_OBJECT

public:
    MApplication(int &argc, char **argv);
    virtual ~MApplication() Q_DECL_OVERRIDE;

    bool IsTheOnly() const;
    TMainWindow *MainWindow();
    QList<TMainWindow*> MainWindows();

#if defined(Q_WS_MAC)
    bool event(QEvent *event);
#endif

    void InitOptions();
    void LoadTranslation();

    virtual const VTranslateVars *TrVars();
    void InitTrVars();

    virtual void  OpenSettings() Q_DECL_OVERRIDE;
    VTapeSettings *TapeSettings();

    QString translationsPath() const;
    QString diagramsPath() const;

    void ShowDataBase();
    void RetranslateGroups();
    void RetranslateTables();

    void ParseCommandLine(const QStringList &arguments);

public slots:
    TMainWindow *NewMainWindow();

private slots:
    void OpenFile(const QString &path);
    void NewLocalSocketConnection();

private:
    Q_DISABLE_COPY(MApplication)
    QList<QPointer<TMainWindow> > mainWindows;
    QLocalServer *localServer;
    VTranslateVars *trVars;
    QPointer<DialogMDataBase> dataBase;
    QTranslator *qtTranslator;
    QTranslator *qtxmlTranslator;
    QTranslator *appTranslator;
    QTranslator *pmsTranslator;

    void Clean();
    void ClearTranslation();
};

#endif // MAPPLICATION_H
