#ifndef IMLPARSER_H_
#define IMLPARSER_H_

#include <istream>
#include <list>

#include "IMLTag.h"

/*
 * This is parser for IML.
 * IML stands for Into the Dungeon Markup Language.
 * It based on XML and used for describing data of the game.
 * IML is much simpler than XML, tags cannot have attributes and only ASCII
 * characters are accepted.
 */
class IMLParser {
  public:
    IMLParser() : root_(0), line_(0), col_(0) {}
    // Returns true if successful parse.
    bool createTree(std::istream &source);
    IMLNode *getRoot() { return root_; }
  private:
    bool parseMarkup(std::istream &source, std::list<IMLNode*> &stack);
    bool parseOpenTag(std::istream &source, std::list<IMLNode*> &stack);
    bool parseCloseTag(std::istream &source, std::list<IMLNode*> &stack);
    bool parseComment(std::istream &source, std::list<IMLNode*> &stack);
    bool parseAttribute(std::istream &source, AttributesMap &attributes);
    bool isWhiteSpace(int c);

    IMLNode *root_;
    int line_;
    int col_;
};

#endif
