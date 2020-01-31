/************************************************************************
 **
 **  @file   mapplication.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 7, 2015
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

enum class SocketConnection : bool {Client = false, Server = true};

class MApplication : public VAbstractApplication
{
    Q_OBJECT

public:
    MApplication(int &argc, char **argv);
    virtual ~MApplication() override;

    virtual bool notify(QObject * receiver, QEvent * event) override;

    bool IsTestMode() const;
    virtual bool IsAppInGUIMode() const override;
    TMainWindow *MainWindow();
    QList<TMainWindow*> MainWindows();
    TMainWindow *NewMainWindow();

    void InitOptions();

    virtual const VTranslateVars *TrVars() override;

    virtual void  OpenSettings() override;
    VTapeSettings *TapeSettings();
    void               ActivateDarkMode();

    QString diagramsPath() const;

    void ShowDataBase();
    void RetranslateGroups();
    void RetranslateTables();

    void ParseCommandLine(const SocketConnection &connection, const QStringList &arguments);

public slots:
    void ProcessCMD();

protected:
    virtual void InitTrVars() override;
    virtual bool event(QEvent *e) override;

protected slots:
    virtual void AboutToQuit() override;

private slots:
    void NewLocalSocketConnection();

private:
    Q_DISABLE_COPY(MApplication)
    QList<QPointer<TMainWindow> > mainWindows;
    QLocalServer *localServer;
    VTranslateVars *trVars;
    QPointer<DialogMDataBase> dataBase;
    bool testMode;

    void Clean();
};
//---------------------------------------------------------------------------------------------------------------------

inline const VTranslateVars *MApplication::TrVars()
{
    return trVars;
}

#endif // MAPPLICATION_H
