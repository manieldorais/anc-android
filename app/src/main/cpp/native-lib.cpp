#include <jni.h>
#include "AudioEngine.h"

static AudioEngine engine;

// NOMES CORRIGIDOS PARA O PACOTE 'com.anc.app'
// Nota: Não repita 'MainActivity' duas vezes no nome da função.

extern "C" JNIEXPORT void JNICALL
Java_com_anc_app_MainActivity_startAudioEngine(JNIEnv *env, jobject) {
engine.start();
}

extern "C" JNIEXPORT void JNICALL
Java_com_anc_app_MainActivity_stopAudioEngine(JNIEnv *env, jobject) {
engine.stop();
}

extern "C" JNIEXPORT void JNICALL
Java_com_anc_app_MainActivity_setPhaseInverted(JNIEnv *env, jobject, jboolean inverted) {
engine.setPhaseInverted(inverted);
}

extern "C" JNIEXPORT void JNICALL
Java_com_anc_app_MainActivity_setGain(JNIEnv *env, jobject, jfloat gain) {
engine.setGain(gain);
}

// NOVA FUNÇÃO: Retorna o volume atual (0.0 a 1.0)
extern "C" JNIEXPORT jfloat JNICALL
Java_com_anc_app_MainActivity_getCurrentAmplitude(JNIEnv *env, jobject) {
    return engine.getCurrentAmplitude();
}