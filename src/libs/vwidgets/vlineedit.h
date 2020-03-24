/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef VLINEEDIT_H
#define VLINEEDIT_H

#include <QLineEdit>

class QStringListModel;

class VLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit VLineEdit(QWidget * parent = nullptr);
    VLineEdit(const QString &contents, QWidget *parent = nullptr);

protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
private:
    Q_DISABLE_COPY(VLineEdit)
    bool m_selectOnMousePress;
};

/*! Line edit widget with auto completion based on QStringListModel.
  Modified behaviour: completion list will appear even when contents of
  line edit is empty. Full list of options will be showed when line edit
  has focus and is empty.
  */
class VCompleterLineEdit : public VLineEdit
{
    Q_OBJECT
public:
    explicit VCompleterLineEdit(QWidget *parent = nullptr);

    //! Set list of options used for completion.
    void SetCompletion(const QStringList &list);

protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void customEvent(QEvent* e) override;

private slots:
    void ShowCompletion();
    void CompletionPopup();

private:
    Q_DISABLE_COPY(VCompleterLineEdit)
    QStringListModel *m_model;
};

#endif // VLINEEDIT_H
