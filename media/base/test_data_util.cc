//
// Created by wangrl2016 on 2023/1/3.
//

#include <filesystem>
#include <fstream>
#include <glog/logging.h>
#include "media/base/test_data_util.h"
#include "user_config.h"

namespace media {
    std::shared_ptr<DecoderBuffer> ReadTestDataFile(
            const std::string& name) {
        // TODO: Window路径存在不同
        std::string filename = std::string(MEDIA_TEST_FILE_DIR)
                               + "/resources/test/media/"
                               + std::string(name);
        auto file_size = std::filesystem::file_size(filename);
        CHECK(file_size > 0) << "Failed to get file size for '" << name << "'";

        std::shared_ptr<DecoderBuffer> buffer(new DecoderBuffer(file_size));
        auto* data = reinterpret_cast<char*>(buffer->writable_data());
        std::ifstream ifs(name.c_str(), std::ios::binary);
        // CHECK(ifs.is_open()) << "Failed to read '" << name << "'";
        ifs.read(data, std::streamsize(file_size));
        ifs.close();

        return buffer;
    }
}
