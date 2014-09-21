#include "iml/IMLParser.h"

#include <cctype>
#include <cstdio>
#include <fstream>
#include <string>

#include "iml/IMLComment.h"
#include "iml/IMLNode.h"
#include "iml/IMLTag.h"
#include "iml/IMLValue.h"

bool IMLParser::createTree(std::istream &source)
{
  line_ = 0;
  col_ = 0;
  std::string val;
  root_ = new IMLNode("Document");
  std::list<IMLNode*> stack;
  stack.push_back(root_);
  while (source.good()) {
    col_++;
    int c = source.get();
    if (c == '<') {
      IMLNode *n = new IMLValue(val);
      stack.back()->addChild(n);
      val.clear();
      if (!parseMarkup(source, stack)) {
        return false;
      }
    } else {
      val += c;
      if (c == '\n') {
        line_++;
        col_ = 0;
      }
    }
  }
  return true;
}

bool IMLParser::parseMarkup(std::istream &source, std::list<IMLNode*> &stack)
{
  if (source.good()) {
    col_++;
    int c = source.get();
    if (isalpha(c)) {
      source.unget();
      col_--;
      return parseOpenTag(source, stack);
    } else if (c == '/') {
      return parseCloseTag(source, stack);
    } else if (c == '!') {
      return parseComment(source, stack);
    } else {
      fprintf(stderr, "Invalid symbole '%c' at: %d : %d\n", c, line_, col_);
      return false;
    }
  }
  return false;
}

bool IMLParser::parseOpenTag(std::istream &source, std::list<IMLNode*> &stack)
{
  std::string name;
  int white = 0;
  bool emptyTag = false;
  AttributesMap attributes;
  while (source.good()) {
    col_++;
    int c = source.get();
    if (isalpha(c) && !emptyTag) {
      if (white) {
        source.unget();
        col_--;
        if (!parseAttribute(source, attributes)) {
          return false;
        }
      } else {
        name += c;
      }
    } else if (c == '>') {
      IMLNode *node = new IMLTag(name, attributes, emptyTag);
      stack.back()->addChild(node);
      if (emptyTag) {
        // Add Empty Tag Node?
        // Don't push on stack (doesn't have sub nodes)
      } else {
        // Pushed on stack (needs to be closed)
        stack.push_back(node);
      }
      return true;
    } else if (c == '/' && !emptyTag) {
      emptyTag = true;
    } else if (!name.empty() && isWhiteSpace(c) && !emptyTag) {
      white++;
    } else {
      fprintf(stderr, "Invalid symbol '%c' at: %d : %d\n", c, line_, col_);
      return false;
    }
  }
  return false;
}

bool IMLParser::parseCloseTag(std::istream &source, std::list<IMLNode*> &stack)
{
  std::string name;
  while (source.good()) {
    col_++;
    int c = source.get();
    if (isalpha(c)) {
      name += c;
    } else if (c == '>') {
      stack.pop_back();
      return true;
    } else {
      fprintf(stderr, "Invalid symbol '%c' at: %d : %d\n", c, line_, col_);
      return false;
    }
  }
  return false;
}

bool IMLParser::parseComment(std::istream &source, std::list<IMLNode*> &stack)
{
  int minus = 0;
  std::string comment;
  while (source.good()) {
    col_++;
    int c = source.get();
    comment += c;
    if (c == '-') {
      minus++;
    } else if (minus == 2 && c == '>') {
      comment = comment.substr(2, comment.length() - 5);
      IMLNode *node = new IMLComment(comment);
      stack.back()->addChild(node);
      return true;
    } else {
      minus = 0;
    }
  }
  return false;
}

bool IMLParser::parseAttribute(std::istream &source, AttributesMap &attributes)
{
  std::string key;
  std::string val;
  bool equals = false;
  int quotes = 0;
  while (source.good()) {
    col_++;
    int c = source.get();
    if (equals) {
      if (quotes == 1 && c != '"') {
        val += c;
      } else if (c == '"') {
        quotes++;
        if (quotes == 2) {
          attributes[key] = val;
          return true;
        }
      } else {
        fprintf(stderr,
            "Invalid symbol '%c' at: %d : %d\n", c, line_, col_);
        return false;
      }
    } else {
      if (isalpha(c)) {
        key += c;
      } else if (c == '=') {
        equals = true;
      } else {
        fprintf(stderr,
            "Invalid symbol '%c' at: %d : %d\n", c, line_, col_);
        return false;
      }
    }
  }
  return false;
}

bool IMLParser::isWhiteSpace(int c)
{
  return c == ' ' || c == '\t' || c == '\n';
}
