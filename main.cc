//
// Created by wangrl2016 on 2022/11/30.
//

// 代码地址：https://github.com/wangrl2016/NewCut

#include <glog/logging.h>
#include <QApplication>
#include "src/view/main_window.h"

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold = google::INFO;

    QApplication app(argc, argv);
    nc::MainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 500);
    mainWindow.show();

    return QApplication::exec();
}
