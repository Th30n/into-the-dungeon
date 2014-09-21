#ifndef IMLCOMMENT_H
#define IMLCOMMENT_H

#include "IMLNode.h"

/*
 * This is a comment node of IML document.
 */
class IMLComment: public IMLNode {
  public:
    IMLComment(std::string comment) : IMLNode(comment) {}
    virtual ~IMLComment() {}
    virtual void writeToStream(std::ostream &ostream) const;

};

#endif
