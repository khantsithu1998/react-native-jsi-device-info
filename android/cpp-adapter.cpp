#include <jni.h>
#include <sys/types.h>
#include "example.h"
#include "pthread.h"
#include <jsi/jsi.h>

using namespace facebook::jsi;
using namespace std;

JavaVM *java_vm;
jclass java_class;
jobject java_object;

/**
 * A simple callback function that allows us to detach current JNI Environment
 * when the thread
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */

void DeferThreadDetach(JNIEnv *env)
{
        static pthread_key_t thread_key;

        // Set up a Thread Specific Data key, and a callback that
        // will be executed when a thread is destroyed.
        // This is only done once, across all threads, and the value
        // associated with the key for any given thread will initially
        // be NULL.
        static auto run_once = []
        {
                const auto err = pthread_key_create(&thread_key, [](void *ts_env)
                                                    {
            if (ts_env) {
                java_vm->DetachCurrentThread();
            } });
                if (err)
                {
                        // Failed to create TSD key. Throw an exception if you want to.
                }
                return 0;
        }();

        // For the callback to actually be executed when a thread exits
        // we need to associate a non-NULL value with the key on that thread.
        // We can use the JNIEnv* as that value.
        const auto ts_env = pthread_getspecific(thread_key);
        if (!ts_env)
        {
                if (pthread_setspecific(thread_key, env))
                {
                        // Failed to set thread-specific value for key. Throw an exception if you want to.
                }
        }
}

/**
 * Get a JNIEnv* valid for this thread, regardless of whether
 * we're on a native thread or a Java thread.
 * If the calling thread is not currently attached to the JVM
 * it will be attached, and then automatically detached when the
 * thread is destroyed.
 *
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */
JNIEnv *GetJniEnv()
{
        JNIEnv *env = nullptr;
        // We still call GetEnv first to detect if the thread already
        // is attached. This is done to avoid setting up a DetachCurrentThread
        // call on a Java thread.

        // g_vm is a global.
        auto get_env_result = java_vm->GetEnv((void **)&env, JNI_VERSION_1_6);
        if (get_env_result == JNI_EDETACHED)
        {
                if (java_vm->AttachCurrentThread(&env, NULL) == JNI_OK)
                {
                        DeferThreadDetach(env);
                }
                else
                {
                        // Failed to attach thread. Throw an exception if you want to.
                }
        }
        else if (get_env_result == JNI_EVERSION)
        {
                // Unsupported JNI version. Throw an exception if you want to.
        }
        return env;
}

static jstring string2jstring(JNIEnv *env, const string &str)
{
        return (*env).NewStringUTF(str.c_str());
}

void install(facebook::jsi::Runtime &jsiRuntime)
{
        auto getApplicationName = Function::createFromHostFunction(jsiRuntime,
                                                                   PropNameID::forAscii(jsiRuntime,
                                                                                        "getApplicationName"),
                                                                   0,
                                                                   [](Runtime &runtime,
                                                                      const Value &thisValue,
                                                                      const Value *arguments,
                                                                      size_t count) -> Value
                                                                   {
                                                                           JNIEnv *jniEnv = GetJniEnv();

                                                                           java_class = jniEnv->GetObjectClass(
                                                                               java_object);
                                                                           jmethodID getAppName = jniEnv->GetMethodID(
                                                                               java_class, "getAppName",
                                                                               "()Ljava/lang/String;");
                                                                           jobject result = jniEnv->CallObjectMethod(
                                                                               java_object, getAppName);
                                                                           const char *str = jniEnv->GetStringUTFChars(
                                                                               (jstring)result, NULL);

                                                                           return Value(runtime,
                                                                                        String::createFromUtf8(
                                                                                            runtime, str));
                                                                   });

        jsiRuntime.global().setProperty(jsiRuntime, "getApplicationName", move(getApplicationName));

        auto getApiLevel = Function::createFromHostFunction(jsiRuntime,
                                                            PropNameID::forAscii(jsiRuntime,
                                                                                 "getApiLevel"),
                                                            0,
                                                            [](Runtime &runtime,
                                                               const Value &thisValue,
                                                               const Value *arguments,
                                                               size_t count) -> Value
                                                            {
                                                                    JNIEnv *jniEnv = GetJniEnv();

                                                                    java_class = jniEnv->GetObjectClass(
                                                                        java_object);
                                                                    jmethodID getLevel = jniEnv->GetMethodID(
                                                                        java_class, "getApiLevel",
                                                                        "()Ljava/lang/String;");
                                                                    jobject result = jniEnv->CallObjectMethod(
                                                                        java_object, getLevel);
                                                                    const char *str = jniEnv->GetStringUTFChars(
                                                                        (jstring)result, NULL);

                                                                    return Value(runtime,
                                                                                 String::createFromUtf8(
                                                                                     runtime, str));
                                                            });
        jsiRuntime.global().setProperty(jsiRuntime, "getApiLevel", move(getApiLevel));

        auto getBaseOS = Function::createFromHostFunction(jsiRuntime,
                                                              PropNameID::forAscii(jsiRuntime,
                                                                                   "getBootloader"),
                                                              0,
                                                              [](Runtime &runtime,
                                                                 const Value &thisValue,
                                                                 const Value *arguments,
                                                                 size_t count) -> Value
                                                              {
                                                                      JNIEnv *jniEnv = GetJniEnv();

                                                                      java_class = jniEnv->GetObjectClass(
                                                                          java_object);
                                                                      jmethodID getBaseOS = jniEnv->GetMethodID(
                                                                          java_class, "getBaseOs",
                                                                          "()Ljava/lang/String;");
                                                                      jobject result = jniEnv->CallObjectMethod(
                                                                          java_object, getBaseOS);
                                                                      const char *str = jniEnv->GetStringUTFChars(
                                                                          (jstring)result, NULL);

                                                                      return Value(runtime,
                                                                                   String::createFromUtf8(
                                                                                       runtime, str));
                                                              });

        jsiRuntime.global().setProperty(jsiRuntime, "getBaseOS", move(getBaseOS));

        auto getBatteryLevel = Function::createFromHostFunction(jsiRuntime,
                                                                PropNameID::forAscii(jsiRuntime,
                                                                                     "getBatteryLevel"),
                                                                0,
                                                                [](Runtime &runtime,
                                                                   const Value &thisValue,
                                                                   const Value *arguments,
                                                                   size_t count) -> Value
                                                                {
                                                                        JNIEnv *jniEnv = GetJniEnv();

                                                                        java_class = jniEnv->GetObjectClass(
                                                                            java_object);
                                                                        jmethodID getBatteryLevel = jniEnv->GetMethodID(
                                                                            java_class, "getBatteryLevel",
                                                                            "()Ljava/lang/String;");
                                                                        jobject result = jniEnv->CallObjectMethod(
                                                                            java_object, getBatteryLevel);
                                                                        const char *str = jniEnv->GetStringUTFChars(
                                                                            (jstring)result, NULL);

                                                                        return Value(runtime,
                                                                                     String::createFromUtf8(
                                                                                         runtime, str));
                                                                });
        jsiRuntime.global().setProperty(jsiRuntime, "getBatteryLevel", move(getBatteryLevel));

        auto getBootloader = Function::createFromHostFunction(jsiRuntime,
                                                              PropNameID::forAscii(jsiRuntime,
                                                                                   "getBootloader"),
                                                              0,
                                                              [](Runtime &runtime,
                                                                 const Value &thisValue,
                                                                 const Value *arguments,
                                                                 size_t count) -> Value
                                                              {
                                                                      JNIEnv *jniEnv = GetJniEnv();

                                                                      java_class = jniEnv->GetObjectClass(
                                                                          java_object);
                                                                      jmethodID getBootloader = jniEnv->GetMethodID(
                                                                          java_class, "getBootloader",
                                                                          "()Ljava/lang/String;");
                                                                      jobject result = jniEnv->CallObjectMethod(
                                                                          java_object, getBootloader);
                                                                      const char *str = jniEnv->GetStringUTFChars(
                                                                          (jstring)result, NULL);

                                                                      return Value(runtime,
                                                                                   String::createFromUtf8(
                                                                                       runtime, str));
                                                              });

        jsiRuntime.global().setProperty(jsiRuntime, "getBootloader", move(getBootloader));

        auto getBuildNumer = Function::createFromHostFunction(jsiRuntime,
                                                              PropNameID::forAscii(jsiRuntime,
                                                                                   "getBuildNumer"),
                                                              0,
                                                              [](Runtime &runtime,
                                                                 const Value &thisValue,
                                                                 const Value *arguments,
                                                                 size_t count) -> Value
                                                              {
                                                                      JNIEnv *jniEnv = GetJniEnv();

                                                                      java_class = jniEnv->GetObjectClass(
                                                                          java_object);
                                                                      jmethodID getBuildNumer = jniEnv->GetMethodID(
                                                                          java_class, "getBuildNumer",
                                                                          "()Ljava/lang/String;");
                                                                      jobject result = jniEnv->CallObjectMethod(
                                                                          java_object, getBuildNumer);
                                                                      const char *str = jniEnv->GetStringUTFChars(
                                                                          (jstring)result, NULL);

                                                                      return Value(runtime,
                                                                                   String::createFromUtf8(
                                                                                       runtime, str));
                                                              });

        jsiRuntime.global().setProperty(jsiRuntime, "getBuildNumer", move(getBuildNumer));

        auto getBuildId = Function::createFromHostFunction(jsiRuntime,
                                                              PropNameID::forAscii(jsiRuntime,
                                                                                   "getBuildId"),
                                                              0,
                                                              [](Runtime &runtime,
                                                                 const Value &thisValue,
                                                                 const Value *arguments,
                                                                 size_t count) -> Value
                                                              {
                                                                      JNIEnv *jniEnv = GetJniEnv();

                                                                      java_class = jniEnv->GetObjectClass(
                                                                          java_object);
                                                                      jmethodID getBuildId = jniEnv->GetMethodID(
                                                                          java_class, "getBuildId",
                                                                          "()Ljava/lang/String;");
                                                                      jobject result = jniEnv->CallObjectMethod(
                                                                          java_object, getBuildId);
                                                                      const char *str = jniEnv->GetStringUTFChars(
                                                                          (jstring)result, NULL);

                                                                      return Value(runtime,
                                                                                   String::createFromUtf8(
                                                                                       runtime, str));
                                                              });

        jsiRuntime.global().setProperty(jsiRuntime, "getBuildId", move(getBuildId));


        auto getDeviceName = Function::createFromHostFunction(jsiRuntime,
                                                              PropNameID::forAscii(jsiRuntime,
                                                                                   "getDeviceName"),
                                                              0,
                                                              [](Runtime &runtime,
                                                                 const Value &thisValue,
                                                                 const Value *arguments,
                                                                 size_t count) -> Value
                                                              {
                                                                      JNIEnv *jniEnv = GetJniEnv();

                                                                      java_class = jniEnv->GetObjectClass(
                                                                          java_object);
                                                                      jmethodID getDeviceName = jniEnv->GetMethodID(
                                                                          java_class, "getDeviceName",
                                                                          "()Ljava/lang/String;");
                                                                      jobject result = jniEnv->CallObjectMethod(
                                                                          java_object, getDeviceName);
                                                                      const char *str = jniEnv->GetStringUTFChars(
                                                                          (jstring)result, NULL);

                                                                      return Value(runtime,
                                                                                   String::createFromUtf8(
                                                                                       runtime, str));
                                                              });

        jsiRuntime.global().setProperty(jsiRuntime, "getDeviceName", move(getDeviceName));

        auto getFingerprint = Function::createFromHostFunction(jsiRuntime,
                                                               PropNameID::forAscii(jsiRuntime,
                                                                                    "getFingerprint"),
                                                               0,
                                                               [](Runtime &runtime,
                                                                  const Value &thisValue,
                                                                  const Value *arguments,
                                                                  size_t count) -> Value
                                                               {
                                                                       JNIEnv *jniEnv = GetJniEnv();

                                                                       java_class = jniEnv->GetObjectClass(
                                                                           java_object);
                                                                       jmethodID getFingerprint = jniEnv->GetMethodID(
                                                                           java_class, "getFingerprint",
                                                                           "()Ljava/lang/String;");
                                                                       jobject result = jniEnv->CallObjectMethod(
                                                                           java_object, getFingerprint);
                                                                       const char *str = jniEnv->GetStringUTFChars(
                                                                           (jstring)result, NULL);

                                                                       return Value(runtime,
                                                                                    String::createFromUtf8(
                                                                                        runtime, str));
                                                               });

        jsiRuntime.global().setProperty(jsiRuntime, "getFingerprint", move(getFingerprint));

        auto getHost = Function::createFromHostFunction(jsiRuntime,
                                                        PropNameID::forAscii(jsiRuntime,
                                                                             "getHost"),
                                                        0,
                                                        [](Runtime &runtime,
                                                           const Value &thisValue,
                                                           const Value *arguments,
                                                           size_t count) -> Value
                                                        {
                                                                JNIEnv *jniEnv = GetJniEnv();

                                                                java_class = jniEnv->GetObjectClass(
                                                                    java_object);
                                                                jmethodID getHost = jniEnv->GetMethodID(
                                                                    java_class, "getHost",
                                                                    "()Ljava/lang/String;");
                                                                jobject result = jniEnv->CallObjectMethod(
                                                                    java_object, getHost);
                                                                const char *str = jniEnv->GetStringUTFChars(
                                                                    (jstring)result, NULL);

                                                                return Value(runtime,
                                                                             String::createFromUtf8(
                                                                                 runtime, str));
                                                        });
        jsiRuntime.global().setProperty(jsiRuntime, "getHost", move(getHost));

        auto getDeviceID = Function::createFromHostFunction(jsiRuntime,
                                                            PropNameID::forAscii(jsiRuntime,
                                                                                 "getBatteryLevel"),
                                                            0,
                                                            [](Runtime &runtime,
                                                               const Value &thisValue,
                                                               const Value *arguments,
                                                               size_t count) -> Value
                                                            {
                                                                    JNIEnv *jniEnv = GetJniEnv();

                                                                    java_class = jniEnv->GetObjectClass(
                                                                        java_object);
                                                                    jmethodID getDeviceID = jniEnv->GetMethodID(
                                                                        java_class, "getDeviceID",
                                                                        "()Ljava/lang/String;");
                                                                    jobject result = jniEnv->CallObjectMethod(
                                                                        java_object, getDeviceID);
                                                                    const char *str = jniEnv->GetStringUTFChars(
                                                                        (jstring)result, NULL);

                                                                    return Value(runtime,
                                                                                 String::createFromUtf8(
                                                                                     runtime, str));
                                                            });

        jsiRuntime.global().setProperty(jsiRuntime, "getDeviceID", move(getDeviceID));

         auto getHardware = Function::createFromHostFunction(jsiRuntime,
                                                             PropNameID::forAscii(jsiRuntime,
                                                                                  "getHardware"),
                                                             0,
                                                             [](Runtime &runtime,
                                                                const Value &thisValue,
                                                                const Value *arguments,
                                                                size_t count) -> Value
                                                             {
                                                                     JNIEnv *jniEnv = GetJniEnv();

                                                                     java_class = jniEnv->GetObjectClass(
                                                                         java_object);
                                                                     jmethodID getHardware = jniEnv->GetMethodID(
                                                                         java_class, "getHardware",
                                                                         "()Ljava/lang/String;");
                                                                     jobject result = jniEnv->CallObjectMethod(
                                                                         java_object, getHardware);
                                                                     const char *str = jniEnv->GetStringUTFChars(
                                                                         (jstring)result, NULL);

                                                                     return Value(runtime,
                                                                                  String::createFromUtf8(
                                                                                      runtime, str));
                                                             });
        jsiRuntime.global().setProperty(jsiRuntime, "getHardware", move(getHardware));


        auto getIPAddress = Function::createFromHostFunction(jsiRuntime,
                                                             PropNameID::forAscii(jsiRuntime,
                                                                                  "getIPAddress"),
                                                             0,
                                                             [](Runtime &runtime,
                                                                const Value &thisValue,
                                                                const Value *arguments,
                                                                size_t count) -> Value
                                                             {
                                                                     JNIEnv *jniEnv = GetJniEnv();

                                                                     java_class = jniEnv->GetObjectClass(
                                                                         java_object);
                                                                     jmethodID getIPAddress = jniEnv->GetMethodID(
                                                                         java_class, "getIPAddress",
                                                                         "()Ljava/lang/String;");
                                                                     jobject result = jniEnv->CallObjectMethod(
                                                                         java_object, getIPAddress);
                                                                     const char *str = jniEnv->GetStringUTFChars(
                                                                         (jstring)result, NULL);

                                                                     return Value(runtime,
                                                                                  String::createFromUtf8(
                                                                                      runtime, str));
                                                             });
        jsiRuntime.global().setProperty(jsiRuntime, "getIPAddress", move(getIPAddress));

        auto getManufacturer = Function::createFromHostFunction(jsiRuntime,
                                                                PropNameID::forAscii(jsiRuntime,
                                                                                     "getManufacturer"),
                                                                0,
                                                                [](Runtime &runtime,
                                                                   const Value &thisValue,
                                                                   const Value *arguments,
                                                                   size_t count) -> Value
                                                                {
                                                                        JNIEnv *jniEnv = GetJniEnv();

                                                                        java_class = jniEnv->GetObjectClass(
                                                                            java_object);
                                                                        jmethodID getManufacturer = jniEnv->GetMethodID(
                                                                            java_class, "getManufacturer",
                                                                            "()Ljava/lang/String;");
                                                                        jobject result = jniEnv->CallObjectMethod(
                                                                            java_object, getManufacturer);
                                                                        const char *str = jniEnv->GetStringUTFChars(
                                                                            (jstring)result, NULL);

                                                                        return Value(runtime,
                                                                                     String::createFromUtf8(
                                                                                         runtime, str));
                                                                });

        jsiRuntime.global().setProperty(jsiRuntime, "getManufacturer", move(getManufacturer));

        auto getSecurityPatch = Function::createFromHostFunction(jsiRuntime,
                                                                 PropNameID::forAscii(jsiRuntime,
                                                                                      "getSecurityPatch"),
                                                                 0,
                                                                 [](Runtime &runtime,
                                                                    const Value &thisValue,
                                                                    const Value *arguments,
                                                                    size_t count) -> Value
                                                                 {
                                                                         JNIEnv *jniEnv = GetJniEnv();

                                                                         java_class = jniEnv->GetObjectClass(
                                                                             java_object);
                                                                         jmethodID getSecurityPatch = jniEnv->GetMethodID(
                                                                             java_class, "getSecurityPatch",
                                                                             "()Ljava/lang/String;");
                                                                         jobject result = jniEnv->CallObjectMethod(
                                                                             java_object, getSecurityPatch);
                                                                         const char *str = jniEnv->GetStringUTFChars(
                                                                             (jstring)result, NULL);

                                                                         return Value(runtime,
                                                                                      String::createFromUtf8(
                                                                                          runtime, str));
                                                                 });
        jsiRuntime.global().setProperty(jsiRuntime, "getSecurityPatch", move(getSecurityPatch));
        auto getPhoneNumber = Function::createFromHostFunction(jsiRuntime,
                                                                 PropNameID::forAscii(jsiRuntime,
                                                                                      "getPhoneNumber"),
                                                                 0,
                                                                 [](Runtime &runtime,
                                                                    const Value &thisValue,
                                                                    const Value *arguments,
                                                                    size_t count) -> Value
                                                                 {
                                                                         JNIEnv *jniEnv = GetJniEnv();

                                                                         java_class = jniEnv->GetObjectClass(
                                                                             java_object);
                                                                         jmethodID getPhoneNumber = jniEnv->GetMethodID(
                                                                             java_class, "getPhoneNumber",
                                                                             "()Ljava/lang/String;");
                                                                         jobject result = jniEnv->CallObjectMethod(
                                                                             java_object, getPhoneNumber);
                                                                         const char *str = jniEnv->GetStringUTFChars(
                                                                             (jstring)result, NULL);

                                                                         return Value(runtime,
                                                                                      String::createFromUtf8(
                                                                                          runtime, str));
                                                                 });
        jsiRuntime.global().setProperty(jsiRuntime, "getPhoneNumber", move(getPhoneNumber));

}

extern "C" JNIEXPORT void JNICALL
Java_com_rnjsideviceinfo_JSIDeviceInfoModule_nativeInstall(JNIEnv *env, jobject thiz, jlong jsi)
{

        auto runtime = reinterpret_cast<facebook::jsi::Runtime *>(jsi);

        if (runtime)
        {
                example::install(*runtime);
                install(*runtime);
        }

        env->GetJavaVM(&java_vm);
        java_object = env->NewGlobalRef(thiz);
}
