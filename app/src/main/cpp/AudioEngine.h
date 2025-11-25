#include <oboe/Oboe.h>
#include <atomic> // Para variáveis thread-safe

class AudioEngine : public oboe::AudioStreamCallback {
public:
    void start();
    void stop();
    void setPhaseInverted(bool inverted);
    void setGain(float gainValue);
    float getCurrentAmplitude(); // Novo método

    oboe::DataCallbackResult onAudioReady(
            oboe::AudioStream *oboeStream,
            void *audioData,
            int32_t numFrames) override;

private:
    std::shared_ptr<oboe::AudioStream> stream;
    // Usamos atomic para evitar conflitos entre a UI e a thread de áudio
    std::atomic<bool> isPhaseInverted { false };
    std::atomic<float> gain { 1.0f };
    std::atomic<float> currentAmplitude { 0.0f }; // Armazena o volume atual
};