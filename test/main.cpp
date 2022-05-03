#include <qcoreapplication.h>
#include <qtextstream.h>
#include <qdebug.h>
#include <qdatetime.h>

#include "../src/utils/textutil.h"

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    QDateTime dateTime = QDateTime::fromString("2016-03-26T10:00:00.000Z", "yyyy-MM-ddTHH:mm:ss.zzzZ");
    qDebug() << dateTime.toMSecsSinceEpoch();

    //test keywords parse
    QTextStream qin(stdin);
    QString tempStr;
    while (true) {
        try {
            qDebug() << "wait for input:";
            qin >> tempStr;
            auto result = TextUtils::filterByKeywords<QString>(tempStr, {"ABCDEFG", "CDEFGHIJ"}, [&](const QString& e) {
                return e;
            });
            qDebug() << "filter result:" << result;
        }
        catch (TextParseException&) {
            qDebug() << "keywords parse error!";
        }
    }

    return a.exec();
}
