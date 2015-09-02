// vim: set ts=2 sw=2 tw=99 et:

#ifndef wasm_ast_h__
#define wasm_ast_h__

#include "memory.h"
#include "list.h"

#include <iosfwd>

namespace wasm_ast {

struct Node : memory::Base, ListElem<Node>
{

};

struct Module : Node
{
};

std::ostream&
operator<<(std::ostream&, const Module&);

}  // namespace wasm_ast
#endif // defined wasm_ast_h__
