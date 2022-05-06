#include <qapplication.h>
#include <qicon.h>
#include <qfont.h>

#include "bugreport.h"

int main(int argc, char* argv[]) {

    if (argc != 5) return -1;

    QApplication a(argc, argv);
    QFont font = a.font();
    font.setFamily("Microsoft YaHei UI");
    a.setFont(font);
    a.setWindowIcon(QIcon(":/logo.ico"));

    BugReport w;
    w.show();

    return a.exec();
}
