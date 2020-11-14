#pragma once

#include "Common/matrix_object.h"

#include <windows.h>

namespace SV::binary_save_helper
{
   template<typename T>
   void to_file(LPCSTR filename, const pathfinder::Matrix<T>& data)
   {
      size_t rowCount = data.GetRowCount();
      size_t colCount = data.GetColCount();
      std::ofstream file(filename, std::ios::out | std::ios::binary);
      for (size_t ridx = 0; ridx < rowCount; ridx++)
      {
         for (size_t cidx = 0; cidx < colCount; cidx++)
         {
            T buffer = data.Get(ridx, cidx);
            file.write(reinterpret_cast<const char*>(&buffer), sizeof(T));
         }
      }
      file.close();
   }
}