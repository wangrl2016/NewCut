#include <glog/logging.h>
#include <QApplication>
#include "main_window.h"

int main(int argc, char *argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold = google::INFO;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
