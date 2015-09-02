// vim: set ts=2 sw=2 tw=99 et:

#ifndef emitter_h__
#define emitter_h__

#include "asmjs_ast.h"
#include "wasm_ast.h"

namespace emitter {

wasm_ast::Module&
emit_wasm(memory::Pool& pool, const asmjs_ast::Func& func);

}  // namespace emitter
#endif  // defined emitter_h__
