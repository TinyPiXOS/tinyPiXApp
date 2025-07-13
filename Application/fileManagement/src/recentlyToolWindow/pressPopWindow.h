
/*
    最近-长按触发菜单窗口
*/

#ifndef __PRESS_POP_WINDOW_H
#define __PRESS_POP_WINDOW_H

#include "tpDialog.h"
#include "tpVBoxLayout.h"
#include "tpIconTopButton.h"
#include "tpMenu.h"
#include "tpVector.h"

class pressPopWindow
    : public tpDialog
{
public:
    pressPopWindow();
    ~pressPopWindow();

private:
    void init();

protected:
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

    virtual bool eventFilter(tpObject *watched, tpEvent *event) override;

private:
    // 选中item后的操作按钮
    tpIconTopButton *delSelectBtn_;
    tpIconTopButton *moveSelectBtn_;
    tpIconTopButton *moreSelectBtn_;

    tpVector<tpIconTopButton *> buttonList_;

    tpMenu *moreConfigMenu_;
};

#endif