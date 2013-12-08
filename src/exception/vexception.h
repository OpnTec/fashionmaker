/************************************************************************
 **
 **  @file   vexception.h
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


#ifndef VEXCEPTION_H
#define VEXCEPTION_H

#include <QException>
#include <QString>

/**
 * @brief The VException class parent for all exception. Could be use for abstract exception
 */
class VException : public QException
{
public:
                      /**
                       * @brief VException constructor exception
                       * @param what string with error
                       */
                      VException(const QString &what);
                      /**
                       * @brief VException copy constructor
                       * @param e exception
                       */
                      VException(const VException &e):what(e.What()){}
    virtual           ~VException() Q_DECL_NOEXCEPT_EXPR(true){}
    /**
     * @brief raise method raise for exception
     */
    inline void       raise() const { throw *this; }
    /**
     * @brief clone clone exception
     * @return new exception
     */
    inline VException *clone() const { return new VException(*this); }
    /**
     * @brief ErrorMessage return main error message
     * @return error message
     */
    virtual QString   ErrorMessage() const;
    /**
     * @brief DetailedInformation return detailed information about error
     * @return detailed information
     */
    virtual QString   DetailedInformation() const { return QString(); }
    /**
     * @brief What return string with error
     * @return string with error
     */
    inline QString    What() const {return what;}
protected:
    /**
     * @brief what string with error
     */
    QString           what;
};

#endif // VEXCEPTION_H
