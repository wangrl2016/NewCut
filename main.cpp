#include <glog/logging.h>
#include <QApplication>
#include <QCommandLineParser>
#include "main_window.h"

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold = google::INFO;

    // Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtPorject");
    QCoreApplication::setApplicationName("NewCut");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    nc::MainWindow main_window;
    if (!parser.positionalArguments().isEmpty())
        main_window.LoadFile(parser.positionalArguments().first());
    main_window.show();
    return QApplication::exec();
}
