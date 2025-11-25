#include "AudioEngine.h"

void AudioEngine::start() {
    oboe::AudioStreamBuilder builder;
    builder.setDirection(oboe::Direction::Input);
    builder.setPerformanceMode(oboe::PerformanceMode::LowLatency);
    builder.setSharingMode(oboe::SharingMode::Exclusive);
    builder.setFormat(oboe::AudioFormat::Float);
    builder.setChannelCount(oboe::ChannelCount::Mono);
    builder.setCallback(this);

    builder.openStream(stream);
    stream->requestStart();
}

void AudioEngine::stop() {
    if (stream) {
        stream->stop();
        stream->close();
    }
}

void AudioEngine::setPhaseInverted(bool inverted) {
    isPhaseInverted.store(inverted);
}

void AudioEngine::setGain(float gainValue) {
    gain.store(gainValue);
}

float AudioEngine::getCurrentAmplitude() {
    return currentAmplitude.load();
}

oboe::DataCallbackResult AudioEngine::onAudioReady(
        oboe::AudioStream *oboeStream,
        void *audioData,
        int32_t numFrames) {

    float *floatData = (float *) audioData;
    float currentGain = gain.load();
    bool inverted = isPhaseInverted.load();

    float maxPeak = 0.0f; // Para medir o volume deste frame

    // Loop de processamento ultra-rápido
    for (int i = 0; i < numFrames; ++i) {
        float sample = floatData[i];

        // Medir Amplitude (Volume) antes de processar
        float absValue = std::abs(sample);
        if (absValue > maxPeak) {
            maxPeak = absValue;
        }

        // Aplica inversão e ganho
        if (inverted) {
            sample = -sample;
        }
        sample *= currentGain;

        // floatData[i] = sample; // Output simulado
    }

    // Atualiza a amplitude atômica para a UI ler
    currentAmplitude.store(maxPeak);

    return oboe::DataCallbackResult::Continue;
}
