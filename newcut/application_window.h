//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_APPLICATION_WINDOW_H
#define NEWCUT_APPLICATION_WINDOW_H

#include "newcut/main_window.h"

namespace nc {
    class ActionHandler;
    class ActionGroupManager;

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


        void SlotFileOpen();

    private:
        // Pointer to the application window.
        static ApplicationWindow* app_window_;
        QTimer* auto_save_timer_ = nullptr;

        ActionHandler* action_handler_ = nullptr;
    };

#if defined(_WIN32)
    extern int qt_ntfs_permission_lookup;
#endif
}


#endif //NEWCUT_APPLICATION_WINDOW_H
