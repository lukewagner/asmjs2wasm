// vim: set ts=2 sw=2 tw=99 et:

#ifndef asmjs_ast_h__
#define asmjs_ast_h__

#include "cashew/parser.h"
#include "memory.h"
#include "list.h"

#include <vector>

namespace asmjs_ast {

using cashew::IString;

enum class NodeKind
{
  Double, Int, String, Array, Object,
  Name, Prefix, Binary, Conditional, Call, New, Dot, Index,
  Var, Block, Return, If, While, Do, Label, Break, Continue, Switch,
  Func,
};

struct Node : memory::Base, ListElem<Node>
{
  Node(NodeKind kind) : kind(kind) {}

  NodeKind kind;

  template <class T> bool is() const { return kind == T::Kind; }
  template <class T> T& as() { assert(is<T>()); return *static_cast<T*>(this); }
  template <class T> const T& as() const  { assert(is<T>()); return *static_cast<const T*>(this); }
};

struct Double : Node
{
  static const NodeKind Kind = NodeKind::Double;
  Double(double f64) : Node(Kind), f64(f64) {}

  const double f64;
};

struct Int : Node
{
  static const NodeKind Kind = NodeKind::Int;
  Int(uint32_t u32) : Node(Kind), u32(u32) {}

  const uint32_t u32;
};

struct String : Node
{
  static const NodeKind Kind = NodeKind::String;
  String(IString str) : Node(Kind), str(str) {}

  const IString str;
};

struct Array : Node
{
  static const NodeKind Kind = NodeKind::Array;
  Array() : Node(Kind) {}

  List<Node> elems;
};

struct Field : memory::Base, ListElem<Field>
{
  Field(IString key, Node& value) : key(key), value(value) {}

  const IString key;
  Node& value;
};

struct Object : Node
{
  static const NodeKind Kind = NodeKind::Object;
  Object() : Node(Kind) {}

  List<Field> fields;
};

struct Name : Node
{
  static const NodeKind Kind = NodeKind::Name;
  Name(IString str) : Node(Kind), str(str) {}

  uint32_t index;
  const IString str;
};

struct Prefix : Node
{
  static const NodeKind Kind = NodeKind::Prefix;
  Prefix(IString op, Node& kid) : Node(Kind), op(op), kid(kid) {}

  const IString op;
  Node& kid;
};

struct Binary : Node
{
  static const NodeKind Kind = NodeKind::Binary;
  Binary(IString op, Node& lhs, Node& rhs) : Node(Kind), op(op), lhs(lhs), rhs(rhs) {}

  const IString op;
  Node& lhs;
  Node& rhs;
};

struct Conditional : Node
{
  static const NodeKind Kind = NodeKind::Conditional;
  Conditional(Node& cond, Node& lhs, Node& rhs) : Node(Kind), cond(cond), lhs(lhs), rhs(rhs) {}

  Node& cond;
  Node& lhs;
  Node& rhs;
};

struct Call : Node
{
  static const NodeKind Kind = NodeKind::Call;
  Call(Node& callee) : Node(Kind), callee(callee) {}

  Node& callee;
  List<Node> args;
};

struct New : Node
{
  static const NodeKind Kind = NodeKind::New;
  New(Call& call) : Node(Kind), call(call) {}

  Call& call;
};

struct Dot : Node
{
  static const NodeKind Kind = NodeKind::Dot;
  Dot(Node& base, IString name) : Node(Kind), base(base), name(name) {}

  Node& base;
  IString name;
};

struct Index : Node
{
  static const NodeKind Kind = NodeKind::Index;
  Index(Node& array, Node& index) : Node(Kind), array(array), index(index) {}

  Node& array;
  Node& index;
};

struct VarDecl : memory::Base, ListElem<VarDecl>
{
  VarDecl(IString name, Node& init) : name(name), init(init) {}

  const IString name;
  Node& init;
};

struct Var : Node
{
  static const NodeKind Kind = NodeKind::Var;
  Var() : Node(Kind) {}

  List<VarDecl> decls;
};

struct Block : Node
{
  static const NodeKind Kind = NodeKind::Block;
  Block() : Node(Kind) {}
  Block(Node& a) : Block() { stmts.append(a); }
  Block(Node& a, Node& b) : Block() { stmts.append(a); stmts.append(b); }

  List<Node> stmts;
};

struct Return : Node
{
  static const NodeKind Kind = NodeKind::Return;
  Return(Node* expr) : Node(Kind), expr(expr) {}

  Node* const expr;
};

struct If : Node
{
  static const NodeKind Kind = NodeKind::If;
  If(Node& cond, Node& if_true, Node* if_false) : Node(Kind), cond(cond), if_true(if_true), if_false(if_false) {}

  Node& cond;
  Node& if_true;
  Node* const if_false;
};

struct While : Node
{
  static const NodeKind Kind = NodeKind::While;
  While(Node& cond, Node& body) : Node(Kind), cond(cond), body(body) {}

  Node& cond;
  Node& body;
};

struct Do : Node
{
  static const NodeKind Kind = NodeKind::Do;
  Do(Node& body, Node& cond) : Node(Kind), body(body), cond(cond) {}

  Node& body;
  Node& cond;
};

struct Label : Node
{
  static const NodeKind Kind = NodeKind::Label;
  Label(IString str, Node& stmt) : Node(Kind), str(str), stmt(stmt) {}

  const IString str;
  Node& stmt;
};

struct Break : Node
{
  static const NodeKind Kind = NodeKind::Break;
  Break(IString str) : Node(Kind), str(str) {}

  const IString str;
};

struct Continue : Node
{
  static const NodeKind Kind = NodeKind::Continue;
  Continue(IString str) : Node(Kind), str(str) {}

  const IString str;
};

struct Case : memory::Base, ListElem<Case>
{
  Case() : label(nullptr) { /* default */ }
  Case(Node& label) : label(&label) {}

  Node* const label;
  List<Node> stmts;
};

struct Switch : Node
{
  static const NodeKind Kind = NodeKind::Switch;
  Switch(Node& expr) : Node(Kind), expr(expr) {}

  Node& expr;
  List<Case> cases;
};

struct Param : memory::Base, ListElem<Param>
{
  Param(IString name) : name(name) {}

  IString const name;
};

struct Func : Node
{
  static const NodeKind Kind = NodeKind::Func;
  Func(IString name, Block& body) : Node(Kind), name(name), body(body) {}

  IString const name;
  List<Param> params;
  Block& body;
};

}  // namespace asmjs_ast
#endif  // defined asmjs_ast_h__
