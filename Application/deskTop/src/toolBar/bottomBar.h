#ifndef __BOTTOM_BAR_H
#define __BOTTOM_BAR_H

#include "tpDialog.h"
#include "tpList.h"
#include "deskTopGlobal.hpp"
#include "toolModule/desktopAppButton.h"

class bottomBar
	: public tpChildWidget
{
public:
	// 顶层窗口指针
	bottomBar(tpScreen* topScreen);
	virtual ~bottomBar();

	virtual void setVisible(bool visible = true) override;

	// 设置工具栏按钮列表
	void setBottomAppBtn(const tpList<desktopAppButton*>& appList);

	/// @brief 获取底部工具栏APP按钮列表
	/// @return 
	tpList<desktopAppButton*> bottomAppBtnList();

	// 卸载应用
	void removeApp(desktopAppButton* removeBtn);

protected:
	virtual bool onResizeEvent(tpObjectResizeEvent *event) override;
	virtual bool onMoveEvent(tpObjectMoveEvent *event) override;
    virtual bool onMousePressEvent(tpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;

private:
	void caculateBottomAppPos();

private:
	tpScreen* topScreen_;

	// 系统底部应用图标信息
	tpList<desktopAppButton *> systemAppBtnList_;

	// 所有在底部的icon列表
	// tpList<ApplicationInfoSPtr> bottomAppInfoList_;
};

#endif
