
#ifndef _PYGAME_FREETYPE_H_
#define _PYGAME_FREETYPE_H_

#define PGFT_PYGAME1_COMPAT
#define HAVE_PYGAME_SDL_VIDEO
#define HAVE_PYGAME_SDL_RWOPS

#include "pygame.h"
#include "pgcompat.h"

#if PY3
#   define IS_PYTHON_3
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_XFREE86_H
#include FT_TRIGONOMETRY_H


#define FT_STYLE_NORMAL     0x00
#define FT_STYLE_STRONG     0x01
#define FT_STYLE_OBLIQUE    0x02
#define FT_STYLE_UNDERLINE  0x04
#define FT_STYLE_WIDE       0x08
#define FT_STYLE_DEFAULT    0xFF


#define FT_BBOX_EXACT           FT_GLYPH_BBOX_SUBPIXELS
#define FT_BBOX_EXACT_GRIDFIT   FT_GLYPH_BBOX_GRIDFIT
#define FT_BBOX_PIXEL           FT_GLYPH_BBOX_TRUNCATE
#define FT_BBOX_PIXEL_GRIDFIT   FT_GLYPH_BBOX_PIXELS


#define FT_RFLAG_NONE                  (0)
#define FT_RFLAG_ANTIALIAS             (1 << 0)
#define FT_RFLAG_AUTOHINT              (1 << 1)
#define FT_RFLAG_VERTICAL              (1 << 2)
#define FT_RFLAG_HINTED                (1 << 3)
#define FT_RFLAG_KERNING               (1 << 4)
#define FT_RFLAG_TRANSFORM             (1 << 5)
#define FT_RFLAG_PAD                   (1 << 6)
#define FT_RFLAG_ORIGIN                (1 << 7)
#define FT_RFLAG_UCS4                  (1 << 8)
#define FT_RFLAG_USE_BITMAP_STRIKES    (1 << 9)
#define FT_RFLAG_DEFAULTS              (FT_RFLAG_HINTED | \
                                        FT_RFLAG_USE_BITMAP_STRIKES | \
                                        FT_RFLAG_ANTIALIAS)


#define FT_RENDER_NEWBYTEARRAY      0x0
#define FT_RENDER_NEWSURFACE        0x1
#define FT_RENDER_EXISTINGSURFACE   0x2

typedef struct _scale_s {
    FT_UInt x, y;
} Scale_t;
typedef FT_Angle Angle_t;

struct fontinternals_;
struct freetypeinstance_;

typedef struct {
    FT_Long font_index;
    FT_Open_Args open_args;
} pgFontId;

typedef struct {
    PyObject_HEAD
    pgFontId id;
    PyObject *path;
    int is_scalable;

    Scale_t face_size;
    FT_Int16 style;
    FT_Int16 render_flags;
    double strength;
    double underline_adjustment;
    FT_UInt resolution;
    Angle_t rotation;
    FT_Matrix transform;
    FT_Byte fgcolor[4];

    struct freetypeinstance_ *freetype;  /* Personal reference */
    struct fontinternals_ *_internals;
} pgFontObject;

#define pgFont_IS_ALIVE(o) \
    (((pgFontObject *)(o))->_internals != 0)


#define PYGAMEAPI_FREETYPE_FIRSTSLOT 0
#define PYGAMEAPI_FREETYPE_NUMSLOTS 2

#ifndef PYGAME_FREETYPE_INTERNAL

#define pgFont_Check(x) ((x)->ob_type == (PyTypeObject*)PgFREETYPE_C_API[0])
#define pgFont_Type (*(PyTypeObject*)PgFREETYPE_C_API[1])
#define pgFont_New (*(PyObject*(*)(const char*, long))PgFREETYPE_C_API[1])

#define import_pygame_freetype() \
    _IMPORT_PYGAME_MODULE(freetype, FREETYPE, PgFREETYPE_C_API)

static void *PgFREETYPE_C_API[PYGAMEAPI_FREETYPE_NUMSLOTS] = {0};
#endif /* PYGAME_FREETYPE_INTERNAL */

#endif /* _PYGAME_FREETYPE_H_ */
