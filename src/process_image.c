#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
     int index = y * im.w * im.c + x * im.c + c;
     return im.data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    int index = (y * im.w + x) * im.c + c;
    im.data[index] = v;
}

image copy_image(image im)
{
     image copy;
    copy.w = im.w;
    copy.h = im.h;
    copy.c = im.c;

    int size = im.w * im.h * im.c;
    copy.data = malloc(size * sizeof(float));
    memcpy(copy.data, im.data, size * sizeof(float));

    return copy;
}

image rgb_to_grayscale(image im)
{
    if (im.c!= 3) {
        fprintf(stderr, "Error: Input image is not an RGB image.\n");
        exit(1);
    }

    // Create a new grayscale image with the same width and height
    image grayscale;
    grayscale.w = im.w;
    grayscale.h = im.h;
    grayscale.c = 1;

    // Allocate memory for the grayscale image data
    int size = grayscale.w * grayscale.h * grayscale.c;
    grayscale.data = malloc(size * sizeof(float));

    for (int y = 0; y < grayscale.h; y++) {
        for (int x = 0; x < grayscale.w; x++) {
            // Get the RGB values of the pixel
            float r = im.data[(y * im.w + x) * im.c];
            float g = im.data[(y * im.w + x) * im.c + 1];
            float b = im.data[(y * im.w + x) * im.c + 2];

            // Calculate the grayscale value using the luminosity method
            float gray = 0.299 * r + 0.587 * g + 0.114 * b;

            // Set the grayscale value for the pixel in the grayscale image
            grayscale.data[y * grayscale.w + x] = gray;
        }
    }

    return grayscale;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int i = 0; i < im.w * im.h; i++) {
        int index = (i * im.c) + c;
        im.data[index] += v;
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
for (int i = 0; i < im.w * im.h * im.c; i++) {
        im.data[i] = (im.data[i] < 0) ? 0 : (im.data[i] > 1) ? 1 : im.data[i];
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    image hsv;
    double r = im.w / 255.0;
    double g = im.h / 255.0;
    double b = im.c / 255.0;
    double V = three_way_max(r, g, b);
    double m = three_way_min(r, g, b);
    double diff = V - m;
    if (V == 0) {
        hsv.h = 0;
    } else {
        hsv.h = (int)((diff / V) * 100);
    }

    // Calculate H
    if (diff == 0) {
        hsv.w = 0;
    } else {
        if (V == r) {
            hsv.w = (int)(((g - b) / diff) * 60);
        } else if (V == g) {
            hsv.w = (int)((((b - r) / diff) + 2) * 60);
        } else if (V == b) {
            hsv.w = (int)((((r - g) / diff) + 4) * 60);
        }
    }

    if (hsv.w < 0) {
        hsv.w += 360;
    }
    hsv.w *= 360;
    hsv.c = (int)(V * 100);
    im=hsv;
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    image rgb;
    int h = im.w % 360; // Ensure h is within [0, 359]
    int s = im.h;
    int v = im.c;

    if (s == 0) {
        // If saturation is 0, the color is grayscale
        rgb.w = v;
        rgb.h = v;
        rgb.c = v;
    } else {
        int region = h / 60; // Determine the region in the color wheel
        int remainder = (h % 60) * 6;

        int p = (v * (100 - s)) / 100;
        int q = (v * (100 - (s * remainder) / 60)) / 100;
        int t = (v * (100 - (s * (60 - remainder)) / 60)) / 100;
        switch (region) {
            case 0:
                rgb.w = v;
                rgb.h = t;
                rgb.c = p;
                break;
            case 1:
                rgb.w = q;
                rgb.h = v;
                rgb.c = p;
                break;
            case 2:
                rgb.w = p;
                rgb.h = v;
                rgb.c = t;
                break;
            case 3:
                rgb.w = p;
                rgb.h = q;
                rgb.c = v;
                break;
            case 4:
                rgb.w = t;
                rgb.h = p;
                rgb.c = v;
                break;
            default:
                rgb.w = v;
                rgb.h = p;
                rgb.c = q;
                break;
        }

    }
    im=rgb;
}
