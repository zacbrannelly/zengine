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

    // Ensure contentView is layer-backed
    [contentView setWantsLayer:YES];

    CAMetalLayer *metalLayer = [CAMetalLayer layer];

    // Match the layer's scale with the window's backing scale factor
    CGFloat scaleFactor = [window backingScaleFactor];
    metalLayer.contentsScale = scaleFactor;

    // Set the layer as the contentView's layer
    [contentView setLayer:metalLayer];

    // Adjust the drawable size based on the view's size and scale factor
    CGSize viewSize = contentView.bounds.size;
    metalLayer.drawableSize = CGSizeMake(viewSize.width * scaleFactor, viewSize.height * scaleFactor);

    return metalLayer;
#else
    // TODO: Figure out how to pass the view to the renderer
    return nil;
#endif
  }
}
