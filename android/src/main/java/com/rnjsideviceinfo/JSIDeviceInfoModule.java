package com.rnjsideviceinfo;

import android.content.SharedPreferences;
import android.content.Intent;
import android.os.Build;
import android.os.BatteryManager;
import android.net.wifi.WifiManager;
import android.provider.Settings;
import android.preference.PreferenceManager;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.module.annotations.ReactModule;
import com.facebook.react.bridge.ReactMethod;

@ReactModule(name = JSIDeviceInfoModule.NAME)
public class JSIDeviceInfoModule extends ReactContextBaseJavaModule {
  public static final String NAME = "SimpleJsi";

  private native void nativeInstall(long jsiPtr, String docDir);

  public JSIDeviceInfoModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      System.loadLibrary("cpp");

      ReactApplicationContext context = getReactApplicationContext();
      nativeInstall(
          context.getJavaScriptContextHolder().get(),
          context.getFilesDir().getAbsolutePath());
      return true;
    } catch (Exception exception) {
      return false;
    }
  }

  public String getModel() {
    String manufacturer = Build.MANUFACTURER;
    String model = Build.MODEL;
    if (model.startsWith(manufacturer)) {
      return model;
    } else {
      return manufacturer + " " + model;
    }
  }

  public String getLevel() {
    return String.valueOf(Build.VERSION.SDK_INT);
  }

  public String getAppName() {
    return getReactApplicationContext().getApplicationInfo().loadLabel(getReactApplicationContext().getPackageManager())
        .toString();
  }

  // public String getAppVersion(){
  // return
  // getReactApplicationContext().getPackageManager().getPackageInfo(getReactApplicationContext().getPackageName(),
  // 0).versionName;
  // }

  // public String getAppBuild(){
  // return
  // String.valueOf(getReactApplicationContext().getPackageManager().getPackageInfo(getReactApplicationContext().getPackageName(),
  // 0).versionCode);
  // }

  public String getBatteryLevel() {
    BatteryManager bm = (BatteryManager) this.getReactApplicationContext()
        .getSystemService(this.getReactApplicationContext().BATTERY_SERVICE);
    return String.valueOf(bm.getIntProperty(BatteryManager.BATTERY_PROPERTY_CAPACITY)) + "%";
  }

  public String getDeviceID() {
    return Settings.Secure.getString(getReactApplicationContext().getContentResolver(), Settings.Secure.ANDROID_ID);
  }

  public String getHost() {
    SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getReactApplicationContext());
    return prefs.getString("host", "http://");
  }

  public String getIPAddress() {
    WifiManager wm = (WifiManager) getReactApplicationContext().getApplicationContext()
        .getSystemService(getReactApplicationContext().WIFI_SERVICE);
    int ipAddress = wm.getConnectionInfo().getIpAddress();
    // @SuppressLint("DefaultLocale")
    final String formattedIpAddress = String.format("%d.%d.%d.%d",
        (ipAddress & 0xff),
        (ipAddress >> 8 & 0xff),
        (ipAddress >> 16 & 0xff),
        (ipAddress >> 24 & 0xff));
    return formattedIpAddress;
  }

  public String getManufacturer(){
    return  Build.MANUFACTURER;
  }

  public String getSecurityPatch() {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
      return Build.VERSION.SECURITY_PATCH;
    }
    return "unknown";
  }

  public void setItem(final String key, final String value) {

    SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this.getReactApplicationContext());
    SharedPreferences.Editor editor = preferences.edit();
    editor.putString(key, value);
    editor.apply();
  }

  public String getItem(final String key) {
    SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this.getReactApplicationContext());
    String value = preferences.getString(key, "");
    return value;
  }

}
