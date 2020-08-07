#pragma once
#include <iostream>

// NOTE: пока что не используется

/*enum class ECBase : unsigned short
{
   First,
   Last
};

inline ECBase operator++(ECBase& x)
{
   return x = (ECBase)(std::underlying_type<ECBase>::type(x) + 1);
}

ECBase operator*(ECBase c)
{
   return c;
}

ECBase begin(ECBase r)
{
   return ECBase::First;
}

ECBase end(ECBase r)
{
   ECBase l = ECBase::Last;
   return ++l;
}*/