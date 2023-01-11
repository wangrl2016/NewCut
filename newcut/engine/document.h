//
// Created by wr on 2023/1/8.
//

#ifndef NEWCUT_DOCUMENT_H
#define NEWCUT_DOCUMENT_H

#include <QString>
#include "newcut/engine/entity_container.h"
#include "newcut/engine/enum_collect.h"
#include "newcut/engine/pen.h"
#include "newcut/gui/graphic_view.h"

namespace nc {
    // Base class for documents. Documents can be either graphics or
    // blocks and are typically shown in graphic views. Documents hold
    // an active pen for drawing in the Document, a file name, and they
    // know whether they have been modified or not.
    class Document : public EntityContainer {
    public:
        virtual bool IsModified() const {
            return modified_;
        }

        virtual QString GetFilename() const {
            return filename_;
        }

    protected:
        // Flag set if the document was modified and not yet saved.
        bool modified_;

        // Active pen.
        Pen active_pen;

        // File name of the document or empty for a new document.
        QString filename_;

        // Auto-save file name of document.
        QString auto_save_filename_;

        EnumCollect::FormatType format_type_;

        // Used to read/save current view.
        GraphicView* graphic_view_;
    };
}

#endif //NEWCUT_DOCUMENT_H
