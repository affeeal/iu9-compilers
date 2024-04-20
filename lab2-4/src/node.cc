#include "node.h"

#include <boost/json/object.hpp>

#include "token.h"

namespace parser {

namespace ast {

boost::json::value Program::ToJson() const {
  auto program = boost::json::object{};

  auto& funcs = (program["funcs"] = boost::json::array{}).as_array();
  funcs.reserve(funcs_.size());

  for (auto&& func : funcs_) {
    funcs.push_back(func->ToJson());
  }

  return program;
}

boost::json::value Func::ToJson() const {
  return {
      {"ident_code", ident_code_},
      {"type", type_->ToJson()},
      {"body", body_->ToJson()},
  };
}

boost::json::value FuncType::ToJson() const {
  return {
      {"input", input_->ToJson()},
      {"output", output_->ToJson()},
  };
}

boost::json::value ElementaryType::ToJson() const {
  return {
      {"discriminator_type", "elementary_type"},
      {"tag", lexer::ToString(tag_)},
  };
}

boost::json::value ListType::ToJson() const {
  return {
      {"discriminator_type", "list_type"},
      {"type", type_->ToJson()},
  };
}

boost::json::value TupleType::ToJson() const {
  auto tuple = boost::json::object{};
  tuple["discriminator_type"] = "tuple_type";

  auto& types = (tuple["types"] = boost::json::array{}).as_array();
  types.reserve(types_.size());

  for (auto&& type : types_) {
    types.push_back(type->ToJson());
  }

  return tuple;
}

boost::json::value FuncBody::ToJson() const {
  auto func_body = boost::json::object{};

  auto& stmts = (func_body["stmts"] = boost::json::array{}).as_array();
  stmts.reserve(stmts_.size());

  for (auto&& stmt : stmts_) {
    stmts.push_back(stmt->ToJson());
  }

  return func_body;
}

boost::json::value Statement::ToJson() const {
  return {
      {"pattern", pattern_->ToJson()},
      {"result", nullptr},
  };
}

boost::json::value PatternBinary::ToJson() const {
  return {
      {"discriminator_type", "pattern_binary"},
      {"lhs", lhs_->ToJson()},
      {"rhs", rhs_->ToJson()},
      {"op", lexer::ToString(op_)},
  };
}

boost::json::value PatternList::ToJson() const {
  auto pattern_list = boost::json::object{};
  pattern_list["discriminator_type"] = "pattern_list";

  auto& patterns = (pattern_list["patterns"] = boost::json::array{}).as_array();
  patterns.reserve(patterns_.size());

  for (auto&& pattern : patterns_) {
    patterns.push_back(pattern->ToJson());
  }

  return pattern_list;
}

boost::json::value PatternTuple::ToJson() const {
  auto pattern_tuple = boost::json::object{};
  pattern_tuple["discriminator_type"] = "pattern_tuple";

  auto& patterns =
      (pattern_tuple["patterns"] = boost::json::array{}).as_array();
  patterns.reserve(patterns_.size());

  for (auto&& pattern : patterns_) {
    patterns.push_back(pattern->ToJson());
  }

  return pattern_tuple;
}

boost::json::value Var::ToJson() const {
  return {
      {"discriminator_type", "var"},
      {"ident_code", ident_code_},
  };
}

boost::json::value IntConst::ToJson() const {
  return {
      {"discriminator_type", "int_const"},
      {"value", value_},
  };
}

boost::json::value ResultBinary::ToJson() const {
  return {
      {"discriminator_type", "result_binary"},
      {"lhs", lhs_->ToJson()},
      {"rhs", rhs_->ToJson()},
      {"op", lexer::ToString(op_)},
  };
}

boost::json::value ResultList::ToJson() const {
  auto result_list = boost::json::object{};
  result_list["discriminator_type"] = "result_list";

  auto& results = (result_list["results"] = boost::json::array{}).as_array();
  results.reserve(results_.size());

  for (auto&& result : results_) {
    results.push_back(result->ToJson());
  }

  return result_list;
}

boost::json::value ResultTuple::ToJson() const {
  auto result_tuple = boost::json::object{};
  result_tuple["discriminator_type"] = "result_tuple";

  auto& results = (result_tuple["results"] = boost::json::array{}).as_array();
  results.reserve(results_.size());

  for (auto&& result : results_) {
    results.push_back(result->ToJson());
  }

  return result_tuple;
}

boost::json::value FuncCall::ToJson() const {
  return {
      {"arg", arg_->ToJson()},
      {"ident_code", ident_code_},
  };
}

}  // namespace ast

}  // namespace parser
