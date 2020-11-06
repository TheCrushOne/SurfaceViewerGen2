#include "stdafx.h"
#include "GDIRender.h"
#include "SVCG/enhanced_geometry.h"
#include "math/math_utils.h"

#include "wingdi.h"
#include "gdiplus.h"

using namespace SV;
using namespace SV::render;

namespace
{
   inline DWORD get_pen_style(LINE_STYLE style)
   {
      return (int)(style == LINE_STYLE::LL_NONE ? LINE_STYLE::LL_SOLID : style);
   }

   inline DWORD get_brush_style(FILL_TYPE fill)
   {
      return (int)fill;
   }
}

bool GDIRender::IsNeedRender(const CG::geo_contour& points)const
{
   for (const auto& p : points)
   {
      const auto pix = GeoToPixel(p);
      if (::PtInRect(&_rect, CPoint{ (int)pix.x, (int)pix.y }))
         return true;
   }
   return false;
}

void GDIRender::drawEllipse(CDC* dc, renderObject& obj, const POINT& pt)
{
   const int delta = (int)(obj.info.width * 0.5);
   dc->Ellipse((int)pt.x - delta, (int)pt.y - delta, (int)pt.x + delta, (int)pt.y + delta);
}

void GDIRender::drawEllipse(CDC* dc, renderObject& obj)
{
   ATLASSERT(obj.points.size() == 1);
   drawEllipse(dc, obj, obj.pixels[0]);
}

void GDIRender::Init(void* hWnd)
{
   _hwnd = (HWND)hWnd;
   // _font.CreateFont(15, 10, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_MODERN, "Courier New");
   _font.CreateStockObject(DEFAULT_GUI_FONT);

   Gdiplus::GdiplusStartupInput    gdiplusStartupInput;
   ULONG_PTR                       gdiplusToken;
   if (GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Ok)
      ATLASSERT(!"GDI+ initialization error");
}

void GDIRender::SetSize(size_t w, size_t h)
{
   _width = w;
   _height = h;
   _rect.SetRect(0, 0, (int)_width, (int)_height);
   calcRenderGeoRect();
}

void GDIRender::SetCenter(const CG::geo_point& center)
{
   _center = center;

   calcRenderGeoRect();
}

void GDIRender::SetScale(double scale)
{
   _scale = scale;
   if (_scale < 10.)
      _scale = 10.;
   else if (_scale > 5000.)
      _scale = 5000.;

   calcRenderGeoRect();
}

void GDIRender::AddObject(object&& obj, bool dynamic)
{
   getContainer(dynamic).emplace_back(std::move(obj));
}

void GDIRender::AddArc(const CG::geo_point& center, double radius, double beg, double end, double step, unsigned long clr, LINE_STYLE conture, unsigned int width, bool dynamic)
{
   object obj;
   obj.info.style = conture;
   obj.info.color = clr;
   obj.info.width = width;
   obj.points.reserve(static_cast<size_t>((end - beg) / step));
   for (double deg = beg; deg <= end; deg += step)
      obj.points.emplace_back(math::calc_point(center, radius, deg));
   obj.points.emplace_back(math::calc_point(center, radius, end));

   AddObject(std::move(obj), dynamic);
}

math::point GDIRender::GeoToPixel(const CG::geo_point& pos)const
{
   auto dlon = pos.lon - _center.lon;
   auto dlat = pos.lat - _center.lat;
   auto dir = math::normal_K360(math::rad_to_grad(atan2(dlon, dlat)));
   auto dist = sqrt(math::sqr(dlat) + math::sqr(dlon)) * 60;
   math::point point{ _height * .5 - math::cos_grad(dir) * dist * _scale, _width * .5 + math::sin_grad(dir) * dist * math::cos_grad(_center.lat) * _scale };
   return point;
}

CG::geo_point GDIRender::PixelToGeo(const math::point& pos)const
{
   math::point center{ _height * .5, _width * .5 };
   const auto course = -math::direction(pos, center);
   const auto dist = math::distance(center, pos) / _scale * MIN_TO_RAD;

   const auto courseRad = math::grad_to_rad(course);
   double dLat = dist * cos(courseRad);
   double dLon = dist * sin(courseRad);
   return CG::geo_point(_center.lat + dLat, _center.lon + dLon / math::cos_grad(_center.lat));
}

void GDIRender::selectPen(CDC* dc, const render::object_info& info)
{
   static UINT64 selectedPen = 0;
   obj_key key;
   key.color = info.color;
   key.width = info.width;
   key.style = (char)info.style;
   if (_hOldPen != NULL && selectedPen == key.value)
      return;

   const auto itf = _pens.find(key.value);
   if (itf != _pens.end())
      _hOldPen = dc->SelectObject(&(*itf).second);
   else
   {
      _pens[key.value].CreatePen(get_pen_style(info.style), info.width, info.color);
      _hOldPen = dc->SelectObject(&_pens[key.value]);
   }
   selectedPen = key.value;
}


void GDIRender::selectBrush(CDC* dc, const render::object_info& info)
{
   static UINT64 selectedBrush = 0;
   obj_key key;
   key.color = info.color;
   key.style = (char)info.fill;
   dc->SetBkMode(TRANSPARENT);
   if (_hOldBrush != NULL && selectedBrush == key.value)
      return;
   const auto itf = _brushs.find(key.value);
   if (itf != _brushs.end())
      _hOldBrush = dc->SelectObject(&(*itf).second);
   else
   {
      switch (info.fill)
      {
      case FILL_TYPE::FT_NONE:
      case FILL_TYPE::FT_NULL:
         _brushs[key.value].CreateStockObject(NULL_BRUSH);  break;
      case FILL_TYPE::FT_SOLID: _brushs[key.value].CreateSolidBrush(info.color);   break;
      default:     _brushs[key.value].CreateHatchBrush(get_brush_style(info.fill), info.color);
      }
      _hOldBrush = dc->SelectObject(&_brushs[key.value]);
   }
   selectedBrush = key.value;
}

Gdiplus::Pen* GDIRender::getAlphaPen(const render::object_info& info)
{
   obj_key key;
   key.color = info.color;
   key.width = info.width;
   key.style = (char)info.style;
   const auto itf = _alphaPens.find(key.value);
   if (itf != _alphaPens.end())
      return (*itf).second;

   _alphaPens[key.value] = new Gdiplus::Pen(Gdiplus::Color(info.alpha, GetRValue(info.color), GetGValue(info.color), GetBValue(info.color)), info.width);
   _alphaPens[key.value]->SetDashStyle((Gdiplus::DashStyle)info.style);
   return _alphaPens[key.value];
}

Gdiplus::SolidBrush* GDIRender::getAlphaBrush(const render::object_info& info)
{
   obj_key key;
   key.color = info.color;
   key.width = info.alpha;
   const auto itf = _alphaBrushs.find(key.value);
   if (itf != _alphaBrushs.end())
      return (*itf).second;

   _alphaBrushs[key.value] = new Gdiplus::SolidBrush(Gdiplus::Color(info.alpha, GetRValue(info.color), GetGValue(info.color), GetBValue(info.color)));

   return _alphaBrushs[key.value];
}

void GDIRender::Render(size_t user_data)
{
   CDC* pDC = (CDC*)user_data;
   CDC memDC;
   memDC.CreateCompatibleDC(pDC);

   _bitmap.DeleteObject();
   _bitmap.CreateCompatibleBitmap(pDC, _rect.Width(), _rect.Height());

   auto pOldBmp = memDC.SelectObject(_bitmap);

   _hOldPen = NULL;
   _hOldBrush = NULL;
   auto hOld = memDC.SelectObject(&_font);
   memDC.SetBkMode(TRANSPARENT);

   memDC.FillSolidRect(_rect.left, _rect.top, _rect.Width(), _rect.Height(), RGB(255, 255, 255));// RGB(135, 245, 255));

   CRect pixRect{ 0, 0, (int)_width, (int)_height };
   std::for_each(_staticRenderObjects.begin(), _staticRenderObjects.end(), [this, pixRect](auto& obj) { obj.calcOutCode(this->_geoRect, pixRect); });
   std::for_each(_dynamicRenderObjects.begin(), _dynamicRenderObjects.end(), [this, pixRect](auto& obj) { obj.calcOutCode(this->_geoRect, pixRect); });

   getObjectInsideScreen();

   renderObjects(&memDC, _objectsWithinScreen);

   memDC.SelectObject(&hOld);
   if (_hOldPen)
      memDC.SelectObject(&_hOldPen);
   if (_hOldBrush)
      memDC.SelectObject(&_hOldBrush);

   _dynamicRenderObjects.clear();

   pDC->BitBlt(_rect.left, _rect.top, _rect.Width(), _rect.Height(), &memDC, _rect.left, _rect.top, SRCCOPY);
   memDC.SelectObject(pOldBmp);
}

void GDIRender::renderObjects(CDC* dc, pixelObjects& objects)
{
   static pixelObjects alphaObjects;
   alphaObjects.clear();

   for (auto& obj : objects)
   {
      if (GetScale() < obj.min_scale_2_draw)
      {
         continue;
      }

      selectPen(dc, obj.info);
      selectBrush(dc, obj.info);
      if (obj.info.alpha < 255 || !obj.info.imageFile.empty())
         alphaObjects.push_back(obj);
      if (obj.points.size() == 1 && obj.info.alpha == 255 && obj.info.imageFile.empty())
         drawEllipse(dc, obj);
      else if (obj.points.size() > 1)
      {
         if (obj.points.size() == 2 && obj.info.fill != FILL_TYPE::FT_NONE) //circle
         {
            auto delta = (int)math::distance(math::point{ (double)obj.pixels[0].y, (double)obj.pixels[0].x }
            , math::point{ (double)obj.pixels[1].y, (double)obj.pixels[1].x });
            dc->Ellipse(obj.pixels[0].x - delta, obj.pixels[0].y - delta, obj.pixels[0].x + delta, obj.pixels[0].y + delta);
         }
         else
         {
            if (obj.info.style == LINE_STYLE::LL_NONE)
            {
               for (const auto& pt : obj.pixels)
               {
                  drawEllipse(dc, obj, pt);
               }
            }
            else
            {
               if (obj.info.fill == FILL_TYPE::FT_NONE && obj.info.alpha == 255)
                  dc->Polyline(obj.pixels.data(), obj.pixels.size());
               if (obj.info.fill != FILL_TYPE::FT_NONE)
                  obj.info.alpha == 255 ? dc->Polygon(obj.pixels.data(), obj.pixels.size()) : dc->Polyline(obj.pixels.data(), obj.pixels.size());
            }
         }


      }

      if (obj.info.imageFile.empty() && (!obj._static || (GetScale() >= _minTextScale && !obj.info.text.empty())))
      {
         POINT textPix{ 0,0 };
         for (const auto& pix : obj.pixels)
         {
            textPix.x += pix.x;
            textPix.y += pix.y;
         }
         textPix.x /= obj.pixels.size();
         textPix.y /= obj.pixels.size();
         const auto ptText = GeoToPixel(obj.points[(int)(obj.points.size() * .5)]);
         // TODO: вот тут странно отвалилось
         dc->TextOut(textPix.x + obj.info.text_offset_x, textPix.y + obj.info.text_offset_y, obj.info.text.c_str(), obj.info.text.size());
      }
   }

   if (!alphaObjects.empty())
      renderAlphaObjects(dc, alphaObjects);
}

void GDIRender::renderAlphaObjects(CDC* dc, pixelObjects& objects)
{
   Gdiplus::Graphics graphics(dc->GetSafeHdc());
   static Gdiplus::Font font(&Gdiplus::FontFamily(L"Arial"), 8);
   for (auto& obj : objects)
   {
      if (!obj.info.imageFile.empty())
      {
         const auto& center = obj.pixels[0];
         int xpos = center.x;
         if ((obj.info.anchor_type & ANCHOR_TYPE::AT_LEFTSIDE) != 0)
            xpos += 0;
         else if ((obj.info.anchor_type & ANCHOR_TYPE::AT_RIGHTSIDE) != 0)
            xpos -= obj.info.width;
         else
            xpos -= obj.info.width * .5;

         int ypos = center.y;
         if ((obj.info.anchor_type & ANCHOR_TYPE::AT_TOPSIDE) != 0)
            ypos += 0;
         else if ((obj.info.anchor_type & ANCHOR_TYPE::AT_BOTTOMSIDE) != 0)
            ypos -= obj.info.width;
         else
            ypos -= obj.info.width * .5;

         graphics.TranslateTransform(center.x, center.y);
         graphics.RotateTransform(obj.angle);
         graphics.TranslateTransform(-center.x, -center.y);
         graphics.DrawImage(
            getImage(obj.info.imageFile.c_str()),
            xpos,
            ypos,
            obj.info.width,
            obj.info.width
         );
         graphics.ResetTransform();

         USES_CONVERSION;
         graphics.DrawString(A2W(obj.info.text.c_str()), -1, &font, Gdiplus::PointF(center.x + obj.info.text_offset_x, center.y + obj.info.text_offset_y), getAlphaBrush(obj.info));
         continue;
      }

      std::vector<Gdiplus::Point> pts(obj.pixels.size());

      for (size_t i = 0; i < obj.pixels.size(); ++i)
      {
         pts[i].X = obj.pixels[i].x;
         pts[i].Y = obj.pixels[i].y;
      }
      if (pts.size() == 1)
      {
         int R = obj.info.width;
         graphics.FillEllipse(getAlphaBrush(obj.info), obj.pixels[0].x - R, obj.pixels[0].y - R, 2 * R, 2 * R);
      }
      else if (pts.size() == 2 && obj.info.fill != FILL_TYPE::FT_NONE)
      {
         int R = (int)math::distance(math::point{ (double)obj.pixels[0].y, (double)obj.pixels[0].x }
         , math::point{ (double)obj.pixels[1].y, (double)obj.pixels[1].x });
         graphics.FillEllipse(getAlphaBrush(obj.info), obj.pixels[0].x - R, obj.pixels[0].y - R, 2 * R, 2 * R);
      }
      else if (obj.info.fill != FILL_TYPE::FT_NONE)
         graphics.FillPolygon(getAlphaBrush(obj.info), pts.data(), pts.size());
      else
         graphics.DrawLines(getAlphaPen(obj.info), pts.data(), pts.size());
   }
}


find_info GDIRender::FindObject(const math::point& pos, FIND_TYPE type)const
{
   constexpr auto size = 8;
   math::geo_rect geoFind;
   geoFind.leftTop = PixelToGeo({ pos.y - size, pos.x - size });
   geoFind.rightBottom = PixelToGeo({ pos.y + size, pos.x + size });
   CRect pixFind{ (int)pos.x - size, (int)pos.y - size, (int)pos.x + size, (int)pos.y + size };
   auto it = _objectsWithinScreen.rbegin();
   for (; it != _objectsWithinScreen.rend(); ++it)
   {
      auto& obj = (*it);
      if ((int)obj.find.type < (int)type)
         continue;
      obj.calcOutCode(geoFind, pixFind);
      if (obj.IsInsideScreen())
      {
         if (obj.pixels.size() > 1)
         {
            if (obj.info.fill != FILL_TYPE::FT_NONE)//polygon
            {
               CG::vector_2 polypoints;
               polypoints.reserve(size);
               for (size_t index = 0; index < obj.pixels.size(); ++index)
               {
                  const auto& gpt = obj.pixels[index];
                  polypoints.emplace_back(CG::point_2(gpt.y, gpt.x));
               }

               CG::PolyPolygon_2 poligon;
               poligon.AddContour(polypoints);

               if (poligon.IsPointInPolygon(CG::point_2(pos.y, pos.x), nullptr))
                  return obj.find;
            }
            else//polyline
            {
               for (size_t i = 0; i < obj.pixels.size() - 1; ++i)
               {
                  const auto d = math::dist_to_segment(math::point{ (double)obj.pixels[i].y,  (double)obj.pixels[i].x }
                     , math::point{ (double)obj.pixels[i + 1].y, (double)obj.pixels[i + 1].x }
                  , pos);
                  if (d < size * .5)
                     return obj.find;
               }

            }
            continue;

         }
         return obj.find;
      }
   }
   return find_info{};
}


void GDIRender::calcRenderGeoRect()
{
   _geoRect.leftTop = PixelToGeo({ 0., 0. });
   _geoRect.rightBottom = PixelToGeo({ (double)_height, (double)_width });
}

void GDIRender::getObjectInsideScreen()
{
   _objectsWithinScreen.clear();
   _objectsWithinScreen.reserve(_dynamicRenderObjects.size());
   for (auto& obj : _staticRenderObjects)
   {
      if (GetScale() < _minTextScale && obj.points.size() == 1)
         continue;
      if (obj.IsInsideScreen())
      {
         obj.pixels = toPixels(obj.points, obj.info.pixels);

         //          if (!std::any_of(obj.pixels.begin(), obj.pixels.end(), [this](const auto& p) {return ::PtInRect(&this->_rect, p); }))
         //             continue;
         //TODO добавить проверку на пересечения с экраном
         obj._static = true;
         _objectsWithinScreen.push_back(obj);
      }
   }

   for (auto& obj : _dynamicRenderObjects)
   {
      if (obj.IsInsideScreen())
      {
         obj.pixels = toPixels(obj.points, obj.info.pixels);
         _objectsWithinScreen.emplace_back(std::move(obj));
      }
   }
}

CG::geo_contour_vct GDIRender::GetObjectsInsideScreenPts()
{
   CG::geo_contour_vct pts;
   getObjectInsideScreen();
   for (auto& obj : _objectsWithinScreen)
   {
      pts.emplace_back(obj.points);
   }
   return pts;
}