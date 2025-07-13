
/*
    最近-清理存储空间窗口
*/

#ifndef __CLEAR_SPACE_WINDOW_H
#define __CLEAR_SPACE_WINDOW_H

#include "tpDialog.h"
#include "tpVBoxLayout.h"
#include "tpButton.h"
#include "tpPercentProgressBar.h"

class clearSpaceWindow
    : public tpDialog
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
    tpChildWidget *genLocalSpaceWidget();

    // 生成清理空间子窗口
    tpChildWidget *genClearFileWidget();

private:
    // 返回上层按钮
    tpButton *backBtn_;

    tpButton *releaseSpaceBtn_;
};

#endif