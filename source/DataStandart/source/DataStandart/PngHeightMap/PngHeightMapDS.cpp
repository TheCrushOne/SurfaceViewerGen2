#include "stdafx.h"
#include "PngHeightMapDS.h"
#include "colreg/ThirdParty/md5/md5.h"

#include <filesystem>
#include <fstream>

using namespace SV;
using namespace SV::data_standart;

void PngHeightMapDataStandart::resolvePathDee()
{
   // NOTE: скорее всего не запускается...
   std::filesystem::path filePath(m_dataStandartData.file);
   if (filePath.is_relative())
      m_dataStandartData.file = (std::filesystem::path(m_baseFolder) / filePath).generic_string().c_str();
}

const png_data& PngHeightMapDataStandart::GetPngData() const
{
   if (!m_read)
   {
      m_read = true;
      fopen_s(&m_file, m_dataStandartData.file.c_str(), "rb");
      ATLASSERT(m_file);

      const_cast<PngHeightMapDataStandart*>(this)->readDataFromPng();
   }

   return m_data;
}

void PngHeightMapDataStandart::ReleasePngData()
{
   if (m_file) 
      fclose(m_file);
   m_file = nullptr;
}


void PngHeightMapDataStandart::readDataFromPng()
{
   int width, height;
   png_byte color_type;
   png_byte bit_depth;

   png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (!png)
      abort();

   png_infop info = png_create_info_struct(png);
   if (!info)
      abort();

   if (setjmp(png_jmpbuf(png)))
      abort();

   png_init_io(png, m_file);

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

   size_t byte_per_pixel = 4;
   // Так...вроде правильно
   m_data.row_count = height;
   m_data.col_count = png_get_rowbytes(png, info) / byte_per_pixel;

   png_read_image(png, m_row_pointers);

   convertToRawData();
}

// TODO: перенести коррекции в настройки и сделать отдельный модуль
// к-т преобразования градиента в высоту 0-256 -> height_min-height_max
//#define HEIGHT_CORRECTOR(h) 0.8*(h - 140.)
#define HEIGHT_CORRECTOR(h) h

void PngHeightMapDataStandart::convertToRawData()
{
   m_data.raw_data.resize(m_data.row_count);
   for (int l = 0; l < m_data.row_count; l++)
   {
      png_bytep row = m_row_pointers[l];
      m_data.raw_data[l].resize(m_data.col_count);
      for (int w = 0; w < m_data.col_count; w++)
      {
         png_bytep px = &(row[4 * w]);
         double r = (double)px[0];
         double g = (double)px[1];
         double b = (double)px[2];
         double a = (double)px[3];
         m_data.raw_data[l][w] = HEIGHT_CORRECTOR(r / 3. + g / 3. + b / 3.);
      }
   }
}

#undef HEIGHT_CORRECTOR

iDataStandart* CreatePngHeighMapDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
{
   return new PngHeightMapDataStandart(pack, base_folder, pService);
}