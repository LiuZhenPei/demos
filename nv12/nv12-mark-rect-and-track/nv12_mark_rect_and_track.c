#include <stdio.h>
#include <stdlib.h>

#define uchar unsigned char

typedef struct rectangle{
    int x;
    int y;
    int w;
    int h;
    char YUV[3];
    uchar thin;
}RECT;

void RgbToYuv(uchar RGB[], char YUV[]);
void AddColarToYUV(uchar* pic, int pic_w, int pic_h, int x, int y, char YUV[]);
void FindAxis(RECT rect[], int n, int rect_all_axis[]);
void DrawWidthLine(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int axis[]);
void DrawHeightLine(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int axis[]);
void BresenhamLine(uchar* pic, int pic_w, int pic_h, int start[], int end[], char YUV[]);
void DrawTrack(uchar* pic, int pic_w, int pic_h, int point_axis[], int c, char YUV[]);
void NV12MarkRect(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int point_axis[], int count);

int main()
{
    /*****************************************************/
    /****                                             ****/
    /****             Configure Parameter             ****/
    /****                                             ****/
    /*****************************************************/

    /* Config the nv12's pixel size */
    const int pict_w  = 1920;
    const int pict_h  = 1080;
    int picSize = pict_w * pict_h * 1.5;

    /* Allocate memory for nv12 */
    uchar* picBuf = (unsigned char *)malloc(picSize);

    /* RGB convert to YUV */
    uchar RGBblue[3] = {255, 0, 0};
    char YUVblue[3];
    RgbToYuv(RGBblue, YUVblue);

    /* Config rectangle's quanlity and parameter */
    int n = 2;
    uchar t = 1;
    RECT Rect[] = {
        {
            .x = 0,
            .y = 0,
            .w = 96,
            .h = 96,
            .thin = t,
            .YUV[0] = YUVblue[0],
            .YUV[1] = YUVblue[1],
            .YUV[2] = YUVblue[2] },
        {
            .x = 144,
            .y = 144,
            .w = 128,
            .h = 128,
            .thin = t,
            .YUV[0] = YUVblue[0],
            .YUV[1] = YUVblue[1],
            .YUV[2] = YUVblue[2] }
    };

    /* Config point axis's count and draw the point at NV12 */
    int c = 30;
    int point[] = { 12,233, 132,15, 142,22, 312,34, 112,45, 212,58, 127,62, 132,72, 212,84, 129,91,
                    112,66, 187,38, 155,13, 145,56, 134,23, 165,56, 176,67, 189,87, 174,52, 121,99,
                    212,66, 287,38, 255,13, 245,56, 234,23, 265,56, 276,67, 289,87, 274,52, 221,99 };

    /* Config input and output path */
    char input[255]  = "videotestsrc_1920x1080.nv12";
    char output[255] = "outputFile.nv12";

    /******************************************************/

    FILE *fin  = fopen(input,  "rb");
    FILE *fout = fopen(output, "wb+");

    /* Read file data to buffer */
    fread(picBuf, 1, picSize, fin);

    /* Draw rectangle and point to nv12 */
    NV12MarkRect(picBuf, pict_w, pict_h, Rect, n, point, c);

    /* Write data of buf to fout */
    fwrite(picBuf, 1, picSize, fout);

    /* Free the allocation memory */
    free(picBuf);

    /* Close the file */
    fclose(fin);
    fclose(fout);

    return 0;
}

void RgbToYuv(uchar RGB[3], char YUV[3])
{
    /* RGB convert to YUV */
    YUV[0] =  (0.257 * RGB[0]) + (0.504 * RGB[1]) + (0.098 * RGB[2]) + 16 ;
    YUV[1] = -(0.148 * RGB[0]) - (0.291 * RGB[1]) + (0.439 * RGB[2]) + 128;
    YUV[2] =  (0.439 * RGB[0]) - (0.368 * RGB[1]) - (0.071 * RGB[2]) + 128;

}

void AddColarToYUV(uchar* pic, int pic_w, int pic_h, int x, int y, char YUV[])
{
    int y_index, u_index, v_index;
    y_index = y * pic_w + x;
    u_index = ((y >> 1) + pic_h) * pic_w + x - (x & 1);
    v_index = u_index + 1;

    pic[y_index] = YUV[0];
    pic[u_index] = YUV[1];
    pic[v_index] = YUV[2];
}

void FindAxis(RECT rect[], int n, int axis[])
{
    axis[0] = axis[2] = axis[4] = rect[n].x;
    axis[6] = rect[n].x + rect[n].w - rect[n].thin;
    axis[1] = rect[n].y;
    axis[3] = rect[n].y + rect[n].h - rect[n].thin;
    axis[5] = axis[7] = rect[n].y + rect[n].thin;
}

void DrawWidthLine(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int axis[])
{
    int i, x, y;
    for (i = 0; i < 4; i += 2)
        for (y = axis[i+1]+rect[n].thin-1; y >= axis[i+1]; y--)
            for (x = axis[i]+rect[n].w-1; x >= axis[i]; x--)
                AddColarToYUV(pic, pic_w, pic_h, x, y, rect[n].YUV);
}

void DrawHeightLine(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int axis[])
{
    int i, x, y;
    for (i = 4; i < 8; i += 2)
        for (x = axis[i]+rect[n].thin-1; x >= axis[i]; x--)
            for (y = axis[i+1]+rect[n].h-(2*rect[n].thin)-1; y >= axis[i+1]-1; y--)
                AddColarToYUV(pic, pic_w, pic_h, x, y, rect[n].YUV);
}

void BresenhamLine(uchar* pic, int pic_w, int pic_h, int start[], int end[], char YUV[])
{
     int dx = end[0] - start[0];
     int dy = end[1] - start[1];
     int ux = ((dx > 0) << 1) - 1;
     int uy = ((dy > 0) << 1) - 1;
     int x = start[0], y = start[1];

     int eps = 0;
     dx = abs(dx);
     dy = abs(dy);

     if (dx > dy){
         for (x = start[0]; x != end[0]+ux; x += ux){
             AddColarToYUV(pic, pic_w, pic_h, x, y, YUV);
             eps += dy;
             if ((eps << 1) >= dx){
                  y += uy;
                  eps -= dx;
              }
         }
     }
     else {
         for (y = start[1]; y != end[1]+uy; y += uy){
             AddColarToYUV(pic, pic_w, pic_h, x, y, YUV);
             eps += dx;
             if ((eps << 1) >= dy){
                  x += ux;
                  eps -= dy;
              }
         }
     }
}

void DrawTrack(uchar* pic, int pic_w, int pic_h, int point_axis[], int c, char YUV[])
{
    int i;
    int start[2];
    int end[2];

    for (i = 0; i <= 2*(c-2); i += 2){

        start[0] = point_axis[i];
        start[1] = point_axis[i+1];
        end[0] = point_axis[i+2];
        end[1] = point_axis[i+3];

        BresenhamLine(pic, pic_w, pic_h, start, end, YUV);
    }
}

void NV12MarkRect(uchar* pic, int pic_w, int pic_h, RECT rect[], int num, int point_axis[], int count)
{
    int i;
    int axis[8];

    for (i = num-1; i >= 0; i--){

        FindAxis(rect, i, axis);
        DrawWidthLine(pic, pic_w, pic_h, rect, i, axis);
        DrawHeightLine(pic, pic_w, pic_h, rect, i, axis);
    }

    uchar tRGB[3] = {255, 0, 128};
    char tYUV[3];
    RgbToYuv(tRGB, tYUV);

    DrawTrack(pic, pic_w, pic_h, point_axis, count, tYUV);
}
