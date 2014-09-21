#include "iml/IMLTag.h"

#include <cstdio>

IMLTag::IMLTag(std::string n, AttributesMap attributes, bool is_empty_tag)
  : IMLNode(n), attributes_(attributes), is_empty_tag_(is_empty_tag)
{
}

void IMLTag::writeToStream(std::ostream &ostream) const
{
  ostream << "<" << getName();
  //printf("<%s", getName().c_str());
  AttributesMap::const_iterator it = attributes_.begin();
  for (; it != attributes_.end(); ++it) {
    ostream << " " << it->first << "=\"" << it->second << "\"";
    //printf(" %s=\"%s\"", it->first.c_str(), it->second.c_str());
  }
  if (is_empty_tag_) {
    ostream << "/";
    //printf("/");
  }
  ostream << ">";
  //printf(">");
  IMLNode::writeToStream(ostream);
  if (!is_empty_tag_) {
    ostream << "</" << getName() << ">";
    //printf("</%s>", getName().c_str());
  }
}
