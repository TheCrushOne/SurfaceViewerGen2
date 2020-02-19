#pragma once

#include <memory>

//namespace SVM
//{
//   template<class T>
//   class iMatrix;
//}

class iDispatchable
{
public:
   virtual void OnScenarioLoad() = 0;
   virtual ~iDispatchable() = default;
};