/************************************************************************
 **
 **  @file   VLockGuard.h
 **  @author Alex Zaharov <alexzkhr@gmail.com>
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 9, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VLOCKGUARD_H
#define VLOCKGUARD_H

#include <QString>
#include <stdint.h>
#include <memory>

#include "../vmisc/diagnostic.h"

#include <QFileInfo>
#include <QLockFile>
#if defined(Q_OS_WIN)
#include <windows.h>
#endif

/*@brief
 * This class creates Guarded object if and only if lock file taken. It keeps shared_ptr to object and lock-file.
 * Can use optional object allocator and deleter.
 *
 * On older Qt lock assumed always taken and compile-time warning is shown.
 *
*/
template <typename Guarded>
class VLockGuard
{
public:
    explicit VLockGuard(const QString& lockName, int stale = 0, int timeout = 0);

    template <typename Alloc>
    VLockGuard(const QString& lockName, Alloc a, int stale = 0, int timeout=0);

    template <typename Alloc, typename Delete>
    VLockGuard(const QString& lockName, Alloc a, Delete d, int stale = 0, int timeout=0);

    const std::shared_ptr<Guarded> &GetProtected() const;
    int             GetLockError() const;
    bool            IsLocked() const;
    QString         GetLockFile() const;

private:
    Q_DISABLE_COPY(VLockGuard<Guarded>)

    std::shared_ptr<Guarded> holder;
    int                      lockError;
    QString                  lockFile;
    std::shared_ptr<QLockFile> lock;

    // cppcheck-suppress functionStatic
    bool TryLock(const QString &lockName, int stale, int timeout);
};

//---------------------------------------------------------------------------------------------------------------------
template <typename Guarded>
VLockGuard<Guarded>::VLockGuard(const QString &lockName, int stale, int timeout)
    : holder(nullptr), lockError(0), lockFile(), lock(nullptr)
{
    if (TryLock(lockName, stale, timeout))
    {
        holder.reset(new Guarded());
    }
}

//---------------------------------------------------------------------------------------------------------------------
//using allocator lambdas seems logically better than supplying pointer, because we will take ownership of allocated
//object
template <typename Guarded> template <typename Alloc>
VLockGuard<Guarded>::VLockGuard(const QString& lockName, Alloc a, int stale, int timeout)
    : holder(nullptr), lockError(0), lockFile(), lock(nullptr)
{
    if (TryLock(lockName, stale, timeout))
    {
        holder.reset(a());
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Guarded> template <typename Alloc, typename Delete>
VLockGuard<Guarded>::VLockGuard(const QString& lockName, Alloc a, Delete d, int stale, int timeout)
    : holder(nullptr), lockError(0), lockFile(), lock(nullptr)
{
    if (TryLock(lockName, stale, timeout))
    {
        holder.reset(a(), d);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Guarded>
const std::shared_ptr<Guarded> &VLockGuard<Guarded>::GetProtected() const
{
    return holder;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Guarded>
int VLockGuard<Guarded>::GetLockError() const
{
    return lockError;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Guarded>
bool VLockGuard<Guarded>::IsLocked() const
{
    return holder != nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Guarded>
QString VLockGuard<Guarded>::GetLockFile() const
{
    return lockFile;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Guarded>
bool VLockGuard<Guarded>::TryLock(const QString &lockName, int stale, int timeout)
{
    bool res = true;

    lockFile = lockName + QLatin1String(".lock");
#if defined(Q_OS_UNIX)
    QFileInfo info(lockFile);
    lockFile = info.absolutePath() + QLatin1String("/.") + info.fileName();
#endif
    lock.reset(new QLockFile(lockFile));

    lock->setStaleLockTime(stale);
    lock->tryLock(timeout);

    if (QLockFile::LockFailedError == lock->error())
    {
        // This happens if a stale lock file exists and another process uses that PID.
        // Try removing the stale file, which will fail if a real process is holding a
        // file-level lock. A false error is more problematic than not locking properly
        // on corner-case systems.
        lock->removeStaleLockFile();
        lock->tryLock(timeout);
    }
    res = QLockFile::NoError == (lockError = lock->error());
    if (!res)
    {
        lock.reset();
    }
#if defined(Q_OS_WIN)
    else
    {
        SetFileAttributesW(lockFile.toStdWString().c_str(), FILE_ATTRIBUTE_HIDDEN);
    }
#endif
    return res;
}

//use pointer and function below to persistent things like class-member, because lock is taken by constructor
//helper functions allow to write shorter creating and setting new lock-pointer

QT_WARNING_PUSH
QT_WARNING_DISABLE_INTEL(1418)

template <typename Guarded>
void VlpCreateLock(std::shared_ptr<VLockGuard<Guarded>>& r, const QString& lockName, int stale = 0, int timeout = 0)
{
    r.reset(new VLockGuard<Guarded>(lockName, stale, timeout));
}

template <typename Guarded, typename Alloc>
void VlpCreateLock(std::shared_ptr<VLockGuard<Guarded>>& r, const QString& lockName, Alloc a, int stale = 0,
                   int timeout = 0)
{
    r.reset(new VLockGuard<Guarded>(lockName, a, stale, timeout));
}

template <typename Guarded, typename Alloc, typename Del>
void VlpCreateLock(std::shared_ptr<VLockGuard<Guarded>>& r, const QString& lockName, Alloc a, Del d, int stale = 0,
                   int timeout = 0)
{
    r.reset(new VLockGuard<Guarded>(lockName, a, d, stale, timeout));
}

QT_WARNING_POP

#endif // VLOCKGUARD_H
