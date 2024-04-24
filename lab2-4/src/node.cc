#include "node.h"

#include "token.h"

namespace parser {

namespace ast {

static constexpr std::string_view kDiscriminatorType = "discriminator_type";

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
      {kDiscriminatorType, "elementary_type"},
      {"tag", lexer::ToString(tag_)},
  };
}

boost::json::value ListType::ToJson() const {
  return {
      {kDiscriminatorType, "list_type"},
      {"type", type_->ToJson()},
  };
}

boost::json::value TupleType::ToJson() const {
  auto tuple = boost::json::object{};
  tuple[kDiscriminatorType] = "tuple_type";

  auto& types = (tuple["types"] = boost::json::array{}).as_array();
  types.reserve(types_.size());

  for (auto&& type : types_) {
    types.push_back(type->ToJson());
  }

  return tuple;
}

boost::json::value FuncBody::ToJson() const {
  auto func_body = boost::json::object{};

  auto& sents = (func_body["sents"] = boost::json::array{}).as_array();
  sents.reserve(sents_.size());

  for (auto&& sent : sents_) {
    sents.push_back(sent->ToJson());
  }

  return func_body;
}

boost::json::value Sentence::ToJson() const {
  return {
      {"pattern", pattern_->ToJson()},
      {"result", result_->ToJson()},
  };
}

boost::json::value PatternBinary::ToJson() const {
  return {
      {kDiscriminatorType, "pattern_binary"},
      {"op", lexer::ToString(op_)},
      {"lhs", lhs_->ToJson()},
      {"rhs", rhs_->ToJson()},
  };
}

boost::json::value PatternTuple::ToJson() const {
  auto pattern_tuple = boost::json::object{};
  pattern_tuple[kDiscriminatorType] = "pattern_tuple";

  auto& patterns =
      (pattern_tuple["patterns"] = boost::json::array{}).as_array();
  patterns.reserve(patterns_.size());

  for (auto&& pattern : patterns_) {
    patterns.push_back(pattern->ToJson());
  }

  return pattern_tuple;
}

boost::json::value EmptyList::ToJson() const {
  return {
      {kDiscriminatorType, "empty_list"},
  };
}

boost::json::value Var::ToJson() const {
  return {
      {kDiscriminatorType, "var"},
      {"ident_code", ident_code_},
  };
}

template <>
boost::json::value Const<std::int64_t>::ToJson() const {
  return {
      {kDiscriminatorType, "int_const"},
      {"value", value_},
  };
}

boost::json::value ResultBinary::ToJson() const {
  return {
      {kDiscriminatorType, "result_binary"},
      {"op", lexer::ToString(op_)},
      {"lhs", lhs_->ToJson()},
      {"rhs", rhs_->ToJson()},
  };
}

boost::json::value ResultTuple::ToJson() const {
  auto result_tuple = boost::json::object{};
  result_tuple[kDiscriminatorType] = "result_tuple";

  auto& results = (result_tuple["results"] = boost::json::array{}).as_array();
  results.reserve(results_.size());

  for (auto&& result : results_) {
    results.push_back(result->ToJson());
  }

  return result_tuple;
}

boost::json::value FuncCall::ToJson() const {
  return {
      {kDiscriminatorType, "func_call"},
      {"ident_code", ident_code_},
      {"arg", arg_->ToJson()},
  };
}

}  // namespace ast

}  // namespace parser
