//
// Created by wangrl2016 on 2023/1/9.
//

#include <glog/logging.h>
#include "scoped_audio_unit.h"

namespace media {
    constexpr AudioComponentDescription desc = {
            kAudioUnitType_Output,
            kAudioUnitSubType_HALOutput,
            kAudioUnitManufacturer_Apple,
            0, 0
    };

    static void DestroyAudioUnit(AudioUnit audio_unit) {
        OSStatus result = AudioUnitUninitialize(audio_unit);
        if (result != noErr) {
            LOG(ERROR) << __FUNCTION__ << " failed: " << audio_unit;
        }
        result = AudioComponentInstanceDispose(audio_unit);
        if (result != noErr) {
            LOG(ERROR) << __FUNCTION__ << " failed: " << audio_unit;
        }
    }

    ScopedAudioUnit::ScopedAudioUnit(AudioDeviceID device, AUElement element) {
        AudioComponent comp = AudioComponentFindNext(0, &desc);
        if (!comp) {
            return;
        }

        AudioUnit audio_unit;
        OSStatus result = AudioComponentInstanceNew(comp, &audio_unit);
        if (result != noErr) {
            LOG(ERROR) << __FUNCTION__ << " failed";
            return;
        }

        const UInt32 enable_input_io = element == AUElement::INPUT ? 1 : 0;
        result = AudioUnitSetProperty(audio_unit,
                                      kAudioOutputUnitProperty_EnableIO,
                                      kAudioUnitScope_Input,
                                      AUElement::INPUT,
                                      &enable_input_io,
                                      sizeof(enable_input_io));
        if (result != noErr) {
            LOG(ERROR) << "Failed to set input enable IO for audio unit.";
            DestroyAudioUnit(audio_unit);
            return;
        }

        const UInt32 enable_output_io = !enable_input_io;
        result = AudioUnitSetProperty(audio_unit,
                                      kAudioOutputUnitProperty_EnableIO,
                                      kAudioUnitScope_Output,
                                      AUElement::OUTPUT,
                                      &enable_output_io,
                                      sizeof(enable_output_io));
        if (result != noErr) {
            LOG(ERROR) << "Failed to set output enable IO for audio unit.";
            DestroyAudioUnit(audio_unit);
            return;
        }
    }
}
