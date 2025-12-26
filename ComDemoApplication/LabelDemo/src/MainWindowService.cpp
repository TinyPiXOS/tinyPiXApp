#include "MainWindowService.h"
#include "TpLabel.h"
#include "TpFont.h"
#include "TpImage.h"
#include <TpDesktopAPI.h>
#include <TpEvent.h>

MainWindowService::MainWindowService()
    : TpDesktopMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(226, 226, 226));
    TpDesktopAPI::Instance()->setStatusBarStyle(_RGB(226, 226, 226));

    TpLabel *label1 = new TpLabel(this);
    label1->font()->setFontColor(_RGB(255, 0, 0));
    label1->setText("默认字体");
    label1->setRect(50, 50, 250, 30);

    TpLabel *label2 = new TpLabel(this);
    label2->font()->setFontColor(_RGB(0, 255, 0));
    label2->font()->setBold(true);
    label2->setAlign(Tp::AlignCenter);
    label2->setText("加粗字体");
    label2->setRect(50, 80, 250, 30);

    TpLabel *label3 = new TpLabel(this);
    label3->font()->setFontColor(_RGB(0, 255, 0));
    label3->font()->setItalic(true);
    label3->setAlign(Tp::AlignRight);
    label3->setText("斜体");
    label3->setRect(50, 110, 250, 30);

    TpLabel *label6 = new TpLabel(this);
    label6->setBackGroundImage(TpImage(applicationDirPath() + "/test.svg"));
    label6->setRect(300, 300, 300, 300);
}

MainWindowService::~MainWindowService()
{
}

bool MainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "LabelDemo::appChange" << std::endl;

    return true;
}

bool MainWindowService::onVisibleEvent(TpVisibleEvent *event)
{
    if (event->visible())
    {
        TpDesktopAPI::Instance()->setStatusBarStyle(_RGB(226, 226, 226));
    }

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "LabelDemo::onActiveEvent" << std::endl;

    return true;
}
