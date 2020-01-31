/************************************************************************
 **
 **  @file   vcmdexport.h
 **  @author Alex Zaharov <alexzkhr@gmail.com>
 **  @date   25 8, 2015
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

#ifndef VCMDEXPORT_H
#define VCMDEXPORT_H

#include <memory>
#include <vector>
#include <QTextStream>
#include <QCoreApplication>
#include <QCommandLineParser>

#include "../dialogs/dialoglayoutsettings.h"
#include "../vmisc/vsysexits.h"

class VCommandLine;
using VCommandLinePtr = std::shared_ptr<VCommandLine>;
using VLayoutGeneratorPtr = std::shared_ptr<VLayoutGenerator>;
enum class PageOrientation : bool;

//@brief: class used to install export command line options and parse their values
//QCommandLineParser* object must exists until this object alive
class VCommandLine
{
public:
    virtual ~VCommandLine() = default;

    //@brief creates object and applies export related options to parser

    //@brief tests if user enabled test mode from cmd, throws exception if not exactly 1 input VAL file supplied in
    //case test mode enabled
    bool IsTestModeEnabled() const;

    //@brief Make all parsing warnings into errors. Have effect only in console mode. Use to force Valentina to
    //immediately terminate if a pattern contains a parsing warning.
    bool IsPedantic() const;

    bool IsNoScalingEnabled() const;

    //@brief tests if user enabled export from cmd, throws exception if not exactly 1 input VAL file supplied in case
    //export enabled
    bool IsExportEnabled() const;

    //@brief tests if user enabled export final measurements from cmd, throws exception if not exactly 1 input VAL
    //file supplied in case export enabled
    bool IsExportFMEnabled() const;

    //@brief returns path to custom measure file or empty string
    QString OptMeasurePath() const;

    //@brief returns the base name of layout files or empty string if not set
    QString OptBaseName() const;

    //@brief returns the absolute path to output destination directory or path to application's current directory if
    //not set
    QString OptDestinationPath() const;

    //@brief returns export type set, defaults 0 - svg
    int OptExportType() const;

    bool IsBinaryDXF() const;
    bool IsTextAsPaths() const;
    bool IsExportOnlyDetails() const;
    bool IsCSVWithHeader() const;

    //@brief returns the piece name regex or empty string if not set
    QString OptExportSuchDetails() const;

    //@brief returns user selected csv codec or empty string if not set
    QString OptCSVCodecName() const;

    //@brief returns user selected csv separator or empty string if not set
    QChar OptCSVSeparator() const;

    //@brief returns the destination path for export final measurements or empty string if not set
    QString OptExportFMTo() const;

    //@brief returns list of user defined materials
    QMap<int, QString> OptUserMaterials() const;

    //generator creation is moved here ... because most options are for it only, so no need to create extra getters...
    //@brief creates VLayoutGenerator
    VLayoutGeneratorPtr DefaultGenerator() const;

    //@brief gets filenames which should be loaded
    QStringList OptInputFileNames() const;

    bool IsGuiEnabled()const;

    bool IsSetGradationSize() const;
    bool IsSetGradationHeight() const;

    QString OptGradationSize() const;
    QString OptGradationHeight() const;
    
    QMarginsF TiledPageMargins() const;
    VAbstractLayoutDialog::PaperSizeTemplate OptTiledPaperSize() const;
    PageOrientation OptTiledPageOrientation() const;

protected:

    VCommandLine();

    //@brief returns VAbstractLayoutDialog::PaperSizeTemplate
    VAbstractLayoutDialog::PaperSizeTemplate OptPaperSize() const;

    Cases OptGroup() const;

    //@brief: called in destructor of application, so instance destroyed and new maybe created (never happen scenario though)
    static void Reset();

    //@brief called to create single object, by VApplication only
    static VCommandLinePtr Get(const QCoreApplication& app);

private:
    Q_DISABLE_COPY(VCommandLine)
    static VCommandLinePtr instance;
    QCommandLineParser parser;
    bool isGuiEnabled;
    friend class VApplication;
    
    VAbstractLayoutDialog::PaperSizeTemplate FormatSize(const QString &key) const;

    void InitCommandLineOptions();
    bool IsOptionSet(const QString &option) const;
    QString OptionValue(const QString &option) const;
    QStringList OptionValues(const QString &option) const;

    int   OptNestingTime() const;
    qreal OptEfficiencyCoefficient() const;
};

#endif // VCMDEXPORT_H
