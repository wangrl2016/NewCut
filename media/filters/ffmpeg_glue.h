//
// Created by wangrl2016 on 2022/12/17.
//

#ifndef NEWCUT_FFMPEG_GLUE_H
#define NEWCUT_FFMPEG_GLUE_H

#include <cstdint>
#include <memory>
extern "C" {
#include <libavformat/avformat.h>
}
#include "media/ffmpeg/ffmpeg_deleters.h"

namespace media {
    class FFmpegURLProtocol {
    public:
        // Read the given amount of bytes into data, returns the number of bytes read
        // if successful, kReadError otherwise.
        virtual int Read(int size, uint8_t* data) = 0;

        // Returns true and the current file position for this file, false if the
        // file position could not be retrieved.
        virtual bool GetPosition(int64_t* position_out) = 0;

        // Return true if the file position could be set, false otherwise.
        virtual bool SetPosition(int64_t position) = 0;

        // Returns true and the file size, false if the file size could not be
        virtual bool GetSize(int64_t* size_out) = 0;

        // Returns false if this protocol supports random seeking.
        virtual bool IsStreaming() = 0;
    };

    class FFmpegGlue {
    public:
        // Set file documention for usage. |protocol| must outlive FFmpegGlue.
        explicit FFmpegGlue(FFmpegURLProtocol* protocol);

        FFmpegGlue(const FFmpegGlue&) = delete;

        FFmpegGlue& operator=(const FFmpegGlue&) = delete;

        ~FFmpegGlue();

        // Opens an AVFormatContext specially prepared to process reads and seeks
        // through the FFmpegURLProtocol provided during construction.
        // |is_local_file| is an optional parameter used for metrics reporting.
        bool OpenContext(bool is_local_file = false);

        AVFormatContext* format_context() { return format_context_; }

    private:
        bool open_called_ = false;
        bool detected_hls = false;
        AVFormatContext* format_context_;
        std::unique_ptr<AVIOContext, ScopedPtrAVFree> avio_context_;
    };
}

#endif //NEWCUT_FFMPEG_GLUE_H
