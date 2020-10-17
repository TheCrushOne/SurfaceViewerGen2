#pragma once

#include "common/base_types.h"

namespace SV::layer_provider
{
   // TODO: переделать на три типа
   //! Информация по кораблю
   struct ship_info
   {
      ship_info()
      {
         memset(this, 0, sizeof(ship_info));
         safety_contour = draft = -1;
      }

      char                 name[MAX_SHIP_NAME_LENGHT];         ///< Название судна
      id_type              id;                                 ///< Уникальный идентификатор судна
      int                  length;                             ///< Длина, метры
      int                  width;                              ///< Ширина, метры
      int                  draft;                              ///< Осадка, метры
      int                  deadweight;                         ///< Дедвейт
      bool                 dangerous_cargo;                    ///< Опасный груз
      int                  safety_contour;                     ///< метры, Мин. Глубина для безопасного хождения корабля, может задаваться извне (капитаном). Не может быть меньше draft
   };
}