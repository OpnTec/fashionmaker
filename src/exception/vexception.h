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
#include <QWidget>
#include <QCoreApplication>

/**
 * @brief The VException class parent for all exception. Could be use for abstract exception
 */
class VException : public QException
{
    Q_DECLARE_TR_FUNCTIONS(VException)
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
                      VException(const VException &e);
    virtual           ~VException() noexcept (true){}
    /**
     * @brief raise method raise for exception
     */
    void            raise() const;
    /**
     * @brief clone clone exception
     * @return new exception
     */
    VException      *clone() const;
    /**
     * @brief ErrorMessage return main error message
     * @return error message
     */
    virtual QString ErrorMessage() const;
    /**
     * @brief DetailedInformation return detailed information about error
     * @return detailed information
     */
    virtual QString DetailedInformation() const;
    /**
     * @brief What return string with error
     * @return string with error
     */
    QString         What() const;
    /**
     * @brief CriticalMessageBox show Critical Message Box.
     * @param situation main text message box.
     */
    virtual void    CriticalMessageBox(const QString &situation, QWidget *parent = nullptr) const;
    /**
     * @brief AddMoreInformation add more information for error
     * @param info information
     */
    void         AddMoreInformation(const QString &info);
    /**
     * @brief MoreInformation return more information for error
     * @return information
     */
    QString      MoreInformation() const;
protected:
    /**
     * @brief what string with error
     */
    QString         what;
    /**
     * @brief moreInfo more information about error
     */
    QString         moreInfo;
    QString         MoreInfo(const QString &detInfo) const;
};

inline void VException::raise() const
{
    throw *this;
}

inline VException *VException::clone() const
{
    return new VException(*this);
}

inline QString VException::DetailedInformation() const
{
    return moreInfo;
}

inline QString VException::What() const
{
    return what;
}

inline QString VException::MoreInformation() const
{
    return moreInfo;
}

#endif // VEXCEPTION_H
