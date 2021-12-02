#import "FlutterGitPlugin.h"
#import "flutter_git.h"
#import <objc/runtime.h>


static FlutterGitPlugin *FlutterGitPlugin_instance = nil;

@interface FlutterGitPlugin ()

@property (nonatomic, strong) FlutterMethodChannel *channel;

@end

@implementation FlutterGitPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
    FlutterMethodChannel* channel = [FlutterMethodChannel
        methodChannelWithName:@"flutter_git"
              binaryMessenger:[registrar messenger]];
    FlutterGitPlugin* instance = [[FlutterGitPlugin alloc] init];
      instance.channel = channel;
    [registrar addMethodCallDelegate:instance channel:channel];
    // Just make sure the library is linked.
    NSLog(@"%lu", (unsigned long)&flutter_init);
    
    FlutterGitPlugin_instance = instance;
}

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    result(FlutterMethodNotImplemented);
}

+ (void)sendEvent:(const char *)name withData:(const char *)data {
    [FlutterGitPlugin_instance.channel invokeMethod:@"event"
                                          arguments:@{
        @"name": [NSString stringWithUTF8String:name],
        @"data": [NSString stringWithUTF8String:data],
    }];
}

@end
