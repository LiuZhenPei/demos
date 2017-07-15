#include <stdio.h>
#include <stdlib.h>
#include "libyuv.h"

int NV12Scale(uint8 *psrc_buf, int psrc_w, int psrc_h, uint8 *pdst_buf, int pdst_w, int pdst_h, libyuv::FilterModeEnum pfmode);

int main(int argc, const char *argv[])
{
    const int src_w = 1920;
    const int src_h = 1080;
    const int dst_w = 1280;
    const int dst_h =  720;

    libyuv::FilterModeEnum fmode = libyuv::kFilterNone;

    char inputPathname[30]  = "videotestsrc_1920x1080.nv12";
    char outputPathname[30] = "videotestdst_1280x720.nv12";

    FILE * fin  = fopen(inputPathname,  "r+");
    FILE * fout = fopen(outputPathname, "w+");

    /* Allocate memory for nv12 */
    uint8 *src_buf   = (uint8 *)malloc(src_w * src_h * 3 / 2);
    uint8 *dst_buf   = (uint8 *)malloc(dst_w * dst_h * 3 / 2);

    /* Read file data to buffer */
    fread(src_buf, sizeof(uint8), src_w * src_h * 3 / 2, fin);

    /* Scale NV12 */
    NV12Scale(src_buf, src_w, src_h, dst_buf, dst_w, dst_h, fmode);

    /* Write the data to file */
    fwrite(dst_buf, sizeof(uint8), dst_w * dst_h * 3 /2, fout);

    free(src_buf);
    free(dst_buf);

    fclose(fin);
    fclose(fout);

    return 0;
}

int NV12Scale(uint8 *psrc_buf, int psrc_w, int psrc_h, uint8 *pdst_buf, int pdst_w, int pdst_h, libyuv::FilterModeEnum pfmode)
{
    uint8 *i420_buf1 = (uint8 *)malloc(psrc_w * psrc_h * 3 / 2);
    uint8 *i420_buf2 = (uint8 *)malloc(pdst_w * pdst_h * 3 / 2);

    /* NV12_1920x1080 -> I420_1920x1080 */
    libyuv::NV12ToI420(&psrc_buf[0],                        psrc_w,
                       &psrc_buf[psrc_w * psrc_h],          psrc_w,
		       &i420_buf1[0],                       psrc_w,
		       &i420_buf1[psrc_w * psrc_h],         psrc_w / 2,
		       &i420_buf1[psrc_w * psrc_h * 5 / 4], psrc_w / 2,
		       psrc_w, psrc_h);

    /* I420_1920x1080 -> I420_1280x720 */
    libyuv::I420Scale(&i420_buf1[0],                       psrc_w,
                      &i420_buf1[psrc_w * psrc_h],         psrc_w / 2,
                      &i420_buf1[psrc_w * psrc_h * 5 / 4], psrc_w / 2,
                      psrc_w, psrc_h,
                      &i420_buf2[0],                       pdst_w,
                      &i420_buf2[pdst_w * pdst_h],         pdst_w / 2,
                      &i420_buf2[pdst_w * pdst_h * 5 / 4], pdst_w / 2,
                      pdst_w, pdst_h,
                      pfmode);

    /* I420_1280x720 -> NV12_1280x720 */
    libyuv::I420ToNV12(&i420_buf2[0],                       pdst_w,
                       &i420_buf2[pdst_w * pdst_h],         pdst_w / 2,
                       &i420_buf2[pdst_w * pdst_h * 5 / 4], pdst_w / 2,
		       &pdst_buf[0],                        pdst_w,
		       &pdst_buf[pdst_w * pdst_h],          pdst_w,
		       pdst_w,pdst_h);

    free(i420_buf1);
    free(i420_buf2);

    return 0;
}
