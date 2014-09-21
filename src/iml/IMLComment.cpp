#include "iml/IMLComment.h"

void IMLComment::writeToStream(std::ostream &ostream) const
{
  ostream << "<!--" << getName() << "-->";
}
