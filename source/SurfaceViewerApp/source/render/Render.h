#pragma once
#include <vector>
#include <string>
#include "math/math_utils.h"
#include "colreg/ChartSafetyStructs.h"

#define _BIT_(n) (1i64 << n)

namespace render
{
   enum class LINE_STYLE : char
   {
      LL_SOLID = 0,
      LL_DASH,        // -------
      LL_DOT,         // .......
      LL_DASH_DOT,    // _._._._
      LL_DASH_DOT_DOT, // _.._.._
      LL_NONE,
   };


   enum class FILL_TYPE : char
   {
      FT_HORIZONTAL = 0,/* ----- */
      FT_VERTICAL,     /* ||||| */
      FT_FDIAGONAL,    /* \\\\\ */
      FT_BDIAGONAL,    /* ///// */
      FT_CROSS,        /* +++++ */
      FT_DIAGCROSS,    /* xxxxx */
      FT_SOLID,
      FT_NULL,
      FT_NONE,
   };

   // WARNING: не должно быть классов около битовых енамов!!!
   enum ANCHOR_TYPE : unsigned long long int
   {
      AT_CENTER = 0,
      AT_TOP = _BIT_(0),
      AT_BOTTOM = _BIT_(1),
      AT_LEFT = _BIT_(2),
      AT_RIGHT = _BIT_(3),
      AT_TOPLEFT = _BIT_(4),
      AT_TOPRIGHT = _BIT_(5),
      AT_BOTTOMLEFT = _BIT_(6),
      AT_BOTTOMRIGHT = _BIT_(7),

      AT_LEFTSIDE = AT_LEFT | AT_TOPLEFT | AT_BOTTOMLEFT,
      AT_RIGHTSIDE = AT_RIGHT | AT_TOPRIGHT | AT_BOTTOMRIGHT,
      AT_TOPSIDE = AT_TOP | AT_TOPLEFT | AT_TOPRIGHT,
      AT_BOTTOMSIDE = AT_BOTTOM | AT_BOTTOMLEFT | AT_BOTTOMRIGHT,

      AT_ALL = 0xFFFFFFFFFFFFFFFF,
   };

   struct object_info
   {
      int width = 1;                            ///< ширина линии/радиус точки     
      LINE_STYLE style = LINE_STYLE::LL_SOLID;  ///< Соединение точек
      FILL_TYPE fill = FILL_TYPE::FT_NONE;      ///< Заливка
      unsigned long color = 0;                  ///< Цвет RGB    
      std::string text;                         ///< Текст объекта
      // NOTE: 10 было для якорей
      //char text_offset_x = 10;                  ///< Смещение текста по Х
      //char text_offset_y = 10;                  ///< Смещение текста по Y
      char text_offset_x = 0;                  ///< Смещение текста по Х
      char text_offset_y = 0;                  ///< Смещение текста по Y
      int alpha = 255;                          ///< Прозрачночть(0-255)
      std::string imageFile;
      ANCHOR_TYPE anchor_type = ANCHOR_TYPE::AT_CENTER;
      double angle = 0;
      bool useAngle = false;                    ///< Использовать угол при позиционировании картинки
      bool pixels = false;
   };

   //как ищем
   enum class FIND_TYPE : char
   {
      FT_NONE = 0,      //<не искать     
      FT_FIND_DETAILED, //<детальный поиск
      FT_FIND_FAST,     //<быстрый поиск
   };

   enum class FIND_OBJECT_TYPE : char
   {
      FOT_NONE = 0,
      FOT_ROVER,
      FOT_SHIP,
      FOT_DRONE,
      FOT_ROVER_DOMAIN,
      FOT_SHIP_DOMAIN,
      FOT_DRONE_DOMAIN,
      FOT_ROUTE_SEGMENT,
      FOT_ROUTE_POINT,
      FOT_TRACK,
      FOT_SELECTED,
      FOT_CHART_OBJECT,
      /*FOT_SHIP,
      FOT_ROUTE_SEGMENT,
      FOT_ROUTE_POINT,
      FOT_TRACK,
      FOT_DANGER,
      FOT_DOMAIN,
      
      FOT_DEBUG_INFO,
      
      FOT_GRAPH_RELATION,
      FOT_ROUTE_ZONE,
      FOT_WIND_ARROW,
      FOT_WAVE_ARROW,
      FOT_CURRENT_ARROW,
      FOT_FOG,*/
   };

   struct find_info
   {
      FIND_TYPE type = FIND_TYPE::FT_NONE;
      colreg::id_type id = colreg::INVALID_ID;
      FIND_OBJECT_TYPE find_object_type = FIND_OBJECT_TYPE::FOT_NONE;
      size_t user_data = 0;
      colreg::OBJECT_TYPE chart_object_type = colreg::OBJECT_TYPE::OT_NONE;
   };

   struct object
   {
      math::geo_points points;
      object_info info;
      find_info find;
      float min_scale_2_draw = 0.0;
      double angle = 0.0;
   };

   using objects = std::vector<object>;

   struct iRender abstract
   {
      ~iRender() = default;
      virtual void Init(void* hWnd) = 0;
      virtual void SetSize(size_t w, size_t h) = 0;
      virtual SIZE GetSize() const = 0;
      virtual void SetCenter(const math::geo_point& center) = 0;
      virtual math::geo_point GetCenter() const = 0;
      virtual void SetScale(double scale) = 0;
      virtual double GetScale()const = 0;
      virtual void AddObject(object&& obj, bool dynamic = true) = 0;
      virtual void AddArc(const math::geo_point& center, double radius, double beg, double end, double step, unsigned long clr, LINE_STYLE conture, unsigned int width = 2, bool dynamic = true) = 0;
      virtual math::point GeoToPixel(const math::geo_point& pos) const = 0;
      virtual math::geo_point PixelToGeo(const math::point& pos) const = 0;
      virtual bool IsNeedRender(const math::geo_points& points) const = 0;
      virtual void Render(size_t user_data) = 0;
      virtual void Clear() = 0;
      virtual std::vector<std::vector<math::geo_point>> GetObjectsInsideScreenPts() = 0;

      virtual find_info FindObject(const math::point& pos, FIND_TYPE type) const = 0;
      virtual std::vector<colreg::id_type> GetStaticIds() const = 0;
      virtual const math::geo_point* GetStaticCenterId(colreg::id_type id) const = 0;
   };
};