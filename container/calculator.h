/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QtGlobal>
#include <QString>
#include <QMap>
#include <QLineF>
#include <QPointF>
#include "vcontainer.h"

/**
 * @brief The Calculator клас калькулятора формул лекал. Виконує розрахунок формул з підставлянням
 * значеннь зміних.
 */
class Calculator
{
public:
    /**
     * @brief Calculator конструктор класу. Використовується при розрахунку лекала.
     * @param data покажчик на контейнер змінних
     */
    Calculator(const VContainer *data);
    /**
     * @brief eval виконує розрахунок формули.
     * @param prog рядко в якому зберігається формула.
     * @return значення формули.
     */
    qreal eval(QString prog, QString *errorMsg);
private:
    Q_DISABLE_COPY(Calculator)
    QString *errorMsg;
    /**
     * @brief token теперішня лексема.
     */
    QString token;
    /**
     * @brief tok внутрішне представлення лексеми.
     */
    qint32 tok;
    /**
     * @brief token_type тип лексеми.
     */
    qint32 token_type;
    /**
     * @brief prog рядок в якому зберігається формула.
     */
    QString prog; /* Содержит анализируемое выражение */
    /**
     * @brief index номер символу в рядку формули.
     */
    qint32 index; /* Индекс символа в строке*/
    /**
     * @brief data контейнер усіх змінних.
     */
    const VContainer *data;
    /**
     * @brief debugFormula рядок розшифрованої формули.
     */
    QString debugFormula;
    /**
     * @brief get_exp виконує розрахунок формули.
     * @return значення формули.
     */
    qreal get_exp();
    /**
     * @brief get_token повертає наступну лексему.
     */
    void get_token();/* Получить лексему */
    /**
     * @brief StrChr перевіряє чи символ належить рядку.
     * @param string рядок
     * @param c символ.
     * @return true - належить рядку, false - не належить рядку.
     */
    static bool StrChr(QString string, QChar c);
    /**
     * @brief putback повертає зчитану лексему назад у потік.
     */
    void putback();
    /**
     * @brief level2 метод додавання і віднімання двух термів.
     * @param result результат операції.
     */
    void level2(qreal *result);
    /**
     * @brief level3 метод множення, ділення, знаходження процентів.
     * @param result результат операції.
     */
    void level3(qreal *result);
    /**
     * @brief level4 метод знаходження степені двох чисел.
     * @param result результат операції.
     */
    void level4(qreal *result);
    /**
     * @brief level5 метод знаходження унарного плюса чи мінуса.
     * @param result результат операції.
     */
    void level5(qreal *result);
    /**
     * @brief level6 метод обробки виразу в круглих лапках.
     * @param result результат операції.
     */
    void level6(qreal *result);
    /**
     * @brief primitive метод визначення значення зміної по її імені.
     * @param result результат операції.
     */
    void primitive(qreal *result);
    /**
     * @brief arith виконання специфікованої арифметики. Результат записується в перший елемент.
     * @param o знак операції.
     * @param r перший елемент.
     * @param h другий елемент.
     */
    static void arith(QChar o, qreal *r, qreal *h);
    /**
     * @brief unary метод зміни знаку.
     * @param o символ знаку.
     * @param r елемент.
     */
    static void unary(QChar o, qreal *r);
    /**
     * @brief find_var метод знаходить змінну за іменем.
     * @param s ім'я змінної.
     * @return значення зміної.
     */
    qreal find_var(QString s);
    void serror(qint32 error);
    /**
     * @brief look_up пошук відповідного внутрішнього формату для теперішньої лексеми в таблиці лексем.            текущей лексемы в таблице лексем
     * @param s ім'я лексеми.
     * @return внутрішній номер лексеми.
     */
    static char look_up(QString s);
    /**
     * @brief isdelim повертає "істино", якщо с розділювач.
     * @param c символ.
     * @return розділювач, або ні.
     */
    static bool isdelim(QChar c);
    /**
     * @brief iswhite перевіряє чи с пробіл чи табуляція.
     * @param c символ.
     * @return так або ні.
     */
    static bool iswhite(QChar c);
};

#endif // CALCULATOR_H
