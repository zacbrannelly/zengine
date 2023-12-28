#import "ViewController.h"
#import "EngineInterface.h"

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
  _metalView.paused = YES; // Pause the MTKView's default loop
  _metalView.enableSetNeedsDisplay = NO;
  [self.view addSubview:_metalView];

  _engine = [[EngineInterface alloc] init];
  CAMetalLayer* metalLayer = (CAMetalLayer*)_metalView.layer;
  [_engine initEngine: (__bridge void *)(metalLayer)
            withWidth: _metalView.bounds.size.width
            andHeight: _metalView.bounds.size.height];
  [self setupGameLoop];
}

- (void)setupGameLoop {
  _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(gameLoop)];
  [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)gameLoop {
  [_engine updateEngine];
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
