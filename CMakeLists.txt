# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 3.2)


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================
project (Ymsl)
set (YMSL_VERSION_MAJOR 1)
set (YMSL_VERSION_MINOR 0)
set (YMSL_VERSION "${YMSL_VERSION_MAJOR}.${YMSL_VERSION_MINOR}")


# ===================================================================
# CTest モジュールの読み込み
# ===================================================================
include (CTest)


# ===================================================================
# パッケージの検査
# ===================================================================

include (FindDoxygen)

find_package(BISON REQUIRED)

find_package (GTest)

find_package (YmTools REQUIRED)


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}
  ${YmTools_INCLUDE_DIRS}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (ymsl_SOURCES
  src/parser/RsrvWordDic.cc
  src/parser/YmslScanner.cc

  ${CMAKE_CURRENT_BINARY_DIR}/grammer.cc

  src/ast/Ast.cc
  src/ast/AstMgr.cc
  src/ast/AstPrinter.cc
  src/ast/AstSymbol.cc

  src/ast/expr/AstArrayRef.cc
  src/ast/expr/AstBinOp.cc
  src/ast/expr/AstExpr.cc
  src/ast/expr/AstFalse.cc
  src/ast/expr/AstFloatConst.cc
  src/ast/expr/AstFuncCall.cc
  src/ast/expr/AstIntConst.cc
  src/ast/expr/AstIteOp.cc
  src/ast/expr/AstMemberRef.cc
  src/ast/expr/AstOp.cc
  src/ast/expr/AstStringConst.cc
  src/ast/expr/AstSymbolExpr.cc
  src/ast/expr/AstTrue.cc
  src/ast/expr/AstUniOp.cc

  src/ast/stmt/AstAssignment.cc
  src/ast/stmt/AstBlockStmt.cc
  src/ast/stmt/AstBreak.cc
  src/ast/stmt/AstCaseItem.cc
  src/ast/stmt/AstConstDecl.cc
  src/ast/stmt/AstContinue.cc
  src/ast/stmt/AstDecr.cc
  src/ast/stmt/AstDoWhile.cc
  src/ast/stmt/AstEnumConst.cc
  src/ast/stmt/AstEnumDecl.cc
  src/ast/stmt/AstExprStmt.cc
  src/ast/stmt/AstFor.cc
  src/ast/stmt/AstFuncDecl.cc
  src/ast/stmt/AstGoto.cc
  src/ast/stmt/AstIf.cc
  src/ast/stmt/AstImport.cc
  src/ast/stmt/AstIncr.cc
  src/ast/stmt/AstInplaceOp.cc
  src/ast/stmt/AstLabel.cc
  src/ast/stmt/AstNullStmt.cc
  src/ast/stmt/AstParam.cc
  src/ast/stmt/AstReturn.cc
  src/ast/stmt/AstStatement.cc
  src/ast/stmt/AstSwitch.cc
  src/ast/stmt/AstToplevel.cc
  src/ast/stmt/AstVarDecl.cc
  src/ast/stmt/AstWhile.cc

  src/ast/type/AstType.cc

  src/compiler/YmslCompiler.cc
  src/compiler/ArrayType.cc
  src/compiler/EnumType.cc
  src/compiler/FuncType.cc
  src/compiler/MapType.cc
  src/compiler/NamedType.cc
  src/compiler/PrimType.cc
  src/compiler/Scope.cc
  src/compiler/SetType.cc
  src/compiler/Type.cc
  src/compiler/TypeMgr.cc

  src/ir/IrCodeBlock.cc
  src/ir/IrFuncBlock.cc
  src/ir/IrToplevel.cc
  src/ir/IrMgr.cc
  src/ir/IrMgr_expr.cc
  src/ir/IrMgr_handle.cc
  src/ir/IrMgr_node.cc
  src/ir/IrMgr_stmt.cc
  src/ir/IrInterp.cc
  src/ir/IrPrinter.cc

  src/ir/handle/IrHandle.cc
  src/ir/handle/IrBooleanConst.cc
  src/ir/handle/IrIndexHandle.cc
  src/ir/handle/IrIntConst.cc
  src/ir/handle/IrFloatConst.cc
  src/ir/handle/IrStringConst.cc
  src/ir/handle/IrFuncHandle.cc
  src/ir/handle/IrLabelHandle.cc
  src/ir/handle/IrScopeHandle.cc
  src/ir/handle/IrTypeHandle.cc
  src/ir/handle/IrLocalVarHandle.cc
  src/ir/handle/IrGlobalVarHandle.cc
  src/ir/handle/IrArrayRef.cc
  src/ir/handle/IrMemberRef.cc

  src/ir/node/IrNode.cc
  src/ir/node/IrLoad.cc
  src/ir/node/IrStore.cc
  src/ir/node/IrOp.cc
  src/ir/node/IrUniOp.cc
  src/ir/node/IrBinOp.cc
  src/ir/node/IrTriOp.cc
  src/ir/node/IrInplaceOp.cc
  src/ir/node/IrInplaceUniOp.cc
  src/ir/node/IrInplaceBinOp.cc
  src/ir/node/IrFuncCall.cc
  src/ir/node/IrReturn.cc
  src/ir/node/IrJump.cc
  src/ir/node/IrLabel.cc

  src/vsm/Vsm.cc
  src/vsm/VsmBuiltinFunc.cc
  src/vsm/VsmCodeList.cc
  src/vsm/VsmGen.cc
  src/vsm/VsmFunction.cc
  src/vsm/VsmNativeFunc.cc
  src/vsm/VsmModule.cc
  src/vsm/VsmNativeModule.cc
  src/vsm/VsmVar.cc

  src/builtin/YmslPrint.cc
  )


# Create target for the parser
add_custom_target ( grammer ALL
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/grammer.cc"
  )

# Create custom command for bison/yacc (note the DEPENDS)
add_custom_command (
  COMMAND "${BISON_EXECUTABLE}"
  -y "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/grammer.yy"
  -o "${CMAKE_CURRENT_BINARY_DIR}/grammer.cc"
  -d -v
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/grammer.cc"
  "${CMAKE_CURRENT_BINARY_DIR}/grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/grammer.output"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/grammer.yy"
  )

# Since verilog_grammer.cc does not exists yet when cmake is run, mark
# it as generated
set_source_files_properties ( ${CMAKE_CURRENT_BINARY_DIR}/grammer.cc GENERATED )


# ===================================================================
#  ターゲットの設定
# ===================================================================

add_library(ymsl
  ${ymsl_SOURCES}
  )

target_link_libraries(ymsl
  ${Readline_LIBRARY}
  ym_utils
  )

add_executable(scanner_test
  tests/scanner_test.cc
  )

target_link_libraries(scanner_test
  ymsl
  )

add_executable(parser_test
  tests/parser_test.cc
  )

target_link_libraries(parser_test
  ymsl
  )

add_executable(TypeMgr_test
  tests/TypeMgr_test.cc
  )

target_link_libraries(TypeMgr_test
  ymsl
  )

add_executable(IrMgr_test
  tests/IrMgr_test.cc
  )

target_link_libraries(IrMgr_test
  ymsl
  )
