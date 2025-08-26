#ifndef MAIN_APP_SCROLL_PANEL
#define MAIN_APP_SCROLL_PANEL

#include "TpScrollPanel.h"
#include "TpEvent.h"

/// @brief
class mainAppScrollPanel
    : public TpScrollPanel
{
public:
    mainAppScrollPanel(TpChildWidget *parent = nullptr);
    ~mainAppScrollPanel();

    void setMaxPage(const uint32_t &maxPage);

    void setPage(const uint32_t &page);

    // APP移动时，绘制原本位置的圆角框
    void setMoveAppRect(const ItpRect &rect, const int32_t &round);

public
signals:
    // 页码切换事件,下标从0开始
    declare_signal(onPageChanged, uint32_t);

    /// @brief 长按信号
    declare_signal(onLongPress);

protected:
    virtual bool eventFilter(TpObject *watched, TpEvent *event) override;

    virtual bool onMousePressEvent(TpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;
    virtual bool onMouseLongPressEvent(TpMouseEvent *event) override;
    virtual bool onWheelEvent(TpWheelEvent *event) override;

    virtual bool onMouseMoveEvent(TpMouseEvent *event) override;
	virtual bool onLeaveEvent(TpObjectLeaveEvent *event) override;

private:
    bool mouseLeftPress_;

    // 鼠标点击坐标
    ItpPoint originPressPoint_;
    // 鼠标点击时的偏移量
    int32_t originPressPos_;

    // 滑动翻页时缓存坐标
    ItpPoint updatePoint_;

    // 最大页数
    uint32_t maxPageCount_;

    // 触摸从右往左划还是从左往右滑
    bool isLeftRoll_;
    // 是否切换页面
    bool isSwitchPage_;
};

#endif