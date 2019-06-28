#include <nanogui/nanogui.h>
#import <Cocoa/Cocoa.h>

#if defined(NANOGUI_USE_METAL)
#  import <Metal/Metal.h>
#  import <QuartzCore/CAMetalLayer.h>
#  include "darwin.h"
#endif

NAMESPACE_BEGIN(nanogui)

std::vector<std::string> file_dialog(const std::vector<std::pair<std::string, std::string>> &filetypes,
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

void *metal_init(void *nswin_) {
    NSWindow *nswin = (__bridge NSWindow *) nswin_;
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device)
        throw std::runtime_error("init_metal(): unable to create system default device.");
    CAMetalLayer* layer = [CAMetalLayer layer];
    if (!layer)
        throw std::runtime_error("init_metal(): unable to create layer.");
    bool wide_gamut = [nswin canRepresentDisplayGamut: NSDisplayGamutP3];
    layer.device = device;
    layer.pixelFormat = wide_gamut ? MTLPixelFormatBGR10A2Unorm : MTLPixelFormatBGRA8Unorm;
    layer.displaySyncEnabled = false;
    nswin.contentView.layer = layer;
    nswin.contentView.wantsLayer = YES;
    return (__bridge_retained void *) layer;
}

void metal_release(void *layer_) {
    CAMetalLayer *layer = (__bridge_transfer CAMetalLayer *) layer_;
    (void) layer;
}

void metal_set_size(void *layer_, const Vector2i &size) {
    CAMetalLayer *layer = (__bridge CAMetalLayer *) layer_;
    layer.drawableSize = CGSizeMake(size.x(), size.y());
}

#endif

NAMESPACE_END(nanogui)
