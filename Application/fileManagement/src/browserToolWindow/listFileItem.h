
/*
    浏览-文件列表模式下的item
*/

#ifndef __LIST_FILE_ITEM_H
#define __LIST_FILE_ITEM_H

#include "TpChildWidget.h"
#include "TpLabel.h"
#include "TpSignalSlot.h"

class listFileItem
    : public TpChildWidget
{
public:
    listFileItem(TpChildWidget *parent = nullptr);
    ~listFileItem();

    /// @brief 设置文件名称
    /// @param name 文件名字符串
    void setName(const TpString &name);

    /// @brief 设置文件绝对路径
    /// @param filePath
    void setPath(const TpString &filePath);

    /// @brief 获取当前item对应的文件/文件夹绝对路径
    /// @return
    TpString path();

public
signals:
    /// @brief 点击信号，鼠标释放时触发
    /// @param TpCollapsibleFileItem* 点击的item
    declare_signal(onClicked, listFileItem *);

protected:
    virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;
    virtual bool onResizeEvent(TpObjectResizeEvent *event) override;
    virtual bool eventFilter(TpObject *watched, TpEvent *event) override;

private:
    TpLabel *iconLabel_;
    TpLabel *nameLabel_;

    // 存放大小或文件夹子项
    TpLabel *sizeLabel_;
    TpLabel *dateTimeLabel_;

    TpString filePath_;

    // 是否触发item事件，如果鼠标按下后拖动，不再处理事件
    bool isTrigger = true;
};

#endif