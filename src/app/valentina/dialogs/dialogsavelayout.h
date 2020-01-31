/************************************************************************
 **
 **  @file   dialogsavelayout.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 1, 2015
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

#ifndef DIALOGSAVELAYOUT_H
#define DIALOGSAVELAYOUT_H

#include "../vgeometry/vgeometrydef.h"
#include "vabstractlayoutdialog.h"
#include "../vlayout/vlayoutdef.h"

#ifndef PDFTOPS
#ifdef Q_OS_WIN
#   define PDFTOPS QStringLiteral("pdftops.exe")
#else
#   define PDFTOPS QStringLiteral("pdftops")
#endif
#endif

namespace Ui
{
    class DialogSaveLAyout;
}

class DialogSaveLayout : public  VAbstractLayoutDialog
{
    Q_OBJECT

public:
    explicit DialogSaveLayout(int count, Draw mode = Draw::Layout, const QString &fileName = QString(),
                              QWidget *parent = nullptr);
    virtual ~DialogSaveLayout();

    QString Path() const;
    QString FileName() const;

    LayoutExportFormats Format() const;
    void                SelectFormat(LayoutExportFormats format);

    void SetBinaryDXFFormat(bool binary);
    bool IsBinaryDXFFormat() const;

    static QString MakeHelpFormatList();
    void   SetDestinationPath(const QString& cmdDestinationPath);

    Draw Mode() const;

    static QString ExportFormatDescription(LayoutExportFormats format);
    static QString ExportFormatSuffix(LayoutExportFormats format);

    bool IsTextAsPaths() const;
    void SetTextAsPaths(bool textAsPaths);

    void SetTiledExportMode(bool tiledExportMode);

    void      SetTiledMargins(QMarginsF margins);
    QMarginsF GetTiledMargins() const;

    void              SetTiledPageFormat(PaperSizeTemplate format);
    PaperSizeTemplate GetTiledPageFormat() const;

    void            SetTiledPageOrientation(PageOrientation orientation);
    PageOrientation GetTiledPageOrientation() const;

protected:
    virtual void showEvent(QShowEvent *event) override;
    void InitTemplates(QComboBox *comboBoxTemplates);

private slots:
    void Save();
    void PathChanged(const QString &text);
    void ShowExample();
private:
    Q_DISABLE_COPY(DialogSaveLayout)
    Ui::DialogSaveLAyout *ui;
    int count;
    bool isInitialized;
    Draw m_mode;
    bool m_tiledExportMode;

    static bool havePdf;
    static bool tested;
    static bool SupportPSTest();
    static bool TestPdf();
    static QVector<std::pair<QString, LayoutExportFormats> > InitFormats();

    void RemoveFormatFromList(LayoutExportFormats format);

    void ReadSettings();
    void WriteSettings() const;
};

#endif // DIALOGSAVELAYOUT_H
