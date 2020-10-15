#include <nanogui/nanogui.h>
#import <Cocoa/Cocoa.h>

#if defined(NANOGUI_USE_METAL)
#  import <Metal/Metal.h>
#  import <QuartzCore/CAMetalLayer.h>
#endif

NAMESPACE_BEGIN(nanogui)

std::vector<std::string>
file_dialog(const std::vector<std::pair<std::string, std::string>> &filetypes,
            bool save, bool multiple) {
    if (save && multiple)
        throw std::invalid_argument("file_dialog(): 'save' and 'multiple' must not both be true.");

    std::vector<std::string> result;
    if (save) {
        NSSavePanel *saveDlg = [NSSavePanel savePanel];

        NSMutableArray *types = [NSMutableArray new];
        for (size_t idx = 0; idx < filetypes.size(); ++idx)
            [types addObject: [NSString stringWithUTF8String: filetypes[idx].first.c_str()]];

        [saveDlg setAllowedFileTypes: types];

        if ([saveDlg runModal] == NSModalResponseOK)
            result.emplace_back([[[saveDlg URL] path] UTF8String]);
    } else {
        NSOpenPanel *openDlg = [NSOpenPanel openPanel];

        [openDlg setCanChooseFiles:YES];
        [openDlg setCanChooseDirectories:NO];
        [openDlg setAllowsMultipleSelection:multiple];
        NSMutableArray *types = [NSMutableArray new];
        for (size_t idx = 0; idx < filetypes.size(); ++idx)
            [types addObject: [NSString stringWithUTF8String: filetypes[idx].first.c_str()]];

        [openDlg setAllowedFileTypes: types];

        if ([openDlg runModal] == NSModalResponseOK) {
            for (NSURL* url in [openDlg URLs]) {
                result.emplace_back((char*) [[url path] UTF8String]);
            }
        }
    }
    return result;
}

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

void metal_init() {
    if (s_metal_device || s_metal_command_queue)
        throw std::runtime_error("init_metal(): already initialized!");

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device)
        throw std::runtime_error("init_metal(): unable to create system default device.");

    id<MTLCommandQueue> command_queue = [device newCommandQueue];
    if (!command_queue)
        throw std::runtime_error("init_metal(): unable to create command queue.");

    s_metal_device = (__bridge_retained void *) device;
    s_metal_command_queue = (__bridge_retained void *) command_queue;
}

void metal_shutdown() {
    (void) (__bridge_transfer id<MTLDevice>) s_metal_command_queue;
    (void) (__bridge_transfer id<MTLCommandQueue>) s_metal_device;
}

void* metal_device() { return s_metal_device; }
void* metal_command_queue() { return s_metal_command_queue; }

void metal_window_init(void *nswin_, bool float_buffer) {
    CAMetalLayer *layer = [CAMetalLayer layer];
    if (!layer)
        throw std::runtime_error("init_metal(): unable to create layer.");
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    nswin.contentView.layer = layer;
    nswin.contentView.wantsLayer = YES;
    nswin.contentView.layerContentsPlacement = NSViewLayerContentsPlacementTopLeft;
    layer.device = (__bridge id<MTLDevice>) s_metal_device;
    layer.colorspace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    layer.contentsScale = nswin.backingScaleFactor;
    if (float_buffer) {
        layer.wantsExtendedDynamicRangeContent = YES;
        layer.pixelFormat = MTLPixelFormatRGBA16Float;
    } else {
        layer.wantsExtendedDynamicRangeContent = NO;
        layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    }
    layer.displaySyncEnabled = NO;
    layer.allowsNextDrawableTimeout = NO;
    layer.framebufferOnly = NO;
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

void* metal_window_layer(void *nswin_) {
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    return (__bridge void *) nswin.contentView.layer;
}

void* metal_window_next_drawable(void *nswin_) {
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    CAMetalLayer *layer = (CAMetalLayer *) nswin.contentView.layer;
    id<MTLDrawable> drawable = layer.nextDrawable;
    return (__bridge_retained void *) drawable;
}

void *metal_drawable_texture(void *drawable_) {
    id<CAMetalDrawable> drawable = (__bridge id<CAMetalDrawable>) drawable_;
    return (__bridge void *) drawable.texture;
}

void metal_present_and_release_drawable(void *drawable_) {
    id<CAMetalDrawable> drawable = (__bridge_transfer id<CAMetalDrawable>) drawable_;
    [drawable present];
}

#endif

NAMESPACE_END(nanogui)
