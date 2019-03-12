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
int GetBit(unsigned char *buffer, int numBit)
{
    while (numBit > 7)
    {
        buffer++;
        numBit -= 8;
    }

    return ((*buffer) >> (7 - numBit)) & 0x01;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawRow(unsigned char *buffer, int numBits)
{
    for (int i = 0; i < numBits; i++)
    {
        if (GetBit(buffer, i))
        {
            printf("*");
        }
        else
        {
            printf(".");
        }
    }
    
    printf("\n");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawSymbol(unsigned char *buffer, int width, int height)
{
    int bytesInRow = (width + 8) / 8;

    if ((width % 8) == 0)
    {
        bytesInRow = width / 8;
    }

    for (int i = 0; i < height; i++)
    {
        DrawRow(buffer, width);
        buffer += bytesInRow;
    }

    printf("\n");
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

    const wchar_t *text = L"0123456789";

    if (FT_Init_FreeType(&library))
    {
        printf("Can not initialize FreeType library");
    }

    error = FT_New_Face(library, "../../../font/TECH__28.TTF", 0, &face);
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

    x = 300;
    y = 200;

    for (int i = 0; i < 3; i++)
    {
        for (int s = 0; s < 10; s++)
        {
            printf("%c", s | 0x30);
        }
    }

    printf("\n");

    if (FT_Set_Char_Size(face, 32, 32 * 64, 600, 600))
    {
        printf("can not set char size");
    }

    if (FT_Set_Pixel_Sizes(face, 32, 64))
    {
        printf("can nont set pixel sizes");
    }

    for (unsigned n = 0; n < 10; n++)
    {
        FT_UInt glyph_index = FT_Get_Char_Index(face, text[n]);

        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT))
        {
            continue;
        }

        FT_GlyphSlot slot = face->glyph;

        if (FT_Render_Glyph(slot, FT_RENDER_MODE_MONO))
        {
            continue;
        }

        FT_Glyph glyf;

        if (FT_Get_Glyph(slot, &glyf))
        {
            printf("Can not get glyph");
        }

        if (FT_Glyph_To_Bitmap(&glyf, FT_RENDER_MODE_MONO, NULL, 0))
        {
            printf("Can not do glyph to bitmap");
        }

        FT_BitmapGlyph bitmap = (FT_BitmapGlyph)glyf;
        FT_Bitmap *source = &bitmap->bitmap;

        DrawSymbol(source->buffer, source->width, source->rows);
    }

    return 0;
}

