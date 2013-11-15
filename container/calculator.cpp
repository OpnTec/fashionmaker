/************************************************************************
 **
 **  @file   calculator.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

#include "calculator.h"

#define DELIMITER  1
#define VARIABLE   2
#define NUMBER     3
#define COMMAND    4
#define STRING     5
#define QUOTE      6
#define FINISHED   10
#define EOL        9

qreal Calculator::eval(QString prog, QString *errorMsg)
{
    this->errorMsg = errorMsg;
    this->errorMsg->clear();
    debugFormula.clear();
    this->prog = prog;
    //qDebug()<<"Формула: "<<prog;
    index = 0;
    qreal result = get_exp();
    QString str = QString(" = %1").arg(result, 0, 'f', 3);
    debugFormula.append(str);
    //qDebug()<<"Результат:"<<debugFormula;
    return result;
}

qreal Calculator::get_exp()
{
    qreal result = 0;
    get_token();
    if (token.isEmpty())
    {
        serror(2);
        return 0;
    }
    level2(&result);
    putback(); /* возвращает последнюю считаную
                  лексему обратно во входной поток */
    return result;
}

/* Сложение или вычитание двух термов */
void Calculator::level2(qreal *result)
{
    QChar op;
    qreal hold;

    level3(result);
    while ((op=token[0]) == '+' || op == '-')
    {
        get_token();
        level3(&hold);
        arith(op, result, &hold);
    }
}

/* Вычисление произведения или частного двух фвкторов */
void Calculator::level3(qreal *result)
{
    QChar op;
    qreal hold;

    level4(result);

    while ((op = token[0]) == '*' || op == '/' || op == '%')
    {
        get_token();
        level4(&hold);
        arith(op, result, &hold);
    }
}

/* Обработка степени числа (целочисленной) */
void Calculator::level4(qreal *result)
{
    qreal hold;

    level5(result);
    if (token[0] == '^')
    {
        get_token();
        level4(&hold);
        arith('^', result, &hold);
    }
}

/* Унарный + или - */
void Calculator::level5(qreal *result)
{
    QChar op;

    op = '\0';
    if ((token_type==DELIMITER) && (token[0]=='+' || token[0]=='-'))
    {
        op = token[0];
        get_token();
    }
    level6(result);
    if (op != '\0')
    {
        unary(op, result);
    }
}

/* Обработка выражения в круглых скобках */
void Calculator::level6(qreal *result)
{
    if ((token[0] == '(') && (token_type == DELIMITER))
    {
        get_token();
        level2(result);
        if (token[0] != ')')
        {
            serror(1);
        }
        get_token();
    } else
        primitive(result);
}

/* Определение значения переменной по ее имени */
void Calculator::primitive(qreal *result)
{
    QString str;
    switch (token_type)
    {
        case VARIABLE:
            *result = find_var(token);
            str = QString("%1").arg(*result, 0, 'f', 3);
            debugFormula.append(str);
            get_token();
            return;
        case NUMBER:
            *result  = token.toDouble();
            str = QString("%1").arg(*result, 0, 'f', 3);
            debugFormula.append(str);
            get_token();
            return;
        default:
            serror(0);
    }
}

/* Выполнение специфицированной арифметики */
void Calculator::arith(QChar o, qreal *r, qreal *h)
{
    qreal  t;//, ex;

    switch (o.toLatin1())
    {
        case '-':
            *r = *r-*h;
            break;
        case '+':
            *r = *r+*h;
            break;
        case '*':
            *r = *r * *h;
            break;
        case '/':
            *r = (*r)/(*h);
            break;
        case '%':
            t = (*r)/(*h);
            *r = *r-(t*(*h));
            break;
        case '^':
            *r = pow(*r, *h);
//            ex =*r;
//            if(*h==0) {
//                *r = 1;
//                break;
//            }
//            for(t=*h-1; t>0; --t)
//                *r = (*r) * ex;
            break;
        default:
            break;
    }
}

/* Изменение знака */
void Calculator::unary(QChar o, qreal *r)
{
    if (o=='-')
    {
        *r = -(*r);
    }
}

/* Поиск значения переменной */
qreal Calculator::find_var(QString s)
{
    bool ok = false;
    qreal value = data->FindVar(s, &ok);
    if (ok == false)
    {
        qDebug()<<s;
        serror(4); /* не переменная */
        return 0;
    }
    return value;
}

/* выдать сообщение об ошибке */
void Calculator::serror(qint32 error)
{
    QString e[]=
    {
                 "Синтаксическая ошибка",
                 "Непарные круглые скобки",
                 "Это не выражение",
                 "Предполагается символ равенства",
                 "Не переменная"
    };
    errorMsg->clear();
    *errorMsg = e[error];
    qDebug()<<e[error];
}

/* Поиск соответствия внутреннего формата для
                текущей лексемы в таблице лексем.
             */
char Calculator::look_up(QString s)
{
    QString p;

    /* преобразование к нижнему регистру */
    p = s;
    p = p.toLower();

    /* просматривается, если лексема обнаружена в
       таблице */
    /*
     *у нас більше немає команд що потрібно опрацьовувати
     */
//    if(commands.contains(p)){
//        return commands[p];
//    }
    return 0; /* нераспознанная команда */
}

/* Возвращает "истину", если "c" разделитель */
bool Calculator::isdelim(QChar c)
{
    if (StrChr(" ;,+-<>/*%^=()", c) || c=='\n' || c=='\r' || c=='\0')
    {
        return true;
    }
    return false;
}

/* Возвращает 1, если "с" пробел или табуляция */
bool Calculator::iswhite(QChar c)
{
    if (c==' ' || c=='\t')
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Calculator::get_token()
{
    QString *temp;

    token_type=0; tok=0;
    token.clear();
    temp=&token;

    if (prog[index]=='\0')
    { /* Конец файла */
        token="\0";
        tok=FINISHED;
        token_type=DELIMITER;
        return;
    }

    while (iswhite(prog[index]))
    {
        ++index;  /* пропуск пробелов */
    }

    if (prog[index]=='\r')
    { /* crtl */
        ++index; ++index;
        tok= EOL; token='\r';
        token.append('\n');token.append("\0");
        token_type = DELIMITER;
        return;
    }

    if (StrChr("+-*^/%=;(),><", prog[index]))
    { /* разделитель */
        *temp=prog[index];
        index++; /* переход на следующую позицию */
        temp->append("\0");
        token_type=DELIMITER;
        debugFormula.append(token);
        return;
    }
    if (prog[index]=='"')
    { /* строка в кавычках */
        index++;
        while (prog[index] != '"' && prog[index] != '\r')
        {
            temp->append(prog[index]);
            index++;
        }
        if (prog[index]=='\r')
        {
            serror(1);
        }
        index++;temp->append("\0");
        token_type=QUOTE;
        return;
    }
    if (prog[index].isDigit())
    { /* число */
        while (isdelim(prog[index]) == false)
        {
            temp->append(prog[index]);
            index++;
        }
        temp->append('\0');
        token_type = NUMBER;
        return;
    }

    if (prog[index].isPrint())
    { /* переменная или команда */
        while (isdelim(prog[index]) == false)
        {
            temp->append(prog[index]);
            index++;
        }
        token_type=STRING;
    }
    temp->append("\0");

    /* Просматривается, если строка есть команда или переменная */
    if (token_type==STRING)
    {
        tok=look_up(token); /* преобразование во внутренний
                                          формат */
        if (tok == false)
        {
            token_type = VARIABLE;
        }
        else
        {
            token_type = COMMAND; /* это команда */
        }
    }
    return;
}

bool Calculator::StrChr(QString string, QChar c)
{
    return string.contains(c, Qt::CaseInsensitive);
}

/*  Возвращает лексему обратно во входной поток */
void Calculator::putback()
{
    QString t;
    t = token;
    index = index - t.size();
}
