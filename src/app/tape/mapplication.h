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

#include <QApplication>

#include "../vpatterndb/vtranslatevars.h"
#include "../vmisc/def.h"
#include "../vmisc/vtapesettings.h"

class MApplication;// use in define
class TMainWindow;
class QLocalServer;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<MApplication*>(QApplication::instance()))

class MApplication : public QApplication
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

    virtual const VTranslateVars *TrVars();
    void InitTrVars();

    void      OpenSettings();
    VTapeSettings *Settings();

    QString translationsPath() const;

    template <typename T>
    QString LocaleToString(const T &value);

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

    /**
     * @brief settings pointer to settings. Help hide constructor creation settings. Make make code more readable.
     */
    VTapeSettings *settings;

    void Clean();
};


//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline QString MApplication::LocaleToString(const T &value)
{
    QLocale loc;
    qApp->Settings()->GetOsSeparator() ? loc = QLocale::system() : loc = QLocale(QLocale::C);
    return loc.toString(value);
}

#endif // MAPPLICATION_H
