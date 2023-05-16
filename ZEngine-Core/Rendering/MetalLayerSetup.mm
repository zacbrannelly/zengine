#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <Cocoa/Cocoa.h>

namespace bgfx {
  void* setupMetalLayer(void* wnd) {
    NSWindow *window = (NSWindow*)wnd;
    NSView *contentView = [window contentView];
    [contentView setWantsLayer:YES];
    CAMetalLayer *res = [CAMetalLayer layer];
    [contentView setLayer:res];
    return res;
  }
}
