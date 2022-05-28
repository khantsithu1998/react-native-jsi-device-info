package com.rnjsideviceinfo

import com.facebook.react.module.annotations.ReactModule
import com.rnjsideviceinfo.JSIDeviceInfoModule
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import java.lang.Exception
import android.os.Build
import android.os.BatteryManager
import android.content.Context
import android.content.SharedPreferences
import android.preference.PreferenceManager
import android.net.wifi.WifiManager
import android.provider.Settings
import androidx.annotation.RequiresApi

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
            nativeInstall(
                context.javaScriptContextHolder.get(),
                context.filesDir.absolutePath
            )
            true
        } catch (exception: Exception) {
            false
        }
    }

    val model: String
        get() {
            val manufacturer = Build.MANUFACTURER
            val model = Build.MODEL
            return if (model.startsWith(manufacturer)) {
                model
            } else {
                "$manufacturer $model"
            }
        }

    val level: String
        get() = Build.VERSION.SDK_INT.toString()

    val appName: String
        get() = reactApplicationContext.applicationInfo.loadLabel(reactApplicationContext.packageManager)
            .toString()

    val batteryLevel: String
        @RequiresApi(Build.VERSION_CODES.LOLLIPOP)
        get() {
            val bm = this.reactApplicationContext
                .getSystemService(Context.BATTERY_SERVICE) as BatteryManager
            return bm.getIntProperty(BatteryManager.BATTERY_PROPERTY_CAPACITY).toString() + "%"
        }

    val deviceID: String
        get() = Settings.Secure.getString(
            reactApplicationContext.contentResolver,
            Settings.Secure.ANDROID_ID
        )

    val host: String?
        get() {
            val prefs = PreferenceManager.getDefaultSharedPreferences(
                reactApplicationContext
            )
            return prefs.getString("host", "http://")
        }

    // @SuppressLint("DefaultLocale")
    val iPAddress: String
        get() {
            val wm = reactApplicationContext.applicationContext
                .getSystemService(Context.WIFI_SERVICE) as WifiManager
            val ipAddress = wm.connectionInfo.ipAddress
            // @SuppressLint("DefaultLocale")
            return String.format(
                "%d.%d.%d.%d",
                ipAddress and 0xff,
                ipAddress shr 8 and 0xff,
                ipAddress shr 16 and 0xff,
                ipAddress shr 24 and 0xff
            )
        }

    val manufacturer: String
        get() = Build.MANUFACTURER

    val securityPatch: String
        get() = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            Build.VERSION.SECURITY_PATCH
        } else "unknown"

    fun setItem(key: String?, value: String?) {
        val preferences =
            PreferenceManager.getDefaultSharedPreferences(this.reactApplicationContext)
        val editor = preferences.edit()
        editor.putString(key, value)
        editor.apply()
    }

    fun getItem(key: String?): String? {
        val preferences =
            PreferenceManager.getDefaultSharedPreferences(this.reactApplicationContext)
        return preferences.getString(key, "")
    }

    companion object {
        const val NAME = "SimpleJsi"
    }
}
