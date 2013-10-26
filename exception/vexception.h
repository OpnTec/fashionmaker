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


#ifndef VEXCEPTION_H
#define VEXCEPTION_H

#include <QException>

class VException : public QException{
public:
                      VException(const QString &what);
                      VException(const VException &e):what(e.What()){}
    virtual           ~VException() noexcept(true){}
    inline void       raise() const { throw *this; }
    inline VException *clone() const { return new VException(*this); }
    virtual QString   ErrorMessage() const;
    virtual QString   DetailedInformation() const { return QString(); }
    inline QString    What() const {return what;}
protected:
    QString           what;
};

#endif // VEXCEPTION_H
