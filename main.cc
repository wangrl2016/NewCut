//
// Created by wangrl2016 on 2022/12/21.
//

#include <glog/logging.h>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include "view/main_window.h"

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold = google::INFO;

    QApplication app(argc, argv);

    nc::MainWindow window;
    QRect screen_rect = QGuiApplication::primaryScreen()->geometry();
    int screen_width = screen_rect.width();
    int screen_height = screen_rect.height();
    // 设置左右上下边缘为1/8的宽和高
    window.setGeometry(screen_width / 8,
                       screen_height / 8,
                       screen_width / 4 * 3,
                       screen_height / 4 * 3);
    LOG(INFO) << "Main window geometry (" << screen_width / 8
              << ", " << screen_height / 8
              << ", " << screen_width / 4 * 3
              << ", " << screen_height / 4 * 3 << ")";
    window.show();

    return QApplication::exec();
}
