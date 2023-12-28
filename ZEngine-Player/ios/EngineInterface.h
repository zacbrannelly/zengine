#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface EngineInterface : NSObject

- (void) initEngine:(void*) nativeWindowPtr withWidth:(int) width andHeight:(int) height;
- (void) updateEngine;
- (void) shutdown;

@end
