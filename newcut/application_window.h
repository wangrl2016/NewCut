//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_APPLICATION_WINDOW_H
#define NEWCUT_APPLICATION_WINDOW_H

#include <QMdiArea>
#include "newcut/main_window.h"
#include "newcut/engine/enum_collect.h"

namespace nc {
    class ActionHandler;
    class ActionGroupManager;
    class Document;
    class MdiWindow;

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

        QMdiArea const* GetMdiArea() const;

        QMdiArea* GetMdiArea();

        // Pointer to the currently active Mdi window or nullptr if no
        // MdiWindow is active.
        const MdiWindow* GetMdiWindow() const;

        MdiWindow* GetMdiWindow();

    public slots:
        void ShowAboutWindow();     // about dialog


        // Generates a new document for a graphic.
        MdiWindow* SlotFileNew(Document* doc = nullptr);

        void SlotFileOpen(const QString& filename, EnumCollect::FormatType type);

        void SlotFileOpen(const QString& filename); // Assume Unknown type

        void SlotFileOpenRecent(QAction* action);

        void SlotFileSave();

        // Saves a document under a different filename
        void SlotFileSaveAs();

    private:
        bool DoSave(MdiWindow* w, bool force_save_as = false);

        void DoClose(MdiWindow* w, bool activate_next = true);

        void DoActivate(QMdiSubWindow* w);

    private:
        // Pointer to the application window.
        static ApplicationWindow* app_window_;
        QTimer* auto_save_timer_ = nullptr;

        ActionHandler* action_handler_ = nullptr;

        QMdiArea* mdi_area_cad_ = nullptr;
        QMdiSubWindow* active_mdi_sub_window_ = nullptr;
        QMdiSubWindow* current_sub_window_ = nullptr;
    };
}


#endif //NEWCUT_APPLICATION_WINDOW_H
