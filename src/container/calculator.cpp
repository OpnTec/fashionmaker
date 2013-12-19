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
#include <QDebug>

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
    //qDebug()<<"Formula: "<<prog;
    index = 0;
    qreal result = get_exp();
    QString str = QString(" = %1").arg(result, 0, 'f', 3);
    debugFormula.append(str);
    //qDebug()<<"Result:"<<debugFormula;
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
    putback();
    return result;
}


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
            break;
        default:
            break;
    }
}

void Calculator::unary(QChar o, qreal *r)
{
    if (o=='-')
    {
        *r = -(*r);
    }
}

qreal Calculator::find_var(QString s)
{
    bool ok = false;
    qreal value = data->FindVar(s, &ok);
    if (ok == false)
    {
        qDebug()<<s;
        serror(4); /* don't variable */
        return 0;
    }
    return value;
}

void Calculator::serror(qint32 error)
{
    //TODO please translate text down here into english
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

char Calculator::look_up(QString s)
{
    QString p;

    p = s;
    p = p.toLower();

    return 0;
}

bool Calculator::isdelim(QChar c)
{
    if (StrChr(" ;,+-<>/*%^=()", c) || c=='\n' || c=='\r' || c=='\0')
    {
        return true;
    }
    return false;
}

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
    { /* end of file */
        token="\0";
        tok=FINISHED;
        token_type=DELIMITER;
        return;
    }

    while (iswhite(prog[index]))
    {
        ++index;  /* skip spaces */
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
    { /* delimiter */
        *temp=prog[index];
        index++; /* jump to the next position */
        temp->append("\0");
        token_type=DELIMITER;
        debugFormula.append(token);
        return;
    }
    if (prog[index]=='"')
    { /* quoted string */
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
    { /* number */
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
    { /* variable or command */
        while (isdelim(prog[index]) == false)
        {
            temp->append(prog[index]);
            index++;
        }
        token_type=STRING;
    }
    temp->append("\0");

    /* Seen if there is a command line or a variable */
    if (token_type==STRING)
    {
        tok=look_up(token);
        if (tok == false)
        {
            token_type = VARIABLE;
        }
        else
        {
            token_type = COMMAND; /* It is command */
        }
    }
    return;
}

bool Calculator::StrChr(QString string, QChar c)
{
    return string.contains(c, Qt::CaseInsensitive);
}

void Calculator::putback()
{
    QString t;
    t = token;
    index = index - t.size();
}
