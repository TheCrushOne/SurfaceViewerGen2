#pragma once

#include "datastandart\PathStorageDataStandartInterface.h"
#include "datastandart\DataStandartBase.h"
#include "datastandart\DataStandartStruct.h"

namespace SV::data_standart
{
   //template<int DSType = DataStandartType::DST_PATHS, typename DSStruct = path_storage_data_standart>
   class PathStorageDataStandart
      : public iPathStorageDataStandart
      , public DataStandart<DataStandartType::DST_PATHS, path_storage_data_standart>
   {
      struct tag
      {
         static constexpr char air_routes[] = "air_routes";
         static constexpr char land_routes[] = "land_routes";

         static constexpr char start[] = "start";
         static constexpr char finish[] = "finish";

         static constexpr char meta[] = "meta";
         static constexpr char id[] = "id";
         static constexpr char name[] = "name";

         static constexpr char control_point_list[] = "control_point_list";
         static constexpr char route_list[] = "route_list";

         static constexpr char col[] = "col";
         static constexpr char row[] = "row";
         static constexpr char go[] = "go";
         static constexpr char fly[] = "fly";
         static constexpr char height[] = "height";
         static constexpr char is_control[] = "is_control";

         static constexpr char col_count[] = "col_count";
         static constexpr char row_count[] = "row_count";
      };
      struct meta
      {
         size_t row_count;
         size_t col_count;
      };
   public:
      PathStorageDataStandart(central_pack* pack, LPCSTR base_folder, navigation_dispatcher::iComService* pService)
         : DataStandart(pack, base_folder, pService)
      {}
   public:
      // iDataStandart
      // Common
      virtual bool Create() override final
      {
         resolvePathDee();
         //if (fpath(m_dataSourceData.file).is_exists())
            //return fpath(m_dataSourceData.file).delete_from_disk();
         return true;
      }
      bool DeserializeAttrs(const xml_properties::PropertyItem& standart) override final { return deserializeAttrs(standart); }
      LPCSTR GetPath() override { return getPath(); }
      DataStandartType GetType() const override { return getType(); }
      void Release() override final { delete this; }
      size_t GetDataHash() override final { return getDataHash(); }
      // Read
      // Write

      // iSurfaceViewerGenMapDataStandart
      // Common
      // Read
      const pathfinder::route_data& GetData() const override final;
      const pathfinder::UnsignedMatrix& GetLandUnitExplication() const override final;
      const pathfinder::UnsignedMatrix& GetAirUnitExplication() const override final;
      const std::vector<pathfinder::UnsignedMatrix>& GetCoverageHistory() const override final;
      // Write
      void SetData(const pathfinder::route_data& paths, const pathfinder::UnsignedMatrix& land, const pathfinder::UnsignedMatrix& air, const std::vector<pathfinder::SharedUnsignedMatrix>& coverages) override final;
   private:
      // Common
      std::string getMetaFilePath() { return std::string(getPath()) + "\\meta.inf"; }
      std::string getDataFilePath() { return std::string(getPath()) + "\\pathdata.pl"; }
      std::string getLandExplicationFilePath(size_t idx = 0) { return std::string(getPath()) + "\\land_0.exp"; }
      std::string getAirExplicationFilePath(size_t idx = 0) { return std::string(getPath()) + "\\air_0.exp"; }
      std::string getCoverageFilePath(size_t step) { return std::string(getPath()) + "\\step_" + std::to_string(step) + ".cov"; }
      size_t getLandCount() { return m_paths.land_routes.size(); }
      size_t getAirCount() { return m_paths.air_routes.size(); }
      // Read
      void readMetaFromFile();
      void readPathDataFromFile();
      void readCoverageDataFromFile();
      void readLandExplicationDataFromFile(size_t idx = 0);
      void readAirExplicationDataFromFile(size_t idx = 0);

      static CG::route_point routePointRead(const Json::Value& j);
      static settings::route routeRead(const Json::Value& jroute);
      // Write
      void saveMetaToFile();
      void savePathDataToFile();
      void saveCoverageDataToFile();
      void saveLandExplicationDataToFile(size_t idx = 0);
      void saveAirExplicationDataToFile(size_t idx = 0);

      static Json::Value routePointWrite(const CG::route_point&);
      static Json::Value routeWrite(const settings::route&);
      static Json::Value pathListWriter(const std::vector<settings::route>&);
   private:
      void resolvePathDee();
      LPCSTR getPath() override final { return m_dataStandartData.folder.c_str(); }
      
      static void pathListRead(std::vector<settings::route>& route_list, const Json::Value& jroute_list);
   private:
      pathfinder::route_data m_paths;
      pathfinder::UnsignedMatrix m_landExplication;
      pathfinder::UnsignedMatrix m_airExplication;
      std::vector<pathfinder::UnsignedMatrix> m_coverageHistory;
      meta m_meta;
   };
}