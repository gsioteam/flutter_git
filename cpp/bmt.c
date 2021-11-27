//
// Created by gen on 11/19/21.
//

#ifdef __ANDROID__

#include <jni.h>

JavaVM* bmt_vm = NULL;
jclass bmt_plugin = NULL;

void bmt_sendEvent(const char *name, const char *data) {
    if (bmt_vm && bmt_plugin) {
        JNIEnv *env = NULL;
        if ((*bmt_vm)->AttachCurrentThread(bmt_vm, &env, NULL) != 0) {
        }

        if (env) {
            jclass clazz = (*env)->GetObjectClass(env, bmt_plugin);
            jmethodID method = (*env)->GetMethodID(env, clazz, "sendEvent",
                    "(Ljava/lang/String;Ljava/lang/String;)V");

            jstring jname = (*env)->NewStringUTF(env, name);
            jstring jdata = (*env)->NewStringUTF(env, data);

            (*env)->CallVoidMethod(env, bmt_plugin, method, jname, jdata);

            (*env)->DeleteLocalRef(env, jname);
            (*env)->DeleteLocalRef(env, jdata);

            if ((*env)->ExceptionCheck(env)) {
                (*env)->ExceptionDescribe(env);
            }

            (*bmt_vm)->DetachCurrentThread(bmt_vm);
        }
    }
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    bmt_vm = vm;
    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM *vm, void *reserved) {
    bmt_vm = NULL;
}

JNIEXPORT void JNICALL
Java_com_neo_flutter_1git_FlutterGitPlugin_setup(JNIEnv *env, jobject thiz, jobject plugin) {
    if (bmt_plugin) {
        (*env)->DeleteGlobalRef(env, bmt_plugin);
    }
    if (plugin) {
        bmt_plugin = (*env)->NewGlobalRef(env, plugin);
    } else {
        bmt_plugin = NULL;
    }
}

#else

#include <objc/runtime.h>

typedef void (*IMP_sendEvent)(Class, SEL, const char *name, const char *data);

extern void bmt_sendEvent(const char *name, const char *data) {
    Class NativeMainThreadPlugin = objc_getClass("NativeMainThreadPlugin");
    SEL sel = sel_registerName("sendEvent:withData:");
    IMP_sendEvent imp = (IMP_sendEvent)class_getMethodImplementation(NativeMainThreadPlugin, sel);
    imp(NativeMainThreadPlugin, sel, name, data);
}
#endif
