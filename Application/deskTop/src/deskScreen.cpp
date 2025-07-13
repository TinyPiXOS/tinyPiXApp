#include "deskScreen.h"
#include "tpSurface.h"
#include "tpEvent.h"
#include "deskTopGlobal.hpp"
#include "tpJsonDocument.h"
#include "tpFile.h"
#include "tpDir.h"
#include "tpFont.h"
#include "tpProcess.h"
#include "tpMessageBox.h"
#include "tpAppConfigIO.h"

#include <iostream>

#ifndef APP_CONFIG_SON_PATH
#define APP_CONFIG_SON_PATH "conf/app/"
#endif

#ifndef APP_FILES_SON_PATH
#define APP_FILES_SON_PATH "app/"
#endif

sysLockWindow *globalSysLockWindow = nullptr;
appTaskManageWindow *globalAppTaskWindow = nullptr;
appSettingBar *globalTopSettingBar_ = nullptr;

tpScreen *globalMainScreen_ = nullptr;
IPiSysApiAgent *globalAgent = nullptr;

// <pid, 应用信息>
tpHash<int32_t, RunAppInfo> globalRunAppMap_ = tpHash<int32_t, RunAppInfo>();
std::mutex readRunAppMutex_;
tpHash<tpString, int32_t> globalUuidPidMap_ = tpHash<tpString, int32_t>();

static inline void BAR_SET_ATTRIB(tpDialog *vars, int32_t pop, int32_t x, int32_t y, int32_t width, int32_t height)
{
	if (pop)
	{
		height = 1;
	}

	vars->setRect(x, y, width, height);
	vars->setVisible(true);
	vars->update();
}

void tpDeskScreen::construct()
{
	// 订阅数据
	initializeGateway();
	subscribeGatewayData(RunAppTopic, this);

	setEnableBackGroundColor(false);
	setEnabledBorderColor(false);

	globalMainScreen_ = this;

	std::cout << "tpDeskScreen::construct()" << std::endl;

	initData();

	// 绑定桌面长按事件
	// connect(mainAppPanel_, onLongPress, [=]()
	// 		{ slotOperateApp(nullptr); });

	// 此处应该读取配置文件中的设置文件，可以保留上次设置的背景图片

	tpShared<tpSurface> surface = tpMakeShared<tpSurface>();

	surface->fromFile(applicationDirPath() + "/../res/默认桌面背景.png");

	this->setBackGroundImage(surface, true);

	// 此处应该读取配置文件中的设置文件，加载配置参数

	refreshBar();

	intDeskAppConfig();
}

void tpDeskScreen::destruction()
{
	if (topFloatBar_)
	{
		delete topFloatBar_;
		topFloatBar_ = nullptr;
	}
	if (bottomFloatBar_)
	{
		delete bottomFloatBar_;
		bottomFloatBar_ = nullptr;
	}
	if (navigationFloatBar_)
	{
		delete navigationFloatBar_;
		navigationFloatBar_ = nullptr;
	}
}

void tpDeskScreen::recvData(const char *topic, const void *data, const uint32_t &size)
{
	tpString topicString(topic);
	if (topicString.compare(RunAppTopic) == 0)
	{
		RunApp recvRunData;
		recvRunData.StructDeserialize(data, size);

		std::cout << "RecvStartApp UUID : " << recvRunData.appUuid << std::endl;

		// 启动指定应用
		tpVector<tpString> argList;
		for (const auto &recvArg : recvRunData.argList)
		{
			std::cout << " recvArg : " << recvArg << std::endl;
			argList.emplace_back(recvArg);
		}
		startApp(recvRunData.appUuid, argList);
	}
}

bool tpDeskScreen::appChange(int32_t id, int32_t pid, int32_t rotate, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
	int32_t sysid = this->objectSysID();

	// std::cout << "tpDeskScreen::appChange " << std::endl;
	std::cout << "id " << id << std::endl;
	std::cout << "sysid " << sysid << std::endl;

	// std::cout << "Desktop visible" << visible << std::endl;
	// std::cout << "Desktop active" << active << std::endl;

	if (sysid == id)
	{
		if (bottomFloatBar_)
			bottomFloatBar_->setVisible(true);
		if (topFloatBar_)
			topFloatBar_->setVisible(true);
		return false;
	}

	// 应用启动，不显示上下工具栏
	// bottomFloatBar_->setVisible(false);
	topFloatBar_->setVisible(true);

	if (active == false)
	{
		// 桌面即将隐藏
		return false;
	}

	printf("id = %d, rotate = %d\n", id, rotate);

	int32_t width = 0, height = 0;

	switch (rotate)
	{
	case TP_ROT_0:
	case TP_ROT_360:
	case TP_ROT_180:
	case TP_ROT_180X:
	{
		width = this->width();
		height = this->height();
		isLandscapeScreen_ = true;
	}
	break;
	case TP_ROT_90:
	case TP_ROT_270X:
	case TP_ROT_270:
	case TP_ROT_90X:
	{
		width = this->height();
		height = this->width();
		isLandscapeScreen_ = false;
	}
	break;
	default:
		return false;
	}

	return true;
}

bool tpDeskScreen::onRotateEvent(tpObjectRotateEvent *event)
{
	if (this->actived())
	{
		refreshBar();

		intDeskAppConfig();
	}

	return true;
}

bool tpDeskScreen::onActiveEvent(tpObjectActiveEvent *event)
{
	std::cout << "tpDeskScreen::onActiveEvent " << std::endl;

	if (event->isActived())
	{
		refreshBar();
	}
	else
	{
		if (bottomFloatBar_)
			bottomFloatBar_->setVisible(false);
	}

	return true;
}

bool tpDeskScreen::onMousePressEvent(tpMouseEvent *event)
{
	// 记录鼠标点击坐标
	uint32_t pressY = event->globalPos().y;

	if (pressY < TOP_BAR_HEIGHT)
	{
		pressTopBar_ = true;
		pressTopBarPoint_ = event->globalPos();
	}

	return false;
}

bool tpDeskScreen::onMouseRleaseEvent(tpMouseEvent *event)
{
	pressTopBar_ = false;
	pressAppBtn_ = nullptr;
	isMoveMode_ = false;

	return false;
}

bool tpDeskScreen::onMouseMoveEvent(tpMouseEvent *event)
{
	if (pressTopBar_)
	{
		ItpPoint curMousePos = event->globalPos();

		if ((curMousePos.y - pressTopBarPoint_.y) >= 5)
		{
			// 触发一次下拉事件后，不再重复触发
			pressTopBar_ = false;

			if (globalTopSettingBar_)
			{
				std::cout << "topSettingBar_ Show " << std::endl;
				globalTopSettingBar_->setVisible(true);
				globalTopSettingBar_->update();
			}
		}
	}

#if 0 // 暂时屏蔽长按移动图标功能
	if (isMoveMode_ && pressAppBtn_)
	{
		ItpPoint curGlobalPoint = event->globalPos();
		// ItpPoint curPoint = event->globalPos();

		// std::cout << "GlobalPos : " << curGlobalPoint.x << "  " << curGlobalPoint.y << std::endl;
		// std::cout << "curPoint : " << curPoint.x << "  " << curPoint.y << std::endl;
		// std::cout << "mainAppPanel_ Pos : " << mainAppPanel_->pos().x << "  " << mainAppPanel_->pos().y << std::endl;

		if (operateMenu_->visible())
			operateMenu_->close();
		if (maskWindow_->visible())
			maskWindow_->close();

		ItpSize iconSize = pressAppBtn_->iconSize();
		pressAppBtn_->move(curGlobalPoint.x - mainAppPanel_->pos().x - iconSize.w / 2.0, curGlobalPoint.y - mainAppPanel_->pos().y - iconSize.h / 2.0);
		mainAppPanel_->update();
		// pressAppBtn_->move(curGlobalPoint.x, curGlobalPoint.y);
	}
#endif

	return false;
}

bool tpDeskScreen::onLeaveEvent(tpObjectLeaveEvent *event)
{
	// if (event->eventType() == tpEvent::EVENT_OBJECT_LEAVE_TYPE)
	// {
	//     if (event->leave())
	//     {
	//         pressTopBar_ = false;
	//         // update();
	//     }
	// }

	return true;
}

bool tpDeskScreen::eventFilter(tpObject *watched, tpEvent *event)
{
	if (event->eventType() == tpEvent::EVENT_MOUSE_PRESS_TYPE)
	{
		tpMouseEvent *mouseKeyEvent = dynamic_cast<tpMouseEvent *>(event);
		if (!mouseKeyEvent)
			return false;

		// std::cout << "eventFilter MouseKey States : " << mouseKeyEvent->state() << std::endl;
		onMousePressEvent(mouseKeyEvent);

		if (isMoveMode_ && watched == mainAppPanel_)
		{
			std::cout << "Filter mainAppPanel_ " << std::endl;
			return true;
		}
	}
	else if (event->eventType() == tpEvent::EVENT_MOUSE_RELEASE_TYPE)
	{
		tpMouseEvent *mouseKeyEvent = dynamic_cast<tpMouseEvent *>(event);
		if (!mouseKeyEvent)
			return false;

		// std::cout << "eventFilter MouseKey States : " << mouseKeyEvent->state() << std::endl;
		onMouseRleaseEvent(mouseKeyEvent);

		if (isMoveMode_ && watched == mainAppPanel_)
		{
			std::cout << "Filter mainAppPanel_ " << std::endl;
			return true;
		}
	}
	else if (event->eventType() == tpEvent::EVENT_MOUSE_MOVE_TYPE)
	{
		tpMouseEvent *mouseMotionEvent = dynamic_cast<tpMouseEvent *>(event);
		if (!mouseMotionEvent)
			return false;

		onMouseMoveEvent(mouseMotionEvent);

		if (isMoveMode_ && watched == mainAppPanel_)
		{
			return true;
		}
	}
	else
	{
	}

	return false;
}

void tpDeskScreen::slotOperateApp(desktopAppButton *operateBtn)
{
	if (operateBtn)
	{
		ItpRect btnRect = operateBtn->toScreen();
		btnRect.w = operateBtn->iconSize().w;
		btnRect.h = operateBtn->iconSize().h;

		// 绘制全局遮罩层，只留长按的按钮显示
		maskWindow_->setAppRect(btnRect, operateBtn->roundCorners());
		maskWindow_->showMaximum();

		// uint32_t delIndex = operateMenu_->addItem("卸载", applicationDirPath() + "/../res/删除.png");

		operateMenu_->exec(btnRect.x, btnRect.y - tpDisplay::dp2Px(11) - operateMenu_->height());
	}
	else
	{
		// APP抖动
		tpList<tpObject *> appList = mainAppPanel_->objectList();
		for (const auto &appButton : appList)
		{
			desktopAppButton *iconButton = dynamic_cast<desktopAppButton *>(appButton);
			if (!iconButton)
				continue;

			// iconButton->setRotate();
		}
	}

	std::cout << " isMoveMode_ True " << std::endl;
	isMoveMode_ = true;
}

void tpDeskScreen::slotDeleteApp(desktopAppButton *operateBtn)
{
	if (!operateBtn)
	{
		std::cout << "卸载应用对象获取失败!" << std::endl;
		return;
	}

	tpString removeUuid = operateBtn->property("UUID").toString();

	if (removeUuid.empty())
	{
		std::cout << "应用UUID获取失败!" << std::endl;
		return;
	}

	// 如果应用正在运行，先杀掉进程
	if (globalUuidPidMap_.contains(removeUuid))
	{
		int32_t pid = globalUuidPidMap_.value(removeUuid);

		std::lock_guard<std::mutex> lock_g(readRunAppMutex_);
		globalUuidPidMap_.erase(removeUuid);
		globalRunAppMap_.erase(pid);

		std::cout << "结束应用 pid: " << pid << std::endl;
		tinyPiX_sys_kill_process(globalAgent, pid);
	}

	// 重置缓存操作按钮
	pressAppBtn_ = nullptr;

	// UI移除应用图标
	// 判断是不是底部工具栏的应用
	if (bottomFloatBar_->bottomAppBtnList().contains(operateBtn))
	{
		bottomFloatBar_->removeApp(operateBtn);

		for (const auto &bottomAppInfo : bottomBarAppList_)
		{
			if (bottomAppInfo->appUuid.compare(removeUuid) == 0)
			{
				bottomBarAppList_.remove(bottomAppInfo);
			}
		}
	}
	else
	{
		// 桌面移除卸载APP
		for (auto &allAppInfoIter : allAppInfoMap_)
		{
			for (auto &appInfo : allAppInfoIter.second)
			{
				if (appInfo->appUuid.compare(removeUuid) == 0)
				{
					allAppInfoIter.second.remove(appInfo);
					break;
				}
			}
		}

		// 滚动窗移除APP
		mainAppPanel_->delObject(operateBtn);

		// 内存泄漏
		// operateBtn->deleteLater();
	}

	// 卸载应用
	appInstallPtr_->remove(removeUuid);

	update();
}

void tpDeskScreen::slotTimeoutInstallApp()
{
	std::cout << "安装应用UUID" << appInstallPtr_->getAppUUID() << std::endl;

	int installSchedule = appInstallPtr_->getInstallSchedule();
	std::cout << "安装进度： " << installSchedule << std::endl;

	// 更新安装进度UI
	installingApp_->setInstallProgress(installSchedule);
	if (installSchedule == 100)
	{
		// 安装完成
		appInstallTimer_->stop();
	}
}

void tpDeskScreen::initData()
{
	globalTopSettingBar_ = new appSettingBar(this);
	if (globalTopSettingBar_ == nullptr)
	{
		std::cout << "topSettingBar init error!" << std::endl;
		std::exit(0);
	}
	globalTopSettingBar_->setVisible(false);

	topFloatBar_ = new topBar(this);
	if (topFloatBar_ == nullptr)
	{
		std::cout << "topbar init error!" << std::endl;
		std::exit(0);
	}
	topFloatBar_->installEventFilter(this);

	bottomFloatBar_ = new bottomBar(this);
	if (bottomFloatBar_ == nullptr)
	{
		std::cout << "bottomBar init error!" << std::endl;
		std::exit(0);
	}
	bottomFloatBar_->installEventFilter(this);

	navigationFloatBar_ = new navigationBar();
	if (navigationFloatBar_ == nullptr)
	{
		std::cout << "navigationFloatBar_ init error!" << std::endl;
		std::exit(0);
	}
	navigationFloatBar_->installEventFilter(this);

	// globalSysLockWindow = new sysLockWindow();
	// if (globalSysLockWindow == nullptr)
	// {
	// 	std::cout << "sysLockWindow init error!" << std::endl;
	// 	std::exit(0);
	// }
	// globalSysLockWindow->setVisible(false);

	globalAppTaskWindow = new appTaskManageWindow();
	if (globalAppTaskWindow == nullptr)
	{
		std::cout << "globalAppTaskWindow init error!" << std::endl;
		std::exit(0);
	}

	globalAgent = tinyPiX_sys_create();

	mainAppPanel_ = new mainAppScrollPanel(this);
	if (mainAppPanel_ == nullptr)
	{
		std::cout << "mainAppPanel_ init error!" << std::endl;
		std::exit(0);
	}
	mainAppPanel_->installEventFilter(this);

	carouselButton_ = new tpCarouselButton(this);

	connect(mainAppPanel_, onPageChanged, [=](uint32_t curPage_)
			{ carouselButton_->setCurrentIndex(curPage_); });

	maskWindow_ = new appOperateMaskWindow();
	maskWindow_->installEventFilter(this);

	operateMenu_ = new tpMenu();
	uint32_t delIndex = operateMenu_->addItem("卸载", applicationDirPath() + "/../res/删除.png");
	connect(operateMenu_, onClicked, [=](uint32_t index)
			{
				std::cout << " index " << index << std::endl;
				std::cout << " delIndex " << delIndex << std::endl;

				if (index == delIndex)
				{
					slotDeleteApp(pressAppBtn_);
					// tpMessageBox::information("卸载成功");
					operateMenu_->close();
					maskWindow_->close();
				} });

	appInstallPtr_ = new tpAppInstall("");
	appInstallTimer_ = new tpTimer(800);
	connect(appInstallTimer_, timeout, this, tpDeskScreen::slotTimeoutInstallApp);
}

void tpDeskScreen::intDeskAppConfig()
{
	// 获取所有已安装的APP的UUID列表
	tpVector<tpString> installAppUuidList = tpAppConfigIO::installAppUuidList();

	// 清理布局，重新构建行列
	refreshMainAppPanel();

	// 应用数量为0；后边就不用处理了
	uint32_t appCount = installAppUuidList.size();
	if (appCount == 0)
		return;

	// 解析APP所在页和行列信息
	tpJsonObject appPageInfoJsonObj;

	tpFile appPageInfoFile(applicationDirPath() + "/../conf/appIndexConfig.json");
	appPageInfoFile.open(tpFile::ReadOnly);
	if (appPageInfoFile.isOpen())
	{
		tpString appPageInfoStr = appPageInfoFile.readAll();
		appPageInfoFile.close();

		// 转换json对象
		appPageInfoJsonObj = tpJsonDocument::fromJson(appPageInfoStr).object();
	}

	// 解析在BottomBar中的App
	tpVector<tpString> bottomBarAppUuidList;

	tpFile bottomBarAppInfoFile(applicationDirPath() + "/../conf/bottomBarAppConfig.json");
	bottomBarAppInfoFile.open(tpFile::ReadOnly);
	if (bottomBarAppInfoFile.isOpen())
	{
		tpString botomBarAppInfoJsonStr = bottomBarAppInfoFile.readAll();

		tpJsonObject bottomBarAppJson = tpJsonDocument::fromJson(botomBarAppInfoJsonStr).object();

		tpJsonArray bottomAppIdList = bottomBarAppJson.value("appList").toArray();
		for (int i = 0; i < bottomAppIdList.count(); ++i)
		{
			tpString bottomUuid = bottomAppIdList.at(i).toString();
			bottomBarAppUuidList.emplace_back(bottomUuid);
		}

		bottomBarAppInfoFile.close();
	}

	curPage_ = 0;

	uint32_t pageMaxAppCount = APP_MAX_ROW * APP_MAX_COLUMN;

	// 上一页放不下的APP
	tpList<ApplicationInfoSPtr> cachePageAppList;

	for (int32_t i = 0; i < installAppUuidList.size(); ++i)
	{
		tpString curAppUuid = installAppUuidList.at(i);

		// 查询该应用是否有配置页数和行列信息
		if (appPageInfoJsonObj.contains(curAppUuid))
		{
			tpJsonObject curAppPageInfoObj = appPageInfoJsonObj.value(curAppUuid).toObject();
			uint32_t appPage = curAppPageInfoObj.value("page").toUint();
			uint32_t appIndex = curAppPageInfoObj.value("index").toUint();

			std::cout << "appUuid: " << curAppUuid << "  page: " << appPage << "  index : " << appIndex << std::endl;

			ApplicationInfoSPtr appInfoSPtr = std::make_shared<ApplicationInfo>();
			appInfoSPtr->appUuid = curAppUuid;
			appInfoSPtr->page = appPage;
			appInfoSPtr->index = appIndex;

			auto &curPageAppList = allAppInfoMap_[appPage];

			if (curPageAppList.size() >= pageMaxAppCount)
			{
				cachePageAppList.emplace_back(appInfoSPtr);
			}
			else
			{
				allAppInfoMap_[appPage].insertData(appIndex, appInfoSPtr);
			}
		}
		else if (bottomBarAppUuidList.contains(curAppUuid))
		{
			// 查询该APP是否是放置在bottomBar的
			ApplicationInfoSPtr appInfoSPtr = std::make_shared<ApplicationInfo>();
			appInfoSPtr->appUuid = curAppUuid;
			appInfoSPtr->page = 0;
			appInfoSPtr->index = 0;

			bottomBarAppList_.emplace_back(appInfoSPtr);
			// bottomFloatBar_->addBottomApp(appInfoSPtr);
		}
		else
		{
			// 当前应用，配置文件中没有记录页数和行列信息；放入缓存容器
			ApplicationInfoSPtr appInfoSPtr = std::make_shared<ApplicationInfo>();
			appInfoSPtr->appUuid = curAppUuid;
			appInfoSPtr->page = 0;
			appInfoSPtr->index = 0;
			cachePageAppList.emplace_back(appInfoSPtr);
		}
	}

	// 放置冗余的APP
	for (auto &cacheAppPtr : cachePageAppList)
	{
		uint32_t currentPage = cacheAppPtr->page;

		// 寻找足够空间的页
		while (allAppInfoMap_[currentPage].size() >= pageMaxAppCount)
		{
			++currentPage;
		}

		auto &newPageApps = allAppInfoMap_[currentPage];

		uint32_t findIndex = findAppIndex(newPageApps);

		// 将APP移动到新页
		cacheAppPtr->page = currentPage;
		cacheAppPtr->index = findIndex;
		newPageApps.emplace_back(cacheAppPtr);
	}

	// 计算当前APP共有多少页
	uint32_t pageMaxNum = 0;
	for (const auto &appIter : allAppInfoMap_)
	{
		if (appIter.first > pageMaxNum)
			pageMaxNum = appIter.first;
	}

	mainAppPanel_->setMaxPage(pageMaxNum + 1);
	carouselButton_->setCount(pageMaxNum + 1);

	createAppBtn();

	// 指定页码刷新
	refreshAppPage(curPage_);
}

void tpDeskScreen::createAppBtn()
{
	uint32_t pageMaxAppCount = APP_MAX_ROW * APP_MAX_COLUMN;

	tpString appConfigDirPath = appConfigPathStr_ + APP_CONFIG_SON_PATH;
	tpString appFileDirPath = appConfigPathStr_ + APP_FILES_SON_PATH;

	// APP显示区域的宽度
	uint32_t mainAppPanelWidth = mainAppPanel_->rect().w;
	uint32_t mainAppPanelHeight = mainAppPanel_->rect().h;

	// APP显示区域是沾满了整个窗口的，要偏移预留空间，让APP图标显示在中间区域，与下部Bar对齐
	uint32_t panelHMargin = (mainAppPanelWidth - BOTTOM_BAR_WIDTH) / 2.0;

	// 最后一个按钮的指针，用于获取按钮的宽高
	desktopAppButton *finalBtn = nullptr;

	// 将每一页的APP都添加到滚动窗口，按页码计算X偏移量
	for (const auto &pageAppInfoIter : allAppInfoMap_)
	{
		uint32_t appPage = pageAppInfoIter.first;
		tpList<ApplicationInfoSPtr> curPageAppList = pageAppInfoIter.second;

		for (const auto &appInfoSptrIter : curPageAppList)
		{
			// 解析应用图标、名称信息
			tpAppConfigIO configIO(appInfoSptrIter->appUuid);

			desktopAppButton *appBtn = createDeskAppBtn(appInfoSptrIter, configIO.iconPath(), configIO.appName());
			if (!appBtn)
				continue;
			// appBtn->setInstallProgress(40);
			finalBtn = appBtn;
		}
	}

	// 计算一页里面，所有行列的XY坐标
	if (finalBtn)
	{
		tpVector<int32_t> rowYList;
		tpVector<int32_t> columnXList;

		for (int row = 0; row < APP_MAX_ROW; ++row)
		{
			uint32_t appY = row * (APP_V_INTERVAL + finalBtn->rect().h);
			rowYList.emplace_back(appY);

			if (row == 0)
			{
				for (int column = 0; column < APP_MAX_COLUMN; ++column)
				{
					// pageNum * mainAppPanelWidth + appX
					uint32_t appX = panelHMargin + (column % APP_MAX_COLUMN) * (APP_H_INTERVAL + finalBtn->rect().w);
					columnXList.emplace_back(appX);
				}
			}
		}

		caculateAppPosPtr_.setRowY(rowYList);
		caculateAppPosPtr_.setColumnX(columnXList);
	}

	// 创建工具栏的图标
	tpList<desktopAppButton *> bottomAppList;
	for (const auto &bottomAppInfo : bottomBarAppList_)
	{
		// 解析应用图标、名称信息
		tpAppConfigIO configIO(bottomAppInfo->appUuid);

		desktopAppButton *appBtn = configAppBtn(bottomAppInfo->appUuid, configIO.iconPath(), configIO.appName());
		bottomAppList.emplace_back(appBtn);
	}
	bottomFloatBar_->setBottomAppBtn(bottomAppList);

	mainAppPanel_->recal(true);
}

void tpDeskScreen::refreshMainAppPanel()
{
	uint32_t tHeight = topFloatBar_->height();
	uint32_t bottomBarHeight = bottomFloatBar_->height();

	// 计算APP显示区域大小位置
	uint32_t appDisplayHeight = height() - tHeight - bottomBarHeight;

	// uint32_t appIntervalPx = appDisplayHeight * 0.0279;

	// 减去距离上部Bar和下部bar的距离，在减去bottomBar距离边界距离
	uint32_t mainAppPanelHeight = height() - tHeight - bottomBarHeight - tpDisplay::dp2Px(20) * 2 - tpDisplay::dp2Px(19);

	// uint32_t mainAppPanelHMargin = (width() - APP_PANEL_WIDTH) / 2.0;

	mainAppPanel_->setWidth(rect().w);
	mainAppPanel_->setHeight(mainAppPanelHeight);
	mainAppPanel_->move(0, MAIN_PANEL_TOP_DISTANCE + tHeight);

	mainAppPanel_->setScrollMode(!isLandscapeScreen_);
	mainAppPanel_->setHorizontalScrollBarVisible(false);
	mainAppPanel_->setVerticalScrollBarVisible(false);

	mainAppPanel_->recal(true);
	mainAppPanel_->update();

	uint32_t carouselButtonX = (width() - carouselButton_->width()) / 2.0;
	uint32_t carouselButtonY = mainAppPanel_->pos().y + mainAppPanel_->height() - carouselButton_->height();

	// std::cout << "carouselButtonX " << carouselButtonX << "  carouselButtonY" << carouselButtonY << std::endl;
	// std::cout << "carouselButtonW " << carouselButton_->width() << "  carouselButtonH" << carouselButton_->height() << std::endl;

	carouselButton_->move(carouselButtonX, carouselButtonY);
}

void tpDeskScreen::refreshAppPage(const uint32_t &pageNum)
{
}

void tpDeskScreen::refreshBar()
{
	if (!topFloatBar_ || !bottomFloatBar_)
		return;

	// 计算底部bar的X坐标
	uint32_t bottomX = (width() - BOTTOM_BAR_WIDTH) / 2.0;
	uint32_t bottomY = height() - BOTTOM_BAR_HEIGHT - tpDisplay::dp2Px(19);

	bottomFloatBar_->setRoundCorners(13);

	BAR_SET_ATTRIB(topFloatBar_, tpFixScreen::ITP_FULL_STYLE, 0, 0, width(), TOP_BAR_HEIGHT);
	BAR_SET_ATTRIB(bottomFloatBar_, tpFixScreen::ITP_FULL_STYLE, bottomX, bottomY, BOTTOM_BAR_WIDTH, BOTTOM_BAR_HEIGHT);

	uint32_t navigationX = (width() - navigationFloatBar_->width()) / 2.0;
	BAR_SET_ATTRIB(navigationFloatBar_, tpFixScreen::ITP_FULL_STYLE, navigationX, height() - navigationFloatBar_->height(), navigationFloatBar_->width(), navigationFloatBar_->height());
}

desktopAppButton *tpDeskScreen::createDeskAppBtn(ApplicationInfoSPtr appInfo, const tpString &iconPath, const tpString &appName)
{
	desktopAppButton *appBtn = configAppBtn(appInfo->appUuid, iconPath, appName);
	if (!appBtn)
		return nullptr;

	// APP显示区域的宽度
	uint32_t mainAppPanelWidth = mainAppPanel_->rect().w;
	uint32_t mainAppPanelHeight = mainAppPanel_->rect().h;

	// APP显示区域是沾满了整个窗口的，要偏移预留空间，让APP图标显示在中间区域，与下部Bar对齐
	uint32_t panelHMargin = (mainAppPanelWidth - BOTTOM_BAR_WIDTH) / 2.0;

	uint32_t appIndex = appInfo->index;
	uint32_t appPage = appInfo->page;

	ItpRect iconButtonRect = appBtn->rect();

	uint32_t curAppRow = (appIndex / APP_MAX_COLUMN);

	uint32_t appX = appPage * mainAppPanelWidth + panelHMargin + (appIndex % APP_MAX_COLUMN) * (APP_H_INTERVAL + iconButtonRect.w);
	uint32_t appY = curAppRow * (APP_V_INTERVAL + iconButtonRect.h);

	appBtn->move(appX, appY);

	appBtn->installEventFilter(mainAppPanel_);
	mainAppPanel_->addObject(appBtn);

	return appBtn;
}

desktopAppButton *tpDeskScreen::configAppBtn(const tpString &appUuid, const tpString &iconPath, const tpString &appName)
{
	// 根据APP uuid去查询App配置文件
	desktopAppButton *appBtn = new desktopAppButton(iconPath, appName, nullptr);
	appBtn->setProperty("UUID", appUuid);

	connect(appBtn, onClicked, [=](bool)
			{ startApp(appUuid); });

	connect(appBtn, onLongPress, [=]()
			{ slotOperateApp(appBtn); });

	connect(appBtn, onPressed, [=]()
			{ pressAppBtn_ = appBtn; pressBtnRect_ = appBtn->rect(); });

	appBtn->setEnableBackGroundColor(false);
	appBtn->setEnabledBorderColor(false);
	appBtn->setVisible(true);
	appBtn->setRoundCorners(13);

	appBtn->font()->setFontColor(_RGB(255, 255, 255), _RGB(255, 255, 255));
	appBtn->font()->setFontSize(tpDisplay::sp2Px(14));

	appBtn->setIconSize(APP_WIDTH_HEIGHT, APP_WIDTH_HEIGHT);

	return appBtn;
}

void tpDeskScreen::startApp(const tpString &uuid, const tpVector<tpString> &argList)
{
	// 是否是系统安装包
	if (uuid.compare("SytemAppInstallUuid") == 0)
	{
		if (argList.size() == 0)
		{
			std::cout << "未收到安装文件路径 " << std::endl;
			return;
		}

		installApp(argList.front());
		return;
	}

	tpString appFileDirPath = appConfigPathStr_ + APP_FILES_SON_PATH + uuid;

	tpDir appFileDir(appFileDirPath);
	if (!appFileDir.exists())
	{
		std::cout << "UUid: " << uuid << " 应用文件夹不存在" << std::endl;
		return;
	}

	// 启动对应应用
	if (globalUuidPidMap_.contains(uuid))
	{
		int32_t pid = globalUuidPidMap_.value(uuid);

		// 根据pid查询winid
		PiShmBytes *appIdList = nullptr;
		int appSize = 0;
		tinyPiX_sys_find_win_ids(globalAgent, &appIdList, &appSize, 1);

		int32_t winId = 0;
		for (int i = 0; i < appSize; ++i)
		{
			PiShmBytes appIdInfo = appIdList[i];
			if (appIdInfo.pid == pid)
			{
				winId = appIdInfo.id;
				break;
			}
		}

		std::cout << "恢复应用 pid: " << pid << std::endl;
		tinyPiX_sys_set_visible(globalAgent, winId, true);
		tinyPiX_sys_set_active(globalAgent, winId, true);
	}
	else
	{
		// 解析应用图标、名称信息
		tpAppConfigIO configIO(uuid);

		tpString runnerPath = configIO.runnerPath();
		tpFileInfo runnerFileInfo(runnerPath);
		if (!runnerFileInfo.exists())
		{
			std::cout << "应用 " << configIO.appName() << " 可执行程序不存在!" << std::endl;
			return;
		}

		tpProcess exeProcess;
		exeProcess.start(runnerPath, argList);
		// exeProcess.start(exePathStr);
		int32_t processPID = exeProcess.launchProcessID();

		RunAppInfo runAppInfo;
		runAppInfo.appName = configIO.appName();
		runAppInfo.appUuid = uuid;
		runAppInfo.appIconPath = configIO.iconPath();
		runAppInfo.pid = processPID;

		std::cout << "processPID " << processPID << std::endl;
		std::lock_guard<std::mutex> lock_g(readRunAppMutex_);
		globalRunAppMap_[processPID] = runAppInfo;
		globalUuidPidMap_[uuid] = processPID;
	}
}

void tpDeskScreen::installApp(const tpString &pkgPath)
{
	// 应用安装
	appInstallPtr_->setPath(pkgPath);
	tpString installAppUuid = appInstallPtr_->getAppUUID();

	// 判断应用是否已安装
	tpVector<tpString> instasllAppIDList = tpAppConfigIO::installAppUuidList();
	if (instasllAppIDList.contains(installAppUuid))
	{
		tpMessageBox::information("应用已安装!");
		return;
	}

	// 查找应用安装的桌面索引
	int32_t installPageNum = -1;
	uint32_t installIndex = 0;

	uint32_t pageMaxAppCount = APP_MAX_ROW * APP_MAX_COLUMN;
	for (const auto &pageAppInfoIter : allAppInfoMap_)
	{
		// 当前页应用已放置满
		if (pageAppInfoIter.second.size() >= pageMaxAppCount)
			continue;

		installPageNum = pageAppInfoIter.first;
		installIndex = findAppIndex(pageAppInfoIter.second);
		break;
	}

	// 所有页都被装满了，开辟新页
	if (installPageNum == -1)
	{
		installPageNum = 0;

	ReFindPageNum:
		for (const auto &pageAppInfoIter : allAppInfoMap_)
		{
			if (pageAppInfoIter.first == installPageNum)
			{
				installPageNum++;
				goto ReFindPageNum;
			}
		}
	}

	appInstallPtr_->install();

	std::cout << "开始安装应用： " << installAppUuid << std::endl;

	ApplicationInfoSPtr installAppInfp = tpMakeShared<ApplicationInfo>();
	installAppInfp->appUuid = installAppUuid;
	installAppInfp->index = installIndex;
	installAppInfp->page = installPageNum;

	allAppInfoMap_[installPageNum].emplace_back(installAppInfp);

	// 桌面添加应用图标以及遮罩层
	tpString iconPath = appInstallPtr_->getIcon();
	tpString appName = appInstallPtr_->getAppName();

	std::cout << "Install iconPath" << iconPath << std::endl;
	std::cout << "Install appName" << appName << std::endl;

	installingApp_ = createDeskAppBtn(installAppInfp, iconPath, appName);
	installingApp_->setInstallProgress(0);

	// 启动定时器更新安装进度
	appInstallTimer_->start();
}

uint32_t tpDeskScreen::findAppIndex(const tpList<ApplicationInfoSPtr> &pageAppList)
{
	uint32_t findIndex = 0;
	if (pageAppList.size() == 0)
		return findIndex;

	// 收集所有已使用的索引值
	std::set<uint32_t> usedIndices;
	for (const auto &appInfo : pageAppList)
	{
		usedIndices.insert(appInfo->index);
	}

	// 查找最小未使用索引
	while (true)
	{
		// 如果当前候选值不在已使用索引中，即找到最小值
		if (usedIndices.find(findIndex) == usedIndices.end())
		{
			return findIndex;
		}
		findIndex++;

		// 防止整数溢出（虽然实际场景中几乎不可能）
		if (findIndex == std::numeric_limits<uint32_t>::max())
		{
			break;
		}
	}

	return findIndex;
}
