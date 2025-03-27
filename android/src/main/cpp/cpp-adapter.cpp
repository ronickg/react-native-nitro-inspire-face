#include <jni.h>
#include "nitroinspirefaceOnLoad.hpp"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
  return margelo::nitro::nitroinspireface::initialize(vm);
}
