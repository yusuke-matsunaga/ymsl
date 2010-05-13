
/// @file magus/lutmap/CutHolder.cc
/// @brief CutHolder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CutHolder.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "CutHolder.h"
#include "Cut.h"
#include "SbjGraph.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

// コンストラクタ
CutHolder::CutHolder()
{
  mCutList = NULL;
  mCurChunk = NULL;
  mLast = 0;
}

// デストラクタ
CutHolder::~CutHolder()
{
  clear();
}

// @brief 保持しているカットのリストを削除する．
void
CutHolder::clear()
{
  delete [] mCutList;
  for (vector<char*>::iterator p = mChunk.begin();
       p != mChunk.end(); ++ p) {
    delete [] *p;
  }
  mCurChunk = NULL;
  mLast = 0;
  mChunk.clear();
}

// 最初に呼ばれる関数
void
CutHolder::all_init(const SbjGraph& sbjgraph,
		    ymuint limit)
{
  clear();
  ymuint n = sbjgraph.max_node_id();
  mLimit = limit;
  mCutList = new CutList[n];
}

void
CutHolder::node_init(SbjNode* node,
		     ymuint pos)
{
}

void
CutHolder::found(SbjNode* root)
{
}

void
CutHolder::found(SbjNode* root,
		 ymuint ni,
		 SbjNode* inputs[])
{
  Cut* cut = alloc_cut(ni);
  cut->mRoot = root;
  cut->mNi = ni;
  for (ymuint i = 0; i < ni; ++ i) {
    cut->mInputs[i] = inputs[i];
  }
  mCutList[root->id()].push_back(cut);
}

void
CutHolder::node_end(SbjNode* node,
		    ymuint pos,
		    ymuint ncuts)
{
}

// 最後に呼ばれる関数
void
CutHolder::all_end(const SbjGraph& sbjgraph,
		   ymuint limit)
{
}

// カットの領域を確保する．
Cut*
CutHolder::alloc_cut(ymuint ni)
{
  ymuint size = sizeof(Cut) + (ni - 1) * sizeof(SbjNode*);
  assert_cond(size < kPageSize, __FILE__, __LINE__);
  if ( mCurChunk == NULL || mLast + size > kPageSize ) {
    mCurChunk = new char[kPageSize];
    mLast = 0;
    mChunk.push_back(mCurChunk);
  }
  char* top = mCurChunk + mLast;
  mLast += size;
  return reinterpret_cast<Cut*>(top);
}

END_NAMESPACE_MAGUS_LUTMAP
