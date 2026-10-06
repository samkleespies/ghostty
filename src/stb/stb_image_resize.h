/* Compatibility entry point for Ghostty's existing linear uint8 resizes.
 * Implementation: stb_image_resize2.h from nothings/stb.
 * Keep all channels independent, matching the old resize API (including alpha).
 */
#ifndef GHOSTTY_STB_IMAGE_RESIZE_COMPAT_H
#define GHOSTTY_STB_IMAGE_RESIZE_COMPAT_H
#include <limits.h>
#include "stb_image_resize2.h"
#ifdef __cplusplus
extern "C" {
#endif
int stbir_resize_uint8(const unsigned char *input, int input_w, int input_h,
                      int input_stride, unsigned char *output, int output_w,
                      int output_h, int output_stride, int channels);
#ifdef STB_IMAGE_RESIZE_IMPLEMENTATION
int stbir_resize_uint8(const unsigned char *input, int input_w, int input_h,
                      int input_stride, unsigned char *output, int output_w,
                      int output_h, int output_stride, int channels) {
    stbir_pixel_layout layout;
    switch (channels) {
        case 1: layout = STBIR_1CHANNEL; break;
        case 2: layout = STBIR_2CHANNEL; break;
        case 3: layout = STBIR_RGB; break;
        case 4: layout = STBIR_4CHANNEL; break;
        default: return 0;
    }
    if (!input || !output || input_w <= 0 || input_h <= 0 || output_w <= 0 || output_h <= 0)
        return 0;
    // Match Ghostty's PNG dimension limit and bound filter-support arithmetic.
    if (input_w > 131072 || input_h > 131072 || output_w > 131072 || output_h > 131072)
        return 0;
    // The legacy API uses int dimensions/strides; reject unrepresentable buffers.
    if (input_w > INT_MAX / channels || output_w > INT_MAX / channels)
        return 0;
    int input_row = input_w * channels, output_row = output_w * channels;
    if (input_stride == 0) input_stride = input_row;
    if (output_stride == 0) output_stride = output_row;
    if (input_stride < input_row || output_stride < output_row ||
        input_h > INT_MAX / input_stride || output_h > INT_MAX / output_stride)
        return 0;
    return stbir_resize_uint8_linear(input, input_w, input_h, input_stride,
                                    output, output_w, output_h, output_stride, layout) != NULL;
}
#endif
#ifdef __cplusplus
}
#endif
#endif
