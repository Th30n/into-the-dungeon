#ifndef IMLVALUE_H_
#define IMLVALUE_H_

#include "IMLNode.h"

/*
 * This class represents a node of IML document that contains
 * content outside of tags.
 */
class IMLValue: public IMLNode {
  public:
    IMLValue(std::string name) : IMLNode(name) {}
    virtual ~IMLValue() {}
    virtual bool addChild(IMLNode *node) { return false; }
    virtual void writeToStream(std::ostream &ostream) const;
};

#endif
