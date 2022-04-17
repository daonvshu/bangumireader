#include <qapplication.h>
#include <qicon.h>

#include <qsystemtrayicon.h>
#include <qdesktopwidget.h>

#include "qfonticon.h"

#include "mikanbangumireader.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    QIcon icon(":/log.ico");
    a.setWindowIcon(icon);

    //Material Design Icons
    QFontIcon::addFont(":/materialdesignicons-webfont.ttf");

    MikanBangumiReader mikanBangumiReader(a.desktop());
    mikanBangumiReader.show();

    return a.exec();
}

