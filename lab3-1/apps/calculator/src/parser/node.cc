#include "node.h"

namespace parser {

INode& InnerNode::AddChild(std::unique_ptr<INode>&& node) {
  children_.push_back(std::move(node));
  return *children_.back();
}

}  // namespace parser
