#pragma once

#include "research_types.h"

struct TimeData
{
   __int64 start;
   __int64 finish;
   __int64 diff;

   void apply() { diff = finish - start; }
};

struct TimeResearchComplexStorage
{
   struct StorageCell
   {
      struct Index
      {

      } index;
      struct Result
      {

      } result;
   };

   std::vector<StorageCell> data;
};

struct LengthResearchComplexStorage
{
   struct StorageCell
   {
      struct Index
      {

      } index;
      struct Result
      {

      } result;
   };

   std::vector<StorageCell> data;
};

struct ThreadResearchComplexStorage
{
   struct SuperCell
   {
      struct Index
      {
         size_t thread_pool_idx;
         size_t task_pool_idx;
         size_t fly_count_idx;

         size_t thread_pool_value;
         size_t task_pool_value;
         size_t fly_count_value;
      } index;

      struct Result
      {
         TimeData time;
      } result;
   };

   struct Info
   {
      size_t thread_pool_summary;
      size_t task_pool_summary;
      size_t fly_count_summary;
   };

   std::vector<SuperCell> data;
   Info info;
};