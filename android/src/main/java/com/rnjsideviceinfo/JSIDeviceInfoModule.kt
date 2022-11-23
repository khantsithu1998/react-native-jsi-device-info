package com.rnjsideviceinfo

import android.Manifest
import android.content.Context
import android.content.pm.PackageInfo
import android.content.pm.PackageManager
import android.hardware.camera2.CameraManager
import android.hardware.Camera
import android.location.LocationManager
import android.media.AudioManager
import android.net.wifi.WifiManager
import android.os.BatteryManager
import android.os.Build
import android.provider.Settings
import android.telephony.TelephonyManager
import android.util.Log
import androidx.annotation.RequiresApi
import androidx.core.app.ActivityCompat
import com.facebook.react.bridge.Arguments
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.bridge.WritableArray
import com.facebook.react.bridge.WritableMap
import com.facebook.react.module.annotations.ReactModule


@ReactModule(name = JSIDeviceInfoModule.NAME)
class JSIDeviceInfoModule(reactContext: ReactApplicationContext?) :
    ReactContextBaseJavaModule(reactContext) {
  private external fun nativeInstall(jsiPtr: Long, docDir: String)
  override fun getName(): String {
    return NAME
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  fun install(): Boolean {
    return try {
      System.loadLibrary("cpp")
      val context = reactApplicationContext
      nativeInstall(context.javaScriptContextHolder.get(), context.filesDir.absolutePath)
      true
    } catch (exception: Exception) {
      false
    }
  }

  val deviceName: String
    get() {
      val manufacturer = Build.MANUFACTURER
      val model = Build.MODEL
      return if (model.startsWith(manufacturer)) {
        model
      } else {
        "$manufacturer $model"
      }
    }

  val apiLevel: String
    get() = Build.VERSION.SDK_INT.toString()

  val isAirplaneMode: Boolean
    get() {
      val isAirplaneMode: Boolean =
          if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR1) {
            Settings.System.getInt(
                reactApplicationContext.contentResolver,
                Settings.System.AIRPLANE_MODE_ON,
                0
            ) != 0
          } else {
            Settings.Global.getInt(
                reactApplicationContext.contentResolver,
                Settings.Global.AIRPLANE_MODE_ON,
                0
            ) != 0
          }
      return isAirplaneMode
    }

  val appName: String
    get() =
        reactApplicationContext
            .applicationInfo
            .loadLabel(reactApplicationContext.packageManager)
            .toString()

  val availableLocationProviders: WritableMap?
    get() {
      val mLocationManager =
          reactApplicationContext.getSystemService(Context.LOCATION_SERVICE) as LocationManager
      val providersAvailability = Arguments.createMap()
      try {
        val providers = mLocationManager.getProviders(false)
        for (provider in providers) {
          providersAvailability.putBoolean(provider!!, mLocationManager.isProviderEnabled(provider))
        }
      } catch (e: Exception) {
        System.err.println("Unable to get location providers. LocationManager was null")
      }
      return providersAvailability
    }

  val baseOs: String
    get() {
      Log.d("OS","Base OS :" + Build.VERSION.BASE_OS)
      if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {

        return Build.VERSION.BASE_OS
      }
      return "unknown"
    }

  val batteryLevel: String
    @RequiresApi(Build.VERSION_CODES.LOLLIPOP)
    get() {
      val bm =
          this.reactApplicationContext.getSystemService(Context.BATTERY_SERVICE) as BatteryManager
      return bm.getIntProperty(BatteryManager.BATTERY_PROPERTY_CAPACITY).toString() + "%"
    }

  val bootloader: String
    get() = Build.BOOTLOADER.toString()

  @Throws(Exception::class)
  private fun getPackageInfo(): PackageInfo? {
    return reactApplicationContext.packageManager.getPackageInfo(
      reactApplicationContext.packageName,
      0
    )
  }

  val buildNumber: String @RequiresApi(Build.VERSION_CODES.P)
  get() = getPackageInfo()?.longVersionCode.toString();

  val buildId : String get() = Build.ID.toString()

  val deviceID: String
    get() =
        Settings.Secure.getString(
            reactApplicationContext.contentResolver,
            Settings.Secure.ANDROID_ID
        )

  val fingerprint: String
    get() = Build.FINGERPRINT

  val hardware: String?
    get() = Build.HARDWARE

  val hasGms: Boolean
    get() {
      return try {
        val googleApiAvailability =
            Class.forName("com.google.android.gms.common.GoogleApiAvailability")
        val getInstanceMethod = googleApiAvailability.getMethod("getInstance")
        val gmsObject = getInstanceMethod.invoke(null)
        val isGooglePlayServicesAvailableMethod =
            gmsObject.javaClass.getMethod("isGooglePlayServicesAvailable", Context::class.java)
        val isGMS =
            isGooglePlayServicesAvailableMethod.invoke(gmsObject, reactApplicationContext) as Int
        isGMS == 0 // ConnectionResult.SUCCESS
      } catch (e: Exception) {
        false
      }
    }

  fun hasSystemFeature(feature: String?): Boolean {
    return if (feature == null || feature == "") {
      false
    } else reactApplicationContext.packageManager.hasSystemFeature(feature)
  }

  val isHeadphonesConnected: Boolean
    get() {
      val audioManager =
          reactApplicationContext.getSystemService(Context.AUDIO_SERVICE) as AudioManager
      return audioManager.isWiredHeadsetOn || audioManager.isBluetoothA2dpOn
    }

  fun isCameraPresent(): Boolean {
    return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
      val manager =
        getReactApplicationContext().getSystemService(Context.CAMERA_SERVICE) as CameraManager
      try {
        manager.cameraIdList.isNotEmpty()
      } catch (e: Exception) {
        false
      }
    } else {
      Camera.getNumberOfCameras() > 0
    }
  }
  val host: String?
    get() = Build.HOST

  val installReferrer: String?
    get() {
      val sharedPref =
          reactApplicationContext.getSharedPreferences(
              "react-native-jsi-device-info",
              Context.MODE_PRIVATE
          )
      return sharedPref.getString("installReferrer", Build.UNKNOWN)
    }

  // @SuppressLint("DefaultLocale")

  val iPAddress: String
    get() {
      val wifiManager =
          reactApplicationContext.applicationContext.getSystemService(Context.WIFI_SERVICE) as
              WifiManager
      val ipAddress = wifiManager.connectionInfo.ipAddress
      // @SuppressLint("DefaultLocale")
      return String.format(
          "%d.%d.%d.%d",
          ipAddress and 0xff,
          ipAddress shr 8 and 0xff,
          ipAddress shr 16 and 0xff,
          ipAddress shr 24 and 0xff
      )
    }

  val manufacturer: String?
    get() = Build.MANUFACTURER

  val getPackageInfo: PackageInfo?
    get() {
      return reactApplicationContext.packageManager.getPackageInfo(
          reactApplicationContext.packageName,
          0
      )
    }

  val phoneNumber: String?
    get() {
      if (reactApplicationContext != null &&
              (reactApplicationContext.checkCallingOrSelfPermission(
                  Manifest.permission.READ_PHONE_STATE
              ) == PackageManager.PERMISSION_GRANTED ||
                  Build.VERSION.SDK_INT >= Build.VERSION_CODES.M &&
                      reactApplicationContext.checkCallingOrSelfPermission(
                          Manifest.permission.READ_SMS
                      ) == PackageManager.PERMISSION_GRANTED ||
                  Build.VERSION.SDK_INT >= Build.VERSION_CODES.O &&
                      reactApplicationContext.checkCallingOrSelfPermission(
                          Manifest.permission.READ_PHONE_NUMBERS
                      ) == PackageManager.PERMISSION_GRANTED)
      ) {
        val telMgr =
            reactApplicationContext.getSystemService(Context.TELEPHONY_SERVICE) as TelephonyManager
        if (telMgr != null) {
          try {
            return telMgr.line1Number
          } catch (e: SecurityException) {
            System.err.println(
                "getLine1Number called with permission, but threw anyway: " + e.message
            )
          }
        } else {
          System.err.println("Unable to getPhoneNumber. TelephonyManager was null")
        }
      }
      return "unknown"
    }

  val securityPatch: String
    get() =
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
          Build.VERSION.SECURITY_PATCH
        } else "unknown"

  val serialNumber: String?
    get() {
      try {
        return if (Build.VERSION.SDK_INT >= 26) {
          // There are a lot of conditions to access to getSerial api
          // For details, see https://developer.android.com/reference/android/os/Build#getSerial()
          // Rather than check each one, just try and rely on the catch below, for discussion on
          // this approach, refer to
          // https://github.com/react-native-device-info/react-native-device-info/issues/1320
          if (ActivityCompat.checkSelfPermission(
                  reactApplicationContext,
                  Manifest.permission.READ_PHONE_STATE
              ) != PackageManager.PERMISSION_GRANTED
          ) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return "unknown"
          }
          Build.getSerial()
        } else {
          Build.SERIAL
        }
      } catch (e: Exception) {
        // This is almost always a PermissionException. We will log it but return unknown
        System.err.println("getSerialNumber failed, it probably should not be used: " + e.message)
      }
      return "unknown"
    }

  val systemAvailableFeatures: WritableArray?
    get() {
      val featureList = reactApplicationContext.packageManager.systemAvailableFeatures
      val promiseArray = Arguments.createArray()
      for (f in featureList) {
        if (f.name != null) {
          promiseArray.pushString(f.name)
        }
      }
      return promiseArray
    }

  companion object {
    const val NAME = "SimpleJsi"
  }
}
