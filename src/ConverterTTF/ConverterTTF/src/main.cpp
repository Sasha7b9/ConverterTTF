#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


struct Bitmap
{
    int             rows;
    int             width;
    int             pitch;
    int             grays;
    unsigned char*  buffer;
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawBitmap(FT_Bitmap *bitmap, int x, int y)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
FT_Error GlyphToBitmap(FT_Glyph glyf, Bitmap *target, int *left, int *top, int *x_advance, int *y_advance)
{
    FT_Error error = FT_Err_Ok;

    error = FT_Glyph_To_Bitmap(&glyf, FT_RENDER_MODE_MONO, NULL, 0);

    if (error)
    {
        return error;
    }

    FT_BitmapGlyph bitmap = (FT_BitmapGlyph)glyf;
    FT_Bitmap *source = &bitmap->bitmap;

    target->rows = (int)source->rows;
    target->width = (int)source->width;
    target->pitch = source->pitch;
    target->buffer = source->buffer;
    target->grays = source->num_grays;

    *left = bitmap->left;
    *top = bitmap->top;

    *x_advance = (glyf->advance.x + 0x8000) >> 16;
    *y_advance = (glyf->advance.y + 0x8000) >> 16;

    return error;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
    FT_Library   library;
    FT_Face      face;

    FT_Error     error;
    int          x, y;

    const char *text = "Test string";

    if (FT_Init_FreeType(&library))
    {
        printf("Can not initialize FreeType library");
    }

    error = FT_New_Face(library, "../../../font/AGENCYB.TTF", 0, &face);
    if (error == FT_Err_Unknown_File_Format)
    {
        printf("Font file could be opened and read, but it appears that, its, font format is unsupported");
    }
    else if (error)
    {
        printf("File can not be opened for read, or is broken");
    }

    if (error)
    {
        return 0;
    }

    FT_GlyphSlot slot = face->glyph;

    error = FT_Set_Char_Size(face, 0, 16 * 64, 300, 300);

    error = FT_Set_Pixel_Sizes(face, 0, 16);

    x = 300;
    y = 200;

    for (unsigned n = 0; n < strlen(text); n++)
    {
        FT_UInt glyph_index = FT_Get_Char_Index(face, text[n]);

        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if (error)
        {
            continue;
        }

        error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
        if (error)
        {
            continue;
        }

        FT_Glyph glyf;

        error = FT_Get_Glyph(slot, &glyf);

        if (error)
        {
            printf("Can not get glyph");
        }

        error = FT_Glyph_To_Bitmap(&glyf, FT_RENDER_MODE_MONO, NULL, 0);

        if (error)
        {
            printf("Can not do glyph to bitmap");
        }

        FT_BitmapGlyph bitmap = (FT_BitmapGlyph)glyf;
        FT_Bitmap *source = &bitmap->bitmap;

        source = source;

        x += slot->advance.x >> 6;
        y += slot->advance.y >> 6;
    }

    return 0;
}

