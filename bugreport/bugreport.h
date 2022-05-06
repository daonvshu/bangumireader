#pragma once

#include <qwidget.h>

#include "ui_bugreport.h"

struct CrashInfo;
class BugReport : public QWidget {
    Q_OBJECT

public:
    explicit BugReport(QWidget* parent = nullptr);
    ~BugReport();

private:
    Ui::BugReport ui;
    CrashInfo* crashInfo;

public slots:
    void on_btn_close_clicked();
    void on_btn_open_clicked();
};