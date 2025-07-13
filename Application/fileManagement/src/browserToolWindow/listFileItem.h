
/*
    浏览-文件列表模式下的item
*/

#ifndef __LIST_FILE_ITEM_H
#define __LIST_FILE_ITEM_H

#include "tpChildWidget.h"
#include "tpLabel.h"
#include "tpSignalSlot.h"

class listFileItem
    : public tpChildWidget
{
public:
    listFileItem(tpChildWidget *parent = nullptr);
    ~listFileItem();

    /// @brief 设置文件名称
    /// @param name 文件名字符串
    void setName(const tpString &name);

    /// @brief 设置文件绝对路径
    /// @param filePath
    void setPath(const tpString &filePath);

    /// @brief 获取当前item对应的文件/文件夹绝对路径
    /// @return
    tpString path();

public
signals:
    /// @brief 点击信号，鼠标释放时触发
    /// @param tpCollapsibleFileItem* 点击的item
    declare_signal(onClicked, listFileItem *);

protected:
    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;
    virtual bool eventFilter(tpObject *watched, tpEvent *event) override;

private:
    tpLabel *iconLabel_;
    tpLabel *nameLabel_;

    // 存放大小或文件夹子项
    tpLabel *sizeLabel_;
    tpLabel *dateTimeLabel_;

    tpString filePath_;

    // 是否触发item事件，如果鼠标按下后拖动，不再处理事件
    bool isTrigger = true;
};

#endif