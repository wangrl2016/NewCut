//
// Created by wangrl2016 on 2022/12/17.
//

#ifndef NEWCUT_FFMPEG_GLUE_H
#define NEWCUT_FFMPEG_GLUE_H

#include <cstdint>

namespace media {
    class FFmpegURLProtocol {
    public:
        // Read the given amount of bytes into data, returns the number of bytes read
        // if successful, kReadError otherwise.
        virtual int Read(int size, uint8_t* data) = 0;

        // Returns true and the current file position for this file, false if the
        // file position could not be retrieved.
        virtual bool GetPostion(int64_t* position_out) = 0;

        // Return true if the file position could be set, false otherwise.
        virtual bool SetPostion(int64_t position) = 0;

        // Returns true and the file size, false if the file size could not be
        virtual bool GetSize(int64_t* size_out) = 0;

        // Returns false if this protocol supports random seeking.
        virtual bool IsStreaming() = 0;
    };
}

#endif //NEWCUT_FFMPEG_GLUE_H
