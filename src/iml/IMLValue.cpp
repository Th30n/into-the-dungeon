#include "IMLValue.h"

#include <cstdio>

void IMLValue::writeToStream(std::ostream &ostream) const
{
  ostream << getName();
}
