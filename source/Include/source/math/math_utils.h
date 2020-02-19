#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <algorithm>
#include <limits>
#include "point.h"
#include "colreg/CommonStructs.h"

constexpr double MILE = 1852.0; // Метров в миле
constexpr double METERS_TO_MILE = 1. / MILE;
constexpr double HOUR_TO_SEC = 3600.;
constexpr double HOUR_TO_MIN = 60.;
constexpr double SEC_TO_HOUR = 1. / HOUR_TO_SEC;
constexpr double DEG_TO_RAD = M_PI / 180.;
constexpr double RAD_TO_DEG = 180. / M_PI;
constexpr double KNOOT_TO_MS = MILE / 3600.;
constexpr double MS_TO_KNOOT = 3600 / MILE;
constexpr double MIN_TO_RAD = 1. / 60.;

namespace
{
   /**
   * \struct straight
   * для коэф. прямой в виде общего уравнения Ax + By + C = 0
   */
   struct straight 
   {
      double A;
      double B;
      double C;
   };

   inline straight create_straight(const colreg::geo_point& A, const colreg::geo_point& B)
   {
      straight s;
      s.A = A.lat - B.lat;
      s.B = B.lon - A.lon;
      s.C = A.lon * B.lat - B.lon * A.lat;
      return s;
   }

   //! точка пересечения прямых a и b
   inline colreg::geo_point intersection(const straight& a, const straight& b)
   {
      return colreg::geo_point{ (a.C*b.A - b.C*a.A) / (b.B*a.A - a.B*b.A), (b.C*a.B - a.C*b.B) / (a.A*b.B - b.A*a.B) };
   }

   //! пересекаются ли AB и CD по одной из координат
   inline bool is_intersect_coord(double a, double b, double c, double d) 
   {
      if (a > b)  std::swap(a, b);
      if (c > d)  std::swap(c, d);
      return std::max(a, c) <= std::min(b, d);
   }
}
namespace math
{
   using geo_point = colreg::geo_point;
   using geo_points = std::vector<geo_point>;
   struct geo_rect
   {
      geo_rect()
         : leftTop{ 360. , 360. }
         , rightBottom{ -360. , -360. }
      {}

      geo_point leftTop;
      geo_point rightBottom;
   };

   template<class T> T sqr(T x) { return x*x; }

   constexpr float PI_FLOAT = 3.1415926535897932384626433832795f;
   constexpr float PIBY2_FLOAT = PI_FLOAT/2;
   // |error| < 0.005. ~ на 30% быстрее SSE2
   inline float atan2_approximation(float y, float x)
   {
      if(x == 0.0f)
      {
         if(y > 0.0f) return PIBY2_FLOAT;
         if(y == 0.0f) return 0.0f;
         return -PIBY2_FLOAT;
      }
      float atan;
      float z = y / x;
      if(fabs(z) < 1.0f)
      {
         atan = z / (1.0f + 0.28f*z*z);
         if(x < 0.0f)
         {
            if(y < 0.0f) return atan - PI_FLOAT;
            return atan + PI_FLOAT;
         }
      }
      else
      {
         atan = PIBY2_FLOAT - z / (z*z + 0.28f);
         if(y < 0.0f) return atan - PI_FLOAT;
      }
      return atan;
   }

   // Знак числа -1 0 1
   template<class T>
   inline int sign(T a)
   {
      int result = ((a > 0) - (a < 0));
      return result == 0 ? 1 : result;
   }

   // Нормализация направления 360
   inline double normal_K360(double K)
   {
      K -= (int)(K / 360) * 360.;
      return K < 0 ? 360 + K : K;
   }

   // Нормализация направления +/- 180
   inline double normal_K180(double K)
   {
      K = normal_K360(K);
      if (K <= -180.) return K + 360.;
      if (K > 180.) return K - 360.;

      return K;
   }

   inline bool	is_between(double x, double a, double b)
   {
      return  x >= a && x <= b;
   }

   // Преобразует градусы в радианы
   template< typename T >
   inline constexpr T grad_to_rad(T x)
   {
      return (x * DEG_TO_RAD);
   }

   // Преобразует радианы в градусы
   inline double constexpr  rad_to_grad(double x)
   {
      return	(x * RAD_TO_DEG);
   }

   // Преобразует минуты в радианы
   inline double constexpr fi_to_rad(double x)
   {
      return grad_to_rad(x * MIN_TO_RAD);
   }

   // Преобразует узлы в мс
   inline double constexpr knots_to_mc(double x)
   {
      return x * KNOOT_TO_MS;
   }

   // Преобразует мс в узлы
   inline double constexpr mc_to_knots(double x)
   {
      return x * MS_TO_KNOOT;
   }

   // Разность курсов
   inline double calc_DK(double K_old, double K_new)
   {
      return normal_K180(K_new - K_old);
   }

   // Направление от точки A к B, градусы
   inline double direction(const geo_point& A, const geo_point& B)
   {
      double dw = (B.lon - A.lon) * cos(grad_to_rad(0.5*(B.lat + A.lat)));
      return math::normal_K360(math::rad_to_grad(atan2(dw, (B.lat - A.lat))));
   }

   // Разность курсов по 3-м точкам
   inline double calc_DK(const geo_point& A, const geo_point& B, const geo_point& C)
   {
      return calc_DK(direction(A, B), direction(B, C));
   }

   // Курс между K0 и K1 ?
   inline bool between_K(double K, double K0, double K1)
   {
      double DK01 = fabs(calc_DK(K0, K1));
      double DK_0K = fabs(calc_DK(K0, K));
      double DK_K1 = fabs(calc_DK(K, K1));
      return DK_0K + DK_K1 - DK01 < 0.5;
   }

   // Пересчет параметров цели из истинных в относительные (K,V - свои; KC,VC - цели)
   inline void KV_tru_to_rel(double K, double V, double KC, double VC, double *KC_rel, double *VC_rel, double MIN_V)
   {
      if (V < MIN_V) // наше судно неподвижно
      {
         *KC_rel = KC; // относительное и истинное движение совпадают
         *VC_rel = VC;
         return;
      }
      if (VC < MIN_V) // цель неподвижна
      {
         *KC_rel = normal_K360(K + 180); // относительное направление - обратно нашему судну
         *VC_rel = V;
         return;
      }
      // перевод в радианы
      double KR = grad_to_rad(K);
      double KCR = grad_to_rad(KC);

      double V0_cos = V * cos(KR);
      double V0_sin = V * sin(KR);
      double VC_cos = VC * cos(KCR);
      double VC_sin = VC * sin(KCR);
      double VC_rel_cos = VC_cos - V0_cos;
      double VC_rel_sin = VC_sin - V0_sin;
      *VC_rel = sqrt(sqr(VC_rel_cos) + sqr(VC_rel_sin));
      double KC_relA = atan2(VC_rel_sin, VC_rel_cos);
      KC_relA = rad_to_grad(KC_relA); // в градусы
      *KC_rel = normal_K360(KC_relA); // нормализовали
   }

   // Пересчет параметров цели из  относительных в  истинные(K,V - свои; KC,VC - цели)
   inline void KV_rel_to_tru(double K, double V, double *KC, double *VC, double KC_rel, double VC_rel, double MIN_V)
   {
      if (V < MIN_V) // наше судно неподвижно
      {
         *KC = KC_rel; // относительное и истинное движение совпадают
         *VC = VC_rel;
         return;
      }
      if (VC_rel < MIN_V) // относительная скорость мала
      {
         *KC = K; // истинное направление - параллельно нашему судну
         *VC = V;
         return;
      }
      // перевод в радианы
      double KR = grad_to_rad(K);
      double KC_relR = grad_to_rad(KC_rel);

      double VC_rel_cos = VC_rel * cos(KC_relR);
      double VC_rel_sin = VC_rel * sin(KC_relR);
      double V0_cos = V * cos(KR);
      double V0_sin = V * sin(KR);
      double VC_cos = VC_rel_cos + V0_cos;
      double VC_sin = VC_rel_sin + V0_sin;
      *VC = sqrt(sqr(VC_cos) + sqr(VC_sin));
      double KC_A = atan2(VC_sin, VC_cos);
      KC_A = rad_to_grad(KC_A); // в градусы
      *KC = normal_K360(KC_A);  // нормализовали
   }

   template<class T>
   T clamp(T x, T xMin, T xMax)
   {
      if (x < xMin)
         return xMin;
      else if (x > xMax)
         return xMax;
      return x;
   }

//    inline double NauticalMilesOffset(double lat1, double lat2, double lon1, double lon2)
//    {
//       if (cg::eq(lat1, lat2) && cg::eq(lon1, lon2))
//          return 0;
// 
//       const double a = cg::deg2rad(fabs(cg::norm180(90 - lat1)));
//       const double b = cg::deg2rad(fabs(cg::norm180(90 - lat2)));
//       const double phi = cg::deg2rad(fabs(cg::norm180(lon1 - lon2)));
// 
//       const double c = acos(cos(a)*cos(b) + sin(a)*sin(b)*cos(phi));
//       return 60 * cg::rad2deg(c);
//    }

   
   inline void distance_direction(const geo_point& A, const geo_point& B, double& dist, double& dir)
   {
      double LatMed = grad_to_rad(0.5*(A.lat + B.lat));
      double cosLatMed = cos(LatMed);
      double dlon = B.lon - A.lon;
      double dlat = B.lat - A.lat;
      double dw = dlon * cosLatMed;
      dir = math::normal_K360(math::rad_to_grad(atan2(dw, dlat )));
      dist = sqrt(sqr(dlat) + sqr(dw)) * 60;
   }

   // Вычисляет квадрат расстояние от A до B, мили
   inline double distance_sqr(const geo_point& A, const geo_point& B)
   {
      double LatMed = grad_to_rad(0.5*(A.lat + B.lat));
      return ( sqr(A.lat - B.lat) + sqr((A.lon - B.lon) * cos(LatMed))) * 60*60;
   }

   // Вычисляет расстояние от A до B, мили
   inline double distance(const geo_point& A, const geo_point& B)
   {
      return sqrt( distance_sqr( A, B ) );
   }

   // Вычисляет расстояние от A до B в прямоугольной системе координат
   inline double distance(const point& A, const point& B)
   {
      const point displacement = A - B;
      return hypot(displacement.x, displacement.y);
   }

   // Вычисляет направление от A до B в прямоугольной системе координат
   inline double direction(const point& A, const point& B)
   {
      const point displacement = B - A;
      return normal_K360(rad_to_grad(atan2(displacement.x, displacement.y)));
   }

   template< typename TPoint >
   inline typename std::enable_if<!std::is_arithmetic<TPoint>::value, bool>::type
   isEqual(const TPoint& A, const TPoint& B, double eps)
   {
      return math::distance(A, B) < eps;
   }

   template<typename T>
   inline typename std::enable_if<std::is_arithmetic<T>::value, bool>::type
   isEqual( T A, T B, T eps)
   {
      return fabs(A-B) < eps;
   }
   

   /* Вычисление точки, расположенной от точки A на расстоянии dist по направлению course
   \param[in] dist в милях
   \param[in] course в градусах
   \return geo_point
   */
   inline geo_point calc_point(const geo_point& A, double dist, double course)
   {
      dist *= MIN_TO_RAD;
      const auto courseRad = math::grad_to_rad(course);
      double dLat = dist*cos(courseRad);
      double LatMed = grad_to_rad(0.5*(A.lat + A.lat + dLat));
      double dLon = dist * sin(courseRad) / cos(LatMed);
      return geo_point(A.lat + dLat, A.lon + dLon);
   }

   // Возращает скалярное произведение отрезков AB и AC
   template< typename TPoint >
   inline double dot_product_geo(const TPoint& A, const TPoint& B, const TPoint& C)
   {
      return (B.lon - A.lon) * (C.lon - A.lon) + (B.lat - A.lat) * (C.lat - A.lat);
   }

   // Возращает знаковый модуль векторного произведения отрезков AB и AC
   template< typename TPoint >
   inline double cross_product_dir_geo(const TPoint& A, const TPoint& B, const TPoint& C)
   {
      return (B.lon - A.lon) * (C.lat - A.lat) - (C.lon - A.lon) * (B.lat - A.lat);
   }
   
   inline double cross_product_dir_geo(const point& A, const point& B, const point& C)
   {
      return (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y);
   }

   enum class SIDE : char
   {
      S_LEFT,
      S_RIGHT,
      S_MIDDLE
   };

   enum class REL_POS : char
   {
      P_INFRONT,
      P_BEHIND,
      P_MIDDLE
   };

   // Возвращает сторону расположение точки С от отрезка AB
   template< typename TPoint >
   typename std::enable_if<!std::is_convertible<TPoint, geo_point>::value, SIDE>::type
   side(const TPoint& A, const TPoint& B, const TPoint& C, double eps = 1e-6)
   {     
      double vp = cross_product_dir_geo(A, B, C);
      return isEqual(vp, 0., eps) ? SIDE::S_MIDDLE : ((vp > 0) ? SIDE::S_LEFT : SIDE::S_RIGHT);
   }

   template< class TPoint1, class TPoint2, class TPoint3 >
   typename std::enable_if<std::is_convertible<TPoint1, geo_point>::value, SIDE>::type
      side(const TPoint1& A, const TPoint2& B, const TPoint3& C, double eps = 1e-6)
   {
      double vp = cross_product_dir_geo<geo_point>(A, B, C);
      return isEqual(vp, 0., eps) ? SIDE::S_MIDDLE : ((vp > 0) ? SIDE::S_LEFT : SIDE::S_RIGHT);
   }
   
   // Возвращает расположение точки С от вектора AB
   template< typename TPoint >
   inline REL_POS rel_pos(const TPoint& A, const TPoint& B, const TPoint& C, double eps = 1e-6)
   {
      double sp = dot_product_geo(A, B, C);
      return isEqual(sp, 0., eps) ? REL_POS::P_MIDDLE : ((sp > 0) ? REL_POS::P_INFRONT : REL_POS::P_BEHIND);
   }
   
   inline REL_POS rel_pos(const geo_point& A, const geo_point& B, const geo_point& C, double eps = 1e-3)
   {
      const auto distAB = distance(A, B)* MILE;
      const auto dirAB = grad_to_rad(direction(A, B));
      const auto distAC = distance(A, C)* MILE;
      const auto dirAC = grad_to_rad(direction(A, C));
      math::point B_{ cos(dirAB) * distAB, sin(dirAB) * distAB };
      math::point C_{ cos(dirAC) * distAC, sin(dirAC) * distAC };
      double sp = dot_product_geo(math::point{ 0,0 }, B_, C_);
      return isEqual(sp, 0., eps) ? REL_POS::P_MIDDLE : ((sp > 0) ? REL_POS::P_INFRONT : REL_POS::P_BEHIND);
   }

   using VectorPoints = std::vector<geo_point>;

   struct Arc
   {
      inline colreg::geo_point& start()              { return _arc[0]; }
      inline colreg::geo_point& end()                { return _arc[1]; }
      inline colreg::geo_point& center()             { return _arc[2]; }
      inline double& offset()                      { return _offset; }

      inline const colreg::geo_point& start()  const { return _arc[0]; }
      inline const colreg::geo_point& end()    const { return _arc[1]; }
      inline const colreg::geo_point& center() const { return _arc[2]; }
      inline const double& offset()          const { return _offset; }
   private:
      using ArcPoints = colreg::geo_point[3];
      ArcPoints _arc;
      double _offset;   //расстояние от угловой точки до точки start/end
   };
   /*!
   Вычисление дуги окружности заданного радиуса, вписанной в угол между двумя отрезками AB и BC.
   \return true - eсли окружность вписывается
   \param[out] arc дуга
   */
   inline bool inscribe_arc_to_angle(const geo_point& A, const geo_point& B, const geo_point& C, double radius, Arc& arc)
   {
      ATLASSERT(!(A == B || A == C || B == C));

      const auto angle1    = direction(A, B);
      const auto angle2    = direction(B, C);
      const auto dk        = calc_DK(angle1, angle2);
      const auto turnAngle = 180 - fabs(dk);
      arc.offset() = fabs(radius / tan(math::grad_to_rad(turnAngle * 0.5)));//расстояние от угловой точки до точки касательния с окружностью, мили
      if (distance(A, B) < arc.offset() || distance(B, C)< arc.offset())
         return false; //не вписать окружность в угол

      arc.start()       = calc_point(B, arc.offset(), angle1 + 180.);
      arc.end()         = calc_point(B, arc.offset(), angle2);
      arc.center()      = dk > 0 ? calc_point(arc.start(), radius, angle1 + 90.) : calc_point(arc.start(), radius, angle1 - 90.);
      return true;
   }

   /*!
   Вычисление дуги окружности заданного радиуса, вписанной в угол между прямой из точки А по направлению dir и точкой С.
   \return true - eсли окружность вписывается
   \param[out] arc[0] касательная к AB
   \param[out] arc[1] касательная к BC
   \param[out] arc[2] центр окружности
   */
   inline bool inscribe_arc_to_angle(const geo_point& A, double dir, const geo_point& C, double radius, Arc& arc)
   {
      auto angle1 = dir;
      auto angle2 = direction(A, C);
      auto dk = calc_DK(angle1, angle2);

      auto delta = fabs(calc_DK(angle1, angle2));
      //    if(delta<1)
      //       return false;

      auto turnAngle = 180 - delta;

      arc.start() = A;
      //найдем центр окружности
      arc.center() = /*s == SIDE::S_RIGHT*/ dk > 0 ? calc_point(arc.start(), radius, normal_K360(angle1 + 90)) : calc_point(arc.start(), radius, normal_K360(angle1 - 90));

      //гипотенуза 
      auto h = distance(arc.center(), C);
      ATLASSERT(h > 0);
      ATLASSERT(radius < h);
      auto a = math::rad_to_grad(acos(radius / h));
      auto angleToCenter = direction(arc.start(), arc.center());

      arc.end() = dk > 0 ? calc_point(arc.center(), radius, angleToCenter - a) : calc_point(arc.center(), radius, angleToCenter + a);

      return true;
   }

   // Вычисление точек касательных двух окружностей (параллельные)
   inline bool tangents_between_circles_parallel(const geo_point& c1, const geo_point& c2, double r1, double r2, geo_point& p11, geo_point& p12, geo_point& p21, geo_point& p22)
   {
      const auto distBetweenCentres = distance(c1, c2);
      const auto angleBetweenCentres = direction(r2 > r1 ? c2 : c1, r2 > r1 ? c1 : c2); //направление от бОльшего к меньшему
      if (distBetweenCentres < (r1 + r2) || r1 <= 0. || r2 <= 0.) //не рассматриваем пересекающиеся окружности
         return false;

      //
     
      /////////////////////точки касательных к окружностям//////////////////////
      if (isEqual(r1, r2, 1e-8) )
      {
         p11 = calc_point(c1, r1, angleBetweenCentres + 90);
         p12 = calc_point(c1, r1, angleBetweenCentres - 90);

         p21 = calc_point(c2, r2, angleBetweenCentres + 90);
         p22 = calc_point(c2, r2, angleBetweenCentres - 90);

      }
      else
      {
         double smallRad = std::min(r1, r2);
         double bigRad = std::max(r1, r2);          
         double smallHypot = distBetweenCentres / (bigRad / smallRad - 1);
         double angle = rad_to_grad( acos(smallRad / smallHypot) );
   
         p11 = calc_point(c1, r1, angleBetweenCentres + angle);
         p12 = calc_point(c1, r1, angleBetweenCentres - angle);

         p21 = calc_point(c2, r2, angleBetweenCentres + angle);
         p22 = calc_point(c2, r2, angleBetweenCentres - angle);
      }
      //////////////////////////////////////////////////////////////////////////
      return true;
   }

   // Вычисление точек касательных двух окружностей (диагональные)
   inline bool tangents_between_circles_diagonal(const geo_point& c1, const geo_point& c2, double r1, double r2, geo_point& p11, geo_point& p12, geo_point& p21, geo_point& p22)
   {
      const auto distBetweenCentres = distance(c1, c2);
      const auto angleBetweenCentres = direction(c1, c2);
      if (distBetweenCentres < (r1 + r2) || r1 <= 0. || r2 <= 0.) //не рассматриваем пересекающиеся окружности
         return false;

      // гипотенузы
      const auto h2 = distBetweenCentres / (1 + (r1 / r2));
      const auto h1 = distBetweenCentres - h2;
      const auto angle1 = RAD_TO_DEG*(acos(r1 / h1));
      const auto angle2 = RAD_TO_DEG*(acos(r2 / h2));

      /////////////////////точки касательных к окружностям//////////////////////
      p11 = calc_point(c1, r1, angleBetweenCentres + angle1);
      p12 = calc_point(c1, r1, angleBetweenCentres - angle1);

      p21 = calc_point(c2, r2, angleBetweenCentres + 180 - angle2);
      p22 = calc_point(c2, r2, angleBetweenCentres + 180 + angle2);
      //////////////////////////////////////////////////////////////////////////
      return true;
   }


   /* Решение квадратного уравнения.
   \param[out] Xs Корени уравнения
   \return Возвращает число найденных решений - 0, 1 или 2
   */
   template< class T >
   inline auto quadratic(T A, T B, T C, T Xs[2])
   {
      double d = B*B - 4.0*A*C;
      if (d > 0.0)
      {
         Xs[0] = (-B + sqrt(d)) / (2.0*A);
         Xs[1] = (-B - sqrt(d)) / (2.0*A);
         return 2;
      }
      else if (d == 0.0)
      {
         Xs[0] = -B / (2.0*A);
         return 1;
      }

      return 0;
   };


   inline geo_rect get_rect_bound(const geo_points& points)
   {
      geo_rect rectBound;
      for (const auto & p : points)
      {
         if ( p.lon < rectBound.leftTop.lon)
            rectBound.leftTop.lon = p.lon;
         if (p.lon > rectBound.rightBottom.lon)
            rectBound.rightBottom.lon = p.lon;
         if (p.lat < rectBound.leftTop.lat)
            rectBound.leftTop.lat = p.lat;
         if (p.lat > rectBound.rightBottom.lat)
            rectBound.rightBottom.lat = p.lat;
      }
      return rectBound;
   }

   template< class T>
   inline bool point_in_rect(const T& t, const T& p1, const T& p2)
   {
      auto x0 = std::min(p1.lon, p2.lon);
      auto x1 = std::max(p1.lon, p2.lon);
      auto y0 = std::min(p1.lat, p2.lat);
      auto y1 = std::max(p1.lat, p2.lat);
      return t.lon >= x0 && t.lon <= x1 && t.lat >= y0 && t.lat <= y1;
   }

   
   //! точка пересечения прямых через точки AB и BC
   inline colreg::geo_point intersection(const geo_point& A, const geo_point& B, const geo_point& C, const geo_point& D)
   {
      return intersection( create_straight(A, B), create_straight(C, D));
   }

   //! точка пересечения прямых заданных точкой A и направленим dirA и точкой B и направленим dirB (направление в градусах).
   inline colreg::geo_point intersection(const geo_point& A, double dirA, const geo_point& B, double dirB)
   {
      return intersection(create_straight(A, calc_point(A, 1., dirA)), create_straight(B, calc_point(B, 1., dirB)));
   }

   /* Пересечение двух отрезков заданных точками AB и CD
   \return Пересекеются ли отрезки
   */
   inline bool is_segments_intersection(const geo_point& A, const geo_point& B, const geo_point& C, const geo_point& D)
   {  
      return is_intersect_coord(A.lat, B.lat, C.lat, D.lat) && is_intersect_coord(A.lon, B.lon, C.lon, D.lon)
         && side(A, B, C) != side(A, B, D) && side(C, D, A) != side(C, D, B);
   }
   
   /* 
   Проекция точки С на прямую AB
   */
   template< typename TPoint >
   inline TPoint projection(const TPoint& A, const TPoint& B, const TPoint& C) // проекция точки С на прямую AB
   {
      const double x = B.lat - A.lat; //x и y - координаты вектора, перпендикулярного к AB
      const double y = A.lon - B.lon;
      const double L = (A.lon*B.lat - B.lon*A.lat + A.lat*C.lon - B.lat*C.lon + B.lon*C.lat - A.lon*C.lat) / (x*x + y*y);
      return TPoint{C.lat + y * L, C.lon + x * L};
   }

   /* Получить проекцию точки C на отрезок AB
   \param[out] pProj - проекция точки на AB
   \return Лежит ли проекция внутри отрезока отрезок
   */
   template< typename TPoint >
   inline bool point_projection_on_segment(const TPoint& A, const TPoint& B, const TPoint& C, TPoint& pProj, double eps = 1e-6)
   {
      if (rel_pos(A, B, C, eps) == REL_POS::P_BEHIND)
         return false;

      if (rel_pos(B, A, C, eps) == REL_POS::P_BEHIND)
         return false;

      pProj = projection(A, B, C);
      return true;
   }

   inline bool point_projection_on_segment(const geo_point& A, const geo_point& B, const geo_point& C, geo_point& pProj, double eps = 1e-6)
   {
      if (rel_pos(A, B, C, eps) == REL_POS::P_BEHIND)
         return false;

      if (rel_pos(B, A, C, eps) == REL_POS::P_BEHIND)
         return false;

      pProj = projection(A, B, C);
      return true;
   }

   /* 
   Получить дистанцию от точки C до отрезка AB, мили
   */
   template<class T>
   inline double dist_to_segment(const T& A, const T& B, const T& C)
   {
      if( rel_pos(A, B , C) == REL_POS::P_BEHIND )
         return distance(C, A);
      
      if (rel_pos(B, A, C) == REL_POS::P_BEHIND)
         return distance(C, B);
    
      return distance(C, projection(A, B, C));
   }

   // Вернуть синус угла заданного в градусах
   template< typename T >
   T sin_grad(T x)
   {
      return sin(grad_to_rad(x));
   }

   // Вернуть синус угла заданного в градусах
   template< typename T >
   T cos_grad(T x)
   {
      return cos(grad_to_rad(x));
   }

   enum class INSIDE_SECTOR_RES : char
   {
      ISR_INSIDE,
      ISR_OUTSIDE,
      ISR_BORDER
   };

   
   //! Находится ли точка check внутри сектора с центром center от startAngle до endAngle (угол сектора < 180)
   inline INSIDE_SECTOR_RES is_inside_sector(const geo_point& center, const geo_point& startAngle, const geo_point& endAngle, const geo_point& check, double eps = 1e-10)
   {
      const auto sideSector = side(center, startAngle, endAngle, eps);
      const auto s1 = side(center, check, startAngle, eps);
      const auto s2 = side(center, check, endAngle, eps);
      if (s1 == SIDE::S_MIDDLE || s2 == SIDE::S_MIDDLE)
         return INSIDE_SECTOR_RES::ISR_BORDER;
      if (sideSector == SIDE::S_MIDDLE && s1 != s2)
         return  INSIDE_SECTOR_RES::ISR_INSIDE;
      return (s1 != sideSector && s2 == sideSector) ? INSIDE_SECTOR_RES::ISR_INSIDE : INSIDE_SECTOR_RES::ISR_OUTSIDE;
   }

   //! Находится ли точка check внутри или в радиусе R метров от границы сектора с центром center от startAngle до endAngle (угол сектора < 180)
   inline INSIDE_SECTOR_RES is_near_sector(const geo_point& center, const geo_point& startAngle, const geo_point& endAngle, const geo_point& check, double R, double eps = 1e-10)
   {
      R *= METERS_TO_MILE;
      if( math::distance(startAngle, check) < R || math::distance(endAngle, check) < R)
         return INSIDE_SECTOR_RES::ISR_BORDER;

      return is_inside_sector(center, startAngle, endAngle, check, eps);
   }

   inline INSIDE_SECTOR_RES is_inside_sector(const point& center, const point& startAngle, const point& endAngle, const point& check, double eps = 1e-7)
   {
      const auto sideSector = side(center, startAngle, endAngle, eps);
      const auto s1 = side(center, check, startAngle, eps);
      const auto s2 = side(center, check, endAngle, eps);
      if (s1 == SIDE::S_MIDDLE || s2 == SIDE::S_MIDDLE)
         return INSIDE_SECTOR_RES::ISR_BORDER;
      return (s1 != sideSector && s2 == sideSector) ? INSIDE_SECTOR_RES::ISR_INSIDE : INSIDE_SECTOR_RES::ISR_OUTSIDE;
   }
   
   
   /*
   Пересекает ли отрезок p1-p2 сектор с центром center от startAngle до endAngle
   */
   inline bool is_intersect_sector(const geo_point& center, const geo_point& startAngle, const geo_point& endAngle, const geo_point& p1, const geo_point& p2, double eps = 1e-10)
   {
      const auto sideSector = side(center, startAngle, endAngle, eps );
      const auto sideP1_End = side(center, p1, endAngle, eps);
      const auto sideP2_End = side(center, p2, endAngle, eps);

      if (sideP1_End == SIDE::S_MIDDLE || sideP2_End == SIDE::S_MIDDLE)
          return true;
      const auto sideP1_Start = side(center, p1, startAngle, eps);
      const auto sideP2_Start = side(center, p2, startAngle, eps);
      const auto pAdd = calc_point(center, 1.0, direction(p1, p2));
      const auto sidePAdd_Start = side(center, pAdd, startAngle, eps);
      const auto sidePAdd_End = side(center, pAdd, endAngle, eps);
      const auto sidePAdd_P1 = side(center, pAdd, p1, eps);
      const auto sidePAdd_P2 = side(center, pAdd, p2, eps);
      return ((sideP1_End == sideSector || sideP1_End == SIDE::S_MIDDLE || sideP2_End == sideSector || sideP2_End == SIDE::S_MIDDLE) &&
         (sideP1_Start != sideSector || sideP1_Start == SIDE::S_MIDDLE || sideP2_Start != sideSector || sideP2_Start == SIDE::S_MIDDLE)) &&
         (sidePAdd_P1 == sidePAdd_Start || sidePAdd_P1 == sidePAdd_End || sidePAdd_P2 == sidePAdd_Start || sidePAdd_P2 == sidePAdd_End);
   }

   /* нахождение значения y по x, с помощью линейной интерполяции на отрезке X1Y1 - X2Y2

   |      /
Y2 |-----/
 y |    /
   |   /
Y1 |--/
   |_/____________
      X1 x X2
   */
   inline double linear_interpolation(double x, double X1, double X2, double Y1, double Y2)
   {
      if (X1 == X2)
      { 
        return 0.5*(Y1 + Y2);
      }
      else if (X1 < X2)
      {
        if (x < X1)
          return Y1;
        if (x > X2)
          return Y2;
      }
      else
      {
        if (x > X1)
          return Y1;
        if (x < X2)
          return Y2;
      }
      return Y1 + (x - X1) * (Y2 - Y1) / (X2 - X1);
   }

   inline point calc_point( const point& from, double dist, double dir)
   {
      return from + point{ cos_grad(dir) * dist, sin_grad(dir) * dist };
   }


   //! Находится ли угол check внутри сектора от angleFrom до angleTo в градусах
   inline INSIDE_SECTOR_RES is_inside_sector(double angleFrom, double angleTo, double check)
   {
      point center;
      return  is_inside_sector(center, math::calc_point(center, 1, angleFrom), math::calc_point(center, 1, angleTo), math::calc_point(center, 1, check));
   }


}

//Transform.h использует math_utils, поэтому по человечески не получилось
#include "transform.h"

namespace math
{
   inline double dist_to_segment(const colreg::geo_point& A, const colreg::geo_point& B, const colreg::geo_point& C)
   {
      math::GeoToCartasianTransform transform(C, 0.0);
      const auto mA = transform.ToLocal(A);
      const auto mB = transform.ToLocal(B);
      const auto mC = transform.ToLocal(C);
      return distance(mC, projection(mA, mB, mC));
   }
}