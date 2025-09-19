#include "listFileItem.h"
#include "TpFileInfo.h"
#include "TpImage.h"
#include "TpVBoxLayout.h"
#include "TpHBoxLayout.h"
#include "TpDir.h"
#include "TpFont.h"
#include "TpEvent.h"

TpString parseFileSuffix(const TpString &suffix, const bool &isDir)
{
    if (isDir)
        return "文件夹";

    if (suffix.compare("txt") == 0)
        return "文本文件";
    else if (suffix.compare("exe") == 0)
        return "程序";
    else if (suffix.compare("doc") == 0 || suffix.compare("docx") == 0)
        return "word文档";
    else if (suffix.compare("xls") == 0 || suffix.compare("xlsx") == 0)
        return "excel表格";
    else if (suffix.compare("ppt") == 0 || suffix.compare("pptx") == 0)
        return "PPT";
    else if (suffix.compare("mp3") == 0)
        return "音频文件";
    else if (suffix.compare("mp4") == 0 || suffix.compare("avi") == 0)
        return "视频文件";
    else if (suffix.compare("zip") == 0 || suffix.compare("rar") == 0 || suffix.compare("7z") == 0)
        return "压缩包";
    else if (suffix.compare("png") == 0 || suffix.compare("jpg") == 0 || suffix.compare("jpeg") == 0)
        return "图片";
    else
        return "未知";
}

TpString parseIconPath(const TpString &suffix, const bool &isDir)
{
    TpString resPath = "/usr/res/TinyPiX/fileIcon/";

    // return resPath + "pdf.png";

    if (isDir)
        return resPath + "文件夹.png";

    if (suffix.compare("txt") == 0)
        return resPath + "文本.png";
    else if (suffix.compare("exe") == 0)
        return resPath + "程序.png";
    else if (suffix.compare("doc") == 0 || suffix.compare("docx") == 0)
        return resPath + "word.png";
    else if (suffix.compare("xls") == 0 || suffix.compare("xlsx") == 0)
        return resPath + "excel.png";
    else if (suffix.compare("ppt") == 0 || suffix.compare("pptx") == 0)
        return resPath + "ppt.png";
    else if (suffix.compare("mp3") == 0)
        return resPath + "音频.png";
    else if (suffix.compare("mp4") == 0 || suffix.compare("avi") == 0)
        return resPath + "视频.png";
    else if (suffix.compare("zip") == 0 || suffix.compare("rar") == 0 || suffix.compare("7z") == 0)
        return resPath + "压缩包.png";
    else if (suffix.compare("png") == 0 || suffix.compare("jpg") == 0 || suffix.compare("jpeg") == 0)
        return resPath + "图片.png";
    else
        return resPath + "未知.png";
}

listFileItem::listFileItem(TpChildWidget *parent)
    : TpChildWidget(parent)
{
    iconLabel_ = new TpLabel(this);
    iconLabel_->setFixedSize(50, 50);

    nameLabel_ = new TpLabel(this);
    nameLabel_->font()->setFontSize(11);
    nameLabel_->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    nameLabel_->installEventFilter(this);

    sizeLabel_ = new TpLabel(this);
    sizeLabel_->font()->setFontSize(9);
    sizeLabel_->font()->setFontColor(_RGB(140, 140, 140), _RGB(140, 140, 140));
    sizeLabel_->installEventFilter(this);

    dateTimeLabel_ = new TpLabel(this);
    dateTimeLabel_->font()->setFontSize(11);
    dateTimeLabel_->font()->setFontColor(_RGB(89, 89, 89), _RGB(89, 89, 89));
    dateTimeLabel_->installEventFilter(this);

    TpVBoxLayout *nameLayout = new TpVBoxLayout();
    nameLayout->setSpacing(0);
    nameLayout->addWidget(nameLabel_);
    nameLayout->addWidget(sizeLabel_);

    TpHBoxLayout *mainLayout = new TpHBoxLayout();
    mainLayout->setSpacing(15);

    mainLayout->addWidget(iconLabel_);
    mainLayout->addLayout(nameLayout);
    mainLayout->addSpacer(new TpSpacerItem(20, 20, TpSpacerItem::Expanding));
    mainLayout->addWidget(dateTimeLabel_);

    setLayout(mainLayout);
}

listFileItem::~listFileItem()
{
}

void listFileItem::setName(const TpString &name)
{
    nameLabel_->setText(name);
    update();
}

void listFileItem::setPath(const TpString &filePath)
{
    filePath_ = filePath;

    // 解析文件类型，获取资源图片
    TpFileInfo fileInfo(filePath);

    bool isDir = false;
    TpString suffix = "";
    if (fileInfo.isDir())
    {
        isDir = true;

        TpDir pathDir(filePath);
        sizeLabel_->setText(TpString::number(pathDir.entryInfoList().size()) + "项");
    }
    else
    {
        int32_t findSuffixIndex = filePath.rfind(".");
        suffix = filePath.mid(findSuffixIndex + 1);

        sizeLabel_->setText(TpString::number(fileInfo.size()) + "Kb");
    }

    // 设置文件最后更新时间
    dateTimeLabel_->setText(fileInfo.lastModified("%Y-%m-%d %H:%M"));

    // TpString typeStr = parseFileSuffix(suffix, isDir);
    TpString iconPath = parseIconPath(suffix, isDir);

    // typeLabel_->setText(typeStr);

    iconLabel_->setBackGroundImage(TpImage(iconPath));

    update();
}

TpString listFileItem::path()
{
    return filePath_;
}

bool listFileItem::onMouseRleaseEvent(TpMouseEvent *event)
{
    TpChildWidget::onMouseRleaseEvent(event);

    onClicked.emit(this);

    return true;
}

bool listFileItem::onResizeEvent(TpResizeEvent *event)
{
    TpChildWidget::onResizeEvent(event);

    // iconLabel_->setFixedSize(height() * 0.666, height() * 0.666);

    // uint32_t iconY = (height() - iconLabel_->height()) / 2.0;
    // iconLabel_->move(25, iconY);

    return true;
}

bool listFileItem::eventFilter(TpObject *watched, TpEvent *event)
{
    if (event->eventType() == TpEvent::EVENT_MOUSE_RELEASE_TYPE)
    {
        TpMouseEvent *mouseEvent = dynamic_cast<TpMouseEvent *>(event);
        onMouseRleaseEvent(mouseEvent);
    }

    return false;
}
