#ifndef IMLTAG_H_
#define IMLTAG_H_

#include "IMLNode.h"

#include <map>
#include <string>

typedef std::map<std::string, std::string> AttributesMap;

/* 
 * This class is an IML Tag node.
 */
class IMLTag : public IMLNode {
  public:
    IMLTag(std::string name, bool is_empty_tag);
    IMLTag(std::string n, AttributesMap attributes, bool is_empty_tag);
    virtual ~IMLTag() {}
    virtual void writeToStream(std::ostream &ostream) const;
    AttributesMap &getAttributes() { return attributes_; }
    const AttributesMap &getAttributes() const { return attributes_; }
  private:
    AttributesMap attributes_;
    bool is_empty_tag_;
};

#endif
