#include "IMLNode.h"

#include <cstdio>
#include <iostream>

IMLNode::~IMLNode()
{
  std::list<IMLNode*>::iterator it = children_.begin();
  for (; it != children_.end(); it++) {
    delete *it;
  }
}

bool IMLNode::addChild(IMLNode *node)
{
  children_.push_back(node);
  return true;
}

void IMLNode::writeToStream(std::ostream &ostream) const
{
  std::list<IMLNode*>::const_iterator it = children_.begin();
  for (; it != children_.end(); it++) {
    (*it)->writeToStream(ostream);
  }
}

void IMLNode::print() const
{
  writeToStream(std::cout);
}

IMLNode *IMLNode::findByName(std::string name) const
{
  std::list<IMLNode*>::const_iterator it = children_.begin();
  for (; it != children_.end(); it++) {
    if ((*it)->getName() == name) {
      return *it;
    }
  }
  for (it = children_.begin(); it != children_.end(); it++) {
    IMLNode *n = (*it)->findByName(name);
    if (n) {
      return n;
    }
  }
  return 0;
}
