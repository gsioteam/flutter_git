package com.neo.flutter_git;

import android.os.Handler;
import android.util.Log;

import androidx.annotation.NonNull;

import java.util.HashMap;
import java.util.Map;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;

/** FlutterGitPlugin */
public class FlutterGitPlugin implements FlutterPlugin, MethodCallHandler {

  static {
    System.loadLibrary("flutter_git");
  }

  /// The MethodChannel that will the communication between Flutter and native Android
  ///
  /// This local reference serves to register the plugin with the Flutter Engine and unregister it
  /// when the Flutter Engine is detached from the Activity
  private MethodChannel channel;
  private Handler handler;

  @Override
  public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {
    channel = new MethodChannel(flutterPluginBinding.getBinaryMessenger(), "flutter_git");
    channel.setMethodCallHandler(this);
    handler = new Handler();
    setup(this);
  }

  @Override
  public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
    if (call.method.equals("getPlatformVersion")) {
      result.success("Android " + android.os.Build.VERSION.RELEASE);
    } else {
      result.notImplemented();
    }
  }

  @Override
  public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
    channel.setMethodCallHandler(null);
    setup(null);
  }


  public void sendEvent(final String name, final String data) {
    handler.post(new Runnable() {
      @Override
      public void run() {
        Map<String, String> map = new HashMap<>();
        map.put("name", name);
        map.put("data", data);
        channel.invokeMethod("event", map);
      }
    });
  }

  native void setup(FlutterGitPlugin plugin);
}
