#include "listFileItem.h"
#include "tpFileInfo.h"
#include "tpSurface.h"
#include "tpVBoxLayout.h"
#include "tpHBoxLayout.h"
#include "tpDir.h"
#include "tpFont.h"
#include "tpEvent.h"

tpString parseFileSuffix(const tpString &suffix, const bool &isDir)
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

tpString parseIconPath(const tpString &suffix, const bool &isDir)
{
    tpString resPath = "/usr/res/tinyPiX/fileIcon/";

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

listFileItem::listFileItem(tpChildWidget *parent)
    : tpChildWidget(parent)
{
    iconLabel_ = new tpLabel(this);
    iconLabel_->setFixedSize(50, 50);

    nameLabel_ = new tpLabel(this);
    nameLabel_->font()->setFontSize(11);
    nameLabel_->font()->setFontColor(_RGB(38, 38, 38), _RGB(38, 38, 38));
    nameLabel_->installEventFilter(this);

    sizeLabel_ = new tpLabel(this);
    sizeLabel_->font()->setFontSize(9);
    sizeLabel_->font()->setFontColor(_RGB(140, 140, 140), _RGB(140, 140, 140));
    sizeLabel_->installEventFilter(this);

    dateTimeLabel_ = new tpLabel(this);
    dateTimeLabel_->font()->setFontSize(11);
    dateTimeLabel_->font()->setFontColor(_RGB(89, 89, 89), _RGB(89, 89, 89));
    dateTimeLabel_->installEventFilter(this);

    tpVBoxLayout *nameLayout = new tpVBoxLayout();
    nameLayout->setSpacing(0);
    nameLayout->addWidget(nameLabel_);
    nameLayout->addWidget(sizeLabel_);

    tpHBoxLayout *mainLayout = new tpHBoxLayout();
    mainLayout->setSpacing(15);

    mainLayout->addWidget(iconLabel_);
    mainLayout->addLayout(nameLayout);
    mainLayout->addSpacer(new tpSpacerItem(20, 20, tpSpacerItem::Expanding));
    mainLayout->addWidget(dateTimeLabel_);

    setLayout(mainLayout);
}

listFileItem::~listFileItem()
{
}

void listFileItem::setName(const tpString &name)
{
    nameLabel_->setText(name);
    update();
}

void listFileItem::setPath(const tpString &filePath)
{
    filePath_ = filePath;

    // 解析文件类型，获取资源图片
    tpFileInfo fileInfo(filePath);

    bool isDir = false;
    tpString suffix = "";
    if (fileInfo.isDir())
    {
        isDir = true;

        tpDir pathDir(filePath);
        sizeLabel_->setText(tpString::number(pathDir.entryInfoList().size()) + "项");
    }
    else
    {
        int32_t findSuffixIndex = filePath.rfind(".");
        suffix = filePath.mid(findSuffixIndex + 1);

        sizeLabel_->setText(tpString::number(fileInfo.size()) + "Kb");
    }

    // 设置文件最后更新时间
    dateTimeLabel_->setText(fileInfo.lastModified("%Y-%m-%d %H:%M"));

    // tpString typeStr = parseFileSuffix(suffix, isDir);
    tpString iconPath = parseIconPath(suffix, isDir);

    // typeLabel_->setText(typeStr);

    tpShared<tpSurface> iconSurface = tpMakeShared<tpSurface>();
    iconSurface->fromFile(iconPath);
    iconLabel_->setBackGroundImage(iconSurface);

    update();
}

tpString listFileItem::path()
{
    return filePath_;
}

bool listFileItem::onMouseRleaseEvent(tpMouseEvent *event)
{
    tpChildWidget::onMouseRleaseEvent(event);

    onClicked.emit(this);

    return true;
}

bool listFileItem::onResizeEvent(tpObjectResizeEvent *event)
{
    tpChildWidget::onResizeEvent(event);

    // iconLabel_->setFixedSize(height() * 0.666, height() * 0.666);

    // uint32_t iconY = (height() - iconLabel_->height()) / 2.0;
    // iconLabel_->move(25, iconY);

    return true;
}

bool listFileItem::eventFilter(tpObject *watched, tpEvent *event)
{
    if (event->eventType() == tpEvent::EVENT_MOUSE_RELEASE_TYPE)
    {
        tpMouseEvent *mouseEvent = dynamic_cast<tpMouseEvent *>(event);
        onMouseRleaseEvent(mouseEvent);
    }

    return false;
}
