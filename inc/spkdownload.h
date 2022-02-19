
#pragma once

#include "spkstore.h"
#include <QTimer>

/**
 * @note SpkDownloadMgr does NOT do download scheduling and other things; it's only a multithreaded
 *       downloader; it manages the threads that are downloading stuff from the Internet.
 *
 *       Because of this, SpkDownloadMgr does not support complex download queues, cannot handle
 *       pauses, and can only work on a sequential list of tasks.
 */

class SpkDownloadMgr : public QObject
{
    Q_OBJECT
  public:
    SpkDownloadMgr(QObject *parent = nullptr);
    ~SpkDownloadMgr();

    static constexpr int MaximumThreadRetryCount = 3;

    enum TaskResult
    {
      Success = 0,
      FailCannotCreateFile, ///< Failed because destination file cannot be created
      FailNoVaibleServer, ///< Failed because no server provides file size or download stalled on
                          ///< all of them
      FailCancel, ///< User has cancelled the task
      Fail
    };

    /**
     * @brief DownloadWorker is not a real worker but more of a worker-like structure. It holds
     *        information about one download thread, such as offset, total bytes and received bytes.
     *        Workers' scheduling is still done by SpkDownloadMgr synchronously, avoiding
     *        unnecessary race conditions and data safety problems.
     *        DownloadWorker is also used in mFailureRetryQueue to indicate the blocks that needed
     *        to be retried on other servers.
     *
     *        Each worker has a watch dog value, incremented each time the download speed is
     *        updated, and zeroed each time the worker has data ready. If the value exceeds a
     *        preset maximum, then this worker is considered timed out and killed.
     */
    struct DownloadWorker
    {
      QNetworkReply *Reply; ///< Reply from the network
      int Watchdog; ///< Watch dog value watching for a timed out worker
      qint64 BeginOffset; ///< Where should a worker start downloading
      qint64 BytesNeeded; ///< How many bytes a worker should fetch in total
      qint64 BytesRecvd; ///< How many bytes a worker has received till now
    };

    constexpr static int WatchDogMaximum = 7;

    struct RemoteFileInfo
    {
      qint64 Size = -1;
      bool SupportPartialDownload = false; ///< Whether this file can be downloaded multithreaded
      QByteArray Md5;
    };

    void SetNewServers(QList<QString> servers);

    /**
     * @note This function uses BLOCKING IO!
     */
    static RemoteFileInfo GetRemoteFileInfo(QUrl url);

    QString GetDestFilePath(QString downloadPath);

  private:
    QList<QString> mServers; ///< Multithreaded download

    QList<DownloadWorker> mScheduledWorkers;

    // If one scheduled task fails a few times in a row, we must give it up on that server and put
    // its responsible block onto this queue so we can try downloading the block from other servers
    QQueue<DownloadWorker> mFailureRetryQueue;

    QFile mDestFile;
    QString mDestFolder, mCurrentRemotePath;
    RemoteFileInfo mCurrentRemoteFileInfo;
    QTimer mProgressEmitterTimer;
    qint64 mDownloadedBytes;

    int mCurrentDownloadId; ///< Indicates download status. -1 means no download going on.
    int mActiveWorkerCount;

    QString mBulkServerPaths; ///< Config string, modification are taken care of by callback.

  public slots:
    void SetDestinationFolder(QString path);

    /**
     * @brief StartNewDownload try to start new download task.
     * @param path File path. Domain name excluded. No leading slashes.
     * @param downloadId Emitted with progress, finish and error so the UI know whose status it is.
     * @return true for success and false for failure.
     */
    bool StartNewDownload(QString path, int downloadId);
    bool PauseCurrentDownload();
    bool CancelCurrentDownload();

  private slots:
    void WorkerFinish();
    void WorkerDownloadProgress(); ///< Be connected to ***QNetworkReply::readyRead***
    void WorkerError(QNetworkReply::NetworkError);
    void ProgressTimer();

  private:
    void ProcessWorkerError(DownloadWorker &, int id);
    void LinkReplyWithMe(QNetworkReply*);
    void TryScheduleFailureRetries();
    void TryScheduleFailureRetries(int i); ///< Try schedule on a specific task slot.

    bool ServerAddressesChangedCallback(); ///< Called by SpkConfig upon address changing

  signals:
    void DownloadProgressed(qint64 bytes, qint64 total, int id);
    void DownloadStopped(TaskResult status, int id);


};
