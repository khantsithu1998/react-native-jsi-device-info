# react-native-jsi-device-info

[![npm version](https://badge.fury.io/js/react-native-jsi-device-info.svg)](http://badge.fury.io/js/react-native-jsi-device-info)
[![npm total downloads](https://img.shields.io/npm/dt/react-native-jsi-device-info.svg)](https://img.shields.io/npm/dt/react-native-jsi-device-info.svg)
[![npm monthly downloads](https://img.shields.io/npm/dm/react-native-jsi-device-info.svg)](https://img.shields.io/npm/dm/react-native-jsi-device-info.svg)
[![npm weekly downloads](https://img.shields.io/npm/dw/react-native-jsi-device-info.svg)](https://img.shields.io/npm/dw/react-native-jsi-device-info.svg)

Device Information Through JSI for [React Native](https://github.com/facebook/react-native).

## Installation

### Vanilla React Native

yarn 

```sh
yarn add react-native-jsi-device-info
```

or

npm
```sh
npm install react-native-jsi-device-info
```

And 

```sh
cd ios && pod install
```

### Expo

```sh
expo install react-native-jsi-device-info
expo prebuild
```

## Basic usage example
```tsx
import JSIDeviceInfo from "react-native-jsi-device-info";

JSIDeviceInfo.getDeviceName() // returns Device Name 
// Example
// iOS: "Khant Si Thu's iPhone 7"
// Android: ?
// Windows: ?
```
Run the example app for more.


## API

Note that many APIs are platform-specific. If there is no implementation for a platform, then the "default" return values you will receive are `"unknown"` for string, `-1` for number, and `false` for boolean. Arrays and Objects will be empty (`[]` and `{}` respectively).

The example app in this repository shows an example usage of every single API, consult the example app if you have questions, and if you think you see a problem make sure you can reproduce it using the example app before reporting it, thank you.

| Method                                                            | Return Type         |  iOS | Android |
| ----------------------------------------------------------------- | ------------------- | :--: | :-----: | 
| [getAndroidId()](#getandroidid)                                   | `string`            |  ❌  |   ✅    | 
| [getApiLevel()](#getapilevel)                                     | `number`            |  ❌  |   ✅    |  
| [getApplicationName()](#getapplicationname)                       | `string`            |  ✅  |   ✅    |  
| [getAvailableLocationProviders()](#getAvailableLocationProviders) | `Object`            |  🚧  |   🚧    |  
| [getBaseOs()](#getbaseOs)                                         | `string`            |  ❌  |   ✅    | 
| [getBuildId()](#getbuildid)                                       | `string`            |  ✅  |   ✅    | 
| [getBatteryLevel()](#getbatterylevel)                             | `string`            |  ✅  |   ✅    |  
| [getBootloader()](#getbootloader)                                 | `string`            |  🚧  |   🚧    |   
| [getBrand()](#getbrand)                                           | `string`            |  ✅  |   ✅    |   
| [getBuildNumber()](#getbuildnumber)                               | `string`            |  ✅  |   ✅    |   
| [getBundleId()](#getbundleid)                                     | `string`            |  ✅  |   ✅    |  
| [isCameraPresent()](#iscamerapresent)                             | `boolean`           |  🚧  |   🚧    |  
| [getCarrier()](#getcarrier)                                       | `string`            |  🚧  |   🚧    |   
| [getCodename()](#getcodename)                                     | `string`            |  🚧  |   🚧    |   
| [getDevice()](#getdevice)                                         | `string`            |  🚧  |   🚧    |   
| [getDeviceId()](#getdeviceid)                                     | `string`            |  🚧  |   🚧    |   
| [getDeviceType()](#getDeviceType)                                 | `string`            |  🚧  |   🚧    |   
| [getDisplay()](#getdisplay)                                       | `string`            |  🚧  |   🚧    |   
| [getDeviceName()](#getdevicename)                                 | `string`            |  ✅  |   ✅    |   
| [getDeviceToken()](#getdevicetoken)                               | `string`            |  ✅  |   ❌    |   
| [getFirstInstallTime()](#getfirstinstalltime)                     | `number`            |  🚧  |   🚧    |   
| [getFingerprint()](#getfingerprint)                               | `string`            |  ❌  |   ✅    |   
| [getFontScale()](#getfontscale)                                   | `number`            |  🚧  |   🚧    |   
| [getFreeDiskStorage()](#getfreediskstorage)                       | `number`            |  🚧  |   🚧    |   
| [getFreeDiskStorageOld()](#getfreediskstorageold)                 | `number`            |  🚧  |   🚧    |   
| [getHardware()](#gethardware)                                     | `string`            |  🚧  |   🚧    |   
| [getHost()](#gethost)                                             | `string`            |  🚧  |   🚧    |   
| [getIpAddress()](#getipaddress)                                   | `string`            |  ✅  |   ✅    |   
| [getIncremental()](#getincremental)                               | `string`            |  🚧  |   🚧    |   
| [getInstallerPackageName()](#getinstallerpackagename)             | `string`            |  🚧  |   🚧    |   
| [getInstallReferrer()](#getinstallreferrer)                       | `string`            |  🚧  |   🚧    |   
| [getInstanceId()](#getinstanceid)                                 | `string`            |  🚧  |   🚧    |   
| [getLastUpdateTime()](#getlastupdatetime)                         | `number`            |  🚧  |   🚧    |   
| [getMacAddress()](#getmacaddress)                                 | `string`            |  🚧  |   🚧    |   
| [getManufacturer()](#getmanufacturer)                             | `string`            |  ✅  |   ✅    |   
| [getMaxMemory()](#getmaxmemory)                                   | `number`            |  🚧  |   🚧    |      
| [getModel()](#getmodel)                                           | `string`            |  ✅  |   ✅    |  
| [getPhoneNumber()](#getphonenumber)                               | `string`            |  🚧  |   🚧    |  
| [getPowerState()](#getpowerstate)                                 | `object`            |  🚧  |   🚧    |  
| [getProduct()](#getproduct)                                       | `string`            |  🚧  |   🚧    |  
| [getPreviewSdkInt()](#getPreviewSdkInt)                           | `number`            |  🚧  |   🚧    |  
| [getReadableVersion()](#getreadableversion)                       | `string`            |  🚧  |   🚧    |  
| [getSerialNumber()](#getserialnumber)                             | `string`            |  🚧  |   🚧    |  
| [getSecurityPatch()](#getsecuritypatch)                           | `string`            |  ❌  |   ✅    |  
| [getSystemAvailableFeatures()](#getSystemAvailableFeatures)       | `string[]`          |  🚧  |   🚧    |  
| [getSystemName()](#getsystemname)                                 | `string`            |  🚧  |   🚧    |  
| [getSystemVersion()](#getsystemversion)                           | `string`            |  🚧  |   🚧    |  
| [getTags()](#gettags)                                             | `string`            |  🚧  |   🚧    |  
| [getType()](#gettype)                                             | `string`            |  🚧  |   🚧    |  
| [getTotalDiskCapacity()](#gettotaldiskcapacity)                   | `number`            |  🚧  |   🚧    |  
| [getTotalDiskCapacityOld()](#gettotaldiskcapacityold)             | `number`            |  🚧  |   🚧    |  
| [getTotalMemory()](#gettotalmemory)                               | `number`            |  🚧  |   🚧    |  
| [getUniqueId()](#getuniqueid)                                     | `string`            |  🚧  |   🚧    |  
| [getUsedMemory()](#getusedmemory)                                 | `number`            |  🚧  |   🚧    |  
| [getUserAgent()](#getuseragent)                                   | `string`            |  🚧  |   🚧    |  
| [getVersion()](#getversion)                                       | `string`            |  🚧  |   🚧    |  
| [getBrightness()](#getBrightness)                                 | `number`            |  🚧  |   🚧    |  
| [hasGms()](#hasGms)                                               | `boolean`           |  🚧  |   🚧    |  
| [hasHms()](#hasHms)                                               | `boolean`           |  🚧  |   🚧    |  
| [hasNotch()](#hasNotch)                                           | `boolean`           |  🚧  |   🚧    |  
| [hasSystemFeature()](#hassystemfeaturefeature)                    | `boolean`           |  🚧  |   🚧    |  
| [isAirplaneMode()](#isairplanemode)                               | `boolean`           |  🚧  |   🚧    |  
| [isBatteryCharging()](#isbatterycharging)                         | `boolean`           |  🚧  |   🚧    |  
| [isEmulator()](#isemulator)                                       | `boolean`           |  🚧  |   🚧    |  
| [isKeyboardConnected()](#iskeyboardconnected)                     | `boolean`           |  🚧  |   🚧    |  
| [isLandscape()](#isLandscape)                                     | `boolean`           |  🚧  |   🚧    |  
| [isLocationEnabled()](#isLocationEnabled)                         | `boolean`           |  🚧  |   🚧    |  
| [isMouseConnected()](#ismouseconneted)                            | `boolean`           |  🚧  |   🚧    |   
| [isHeadphonesConnected()](#isHeadphonesConnected)                 | `boolean`           |  🚧  |   🚧    |   
| [isPinOrFingerprintSet()](#ispinorfingerprintset)                 | `boolean`           |  🚧  |   🚧    |  
| [isTablet()](#istablet)                                           | `boolean`           |  🚧  |   🚧    |   
| [isTabletMode()](#istabletmode)                                   | `boolean`           |  🚧  |   🚧    |   
| [supported32BitAbis()](#supported32BitAbis)                       | `string[]`          |  🚧  |   🚧    |   
| [supported64BitAbis()](#supported64BitAbis)                       | `string[]`          |  🚧  |   🚧    |  
| [supportedAbis()](#supportedAbis)                                 | `string[]`          |  🚧  |   🚧    |  
| [syncUniqueId()](#syncuniqueid)                                   | `string`            |  🚧  |   🚧    |   


## Thanks to these libraries & their authors:
The initial work done by authors of the following libraries has helped a lot in keeping this repo updated.

- [react-native-mmkv](https://github.com/mrousavy/react-native-mmkv/)
- [react-native-reanimated](https://github.com/software-mansion/react-native-reanimated/)
- [react-native-jsi-template](https://github.com/ammarahm-ed/react-native-jsi-template)
- [react-native-device-info](https://github.com/react-native-device-info/react-native-device-info)

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT
