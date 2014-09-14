#ifndef IMLNODE_H_
#define IMLNODE_H_

#include <list>
#include <ostream>
#include <string>

/*
 * This is a node of an IML document.
 */
class IMLNode {
  public:
    IMLNode(std::string name) : name_(name) {}
    // Deletes all children
    virtual ~IMLNode();
    std::string getName() const { return name_; }
    std::list<IMLNode*> &getChildren() { return children_; }
    const std::list<IMLNode*> &getChildren() const { return children_; }
    // Returns true if added
    virtual bool addChild(IMLNode *node);
    // Writes xml string to stream.
    virtual void writeToStream(std::ostream &ostream) const;
    void print() const;
    // Find a node by it's name. Returns null if not found.
    virtual IMLNode *findByName(std::string name) const;
  
  private:
    std::string name_;
    std::list<IMLNode*> children_;
    
};

#endif
