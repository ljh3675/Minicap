#include "showpic.h"
#include "ui_showpic.h"
#include <QMessageBox>
#include <QtNetwork/QTcpSocket>
#include <fstream>
using namespace std;

bool HEADINFO_ONCE_FLAG_MAIN = true;

showpic::showpic(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::showpic)
{
    m_nBufferSize = BUFFER_SIZE;  //缓冲区大小
    m_nOffset = 0;      //缓冲区当前现有数据大小
    m_pBuffer = new char[m_nBufferSize];      //分配缓冲区
    ui->setupUi(this);
    on_pushButton_startsever_clicked();
}

showpic::~showpic()
{
    delete[] m_pBuffer;  //释放缓冲区
    m_pBuffer = NULL;
    m_nBufferSize = 0;
    m_nOffset = 0;
    servercmd->terminate();
    servercmd->waitForFinished();
    servercmd = nullptr;
    delete ui;
}

//解包
//data: 要发送的数据
//nLen: 要发送数据的长度
void showpic::unpackData(std::string &packdata, int nLen)
{
    //char *dataswap = (char *)packdata.data();
    addMsg(packdata, nLen);              //添加数据到缓冲区
    int totalLen = getDataLen();  //缓冲区中数据大小
    int headLen = sizeof(PacketHead);         //包头大小
    while (totalLen > 0)
    {
        if (HEADINFO_ONCE_FLAG_MAIN)
        {
            uint32_t firsthheadlen = sizeof(FirstHeader);
            if (totalLen < firsthheadlen)
            {
                QMessageBox::information(NULL, "title", "totalLen<firsthheadlen");
                break;
            }
            memcpy_s(&header.Version, sizeof(header.Version), m_pBuffer + HEAD_VERSION, sizeof(unsigned char));
            memcpy_s(&header.SizeHeader, sizeof(header.SizeHeader), m_pBuffer + HEAD_SIZEHEADER, sizeof(unsigned char));
            memcpy_s(&header.Pid, sizeof(header.Pid), m_pBuffer + HEAD_PID, sizeof(uint32_t));
            memcpy_s(&header.RWidth, sizeof(header.RWidth), m_pBuffer + HEAD_RWIDTH, sizeof(uint32_t));
            memcpy_s(&header.RHeight, sizeof(header.RHeight), m_pBuffer + HEAD_RHEIGHT, sizeof(uint32_t));
            memcpy_s(&header.VWidth, sizeof(header.VWidth), m_pBuffer + HEAD_VWIDTH, sizeof(uint32_t));
            memcpy_s(&header.VHeight, sizeof(header.VHeight), m_pBuffer + HEAD_VHEIGHT, sizeof(uint32_t));
            memcpy_s(&header.DOrientation, sizeof(header.DOrientation), m_pBuffer + HEAD_DORIENTATION, sizeof(unsigned char));
            memcpy_s(&header.QBitflags, sizeof(header.QBitflags), m_pBuffer + HEAD_QBITFLAGS, sizeof(unsigned char));
            ui->textBrowser_info->append("Version =" + QString::number(header.Version, 10));
            ui->textBrowser_info->append("SizeHeader =" + QString::number(header.SizeHeader, 10));
            ui->textBrowser_info->append("Pid =" + QString::number(header.Pid, 10));
            ui->textBrowser_info->append("RWidth =" + QString::number(header.RWidth, 10));
            ui->textBrowser_info->append("RHeight =" + QString::number(header.RHeight, 10));
            ui->textBrowser_info->append("VWidth =" + QString::number(header.VWidth, 10));
            ui->textBrowser_info->append("VHeight =" + QString::number(header.VHeight, 10));
            ui->textBrowser_info->append("DOrientation =" + QString::number(header.DOrientation, 10));
            ui->textBrowser_info->append("QBitflags =" + QString::number(header.QBitflags, 10));

            deleteFirst(firsthheadlen);
            totalLen -= (firsthheadlen);
            HEADINFO_ONCE_FLAG_MAIN = false;
        }
        else
        {
            //不够包头，不处理
            if (totalLen < headLen)
            {
                break;
            }

            Packet pack;                      //接收到的包
            memcpy_s(&pack.head, sizeof(pack.head), m_pBuffer, headLen);   //包头
            int bodyLen = pack.head.nLen;     //包体大小
            int packLen = headLen + bodyLen;  //一包数据大小
            if (totalLen < packLen)            //不够一包数据，等够了再解析
            {
                break;
            }

            //数据足够多
            pack.body = new unsigned char[bodyLen];
            memcpy_s(pack.body, bodyLen, m_pBuffer + headLen, bodyLen);  //包体
            //recv(pack.body);         //处理得到的包体

            // 显示图片
            QImage Image;
            Image.loadFromData(reinterpret_cast<unsigned char*>(pack.body), bodyLen);
            QPixmap pixmap = QPixmap::fromImage(Image);
            int with = ui->label_image_show->width();
            int height = ui->label_image_show->height();
            //QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
            QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
            ui->label_image_show->setPixmap(fitpixmap);

            deleteFirst(packLen);  //移除缓冲区中第一个数据包
            totalLen -= (packLen);
        }
    }
}

void showpic::readClient()
{
    if (visualTcpSocket->bytesAvailable() > 0)
    {
        QByteArray readdata = visualTcpSocket->readAll();
        if (readdata.size() == 0)
        {
            QMessageBox::information(NULL, "title", "readdata.size() == 0");
        }
        std::string buf(readdata, readdata.size()); // 后面加上size可以防止遇到\0的情况被终止
        unpackData(buf, readdata.size());
    }
}

// 连接minicap服务器 解析
void showpic::on_pushButton_connect_clicked()
{
    // 新建tcpsocket接口
    visualTcpSocket = new QTcpSocket(this);
    // 加入信号和槽函数
    connect(visualTcpSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(visualTcpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(visualTcpSocket, SIGNAL(disconnected()), visualTcpSocket, SLOT(deleteLater()));

    visualTcpSocket->connectToHost("127.0.0.1", 1313);

    ui->label_image_show->setText("connect device please wait");

}

void showpic::onConnected()
{
    ui->label_image_show->setText("connected ! waiting image data");
}

// minicap服务器准备工作 启动
void showpic::on_pushButton_startsever_clicked()
{
    // 测试QProcess功能
    QProcess p(0);
    p.start("adb devices");
    p.waitForStarted();
    p.waitForFinished();
    QString devices = QString::fromLocal8Bit(p.readAllStandardOutput());
    devices = devices.trimmed();
    ui->textBrowser_server->append("adb devices =" + devices);

    // 获取minicap.so sdk
    p.start("adb shell getprop ro.build.version.sdk");
    p.waitForStarted();
    p.waitForFinished();
    QString sdkversion = "android-" + QString::fromLocal8Bit(p.readAllStandardOutput());
    sdkversion = sdkversion.trimmed();
    ui->textBrowser_server->append("SDK =" + sdkversion); // android-29

    // 获取minicap abi
    p.start("adb shell getprop ro.product.cpu.abi");
    p.waitForStarted();
    p.waitForFinished();
    QString abiversion = QString::fromLocal8Bit(p.readAllStandardOutput());
    abiversion = abiversion.trimmed();
    ui->textBrowser_server->append("ABI =" + abiversion); // arm64-v8a

    // 获取设备的分辨率
    p.start("adb shell wm size");
    p.waitForStarted();
    p.waitForFinished();
    QString wmsize = QString::fromLocal8Bit(p.readAllStandardOutput());
    wmsize = wmsize.trimmed();
    ui->textBrowser_server->append(wmsize);
    string realwmsizeflag = wmsize.toStdString();
    QString realwmsize = QString::fromStdString(
        realwmsizeflag.substr(realwmsizeflag.find(":") + 1)).trimmed(); // 720x1440

    //设置虚拟尺寸为分辨率的一半
    int vwmsizeh;
    int vwmsizew;
    QString vwmsize;
    string vwmsizeswap = realwmsize.toStdString();
    if (vwmsizeswap.find("x") == vwmsizeswap.npos) {
        QMessageBox::information(NULL, "title", "cant find realsize - x");
        vwmsize = realwmsize;
    }
    else {
        vwmsizeh = std::stoi(vwmsizeswap.substr(vwmsizeswap.find("x") + 1)) / 2;
        vwmsizew = std::stoi(vwmsizeswap.erase(vwmsizeswap.find("x"))) / 2;
        vwmsize = QString::fromStdString(
            std::to_string(vwmsizew) + "x" + std::to_string(vwmsizeh)).trimmed();
    }

    // 获取程序所在路径+stf_lib https://blog.csdn.net/liyuanbhu/article/details/53710249
    QString exepath = QCoreApplication::applicationDirPath();
    QString minicapso = exepath + "/stf_libs/minicap-shared/aosp/libs";
    QString minicap = exepath + "/stf_libs";

    // 获取文件夹下所有子文件名称 so 
    QDir *sodir = new QDir(minicapso);
    QStringList sofilter;
    QList<QFileInfo> *sofileInfo = new QList<QFileInfo>(sodir->entryInfoList(sofilter));
    if (sofileInfo == nullptr)
    {
        QMessageBox::information(NULL, "title", "cant find solib");
        return;
    }
    for (int i = 0; i < sofileInfo->count(); i++)
    {
        if (sdkversion.compare(sofileInfo->at(i).fileName()) == 0)
        {
            // push minicap.so
            p.start("adb push " + minicapso + "/" + sdkversion + "/" + abiversion + "/minicap.so" + " data/local/tmp");
            p.waitForStarted();
            p.waitForFinished();
            ui->textBrowser_server->append(QString::fromLocal8Bit(p.readAllStandardOutput()));
        }
    }
    // minicap
    QDir *exedir = new QDir(minicap);
    QStringList exefilter;
    QList<QFileInfo> *exefileInfo = new QList<QFileInfo>(exedir->entryInfoList(exefilter));
    if (exefileInfo == nullptr)
    {
        QMessageBox::information(NULL, "title", "cant find exelib");
        return;
    }
    for (int i = 0; i < exefileInfo->count(); i++)
    {
        if (abiversion.compare(exefileInfo->at(i).fileName()) == 0)
        {
            // push minicap
            p.start("adb push " + minicap + "/" + abiversion + "/minicap" + " data/local/tmp");
            p.waitForStarted();
            p.waitForFinished();
            ui->textBrowser_server->append(QString::fromLocal8Bit(p.readAllStandardOutput()));
        }
    }
    // 给予minicap chmod权限
    p.start("adb shell chmod 777 /data/local/tmp/minicap");
    p.waitForStarted();
    p.waitForFinished();

    // 绑定端口
    p.start("adb forward tcp:1313 localabstract:minicap");
    p.waitForStarted();
    p.waitForFinished();

    // 启动minicap server
    servercmd = new QProcess(this);
    connect(servercmd, SIGNAL(readyReadStandardOutput()), this, SLOT(on_readoutput()));
    connect(servercmd, SIGNAL(readyReadStandardError()), this, SLOT(on_readerror()));
    servercmd->start("adb shell LD_LIBRARY_PATH=/data/local/tmp /data/local/tmp/minicap -P " + realwmsize + "@" + vwmsize + "/0"); // 启动指令
    servercmd->waitForStarted();
}
// cmd输出行
void showpic::on_readoutput()
{
    ui->textBrowser_server->append(QString::fromLocal8Bit(servercmd->readAllStandardOutput().data()));
}
// cmd失败输出行
void showpic::on_readerror()
{
    ui->textBrowser_server->append(QString::fromLocal8Bit(servercmd->readAllStandardError().data()));
}

//获得缓冲区中数据大小
int showpic::getDataLen()
{
    return m_nOffset;
}

//重置缓冲区大小
bool showpic::reBufferSize(int nLen)
{
    char *oBuffer = m_pBuffer;  //保存原缓冲区地址
    try
    {
        nLen = nLen < 64 ? 64 : nLen;  //保证最小大小
        while (m_nBufferSize < nLen)
        {
            m_nBufferSize *= 2;
        }
        m_pBuffer = new char[m_nBufferSize]; //分配新缓冲区
        memcpy_s(m_pBuffer, m_nBufferSize, oBuffer, m_nOffset); //将原缓冲区中的内容拷贝到新缓冲区
        delete[]oBuffer;  //释放原缓冲区
    }
    catch (QException e)
    {
        return false;
    }
    return true;
}

//向缓冲区中添加消息
/*
 * pBuf，要添加的数据
 * nLen，数据长度
 * 成功返回true，失败返回false
 */
bool showpic::addMsg(std::string &pBuf, int nLen)
{
    try
    {
        if (m_nOffset + nLen > m_nBufferSize)  // 如果缓冲过小，重新调整其大小
            reBufferSize(m_nOffset + nLen);
        memcpy_s(m_pBuffer + m_nOffset, m_nBufferSize - m_nOffset, pBuf.c_str(), nLen); // 将新数据拷贝到缓冲区尾
        m_nOffset += nLen;  // 修改数据偏移
    }
    catch (QException e)
    {
        return false;
    }
    return true;
}

//缓冲区复位
void showpic::reset()
{
    if (m_nOffset > 0)
    {
        m_pBuffer = new char[m_nBufferSize];
        //m_pBuffer[m_nBufferSize] = { 0 };
        m_nOffset = 0;
    }
}

//移除缓冲区首部第一个数据包
//nLen：一个数据包的大小
void showpic::deleteFirst(int nLen)
{
    if (m_nOffset == 0 || m_pBuffer == NULL)
        return;

    if (m_nOffset >= nLen)
    {
        memcpy_s(m_pBuffer, m_nBufferSize, m_pBuffer + nLen, m_nOffset - nLen);
        m_nOffset -= nLen;
    }
}
