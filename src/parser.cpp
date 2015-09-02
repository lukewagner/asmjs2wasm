// vim: set ts=2 sw=2 tw=99 et:

#include "parser.h"

#include <cstdint>

using namespace asmjs_ast;
using namespace std;

class Builder
{
  memory::Pool& pool_;

public:
  Builder(memory::Pool& pool) : pool_(pool) {}

  // Literals

  Node* makeDouble(double f64)
  {
    return new(pool_) Double(f64);
  }
  Node* makeInt(uint32_t u32)
  {
    return new(pool_) Int(u32);
  }
  Node* makeString(IString str)
  {
    return new(pool_) String(str);
  }
  Node* makeArray()
  {
    return new(pool_) Array();
  }
  void appendToArray(Node* array, Node* elem)
  {
    array->as<Array>().elems.append(*elem);
  }
  Node* makeObject()
  {
    return new(pool_) Object();
  }
  void appendToObject(Node* object, IString key, Node* value)
  {
    object->as<Object>().fields.append(*new(pool_) Field(key, *value));
  }

  // Expressions

  Node* makeName(IString str)
  {
    return new(pool_) Name(str);
  }
  Node* makePrefix(IString op, Node* expr)
  {
    return new(pool_) Prefix(op, *expr);
  }
  Node* makeBinary(Node* lhs, IString op, Node* rhs)
  {
    return new(pool_) Binary(op, *lhs, *rhs);
  }
  Node* makeConditional(Node* cond, Node* lhs, Node* rhs)
  {
    return new(pool_) Conditional(*cond, *lhs, *rhs);
  }
  Node* makeCall(Node* callee)
  {
    return new(pool_) Call(*callee);
  }
  void appendToCall(Node* call, Node* arg)
  {
    call->as<Call>().args.append(*arg);
  }
  Node* makeNew(Node* call)
  {
    return new(pool_) New(call->as<Call>());
  }
  Node* makeDot(Node* base, IString name)
  {
    return new(pool_) Dot(*base, name);
  }
  Node* makeDot(Node* base, Node* name)
  {
    return new(pool_) Dot(*base, name->as<Name>().str);
  }
  Node* makeIndexing(Node* array, Node* index)
  {
    return new(pool_) Index(*array, *index);
  }

  // Statements

  Node* makeStatement(Node* expr)
  {
    return expr;
  }
  Node* makeVar(bool is_const)
  {
    return new(pool_) Var();
  }
  void appendToVar(Node* var, IString name, Node* init)
  {
    var->as<Var>().decls.append(*new(pool_) VarDecl(name, *init));
  }
  Node* makeBlock()
  {
    return new(pool_) Block();
  }
  void appendToBlock(Node* block, Node* stmt)
  {
    (block->is<Func>() ? block->as<Func>().body : block->as<Block>()).stmts.append(*stmt);
  }
  Node* makeReturn(Node* expr)
  {
    return new(pool_) Return(expr);
  }
  Node* makeIf(Node* cond, Node* if_true, Node* if_false)
  {
    return new(pool_) If(*cond, *if_true, if_false);
  }
  Node* makeWhile(Node* cond, Node* body)
  {
    return new(pool_) While(*cond, *body);
  }
  Node* makeDo(Node* body, Node* cond)
  {
    return new(pool_) Do(*body, *cond);
  }
  Node* makeLabel(IString str, Node* stmt)
  {
    return new(pool_) Label(str, *stmt);
  }
  Node* makeBreak(IString str)
  {
    return new(pool_) Break(str);
  }
  Node* makeContinue(IString str)
  {
    return new(pool_) Continue(str);
  }
  Node* makeSwitch(Node* expr)
  {
    return new(pool_) Switch(*expr);
  }
  void appendCaseToSwitch(Node* sw, Node* label)
  {
    sw->as<Switch>().cases.append(*new(pool_) Case(*label));
  }
  void appendDefaultToSwitch(Node* sw)
  {
    sw->as<Switch>().cases.append(*new(pool_) Case());
  }
  void appendCodeToSwitch(Node* sw, Node* stmt, bool)
  {
    sw->as<Switch>().cases.last().stmts.append(*stmt);
  }

  // Function

  Node* makeFunction(IString name)
  {
    return new(pool_) Func(name, *new(pool_)Block());
  }
  void appendArgumentToFunction(Node* fun, IString name)
  {
    fun->as<Func>().params.append(*new(pool_) Param(name));
  }

  Node* makeToplevel()
  {
    return makeBlock();
  }
};

Func&
parser::parse_asmjs(memory::Pool& pool, const string& chars)
{
  // cashew needs to be able to mutate the chars
  vector<char> mutable_chars(chars.begin(), chars.end());

  cashew::Parser<Node*, Builder> parser(pool);
  Node* top_level = parser.parseToplevel(mutable_chars.data());

  List<Node>& top = top_level->as<Block>().stmts;
  if (top.compute_length() != 1 || !top.first().is<Func>())
    throw runtime_error("expecting single top-level asm.js module function");

  Func& func = top.first().as<Func>();
  List<Node>& body = func.body.stmts;
  if (body.empty() || !body.first().is<String>() || !body.first().as<String>().str.equals("use asm"))
    throw runtime_error("expecting single top-level asm.js module function");

  return func;
}


