// vim: set ts=2 sw=2 tw=99 et:

#include "parser.h"
#include "emitter.h"

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>

using namespace std;

int
main(int argc, char** argv)
try
{
  cin.exceptions(ios::failbit | ios::badbit);
  cout.exceptions(ios::failbit | ios::badbit);

  string chars;
  getline(cin, chars, '\0');

  memory::Pool pool;
  const asmjs_ast::Func& asmjs = parser::parse_asmjs(pool, chars);
  const wasm_ast::Module& wasm = emitter::emit_wasm(pool, asmjs);

  cout << wasm;

  return 0;
}
catch(const ios::failure& err)
{
  cerr << "Failed with runtime error: " << err.what() << endl;
  return -1;
}
catch(const runtime_error& err)
{
  cerr << "Failed with runtime error: " << err.what() << endl;
  return -1;
}

