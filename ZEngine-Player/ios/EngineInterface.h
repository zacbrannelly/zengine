#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface EngineInterface : NSObject

- (void) initEngine:(void*) nativeWindowPtr withWidth:(int) width andHeight:(int) height;
- (void) touchCallback:(int) fingerIndex withAction:(int) action andX:(int) x andY:(int) y;
- (void) updateEngine;
- (void) shutdown;

@end
