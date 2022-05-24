//@ts-ignore we want to ignore everything
// else in global except what we need to access.
// Maybe there is a better way to do this.
import { NativeModules } from 'react-native';

// Installing JSI Bindings as done by
// https://github.com/mrousavy/react-native-mmkv

//@ts-ignore
const JSIDeviceInfo: {
  helloWorld(): string;
  multiplyWithCallback(
    x: number,
    y: number,
    callback: (z: number) => void
  ): void;
  multiply(x: number, y: number): number;
  sum(x: number, y: number) : number;
  getDeviceName(): string;
  getApiLevel(): string;
  getApplicationName() : string;
  getHost():string;
  getBatteryLevel(): string;
  getDeviceID(): string;
  getIPAddress(): string;
  getManufacturer(): string;
  getSecurityPatch(): string;
  setItem(key: string, value: string): boolean;
  getItem(key: string): string;
  foo(callback:(error:string | undefined,value:string | undefined) => void):void
  //@ts-ignore
} = global;

export function isLoaded() {
  return typeof JSIDeviceInfo.getItem === 'function';
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
