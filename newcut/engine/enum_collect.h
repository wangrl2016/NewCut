//
// Created by wr on 2022/12/27.
//

#ifndef NEWCUT_ENUM_CLASS_H
#define NEWCUT_ENUM_CLASS_H

namespace nc {
    class EnumCollect {
    public:
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
            kEntityConstrucionLine,
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

        enum SnapRestriction {
            kRestricNothing,        // No restriction to snap mode
            kRestricHorizontal,     // Restrict to 0, 180 degrees
            kRestricVertical,       // Restrict to 90, 270 degrees
            kRestricOrthogonal      // Restrict to 90, 180, 270, 0 degrees
        };
    };
}

#endif //NEWCUT_ENUM_CLASS_H
