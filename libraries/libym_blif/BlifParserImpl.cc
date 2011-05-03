
/// @file libym_blif/BlifParserImpl.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifParserImpl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BlifParserImpl.h"
#include "ym_blif/BlifHandler.h"
#include "ym_utils/FileInfoMgr.h"


BEGIN_NAMESPACE_YM_BLIF

BEGIN_NONAMESPACE

// 文字列を 0/1 に変換する
// それ以外の文字なら -1 を返す．
int
str_to_01(const StrBuff& str)
{
  const char* strptr = str.c_str();
  if ( strptr[1] != '\0' ) {
    return -1;
  }
  if ( strptr[0] == '0' ) {
    return 0;
  }
  if ( strptr[0] == '1' ) {
    return 1;
  }
  return -1;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// BlifParserImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BlifParserImpl::BlifParserImpl()
{
}

// デストラクタ
BlifParserImpl::~BlifParserImpl()
{
  for (list<BlifHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    BlifHandler* handler = *p;
    delete handler;
  }
}

// 読み込みを行なう．
bool
BlifParserImpl::read(const string& filename)
{
  // ファイルをオープンする．
  if ( !mScanner.open_file(filename) ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    mMsgMgr.put_msg(__FILE__, __LINE__, FileRegion(),
		    kMsgFailure, "BLIF_PARSER", buf.str());
    return false;
  }

  // 初期化を行う．
  mIdHash.clear();
  mUngetToken = kTokenEOF;

  // 一つの .inputs/.outputs 文中のトークンの数
  ymuint n_token = 0;

  bool stat = true;
  for (list<BlifHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    BlifHandler* handler = *p;
    if ( !handler->init() ) {
      stat = false;
    }
  }
  if ( !stat ) {
    goto ST_ERROR_EXIT;
  }

 ST_INIT:
  // 読込開始
  {
    tToken tk= get_token();
    if ( tk == kTokenNL ) {
      // 読み飛ばす
      goto ST_INIT;
    }
    if ( tk == kTokenMODEL ) {
      // .model 文の開始
      mLoc1 = mCurLoc;
      goto ST_MODEL;
    }
    // それ以外はエラー
    mMsgMgr.put_msg(__FILE__, __LINE__,
		    mCurLoc,
		    kMsgError,
		    "SYN01",
		    "No '.model' statement.");
    goto ST_ERROR_EXIT;
  }

 ST_MODEL:
  // .model 文の処理
  {
    tToken tk= get_token();
    if ( tk != kTokenSTRING ) {
      mMsgMgr.put_msg(__FILE__, __LINE__,
		      mCurLoc,
		      kMsgError,
		      "SYN02",
		      "String expected after '.model'.");
      goto ST_ERROR_EXIT;
    }
    FileRegion loc = mCurLoc;
    const char* name = mScanner.cur_string().c_str();
    for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      BlifHandler* handler = *p;
      if ( !handler->model(mLoc1, loc, name) ) {
	stat = false;
      }
    }
    if ( !stat ) {
      goto ST_ERROR_EXIT;
    }

    if ( get_token() != kTokenNL ) {
      mMsgMgr.put_msg(__FILE__, __LINE__,
		      mCurLoc,
		      kMsgError,
		      "SYN03",
		      "Newline expected.");
      goto ST_ERROR_EXIT;
    }

    goto ST_NEUTRAL;
  }

 ST_NEUTRAL:
  // 本体の処理
  {
    tToken tk= get_token();
    mLoc1 = mCurLoc;
    switch (tk) {
    case kTokenNL:
      goto ST_NEUTRAL;

    case kTokenEOF:
      goto ST_AFTER_EOF;

    case kTokenMODEL:
      mMsgMgr.put_msg(__FILE__, __LINE__,
		      mLoc1,
		      kMsgError,
		      "SYN04",
		      "Multiple '.model' statements.");
      goto ST_ERROR_EXIT;

    case kTokenINPUTS:
      n_token = 0;
      goto ST_INPUTS;

    case kTokenOUTPUTS:
      n_token = 0;
      goto ST_OUTPUTS;

    case kTokenNAMES:
      mNameArray.clear();
      mIdArray.clear();
      mNc = 0;
      mCoverPat.clear();
      mOpat = ' ';
      goto ST_NAMES;

    case kTokenGATE:
      goto ST_GATE;

    case kTokenLATCH:
      goto ST_LATCH;

    case kTokenEND:
      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->end(mLoc1) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      goto ST_AFTER_END;

    case kTokenEXDC:
      goto ST_EXDC;

    case kTokenWIRE_LOAD_SLOPE:
      goto ST_DUMMY_READ1;

    case kTokenWIRE:
      goto ST_DUMMY_READ1;

    case kTokenINPUT_ARRIVAL:
      goto ST_DUMMY_READ1;

    case kTokenDEFAULT_INPUT_ARRIVAL:
      goto ST_DUMMY_READ1;

    case kTokenOUTPUT_REQUIRED:
      goto ST_DUMMY_READ1;

    case kTokenDEFAULT_OUTPUT_REQUIRED:
      goto ST_DUMMY_READ1;

    case kTokenINPUT_DRIVE:
      goto ST_DUMMY_READ1;

    case kTokenDEFAULT_INPUT_DRIVE:
      goto ST_DUMMY_READ1;

    case kTokenOUTPUT_LOAD:
      goto ST_DUMMY_READ1;

    case kTokenDEFAULT_OUTPUT_LOAD:
      goto ST_DUMMY_READ1;

    default:
      break;
    }
    goto ST_SYNTAX_ERROR;
  }

 ST_INPUTS:
  {
    tToken tk= get_token();
    if ( tk == kTokenSTRING ) {
      FileRegion loc = mCurLoc;
      const char* name = mScanner.cur_string().c_str();
      IdCell* cell = mIdHash.find(name, true);
      if ( cell->is_defined() ) {
	ostringstream buf;
	buf << name << ": Defined more than once. Previous definition is "
	    << cell->def_loc() << ".";
	mMsgMgr.put_msg(__FILE__, __LINE__, loc,
			kMsgError,
			"MLTDEF01", buf.str().c_str());
	goto ST_ERROR_EXIT;
      }
      if ( cell->is_output() ) {
	ostringstream buf;
	buf << name << ": Defined as both input and output."
	    << " Previous difinition is " << cell->loc() << ".";
	mMsgMgr.put_msg(__FILE__, __LINE__, loc,
			kMsgWarning,
			"MLTDEF02", buf.str().c_str());
      }
      cell->set_loc(loc);
      cell->set_defined();
      cell->set_input();
      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->inputs_elem(cell->id()) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      ++ n_token;
      goto ST_INPUTS;
    }
    if ( tk == kTokenNL ) {
      if ( n_token == 0 ) {
	mMsgMgr.put_msg(__FILE__, __LINE__, mCurLoc,
			kMsgWarning,
			"SYN07", "Empty '.inputs' statement. Ignored.");
      }
      goto ST_NEUTRAL;
    }
    goto ST_SYNTAX_ERROR;
  }

 ST_OUTPUTS:
  {
    tToken tk= get_token();
    if ( tk == kTokenSTRING ) {
      FileRegion loc = mCurLoc;
      const char* name = mScanner.cur_string().c_str();
      IdCell* cell = mIdHash.find(name, true);
      if ( cell->is_output() ) {
	ostringstream buf;
	buf << name << ": Defined more than once. Previous definition is "
	    << cell->loc();
	mMsgMgr.put_msg(__FILE__, __LINE__, loc,
			kMsgError,
			"MLTDEF03", buf.str().c_str());
	goto ST_ERROR_EXIT;
      }
      if ( cell->is_input() ) {
	ostringstream buf;
	buf << name << ": Defined as both input and output. "
	    << "Previous definition is "
	    << cell->loc() << ".";
	mMsgMgr.put_msg(__FILE__, __LINE__, loc,
			kMsgWarning,
			"MLTDEF02", buf.str().c_str());
      }
      cell->set_loc(loc);
      cell->set_output();
      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->outputs_elem(cell->id()) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      ++ n_token;
      goto ST_OUTPUTS;
    }
    if ( tk == kTokenNL ) {
      if ( n_token == 0 ) {
	mMsgMgr.put_msg(__FILE__, __LINE__, mCurLoc,
			kMsgWarning,
			"SYN08", "Empty '.outputs' statement. Ignored.");
      }
      goto ST_NEUTRAL;
    }
    goto ST_SYNTAX_ERROR;
  }

 ST_NAMES:
  { // str* nl
    tToken tk= get_token();
    if ( tk == kTokenSTRING ) {
      FileRegion loc = mCurLoc;
      const char* name = mScanner.cur_string().c_str();
      IdCell* cell = mIdHash.find(name, true);
      cell->set_loc(loc);
      mNameArray.push_back(cell);
      goto ST_NAMES;
    }
    if ( tk == kTokenNL ) {
      size_t n = mNameArray.size();
      if ( n == 0 ) {
	// 名前が1つもない場合
	mMsgMgr.put_msg(__FILE__, __LINE__, mCurLoc,
			kMsgError,
			"SYN09",
			"Empty '.names' statement.");
	goto ST_ERROR_EXIT;
      }
      else if ( n == 1 ) {
	// 名前が1つしかない場合
	goto ST_NAMES0;
      }
      else {
	// 通常の場合
	goto ST_NAMES1;
      }
    }
    goto ST_SYNTAX_ERROR;
  }

 ST_NAMES0:
  { // str nl
    tToken tk= get_token();
    FileRegion loc1 = mCurLoc;
    if ( tk == kTokenSTRING ) {
      mName1 = mScanner.cur_string();
      FileRegion loc2 = mCurLoc;
      char opat = mName1[0];
      if ( opat != '0' && opat != '1' ) {
	mMsgMgr.put_msg(__FILE__, __LINE__, loc2,
			kMsgError,
			"SYN15",
			"Illegal character in output cube.");
	goto ST_ERROR_EXIT;
      }
      if ( mOpat == ' ' ) {
	mOpat = opat;
      }
      else if ( mOpat != opat ) {
	mMsgMgr.put_msg(__FILE__, __LINE__, loc2,
			kMsgError,
			"SYN10",
			"Outpat pattern mismatch.");
	goto ST_ERROR_EXIT;
      }
      if ( get_token() == kTokenNL ) {
	++ mNc;
	goto ST_NAMES0;
      }
      mMsgMgr.put_msg(__FILE__, __LINE__, loc2,
		      kMsgError,
		      "SYN14",
		      "Newline is expected.");
      goto ST_ERROR_EXIT;
    }
    else if ( tk == kTokenNL ) {
      // 空行はスキップ
      goto ST_NAMES0;
    }
    else {
      unget_token(tk, loc1);
      goto ST_NAMES_END;
    }
  }

 ST_NAMES1:
  { // str str nl
    tToken tk= get_token();
    FileRegion loc1 = mCurLoc;
    if ( tk == kTokenSTRING ) {
      mName1 = mScanner.cur_string();
      if ( mName1.size() != mNameArray.size() - 1 ) {
	mMsgMgr.put_msg(__FILE__, __LINE__, loc1,
			kMsgError,
			"SYN12",
			"Input pattern does not fit "
			"with the number of fanins.");
	goto ST_ERROR_EXIT;
      }
      ymuint n = mName1.size();
      mCoverPat.reserve(mCoverPat.size() + n);
      for (size_t i = 0; i < n; ++ i) {
	char c = mName1[i];
	if ( c == '1' ) {
	  mCoverPat.put_char('1');
	}
	else if ( c == '0' ) {
	  mCoverPat.put_char('0');
	}
	else if ( c == '-' ) {
	  mCoverPat.put_char('-');
	}
	else {
	  mMsgMgr.put_msg(__FILE__, __LINE__, loc1,
			  kMsgError,
			  "SYN11",
			  "Illegal character in input cube.");
	  goto ST_ERROR_EXIT;
	}
      }
      tk = get_token();
      if ( tk == kTokenSTRING ) {
	FileRegion loc2 = mCurLoc;
	char opat = mScanner.cur_string()[0];
	if ( opat != '0' && opat != '1' ) {
	  mMsgMgr.put_msg(__FILE__, __LINE__, loc2,
			  kMsgError,
			  "SYN15",
			  "Illegal character in output cube.");
	  goto ST_ERROR_EXIT;
	}
	if ( mOpat == ' ' ) {
	  mOpat = opat;
	}
	else if ( mOpat != opat ) {
	  mMsgMgr.put_msg(__FILE__, __LINE__, loc2,
			  kMsgError, "SYN10",
			  "Outpat pattern mismatch.");
	  goto ST_ERROR_EXIT;
	}
	if ( get_token() != kTokenNL ) {
	  mMsgMgr.put_msg(__FILE__, __LINE__, loc2,
			  kMsgError, "SYN14",
			  "Newline is expected.");
	  goto ST_ERROR_EXIT;
	}
	++ mNc;
	goto ST_NAMES1;
      }
      mMsgMgr.put_msg(__FILE__, __LINE__, loc1,
		      kMsgError, "SYN13",
		      "No output cube.");
      goto ST_ERROR_EXIT;
    }
    else if ( tk == kTokenNL ) {
      // 空行はスキップ
      goto ST_NAMES1;
    }
    else {
      unget_token(tk, loc1);
      goto ST_NAMES_END;
    }
  }

 ST_NAMES_END:
  {
    size_t n = mNameArray.size();
    IdCell* cell = mNameArray[n - 1];
    if ( cell->is_defined() ) {
      // 二重定義
      ostringstream buf;
      buf << cell->str() << ": Defined more than once. "
	  << "Previsous Definition is " << cell->def_loc() << ".";
      mMsgMgr.put_msg(__FILE__, __LINE__, cell->loc(),
		      kMsgError,
		      "MLTDEF01", buf.str().c_str());
      goto ST_ERROR_EXIT;
    }
    cell->set_defined();
    for (size_t i = 0; i < n; ++ i) {
      mIdArray.push_back(mNameArray[i]->id());
    }
    for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      BlifHandler* handler = *p;
      if ( !handler->names(mIdArray, mNc, mCoverPat.c_str(), mOpat) ) {
	stat = false;
      }
    }
    if ( !stat ) {
      goto ST_ERROR_EXIT;
    }
    goto ST_NEUTRAL;
  }

 ST_GATE:
  { // str -> ST_GATE1
    tToken tk= get_token();
    if ( tk != kTokenSTRING ) {
      goto ST_GATE_SYNERROR;
    }
    FileRegion loc = mCurLoc;
    const char* name = mScanner.cur_string().c_str();
    for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      BlifHandler* handler = *p;
      if ( !handler->gate_begin(mLoc1, loc, name) ) {
	stat = false;
      }
    }
    if ( !stat ) {
      goto ST_ERROR_EXIT;
    }
    n_token = 0;
    goto ST_GATE1;
  }

 ST_GATE1:
  { // (str '=' str)* nl
    tToken tk= get_token();
    if ( tk == kTokenSTRING ) {
      FileRegion loc1 = mCurLoc;
      mName1 = mScanner.cur_string();
      const char* name1 = mName1.c_str();
      tk = get_token();
      if ( tk != kTokenEQ ) {
	goto ST_GATE_SYNERROR;
      }
      tk = get_token();
      if ( tk != kTokenSTRING ) {
	goto ST_GATE_SYNERROR;
      }
      FileRegion loc2 = mCurLoc;
      const char* name2 = mScanner.cur_string().c_str();
      IdCell* cell = mIdHash.find(name2, true);
      cell->set_loc(loc2);

      // 注意! 出力ピン名を 'o' もしくは 'O' と仮定している．
      if ( strcmp(name1, "o") == 0 ||
	   strcmp(name1, "O") == 0 ) {
	if ( cell->is_defined() ) {
	  // 二重定義
	  ostringstream buf;
	  buf << cell->str() << ": Defined more than once. "
	      << "Previsous Definition is " << cell->def_loc() << ".";
	  mMsgMgr.put_msg(__FILE__, __LINE__, cell->loc(),
			  kMsgError,
			  "MLTDEF01", buf.str().c_str());
	  goto ST_ERROR_EXIT;
	}
	cell->set_defined();
      }

      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->gate_assign(loc1, name1, loc2, name2) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      ++ n_token;
      goto ST_GATE1;
    }
    else if ( tk == kTokenNL ) {
      if ( n_token == 0 ) {
	goto ST_GATE_SYNERROR;
      }
      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->gate_end() ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      goto ST_NEUTRAL;
    }
  }

 ST_GATE_SYNERROR:
  mMsgMgr.put_msg(__FILE__, __LINE__, mCurLoc,
		  kMsgError,
		  "SYN16", "Syntax error in '.gate' statement.");
  goto ST_ERROR_EXIT;

 ST_LATCH:
  {
    tToken tk= get_token();
    if ( tk == kTokenSTRING ) {
      FileRegion loc2 = mCurLoc;
      const char* name1 = mScanner.cur_string().c_str();
      IdCell* cell1 = mIdHash.find(name1, true);
      cell1->set_loc(loc2);

      tk = get_token();
      if ( tk != kTokenSTRING ) {
	goto ST_LATCH_SYNERROR;
      }
      FileRegion loc3 = mCurLoc;
      const char* name2 = mScanner.cur_string().c_str();
      IdCell* cell2 = mIdHash.find(name2, true);
      cell2->set_loc(loc3);

      if ( cell2->is_defined() ) {
	// 二重定義
	ostringstream buf;
	buf << cell2->str() << ": Defined more than once. "
	    << "Previsous Definition is " << cell2->def_loc() << ".";
	mMsgMgr.put_msg(__FILE__, __LINE__, cell2->loc(),
			kMsgError,
			"MLTDEF01", buf.str().c_str());
	goto ST_ERROR_EXIT;
      }
      cell2->set_defined();

      tk = get_token();
      char rval = ' ';
      FileRegion loc4 = mCurLoc;
      if ( tk == kTokenSTRING ) {
	rval = mScanner.cur_string()[0];
	if ( rval != '0' && rval != '1' ) {
	  mMsgMgr.put_msg(__FILE__, __LINE__, loc4,
			  kMsgError,
			  "SYN18",
			  "Illegal character for reset value.");
	  goto ST_ERROR_EXIT;
	}
	tk = get_token();
      }
      if ( tk != kTokenNL ) {
	goto ST_LATCH_SYNERROR;
      }

      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->latch(cell1->id(), cell2->id(),
			     loc4, rval) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      goto ST_NEUTRAL;
    }
    else {
      goto ST_LATCH_SYNERROR;
    }
  }

 ST_LATCH_SYNERROR:
  mMsgMgr.put_msg(__FILE__, __LINE__, mCurLoc,
		  kMsgError,
		  "SYN17", "Syntax error in '.latch' statement.");
  goto ST_ERROR_EXIT;

 ST_AFTER_END:
  {
    tToken tk= get_token();
    if ( tk == kTokenEOF ) {
      goto ST_NORMAL_EXIT;
    }
    if ( tk != kTokenNL ) {
      mMsgMgr.put_msg(__FILE__, __LINE__, mCurLoc,
		      kMsgWarning,
		      "SYN06",
		      "Statement after '.end' is ignored.");
    }
    goto ST_AFTER_END;
  }

 ST_EXDC:
  {
    tToken tk= get_token();
    if ( tk == kTokenEND ) {
      goto ST_NEUTRAL;
    }
    goto ST_EXDC;
  }

 ST_DUMMY_READ1:
  {
    tToken tk= get_token();
    if ( tk == kTokenNL ) {
      goto ST_NEUTRAL;
    }
    goto ST_DUMMY_READ1;
  }

 ST_AFTER_EOF:
  {
    FileRegion loc = mCurLoc;
    mMsgMgr.put_msg(__FILE__, __LINE__, loc,
		    kMsgWarning,
		    "SYN05",
		    "unexpected EOF. '.end' is assumed.");
    for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      BlifHandler* handler = *p;
      if ( !handler->end(loc) ) {
	stat = false;
      }
    }
    if ( !stat ) {
      goto ST_ERROR_EXIT;
    }
    goto ST_NORMAL_EXIT;
  }

 ST_NORMAL_EXIT:
  {
    size_t n = mIdHash.num();
    for (size_t i = 0; i < n; ++ i) {
      IdCell* cell = mIdHash.cell(i);
      if ( !cell->is_defined() ) {
	ostringstream buf;
	buf << cell->str() << ": Undefined.";
	mMsgMgr.put_msg(__FILE__, __LINE__, cell->loc(),
			kMsgError,
			"UNDEF01", buf.str().c_str());
	goto ST_ERROR_EXIT;
      }
    }
  }
  for (list<BlifHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    BlifHandler* handler = *p;
    handler->normal_exit();
  }

  return true;

 ST_SYNTAX_ERROR:
  mMsgMgr.put_msg(__FILE__, __LINE__, mCurLoc,
		  kMsgError,
		  "SYN00",
		  "Syntax error.");
  // わざと次の行につづく
  // goto ST_ERROR_EXIT;

 ST_ERROR_EXIT:
  for (list<BlifHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    BlifHandler* handler = *p;
    handler->error_exit();
  }

  return false;
}

// @brief イベントハンドラの登録
void
BlifParserImpl::add_handler(BlifHandler* handler)
{
  mHandlerList.push_back(handler);
  handler->mParser = this;
}

// @brief トークンを一つ読み出す．
tToken
BlifParserImpl::get_token()
{
  if ( mUngetToken != kTokenEOF ) {
    // トークンバッファに値がある場合にはそれを返す．
    tToken tk = mUngetToken;
    mUngetToken = kTokenEOF;
    mCurLoc = mUngetTokenLoc;
    return tk;
  }
  tToken token = mScanner.get_token();
  mCurLoc = mScanner.cur_loc();
  return token;
}

END_NAMESPACE_YM_BLIF
