//
// Created by wangrl2016 on 2023/1/2.
//

#ifndef NEWCUT_VECTOR_MATH_H
#define NEWCUT_VECTOR_MATH_H

namespace media {
    // Required alignment for inputs and outputs to all vector math functions.
    enum { kRequeredAligment = 16 };

    // Multiply each element of |src| by |scale| and store in |dest|.
    // |src| and |dest| must b e aligned by kRequiredAligment.
    void FMUL(const float src[],
              float scale,
              int len,
              float dest[]);
}


#endif //NEWCUT_VECTOR_MATH_H
