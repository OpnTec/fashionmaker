/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/

#ifndef QXTCSVMODEL_H
#define QXTCSVMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QIODevice>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QModelIndex>
#include <def.h>
class QTextCodec;

class QxtCsvModelPrivate;
class QxtCsvModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit QxtCsvModel(QObject *parent = nullptr);
    explicit QxtCsvModel(QIODevice *file, QObject *parent = nullptr, bool withHeader = false, QChar separator = ',');
    explicit QxtCsvModel(const QString &filename, QObject *parent = nullptr, bool withHeader = false,
                         QChar separator = ',');
    virtual ~QxtCsvModel();

    virtual int      rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int      columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual bool     setData(const QModelIndex& index, const QVariant& data, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual bool     setHeaderData(int section, Qt::Orientation orientation, const QVariant& value,
                                   int role = Qt::DisplayRole) Q_DECL_OVERRIDE;
    void             setHeaderData(const QStringList& data);

    QString text(int row, int column) const;
    void setText(int row, int column, const QString& value);

    QString headerText(int column) const;
    void setHeaderText(int column, const QString& value);


    bool insertRow(int row, const QModelIndex& parent = QModelIndex());
    virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) Q_DECL_OVERRIDE;

    bool removeRow(int row, const QModelIndex& parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) Q_DECL_OVERRIDE;

    bool insertColumn(int col, const QModelIndex& parent = QModelIndex());
    virtual bool insertColumns(int col, int count, const QModelIndex& parent = QModelIndex()) Q_DECL_OVERRIDE;

    bool removeColumn(int col, const QModelIndex& parent = QModelIndex());
    virtual bool removeColumns(int col, int count, const QModelIndex& parent = QModelIndex()) Q_DECL_OVERRIDE;

    void setSource(QIODevice *file, bool withHeader = false, QChar separator = ',', QTextCodec* codec = nullptr);
    void setSource(const QString &filename, bool withHeader = false, QChar separator = ',',
                   QTextCodec* codec = nullptr);

    void toCSV(QIODevice *file, bool withHeader = false, QChar separator = ',', QTextCodec* codec = nullptr) const;
    void toCSV(const QString &filename, bool withHeader = false, QChar separator = ',',
               QTextCodec* codec = nullptr) const;

    enum QuoteOption { NoQuotes = 0,
                       SingleQuote = 1,
                       DoubleQuote = 2,
                       BothQuotes = 3,
                       NoEscape = 0,
                       TwoQuoteEscape = 4,
                       BackslashEscape = 8,
                       AlwaysQuoteOutput = 16,
                       DefaultQuoteMode = BothQuotes | BackslashEscape | AlwaysQuoteOutput };
    Q_DECLARE_FLAGS(QuoteMode, QuoteOption)

    QuoteMode quoteMode() const;
    void setQuoteMode(QuoteMode mode);

    virtual Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QxtCsvModel)
    QXT_DECLARE_PRIVATE(QxtCsvModel)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QxtCsvModel::QuoteMode)

#endif // QXTCSVMODEL_H
