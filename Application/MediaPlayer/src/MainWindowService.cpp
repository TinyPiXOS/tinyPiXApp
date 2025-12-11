#include "MainWindowService.h"
#include "TpSignalSlot.h"
#include "TpHBoxLayout.h"
#include "SystemInfo/TpDisplay.h"
#include "TpLabel.h"
#include "TpLine.h"
#include "TpFont.h"
#include "TpImage.h"
#include "TpFileInfo.h"
#include <Service/TpAppManager.h>
#include "TpEvent.h"

MainWindowService::MainWindowService()
    : TpMainWindow(), topBar_(new StatusBar()), bottomBar_(new BottomBar()), fileType_(UnknowFile), videoPlayer_(nullptr)
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    initUi();

    // 音频文件，设置背景色；和音频icon
    isMusicIconLabel_->setVisible(true);
    isMusicIconLabel_->move((width() - isMusicIconLabel_->width()) / 2.0, (height() - isMusicIconLabel_->height()) / 2.0);
    setBackGroundColor(_RGB(78, 78, 78));
    TpAppManager::Instance()->setStatusBarStyle(_RGB(78, 78, 78));

    topBar_->setFileName("");

    // 测试
    // setPlayFile(applicationDirPath() + "/../res/demo.mp4");
    // setPlayFile(applicationDirPath() + "/../res/千千阙歌.mp3");
}

MainWindowService::~MainWindowService()
{
}

void MainWindowService::setPlayFile(const TpString &filePath)
{
    TpFileInfo inputFile(filePath);
    if (!inputFile.exists())
    {
        std::cout << "文件不存在：" << filePath << std::endl;
        isMusicIconLabel_->setVisible(true);
        isMusicIconLabel_->move((width() - isMusicIconLabel_->width()) / 2.0, (height() - isMusicIconLabel_->height()) / 2.0);
        setBackGroundColor(_RGB(78, 78, 78));

        return;
    }

    TpString fileSuffix = inputFile.suffix();
    fileType_ = checkFileType(fileSuffix);

    if (fileType_ == UnknowFile)
    {
        std::cout << "暂不支持的文件格式：" << fileSuffix << std::endl;
        return;
    }
    else if (fileType_ == AudioFile)
    {
        // 音频文件，设置背景色；和音频icon
        isMusicIconLabel_->setVisible(true);
        isMusicIconLabel_->move((width() - isMusicIconLabel_->width()) / 2.0, (height() - isMusicIconLabel_->height()) / 2.0);
        setBackGroundColor(_RGB(78, 78, 78));

        // 启动音频设备
        // audioPlayer_->addFile(filePath.c_str());

        // if (!audioPlayer_->isOpen())
            // audioPlayer_->openDevice();

        audioPlayer_->setVolume(100);
        // audioPlayer_->playStart();

        std::cout << "播放文件： " << filePath << std::endl;

        updateProgressTimer_->start();
    }
    else if (fileType_ == VideoFile)
    {
        isMusicIconLabel_->setVisible(false);

        // 设置视频第一帧图片
        setBackGroundImage(TpImage(applicationDirPath() + "/../res/测试视频封面.jpg"));

        // 启动视频设备
        videoPlayer_->setWindowSize(width(), height());

        // videoPlayer_->addFile(filePath.c_str());

        // if (!videoPlayer_->isOpen())
        // {
        //     std::cout << "Video Open Device" << std::endl;
        //     videoPlayer_->openDevice();
        // }

        // videoPlayer_->setVolume(100);
        // videoPlayer_->playStart();

        updateProgressTimer_->start();
    }
    else
    {
    }

    // 设置文件名称
    TpString fileBaseName = inputFile.fileName();
    topBar_->setFileName(fileBaseName);

    update();
}

void MainWindowService::setBackBtnVisible(const bool &visible)
{
    topBar_->setBackBtnVisible(visible);
}

bool MainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "MainWindowService::appChange" << std::endl;

    refreshBarSize();

    return true;
}

bool MainWindowService::onVisibleEvent(TpVisibleEvent *event)
{
    if (event->visible())
    {
        TpAppManager::Instance()->setStatusBarStyle(_RGB(78, 78, 78));
    }

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "MainWindowService::onActiveEvent" << std::endl;

    return true;
}

void MainWindowService::initUi()
{
    refreshBarSize();

    isMusicIconLabel_ = new TpLabel(this);
    isMusicIconLabel_->setFixedSize(TpDisplay::dp2Px(100), TpDisplay::dp2Px(100));
    isMusicIconLabel_->setVisible(false);
    isMusicIconLabel_->setRoundCorners(TpDisplay::dp2Px(50));
    isMusicIconLabel_->setBackGroundImage(TpImage(applicationDirPath() + "/../res/音频.png"));

    connect(bottomBar_, medioOperate, this, &MainWindowService::slotOperateMedia);
    connect(bottomBar_, alterPostion, this, &MainWindowService::slotSwitchPos);
    connect(bottomBar_, switchSpeed, this, &MainWindowService::slotSwitchSpeed);

    audioPlayer_ = new TpAudioInterface("hw:1,0");
    videoPlayer_ = new TpVideoInterface();

    TpVideoInterface::UserCallback videoCallback = std::bind(&MainWindowService::videoRbgDataCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    videoPlayer_->setDisplayFunction(videoCallback);
    // videoPlayer_->setDisplayFunction(videoCallback, nullptr, TpVideoInterface::TP_VIDEO_DECODE_RGBA8888);
    videoPlayer_->setScalingMode(TpVideoInterface::TP_VIDEO_SCALING_FIT); // 推荐格式

    updateProgressTimer_ = new TpTimer();
    connect(updateProgressTimer_, timeout, this, &MainWindowService::slotUpdatePlayerProgress);
}

void MainWindowService::slotUpdatePlayerProgress()
{
    // 获取当前时长
    if (fileType_ == AudioFile)
    {
        // if (audioPlayer_->isPlayEnd())
        // {
        //     updateProgressTimer_->stop();
        //     return;
        // }
        // int curTimeS = audioPlayer_->getPosition();
        // bottomBar_->setCurTimeS(curTimeS);

        // double audioAllTimeS = audioPlayer_->getDuration();
        // bottomBar_->setDurationTimeS(audioAllTimeS);

        // std::cout << "audioAllTimeS " << audioAllTimeS << std::endl;
    }
    else if (fileType_ == VideoFile)
    {
        // if (videoPlayer_->isPlayEnd())
        {
            updateProgressTimer_->stop();
            return;
        }

        // int curTimeS = videoPlayer_->getPosition();
        // bottomBar_->setCurTimeS(curTimeS);

        // double audioAllTimeS = videoPlayer_->getDuration();
        // bottomBar_->setDurationTimeS(audioAllTimeS);
    }
    else
    {
    }
}

void MainWindowService::slotOperateMedia(PlayerOperate status)
{
    if (fileType_ == UnknowFile)
        return;

    if (status == Pause)
    {
        if (fileType_ == AudioFile)
        {
            // audioPlayer_->playPause();
            updateProgressTimer_->stop();
        }
        else if (fileType_ == VideoFile)
        {
            // videoPlayer_->playPause();
            updateProgressTimer_->stop();
        }
        else
        {
        }
    }
    else if (status == Continue)
    {
        if (fileType_ == AudioFile)
        {
            // audioPlayer_->playContinue();
            updateProgressTimer_->start();
        }
        else if (fileType_ == VideoFile)
        {
            // videoPlayer_->playContinue();
            updateProgressTimer_->start();
        }
        else
        {
        }
    }
    else if (status == Forward)
    {
        if (fileType_ == AudioFile)
        {
            // std::cout << "audioPlayer_->getPosition() + 5 " << audioPlayer_->getPosition() + 5 << std::endl;
            // audioPlayer_->setPosition(audioPlayer_->getPosition() + 5);
        }
        else if (fileType_ == VideoFile)
        {
            // videoPlayer_->setPosition(audioPlayer_->getPosition() + 5);
        }
        else
        {
        }
    }
    else if (status == Backward)
    {
        if (fileType_ == AudioFile)
        {
            // std::cout << "audioPlayer_->getPosition() - 5 " << audioPlayer_->getPosition() - 5 << std::endl;
            // audioPlayer_->setPosition(audioPlayer_->getPosition() - 5);
        }
        else if (fileType_ == VideoFile)
        {
            // videoPlayer_->setPosition(audioPlayer_->getPosition() - 5);
        }
        else
        {
        }
    }
    else if (status == NextFile)
    {
    }
    else if (status == PreviousFile)
    {
    }
    else
    {
    }
}

void MainWindowService::slotSwitchPos(int32_t curTimeS)
{
    if (fileType_ == UnknowFile)
        return;

    if (fileType_ == AudioFile)
    {
        std::cout << "CurPosTime " << curTimeS << std::endl;
        // audioPlayer_->setPosition(curTimeS);
    }
    else if (fileType_ == VideoFile)
    {
        std::cout << "CurPosTime " << curTimeS << std::endl;
        // videoPlayer_->setPosition(curTimeS);
    }
    else
    {
    }
}

void MainWindowService::slotSwitchSpeed(int32_t speedIndex)
{
    if (fileType_ == UnknowFile)
        return;

    float speed = 1;
    if (speedIndex == 0)
        speed = 3;
    else if (speedIndex == 1)
        speed = 2;
    else if (speedIndex == 2)
        speed = 1.5;
    else
        speed = 1;

    if (fileType_ == AudioFile)
    {
        std::cout << "CurSpeed " << speed << std::endl;
        // audioPlayer_->setSpeed(speed);
    }
    else
    {
    }
}

void MainWindowService::refreshBarSize()
{
    if (topBar_)
    {
        topBar_->setSize(width(), 70);
        topBar_->move(0, 0);

        topBar_->show();
    }

    if (bottomBar_)
    {
        bottomBar_->setSize(width(), 84);
        bottomBar_->move(0, height() - bottomBar_->height());

        bottomBar_->show();
    }
}

MainWindowService::PlayerFileType MainWindowService::checkFileType(const TpString &suffix)
{
    // -1为不可播放文件；0为音频文件，1为视频文件
    if (suffix.compare("mp3") == 0 || suffix.compare("MP3") == 0 || suffix.compare("wav") == 0 || suffix.compare("ogg") == 0 || suffix.compare("flac") == 0 || suffix.compare("aac") == 0)
    {
        return MainWindowService::AudioFile;
    }
    else if (suffix.compare("mp4") == 0 || suffix.compare("MP4") == 0 || suffix.compare("mkv") == 0 || suffix.compare("avi") == 0 || suffix.compare("mov") == 0)
    {
        return MainWindowService::VideoFile;
    }
    else
    {
        return MainWindowService::UnknowFile;
    }
}
#include <png.h>
void savePng(uint32_t *inputBuffer, uint32_t width, uint32_t height)
{
    static int fileIndex = 0;
    TpString saveFilePath = applicationDirPath() + "/" + TpString::number(fileIndex++).c_str() + ".png";

#if 1
    FILE *fp = fopen(saveFilePath.c_str(), "wb");
    // 处理文件打开失败
    if (!fp)
        return;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
    {
        fclose(fp);
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        return;
    }

    // 设置错误处理
    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return;
    }

    png_init_io(png, fp);

    // 设置图像信息
    png_set_IHDR(png, info,
                 width, height,
                 8,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    // 添加关键：设置字节顺序（RGBA）
    // png_set_swap(png); // 如果您的系统是小端序，可能需要这个

    png_write_info(png, info);

    // 写入像素数据
    int32_t *buffer = reinterpret_cast<int32_t *>(inputBuffer);
    // const int width = imageData->actualWidth;
    // const int height = imageData->actualHeight;
    const int rowbytes = width * 4; // 每个像素4字节 (RGBA)

    // 分配行缓冲区
    png_bytep row_buffer = new png_byte[rowbytes];

    for (int y = 0; y < height; y++)
    {
        // 获取当前行数据
        int32_t *src_row = buffer + y * width;

        // 转换为字节数组
        for (int x = 0; x < width; x++)
        {
            uint32_t pixel = static_cast<uint32_t>(src_row[x]);
#if 1                                                     // ARGB
            row_buffer[x * 4 + 0] = (pixel >> 16) & 0xFF; // R
            row_buffer[x * 4 + 1] = (pixel >> 8) & 0xFF;  // G
            row_buffer[x * 4 + 2] = pixel & 0xFF;         // B
            row_buffer[x * 4 + 3] = 0xFF;                 // A
                                                          // row_buffer[x * 4 + 3] = (pixel >> 24) & 0xFF; // A

#else // RGBA
            row_buffer[x * 4 + 0] = (pixel >> 16) & 0xFF; // R
            row_buffer[x * 4 + 1] = (pixel >> 8) & 0xFF;  // G
            row_buffer[x * 4 + 2] = pixel & 0xFF;         // B
            row_buffer[x * 4 + 3] = (pixel >> 24) & 0xFF; // A
#endif
        }

        png_write_row(png, row_buffer);
    }

    delete[] row_buffer;
    png_write_end(png, nullptr);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
#endif
}

int write_frame_to_png(const char *filename, const uint8_t *rgb_data, int width, int height, int pitch)
{
    static int fileIndex = 0;
    TpString saveFilePath = applicationDirPath() + "/" + TpString::number(fileIndex++).c_str() + ".png";

    FILE *fp = fopen(saveFilePath.c_str(), "wb");
    if (!fp)
    {
        perror("Error opening PNG file for writing");
        return -1;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fclose(fp);
        return -1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return -1;
    }

    // 设置错误处理
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return -1;
    }

    png_init_io(png_ptr, fp);

    // 设置PNG图像头信息
    png_set_IHDR(png_ptr, info_ptr, width, height, 8,
                 PNG_COLOR_TYPE_RGB, // 使用RGB格式
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    // 准备行指针
    png_bytep *row_pointers = (png_bytep *)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (int y = 0; y < height; y++)
    {
        // 注意：SDL纹理的pitch可能不等于width*3，这里直接使用传入的pitch值
        row_pointers[y] = (png_byte *)(rgb_data + y * pitch);
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    // 清理资源
    png_free(png_ptr, row_pointers);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}

int MainWindowService::videoRbgDataCallback(uint8_t **data, int *linesize, uint32_t format, void *userdata)
{
    // return 0;

    // 转换代码
    int width = this->width();
    // int height = this->height();
    int height = 576;
    uint32_t *argbBuffer = new uint32_t[width * height];

    std::cout << "linesize " << linesize[0] << std::endl;
    std::cout << "width  " << width << "  " << height << std::endl;

    for (int y = 0; y < height; y++)
    {
        uint8_t *srcRow = data[0] + y * linesize[0]; // 使用linesize处理行对齐

        for (int x = 0; x < width; x++)
        {
            uint8_t r = srcRow[x * 3 + 0];
            uint8_t g = srcRow[x * 3 + 1];
            uint8_t b = srcRow[x * 3 + 2];

            // ARGB格式：0xAARRGGBB
            argbBuffer[y * width + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
    }

    // write_frame_to_png("123", (uint8_t *)data[0], width, height, linesize[0]);
    // savePng((uint32_t *)data[0], width, height);

    TpImage curPoImage;
    curPoImage.load(argbBuffer, TpSize(width, height));

    // savePng(argbBuffer, width, height);
    delete[] argbBuffer;

    setBackGroundImage(curPoImage);

    return 0;
}
