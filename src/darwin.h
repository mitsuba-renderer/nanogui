#pragma once
#if defined(NANOGUI_USE_METAL)

NAMESPACE_BEGIN(nanogui)

void *metal_init(void *nswin_);
void metal_release(void *layer);
void metal_set_size(void *layer, const Vector2i &size);

NAMESPACE_END(nanogui)

#endif
