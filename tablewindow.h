/************************************************************************
 **
 **  @file tablewindow.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QMainWindow>
#include "widgets/vitem.h"

namespace Ui
{
    class TableWindow;
}

/**
 * @brief TableWindow клас вікна створення розкладки.
 */
class TableWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief numberDetal Виводиться кількість деталей що ще залишилися.
     */
    QLabel*               numberDetal;
    /**
     * @brief colission Виводиться чи є колізії.
     */
    QLabel*               colission;
    /**
     * @brief TableWindow Конструктор класу вікна створення розкладки.
     * @param parent Батько об'єкту. За замовчуванням = 0.
     */
                 explicit TableWindow(QWidget *parent = 0);
    /**
     * @brief ~TableWindow Деструктор класу вікна.
     */
    ~TableWindow();
public slots:
    /**
     * @brief ModelChosen Слот, що виконується при отриманні сигналу про розрахунок всіх деталей
     *моделі.
     * @param listDetails Список моделей деталі.
     */
    void                  ModelChosen(QVector<VItem*> listDetails);
    /**
     * @brief StopTable Слот, що виконується при хованні вікна. Виконує неохідні очищення і ховає
     *вікно.
     */
    void                  StopTable();
    /**
     * @brief saveScene Слот виконується при натисненні кнопки зберегти.
     */
    void                  saveScene();
    /**
     * @brief GetNextDetail Слот виконується при натисненні кнопки наступна деталь.
     */
    void                  GetNextDetail();
    /**
     * @brief itemChect Відключає можливість виклику перевороту, якщо не вибрано жодної деталі.
     * @param flag Булеве значення що регулює стан кнопки.
     */
    void                  itemChect(bool flag);
    /**
     * @brief itemOut Слот, що виконується при отриманні сигналу виходу за межі листа.
     * @param number Номер деталі в списку що вийшла за межі.
     * @param flag Зберігає стан деталі.
     */
    void                  itemOut(int number, bool flag);
    /**
     * @brief itemColliding Сигнал, що виконується при отриманні сигналу колізії об'єктів.
     * @param list Список об'єктів що перетинаються.
     * @param number 0 - включити до списку деталей що перетинаються, 1 - виключити зі списку.
     */
    void                  itemColliding(QList<QGraphicsItem *> list, int number);
    /**
     * @brief AddLength Збільшує довжину листа на певне значення за один раз.
     */
    void                  AddLength();
    /**
     * @brief RemoveLength Зменшує довжину листа на певне знечення за один раз.
     *Зменшення відбувається до мінімально заданого значення.
     */
    void                  RemoveLength();
signals:
    /**
     * @brief closed Сигнал посилається при хованні вікна.
     */
    void                  closed();
    /**
     * @brief LengthChanged Сигнал посилається при зміні розміру листа.
     */
    void                  LengthChanged();
protected:
    /**
     * @brief closeEvent Перехоплення події закриття.
     * @param event Подія що отримується.
     */
    void                  closeEvent(QCloseEvent *event);
    /**
     * @brief moveToCenter Переміщує вікно у центер екрану.
     */
    void                  moveToCenter();
    /**
     * @brief showEvent Перехоплення події показу вікна для розміщення його поцентру.
     * @param event Подія що отримується.
     */
    void                  showEvent ( QShowEvent * event );
    void                  keyPressEvent ( QKeyEvent * event );
private:
    Q_DISABLE_COPY(TableWindow)
    /**
     * @brief ui Змінна для доступу до об'єктів вікна.
     */
    Ui::TableWindow*      ui;
    /**
     * @brief listDetails Список деталей на укладання.
     */
    QVector<VItem*>       listDetails;
    /**
     * @brief outItems Змінна зберігає показує чи маємо деталі що виходять за межі листа.
     */
    bool                  outItems;
    /**
     * @brief collidingItems Змінна показує чи маємо деталі що перетинаються.
     */
    bool                  collidingItems;
    /**
     * @brief currentScene Зберігається покажчик на сцену.
     */
    QGraphicsScene*       currentScene;
    /**
     * @brief paper Зберігається покажчик на прямокутник що імітує листа паперу.
     */
    QGraphicsRectItem*    paper;
    /**
     * @brief shadowPaper Зберігається покажчик на прямокутник що імітує тінь листа паперу.
     */
    QGraphicsRectItem*    shadowPaper;
    /**
     * @brief checkNext Метод регулює стан кнопки Next.
     */
    void                  checkNext();
    /**
     * @brief listOutItems Список стану виходу за лист кожної деталі.
     */
    QBitArray*            listOutItems;
    /**
     * @brief listCollidingItems Список що зберігає деталі що перетинаються між собою.
     */
    QList<QGraphicsItem*> listCollidingItems;
    /**
     * @brief AddPaper Додає на сцену лист паперу з тінню.
     */
    void                  AddPaper();
    /**
     * @brief AddDetail Додає наступну деталь.
     */
    void                  AddDetail();
    /**
     * @brief indexDetail Індекс деталі в списку, що буде викладатися наступного разу.
     */
    qint32                indexDetail;
    /**
     * @brief sceneRect Мінімальний розмір листа паперу що буде показуватися на сцені.
     */
    QRectF                sceneRect;
    void SvgFile(const QString &name)const;
    void PngFile(const QString &name)const;
    void PsFile(const QString &name)const;
};

#endif // TABLEWINDOW_H
