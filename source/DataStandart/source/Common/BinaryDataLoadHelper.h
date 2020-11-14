#pragma once

#include "Common/matrix_object.h"

#include <windows.h>

namespace SV::binary_load_helper
{
   template<typename T>
   pathfinder::Matrix<T> from_file(LPCSTR filename, size_t row_count, size_t col_count)
   {
      pathfinder::Matrix<T> data;
      data.SetAverallSize(row_count, col_count);
      std::ifstream file(filename, std::ios::in | std::ios::binary);
      char* buffer = new char[sizeof(T)];
      for (size_t ridx = 0; ridx < row_count; ridx++)
      {
         for (size_t cidx = 0; cidx < col_count; cidx++)
         {
            file.read(buffer, sizeof(T));
            data.Set(ridx, cidx, *reinterpret_cast<T*>(buffer));
         }
      }
      delete[] buffer;
      file.close();
      return data;
   }
}