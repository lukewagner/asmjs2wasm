// vim: set ts=2 sw=2 tw=99 et:

#include "wasm_ast.h"

#include <ostream>

using namespace wasm_ast;
using namespace std;

ostream&
wasm_ast::operator<<(ostream& os, const Module& m)
{
  return os << "(module)\n";
}
