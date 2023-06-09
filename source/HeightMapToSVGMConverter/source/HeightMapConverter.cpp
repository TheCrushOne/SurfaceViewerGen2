#include "stdafx.h"
#include "HeightMapConverter.h"
#include "common/utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <png.h>

using namespace converter;

#define VALID_CHECK_DLL_LOAD(dllName, funcName, guard) \
   guard.Create(SVGUtils::CurrentDllPath(dllName).c_str(), funcName); \
   if (!guard.IsValid()) \
   { \
      GetCommunicator()->RaiseError(); \
      m_lock = true; \
      std::string errMsg = std::string("Can't load '") + dllName + "'!"; \
      Message(ICommunicator::MS_Error, errMsg.c_str()); \
      return; \
   }// \
   //guard->Init(GetPack());

HeightMapConverter::HeightMapConverter()
   : m_row_pointers(nullptr)
{
   VALID_CHECK_DLL_LOAD("SQLiteController", "CreateSQLiteDatabaseController", m_databaseController);
   VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateJsonSettingsSerializer", m_settingsSerializer);
   VALID_CHECK_DLL_LOAD("SettingsHandler", "CreateUnitDataSerializer", m_unitDataSerializer);
}

bool HeightMapConverter::Convert()
{
   if (m_lock)
      return false;
   auto ps = GetPathStorage();
   auto stt = GetSettings();
   std::string srcPngPath = SVGUtils::wstringToString(ps->map_path);
   readDataFromPng(srcPngPath.c_str());
   convertToDatabaseFormat();
  
   m_settingsSerializer->Deserialize(SVGUtils::wstringToString(ps->pathfinder_settings_path).c_str(), stt->pth_stt);
   m_settingsSerializer->Deserialize(SVGUtils::wstringToString(ps->research_settings_path).c_str(), stt->res_stt);
   m_settingsSerializer->Deserialize(SVGUtils::wstringToString(ps->environment_settings_path).c_str(), stt->env_stt);
   m_unitDataSerializer->Deserialize(SVGUtils::wstringToString(ps->unit_data_path).c_str(), m_unitData);

   // NOTE: share provider ��������c� �� ����
   m_databaseController->Init(GetPack());
   m_databaseController->SaveScenarioData(m_unitData, m_rawData);

   safeReleaseData();
   return true;
}

// TODO: ��������� ��������� � ��������� � ������� ��������� ������
// �-� �������������� ��������� � ������ 0-256 -> height_min-height_max
#define HEIGHT_CORRECTOR(h) 0.8*(h - 140.)

void HeightMapConverter::convertToDatabaseFormat()
{
   auto stt = GetSettings();
   m_rawData.resize(stt->map_stt.row_count);
   for (int l = 0; l < stt->map_stt.row_count; l++)
   {
      png_bytep row = m_row_pointers[l];
      m_rawData[l].resize(stt->map_stt.col_count);
      for (int w = 0; w < stt->map_stt.col_count; w++)
      {
         png_bytep px = &(row[4 * w]);
         double r = (double)px[0];
         double g = (double)px[1];
         double b = (double)px[2];
         double a = (double)px[3];
         m_rawData[l][w] = HEIGHT_CORRECTOR(r/3. + g/3. + b/3.);
      }
   }
}

#undef HEIGHT_CORRECTOR

void HeightMapConverter::safeReleaseData()
{
   //for (int l = 0; l < m_settings.map_stt.row_count; l++)
   //   delete m_rawData[l];
   //delete m_rawData;
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

   // ���...����� ���������
   GetSettings()->map_stt.row_count = height;
   GetSettings()->map_stt.col_count = png_get_rowbytes(png, info)/sizeof(png_bytep);

   png_read_image(png, m_row_pointers);

   fclose(fp);
}

iConverter* CreateConverter()
{
   return new HeightMapConverter();
}