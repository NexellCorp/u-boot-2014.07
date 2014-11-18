#include <common.h>
#include <platform.h>
#include <asm/arch/display.h>

#include "mlc.h"

#ifndef MLC_LAYER_VIDEO
#define MLC_LAYER_VIDEO     3
#endif

#ifndef MLC_LAYER_RGB_OVERLAY
#define MLC_LAYER_RGB_OVERLAY 0
#endif

void nxp_mlc_video_set_param(int module, struct nxp_mlc_video_param *param)
{
    int srcw = param->width;
    int srch = param->height;
    int dstw, dsth, hf, vf;

    if (param->right == 0)
        NX_MLC_GetScreenSize(module, &param->right, &param->bottom);

    dstw = param->right - param->left;
    dsth = param->bottom - param->top;
    hf = 1, vf = 1;

    if (srcw == dstw && srch == dsth)
        hf = 0, vf = 0;

    NX_MLC_SetFormatYUV(module, param->format);
    NX_MLC_SetVideoLayerScale(module, srcw, srch, dstw, dsth,
            (CBOOL)hf, (CBOOL)hf, (CBOOL)vf, (CBOOL)vf);
    NX_MLC_SetPosition(module, MLC_LAYER_VIDEO,
            param->left, param->top, param->right - 1, param->bottom - 1);
    NX_MLC_SetLayerPriority(module, param->priority);
    NX_MLC_SetDirtyFlag(module, MLC_LAYER_VIDEO);
    printf("%s exit\n", __func__);
}

void nxp_mlc_video_set_addr(int module, u32 lu_a, u32 cb_a, u32 cr_a, u32 lu_s, u32 cb_s, u32 cr_s)
{
    NX_MLC_SetVideoLayerStride (module, lu_s, cb_s, cr_s);
    NX_MLC_SetVideoLayerAddress(module, lu_a, cb_a, cr_a);
    NX_MLC_SetVideoLayerLineBufferPowerMode(module, CTRUE);
    NX_MLC_SetVideoLayerLineBufferSleepMode(module, CFALSE);
    NX_MLC_SetDirtyFlag(module, MLC_LAYER_VIDEO);
    printf("%s exit\n", __func__);
}

void nxp_mlc_video_run(int module)
{
	NX_MLC_SetTopDirtyFlag(module);
    NX_MLC_SetLayerEnable(module, MLC_LAYER_VIDEO, CTRUE);
    NX_MLC_SetDirtyFlag(module, MLC_LAYER_VIDEO);
    printf("%s exit\n", __func__);
}

static inline u32 _get_pixel_byte(u32 nxp_rgb_format)
{
    switch (nxp_rgb_format) {
        case NX_MLC_RGBFMT_R5G6B5:
        case NX_MLC_RGBFMT_B5G6R5:
        case NX_MLC_RGBFMT_X1R5G5B5:
        case NX_MLC_RGBFMT_X1B5G5R5:
        case NX_MLC_RGBFMT_X4R4G4B4:
        case NX_MLC_RGBFMT_X4B4G4R4:
        case NX_MLC_RGBFMT_X8R3G3B2:
        case NX_MLC_RGBFMT_X8B3G3R2:
        case NX_MLC_RGBFMT_A1R5G5B5:
        case NX_MLC_RGBFMT_A1B5G5R5:
        case NX_MLC_RGBFMT_A4R4G4B4:
        case NX_MLC_RGBFMT_A4B4G4R4:
        case NX_MLC_RGBFMT_A8R3G3B2:
        case NX_MLC_RGBFMT_A8B3G3R2:
            return 2;

        case NX_MLC_RGBFMT_R8G8B8:
        case NX_MLC_RGBFMT_B8G8R8:
            return 3;

        case NX_MLC_RGBFMT_A8R8G8B8:
        case NX_MLC_RGBFMT_A8B8G8R8:
            return 4;

        default:
            printf("%s: invalid nxp_rgb_format(0x%x)\n", __func__, nxp_rgb_format);
            return 0;
    }
}

void nxp_mlc_rgb_overlay_set_param(int module, struct nxp_mlc_rgb_overlay_param *param)
{
    u32 format = param->format;
    u32 pixelbyte = _get_pixel_byte(format);
    u32 stride = param->width * pixelbyte;
    u32 layer = MLC_LAYER_RGB_OVERLAY;
    CBOOL EnAlpha = CFALSE;

    if (format == MLC_RGBFMT_A1R5G5B5 ||
        format == MLC_RGBFMT_A1B5G5R5 ||
        format == MLC_RGBFMT_A4R4G4B4 ||
        format == MLC_RGBFMT_A4B4G4R4 ||
        format == MLC_RGBFMT_A8R3G3B2 ||
        format == MLC_RGBFMT_A8B3G3R2 ||
        format == MLC_RGBFMT_A8R8G8B8 ||
        format == MLC_RGBFMT_A8B8G8R8)
        EnAlpha = CTRUE;

    NX_MLC_SetColorInversion(module, layer, CFALSE, 0);
    NX_MLC_SetAlphaBlending(module, layer, EnAlpha, 0);
    NX_MLC_SetFormatRGB(module, layer, (NX_MLC_RGBFMT)format);
    NX_MLC_SetRGBLayerInvalidPosition(module, layer, 0, 0, 0, 0, 0, CFALSE);
    NX_MLC_SetRGBLayerInvalidPosition(module, layer, 1, 0, 0, 0, 0, CFALSE);
    NX_MLC_SetPosition(module, layer, 0, 0, param->width-1, param->height-1);

    NX_MLC_SetRGBLayerStride (module, layer, pixelbyte, stride);
    NX_MLC_SetRGBLayerAddress(module, layer, param->addr);
    NX_MLC_SetDirtyFlag(module, layer);
}

void nxp_mlc_rgb_overlay_run(int module)
{
    u32 layer = MLC_LAYER_RGB_OVERLAY;
    NX_MLC_SetLayerEnable(module, layer, CTRUE);
    NX_MLC_SetDirtyFlag(module, layer);
}
