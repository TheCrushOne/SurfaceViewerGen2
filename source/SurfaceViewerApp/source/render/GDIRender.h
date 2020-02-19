#pragma once
#include "Render.h"

enum EOutCode
{
   OC_TOP = 0x1,
   OC_BOTTOM = 0x2,
   OC_RIGHT = 0x4,
   OC_LEFT = 0x8
};

inline char calc_geo_out_code(double X, double Y, double  left, double right, double top, double bottom)
{
   char code = 0;
   if (Y > top)
      code = OC_TOP;
   else if (Y < bottom)
      code = OC_BOTTOM;

   if (X > right)
      code |= OC_RIGHT;
   else if (X < left)
      code |= OC_LEFT;
   return code;
}

inline char full_geo_out_code(const char* start, const char* stop)
{
   char t = 127;
   for (; start != stop && t; )
      t &= *start++;
   return t;
}

class GDIRender : public render::iRender
{

   struct renderObject : public render::object
   {
      renderObject(const renderObject& other) = default;
      renderObject(renderObject&& other) = default;

      renderObject(const render::object& other)
      {
         info = other.info;
         points = other.points;
         find = other.find;
         min_scale_2_draw = other.min_scale_2_draw;
      }

      renderObject& operator = (const render::object& other)
      {
         info = other.info;
         points = other.points;
         min_scale_2_draw = other.min_scale_2_draw;
      }

      operator colreg::id_type()const
      {
         return find.id;
      }

      inline void calcOutCode(const math::geo_rect& geoRect, const CRect& pixRect)
      {
         std::vector<char> outCodes;
         outCodes.reserve(points.size());
         if (info.pixels && points.size() == 1)
         {
            outCode = 0;
            return;
         }

         for (unsigned i = 0; i < points.size(); ++i)
         {
            outCodes.push_back(!info.pixels ? calc_geo_out_code(points[i].lon, points[i].lat, geoRect.leftTop.lon, geoRect.rightBottom.lon, geoRect.leftTop.lat, geoRect.rightBottom.lat)
                                            : calc_geo_out_code(points[i].lon, points[i].lat, pixRect.left, pixRect.right, pixRect.top, pixRect.bottom));
         }
         outCode = full_geo_out_code(outCodes.data(), outCodes.data() + outCodes.size());
      }

      inline bool IsInsideScreen()const { return outCode == 0; }

      char outCode = 0;
      std::vector <POINT> pixels;
      bool _static = false;
   };
   using pixelObjects = std::vector < renderObject>;
public:
   void Init(void* hWnd) override;
   void SetSize(size_t w, size_t h) override;
   SIZE GetSize()const override { return SIZE{ (LONG)_width, (LONG)_height }; }
   void SetCenter(const math::geo_point& center) override;
   math::geo_point GetCenter()const override { return _center; }
   void SetScale(double scale) override;
   double GetScale()const override { return _scale; }
   void AddObject(render::object&& obj, bool dynamic)override;
   void AddArc(const math::geo_point& center, double radius, double beg, double end, double step, unsigned long clr, render::LINE_STYLE conture, unsigned int width, bool dynamic) override;
   bool IsNeedRender(const math::geo_points& points)const override;
   void Render(size_t user_data) override;
   void Clear()override
   {
      _staticRenderObjects.clear();
   }

   render::find_info FindObject(const math::point& pos, render::FIND_TYPE type)const override;

   math::point GeoToPixel(const math::geo_point& pos)const  override;
   math::geo_point PixelToGeo(const math::point& pos)const override;

   std::vector<colreg::id_type> GetStaticIds()const override
   {
      std::vector<colreg::id_type> res(_staticRenderObjects.begin(), _staticRenderObjects.end());
      std::sort(res.begin(), res.end(), [](const auto& id1, const auto& id2)
         {
            return id1 < id2;
         });

      res.erase(std::unique(res.begin(), res.end()), res.end());
      return res;
   }

   const math::geo_point* GetStaticCenterId(colreg::id_type id)const override
   {
      const auto itf = std::find_if(_staticRenderObjects.begin(), _staticRenderObjects.end(), [id](const auto& obj)
         {
            return obj.find.id == id;
         });

      if (itf != _staticRenderObjects.end())
         return &(*itf).points[0];

      return nullptr;
   }
   std::vector<std::vector<math::geo_point>> GetObjectsInsideScreenPts() override;

private:

   void calcRenderGeoRect();
   void getObjectInsideScreen();

   void selectPen(CDC* dc, const render::object_info& info);
   void selectBrush(CDC* dc, const render::object_info& info);
   Gdiplus::SolidBrush* getAlphaBrush(const render::object_info& info);
   Gdiplus::Pen* getAlphaPen(const render::object_info& info);

   Gdiplus::Image* getImage(const char* name)
   {
      auto imgIt = _images.find(name);
      if (imgIt == _images.end())
      {
         return _images.try_emplace(name, new Gdiplus::Image(CComBSTR(name))).first->second;
      }
      return imgIt->second;
   }

   void drawEllipse(CDC* dc, renderObject& obj);
   void drawEllipse(CDC* dc, renderObject& obj, const POINT& pt);


   auto& getContainer(bool dynamic) { return dynamic ? _dynamicRenderObjects : _staticRenderObjects; }
   void renderObjects(CDC* dc, pixelObjects& objects);
   void renderAlphaObjects(CDC* dc, pixelObjects& objects);

   inline std::vector< POINT> toPixels(const math::geo_points& pPts, bool isPixels)const
   {
      if (isPixels)
         int t = 0;
      std::vector< POINT> pixels(pPts.size());
      for (size_t i = 0; i < pPts.size(); ++i)
      {
         const auto pix = !isPixels ? GeoToPixel(pPts[i])
            : math::point{ pPts[i].lat, pPts[i].lon };
         pixels[i].x = (int)pix.x;
         pixels[i].y = (int)pix.y;
      }
      return pixels;
   }

private:
   std::vector< renderObject> _staticRenderObjects;
   std::vector< renderObject> _dynamicRenderObjects;
   mutable std::vector< renderObject> _objectsWithinScreen;


   HWND _hwnd;
   double _scale;		         //< לאסרעאב pixel/mile
   math::geo_point _center;
   size_t _width;
   size_t _height;

   union obj_key
   {
      struct
      {
         char width;
         char style;
         unsigned long color;
      };
      UINT64 value;

      obj_key() :value(0) {};
   };

   std::unordered_map < UINT64, CPen> _pens;
   std::unordered_map < UINT64, CBrush> _brushs;
   std::unordered_map < UINT64, Gdiplus::SolidBrush*> _alphaBrushs;
   std::unordered_map < UINT64, Gdiplus::Pen*> _alphaPens;

   std::unordered_map < std::string, Gdiplus::Image* > _images;

   HGDIOBJ _hOldPen = NULL;
   HGDIOBJ _hOldBrush = NULL;

   CFont _font;
   CRect _rect;
   math::geo_rect _geoRect;

   CBitmap _bitmap;

   const double _minTextScale = 500.;
};
