#ifndef NETWORKVIEW_H
#define NETWORKVIEW_H

/// @file NetworkView.h
/// @brief NetworkView のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"
#include "GateMgr.h"


namespace nsYm {
namespace nsLed {

class GateObj;

//////////////////////////////////////////////////////////////////////
/// @class NetworkView NetworkView.h "NetworkView.h"
/// @brief 回路図を表示するためのウィジェット
//////////////////////////////////////////////////////////////////////
class NetworkView :
  public QWidget
{

  Q_OBJECT

public:

  /// @brief コンストラクタ
  NetworkView(QWidget* parent = NULL);

  /// @brief デストラクタ
  virtual
  ~NetworkView();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートを作る．
  /// @param[in] pos 位置
  /// @param[in] type 型
  /// @param[in] ni 入力数
  GateObj*
  new_gate(const QPoint& pos,
	   GateType type,
	   ymuint ni = 0);

  /// @brief ゲートを作る．
  /// @param[in] x, y 位置
  /// @param[in] type 型
  /// @param[in] ni 入力数
  GateObj*
  new_gate(int x,
	   int y,
	   GateType type,
	   ymuint ni = 0);

  /// @brief 倍率を得る．
  qreal
  scale() const;


public slots:
  //////////////////////////////////////////////////////////////////////
  // スロット
  //////////////////////////////////////////////////////////////////////

  /// @brief 倍率を変更する．
  void
  setScale(qreal scale);

  void
  setScale2(int val);


protected:
  //////////////////////////////////////////////////////////////////////
  // QWidget の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 描画イベントのハンドラ
  virtual
  void
  paintEvent(QPaintEvent* event);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズや倍率が変更されたときに再計算を行なう．
  void
  calc_size();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ゲートを管理するオブジェクト
  GateMgr mGateMgr;

  // ゲートリスト
  vector<GateObj*> mGateList;

  // 全てのゲートを囲む最小の矩形
  QRect mBoundingBox;

  // 倍率
  qreal mScale;

};

} // nsLed
} // nsYm

#endif // NETWORKVIEW_H
