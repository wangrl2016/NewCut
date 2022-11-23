#ifndef NEW_CUT_MAIN_WINDOW_H
#define NEW_CUT_MAIN_WINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace nc {
    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow();

        void LoadFile(const QString& filename);

    protected:
        void closeEvent(QCloseEvent* event) override;

    private slots:

        void NewFile();

        void Open();

        void Save();

        void SaveAs();

        void About();

        void DocumentWasModified();

    private:
        void CreateActions();

        void CreateStatusBar();

        void ReadSettings();

        void WriteSettings();

        bool MaybeSave();

        bool SaveFile(const QString& filename);

        void SetCurrentFile(const QString& filename);

        QString StrippedName(const QString& full_filename);

    private:
        QPlainTextEdit* text_edit_;

        QString cur_file_;
    };
}

#endif // NEW_CUT_MAIN_WINDOW_H
