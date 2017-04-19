/************************************************************************
 **
 **  @file   vapplication.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VAPPLICATION_H
#define VAPPLICATION_H

#include "../vmisc/vabstractapplication.h"
#include "../options.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../vpatterndb/vtranslatevars.h"
#include "vsettings.h"
#include "vcmdexport.h"

class VApplication;// use in define

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<VApplication*>(VAbstractApplication::instance()))

/**
 * @brief The VApplication class reimplamentation QApplication class.
 */
class VApplication : public VAbstractApplication
{
    Q_OBJECT
public:

    VApplication(int &argc, char ** argv);
    virtual ~VApplication() Q_DECL_OVERRIDE;
    static void        NewValentina(const QString &fileName = QString());
    virtual bool       notify(QObject * receiver, QEvent * event) Q_DECL_OVERRIDE;

    void               InitOptions();

    QString            TapeFilePath() const;

    QTimer             *getAutoSaveTimer() const;
    void               setAutoSaveTimer(QTimer *value);

    static QStringList LabelLanguages();

    void               StartLogging();
    QTextStream       *LogFile();

    virtual const VTranslateVars *TrVars() Q_DECL_OVERRIDE;

#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
    static void        DrMingw();
    void               CollectReports() const;
#endif // defined(Q_OS_WIN) && defined(Q_CC_GNU)
    bool static IsGUIMode();
    virtual bool IsAppInGUIMode() const Q_DECL_OVERRIDE;

    virtual void OpenSettings() Q_DECL_OVERRIDE;
    VSettings *ValentinaSettings();

protected:
    virtual void       InitTrVars() Q_DECL_OVERRIDE;
    virtual bool	   event(QEvent *e) Q_DECL_OVERRIDE;

private slots:
#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
    void               CleanGist() const;
#endif // defined(Q_OS_WIN) && defined(Q_CC_GNU)

private:
    Q_DISABLE_COPY(VApplication)
    VTranslateVars     *trVars;
    QTimer             *autoSaveTimer;

    std::shared_ptr<VLockGuard<QFile>> lockLog;
    std::shared_ptr<QTextStream> out;

#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
    static const QString GistFileName;

    void               CollectReport(const QString &reportName) const;
    void               SendReport(const QString &reportName) const;
    QString            ReadFileForSending(QFile &file)const;
    void               ClearOldReports()const;
    void               GatherLogs()const;
#endif // defined(Q_OS_WIN) && defined(Q_CC_GNU)

    QString            LogDirPath()const;
    QString            LogPath()const;
    bool               CreateLogDir()const;
    void               BeginLogging();
    void               ClearOldLogs()const;

public:
    //moved to the end of class so merge should go
    const VCommandLinePtr     CommandLine() const;
};

//---------------------------------------------------------------------------------------------------------------------
inline QTimer *VApplication::getAutoSaveTimer() const
{
    return autoSaveTimer;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VApplication::setAutoSaveTimer(QTimer *value)
{
    autoSaveTimer = value;
}
//---------------------------------------------------------------------------------------------------------------------


#endif // VAPPLICATION_H
