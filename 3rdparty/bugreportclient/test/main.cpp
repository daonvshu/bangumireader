#include <qapplication.h>

#include "crashlistener.h"
#include "log.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    Log::useQDebugOnly();

    CrashListener crashListener(
        "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJBdXRoZW50aWNhdGlvbiIsImlzcyI"
        "6Ind3dy5lYW50dGVjaC5jb20iLCJuYW1lIjoiYnVncmVwb3J0LXRlc3QifQ.V4HgOtImEXVWYdS_Y6TStap-xBdeFwIciR86X29kYYU"
    );

    for (int i = 0; i < 3000; i++) {
        qDebug() << "qdebug: the number is:" << i;
        Log_d << "log: this number is:" << i;
    }

    int* ptr = nullptr;
    *ptr = 1;

    return a.exec();
}
