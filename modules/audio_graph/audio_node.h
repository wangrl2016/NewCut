//
// Created by wangrl2016 on 2023/1/10.
//

#ifndef NEWCUT_AUDIO_NODE_H
#define NEWCUT_AUDIO_NODE_H

namespace audio_graph {
    class AudioHandler;
    class AudioNodeOptions;
    class BaseAudioContext;

    // An AudioNode is the basic building block for handling audio within an
    // BaseAudioContext. It may be an audio source, an intermediate processing
    // module, or an audio destination. Each AudioNode can have inputs and/or
    // outputs. An AudioSourceNode has no inputs and a single output.
    // An AudioDestinationNode has one input and no outputs and represents the final
    // destination to the audio hardware. Most processing nodes such as filters
    // will have one input and one output, although multiple inputs and outputs are
    // possible.
    //
    // Each of AudioNode objects own its dedicated AudioHandler object. AudioNode
    // is responsible to provide IDL-accessible interface.
    // AudioHandler is responsible for anything else. We must not touch AudioNode
    // objects in an audio rendering thread.
    //
    // AudioHandler is created and owned by an AudioNode almost all the time. When
    // the AudioNode is about to die, the ownership if its AudioHandler is
    // transferred to DeferredTaskHandler, and it does deref the AudioHandler on
    // the main thread.
    //
    // Be careful to avoid reference cycles. If an AudioHandler has a reference
    // cycle including the owner AudioNode, objects in the cycle are never
    // collected.
    class AudioNode {
    public:
        ~AudioNode();

        AudioHandler& Handler() const;

    protected:
        explicit AudioNode(BaseAudioContext& context);

    };
}

#endif //NEWCUT_AUDIO_NODE_H
