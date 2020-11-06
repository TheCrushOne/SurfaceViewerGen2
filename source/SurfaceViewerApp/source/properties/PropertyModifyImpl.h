/*!
\file
\brief Заголовочный файл property.h
*/
#pragma once
#include "colreg\XmlProperties\xml_property.h"
#include "colreg/PropertyInterface.h"
#include "PropertyModify.h"
#include "common/properties.h"
#include <functional>

//namespace SV
//{
//   class PropertyModifyImpl
//      : public iPropertyModify
//   {
//   public:
//      PropertyModifyImpl(const char* name, const char* description, bool readonly, VALUE_FORMAT_TYPE type)
//         : _name{ name }
//         , _description{ description }
//         , _readonly{ readonly }
//         , _format_type{ type }
//      {}
//
//      const char* get_name() const override { return _name.c_str(); }
//      const char* get_description() const override { return _description.c_str(); }
//      VALUE_FORMAT_TYPE get_value_format_type() const override { return _format_type; }
//      bool is_read_only() const override { return _readonly; }
//      const prop_interface_vct* get_childs() const override { return nullptr; }
//      //const VectorProperties* get_child_list()const override { return nullptr; };
//      void set_list(const std::vector<std::string>& list) override { _list = list; }
//      const std::vector<std::string>& get_list() const { return _list; }
//   protected:
//      std::string _name;
//      std::string _description;
//      VALUE_FORMAT_TYPE _format_type;
//      bool _readonly;
//      std::vector<std::string> _list;
//   };
//}