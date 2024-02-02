#import "ViewController.h"
#import "EngineInterface.h"

#define BUTTON_ACTION_RELEASE 0
#define BUTTON_ACTION_PRESS 1

@interface ViewController ()
{
  CADisplayLink* _displayLink;
  MTKView* _metalView;
  EngineInterface* _engine;
}
@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
    
  // Set up the Metal view
  _metalView = [[MTKView alloc] initWithFrame:self.view.bounds];
  _metalView.device = MTLCreateSystemDefaultDevice();

  // Ensure the content scale factor matches the screen's scale
  _metalView.contentScaleFactor = UIScreen.mainScreen.scale;

  // Set the drawable size to match the view's size in pixels
  CGSize drawableSize = CGSizeMake(_metalView.bounds.size.width * _metalView.contentScaleFactor, _metalView.bounds.size.height * _metalView.contentScaleFactor);
  _metalView.drawableSize = drawableSize;

  _metalView.paused = YES; // Pause the MTKView's default loop
  _metalView.enableSetNeedsDisplay = NO;
  [self.view addSubview:_metalView];

  _engine = [[EngineInterface alloc] init];
  CAMetalLayer* metalLayer = (CAMetalLayer*)_metalView.layer;

  // Pass the drawable size (in pixels) to the engine
  [_engine initEngine: (__bridge void *)(metalLayer)
           withWidth: drawableSize.width
           andHeight: drawableSize.height];
  [self setupGameLoop];
}

- (void)setupGameLoop {
  _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(gameLoop)];
  [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)gameLoop {
  [_engine updateEngine];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
  [super touchesBegan:touches withEvent:event];
  [self handleTouches:touches withAction:BUTTON_ACTION_PRESS]; // Press action
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
  [super touchesMoved:touches withEvent:event];
  [self handleTouches:touches withAction:-1]; // Negative number for move
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
  [super touchesEnded:touches withEvent:event];
  [self handleTouches:touches withAction:BUTTON_ACTION_RELEASE]; // Release action
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
  [super touchesCancelled:touches withEvent:event];
  [self handleTouches:touches withAction:BUTTON_ACTION_RELEASE]; // Release action for cancelled as well
}

- (void)handleTouches:(NSSet<UITouch *> *)touches withAction:(int)action {
  int fingerIndex = 0;
  for (UITouch *touch in touches) {
    CGPoint touchPoint = [touch locationInView:self.view];
    touchPoint.x = touchPoint.x / self.view.frame.size.width * _metalView.drawableSize.width;
    touchPoint.y = touchPoint.y / self.view.frame.size.height * _metalView.drawableSize.height;
    [_engine touchCallback:fingerIndex withAction:action andX:(int)touchPoint.x andY:(int)touchPoint.y];
    fingerIndex++;
  }
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

- (void)dealloc {
  [_displayLink invalidate];
  [_engine shutdown];
}

@end
