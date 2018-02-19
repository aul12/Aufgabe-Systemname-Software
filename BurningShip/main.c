#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdlib.h>

#include "burning_ship.h"

int main(int argc, char** argv) {
    double x_center,y_center,d;
    long width,height,i;
    x_center = y_center = 0.5;
    d = 5;
    width = height = 300;
    i = 100;

    int c;
    while((c=getopt(argc, argv, "x:y:d:w:h:i:")) != -1) {
        switch (c) {
            case 'x':
                x_center = strtod(optarg, 0);
                break;
            case 'y':
                y_center = strtod(optarg, 0);
                break;
            case 'd':
                d = strtod(optarg, 0);
                break;
            case 'w':
                width = strtol(optarg, 0, 10);
                break;
            case 'h':
                height = strtol(optarg, 0, 10);
                break;
            case 'i':
                i = strtol(optarg, 0, 10);
                break;
            default:
                printf("You fucked up!\n");
                break;
        }
    }


    GdkPixbuf* pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8,
                                       width, height);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);

    double numberPerPixel = d/width;

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            guchar* pp = pixels + y * rowstride + x * n_channels;
            double x_pos = numberPerPixel * (x - width/2) - x_center;
            double y_pos = numberPerPixel * (y - width/2) - y_center;

            long steps = getSteps(x_pos, y_pos, i);
            uint8_t color[3];
            getColor(steps, color, i);

            pp[0] = color[0]; /* red */
            pp[1] = color[1]; /* green */
            pp[2] = color[2]; /* blue */
        }
    }
    gdk_pixbuf_save (pixbuf, "image.jpg", "jpeg",
                     NULL, "quality", "100", NULL);
}