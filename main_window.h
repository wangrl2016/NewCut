#ifndef NEW_CUT_MAIN_WINDOW_H
#define NEW_CUT_MAIN_WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace nc {
    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow();

        void LoadFile(const QString& filename);

    private:


    };
}

#endif // NEW_CUT_MAIN_WINDOW_H
