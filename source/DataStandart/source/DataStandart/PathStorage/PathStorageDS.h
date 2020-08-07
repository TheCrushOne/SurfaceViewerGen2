#pragma once

#include "datastandart\PathStorageDataStandartInterface.h"
#include "datastandart\DataStandartBase.h"
#include "datastandart\DataStandartStruct.h"

namespace data_standart
{
   //template<int DSType = DataStandartType::DST_PATHS, typename DSStruct = path_storage_data_standart>
   class PathStorageDataStandart
      : public iPathStorageDataStandart
      , public DataStandart<DataStandartType::DST_PATHS, path_storage_data_standart>
   {
   public:
      PathStorageDataStandart(central_pack* pack, LPCWSTR base_folder, navigation_dispatcher::iComService* pService)
         : DataStandart(pack, base_folder, pService)
      {}
   public:
      // Common
      // Read
      // Write
   protected:
      // Common
      // Read
      virtual bool open() override final
      {
         resolvePathDee();
         // TODO: check existence
         return true;
      }
      // Write
      virtual bool create() override final
      {
         resolvePathDee();
         //if (fpath(m_dataSourceData.file).is_exists())
            //return fpath(m_dataSourceData.file).delete_from_disk();
         return true;
      }
   private:
      void resolvePathDee();
      LPCWSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
   private:
   };
}