
/*
    最近-清理存储空间窗口
*/

#ifndef __CLEAR_SPACE_WINDOW_H
#define __CLEAR_SPACE_WINDOW_H

#include "TpDialog.h"
#include "TpVBoxLayout.h"
#include "TpButton.h"
#include "TpPercentProgressBar.h"

class clearSpaceWindow
    : public TpDialog
{
public:
    clearSpaceWindow();
    ~clearSpaceWindow();

private:
    // 清理垃圾文件
    void SlotClearSpaceFile(bool);

private:
    void init();

    // 构建本地空间子窗口
    TpChildWidget *genLocalSpaceWidget();

    // 生成清理空间子窗口
    TpChildWidget *genClearFileWidget();

private:
    // 返回上层按钮
    TpButton *backBtn_;

    TpButton *releaseSpaceBtn_;
};

#endif