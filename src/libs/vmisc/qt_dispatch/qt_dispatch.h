//
//  Created by Recep ASLANTAS
//  Copyright (c) 2013 Recep ASLANTAS. All rights reserved.
//

#ifndef THREAD_DISPATCHER_H
#define THREAD_DISPATCHER_H

#include <QThread>
#include <QMetaObject>
#include <QThread>
#include <QCoreApplication>
#include <QObject>

#include <functional>

typedef std::function<void()> voidBlock;

class WorkerClass : public QObject
{
  Q_OBJECT

public:
    WorkerClass(QThread *thread)
    {
        moveToThread(thread);
        connect(QThread::currentThread(), &QThread::finished, this, &WorkerClass::deleteLater);
    }
public slots:
    void DoWork(voidBlock block)
    {
        block();
        deleteLater();
    }
};

static void q_dispatch_async(QThread* thread, voidBlock block)
{
  qRegisterMetaType<voidBlock>("voidBlock");

  WorkerClass *worker = new WorkerClass(thread);
  QMetaObject::invokeMethod(worker, "DoWork", Qt::QueuedConnection, Q_ARG(voidBlock, block));
}

static void q_dispatch_async_main(voidBlock block)
{
  QThread *mainThread = QCoreApplication::instance()->thread();
  q_dispatch_async(mainThread, block);
}

typedef std::function<void(QtMsgType, const QMessageLogContext &, const QString &)> msgHandlerBlock;

class MsgHandlerWorkerClass : public QObject
{
  Q_OBJECT

public:
    MsgHandlerWorkerClass(QThread *thread, QtMsgType type, const QMessageLogContext &context, const QString &msg)
        : m_type(type),
          m_msg(msg),
          m_line(context.line),
          m_file(context.file),
          m_function(context.function),
          m_category(context.category)
    {
        moveToThread(thread);
        connect(QThread::currentThread(), &QThread::finished, this, &WorkerClass::deleteLater);
    }
public slots:
    void DoWork(msgHandlerBlock block)
    {
        block(m_type, QMessageLogContext(qUtf8Printable(m_file), m_line, qUtf8Printable(m_function),
                                         qUtf8Printable(m_category)), m_msg);
        deleteLater();
    }
private:
    QtMsgType m_type;
    QString m_msg;

    // We cannot make copy of QMessageLogContext. So, we must save its data instead and recreate it later.
    int m_line;
    QString m_file;
    QString m_function;
    QString m_category;
};

static void q_dispatch_async(QThread* thread, msgHandlerBlock block, QtMsgType type, const QMessageLogContext &context,
                             const QString &msg)
{
  qRegisterMetaType<msgHandlerBlock>("msgHandlerBlock");

  MsgHandlerWorkerClass *worker = new MsgHandlerWorkerClass(thread, type, context, msg);
  QMetaObject::invokeMethod(worker, "DoWork", Qt::QueuedConnection, Q_ARG(msgHandlerBlock, block));
}

static void q_dispatch_async_main(msgHandlerBlock block, QtMsgType type, const QMessageLogContext &context,
                                  const QString &msg)
{
    QThread *mainThread = QCoreApplication::instance()->thread();
    q_dispatch_async(mainThread, block, type, context, msg);
}

#endif // THREAD_DISPATCHER_H
