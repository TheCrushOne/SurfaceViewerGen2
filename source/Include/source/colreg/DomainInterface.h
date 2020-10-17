#pragma once
#include "CommonStructs.h"
#include "DomainStructs.h"
#include "CommonRefs.h"

//namespace colreg
//{
//#pragma pack (push, 1)
//
//   using polar_ref = base_ref<polar>;
//
//   struct domain_geometry
//   {
//      DOMAIN_GEOMETRY_TYPE type;   ///< Domain geometry interpolation type
//      polar_ref            points; ///< Points in polar, specifying relative domain geometry. Zero course is the ship's course
//   };
//
//   //!тип коэффициентов домена
//   enum DOMAIN_SCALE_TYPE
//   {
//      DST_USER = 0,      //< Пользовательские коэффициенты домена
//      DST_TEMPORARY      //< Коэффициенты временного сжатия домена, для расхождения с уменьшинными доменами без генерации опасностей.
//   };
//
//   using domain_geometry_ref = base_ref<domain_geometry>;
//
//   struct iDomain
//   {
//      /*!
//      Get safe radius
//      \return Radius at which it is guaranteed that no dangerous situation will occur, miles
//      */
//      virtual double GetSafeRadius(const ship_info& targetShip, double speed) const = 0;
//
//      /*!
//      Get domain width
//      \param[in] position Track point at which the safety domain topology should be calculated
//      \param[in] chartContext Track point's chart context
//      \return Domain's border information, miles
//      */
//      virtual domain_border_info GetBorderInfo(const track_point_info& position) const = 0;
//
//      /*!
//      Get domain topology for the specified track point
//      \param[in] points Track points at which the safety domain topology should be calculated
//      \param[in] chartContext Track point's chart context
//      \param[in] size Track points count
//      \return Calculated domain topology at provided track point
//      */
//      virtual const domain_geometry_ref* GetTopology(const track_point_info* points, size_t size) const = 0;
//
//      /*! 
//      Get ship danger estimation
//      \param[in] points Track points at which the safety domain topology should be calculated
//      \param[in] chartContext Track point's chart context
//      \param[in] size Track points count
//      \param[in] targetShip Target ship information at the moment the check is performed
//      \param[in] targetPos Target ship position at the moment the check is performed
//      \param[in] targetPosNext Target ship position at the next time step moment the check is performed
//      \return Estimated danger information
//      */
//      virtual ship_danger_estimation EstimateShipDanger(const track_point_info* points, size_t size,
//                                                        const ship_info& targetShip,
//                                                        const track_point_info& targetPos,
//                                                        const track_point_info& targetPosNext) const = 0;
//
//      /*!
//      Set user or temporary domain scale coefficient
//      \DST_TEMPORARY scale Should be setted after getting solution with loosening.type == colreg::SETTINGS_CORRECTION_TYPE::SCT_REDUCE_DANGER_SHIPS_DOMAIN)
//      \In state-full mode domain scale will be restored to origin as soon as possible
//      */
//      virtual void SetDomainScales(DOMAIN_SCALE_TYPE type, const domain_scales& coefs) = 0;
//
//      virtual domain_scales GetDomainScales(colreg::DOMAIN_SCALE_TYPE type = colreg::DOMAIN_SCALE_TYPE::DST_USER) const = 0;
//   };
//#pragma pack (pop)
//}
