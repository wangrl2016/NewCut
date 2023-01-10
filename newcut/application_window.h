//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_APPLICATION_WINDOW_H
#define NEWCUT_APPLICATION_WINDOW_H

#include "newcut/main_window.h"
#include "newcut/engine/enum_collect.h"

namespace nc {
    class ActionHandler;
    class ActionGroupManager;
    class MDIWindow;

    class ApplicationWindow : public MainWindow {
        Q_OBJECT
    public:
        ApplicationWindow();

        ~ApplicationWindow();

        QMap<QString, QAction*> action_map_;
        ActionGroupManager* action_group_manager_;

        static ApplicationWindow* GetAppWindow() {
            return app_window_;
        }

    public slots:
        void ShowAboutWindow();     // about dialog


        void SlotFileOpen(const QString& filename, EnumCollect::FormatType type);

        void SlotFileOpen(const QString& filename); // Assume Unknown type

        void SlotFileOpenRecent(QAction* action);

        void SlotFileSave();

        // Saves a document under a different filename
        void SlotFileSaveAs();

    private:
        bool DoSave(MDIWindow* w, bool force_save_as = false);

    private:
        // Pointer to the application window.
        static ApplicationWindow* app_window_;
        QTimer* auto_save_timer_ = nullptr;

        ActionHandler* action_handler_ = nullptr;
    };
}


#endif //NEWCUT_APPLICATION_WINDOW_H
