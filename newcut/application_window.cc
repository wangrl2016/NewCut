//
// Created by wangrl2016 on 2022/12/28.
//

#include <glog/logging.h>
#include <QDialog>
#include <QSettings>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QApplication>
#include <QLabel>
#include <QTabBar>
#include "user_config.h"
#include "newcut/application_window.h"
#include "newcut/engine/document.h"
#include "newcut/gui/action_factory.h"
#include "newcut/gui/action_handler.h"
#include "newcut/gui/action_group_manager.h"
#include "newcut/gui/mdi_window.h"
#include "newcut/gui/widget_factory.h"
#include "newcut/gui/central_widget.h"

#ifndef APP_ICON
#define APP_ICON    ":/images/newcut/app_icon.png"
#endif

namespace nc {
    ApplicationWindow* ApplicationWindow::app_window_ = nullptr;

    ApplicationWindow::ApplicationWindow()
            : action_group_manager_(new ActionGroupManager(this)),
              auto_save_timer_(nullptr),
              action_handler_(new ActionHandler(this)) {
        LOG(INFO) << __FUNCTION__;

        // accept drop events to open files
        setAcceptDrops(true);

        // make the left and right dock areas dominant
        setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
        setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

        app_window_ = this;

        QSettings settings;

        LOG(INFO) << __FUNCTION__ << " setting icon";
        setWindowIcon(QIcon(APP_ICON));

        // QStatusBar* status_bar = statusBar();

        LOG(INFO) << __FUNCTION__ << " creating CentralWidget";
        auto central = new CentralWidget(this);

        setCentralWidget(central);

        mdi_area_cad_ = central->GetMdiArea();
        mdi_area_cad_->setDocumentMode(true);

        mdi_area_cad_->setViewMode(QMdiArea::TabbedView);
        QList<QTabBar*> tab_bar_list = mdi_area_cad_->findChildren<QTabBar*>();
        QTabBar* tab_bar = tab_bar_list.at(0);
        if (tab_bar) {
            tab_bar->setExpanding(false);
        }


        ActionFactory action_factory(this, action_handler_);
        action_factory.using_theme_ = settings.value("Widgets/AllowTheme", 0).toBool();
        action_factory.FillActionContainer(action_map_, action_group_manager_);

        WidgetFactory widget_factory(this, action_map_, action_group_manager_);

        widget_factory.CreateStandardToolbars(action_handler_);

        widget_factory.CreateMenus(menuBar());
    }

    ApplicationWindow::~ApplicationWindow() {

    }

    QMdiArea const* ApplicationWindow::GetMdiArea() const {
        return mdi_area_cad_;
    }

    QMdiArea* ApplicationWindow::GetMdiArea() {
        return mdi_area_cad_;
    }

    MdiWindow const* ApplicationWindow::GetMdiWindow() const {
        if (mdi_area_cad_) {
            auto w= mdi_area_cad_->currentSubWindow();
            if (w) {
                return qobject_cast<MdiWindow*>(w);
            }
        }
        LOG(INFO) << __FUNCTION__ << " is nullptr";
        return nullptr;
    }

    MdiWindow* ApplicationWindow::GetMdiWindow() {
        if (mdi_area_cad_) {
            auto w = mdi_area_cad_->currentSubWindow();
            if (w) {
                return qobject_cast<MdiWindow*>(w);
            } else {
                LOG(WARNING) << "QMdiSubWindow is nullptr";
            }
        }
        LOG(INFO) << __FUNCTION__ << " is nullptr";
        return nullptr;
    }

    void ApplicationWindow::ShowAboutWindow() {
        QDialog dialog;
        dialog.setWindowTitle(tr("About"));

        auto layout = new QVBoxLayout;
        dialog.setLayout(layout);

        auto frame = new QGroupBox(qApp->applicationName());
        layout->addWidget(frame);

        auto f_layout = new QVBoxLayout;
        frame->setLayout(f_layout);

        QString info(
                tr("Version: %1.%2.%3").arg(NewCut_VERSION_MAJOR)
                        .arg(NewCut_VERSION_MINOR)
                        .arg(NewCut_VERSION_PATCH) + "\n" +
                #if defined(Q_CC_CLANG)
                tr("Compiler: Clang %1.%2.%3").arg(__clang_major__)
                        .arg(__clang_minor__)
                        .arg(__clang_patchlevel__) + "\n" +
                #elif defined(Q_CC_GNU)
                tr("Compiler: GNU GCC %1.%2.%3").arg(__GNUC__)
                        .arg(__GNUC_MIN_OR)
                        .arg(__GNUC_PATCHLEVEL__) + "\n" +
                #elif defined(Q_CC_MSVC)
                tr("Compiler: Microsoft Visual C++") + "\n" +
                #endif
                tr("Compiled on: %1").arg(__DATE__) + "\n" +
                tr("Qt Version: %1").arg(qVersion())
        );
        auto app_info = new QLabel(info);
        app_info->setTextInteractionFlags(Qt::TextSelectableByMouse);
        f_layout->addWidget(app_info);

        dialog.exec();
    }

    MdiWindow* ApplicationWindow::SlotFileNew(Document* doc) {
        LOG(INFO) << __FUNCTION__;

        auto w = new MdiWindow(doc, mdi_area_cad_, Qt::WindowType::Widget);

        mdi_area_cad_->addSubWindow(w);

        return w;
    }

    void ApplicationWindow::SlotFileOpen(const QString& filename,
                                         EnumCollect::FormatType type) {

    }

    void ApplicationWindow::SlotFileOpen(const QString& filename) {

    }

    void ApplicationWindow::SlotFileOpenRecent(QAction* action) {

    }

    void ApplicationWindow::SlotFileSave() {

    }

    void ApplicationWindow::SlotFileSaveAs() {
        LOG(INFO) << __FUNCTION__;
        if (DoSave(GetMdiWindow(), true)) {

        }
    }

    // Force-Save(as) the content of the sub window. Retry on failure.
    // return true success (or window was not modified).
    // return false user cancelled (or window was null).
    bool ApplicationWindow::DoSave(MdiWindow* w, bool force_save_as) {
        LOG(INFO) << __FUNCTION__;
        QString name, msg;
        if (!w) { return false; }
        if (w->GetDocument()->IsModified() || force_save_as) {
            name = w->GetDocument()->GetFilename();
            if (name.isEmpty()) {
                DoActivate(w);  // show the user the drawing for save as
            }
        }
        return true;
    }

    void ApplicationWindow::DoClose(MdiWindow* w, bool activate_next) {

    }

    void ApplicationWindow::DoActivate(QMdiSubWindow* w) {
        LOG(INFO) << __FUNCTION__;
        if (w) {
            // SlotWindowActivated(w);
            w->activateWindow();
            w->raise();
            w->setFocus();

            w->show();
        }
    }


}
