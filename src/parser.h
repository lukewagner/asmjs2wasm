// vim: set ts=2 sw=2 tw=99 et:

#ifndef parser_h__
#define parser_h__

#include "asmjs_ast.h"

#include <string>

namespace parser {

asmjs_ast::Func&
parse_asmjs(memory::Pool& pool, const std::string& chars);

}  // namespace parser
#endif  // defined parser_h__
