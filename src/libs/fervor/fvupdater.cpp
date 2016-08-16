/***************************************************************************************************
 **
 **  Copyright (c) 2012 Linas Valiukas and others.
 **
 **  Permission is hereby granted, free of charge, to any person obtaining a copy of this
 **  software and associated documentation files (the "Software"), to deal in the Software
 **  without restriction, including without limitation the rights to use, copy, modify,
 **  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 **  permit persons to whom the Software is furnished to do so, subject to the following conditions:
 **
 **  The above copyright notice and this permission notice shall be included in all copies or
 **  substantial portions of the Software.
 **
 **  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 **  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 **  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 **  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 **  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **
 ******************************************************************************************************/

#include "fvupdater.h"

#include <qsystemdetection.h>
#include <qxmlstream.h>
#include <QApplication>
#include <QByteArray>
#include <QDate>
#include <QDesktopServices>
#include <QLatin1String>
#include <QMessageBox>
#include <QMessageLogger>
#include <QMutex>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QStringList>
#include <QStringRef>
#include <QVariant>
#include <QXmlStreamAttributes>
#include <QtDebug>

#include "../ifc/exception/vexception.h"
#include "../ifc/xml/vabstractconverter.h"
#include "../vmisc/projectversion.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "fvavailableupdate.h"
#include "fvupdatewindow.h"

const QString defaultFeedURL = QStringLiteral("http://valentinaproject.bitbucket.org/Appcast.xml");

QPointer<FvUpdater> FvUpdater::m_Instance;

//---------------------------------------------------------------------------------------------------------------------
FvUpdater* FvUpdater::sharedUpdater()
{
    static QMutex mutex;
    if (m_Instance.isNull())
    {
        mutex.lock();

        if (m_Instance.isNull())
        {
            m_Instance = new FvUpdater;
        }

        mutex.unlock();
    }

    return m_Instance.data();
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    mutex.unlock();
}

//---------------------------------------------------------------------------------------------------------------------
FvUpdater::FvUpdater()
    : QObject(nullptr),
      m_updaterWindow(nullptr),
      m_proposedUpdate(nullptr),
      m_silentAsMuchAsItCouldGet(true),
      m_feedURL(),
      m_qnam(),
      m_reply(nullptr),
      m_httpRequestAborted(false),
      m_xml()
{
    // noop
}

//---------------------------------------------------------------------------------------------------------------------
FvUpdater::~FvUpdater()
{
    hideUpdaterWindow();
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::showUpdaterWindowUpdatedWithCurrentUpdateProposal()
{
    // Destroy window if already exists
    hideUpdaterWindow();

    // Create a new window
    m_updaterWindow = new FvUpdateWindow(qApp->getMainWindow());
    m_updaterWindow->UpdateWindowWithCurrentProposedUpdate();
    m_updaterWindow->show();
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::hideUpdaterWindow()
{
    if (m_updaterWindow)
    {
        m_updaterWindow->close();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::SetFeedURL(const QUrl &feedURL)
{
    m_feedURL = feedURL;
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::SetFeedURL(const QString &feedURL)
{
    SetFeedURL(QUrl(feedURL));
}

//---------------------------------------------------------------------------------------------------------------------
QString FvUpdater::GetFeedURL() const
{
    return m_feedURL.toString();
}

//---------------------------------------------------------------------------------------------------------------------
QPointer<FvAvailableUpdate> FvUpdater::GetProposedUpdate()
{
    return m_proposedUpdate;
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::InstallUpdate()
{
    qDebug() << "Install update";

    UpdateInstallationConfirmed();
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::SkipUpdate()
{
    qDebug() << "Skip update";

    QPointer<FvAvailableUpdate> proposedUpdate = GetProposedUpdate();
    if (proposedUpdate.isNull())
    {
        qWarning() << "Proposed update is NULL (shouldn't be at this point)";
        return;
    }

    // Start ignoring this particular version
    IgnoreVersion(proposedUpdate->GetEnclosureVersion());

    hideUpdaterWindow();
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::RemindMeLater()
{
    qDebug() << "Remind me later";

    qApp->Settings()->SetDateOfLastRemind(QDate::currentDate());

    hideUpdaterWindow();
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::UpdateInstallationConfirmed()
{
    qDebug() << "Confirm update installation";

    QPointer<FvAvailableUpdate> proposedUpdate = GetProposedUpdate();
    if (proposedUpdate.isNull())
    {
        qWarning() << "Proposed update is NULL (shouldn't be at this point)";
        return;
    }

    // Open a link
    if (not QDesktopServices::openUrl(proposedUpdate->GetEnclosureUrl()))
    {
        showErrorDialog(tr("Cannot open your default browser."), true);
        return;
    }

    hideUpdaterWindow();
}

//---------------------------------------------------------------------------------------------------------------------
bool FvUpdater::CheckForUpdates(bool silentAsMuchAsItCouldGet)
{
    if (m_feedURL.isEmpty())
    {
        qCritical() << "Please set feed URL via setFeedURL() before calling CheckForUpdates().";
        return false;
    }

    m_silentAsMuchAsItCouldGet = silentAsMuchAsItCouldGet;

    // Check if application's organization name and domain are set, fail otherwise
    // (nowhere to store QSettings to)
    if (QApplication::organizationName().isEmpty())
    {
        qCritical() << "QApplication::organizationName is not set. Please do that.";
        return false;
    }
    if (QApplication::organizationDomain().isEmpty())
    {
        qCritical() << "QApplication::organizationDomain is not set. Please do that.";
        return false;
    }

    // Set application name / version is not set yet
    if (QApplication::applicationName().isEmpty())
    {
        qCritical() << "QApplication::applicationName is not set. Please do that.";
        return false;
    }

    if (QApplication::applicationVersion().isEmpty())
    {
        qCritical() << "QApplication::applicationVersion is not set. Please do that.";
        return false;
    }

    cancelDownloadFeed();
    m_httpRequestAborted = false;
    startDownloadFeed(m_feedURL);

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool FvUpdater::CheckForUpdatesSilent()
{
    if (qApp->Settings()->GetDateOfLastRemind().daysTo(QDate::currentDate()) >= 1)
    {
        return CheckForUpdates(true);
    }
    else
    {
        return true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool FvUpdater::CheckForUpdatesNotSilent()
{
    return CheckForUpdates(false);
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::startDownloadFeed(const QUrl &url)
{
    m_xml.clear();

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/xml"));
    request.setHeader(QNetworkRequest::UserAgentHeader, QApplication::applicationName());
    request.setUrl(url);

    m_reply = m_qnam.get(request);

    connect(m_reply, &QNetworkReply::readyRead, [this]()
    {
        // this slot gets called every time the QNetworkReply has new data.
        // We read all of its new data and write it into the file.
        // That way we use less RAM than when reading it at the finished()
        // signal of the QNetworkReply
        m_xml.addData(m_reply->readAll());
    });
    connect(m_reply, &QNetworkReply::downloadProgress, [this](qint64 bytesRead, qint64 totalBytes)
    {
        Q_UNUSED(bytesRead);
        Q_UNUSED(totalBytes);

        if (m_httpRequestAborted)
        {
            return;
        }
    });
    connect(m_reply, &QNetworkReply::finished, this, &FvUpdater::httpFeedDownloadFinished);
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::cancelDownloadFeed()
{
    if (m_reply)
    {
        m_httpRequestAborted = true;
        m_reply->abort();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::httpFeedDownloadFinished()
{
    if (m_httpRequestAborted)
    {
        m_reply->deleteLater();
        return;
    }

    const QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (m_reply->error())
    {
        // Error.
        showErrorDialog(tr("Feed download failed: %1.").arg(m_reply->errorString()), false);
    }
    else if (not redirectionTarget.isNull())
    {
        const QUrl newUrl = m_feedURL.resolved(redirectionTarget.toUrl());

        m_feedURL = newUrl;
        m_reply->deleteLater();

        startDownloadFeed(m_feedURL);
        return;
    }
    else
    {
        // Done.
        xmlParseFeed();
    }

    m_reply->deleteLater();
    m_reply = 0;
}

//---------------------------------------------------------------------------------------------------------------------
bool FvUpdater::xmlParseFeed()
{
    QString xmlEnclosureUrl, xmlEnclosureVersion, xmlEnclosurePlatform;

    // Parse
    while (not m_xml.atEnd())
    {
        m_xml.readNext();

        if (m_xml.isStartElement())
        {
            if (m_xml.name() == QLatin1String("item"))
            {
                xmlEnclosureUrl.clear();
                xmlEnclosureVersion.clear();
                xmlEnclosurePlatform.clear();
            }
            else if (m_xml.name() == QLatin1String("enclosure"))
            {
                const QXmlStreamAttributes attribs = m_xml.attributes();
                const QString fervorPlatform = QStringLiteral("fervor:platform");

                if (attribs.hasAttribute(fervorPlatform))
                {
                    if (CurrentlyRunningOnPlatform(attribs.value(fervorPlatform).toString().trimmed()))
                    {
                        xmlEnclosurePlatform = attribs.value(fervorPlatform).toString().trimmed();

                        const QString attributeUrl = QStringLiteral("url");
                        if (attribs.hasAttribute(attributeUrl))
                        {
                            xmlEnclosureUrl = attribs.value(attributeUrl).toString().trimmed();
                        }
                        else
                        {
                            xmlEnclosureUrl.clear();
                        }

                        const QString fervorVersion = QStringLiteral("fervor:version");
                        if (attribs.hasAttribute(fervorVersion))
                        {
                            const QString candidateVersion = attribs.value(fervorVersion).toString().trimmed();
                            if (not candidateVersion.isEmpty())
                            {
                                xmlEnclosureVersion = candidateVersion;
                            }
                        }
                    }
                }
            }
        }
        else if (m_xml.isEndElement())
        {
            if (m_xml.name() == QLatin1String("item"))
            {
                // That's it - we have analyzed a single <item> and we'll stop
                // here (because the topmost is the most recent one, and thus
                // the newest version.

                return searchDownloadedFeedForUpdates(xmlEnclosureUrl,
                                                      xmlEnclosureVersion,
                                                      xmlEnclosurePlatform);
            }
        }

        if (m_xml.error() && m_xml.error() != QXmlStreamReader::PrematureEndOfDocumentError)
        {
            showErrorDialog(tr("Feed parsing failed: %1 %2.").arg(QString::number(m_xml.lineNumber()),
                                                                  m_xml.errorString()), false);
            return false;

        }
    }

    // No updates were found if we're at this point
    // (not a single <item> element found)
    showInformationDialog(tr("No updates were found."), false);

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool FvUpdater::searchDownloadedFeedForUpdates(const QString &xmlEnclosureUrl,
                                               const QString &xmlEnclosureVersion,
                                               const QString &xmlEnclosurePlatform)
{
    qDebug() << "Enclosure URL:" << xmlEnclosureUrl;
    qDebug() << "Enclosure version:" << xmlEnclosureVersion;
    qDebug() << "Enclosure platform:" << xmlEnclosurePlatform;

    // Validate
    if (xmlEnclosureUrl.isEmpty() || xmlEnclosureVersion.isEmpty() || xmlEnclosurePlatform.isEmpty())
    {
        showErrorDialog(tr("Feed error: invalid \"enclosure\" with the download link"), false);
        return false;
    }

    // Relevant version?
    if (VersionIsIgnored(xmlEnclosureVersion))
    {
        qDebug() << "Version '" << xmlEnclosureVersion << "' is ignored, too old or something like that.";

        showInformationDialog(tr("No updates were found."), false);

        return true;	// Things have succeeded when you think of it.
    }

    //
    // Success! At this point, we have found an update that can be proposed
    // to the user.
    //

    if (m_proposedUpdate)
    {
        delete m_proposedUpdate;
    }
    m_proposedUpdate = new FvAvailableUpdate(this);
    m_proposedUpdate->SetEnclosureUrl(xmlEnclosureUrl);
    m_proposedUpdate->SetEnclosureVersion(xmlEnclosureVersion);
    m_proposedUpdate->SetEnclosurePlatform(xmlEnclosurePlatform);

    // Show "look, there's an update" window
    showUpdaterWindowUpdatedWithCurrentUpdateProposal();

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool FvUpdater::VersionIsIgnored(const QString &version)
{
    // We assume that variable 'version' contains either:
    //	1) The current version of the application (ignore)
    //	2) The version that was skipped before and thus stored in QSettings (ignore)
    //	3) A newer version (don't ignore)
    // 'version' is not likely to contain an older version in any case.

    int decVersion = 0x0;
    try
    {
        decVersion = VAbstractConverter::GetVersion(version);
    }
    catch (const VException &e)
    {
        Q_UNUSED(e)
        return true; // Ignore invalid version
    }

    if (decVersion == APP_VERSION)
    {
        return true;
    }

    const int lastSkippedVersion = qApp->Settings()->GetLatestSkippedVersion();
    if (lastSkippedVersion != 0x0)
    {
        if (decVersion == lastSkippedVersion)
        {
            // Implicitly skipped version - skip
            return true;
        }
    }

    if (decVersion > APP_VERSION)
    {
        // Newer version - do not skip
        return false;
    }

    // Fallback - skip
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::IgnoreVersion(const QString &version)
{
    int decVersion = 0x0;
    try
    {
        decVersion = VAbstractConverter::GetVersion(version);
    }
    catch (const VException &e)
    {
        Q_UNUSED(e)
        return ; // Ignore invalid version
    }

    if (decVersion == APP_VERSION)
    {
        // Don't ignore the current version
        return;
    }

    qApp->Settings()->SetLatestSkippedVersion(decVersion);
}

//---------------------------------------------------------------------------------------------------------------------
bool FvUpdater::CurrentlyRunningOnPlatform(const QString &platform)
{
    const QStringList platforms = QStringList() << "Q_OS_LINUX"
                                                << "Q_OS_MAC"
                                                << "Q_OS_WIN32";

    switch (platforms.indexOf(platform.toUpper().trimmed()))
    {
        case 0: // Q_OS_LINUX
#ifdef Q_OS_LINUX // Defined on Linux.
            return true;
#endif
            break;
        case 1: // Q_OS_MAC
#ifdef Q_OS_MAC // Defined on MAC OS (synonym for Darwin).
            return true;
#endif
            break;
        case 2: // Q_OS_WIN32
#ifdef Q_OS_WIN32 // Defined on all supported versions of Windows.
            return true;
#endif
            break;
        default:
            break;
    }

    // Fallback
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::showErrorDialog(const QString &message, bool showEvenInSilentMode)
{
    if (m_silentAsMuchAsItCouldGet)
    {
        if (not showEvenInSilentMode)
        {
            // Don't show errors in the silent mode
            return;
        }
    }

    QMessageBox dlFailedMsgBox;
    dlFailedMsgBox.setIcon(QMessageBox::Critical);
    dlFailedMsgBox.setText(tr("Error"));
    dlFailedMsgBox.setInformativeText(message);
    dlFailedMsgBox.exec();
}

//---------------------------------------------------------------------------------------------------------------------
void FvUpdater::showInformationDialog(const QString &message, bool showEvenInSilentMode)
{
    if (m_silentAsMuchAsItCouldGet)
    {
        if (not showEvenInSilentMode)
        {
            // Don't show information dialogs in the silent mode
            return;
        }
    }

    QMessageBox dlInformationMsgBox;
    dlInformationMsgBox.setIcon(QMessageBox::Information);
    dlInformationMsgBox.setText(tr("Information"));
    dlInformationMsgBox.setInformativeText(message);
    dlInformationMsgBox.exec();
}
