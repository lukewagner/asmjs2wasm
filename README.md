# asmjs2wasm

This repository contains the source for `asmjs2wasm`, a command-line tool that
converts asm.js to the WebAssembly text format.

**This tool is currently in development and incomplete**

Current limitations of the tool:
* The tool only accepts files containing a single top-level asm.js module
  function definition. To apply this tool to an arbitrary JS file containing
  an asm.js module, all the surrounding asm.js must be stripped out.
* The tool assumes that the incoming asm.js has already been validated and
  may crash or generate wrong code otherwise.
* Until the WebAssembly text format is finalized, the tool emits the
  temporary S-Expression-based text format defined by
  [WebAssembly/spec](https://github.com/WebAssembly/spec).

## Build
```
make
```
(Patches welcome to do something better.)

## Run unit tets
```
make test
```

## Usage
```
cat asmjs_file.js | asmjs2wasm > wasm_file.wasm
```
