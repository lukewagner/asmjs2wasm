// vim: set ts=2 sw=2 tw=99 et:

#include "emitter.h"

using namespace asmjs_ast;
using namespace wasm_ast;

wasm_ast::Module&
emitter::emit_wasm(memory::Pool& pool, const asmjs_ast::Func& func)
{
  return *new(pool) Module;
}

