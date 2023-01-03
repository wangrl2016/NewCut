//
// Created by wangrl2016 on 2023/1/3.
//

#ifndef NEWCUT_TEST_DATA_UTIL_H
#define NEWCUT_TEST_DATA_UTIL_H

#include <memory>
#include "media/base/decoder_buffer.h"

namespace media {
    std::shared_ptr<DecoderBuffer> ReadTestDataFile(
            const std::string& name);
}

#endif //NEWCUT_TEST_DATA_UTIL_H
