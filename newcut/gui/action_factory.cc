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
                                            ActionGroupManager* action_group_manager) {
        QAction* action;

        CommonActions(action_map, action_group_manager);
    }

    void ActionFactory::CommonActions(QMap<QString, QAction*>& action_map,
                                      ActionGroupManager* action_group_manager) {
        QAction* action;

        // Edit

        action = new QAction(tr("&Selection pointer"), action_group_manager->edit_);
        connect(action, SIGNAL(triggered()),
                action_handler_, SLOT(SlotEditKillAllActions()));
        action->setObjectName("EditKillAllActions");
        action_map["EditKillAllActions"] = action;

        action = new QAction(tr("&Undo"), action_group_manager->edit_);
        action->setShortcut(QKeySequence::Undo);
        connect(action, SIGNAL(triggered()),
                action_handler_, SLOT(SlotEditUndo()));
        action->setObjectName("EditUndo");
        action_map["EditUndo"] = action;

        action = new QAction(tr("&Redo"), action_group_manager->edit_);
        action->setShortcut(QKeySequence::Redo);
        connect(action, SIGNAL(triggered()),
                action_handler_, SLOT(SlotEditRedo()));
        action->setObjectName("EditRedo");
        action_map["EditRedo"] = action;

        action = new QAction(tr("&New"), action_group_manager->file_);
        action->setObjectName("FileNew");
        action_map["FileNew"] = action;

        action = new QAction(tr("New From &Template"), action_group_manager->file_);
        action->setObjectName("FileNewTemplate");
        action_map["FileNewTemplate"] = action;

        action = new QAction(tr("&Open..."), action_group_manager->file_);
        action->setShortcut(QKeySequence::Open);
        connect(action, SIGNAL(triggered()),
                application_window_, SLOT(SlotFileOpen()));
        action->setObjectName("FileOpen");
        action_map["FileOpen"] = action;

        action = new QAction(tr("&Save"), action_group_manager->file_);
        action->setObjectName("FileSave");
        action_map["FileSave"] = action;

        action = new QAction(tr("Save &as..."), action_group_manager->file_);
        action->setObjectName("FileSaveAs");
        action_map["FileSaveAs"] = action;

        action = new QAction(tr("Save A&ll..."), action_group_manager->file_);
        action->setObjectName("FileSaveAll");
        action_map["FileSaveAll"] = action;
    }
}
