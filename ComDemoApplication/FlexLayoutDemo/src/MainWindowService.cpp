#include "MainWindowService.h"
#include "TpFlexLayout.h"
#include "TpButton.h"

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));
    
    TpFlexLayout *flexLayout = new TpFlexLayout();

	/// @brief 设置布局方向
	flexLayout->setFlexDirection(TpFlexLayout::Row);
	// flexLayout->setFlexDirection(TpFlexLayout::RowReverse);
	// flexLayout->setFlexDirection(TpFlexLayout::Column);
	// flexLayout->setFlexDirection(TpFlexLayout::ColumnReverse);

	/// @brief 设置主轴对齐方式
	// flexLayout->setJustifyContent(TpFlexLayout::MainFlexStart);   // 默认, 从主轴 开始位置 开始
	// flexLayout->setJustifyContent(TpFlexLayout::MainFlexEnd);      // 从主轴 结束位置 开始
	// flexLayout->setJustifyContent(TpFlexLayout::MainCenter);       // 居中对齐
	flexLayout->setJustifyContent(TpFlexLayout::MainSpaceBetween); // 两端对齐,两边贴着
	// flexLayout->setJustifyContent(TpFlexLayout::MainSpaceEvenly);  // 分散对齐,间距相同
	// flexLayout->setJustifyContent(TpFlexLayout::MainSpaceAround);   // 分散对齐,两边是中间的一半

	/// @brief 设置交叉轴对齐方式
	// flexLayout->setAlignItems(TpFlexLayout::CrossFlexStart); // 顶部对齐
	flexLayout->setAlignItems(TpFlexLayout::CrossFlexEnd);   // 底部对齐
	// flexLayout->setAlignItems(TpFlexLayout::CrossCenter);     // 居中对齐

	/// @brief 设置多行交叉轴对齐方式，注意设置后会覆盖交叉轴对齐样式
	// flexLayout->setAlignContent(TpFlexLayout::Stretch);      // 默认,拉伸，与交叉轴对齐
	// flexLayout->setAlignContent(TpFlexLayout::FlexStart);    // 顶部对齐
	flexLayout->setAlignContent(TpFlexLayout::FlexEnd);      // 底部对齐
	// flexLayout->setAlignContent(TpFlexLayout::Center);       // 居中对齐
	// flexLayout->setAlignContent(TpFlexLayout::SpaceBetween); // 分散对齐,上下贴着
	// flexLayout->setAlignContent(TpFlexLayout::SpaceEvenly);  //  分散对齐,上中下平均
	// flexLayout->setAlignContent(TpFlexLayout::SpaceAround);   // 分散对齐,上下是中一半

	for (int i = 0; i < 8; ++i)
	{
		TpButton *testButton = new TpButton("测试按钮" + TpString::number(i));

		testButton->setSize(150, 60);

		// testButton->setBackGroundColor(_RGB(255, 15, 15));

		flexLayout->addWidget(testButton);
	}

	setLayout(flexLayout);
}

MainWindowService::~MainWindowService()
{
}

bool MainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "systemSetting::appChange" << std::endl;

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    return true;
}
