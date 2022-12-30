//
// Created by wangrl2016 on 2022/12/17.
//

#include "media/filters/ffmpeg_glue.h"

namespace media {
    // Internal buffer size used by AVIO for reading.
    // Currently, we want to use 32kb to preserve existing behavior
    // with the previous URLProtocol base approach.
    enum {
        kBufferSize = 32 * 1024
    };

    // 将opaque中的数据读入到buf中
    static int AVIOReadOperation(void* opaque, uint8_t* buf, int buf_size) {
        return reinterpret_cast<FFmpegURLProtocol*>(opaque)->Read(buf_size, buf);
    }

    static int64_t AVIOSeekOperation(void* opaque, int64_t offset, int whence) {
        auto* protocol = reinterpret_cast<FFmpegURLProtocol*>(opaque);
        int64_t new_offset = AVERROR(EIO);
        switch (whence) {
            case SEEK_SET:
                if (protocol->SetPosition(offset)) {
                    protocol->GetPosition(&new_offset);
                }
                break;
            case SEEK_CUR:
                int64_t pos;
                // 获取当前位置
                if (!protocol->GetPosition(&pos)) {
                    break;
                }
                if (protocol->SetPosition(pos + offset)) {
                    protocol->GetPosition(&new_offset);
                }
                break;
            case SEEK_END:
                int64_t size;
                if (!protocol->GetSize(&size)) {
                    break;
                }
                // 末尾offset需要为负值
                if (protocol->SetPosition(size + offset)) {
                    protocol->GetPosition(&new_offset);
                }
                break;
            case AVSEEK_SIZE:
                // 获取当前位置
                protocol->GetSize(&new_offset);
                break;
            default:
                LOG(ERROR) << "Invalid seek";
        }
        return new_offset;
    }

    static void LogContainer(bool is_local_file,
                             MediaContainerName container) {
        // NOT implementation
    }

    FFmpegGlue::FFmpegGlue(media::FFmpegURLProtocol* protocol) {
        // Initialize an AVIOContext using our custom read and seek operations. Don't
        // keep pointers to the buffer since FFmpeg may reallocate it on the fly. It
        // will be cleaned up.
        format_context_ = avformat_alloc_context();
        // 从protocol里面read或者seek数据
        avio_context_.reset(avio_alloc_context(
                static_cast<unsigned char*>(av_malloc(kBufferSize)), kBufferSize, 0,
                protocol, &AVIOReadOperation, nullptr, AVIOSeekOperation));

        // Ensure FFmpeg only tries to seek on resources we know to be seekable.
        avio_context_->seekable = protocol->IsStreaming() ? 0 : AVIO_SEEKABLE_NORMAL;

        // Ensure wrrting is disabled.
        avio_context_->write_flag = 0;

        // Tell the format context about our custom IO context. avformat_open_input()
        // will set the AVFMT_FLAG_CUSOTM_IO flag for us, but do so here to ensure an
        // early error state doesn't cause FFmpeg to free out resources in error.
        format_context_->flags |= AVFMT_FLAG_CUSTOM_IO;

        // Enable fast, but inaccurate seeks for MP3.
        format_context_->flags |= AVFMT_FLAG_FAST_SEEK;

        // Ensures format parsng erros will bail out.
        format_context_->error_recognition |= AV_EF_EXPLODE;

        format_context_->pb = avio_context_.get();
    }

    bool FFmpegGlue::OpenContext(bool is_local_file) {
        DCHECK(!open_called_) << "OpenContext() shouldn't be called twice.";

        // If avformat_open_input() is called we have to take a slightly different
        // destruction path to avoid double frees.
        open_called_ = true;

        // By passing nullptr for the filename (second parameter) we are telling
        // FFmpeg to use the AVIO context we set up form the AVFormatContext structure.
        const int ret =
                avformat_open_input(&format_context_, nullptr, nullptr, nullptr);

        // If FFmpeg can't identify the file, read the first 8k and attempt to guess
        // at the container type ourselves. This way we can track emergent formats.
        // Only try on AVERROR_INVALIDDATA to avoid running after I/O errors.
        if (ret == AVERROR_INVALIDDATA) {
            std::vector<uint8_t> buffer(8192);
            const int64_t pos = AVIOSeekOperation(avio_context_->opaque, 0, SEEK_SET);
            if (pos < 0) {
                return false;
            }

            const int num_read =
                    AVIOReadOperation(avio_context_->opaque, buffer.data(), buffer.size());
            if (num_read < kMinimumContainerSize) {
                return false;
            }

            container_ = DetermineContainer(buffer.data(), num_read);
            detected_hls_ = container_ == MediaContainerName::CONTAINER_HLS;
            return false;
        } else if (ret < 0) {
            return false;
        }

        // Repy on ffmpeg's parsing if we're able to successfully ope the file.
        if (strcmp(format_context_->iformat->name, "mov,mp4,m4a,3gp,3g2,mj2") == 0) {
            container_ = MediaContainerName::CONTAINER_MOV;
        } else if (strcmp(format_context_->iformat->name, "flac") == 0) {
            container_ = MediaContainerName::CONTAINER_FLAC;
        } else if (strcmp(format_context_->iformat->name, "matroska,webm") == 0) {
            container_ = MediaContainerName::CONTAINER_WEBM;
        } else if (strcmp(format_context_->iformat->name, "ogg") == 0) {
            container_ = MediaContainerName::CONTAINER_OGG;
        } else if (strcmp(format_context_->iformat->name, "wav") == 0) {
            container_ = MediaContainerName::CONTAINER_WAV;
        } else if (strcmp(format_context_->iformat->name, "aac") == 0) {
            container_ = MediaContainerName::CONTAINER_AAC;
        } else if (strcmp(format_context_->iformat->name, "mp3") == 0) {
            container_ = MediaContainerName::CONTAINER_MP3;
        } else if (strcmp(format_context_->iformat->name, "amr") == 0) {
            container_ = MediaContainerName::CONTAINER_AMR;
        } else if (strcmp(format_context_->iformat->name, "avi") == 0) {
            container_ = MediaContainerName::CONTAINER_AVI;
        }

        DCHECK_NE(container_, MediaContainerName::CONTAINER_UNKNOWN);
        return true;
    }

    FFmpegGlue::~FFmpegGlue() {
        // In the event of avformat_open_input() failure, FFmpeg may sometimes free
        // our AVFormatContext behind the scenes, but leave the buffer alive. It will
        // helpfully set |format_context| to nullptr in this case.
        if (!format_context_) {
            av_free(avio_context_->buffer);
            return;
        }

        // If avformat_open_input() hasn't been called, we should simply free the
        // AVFormatContext and buffer instead of using avformat_close_input().
        if (!open_called_) {
            avformat_free_context(format_context_);
            av_free(avio_context_->buffer);
            return;
        }

        avformat_close_input(&format_context_);
        av_free(avio_context_->buffer);
    }
}
