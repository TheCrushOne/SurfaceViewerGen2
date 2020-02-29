#include "stdafx.h"
#include "HeightMapConverter.h"
#include "common/utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <png.h>

using namespace converter;

HeightMapConverter::HeightMapConverter()
   : m_row_pointers(nullptr)
{
   m_databaseController.Create(SVGUtils::CurrentDllPath("SQLiteController").c_str(), "CreateSQLiteDatabaseController");
   if (!m_databaseController.IsValid())
   {
      m_lock = true;
      //MessageString(ICommunicator::MS_Error, "Can't load settings serializer!");
      return;
   }
   m_settingsSerializer.Create(SVGUtils::CurrentDllPath("SettingsHandler").c_str(), "CreateSettingsSerializer");
   if (!m_settingsSerializer.IsValid())
   {
      m_lock = true;
      //MessageString(ICommunicator::MS_Error, "Can't load settings serializer!");
      return;
   }
   m_unitDataSerializer.Create(SVGUtils::CurrentDllPath("SettingsHandler").c_str(), "CreateUnitDataSerializer");
   if (!m_unitDataSerializer.IsValid())
   {
      m_lock = true;
      //MessageString(ICommunicator::MS_Error, "Can't load settings serializer!");
      return;
   }
   return;
}

bool HeightMapConverter::Convert(const file_utils::file_storage_base& src, const file_utils::file_storage_base& dst)
{
   if (m_lock)
      return false;
   std::string srcPngPath = SVGUtils::wstringToString((reinterpret_cast<file_utils::heightmap_file_storage&>(const_cast<file_utils::file_storage_base&>(src))).map_path);
   readDataFromPng(srcPngPath.c_str());
   convertToDatabaseFormat();
   
   auto& srcFs = reinterpret_cast<const file_utils::heightmap_file_storage&>(src);
   m_settingsSerializer->Deserialize(SVGUtils::wstringToString(srcFs.pathfinder_settings_path).c_str(), m_settings.pth_stt);
   m_settingsSerializer->Deserialize(SVGUtils::wstringToString(srcFs.research_settings_path).c_str(), m_settings.res_stt);
   m_unitDataSerializer->Deserialize(SVGUtils::wstringToString(srcFs.unit_data_path).c_str(), m_settings.unit_stt);

   // NOTE: share provider вызываетcя из базы
   m_databaseController->Init(dst);
   m_databaseController->SaveScenarioData(m_settings, const_cast<const double**>(m_rawData));

   safeReleaseData();
   return true;
}

// к-т преобразования градиента в высоту 0-256 -> height_min-height_max
#define HEIGHT_CORRECTOR(h) 0.1*h

void HeightMapConverter::convertToDatabaseFormat()
{
   m_rawData = new double*[m_settings.map_stt.row_count];
   for (int l = 0; l < m_settings.map_stt.row_count; l++)
   {
      m_rawData[l] = new double[m_settings.map_stt.col_count];
      for (int w = 0; w < m_settings.map_stt.col_count; w++)
         m_rawData[l][w] = HEIGHT_CORRECTOR(m_row_pointers[l][w]);
   }
}

#undef HEIGHT_CORRECTOR

void HeightMapConverter::safeReleaseData()
{
   for (int l = 0; l < m_settings.map_stt.row_count; l++)
      delete m_rawData[l];
   delete m_rawData;
}

void HeightMapConverter::readDataFromPng(const char* srcPath)
{
   int width, height;
   png_byte color_type;
   png_byte bit_depth;

   FILE* fp = nullptr;
   auto err_no = fopen_s(&fp, srcPath, "rb");

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

   // Так...вроде правильно
   m_settings.map_stt.row_count = height;
   m_settings.map_stt.col_count = png_get_rowbytes(png, info);

   png_read_image(png, m_row_pointers);

   fclose(fp);
}

iConverter* CreateConverter()
{
   return new HeightMapConverter();
}