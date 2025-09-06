#include <nanogui/nanogui.h>
#import <Cocoa/Cocoa.h>

#if defined(NANOGUI_USE_METAL)
#  import <Metal/Metal.h>
#  import <QuartzCore/CAMetalLayer.h>
#  import <QuartzCore/CATransaction.h>
#endif

#if !defined(MAC_OS_X_VERSION_10_15) || \
    MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_15
@interface NSScreen (ForwardDeclare)
@property(readonly)
    CGFloat maximumPotentialExtendedDynamicRangeColorComponentValue;
@end
#endif

NAMESPACE_BEGIN(nanogui)

void chdir_to_bundle_parent() {
    NSString *path = [[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent];
    chdir([path fileSystemRepresentation]);
}

void disable_saved_application_state_osx() {
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"NSQuitAlwaysKeepsWindows"];
}

#if defined(NANOGUI_USE_METAL)

static void *s_metal_device = nullptr;
static void *s_metal_command_queue = nullptr;
static void *s_metal_cleanup_queue = nullptr;

void metal_init() {
    if (s_metal_device || s_metal_command_queue)
        throw std::runtime_error("init_metal(): already initialized!");

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device)
        throw std::runtime_error("init_metal(): unable to create system default device.");

    id<MTLCommandQueue> command_queue = [device newCommandQueue];
    if (!command_queue)
        throw std::runtime_error("init_metal(): unable to create command queue.");

    dispatch_queue_t metal_cleanup_queue = dispatch_queue_create("nanogui.cleanup", DISPATCH_QUEUE_SERIAL);

    s_metal_device = (__bridge_retained void *) device;
    s_metal_command_queue = (__bridge_retained void *) command_queue;
    s_metal_cleanup_queue = (__bridge_retained void *) metal_cleanup_queue;
}

void metal_shutdown() {
    if (s_metal_cleanup_queue) {
        dispatch_queue_t queue = (__bridge dispatch_queue_t) s_metal_cleanup_queue;
        dispatch_sync(queue, ^{});  // Wait for pending work
        (void) (__bridge_transfer dispatch_queue_t) s_metal_cleanup_queue;  // Transfer to ARC for release
        s_metal_cleanup_queue = nullptr;
    }

    if (s_metal_device) {
        (void) (__bridge_transfer id<MTLDevice>) s_metal_device;
        s_metal_device = nullptr;
    }

    if (s_metal_command_queue) {
        (void) (__bridge_transfer id<MTLCommandQueue>) s_metal_command_queue;
        s_metal_command_queue = nullptr;
    }
}

void* metal_device() { return s_metal_device; }
void* metal_command_queue() { return s_metal_command_queue; }
void* metal_cleanup_queue() { return s_metal_cleanup_queue; }

void metal_sync() {
    id<MTLCommandQueue> command_queue = (__bridge id<MTLCommandQueue>) s_metal_command_queue;
    id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
    [command_buffer commit];
    [command_buffer waitUntilCompleted];
}

void metal_window_init(void *nswin_, bool float_buffer) {
    CAMetalLayer *layer = [CAMetalLayer layer];
    if (!layer)
        throw std::runtime_error("init_metal(): unable to create layer.");
    [layer setOpaque: YES];
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    nswin.contentView.wantsLayer = YES;
    nswin.contentView.layer = layer;
    nswin.contentView.layerContentsPlacement = NSViewLayerContentsPlacementTopLeft;
    layer.device = (__bridge id<MTLDevice>) s_metal_device;
    layer.contentsScale = nswin.backingScaleFactor;
    if (float_buffer) {
        layer.wantsExtendedDynamicRangeContent = YES;
        layer.pixelFormat = MTLPixelFormatRGBA16Float;
        layer.colorspace = CGColorSpaceCreateWithName(kCGColorSpaceExtendedSRGB);
    } else {
        layer.wantsExtendedDynamicRangeContent = NO;
        layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        layer.colorspace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    }
    layer.maximumDrawableCount = 2;
    layer.allowsNextDrawableTimeout = NO;
    layer.framebufferOnly = YES;
    layer.presentsWithTransaction = NO;
}

void metal_window_set_vsync(void *nswin_, bool vsync) {
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    CAMetalLayer *layer = (CAMetalLayer *) nswin.contentView.layer;
    layer.displaySyncEnabled = vsync ? YES : NO;
}

std::pair<bool, bool> metal_10bit_edr_support() {
    NSArray<NSScreen *> * screens = [NSScreen screens];
    bool buffer_10bit = false,
         buffer_ext = false;

    for (NSScreen * screen in screens) {
        if ([screen canRepresentDisplayGamut: NSDisplayGamutP3])
            buffer_10bit = true; // on macOS, P3 gamut is equivalent to 10 bit color depth

        if ([screen respondsToSelector:@selector
                      (maximumPotentialExtendedDynamicRangeColorComponentValue)]) {
            if ([screen maximumPotentialExtendedDynamicRangeColorComponentValue] >= 2.f) {
                buffer_10bit = true;
                buffer_ext = true;
            }
        }
    }

    return { buffer_10bit, buffer_ext };
}

void* metal_layer(void *nswin_) {
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    CAMetalLayer *layer = (CAMetalLayer *) nswin.contentView.layer;
    return (__bridge void *) layer;
}


void metal_window_set_size(void *nswin_, const Vector2i &size) {
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    CAMetalLayer *layer = (CAMetalLayer *) nswin.contentView.layer;
    layer.drawableSize = CGSizeMake(size.x(), size.y());
}

void metal_window_set_content_scale(void *nswin_, float scale) {
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    CAMetalLayer *layer = (CAMetalLayer *) nswin.contentView.layer;

    float old_scale = layer.contentsScale;
    if (old_scale != scale) {
        [CATransaction begin];
        [CATransaction setValue:(id)kCFBooleanTrue
                        forKey:kCATransactionDisableActions];
        layer.contentsScale = scale;
        [CATransaction commit];
    }
}

void* metal_window_layer(void *nswin_) {
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    return (__bridge void *) nswin.contentView.layer;
}

void metal_window_next_drawable(void *nswin_, void **drawable_, void **texture_) {
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    CAMetalLayer *layer = (CAMetalLayer *) nswin.contentView.layer;
    id<CAMetalDrawable> drawable = layer.nextDrawable;
    *texture_ = (__bridge void *) drawable.texture;
    *drawable_ = (__bridge_retained void *) drawable;
}

void metal_present_and_release_drawable(void *drawable_) {
    id<CAMetalDrawable> drawable = (__bridge_transfer id<CAMetalDrawable>) drawable_;
    id<MTLCommandQueue> command_queue = (__bridge id<MTLCommandQueue>) s_metal_command_queue;
    id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
    [command_buffer presentDrawable: drawable];
    [command_buffer commit];
}

#endif

NAMESPACE_END(nanogui)
