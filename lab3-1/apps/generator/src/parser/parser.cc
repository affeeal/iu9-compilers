#include "parser.h"

#include <stack>

#include "analyzer_table.h"
#include "symbol.h"
#include "token.h"

namespace parser {

namespace {

void ThrowParseError(const lexer::Token& token, const std::string& name) {
  std::ostringstream err;
  err << token.get_coords() << ": expected " << name << ", got "
      << token.get_tag();
  throw std::runtime_error(err.str());
}

}  // namespace

INode& InnerNode::AddChild(std::unique_ptr<INode>&& node) {
  children_.push_back(std::move(node));
  return *children_.back();
}

std::unique_ptr<INode> Parser::TopDownParse(lexer::IScanner& scanner) {
  const auto table = AnalyzerTable();

  auto dummy = std::make_unique<InnerNode>();

  auto stack = std::stack<std::pair<Symbol, InnerNode*>>{};
  stack.push({kEndOfProgram, dummy.get()});
  stack.push({{table.get_axiom()}, dummy.get()});

  auto token = scanner.NextToken();

  do {
    const auto [symbol, parent] = stack.top();
    switch (symbol.get_type()) {
      case Symbol::Type::kTerminal: {
        if (symbol.get_name() != lexer::ToString(token->get_tag())) {
          ThrowParseError(*token, symbol.get_name());
        }

        stack.pop();
        parent->AddChild(std::make_unique<LeafNode>(std::move(token)));
        token = scanner.NextToken();
        break;
      }
      case Symbol::Type::kNonterminal: {
        const auto terminal =
            Symbol{lexer::ToString(token->get_tag()), Symbol::Type::kTerminal};
        const auto opt = table.Find(symbol, terminal);
        if (!opt.has_value()) {
          ThrowParseError(*token, symbol.get_name());
        }
        const auto [b, e] = opt.value();

        stack.pop();
        auto& child = static_cast<InnerNode&>(
            parent->AddChild(std::make_unique<InnerNode>()));
        for (auto rb = std::make_reverse_iterator(e),
                  re = std::make_reverse_iterator(b);
             rb != re; ++rb) {
          stack.push({*rb, &child});
        }
        break;
      }
    }
  } while (!stack.empty());

  return std::move(dummy->Children().front());
}

}  // namespace parser
