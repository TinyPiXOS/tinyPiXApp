#ifndef __TP_VDESK_SCREEN_H
#define __TP_VDESK_SCREEN_H

#include "tpFixScreen.h"
#include "tpJsonObject.h"
#include "tpGateway.h"
#include "tpScrollPanel.h"
#include "tpDisplay.h"
#include "tpGridLayout.h"
#include "mainAppScrollPanel.h"
#include "tpMenu.h"
#include "tpCarouselButton.h"
#include "deskTopGlobal.hpp"
#include "tpDesktopGlobal.h"
#include "tpGateway.h"
#include "tpAppInstall.h"
#include "tpTimer.h"

#include "toolWindow/sysLockWindow.h"
#include "toolWindow/appOperateMaskWindow.h"
#include "toolBar/topBar.h"
#include "toolBar/bottomBar.h"
#include "toolBar/navigationBar.h"
#include "toolModule/caculateAppPos.h"
#include "toolModule/desktopAppButton.h"

/*
该类就是桌面类，在桌面中增加各种组件可以使用该类完成，需要注意的事：
1、不需要修改本类的名字，不需要关注构造和析构函数
2、只需要桌椅桌面需要的特征，对应开发即可，可能需要的元素如下：
	1）布局（一般为网格即可，原有的gridlayout需要优化），应对组件的编排
	2）工具条（根据设计，是否提供上下工具条，即toolWidget），并支持相应的事件（比如上拉，下拉等）
	3）支持和其他窗口应用通信的基本要素
3、使用tpDeskScreen.h和tpDeskScreen.c即可，其他类可以自定义
4、只能使用系统已有的库，不支持在建私有库文件。可以重载已有的任何组件
*/

TP_DEF_VOID_TYPE_VAR(ItpScreenPrivData);

class tpDeskScreen
	: public tpFixScreen,
	  public tpGateway
{
public:
	tpDeskScreen();
	virtual ~tpDeskScreen();

public:
	virtual void construct();	// 这个函数直接会被构造函数调用，不需要去修改构造，初始化代码直接在这个函数中写就好
	virtual void destruction(); // 这个函数直接会被析构函数调用，不需要去修改析构，代码直接在这个函数中写就好

public:
	virtual void recvData(const char *topic, const void *data, const uint32_t &size) override;

public:
	virtual bool appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require);

public:
	virtual bool onRotateEvent(tpObjectRotateEvent *event) override;
	virtual bool onActiveEvent(tpObjectActiveEvent *event) override;

	virtual bool onMousePressEvent(tpMouseEvent *event) override;
	virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;
	
	virtual bool onMouseMoveEvent(tpMouseEvent *event) override;
	virtual bool onLeaveEvent(tpObjectLeaveEvent *event) override;

	virtual bool eventFilter(tpObject *watched, tpEvent *event) override;

private:
	/// @brief 长按桌面或图标，触发卸载/移动事件；长按桌面operateBtn为空
	void slotOperateApp(desktopAppButton *operateBtn);

	// 卸载APP
	void slotDeleteApp(desktopAppButton *operateBtn);

	// 安装APP；定时获取安装进度
	void slotTimeoutInstallApp();

private:
	void initData();

	/// @brief 初始化桌面安装应用的所有配置文件信息
	void intDeskAppConfig();

	// 构建所有APP图标；包括工具栏和桌面的
	void createAppBtn();

	// 窗口旋转、大小变化等，刷新中心APP窗口,需放置再 topBar和bottomBar大小计算完毕后调用
	void refreshMainAppPanel();

	// 指定页码刷新
	void refreshAppPage(const uint32_t &pageNum);

	// 刷新上下工具栏配置信息
	void refreshBar();

	/// @brief 创建桌面图标；放置在桌面窗体内；内部会初始化按钮；并根据页码索引放置在对应位置
	desktopAppButton *createDeskAppBtn(ApplicationInfoSPtr appInfo, const tpString &iconPath, const tpString &appName);

	// 给入APPuuid创建配置app按钮;配置按钮信息、绑定信号槽等；工具栏按钮和桌面按钮均通过此函数创建
	// UUID、应用图标绝对路径、应用名称
	desktopAppButton *configAppBtn(const tpString &appUuid, const tpString &iconPath, const tpString &appName);

	// 指定应用UUID启动应用；若已在后台则唤醒
	void startApp(const tpString &uuid, const tpVector<tpString> &argList = tpVector<tpString>());

	// 指定文件路径安装应用
	void installApp(const tpString &pkgPath);

	// 给入当前页所有APP信息，获取可用的APP索引，函数外部需要判断该页APP是否已经放满了
	uint32_t findAppIndex(const tpList<ApplicationInfoSPtr> &pageAppList);

private:
	// 顶部和底部工具栏
	topBar *topFloatBar_ = nullptr;
	bottomBar *bottomFloatBar_ = nullptr;
	navigationBar *navigationFloatBar_ = nullptr;

	tpAppInstall *appInstallPtr_;
	tpTimer *appInstallTimer_;

	// APP显示主区域
	mainAppScrollPanel *mainAppPanel_;

	// 菜单轮播按钮
	tpCarouselButton *carouselButton_;

	// 是否是横屏，默认为横屏
	bool isLandscapeScreen_ = true;

	// 后缀要带 /
	tpString appConfigPathStr_ = "/System/";

	// 当前显示页码
	uint32_t curPage_ = 0;

	// 页码，该页的APP信息
	tpMap<uint32_t, tpList<ApplicationInfoSPtr>> allAppInfoMap_;

	// 底部工具栏的应用信息
	tpList<ApplicationInfoSPtr> bottomBarAppList_;

	// 鼠标点击topBar坐标
	ItpPoint pressTopBarPoint_;
	bool pressTopBar_ = false;

	// 应用长按操作菜单
	tpMenu *operateMenu_;
	// 应用长按遮罩层
	appOperateMaskWindow *maskWindow_;
	// 鼠标按下时的APP对象，鼠标释放时置空
	desktopAppButton *pressAppBtn_;
	ItpRect pressBtnRect_;

	// 当前正在安装的APP
	desktopAppButton *installingApp_;

	// 是否是APP移动模式
	bool isMoveMode_;

	// 用于拖拽APP过程中计算APP吸附位置及移动其他APP图标
	caculateAppPos caculateAppPosPtr_;
};

#endif
