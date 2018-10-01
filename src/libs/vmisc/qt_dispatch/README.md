Qt-Dispatch
===========

Execute block on the main thread or specified thread with Qt framework. 

No required SIGNAL/SLOTs, no required QObject::connect...

Use with functions or C++11 Lamdas 

Functions
------------

```cpp
static void q_dispatch_async_main(fnBlock block); // Execute on Main / UI thread.
static void q_dispatch_async(QThread* thread, fnBlock block); // Execute on some thread.
```

SAMPLE USAGE
------------

```cpp

#include "qt_dispatch.h"

// Usage 1: Using with lambdas

// This function called from non-ui/background thread.
void MainWindow::handleSockData(void *obj) {
  // This usage requires C++11 
  q_dispatch_async_main([&]() {
     // Add widgets to window using ui object. 
     SomeWidget *widget = new SomeWidget();
     ui->someVLayout->addWidget(widget);
     
     // check the thread id
     const bool isGuiThread = QThread::currentThread() ==
                                    QCoreApplication::instance()->thread();
     qWarning() << "isGuiThread: " << isGuiThread; // true
  });
}

// Usage 2: Using with functions

void MainWindow::handleSockData(void *obj{
  void somefunc();
  q_dispatch_async_main(somefunc);
}

void somefunc() {
  const bool isGuiThread = QThread::currentThread() ==
                                    QCoreApplication::instance()->thread();
  qWarning() << "isGuiThread: " << isGuiThread; // true
}


// Usage 3: Using with other threads

void MainWindow::on_action1_triggered() {
  QThread *newThread = new QThread();
  newThread->start();

  q_dispatch_async(newThread, []() {
    const bool isGuiThread = QThread::currentThread() ==
                                    QCoreApplication::instance()->thread();
    qWarning() << "isGuiThread: " << isGuiThread; // false
  });
}
```

Enabling C++11 Compiler Support: Add these lines to your .pro file

```
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
LIBS += -stdlib=libc++ -mmacosx-version-min=10.7
```

###Enjoy!
