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

- (NSString *) getApplicationName {
    NSString *displayName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
    NSString *bundleName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"];
    return displayName ? displayName : bundleName;
}

- (NSString *) getBatteryLevel {
    return [NSString stringWithFormat:@"%f", [UIDevice currentDevice].batteryLevel];
}

- (NSString *) getBundleId {
    return [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleIdentifier"];
}

- (NSString *) getBuildNumber {
    return [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
}

- (NSString *) getDeviceName {
    UIDevice *currentDevice = [UIDevice currentDevice];
    return currentDevice.name;
}

- (NSString *) getDeviceID {
    return [[[UIDevice currentDevice] identifierForVendor] UUIDString];
}

- (NSString *) getModel {
    struct utsname systemInfo;
    uname(&systemInfo);
    
    return [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
}



//- (NSString *) getIpAddress {
//    NSString *address = @"0.0.0.0";
//    struct ifaddrs *interfaces = NULL;
//    struct ifaddrs *temp_addr = NULL;
//    int success = 0;
//    // retrieve the current interfaces - returns 0 on success
//    success = getifaddrs(&interfaces);
//    if (success == 0) {
//        // Loop through linked list of interfaces
//        temp_addr = interfaces;
//        while(temp_addr != NULL) {
//            sa_family_t addr_family = temp_addr->ifa_addr->sa_family;
//            // Check for IPv4 or IPv6-only interfaces
//            if(addr_family == AF_INET || addr_family == AF_INET6) {
//                NSString* ifname = [NSString stringWithUTF8String:temp_addr->ifa_name];
//                    if(
//                        // Check if interface is en0 which is the wifi connection the iPhone
//                        // and the ethernet connection on the Apple TV
//                        [ifname isEqualToString:@"en0"] ||
//                        // Check if interface is en1 which is the wifi connection on the Apple TV
//                        [ifname isEqualToString:@"en1"]
//                    ) {
//                        const struct sockaddr_in *addr = (const struct sockaddr_in*)temp_addr->ifa_addr;
//                        socklen_t addr_len = addr_family == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;
//                        char addr_buffer[addr_len];
//                        // We use inet_ntop because it also supports getting an address from
//                        // interfaces that are IPv6-only
//                        const char *netname = inet_ntop(addr_family, &addr->sin_addr, addr_buffer, addr_len);
//
//                         // Get NSString from C String
//                        address = [NSString stringWithUTF8String:netname];
//                    }
//            }
//            temp_addr = temp_addr->ifa_next;
//        }
//    }

static void install(jsi::Runtime &jsiRuntime, SimpleJsi *simpleJsi) {
    
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

    auto getBuildNumber = Function::createFromHostFunction(jsiRuntime,
                                                          PropNameID::forAscii(jsiRuntime,
                                                                               "getBuildNumber"),
                                                          0,
                                                          [simpleJsi](Runtime &runtime,
                                                                   const Value &thisValue,
                                                                   const Value *arguments,
                                                                   size_t count) -> Value {
        
        jsi::String buildNumber = convertNSStringToJSIString(runtime, [simpleJsi getBuildNumber]);
        
        return Value(runtime, buildNumber);
    });

    jsiRuntime.global().setProperty(jsiRuntime,"getBuildNumber", move(getBuildNumber));

    auto getBundleId = Function::createFromHostFunction(jsiRuntime,
                                                          PropNameID::forAscii(jsiRuntime,
                                                                               "getBundleId"),
                                                          0,
                                                          [simpleJsi](Runtime &runtime,
                                                                   const Value &thisValue,
                                                                   const Value *arguments,
                                                                   size_t count) -> Value {
        
        jsi::String bundleId = convertNSStringToJSIString(runtime, [simpleJsi getBundleId]);
        
        return Value(runtime, bundleId);
    });

    jsiRuntime.global().setProperty(jsiRuntime,"getBundleId", move(getBundleId));
    
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

    auto getModel = Function::createFromHostFunction(jsiRuntime,
                                                          PropNameID::forAscii(jsiRuntime,
                                                                               "getModel"),
                                                          0,
                                                          [simpleJsi](Runtime &runtime,
                                                                   const Value &thisValue,
                                                                   const Value *arguments,
                                                                   size_t count) -> Value {
        
        jsi::String model = convertNSStringToJSIString(runtime, [simpleJsi getModel]);
        
        return Value(runtime, model);
    });

    jsiRuntime.global().setProperty(jsiRuntime, "getModel", move(getModel));

}
@end
