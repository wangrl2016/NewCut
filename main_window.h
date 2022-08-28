#ifndef NEW_CUT_MAIN_WINDOW_H
#define NEW_CUT_MAIN_WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow* ui;
};

#endif // NEW_CUT_MAIN_WINDOW_H
