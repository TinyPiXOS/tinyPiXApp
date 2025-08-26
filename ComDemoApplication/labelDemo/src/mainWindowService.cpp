#include "mainWindowService.h"
#include "TpLabel.h"
#include "TpFont.h"
#include "TpImage.h"

mainWindowService::mainWindowService()
    : TpFixScreen()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(226, 226, 226));
    
    TpLabel *label1 = new TpLabel(this);
	label1->font()->setFontColor(_RGBA(255, 0, 0, 255), 0);
	label1->font()->setAntialias(TpFont::TINY_FONT_ANTIALIAS_BEST);
	label1->font()->setFontWeight(TpFont::TINY_FONT_WEIGHT_THIN);
	label1->setText("TINY_FONT_WEIGHT_THIN");
	label1->setRect(0, 0, 250, 30);
	label1->setVisible(true);
	label1->update();

	TpLabel *label2 = new TpLabel(this);
	label2->font()->setFontColor(_RGBA(0, 255, 0, 255), 0);
	label2->font()->setAntialias(TpFont::TINY_FONT_ANTIALIAS_GOOD);
	label1->font()->setFontWeight(TpFont::TINY_FONT_WEIGHT_ULTRALIGHT);
	label2->setAlign(tinyPiX::AlignCenter);
	label2->setText("TINY_FONT_WEIGHT_ULTRALIGHT");
	label2->setRect(0, 30, 250, 30);
	label2->setVisible(true);
	label2->update();

	TpLabel *label3 = new TpLabel(this);
	// label3->font()->setFontColor(_RGBA(0, 0, 255, 255), 0);
	label3->font()->setAntialias(TpFont::TINY_FONT_ANTIALIAS_GOOD);
	label1->font()->setFontWeight(TpFont::TINY_FONT_WEIGHT_LIGHT);
	label3->setAlign(tinyPiX::AlignRight);
	label3->setText("TINY_FONT_WEIGHT_LIGHT");
	label3->setRect(0, 60, 250, 30);
	label3->setVisible(true);
	label3->update();

	TpLabel *label4 = new TpLabel(this);
	// label4->font()->setFontSize(50);
	label1->font()->setFontWeight(TpFont::TINY_FONT_WEIGHT_SEMILIGHT);
	label4->setText("TINY_FONT_WEIGHT_SEMILIGHT");
	label4->setRect(0, 90, 250, 30);
	label4->setVisible(true);
	label4->update();

	TpList<TpFont::TpFontWeight> fontWeightValue =
		{
			TpFont::TINY_FONT_WEIGHT_BOOK,
			TpFont::TINY_FONT_WEIGHT_NORMAL,
			TpFont::TINY_FONT_WEIGHT_MEDIUM,
			TpFont::TINY_FONT_WEIGHT_SEMIBOLD,
			TpFont::TINY_FONT_WEIGHT_BOLD,
			TpFont::TINY_FONT_WEIGHT_ULTRABOLD,
			TpFont::TINY_FONT_WEIGHT_HEAVY,
			TpFont::TINY_FONT_WEIGHT_ULTRAHEAVY};
	TpList<TpString> fontWeightText =
		{
			"TINY_FONT_WEIGHT_BOOK",
			"TINY_FONT_WEIGHT_NORMAL",
			"TINY_FONT_WEIGHT_MEDIUM",
			"TINY_FONT_WEIGHT_SEMIBOLD",
			"TINY_FONT_WEIGHT_BOLD",
			"TINY_FONT_WEIGHT_ULTRABOLD",
			"TINY_FONT_WEIGHT_HEAVY",
			"TINY_FONT_WEIGHT_ULTRAHEAVY"};
	for (int i = 0; i < fontWeightValue.size(); ++i)
	{
		TpLabel *labelTmp = new TpLabel(this);
		labelTmp->font()->setFontWeight(fontWeightValue.at(i));
		labelTmp->setText(fontWeightText.at(i));
		labelTmp->setRect(0, 120 + i * 30, 250, 30);
		labelTmp->setVisible(true);
		labelTmp->update();
	}

	TpLabel *label6 = new TpLabel(this);
	label6->setBackGroundImage(TpImage(applicationDirPath() + "/../res/test.svg"));
	// label6->setBackGroundColor(_RGB(255, 0, 0));
	// label6->setBlurRadius(20);
	label6->setRect(300, 300, 300, 300);
	// label6->setRoundCorners(100);
	label6->setVisible(true);
	label6->update();
}

mainWindowService::~mainWindowService()
{
}

bool mainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "systemSetting::appChange" << std::endl;

    return true;
}

bool mainWindowService::onResizeEvent(TpObjectResizeEvent *event)
{
    std::cout << "systemSetting::onResizeEvent" << std::endl;

    return true;
}

bool mainWindowService::onActiveEvent(TpObjectActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    return true;
}
