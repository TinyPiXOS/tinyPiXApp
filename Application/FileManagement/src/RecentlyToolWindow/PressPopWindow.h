
/*
    最近-长按触发菜单窗口
*/

#ifndef __PRESS_POP_WINDOW_H
#define __PRESS_POP_WINDOW_H

#include "TpDialog.h"
#include "TpVBoxLayout.h"
#include "TpIconTopButton.h"
#include "TpMenu.h"
#include "TpVector.h"

class PressPopWindow
    : public TpDialog
{
public:
    PressPopWindow();
    ~PressPopWindow();

private:
    void init();

protected:
    virtual bool onResizeEvent(TpResizeEvent *event) override;

    virtual bool eventFilter(TpObject *watched, TpEvent *event) override;

private:
    // 选中item后的操作按钮
    TpIconTopButton *delSelectBtn_;
    TpIconTopButton *moveSelectBtn_;
    TpIconTopButton *moreSelectBtn_;

    TpVector<TpIconTopButton *> buttonList_;

    TpMenu *moreConfigMenu_;
};

#endif