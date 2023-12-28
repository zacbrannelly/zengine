#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

#if !TARGET_OS_IPHONE
#import <Cocoa/Cocoa.h>
#endif

namespace bgfx {
  void* setupMetalLayer(void* wnd) {
#if !TARGET_OS_IPHONE
    NSWindow *window = (NSWindow*)wnd;
    NSView *contentView = [window contentView];
    [contentView setWantsLayer:YES];
    CAMetalLayer *res = [CAMetalLayer layer];
    [contentView setLayer:res];
    return res;
#else
    // TODO: Figure out how to pass the view to the renderer
    return nil;
#endif
  }
}
