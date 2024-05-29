#include "node.h"

namespace parser {

namespace dt {

INode& InnerNode::AddChild(std::unique_ptr<INode>&& node) {
  children_.push_back(std::move(node));
  return *children_.back();
}

}  // namespace dt

}  // namespace parser
