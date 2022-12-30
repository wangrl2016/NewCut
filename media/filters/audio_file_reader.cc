//
// Created by wangrl2016 on 2022/12/17.
//

#include "media/filters/audio_file_reader.h"

namespace media {
    AudioFileReader::AudioFileReader(media::FFmpegURLProtocol* protocol)
            : stream_index_(0),
              protocol_(protocol),
              audio_codec_(AudioCodec::kUnknown),
              channels_(0),
              sample_rate_(0),
              av_sample_format_(0) {}

    AudioFileReader::~AudioFileReader() {
        Close();
    }

    bool AudioFileReader::Open() {
        return OpenDemuxer() && OpenDecoder();
    }

    bool AudioFileReader::OpenDemuxer() {
        glue_ = std::make_unique<FFmpegGlue>(protocol_);
    }
}