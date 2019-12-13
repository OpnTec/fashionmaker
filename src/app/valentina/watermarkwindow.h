/************************************************************************
 **
 **  @file   watermarkwindow.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 12, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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
#ifndef WATERMARKWINDOW_H
#define WATERMARKWINDOW_H

#include <QMainWindow>

#include "../ifc/ifcdef.h"
#include "../vformat/vwatermark.h"
#include "../vmisc/vlockguard.h"

namespace Ui
{
class WatermarkWindow;
}

class WatermarkWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WatermarkWindow(const QString &patternPath, QWidget *parent = nullptr);
    ~WatermarkWindow();

    QString CurrentFile() const;

    bool Open(QString path);

signals:
    void New();
    void OpenAnother(const QString &fileName) const;

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void changeEvent(QEvent* event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_actionNew_triggered();
    bool on_actionSaveAs_triggered();
    bool on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();

    void WatermarkChangesWereSaved(bool saved);

private:
    Q_DISABLE_COPY(WatermarkWindow)
    Ui::WatermarkWindow *ui;
    QString m_patternPath;
    QString m_curFile{};
    VWatermarkData m_data{};
    QColor m_okPathColor;
    bool m_isInitialized{false};

    int     m_curFileFormatVersion{0x0};
    QString m_curFileFormatVersionStr{QLatin1String("0.0.0")};

    QSharedPointer<VLockGuard<char>> lock{};

    bool MaybeSave();
    void UpdateWindowTitle();

    QString GetWatermarkFileName();

    bool ContinueFormatRewrite(const QString &currentFormatVersion, const QString &maxFormatVersion);
    bool SaveWatermark(const QString &fileName, QString &error);
    void SetCurrentFile(const QString &fileName);
    bool OpenNewEditor(const QString &fileName = QString()) const;
    void Clear();
    bool IgnoreLocking(int error, const QString &path);
    void ShowWatermark();
    void ValidatePath();
    void ToolBarStyle(QToolBar *bar);
};

#endif // WATERMARKWINDOW_H
