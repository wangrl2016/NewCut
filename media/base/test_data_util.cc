//
// Created by wangrl2016 on 2023/1/3.
//

#include <filesystem>
#include <fstream>
#include <glog/logging.h>
#include "base/files/file_path.h"
#include "media/base/test_data_util.h"
#include "user_config.h"

namespace media {

    const base::FilePath::CharType kTestDataPath[] =
            FILE_PATH_LITERAL("resources/test/media");

    base::FilePath GetTestDataPath() {
        return base::FilePath(kTestDataPath);
    }

    base::FilePath GetTestDataFilePath(const std::string& name) {
        base::FilePath file_path(FILE_PATH_LITERAL(PROJECT_SOURCE_DIR));
        return file_path.Append(GetTestDataPath()).AppendASCII(name);
    }

    std::shared_ptr<DecoderBuffer> ReadTestDataFile(
            const std::string& name) {
        base::FilePath file_path = GetTestDataFilePath(name);
        auto file_size = std::filesystem::file_size(file_path.value());

        CHECK(file_size > 0) << "Failed to get file size for '" << name << "'";

        std::shared_ptr<DecoderBuffer> buffer(new DecoderBuffer(file_size));
        auto* data = reinterpret_cast<char*>(buffer->writable_data());
        std::ifstream ifs(file_path.value().c_str(), std::ios::binary);
        ifs.read(data, std::streamsize(file_size));
        ifs.close();

        return buffer;
    }
}
