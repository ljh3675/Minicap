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
    ui->setupUi(this);
    on_pushButton_startsever_clicked();
}

showpic::~showpic()
{
    delete ui;
    servercmd = nullptr;
}

//解包
//data: 要发送的数据
//nLen: 要发送数据的长度
void showpic::unpackData(unsigned char * data, int nLen)
{
    m_Buffer.addMsg((char *)data, nLen);              //添加数据到缓冲区
    int totalLen = m_Buffer.getDataLen();  //缓冲区中数据大小
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
            memcpy(&header.Version, m_Buffer.m_pBuffer + HEAD_VERSION, sizeof(unsigned char));
            memcpy(&header.SizeHeader, m_Buffer.m_pBuffer + HEAD_SIZEHEADER, sizeof(unsigned char));
            memcpy(&header.Pid, m_Buffer.m_pBuffer + HEAD_PID, sizeof(uint32_t));
            memcpy(&header.RWidth, m_Buffer.m_pBuffer + HEAD_RWIDTH, sizeof(uint32_t));
            memcpy(&header.RHeight, m_Buffer.m_pBuffer + HEAD_RHEIGHT, sizeof(uint32_t));
            memcpy(&header.VWidth, m_Buffer.m_pBuffer + HEAD_VWIDTH, sizeof(uint32_t));
            memcpy(&header.VHeight, m_Buffer.m_pBuffer + HEAD_VHEIGHT, sizeof(uint32_t));
            memcpy(&header.DOrientation, m_Buffer.m_pBuffer + HEAD_DORIENTATION, sizeof(unsigned char));
            memcpy(&header.QBitflags, m_Buffer.m_pBuffer + HEAD_QBITFLAGS, sizeof(unsigned char));
            ui->textBrowser_info->append("Version =" + QString::number(header.Version, 10));
            ui->textBrowser_info->append("SizeHeader =" + QString::number(header.SizeHeader, 10));
            ui->textBrowser_info->append("Pid =" + QString::number(header.Pid, 10));
            ui->textBrowser_info->append("RWidth =" + QString::number(header.RWidth, 10));
            ui->textBrowser_info->append("RHeight =" + QString::number(header.RHeight, 10));
            ui->textBrowser_info->append("VWidth =" + QString::number(header.VWidth, 10));
            ui->textBrowser_info->append("VHeight =" + QString::number(header.VHeight, 10));
            ui->textBrowser_info->append("DOrientation =" + QString::number(header.DOrientation, 10));
            ui->textBrowser_info->append("QBitflags =" + QString::number(header.QBitflags, 10));

            m_Buffer.deleteFirst(firsthheadlen);
            totalLen -= (firsthheadlen);
            HEADINFO_ONCE_FLAG_MAIN = false;
        }
        else
        {
            // ************测试写文件**********
            ofstream imgFo("Image22.jpg", ios::binary);
            if (!imgFo)
            {
                QMessageBox::information(NULL, "title", "Image2.jpg write error");
            }
            imgFo.write((char*)data, nLen);
            imgFo.close();
            // ************测试写文件**********

            //不够包头，不处理
            if (totalLen < headLen)
            {
                break;
            }

            // ************测试写文件**********
            ofstream imgFooo("Image222.jpg", ios::binary);
            if (!imgFooo)
            {
                QMessageBox::information(NULL, "title", "Image2.jpg write error");
            }
            imgFooo.write((char*)data, nLen);
            imgFooo.close();
            // ************测试写文件**********

            Packet pack;                      //接收到的包
            memcpy(&pack.head, m_Buffer.m_pBuffer, headLen);   //包头
            int bodyLen = pack.head.nLen;     //包体大小
            int packLen = headLen + bodyLen;  //一包数据大小
            if (totalLen < packLen)            //不够一包数据，等够了再解析
            {
                break;
            }

            // ************测试写文件**********
            ofstream imgFoooo("Image2222.jpg", ios::binary);
            if (!imgFoooo)
            {
                QMessageBox::information(NULL, "title", "Image2.jpg write error");
            }
            imgFoooo.write((char*)data, nLen);
            imgFoooo.close();
            // ************测试写文件**********


            //数据足够多
            pack.body = new unsigned char[bodyLen];
            memcpy(pack.body, m_Buffer.m_pBuffer + headLen, bodyLen);  //包体
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

            m_Buffer.deleteFirst(packLen);  //移除缓冲区中第一个数据包
            totalLen -= (packLen);
        }
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

void showpic::readClient()
{
    if (visualTcpSocket->bytesAvailable() > 0)
    {
        QByteArray readdata = visualTcpSocket->readAll();
        if (readdata.size() == 0)
        {
            QMessageBox::information(NULL, "title", "readdata.size() == 0");
        }
        unsigned char *buf = (unsigned char*)readdata.data();

        // ************测试写文件**********
        ofstream imgFo("Image2.jpg", ios::binary);
        if (!imgFo)
        {
            QMessageBox::information(NULL, "title", "Image2.jpg write error");
        }
        imgFo.write((char*)buf, readdata.size());
        imgFo.close();
        // ************测试写文件**********

        unpackData(buf, readdata.size());
    }
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
            //ui->textBrowser_server->append("adb push " + minicapso + "/" + sdkversion + "/" + abiversion + "/minicap.so" + " data/local/tmp");
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
            //ui->textBrowser_server->append("adb push " + minicap + "/" + abiversion + "/minicap" + " data/local/tmp");
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
    servercmd->start("adb shell LD_LIBRARY_PATH=/data/local/tmp /data/local/tmp/minicap -P 720x1440@720x1440/0"); // 启动指令
    servercmd->waitForStarted();
}
// cmd输出行
void showpic::on_readoutput()
{
    QMessageBox::information(NULL, "title", "???");
    ui->textBrowser_server->append(QString::fromLocal8Bit(servercmd->readAllStandardOutput().data()));
}
// cmd失败输出行
void showpic::on_readerror()
{
    ui->textBrowser_server->append(QString::fromLocal8Bit(servercmd->readAllStandardError().data()));
}
