//
// Created by wr on 2022/12/27.
//

#ifndef NEWCUT_ENUM_CLASS_H
#define NEWCUT_ENUM_CLASS_H

namespace nc {
    class EnumCollect {
    public:
        // Entity types returned by the Rtti() method
        enum EntityType {
            kEntityUnknown,
            kEntityContainer,
            kEntityBlock,
            kEntityFontChar,
            kEntityInsert,
            kEntityGraphic,
            kEntityPoint,
            kEntityLine,
            kEntityPolyline,
            kEntityVertex,
            kEntityArc,
            kEntityCircle,
            kEntityEllipse,
            kEntityHyperbola,
            kEntitySolid,
            KEntityConstructionLine,
            kEntityMText,
            kEntityText,
            kEntityDimAligned,
            kEntityDimLinear,
            kEntityDimRadial,
            kEntityDimDiametric,
            kEntityDimAngular,
            kEntityDimLeader,
            kEntityHatch,
            kEntityImage,
            kEntitySpline,
            kEntitySplinePoints,
            kEntityOverlayBox,
            kEntityPreview,
            kEntityPattern,
            kEntityOverlayLine
        };

        // Action types used by action factories
        enum ActionType {
            kActionNone,
            kActionDefault,

            kActionFileNew,
            kActionFileNewTemplate,
            kActionFileOpen,
            kActionFileSave,
            kActionFileSaveAs,
            kActionFileExport,
            kActionFileClose,
            kActionFilePrint,
            kActionFilePrintPDF,
            kActionFilePrintPreview,
            kActionFileExportMakerCam,
            kActionFileQuit,

            kActionEditKillAllActions,
            kActionEditUndo,
            kActionEditRedo,
            kActionEditCut,
            kActionEditCutNoSelect,
            kActionEditPaste,
            kActionOrderNoSelect,
            kActionOrderBottom,
            kActionOrderLower,
            kActionOrderRaise,
            kActionOrderTop,

            kActionViewStatusBar,
            kActionViewLayerList,
            kActionViewBlockList,
            kActionViewCommandLine,
            kActionViewLibrary,

            kActionViewPenToolbar,
            kActionViewOptionToolbar,
            kActionViewCadToolbar,
            kActionViewFileToolbar,
            kActionViewEditToolbar,
            kActionViewEditSnapToolbar,

            kActionViewGrid,
            kActionViewDraft,

            kActionZoomIn,
            kActionZoomOut,
            kActionZoomAuto,
            kActionZoomWindow,
            kActionZoomPan,
            kActionZoomRedraw,
            kActionZoomPrevious,

            kActionSelect,
            kAction
        };

        enum FormatType {
            kFormatUnknown,     // unknown / unsupported format
        };

        enum SnapRestriction {
            kRestrictNothing,        // No restriction to snap mode
            kRestrictHorizontal,     // Restrict to 0, 180 degrees
            kRestrictVertical,       // Restrict to 90, 270 degrees
            kRestrictOrthogonal      // Restrict to 90, 180, 270, 0 degrees
        };

        // Undoable Rtti.
        enum UndoableType {
            kUndoableUnknown,
            kUndoableEntity,
            kUndoableLayer
        };
    };
}

#endif //NEWCUT_ENUM_CLASS_H
