//
// Created by wangrl2016 on 2023/1/9.
//

#ifndef NEWCUT_SCOPED_AUDIO_UNIT_H
#define NEWCUT_SCOPED_AUDIO_UNIT_H

#include <AudioUnit/AudioUnit.h>
#include <CoreAudio/CoreAudio.h>

namespace media {
    // For whatever reason Apple doesn't have constants defined for these; per the
    // documentation, we use bus 0 for output and bus 1 for input:
    // https://developer.apple.com/library/archive/technotes/tn2091/_index.html
    enum AUElement : AudioUnitElement { OUTPUT = 0, INPUT = 1 };

    // A helper class that ensures AudioUnits are properly disposed of.
    class ScopedAudioUnit {
    public:
        // Creates a new AudioUnit and sets its device for |element| to |device|. If
        // the operation fails, is_valid() will return false and audio_unit() will
        // return nullptr;
        ScopedAudioUnit(AudioDeviceID device, AUElement element);

        ScopedAudioUnit(const ScopedAudioUnit&) = delete;

        ScopedAudioUnit& operator=(const ScopedAudioUnit&) = delete;

        ~ScopedAudioUnit();

        bool is_valid() const { return audio_unit_ != nullptr; }

        AudioUnit audio_unit() const { return audio_unit_; }

    private:
        AudioUnit audio_unit_ = nullptr;
    };
}


#endif //NEWCUT_SCOPED_AUDIO_UNIT_H
