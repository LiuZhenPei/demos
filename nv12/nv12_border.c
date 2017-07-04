#include <stdio.h>
#include <stdlib.h>

/* Set up the rectangle border size */
const int border = 5;

int nv12_border(char *pic, int pic_w, int pic_h, int rect_x, int rect_y, int rect_w, int rect_h, int R, int G, int B)
{
    FILE *fpic = fopen(pic, "rb+");
    FILE *fout = fopen("outputFile.nv12", "wb+");

    /* Allocate memory for NV12 */
    unsigned char *buf = (unsigned char *)malloc(pic_w * pic_h * 3 / 2);

    /* Read the file memory to buffer */
    fread(buf, 1, pic_w * pic_h * 3 / 2, fpic);

    /* RGB convert YUV */
    int Y, U, V;
    Y =  0.299  * R + 0.587  * G + 0.114  * B;
    U = -0.1687 * R + 0.3313 * G + 0.5    * B + 128;
    V =  0.5    * R - 0.4187 * G - 0.0813 * B + 128;

    /* Locking the scope of rectangle border range */
    int j, k;
    for(j = rect_y; j < rect_y + rect_h; j++){
        for(k = rect_x; k < rect_x + rect_w; k++){
            if (k < (rect_x + border) || k > (rect_x + rect_w - border) || j < (rect_y + border) || j > (rect_y + rect_h - border)){

                /* Components of YUV's storage address index */
                int y_index = j * pic_w + k;
                int u_index = (y_index / 2 - pic_w / 2 * ((j + 1) / 2)) * 2 + pic_w * pic_h;
                int v_index = u_index + 1;

                /* set up YUV's conponents value of rectangle border */
                buf[y_index] =  Y ;
                buf[u_index] =  U ;
                buf[v_index] =  V ;
            }
        }
    }

    /* Flush the buffer to the output file */
    fwrite(buf, 1, pic_w * pic_h * 3 / 2, fout);

    /* Don't forget to free the allocation memory */
    free(buf);

    /* close the file */
    fclose(fpic);
    fclose(fout);

    return 0;
}

int main()
{
    char* picture = "videotestsrc_1920x1080.nv12";
    nv12_border(picture, 1920, 1080, 1000, 500, 150, 170, 0, 0, 255);

    return 0;
}
