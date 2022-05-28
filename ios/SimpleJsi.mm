#include <ifaddrs.h>
#import "SimpleJsi.h"
#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <jsi/jsi.h>
#import "example.h"
#import <sys/utsname.h>
#import "YeetJSIUtils.h"
#import <React/RCTBridge+Private.h>


using namespace facebook::jsi;
using namespace std;

@implementation SimpleJsi

@synthesize bridge = _bridge;
@synthesize methodQueue = _methodQueue;

RCT_EXPORT_MODULE()

+ (BOOL)requiresMainQueueSetup {
    
    return YES;
}

// Installing JSI Bindings as done by
// https://github.com/mrousavy/react-native-mmkv
RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install)
{
    RCTBridge* bridge = [RCTBridge currentBridge];
    RCTCxxBridge* cxxBridge = (RCTCxxBridge*)bridge;
    if (cxxBridge == nil) {
        return @false;
    }

    auto jsiRuntime = (jsi::Runtime*) cxxBridge.runtime;
    if (jsiRuntime == nil) {
        return @false;
    }

    example::install(*(facebook::jsi::Runtime *)jsiRuntime);
    install(*(facebook::jsi::Runtime *)jsiRuntime, self);
  
   
    return @true;
}


- (NSString *) getModel {
    struct utsname systemInfo;
    uname(&systemInfo);
    
    return [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
}

- (NSString *) getDeviceName {
    UIDevice *currentDevice = [UIDevice currentDevice];
    return currentDevice.name;
}

- (NSString *) getApplicationName {
    NSString *displayName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
    NSString *bundleName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"];
    return displayName ? displayName : bundleName;
}

- (NSString *) getDeviceID {
    return [[[UIDevice currentDevice] identifierForVendor] UUIDString];
}

- (NSString *) getBatteryLevel {
    return [NSString stringWithFormat:@"%f", [UIDevice currentDevice].batteryLevel];
}


- (void) setItem:(NSString * )key :(NSString *)value {
    NSUserDefaults *standardUserDefaults = [NSUserDefaults standardUserDefaults];
    [standardUserDefaults setObject:value forKey:key];
    [standardUserDefaults synchronize];
}

- (NSString *)getItem:(NSString *)key {
    NSUserDefaults *standardUserDefaults = [NSUserDefaults standardUserDefaults];
    return [standardUserDefaults stringForKey:key];
}


static void install(jsi::Runtime &jsiRuntime, SimpleJsi *simpleJsi) {
    auto getDeviceName = Function::createFromHostFunction(jsiRuntime,
                                                          PropNameID::forAscii(jsiRuntime,
                                                                               "getDeviceName"),
                                                          0,
                                                          [simpleJsi](Runtime &runtime,
                                                                   const Value &thisValue,
                                                                   const Value *arguments,
                                                                   size_t count) -> Value {
        
        jsi::String deviceName = convertNSStringToJSIString(runtime, [simpleJsi getDeviceName]);
        
        return Value(runtime, deviceName);
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "getDeviceName", move(getDeviceName));

    auto getDeviceID = Function::createFromHostFunction(jsiRuntime,
                                                          PropNameID::forAscii(jsiRuntime,
                                                                               "getDeviceID"),
                                                          0,
                                                          [simpleJsi](Runtime &runtime,
                                                                   const Value &thisValue,
                                                                   const Value *arguments,
                                                                   size_t count) -> Value {
        
        jsi::String deviceID = convertNSStringToJSIString(runtime, [simpleJsi getDeviceID]);
        
        return Value(runtime, deviceID);
    });

    jsiRuntime.global().setProperty(jsiRuntime, "getDeviceID", move(getDeviceID));

    auto getApplicationName = Function::createFromHostFunction(jsiRuntime,PropNameID::forAscii(jsiRuntime,
                                                                                               "getApplicationName"),
                                                               0,
                                                               [simpleJsi](Runtime &runtime,
                                                                            const Value &thisValue,
                                                                            const Value *arguments,
                                                                            size_t count) -> Value {
        
        jsi::String applicationName = convertNSStringToJSIString(runtime, [simpleJsi getApplicationName]);
        
        return Value(runtime, applicationName);
    });

    jsiRuntime.global().setProperty(jsiRuntime, "getApplicationName", move(getApplicationName));

    auto getBatteryLevel = Function::createFromHostFunction(jsiRuntime,
                                                          PropNameID::forAscii(jsiRuntime,
                                                                               "getBatteryLevel"),
                                                          0,
                                                          [simpleJsi](Runtime &runtime,
                                                                   const Value &thisValue,
                                                                   const Value *arguments,
                                                                   size_t count) -> Value {
        
        jsi::String batteryLevel = convertNSStringToJSIString(runtime, [simpleJsi getBatteryLevel]);
        
        return Value(runtime, batteryLevel);
    });

    jsiRuntime.global().setProperty(jsiRuntime,"getBatteryLevel", move(getBatteryLevel));

    
    auto setItem = Function::createFromHostFunction(jsiRuntime,
                                                    PropNameID::forAscii(jsiRuntime,
                                                                         "setItem"),
                                                    2,
                                                    [simpleJsi](Runtime &runtime,
                                                             const Value &thisValue,
                                                             const Value *arguments,
                                                             size_t count) -> Value {
        
        NSString *key = convertJSIStringToNSString(runtime, arguments[0].getString(runtime));
        NSString *value = convertJSIStringToNSString(runtime, arguments[1].getString(runtime));
        
        [simpleJsi setItem:key :value];
        
        return Value(true);
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "setItem", move(setItem));
    
    
    auto getItem = Function::createFromHostFunction(jsiRuntime,
                                                    PropNameID::forAscii(jsiRuntime,
                                                                         "getItem"),
                                                    0,
                                                    [simpleJsi](Runtime &runtime,
                                                             const Value &thisValue,
                                                             const Value *arguments,
                                                             size_t count) -> Value {
        
        NSString *key = convertJSIStringToNSString(runtime, arguments[0].getString(runtime));
        
        NSString *value = [simpleJsi getItem:key];
        
        return Value(runtime, convertNSStringToJSIString(runtime, value));
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "getItem", move(getItem));
}


@end
