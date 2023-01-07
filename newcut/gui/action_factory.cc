//
// Created by wangrl2016 on 2022/12/29.
//

#include "newcut/application_window.h"
#include "newcut/gui/action_group_manager.h"
#include "newcut/gui/action_factory.h"
#include "newcut/gui/action_handler.h"

namespace nc {
    ActionFactory::ActionFactory(ApplicationWindow* parent, ActionHandler* handler)
            : QObject(parent),
              using_theme_(false),
              application_window_(parent),
              action_handler_(handler) {}

    void ActionFactory::FillActionContainer(QMap<QString, QAction*>& action_map,
                                            ActionGroupManager* agm) {
        QAction* action;

        // Zoom

        action = new QAction(tr("Zoom &Panning"), agm->other);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotZoomPan()));
        action->setObjectName("ZoomPan");
        action_map["ZoomPan"] = action;

        // Select
        action = new QAction(tr("Select Entity"), agm->select);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotSelectSingle()));
        action->setObjectName("SelectSingle");
        action_map["SelectSingle"] = action;

        action = new QAction(tr("Select Window"), agm->select);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotSelectWindow()));
        action->setObjectName("SelectWindow");
        action_map["SelectWindow"] = action;

        action = new QAction(tr("(De-)Select &Contour"), agm->select);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotSelectContour));
        action->setObjectName("SelectContour");
        action_map["SelectContour"] = action;




        CommonActions(action_map, agm);
    }

    void ActionFactory::CommonActions(QMap<QString, QAction*>& action_map,
                                      ActionGroupManager* agm) {
        QAction* action;

        // Edit

        action = new QAction(tr("&Selection pointer"), agm->edit);
        connect(action, SIGNAL(triggered()),
                action_handler_, SLOT(SlotEditKillAllActions()));
        action->setObjectName("EditKillAllActions");
        action_map["EditKillAllActions"] = action;

        action = new QAction(tr("&Undo"), agm->edit);
        action->setShortcut(QKeySequence::Undo);
        connect(action, SIGNAL(triggered()),
                action_handler_, SLOT(SlotEditUndo()));
        action->setObjectName("EditUndo");
        action_map["EditUndo"] = action;

        action = new QAction(tr("&Redo"), agm->edit);
        action->setShortcut(QKeySequence::Redo);
        connect(action, SIGNAL(triggered()),
                action_handler_, SLOT(SlotEditRedo()));
        action->setObjectName("EditRedo");
        action_map["EditRedo"] = action;

        action = new QAction(tr("Cu&t"), agm->edit);
        action->setShortcut(QKeySequence::Cut);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotEditCut()));
        action->setObjectName("EditCut");
        action_map["EditCut"] = action;

        action = new QAction(tr("&Copy"), agm->edit);
        action->setShortcut(QKeySequence::Copy);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotEditCopy()));
        action->setObjectName("EditCopy");
        action_map["EditCopy"] = action;

        action = new QAction(tr("&Paste"), agm->edit);
        action->setShortcut(QKeySequence::Paste);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotEditPaste()));
        action->setObjectName("EditPaste");
        action_map["EditPaste"] = action;

        // Zoom

        action = new QAction(tr("Zoom &In"), agm->view);
        action->setShortcut(QKeySequence::ZoomIn);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotZoomIn()));
        action->setObjectName("ZoomIn");
        action_map["ZoomIn"] = action;

        action = new QAction(tr("Zoom &Out"), agm->view);
        action->setShortcut(QKeySequence::ZoomOut);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotZoomOut()));
        action->setObjectName("ZoomOut");
        action_map["ZoomOut"] = action;

        action = new QAction(tr("&Auto Zoom"), agm->view);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotZoomAuto()));
        action->setObjectName("ZoomAuto");
        action_map["ZoomAuto"] = action;

        action = new QAction(tr("Previous &View"), agm->view);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotZoomPrevious()));
        action->setEnabled(false);
        action->setObjectName("ZoomPrevious");
        action_map["ZoomPrevious"] = action;

        action = new QAction(tr("&Redraw"), agm->view);
        action->setShortcut(QKeySequence::Refresh);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotZoomReDraw()));
        action->setObjectName("ZoomRedraw");
        action_map["ZoomRedraw"] = action;

        action = new QAction(tr("&Window Zoom"), agm->other);
        action->setCheckable(true);
        connect(action, SIGNAL(triggered()), action_handler_, SLOT(SlotZoomWindow()));
        action->setObjectName("ZoomWindow");
        action_map["ZoomWindow"] = action;

        // File

        action = new QAction(tr("&New"), agm->file);
        action->setShortcut(QKeySequence::New);
        connect(action, SIGNAL(triggered()), application_window_, SLOT(SlotFileNew()));
        action->setObjectName("FileNew");
        action_map["FileNew"] = action;

        action = new QAction(tr("New From &Template"), agm->file);
        connect(action, SIGNAL(triggered()), application_window_, SLOT(SlotFileNewTemplate()));
        action->setObjectName("FileNewTemplate");
        action_map["FileNewTemplate"] = action;

        action = new QAction(tr("&Open..."), agm->file);
        action->setShortcut(QKeySequence::Open);
        connect(action, SIGNAL(triggered()),
                application_window_, SLOT(SlotFileOpen()));
        action->setObjectName("FileOpen");
        action_map["FileOpen"] = action;

        action = new QAction(tr("&Save"), agm->file);
        action->setShortcut(QKeySequence::Save);
        connect(action, SIGNAL(triggered()), application_window_, SLOT(SlotFileSave()));
        action->setObjectName("FileSave");
        action_map["FileSave"] = action;

        action = new QAction(tr("Save &as..."), agm->file);
        action->setShortcut(QKeySequence::SaveAs);
        connect(action, SIGNAL(triggered()), application_window_, SLOT(SlotFileSaveAs()));
        action->setObjectName("FileSaveAs");
        action_map["FileSaveAs"] = action;

        action = new QAction(tr("Save A&ll..."), agm->file);
        connect(action, SIGNAL(triggered()), application_window_, SLOT(SlotFileSaveAll()));
        action->setObjectName("FileSaveAll");
        action_map["FileSaveAll"] = action;

        action = new QAction(tr("&Print..."), agm->file);
        action->setShortcut(QKeySequence::Print);
        connect(action, SIGNAL(triggered()), application_window_, SLOT(SlotFilePrint()));
        action->setObjectName("FilePrint");
        action_map["FilePrint"] = action;

        action = new QAction(tr("Print pre&view"), agm->file);
        connect(action, SIGNAL(triggered()), application_window_, SLOT(SlotFilePrintPreview()));
        action->setObjectName("FilePrintPreview");
        action_map["FilePrintPreview"] = action;

        action = new QAction(tr("&Quit"), agm->file);
        action->setShortcut(QKeySequence::Quit);
        connect(action, SIGNAL(triggered()), application_window_, SLOT(SlotFileeQuit()));
        action->setObjectName("FileQuit");
        action_map["FileQuit"] = action;
    }
}
