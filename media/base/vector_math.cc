//
// Created by wangrl2016 on 2023/1/2.
//

#include "media/base/vector_math.h"

namespace media {
    void FMUL(const float src[],
              float scale,
              int len,
              float dest[]) {
        // TODO: 硬件加速
        for (int i = 0; i < len; i++) {
            dest[i] = src[i] * scale;
        }
    }
}
