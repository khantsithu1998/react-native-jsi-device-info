//@ts-ignore we want to ignore everything
// else in global except what we need to access.
// Maybe there is a better way to do this.
import { NativeModules } from 'react-native';

// Installing JSI Bindings as done by
// https://github.com/mrousavy/react-native-mmkv

//@ts-ignore
const JSIDeviceInfo: {
  getApiLevel(): string;
  getApplicationName() : string;
  getBaseOS(): string;
  getBatteryLevel(): string;
  getBootloader(): string;
  getBundleId(): string;
  getBuildNumber(): string;
  getDeviceID(): string;
  getDeviceName(): string;
  getFingerprint(): string;
  getHardware(): string; 
  getHost():string;
  getIPAddress(): string;
  getManufacturer(): string;
  getPhoneNunber(): string;
  getSecurityPatch(): string;
  helloWorld(): string;
  //@ts-ignore
} = global;

export function isLoaded() {
  return typeof JSIDeviceInfo.getDeviceName === 'function';
}

if (!isLoaded()) {
  const result = NativeModules.SimpleJsi?.install();
  if (!result && !isLoaded())
    throw new Error('JSI bindings were not installed for: SimpleJsi Module');

  if (!isLoaded()) {
    throw new Error('JSI bindings were not installed for: SimpleJsi Module');
  }
}

export default JSIDeviceInfo;
