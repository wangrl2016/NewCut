//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_FFMPEG_DELETERS_H
#define NEWCUT_FFMPEG_DELETERS_H

namespace media {
    // Wraps FFmpeg's av_free() in a class that can be passed as a template
    // argument to scoped_ptr malloc.
    struct ScopedPtrAVFree {
        void operator()(void* x) const;
    };

    // Calls av_packet_free(). Do not use this with an AVPacket instance that
    // was allocated with newe or manually av_malloc'd. ScopedAVPacket is the
    // recommended way to manage an AVPacket's lifetime.
    struct ScopedPtrAVFreePacket {
        void operator()(void* x) const;
    };

    // Frees an AVCodecContext object in a class that can be passed as
    // a Deleter argument.
    struct ScopedPtrAVFreeContext {
        void operator()(void* x) const;
    };

    // Frees an AVFrame object in a class that can be passed as a Deleter.
    struct ScopedPtrAVFreeFrame {
        void operator()(void* x) const;
    };
}

#endif //NEWCUT_FFMPEG_DELETERS_H
