

#ifndef SURFACE_H
#define SURFACE_H

#if defined(_POSIX_C_SOURCE)
#undef _POSIX_C_SOURCE
#endif

#include <SDL.h>
#include "pygame.h"


#define PYGAME_BLEND_ADD  0x1
#define PYGAME_BLEND_SUB  0x2
#define PYGAME_BLEND_MULT 0x3
#define PYGAME_BLEND_MIN  0x4
#define PYGAME_BLEND_MAX  0x5

#define PYGAME_BLEND_RGB_ADD  0x1
#define PYGAME_BLEND_RGB_SUB  0x2
#define PYGAME_BLEND_RGB_MULT 0x3
#define PYGAME_BLEND_RGB_MIN  0x4
#define PYGAME_BLEND_RGB_MAX  0x5

#define PYGAME_BLEND_RGBA_ADD  0x6
#define PYGAME_BLEND_RGBA_SUB  0x7
#define PYGAME_BLEND_RGBA_MULT 0x8
#define PYGAME_BLEND_RGBA_MIN  0x9
#define PYGAME_BLEND_RGBA_MAX  0x10
#define PYGAME_BLEND_PREMULTIPLIED  0x11





#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define GET_PIXEL_24(b) (b[0] + (b[1] << 8) + (b[2] << 16))
#else
#define GET_PIXEL_24(b) (b[2] + (b[1] << 8) + (b[0] << 16))
#endif

#define GET_PIXEL(pxl, bpp, source)               \
    switch (bpp)                                  \
    {                                             \
    case 2:                                       \
        pxl = *((Uint16 *) (source));             \
        break;                                    \
    case 4:                                       \
        pxl = *((Uint32 *) (source));             \
        break;                                    \
    default:                                      \
    {                                             \
        Uint8 *b = (Uint8 *) source;              \
        pxl = GET_PIXEL_24(b);                    \
    }                                             \
    break;                                        \
    }

#if IS_SDLv1
#define GET_PIXELVALS(_sR, _sG, _sB, _sA, px, fmt, ppa)               \
    _sR = ((px & fmt->Rmask) >> fmt->Rshift);                         \
    _sR = (_sR << fmt->Rloss) + (_sR >> (8 - (fmt->Rloss << 1)));     \
    _sG = ((px & fmt->Gmask) >> fmt->Gshift);                         \
    _sG = (_sG << fmt->Gloss) + (_sG >> (8 - (fmt->Gloss << 1)));     \
    _sB = ((px & fmt->Bmask) >> fmt->Bshift);                         \
    _sB = (_sB << fmt->Bloss) + (_sB >> (8 - (fmt->Bloss << 1)));     \
    if (ppa)                                                          \
    {                                                                 \
        _sA = ((px & fmt->Amask) >> fmt->Ashift);                     \
        _sA = (_sA << fmt->Aloss) + (_sA >> (8 - (fmt->Aloss << 1))); \
    }                                                                 \
    else                                                              \
    {                                                                 \
        _sA = 255;                                                    \
    }

#define GET_PIXELVALS_1(sr, sg, sb, sa, _src, _fmt)    \
    sr = _fmt->palette->colors[*((Uint8 *) (_src))].r; \
    sg = _fmt->palette->colors[*((Uint8 *) (_src))].g; \
    sb = _fmt->palette->colors[*((Uint8 *) (_src))].b; \
    sa = 255;

#define SET_PIXELVAL(px, fmt, _dR, _dG, _dB, _dA) \
    *(px) = (Uint8) SDL_MapRGB(fmt, _dR, _dG, _dB)
#else 
#define GET_PIXELVALS(_sR, _sG, _sB, _sA, px, fmt, ppa)   \
    SDL_GetRGBA(px, fmt, &(_sR), &(_sG), &(_sB), &(_sA)); \
    if (!ppa) {                                           \
        _sA = 255;                                        \
    }

#define GET_PIXELVALS_1(sr, sg, sb, sa, _src, _fmt)    \
    sr = _fmt->palette->colors[*((Uint8 *) (_src))].r; \
    sg = _fmt->palette->colors[*((Uint8 *) (_src))].g; \
    sb = _fmt->palette->colors[*((Uint8 *) (_src))].b; \
    sa = 255;


#define SET_PIXELVAL(px, fmt, _dR, _dG, _dB, _dA) \
    *(px) = (Uint8) SDL_MapRGBA(fmt, _dR, _dG, _dB, _dA)
#endif 








#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define SET_OFFSETS_24(or, og, ob, fmt)           \
    {                                             \
    or = (fmt->Rshift == 0 ? 0 :                  \
          fmt->Rshift == 8 ? 1 :                  \
                         2   );                   \
    og = (fmt->Gshift == 0 ? 0 :                  \
          fmt->Gshift == 8 ? 1 :                  \
                         2   );                   \
    ob = (fmt->Bshift == 0 ? 0 :                  \
          fmt->Bshift == 8 ? 1 :                  \
                         2   );                   \
    }

#define SET_OFFSETS_32(or, og, ob, fmt)           \
    {                                             \
    or = (fmt->Rshift == 0  ? 0 :                 \
          fmt->Rshift == 8  ? 1 :                 \
          fmt->Rshift == 16 ? 2 :                 \
                          3   );                  \
    og = (fmt->Gshift == 0  ? 0 :                 \
          fmt->Gshift == 8  ? 1 :                 \
          fmt->Gshift == 16 ? 2 :                 \
                          3   );                  \
    ob = (fmt->Bshift == 0  ? 0 :                 \
          fmt->Bshift == 8  ? 1 :                 \
          fmt->Bshift == 16 ? 2 :                 \
                          3   );                  \
    }
#else
#define SET_OFFSETS_24(or, og, ob, fmt)           \
    {                                             \
    or = (fmt->Rshift == 0 ? 2 :                  \
          fmt->Rshift == 8 ? 1 :                  \
                         0   );                   \
    og = (fmt->Gshift == 0 ? 2 :                  \
          fmt->Gshift == 8 ? 1 :                  \
                         0   );                   \
    ob = (fmt->Bshift == 0 ? 2 :                  \
          fmt->Bshift == 8 ? 1 :                  \
                         0   );                   \
    }

#define SET_OFFSETS_32(or, og, ob, fmt)           \
    {                                             \
    or = (fmt->Rshift == 0  ? 3 :                 \
          fmt->Rshift == 8  ? 2 :                 \
          fmt->Rshift == 16 ? 1 :                 \
                          0   );                  \
    og = (fmt->Gshift == 0  ? 3 :                 \
          fmt->Gshift == 8  ? 2 :                 \
          fmt->Gshift == 16 ? 1 :                 \
                          0   );                  \
    ob = (fmt->Bshift == 0  ? 3 :                 \
          fmt->Bshift == 8  ? 2 :                 \
          fmt->Bshift == 16 ? 1 :                 \
                          0   );                  \
    }
#endif


#define CREATE_PIXEL(buf, r, g, b, a, bp, ft)     \
    switch (bp)                                   \
    {                                             \
    case 2:                                       \
        *((Uint16 *) (buf)) =                     \
            ((r >> ft->Rloss) << ft->Rshift) |    \
            ((g >> ft->Gloss) << ft->Gshift) |    \
            ((b >> ft->Bloss) << ft->Bshift) |    \
            ((a >> ft->Aloss) << ft->Ashift);     \
        break;                                    \
    case 4:                                       \
        *((Uint32 *) (buf)) =                     \
            ((r >> ft->Rloss) << ft->Rshift) |    \
            ((g >> ft->Gloss) << ft->Gshift) |    \
            ((b >> ft->Bloss) << ft->Bshift) |    \
            ((a >> ft->Aloss) << ft->Ashift);     \
        break;                                    \
    }

#define LOOP_UNROLLED4(code, n, width) \
    n = (width + 3) / 4;               \
    switch (width & 3)                 \
    {                                  \
    case 0: do { code;                 \
        case 3: code;                  \
        case 2: code;                  \
        case 1: code;                  \
        } while (--n > 0);             \
    }

#define REPEAT_3(code) \
    code;              \
    code;              \
    code;

#define REPEAT_4(code) \
    code;              \
    code;              \
    code;              \
    code;


#define BLEND_ADD(tmp, sR, sG, sB, sA, dR, dG, dB, dA)  \
    tmp = dR + sR; dR = (tmp <= 255 ? tmp : 255);       \
    tmp = dG + sG; dG = (tmp <= 255 ? tmp : 255);       \
    tmp = dB + sB; dB = (tmp <= 255 ? tmp : 255);

#define BLEND_SUB(tmp, sR, sG, sB, sA, dR, dG, dB, dA) \
    tmp = dR - sR; dR = (tmp >= 0 ? tmp : 0);          \
    tmp = dG - sG; dG = (tmp >= 0 ? tmp : 0);          \
    tmp = dB - sB; dB = (tmp >= 0 ? tmp : 0);

#define BLEND_MULT(sR, sG, sB, sA, dR, dG, dB, dA) \
    dR = (dR && sR) ? (dR * sR) >> 8 : 0;          \
    dG = (dG && sG) ? (dG * sG) >> 8 : 0;          \
    dB = (dB && sB) ? (dB * sB) >> 8 : 0;

#define BLEND_MIN(sR, sG, sB, sA, dR, dG, dB, dA) \
    if(sR < dR) { dR = sR; }                      \
    if(sG < dG) { dG = sG; }                      \
    if(sB < dB) { dB = sB; }

#define BLEND_MAX(sR, sG, sB, sA, dR, dG, dB, dA) \
    if(sR > dR) { dR = sR; }                      \
    if(sG > dG) { dG = sG; }                      \
    if(sB > dB) { dB = sB; }






#define BLEND_RGBA_ADD(tmp, sR, sG, sB, sA, dR, dG, dB, dA)  \
    tmp = dR + sR; dR = (tmp <= 255 ? tmp : 255);       \
    tmp = dG + sG; dG = (tmp <= 255 ? tmp : 255);       \
    tmp = dB + sB; dB = (tmp <= 255 ? tmp : 255);       \
    tmp = dA + sA; dA = (tmp <= 255 ? tmp : 255);

#define BLEND_RGBA_SUB(tmp, sR, sG, sB, sA, dR, dG, dB, dA) \
    tmp = dR - sR; dR = (tmp >= 0 ? tmp : 0);          \
    tmp = dG - sG; dG = (tmp >= 0 ? tmp : 0);          \
    tmp = dB - sB; dB = (tmp >= 0 ? tmp : 0);          \
    tmp = dA - sA; dA = (tmp >= 0 ? tmp : 0);

#define BLEND_RGBA_MULT(sR, sG, sB, sA, dR, dG, dB, dA) \
    dR = (dR && sR) ? (dR * sR) >> 8 : 0;          \
    dG = (dG && sG) ? (dG * sG) >> 8 : 0;          \
    dB = (dB && sB) ? (dB * sB) >> 8 : 0;          \
    dA = (dA && sA) ? (dA * sA) >> 8 : 0;

#define BLEND_RGBA_MIN(sR, sG, sB, sA, dR, dG, dB, dA) \
    if(sR < dR) { dR = sR; }                      \
    if(sG < dG) { dG = sG; }                      \
    if(sB < dB) { dB = sB; }                      \
    if(sA < dA) { dA = sA; }

#define BLEND_RGBA_MAX(sR, sG, sB, sA, dR, dG, dB, dA) \
    if(sR > dR) { dR = sR; }                      \
    if(sG > dG) { dG = sG; }                      \
    if(sB > dB) { dB = sB; }                      \
    if(sA > dA) { dA = sA; }











#if 1

#if (-1 >> 1) < 0
#define ALPHA_BLEND_COMP(sC, dC, sA) ((((sC - dC) * sA + sC) >> 8) + dC)
#else
#define ALPHA_BLEND_COMP(sC, dC, sA) (((dC << 8) + (sC - dC) * sA + sC) >> 8)
#endif

#define ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB, dA) \
    do {                                            \
        if (dA)                                     \
        {                                           \
            dR = ALPHA_BLEND_COMP(sR, dR, sA);      \
            dG = ALPHA_BLEND_COMP(sG, dG, sA);      \
            dB = ALPHA_BLEND_COMP(sB, dB, sA);      \
            dA = sA + dA - ((sA * dA) / 255);       \
        }                                           \
        else                                        \
        {                                           \
            dR = sR;                                \
            dG = sG;                                \
            dB = sB;                                \
            dA = sA;                                \
        }                                           \
    } while(0)

#define ALPHA_BLEND_PREMULTIPLIED_COMP(sC, dC, sA) (sC + dC - ((dC * sA) >> 8))

#define ALPHA_BLEND_PREMULTIPLIED(tmp, sR, sG, sB, sA, dR, dG, dB, dA) \
    do {                                            \
            tmp = ALPHA_BLEND_PREMULTIPLIED_COMP(sR, dR, sA); dR = (tmp > 255 ? 255 : tmp); \
            tmp = ALPHA_BLEND_PREMULTIPLIED_COMP(sG, dG, sA); dG = (tmp > 255 ? 255 : tmp); \
            tmp = ALPHA_BLEND_PREMULTIPLIED_COMP(sB, dB, sA); dB = (tmp > 255 ? 255 : tmp); \
            dA = sA + dA - ((sA * dA) / 255);       \
    } while(0)
#elif 0

#define ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB, dA)    \
    do {                                               \
        if(sA){                                        \
            if(dA && sA < 255){                        \
                int dContrib = dA*(255 - sA)/255;      \
                dA = sA+dA - ((sA*dA)/255);            \
                dR = (dR*dContrib + sR*sA)/dA;         \
                dG = (dG*dContrib + sG*sA)/dA;         \
                dB = (dB*dContrib + sB*sA)/dA;         \
            }else{                                     \
                dR = sR;                               \
                dG = sG;                               \
                dB = sB;                               \
                dA = sA;                               \
            }                                          \
        }                                              \
    } while(0)
#endif

int
surface_fill_blend (SDL_Surface *surface, SDL_Rect *rect, Uint32 color,
                    int blendargs);

void
surface_respect_clip_rect (SDL_Surface *surface, SDL_Rect *rect);

int
pygame_AlphaBlit (SDL_Surface * src, SDL_Rect * srcrect,
                  SDL_Surface * dst, SDL_Rect * dstrect, int the_args);

int
pygame_Blit (SDL_Surface * src, SDL_Rect * srcrect,
             SDL_Surface * dst, SDL_Rect * dstrect, int the_args);

#endif 
