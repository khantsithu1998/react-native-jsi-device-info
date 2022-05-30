import * as React from 'react';
import {StyleSheet, View, Text, TouchableOpacity, NativeModules, Platform} from 'react-native';
import JSIDeviceInfo, {isLoaded} from 'react-native-jsi-device-info';

export default function App() {
  const [result, setResult] = React.useState();
  const [deviceName, setDeviceName] = React.useState();

  React.useEffect(() => {
    setResult(JSIDeviceInfo.helloWorld());
    if(Platform.OS === 'android') {
      console.log("OS : ", JSIDeviceInfo.getBaseOS());
      console.log("Host : ", JSIDeviceInfo.getHost());
      console.log("Hardware : ", JSIDeviceInfo.getHardware());
      console.log("Bootloader : ", JSIDeviceInfo.getBootloader());
      console.log("Fingerprint : ", JSIDeviceInfo.getFingerprint());
      console.log("IP Address : ", JSIDeviceInfo.getIPAddress());
      console.log("Manufacturer : ", JSIDeviceInfo.getManufacturer());
    } 
    
    console.log("Device Name : ", JSIDeviceInfo.getDeviceName());
    console.log("Battery Level : ", JSIDeviceInfo.getBatteryLevel());
    console.log("Bundle ID : ", JSIDeviceInfo.getBundleId());
    console.log("Build Number : ", JSIDeviceInfo.getBuildNumber())
  }, []);

  return (
    <View style={styles.container}>
      <Text>Bindings Installed: {isLoaded().toString()}</Text>
      <Text>Result: {result}</Text>

      <TouchableOpacity
        onPress={() => {
          let value = JSIDeviceInfo.getDeviceName();
          setDeviceName(value);
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>Device Name: {deviceName}</Text>
      </TouchableOpacity>

      <TouchableOpacity
        onPress={() => {
          console.log(JSIDeviceInfo.getApplicationName())
          
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>
          Get App Name
        </Text>
      </TouchableOpacity>
      <TouchableOpacity
        onPress={() => {
          console.log(JSIDeviceInfo.getBatteryLevel())
          
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>
          Get Battery Level
        </Text>
      </TouchableOpacity>
      <TouchableOpacity
        onPress={() => {
          console.log(JSIDeviceInfo.getDeviceID())
          
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>
          Get Device ID
        </Text>
      </TouchableOpacity>
      <TouchableOpacity
        onPress={() => {
          console.log(JSIDeviceInfo.getManufacturer())
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>
          Get Manufacturer
        </Text>
      </TouchableOpacity>
      <TouchableOpacity
        onPress={() => {
          console.log(JSIDeviceInfo.getSecurityPatch())
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>
          Get Security Patch
        </Text>
      </TouchableOpacity>
      <TouchableOpacity
        onPress={() => {
          console.log(JSIDeviceInfo.getIPAddress())
          
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>
          Get IP Address
        </Text>
      </TouchableOpacity>
      <TouchableOpacity
        onPress={() => {
          console.log(JSIDeviceInfo.getApiLevel())
          
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>
          Get API Level
        </Text>
      </TouchableOpacity>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
  button: {
    width: '95%',
    alignSelf: 'center',
    height: 40,
    backgroundColor: 'green',
    alignItems: 'center',
    justifyContent: 'center',
    borderRadius: 5,
    marginVertical: 10,
  },
  buttonTxt: {
    color: 'white',
  },
});
