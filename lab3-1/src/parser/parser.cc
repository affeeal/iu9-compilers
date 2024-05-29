#include "parser.h"

#include <stack>

#include "ast.h"
#include "ast/analyzer_table.h"
#include "node.h"
#include "token.h"

namespace parser {

namespace {

void ThrowParseError(const lexer::Token& token, const std::string& name) {
  std::ostringstream err;
  err << token.get_coords().ToString() << ": expected " << name << ", got "
      << token.get_tag();
  throw std::runtime_error(err.str());
}

}  // namespace

std::unique_ptr<dt::INode> Parser::TopDownParse(lexer::IScanner& scanner) {
  const auto table = ast::AnalyzerTable();

  auto dummy = std::make_unique<dt::InnerNode>();

  auto stack = std::stack<std::pair<ast::Symbol, dt::InnerNode*>>{};
  stack.push({{"$", ast::Symbol::Type::kTerminal}, dummy.get()});
  stack.push({{table.get_axiom()}, dummy.get()});

  auto token = scanner.NextToken();

  do {
    const auto [symbol, parent] = stack.top();
    switch (symbol.get_type()) {
      case ast::Symbol::Type::kTerminal: {
        if (symbol.get_name() != lexer::ToString(token->get_tag())) {
          ThrowParseError(*token, symbol.get_name());
        }

        stack.pop();
        parent->AddChild(std::make_unique<dt::LeafNode>(std::move(token)));
        token = scanner.NextToken();
        break;
      }
      case ast::Symbol::Type::kNonterminal: {
        const auto terminal = ast::Symbol{lexer::ToString(token->get_tag()),
                                          ast::Symbol::Type::kTerminal};
        const auto opt = table.Find(symbol, terminal);
        if (!opt.has_value()) {
          ThrowParseError(*token, symbol.get_name());
        }
        const auto [b, e] = opt.value();

        stack.pop();
        auto& child = static_cast<dt::InnerNode&>(
            parent->AddChild(std::make_unique<dt::InnerNode>()));
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
