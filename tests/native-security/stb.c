#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../../src/stb/stb.c"
int main(void) {
    const unsigned char png[] = {137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,1,0,0,0,1,16,6,0,0,0,79,133,24,202,0,0,0,17,73,68,65,84,120,156,99,16,50,9,171,152,181,231,255,127,0,13,250,4,105,216,42,181,35,0,0,0,0,73,69,78,68,174,66,96,130};
    int x, y, channels;
    unsigned char *image = stbi_load_from_memory(png, sizeof(png), &x, &y, &channels, 4);
    assert(image && x == 1 && y == 1 && channels == 4);
    assert(image[0] == 0x12 && image[1] == 0x56 && image[2] == 0x9a && image[3] == 0xff);
    stbi_image_free(image);
    stbi__uint16 *pixel = malloc(sizeof(*pixel)); *pixel = 1234;
    stbi__uint16 *rgba = stbi__convert_format16(pixel, 1, 4, 1, 1);
    assert(rgba && rgba[0] == 1234 && rgba[1] == 1234 && rgba[2] == 1234 && rgba[3] == 65535);
    free(rgba);
    pixel = malloc(sizeof(*pixel));
    assert(!stbi__convert_format16(pixel, 1, 4, 65536, 65536));
    for (int c = 1; c <= 4; c++) {
        unsigned char input[4*4*4], output[9*9*4], down[2*2*4];
        for (int i = 0; i < 4*4*c; i++) input[i] = (unsigned char)(17 + (i%c)*43);
        assert(stbir_resize_uint8(input, 4, 4, 0, output, 9, 9, 0, c));
        assert(stbir_resize_uint8(output, 9, 9, 0, down, 2, 2, 0, c));
        for (int i = 0; i < 2*2*c; i++) assert(down[i] == (unsigned char)(17 + (i%c)*43));
        assert(!stbir_resize_uint8(input, INT_MAX, 2, 0, output, 9, 9, 0, c));
        assert(!stbir_resize_uint8(input, 4, 4, 1, output, 9, 9, 0, c));
    }
    puts("PNG decode, conversion overflow, and 1-4 channel resize tests passed");
}
