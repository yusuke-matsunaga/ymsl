
/// @file libym_verilog/scanner/InputMgr.cc
/// @brief InputMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: InputMgr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "InputMgr.h"
#include <fcntl.h>

#include "ym_utils/FileInfo.h"


// ファイル末尾に改行がなくても warning としない時に 1
#define ALLOW_EOF_WITHOUT_NL 1


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// InputMgr のパブリックなメンバ関数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex
InputMgr::InputMgr(RawLex* lex) :
  mLex(lex),
  mCurFile(NULL)
{
}

// @brief デストラクタ
InputMgr::~InputMgr()
{
  clear();
}

// @brief 初期状態に戻す．
// 読み込みの途中でこれを呼ぶと大変なことになる．
// 普通は正常に読み終わったあとか, エラーの後で呼ぶ．
void
InputMgr::clear()
{
  delete mCurFile;
  mCurFile = NULL;
  for (vector<InputFile*>::iterator p = mFileStack.begin();
       p != mFileStack.end(); ++ p) {
    delete *p;
  }
  mFileStack.clear();
}


//////////////////////////////////////////////////////////////////////
// サーチパスとファイル関係
//////////////////////////////////////////////////////////////////////

// @brief サーチパスリストを設定する．
// @param[in] searchpath セットするサーチパス
void
InputMgr::set_searchpath(const SearchPathList& searchpath)
{
  mSearchPathList = searchpath;
}

// @brief 設定されているサーチパスを考慮して filename を探す．
// @param[in] filename ファイル名
// @return 実際の絶対パス
PathName
InputMgr::search_file(const string& filename)
{
  // filename が存在して読めなければならない．
  return mSearchPathList.search(PathName(filename));
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @param[in] parent_file インクルード元のファイル情報
// @retval true オープンに成功した．
// @retval false ファイルが開けなかった
// 設定されているサーチパスを考慮して filename を探す．
bool
InputMgr::open_file(const string& filename,
		    const FileLoc& parent_file)
{
  PathName pathname = search_file(filename);
  if ( !pathname.is_valid() ) {
    return false;
  }
  // 本当のパス名
  string realname_string = pathname.str();
  const char* realname = realname_string.c_str();

  int fd = open(realname, O_RDONLY);
  if ( fd < 0 ) {
    return false;
  }
  FileInfo file_info = FileInfo(realname, parent_file);
  InputFile* new_file = new InputFile(mLex, fd, file_info);
  if ( mCurFile ) {
    mFileStack.push_back(mCurFile);
  }
  mCurFile = new_file;

  return true;
}

// @brief 現在のファイル位置を強制的に書き換える．
// @param[in] new_filename 新しいファイル名
// @param[in] line     新しい行番号
// @param[in] level
//           - 0 インクルード関係のレベル変化無し
//           - 1 新しいファイルはインクルードされたファイル
//           - 2 新しいファイルはインクルードもとのファイル
void
InputMgr::set_file_loc(const char* new_filename,
		       ymuint line,
		       ymuint level)
{
  if ( mCurFile == NULL ) {
    // ないと思うけど念のため
    return;
  }

  FileInfo cur_fi = mCurFile->mFileInfo;
  switch ( level ) {
  case 0: // レベルの変化無し
    if ( cur_fi.filename() != new_filename ) {
      // 新しい FileInfo を作る．
      FileLoc flp = cur_fi.parent_loc();
      cur_fi = FileInfo(new_filename, flp);
    }
    break;

  case 1: // 新しいインクルードファイル．
    {
      FileLoc parent_loc(cur_fi, cur_file()->cur_line(), 1);
      cur_fi = FileInfo(new_filename, parent_loc);
    }
    break;

  case 2: // インクルードの終り
    cur_fi = cur_fi.parent_loc().file_info();
    if ( cur_fi.filename() != new_filename ) {
      // 新しい FileInfo を作る．
      FileLoc flp = cur_fi.parent_loc();
      cur_fi = FileInfo(new_filename, flp);
    }
    break;
  }
  mCurFile->mFileInfo = cur_fi;
}

// @brief 現在のファイルを返す．
InputFile*
InputMgr::cur_file() const
{
  return mCurFile;
}

// @brief 現在のファイル名を返す．
string
InputMgr::cur_filename() const
{
  return mCurFile->file_info().filename();
}

// @brief 現在の InputFile が EOF を返したときの処理
// @return 処理を続けられる時 true を返す．
bool
InputMgr::wrap_up()
{
  for ( ; ; ) {
    mCurFile->close();
    if ( mFileStack.empty() ) {
      // もうファイルが残っていない．
      return false;
    }
    delete mCurFile;
    mCurFile = mFileStack.back();
    mFileStack.pop_back();

    int c = mCurFile->peek();
    if ( c != EOF ) {
      return true;
    }
  }
}

// @brief ファイルのオープン済チェック
// @param[in] name チェックするファイル名
// @retval true name という名のファイルがオープンされている．
// @retval false name というなのファイルはオープンされていない．
bool
InputMgr::check_file(const char* name) const
{
  if ( cur_filename() == name ) {
    return true;
  }
  for (vector<InputFile*>::const_iterator p = mFileStack.begin();
       p != mFileStack.end(); ++ p) {
    if ( (*p)->mFileInfo.filename() == name ) {
      return true;
    }
  }
  return false;
}

END_NAMESPACE_YM_VERILOG
