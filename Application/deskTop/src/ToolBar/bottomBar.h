#ifndef __BOTTOM_BAR_H
#define __BOTTOM_BAR_H

#include "TpDialog.h"
#include "TpList.h"
#include "deskTopGlobal.hpp"
#include "toolModule/desktopAppButton.h"

class bottomBar
	: public TpChildWidget
{
public:
	// 顶层窗口指针
	bottomBar(TpScreen* topScreen);
	virtual ~bottomBar();

	virtual void setVisible(bool visible = true) override;

	// 设置工具栏按钮列表
	void setBottomAppBtn(const TpList<desktopAppButton*>& appList);

	/// @brief 获取底部工具栏APP按钮列表
	/// @return 
	TpList<desktopAppButton*> bottomAppBtnList();

	// 卸载应用
	void removeApp(desktopAppButton* removeBtn);

protected:
	virtual bool onResizeEvent(TpObjectResizeEvent *event) override;
	virtual bool onMoveEvent(TpObjectMoveEvent *event) override;
    virtual bool onMousePressEvent(TpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;

private:
	void caculateBottomAppPos();

private:
	TpScreen* topScreen_;

	// 系统底部应用图标信息
	TpList<desktopAppButton *> systemAppBtnList_;

	// 所有在底部的icon列表
	// TpList<ApplicationInfoSPtr> bottomAppInfoList_;
};

#endif
