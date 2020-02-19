#pragma once

#include "FlyAttrs\FlyAttrClassW.h"
#include "TTCG\Common\Streams\raw_pipeline_ostream.h"
#include "TTCG\Common\Streams\pipeline_istream.h"
#include "TTCG\Common\Streams\pipeline_io.h"

//
// Forward declarations
//

namespace FlyAttrStructW
{
   template<class T> void Load( const fly_attrs_w &attr, T &data, const T &def = T() );
   template<class T> void Save( fly_attrs_w &attr, const T &data );
}

namespace std 
{
   template<class Type, class Allocator>                          class vector;
   template<class Type, class Allocator>                          class list;
   template<class Key, class Type, class Traits, class Allocator> class map;
}

template<class T, class Cmp> struct _share_string_t;
template<class T>            struct co_ptr;

namespace FlyAttrStructW
{
   enum ProcessMode
   {
      pm_load = 0,
      pm_save
   };

   // Structure for data processing description
   template<class T> struct DataDescr
   {
      static void Load( const fly_attrs_w &attr, LPCWSTR name, T &data, const T &def ) 
      {
         T::DataDescr::Load(attr, name, data, def);
      }

      static void Save( fly_attrs_w &attr, LPCWSTR name, const T &data ) 
      {
         T::DataDescr::Save(attr, name, data);
      }
   };

   // Structure for structures processing description
   template<class T> struct StructureDescr
   {
      typedef T Type; 

      static void Load( const fly_attrs_w &attr, LPCWSTR name, Type &data, const Type &def )
      {   
         fly_attrs_w child_attr = attr.get_branch(name);

         if (child_attr.valid())
            T::DataDescr::ProcessAttr(const_cast<fly_attrs_w &>(child_attr), data, def, pm_load);
         else
            data = def;
      }

      static void Save( fly_attrs_w &attr, LPCWSTR name, const Type &data )
      {  
         fly_attrs_w child_attr = attr.insert_branch(name);
         if (!child_attr.valid())
            return;

         T::DataDescr::ProcessAttr(child_attr, const_cast<Type &>(data), Type(), pm_save);

         if (child_attr.size() == 0)
            attr.remove_item(name);
      }
   };

   // Structure for simple types processing description
   template<class T> struct SimpleDescr
   {
      typedef T Type; 

      static void Load( const fly_attrs_w &attr, LPCWSTR name, Type &data, const Type &def ) 
      {
         data = attr.get_item(name, def);
      }                

      static void Save( fly_attrs_w &attr, LPCWSTR name, const Type &data )
      {
         attr.insert_item(name, data);
      }
   };

   // Structure for BLOB processing description
   template<class T> struct BlobDescr
   {
      typedef T Type; 

      static void Load( const fly_attrs_w &attr, LPCWSTR name, Type &data, const Type &def ) 
      {
         if (!attr.get_blob(name, data))
            data = def;
      }                

      static void Save( fly_attrs_w &attr, LPCWSTR name, const Type &data )
      {
         attr.insert_blob(name, data);
      }
   };

   // Structure for enum processing description
   template<class T> struct EnumDescr
   {
      typedef T Type;

      static void Load( const fly_attrs_w &attr, LPCWSTR name, Type &data, const Type &def ) 
      {
         unsigned int tmpData;

         SimpleDescr<unsigned int>::Load(attr, name, tmpData, def);
         data = static_cast<Type>(tmpData);
      }                

      static void Save( fly_attrs_w &attr, LPCWSTR name, const Type &data )
      {
         SimpleDescr<unsigned int>::Save(attr, name, data);
      }
   };

   // Structure for simple types containers processing description
   template<class T> struct SimpleTypeContainerDescr
   {
      typedef T Type;

      static void Load( const fly_attrs_w &attr, LPCWSTR name, Type &data, const Type &def )
      {
         const void *pData;
         DWORD      size;

         if (!attr.get_blob(name, &pData, size))
            data = def;
         else
         {
            pipeline_istream stream(pData, size);

            read(stream, data);
         }
      }                

      static void Save( fly_attrs_w &attr, LPCWSTR name, const Type &data )
      {
         raw_pipeline_ostream stream;

         write(stream, data);
         attr.insert_blob(name, stream.getData(), stream.getSize());
      }
   };

   // Structure for vector processing description
   template<class T, class A> struct DataDescr< std::vector<T, A> > :
   SimpleTypeContainerDescr< std::vector<T, A> >
   {
   };

   // Structure for list processing description
   template<class T, class A> struct DataDescr< std::list<T, A> >
   {
      typedef std::list<T, A> Type;

      static void Load( const fly_attrs_w &attr, LPCWSTR name, Type &data, const Type &def ) 
      {
         fly_attrs_w child_attr = attr.get_branch(name);
         if (!child_attr.valid())
         {
            data = def;
            return;
         }

         Type::const_iterator def_it = def.begin();
         fly_attrs_w::item_enum it     = fly_attrs_w::item_enum(child_attr);

         while (it)
         {
            T val;
            if ( def.size() > 0 )
               DataDescr<T>::Load(const_cast<fly_attrs_w &>(child_attr), it.key(), val, *def_it);
            else
               DataDescr<T>::Load(const_cast<fly_attrs_w &>(child_attr), it.key(), val, T());

            data.push_back(val);

            ++it;
            if (def_it != def.end())
               ++def_it;
         }
      }

      static void Save( fly_attrs_w &attr, LPCWSTR name, const Type &data )
      {
         fly_attrs_w child_attr = attr.insert_branch(name);
         if (!child_attr.valid())
            return;

         DWORD i = 1;

         for (Type::const_iterator it = data.begin(); it != data.end(); ++it)
         {
            wchar_t buf[10];
            swprintf_s(buf, L"#%07d", i++);

            DataDescr<T>::Save(child_attr, buf, const_cast<T &>(*it));
         }
      }
   };

   // Structure for map processing description
   template<class K, class V, class P, class A> struct DataDescr< std::map<K, V, P, A> > :
   SimpleTypeContainerDescr< std::map<K, V, P, A> >
   {
   };

   // Structure for string processing description
   template<class T, class Cmp> struct DataDescr< _share_string_t<T, Cmp> >
   {
      typedef _share_string_t<T, Cmp> Type;

      static void Load( const fly_attrs_w &attr, LPCWSTR name, Type &data, const Type &def )
      {
         LPCWSTR str, def_str = def;

         str = attr.get_item(name, def_str);
         data = str;
      }

      static void Save( fly_attrs_w &attr, LPCWSTR name, const Type &data )
      {
         LPCWSTR str = data;
         attr.insert_item(name, str);
      }
   };

   // Structure for co_ptr processing description
   template<class T> struct DataDescr< co_ptr<T> >
   {
      typedef co_ptr<T> Type;

      static void Load( const fly_attrs_w &attr, LPCWSTR name, Type &data, const Type &def )
      {
         data.release();

         if (attr.check_item(name))
         {
            Type def_val = def;

            if (def == NULL)
               *def_val.create();

            DataDescr<T>::Load(const_cast<fly_attrs_w &>(attr), name, *data.create(), *def_val.get());
         }
      }

      static void Save( fly_attrs_w &attr, LPCWSTR name, const Type &data )
      {
         if (data)
            DataDescr<T>::Save(attr, name, const_cast<T &>(*data.get()));
      }
   };

   // Structure for attributes processing description
   template<> struct DataDescr<_attributes_t>
   {
      typedef _attributes_t Type;

      static void Load( const fly_attrs_w &attr, LPCWSTR name, Type &data, const Type &def )
      {
         const void *pBlobData;
         DWORD      size;

         if (!attr.get_blob(name, &pBlobData, size))
            data = def;
         else
         {
            HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, size * sizeof(BYTE));

            memcpy(GlobalLock(hMem), pBlobData, size * sizeof(BYTE));
            GlobalUnlock(hMem);

            data.load(CF_ATR, hMem);
         }
      }

      static void Save( fly_attrs_w &attr, LPCWSTR name, const Type &data )
      {
         HGLOBAL buf;
         data.get_data(CF_ATR, &buf);

         attr.insert_blob(name, static_cast<const void *>(buf), DWORD(GlobalSize(buf)));
      }
   };


   // Default data loading
   template<typename T> void DoLoad( const fly_attrs_w &attr, LPCWSTR name, T &data, const T &def )
   {
      DataDescr<T>::Load(attr, name, data, def); 
   }

   // Default data saving
   template<typename T> void DoSave( fly_attrs_w &attr, LPCWSTR name, const T &data )
   {
      DataDescr<T>::Save(attr, name, data); 
   }

   // Data array loading
   template<typename T, int N> void DoLoad( const fly_attrs_w &attr, LPCWSTR name, T (&data)[N], const T (&def)[N] ) 
   {
      const void *pData;
      DWORD      size;

      if (!attr.get_blob(name, &pData, size))
         memcpy(&data[0], &def[0], N * sizeof(T));
      else
         memcpy(&data[0], pData, size * sizeof(T));
   }

   // Data array saving
   template<typename T, int N> void DoSave( fly_attrs_w &attr, LPCWSTR name, const T (&data)[N] ) 
   {
      attr.insert_blob(name, &data[0], N * sizeof(T));
   }

   // BLOB loading
   inline void DoLoad( const fly_attrs_w &attr, LPCWSTR name, void *pData, const void *pDefData ) 
   {
      const void *pBlobData;
      DWORD      size;

      if (!attr.get_blob(name, &pBlobData, size))
         pBlobData = pDefData;

      memcpy(pData, pBlobData, size * sizeof(BYTE));
   }

   // BLOB saving
   inline void DoSave( fly_attrs_w &attr, LPCWSTR name, const void *pData, DWORD dataSize ) 
   {
      attr.insert_blob(name, pData, dataSize);
   }

   // double array loading
   template<typename T> void DoLoadArray( const fly_attrs_w &attr, LPCWSTR name, T& data, const T &def )
   {
      fly_attrs_w::double_array arr;
      if ( attr.check_item( name ))
      {
         arr = attr.get_double_array(name);
         for ( size_t i = 0; i < arr.sz; ++i )
            data.insert( data.end(), arr.pData[i] );
      }
      else
         data = def;
   }

   // double array saving
   template<typename T> void DoSaveArray( fly_attrs_w &attr, LPCWSTR name, const T &data )
   {
      std::vector<double> vec( data.begin(), data.end() );
      fly_attrs_w::double_array arr;
      arr.pData = safe_begin(vec);
      arr.sz    = DWORD(vec.size());
      attr.insert_double_array(name, arr);
   }

   template<typename T> void Load( const fly_attrs_w &attr, T &data, const T &def )
   {
      T::DataDescr::ProcessAttr(const_cast<fly_attrs_w &>(attr), data, def, pm_load);
   }

   template<typename T> void Save( fly_attrs_w &attr, const T &data )
   {
      T::DataDescr::ProcessAttr(attr, const_cast<T &>(data), T(), pm_save);
   }

}

// Begin of fly attributes structure definition macros
#define BEGIN_FLYATTR_STRUCT(StructName)                                                                          \
   struct DataDescr : FlyAttrStructW::StructureDescr<StructName>                                                  \
   {                                                                                                              \
      static void ProcessAttr( fly_attrs_w &attr, Type &data, const Type &def, FlyAttrStructW::ProcessMode mode ) \
      {                                                                                                           \
         using namespace FlyAttrStructW;

// End of fly attributes structure definition macros 
#define END_FLYATTR_STRUCT() \
      }                      \
   };

// Fly attributes structure entry definition macros
#define FLYATTR_STRUCT_ENTRY_EDIT(DataField, DataFieldName)          \
   switch (mode)                                                     \
   {                                                                 \
      case pm_load:                                                  \
      {                                                              \
         DoLoad(attr, DataFieldName, data.DataField, def.DataField); \
         break;                                                      \
      }                                                              \
   case pm_save:                                                     \
      {                                                              \
         DoSave(attr, DataFieldName, data.DataField);                \
         break ;                                                     \
      }                                                              \
   }

// Fly attributes structure entry definition macros
#define FLYATTR_STRUCT_ENTRY(DataField)                   FLYATTR_STRUCT_ENTRY_EDIT(DataField, L#DataField)
#define FLYATTR_STRUCT_ENTRY_AS(DataField, DataFieldName) FLYATTR_STRUCT_ENTRY_EDIT(DataField, DataFieldName)

// Fly attributes structure members range definition macros
#define FLYATTR_STRUCT_RANGE_ENTRY_EDIT(StartDataField, EndDataField, DataName)          \
   switch (mode)                                                                         \
   {                                                                                     \
      case pm_load:                                                                      \
      {                                                                                  \
         void       *pData    = &data.StartDataField;                                    \
         const void *pDefData = &def.StartDataField;                                     \
                                                                                         \
         DoLoad(attr, DataName, pData, pDefData);                                        \
         break;                                                                          \
      }                                                                                  \
      case pm_save:                                                                      \
      {                                                                                  \
         const void *pData   = &data.StartDataField;                                     \
         DWORD      dataSize = (DWORD)&data.EndDataField - (DWORD)&data.StartDataField + \
         sizeof(data.EndDataField);                                                      \
                                                                                         \
         DoSave(attr, DataName, pData, dataSize);                                        \
         break;                                                                          \
      }                                                                                  \
   }

// Fly attributes structure range entry definition macros
#define FLYATTR_STRUCT_RANGE_ENTRY(StartDataField, EndDataField)              FLYATTR_STRUCT_RANGE_ENTRY_EDIT(StartDataField, EndDataField, #StartDataField L"_" #EndDataField)
#define FLYATTR_STRUCT_RANGE_ENTRY_AS(StartDataField, EndDataField, DataName) FLYATTR_STRUCT_RANGE_ENTRY_EDIT(StartDataField, EndDataField, DataName)

// Fly attributes structure array entry definition macros
#define FLYATTR_STRUCT_ARRAY_ENTRY_EDIT(DataField, DataFieldName)      \
   switch (mode)                                                       \
   {                                                                   \
   case pm_load:                                                       \
      {                                                                \
      DoLoadArray(attr, DataFieldName, data.DataField, def.DataField); \
      break;                                                           \
      }                                                                \
   case pm_save:                                                       \
      {                                                                \
      DoSaveArray(attr, DataFieldName, data.DataField);                \
      break ;                                                          \
      }                                                                \
   }

// Fly attributes structure array entry definition macros
#define FLYATTR_STRUCT_ARRAY_ENTRY(DataField)                   FLYATTR_STRUCT_ARRAY_ENTRY_EDIT(DataField, L#DataField)
#define FLYATTR_STRUCT_ARRAY_ENTRY_AS(DataField, DataFieldName) FLYATTR_STRUCT_ARRAY_ENTRY_EDIT(DataField, DataFieldName)

// Define fly attributes structure for BLOB macros
#define FLYATTR_BLOB(BLOBType)                                         \
   namespace FlyAttrStructW                                            \
   {                                                                   \
      template<> struct DataDescr<BLOBType> : BlobDescr<BLOBType> { }; \
   }

// Define fly attributes structure for enum types macros
#define FLYATTR_ENUM(TypeName)                                         \
   namespace FlyAttrStructW                                            \
   {                                                                   \
      template<> struct DataDescr<TypeName> : EnumDescr<TypeName> { }; \
   }

// Define fly attributes structure for simple types macros
#define FLYATTR_SIMPLE(TypeName)                                         \
   namespace FlyAttrStructW                                              \
   {                                                                     \
      template<> struct DataDescr<TypeName> : SimpleDescr<TypeName> { }; \
      template<class A> struct DataDescr< std::list<TypeName, A> > :     \
      SimpleTypeContainerDescr< std::list<TypeName, A> > { };            \
   }

#define FLYATTR_BSTR                                                     \
   namespace FlyAttrStructW                                              \
   {                                                                     \
      template<> struct DataDescr<_bstr_t> : SimpleDescr<_bstr_t> { };   \
   }


// Simple types processing definition
FLYATTR_SIMPLE( char             )
FLYATTR_SIMPLE( unsigned char    )
FLYATTR_SIMPLE( short            )
FLYATTR_SIMPLE( int              )
FLYATTR_SIMPLE( unsigned int     )
FLYATTR_SIMPLE( __int64          )
FLYATTR_SIMPLE( unsigned __int64 )
FLYATTR_SIMPLE( long             )
FLYATTR_SIMPLE( DWORD            )
FLYATTR_SIMPLE( float            )
FLYATTR_SIMPLE( double           )
FLYATTR_SIMPLE( bool             )
FLYATTR_SIMPLE( std::wstring     )
FLYATTR_SIMPLE( std::string      )

FLYATTR_BSTR
