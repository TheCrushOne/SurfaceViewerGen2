#include "HeightMapConverter.h"

#include <stdlib.h>
#include <stdio.h>
#include <png.h>

using namespace converter;

HeightMapConverter::HeightMapConverter(const colreg::ModuleGuard<database::iSVGMDatabaseController> db)
   : m_databaseController(db)
   , m_row_pointers(nullptr)
{
}

bool HeightMapConverter::Convert(const char* srcPath, const char* dstPath)
{
   readDataFromPng(srcPath);
   convertToDatabaseFormat();
   m_databaseController->SaveExternalData(dstPath);
}

void HeightMapConverter::readDataFromPng(const char* srcPath)
{
   int width, height;
   png_byte color_type;
   png_byte bit_depth;

   FILE* fp = fopen(srcPath, "rb");

   png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (!png)
      abort();

   png_infop info = png_create_info_struct(png);
   if (!info)
      abort();

   if (setjmp(png_jmpbuf(png)))
      abort();

   png_init_io(png, fp);

   png_read_info(png, info);

   width = png_get_image_width(png, info);
   height = png_get_image_height(png, info);
   color_type = png_get_color_type(png, info);
   bit_depth = png_get_bit_depth(png, info);

   if (bit_depth == 16)
      png_set_strip_16(png);

   if (color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb(png);

   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_expand_gray_1_2_4_to_8(png);

   if (png_get_valid(png, info, PNG_INFO_tRNS))
      png_set_tRNS_to_alpha(png);

   if (color_type == PNG_COLOR_TYPE_RGB ||
      color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

   if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      png_set_gray_to_rgb(png);

   png_read_update_info(png, info);

   if (m_row_pointers)
      abort();

   m_row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
   for (int y = 0; y < height; y++)
   {
      m_row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
   }

   png_read_image(png, m_row_pointers);

   fclose(fp);
}