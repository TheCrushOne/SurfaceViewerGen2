#pragma once

#include "crossdllinterface/ChartObjectGeneratorInterface.h"

namespace chart_object
{
   struct chart_storage
   {
      std::vector<std::vector<colreg::geo_point>> geom_contour_vct;
      std::vector<colreg::geo_points_ref> geom_contour_ref_vct;
      std::vector<colreg::simple_prop> prop_vct;

      colreg::OBJECT_TYPE type;

      void Commit()
      {
         for (const auto& elem : geom_contour_vct)
            geom_contour_ref_vct.emplace_back(elem.data(), elem.size());
      }
   };

   class ChartObjectGenerator : public iGenerator, public Communicable
   {
   public:
      ChartObjectGenerator();

      bool Init(ICommunicator* comm, settings::environment_settings* envStt) override final;
      bool GenerateStatic(const converter::raw_data_ref& rawdata) override final;
      const colreg::chart_objects_ref& GetChartObjects() const override final { prepareRef(); return m_chartObjectRef; }

      void Release() override final { delete this; }
   protected:
      void addChartObject(chart_storage& storage);
      chart_storage& generateNew() { m_chartStorage.emplace_back(); return m_chartStorage.back(); }
      void prepareRef() const;
      void prepareLocalStorage();
      void generateChartBorder(const converter::raw_data_ref& rawdata);
      void generateIsolines(const converter::raw_data_ref& rawdata);
      void generateNoGoAreas(const converter::raw_data_ref& rawdata);
      void generateNoFlyAreas(const converter::raw_data_ref& rawdata);
      void isoSort(std::vector<colreg::geo_point>& isoline);
   private:
      bool m_lock = false;

      double m_maxRadius;

      settings::environment_settings* m_settings;

      std::vector<chart_storage> m_chartStorage;

      std::vector<colreg::chart_object> m_chartObjVct;

      colreg::chart_objects_ref m_chartObjectRef;
   };
}