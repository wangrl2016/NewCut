//
// Created by wangrl2016 on 2022/12/17.
//

#ifndef NEWCUT_AUDIO_FILE_READER_H
#define NEWCUT_AUDIO_FILE_READER_H

#include "media/filters/ffmpeg_glue.h"

namespace media {
    class AudioFileReader {
    public:
        // Audio file data will be read using the given protocol.
        // The AudioFileReader does not take ownership of |protocol| and
        // simply maintains a weak reference to it.
        explicit AudioFileReader(FFmpegURLProtocol* protocol);

        AudioFileReader(const AudioFileReader&) = delete;

        AudioFileReader& operator=(const AudioFileReader&) = delete;

        virtual ~AudioFileReader();

        // Open() reads the audio data format so that the sample_rate(),
        // channels(), GetDuration(), and GetNumberOfFrames() methods can be called.
        // It returns |true| on success.
        bool Open();

        void Close();
    };
}


#endif //NEWCUT_AUDIO_FILE_READER_H
