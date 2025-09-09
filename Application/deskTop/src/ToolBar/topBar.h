#ifndef __TOP_BAR_H
#define __TOP_BAR_H

#include "TpDialog.h"
#include "TpLabel.h"
#include "TpTimer.h"
#include "TpEvent.h"
#include "TpBattery.h"

class topBar : public TpDialog
{
public:
	topBar(TpScreen *topScreen);
	virtual ~topBar();

	virtual void setVisible(bool visible = true) override;

	/// @brief 设置顶部工具栏颜色
	/// @param appColor 应用底部颜色，根据应用颜色，选择设置工具栏是黑色/白色
	void setColor(const int32_t &appColor);

public:
	virtual void construct();	// 这个函数直接会被构造函数调用，不需要去修改构造，初始化代码直接在这个函数中写就好
	virtual void destruction(); // 这个函数直接会被析构函数调用，不需要去修改析构，代码直接在这个函数中写就好
								// 其余函数和数据自定义

protected:
	virtual bool onResizeEvent(TpResizeEvent *event) override;

	virtual bool onMousePressEvent(TpMouseEvent *event) override;
	virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;

	virtual bool onMouseMoveEvent(TpMouseEvent *event) override;

	virtual bool onLeaveEvent(TpLeaveEvent *event) override;

private:
	void caculateTopAppPos();

	void slotUpdateSystemTime();

	// 将周几的数字转为汉字显示
	TpString transWeekData(const int32_t &dayOfWeek);

private:
	TpScreen *topScreen_;

	TpLabel *sysDateLabel_;
	TpLabel *sysTimeLabel_;

	// 蓝牙和网络连接状态
	TpLabel *wifiLabel_;
	TpLabel *blueToothLabel_;

	// 电量显示窗
	TpBattery *elecBattery_;

	TpTimer *updateTimetimer_;
};

#endif
