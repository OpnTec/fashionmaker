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

#ifndef FVUPDATER_H
#define FVUPDATER_H

#include <QObject>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QXmlStreamReader>
#include <QPointer>

#include "fvupdatewindow.h"
#include "fvavailableupdate.h"

class FvUpdater : public QObject
{
    Q_OBJECT

public:
    // Singleton
    static FvUpdater* sharedUpdater();
    static void drop();

    // Set / get feed URL
    void SetFeedURL(const QUrl &feedURL);
    void SetFeedURL(const QString &feedURL);
    QString GetFeedURL() const;

public slots:
    // Check for updates
    bool CheckForUpdates(bool silentAsMuchAsItCouldGet = true);

    // Aliases
    bool CheckForUpdatesSilent();
    bool CheckForUpdatesNotSilent();

protected:
    friend class FvUpdateWindow; // Uses GetProposedUpdate() and others
    QPointer<FvAvailableUpdate> GetProposedUpdate();

protected slots:
    // Update window button slots
    void InstallUpdate();
    void SkipUpdate();
    void RemindMeLater();

    // Update confirmation dialog button slots
    void UpdateInstallationConfirmed();

private slots:
    void httpFeedReadyRead();
    void httpFeedUpdateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void httpFeedDownloadFinished();

private:
    //
    // Singleton business
    //
    Q_DISABLE_COPY(FvUpdater)
    FvUpdater();		  // Hide main constructor
    virtual ~FvUpdater(); // Hide main destructor

    static QPointer<FvUpdater> m_Instance; // Singleton instance

    QPointer<FvUpdateWindow> m_updaterWindow; // Updater window (NULL if not shown)

    // Available update (NULL if not fetched)
    QPointer<FvAvailableUpdate> m_proposedUpdate;

    // If true, don't show the error dialogs and the "no updates." dialog
    // (silentAsMuchAsItCouldGet from CheckForUpdates() goes here)
    // Useful for automatic update checking upon application startup.
    bool m_silentAsMuchAsItCouldGet;

    //
    // HTTP feed fetcher infrastructure
    //
    QUrl                  m_feedURL;		    // Feed URL that will be fetched
    QNetworkAccessManager m_qnam;
    QNetworkReply*        m_reply;
    bool                  m_httpRequestAborted;

    QXmlStreamReader m_xml;	// XML data collector and parser

    void showUpdaterWindowUpdatedWithCurrentUpdateProposal();		// Show updater window
    void hideUpdaterWindow();										// Hide + destroy m_updaterWindow

    void startDownloadFeed(const QUrl &url); // Start downloading feed
    void cancelDownloadFeed();			     // Stop downloading the current feed

    // Dialogs (notifications)
    // Show an error message
    void showErrorDialog(const QString &message, bool showEvenInSilentMode = false);
    // Show an informational message
    void showInformationDialog(const QString &message, bool showEvenInSilentMode = false);


    bool xmlParseFeed();				// Parse feed in m_xml
    bool searchDownloadedFeedForUpdates(const QString &xmlEnclosureUrl,
                                        const QString &xmlEnclosureVersion,
                                        const QString &xmlEnclosurePlatform);

    static bool VersionIsIgnored(const QString &version);
    static void IgnoreVersion(const QString &version);
    static bool CurrentlyRunningOnPlatform(const QString &platform);
};

#endif // FVUPDATER_H
