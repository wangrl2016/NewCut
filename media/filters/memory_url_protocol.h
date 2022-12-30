//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_MEMORY_URL_PROTOCOL_H
#define NEWCUT_MEMORY_URL_PROTOCOL_H

#include "media/filters/ffmpeg_glue.h"

namespace media {
    // Simple FFmpegURLProtocol that reads from a buffer.
    // NOTE: This object does not copy the buffer so the
    // buffer pointer passed int the constructor need to
    // remain valid for the entire lifetime of the object.
    // 相当于数据转接口
    class MemoryURLProtocol : public FFmpegURLProtocol {
    public:
        MemoryURLProtocol() = delete;

        MemoryURLProtocol(const uint8_t* data, int64_t size, bool streaming);

        MemoryURLProtocol(const MemoryURLProtocol&) = delete;

        MemoryURLProtocol& operator=(const MemoryURLProtocol&) = delete;

        virtual ~MemoryURLProtocol();

        // FFmpegURLProtocol methods.
        int Read(int size, uint8_t* data) override;

        bool GetPosition(int64_t* position_out) override;

        bool SetPosition(int64_t position) override;

        bool GetSize(int64_t* size_out) override;

        bool IsStreaming() override;

    private:
        const uint8_t* data_;
        int64_t size_;
        int64_t position_;
        bool streaming_;
    };
}

#endif //NEWCUT_MEMORY_URL_PROTOCOL_H
