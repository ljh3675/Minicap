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

//���
//data: Ҫ���͵�����
//nLen: Ҫ�������ݵĳ���
void showpic::unpackData(unsigned char * data, int nLen)
{
    m_Buffer.addMsg((char *)data, nLen);              //������ݵ�������
    int totalLen = m_Buffer.getDataLen();  //�����������ݴ�С
    int headLen = sizeof(PacketHead);         //��ͷ��С
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
            // ************����д�ļ�**********
            ofstream imgFo("Image22.jpg", ios::binary);
            if (!imgFo)
            {
                QMessageBox::information(NULL, "title", "Image2.jpg write error");
            }
            imgFo.write((char*)data, nLen);
            imgFo.close();
            // ************����д�ļ�**********

            //������ͷ��������
            if (totalLen < headLen)
            {
                break;
            }

            // ************����д�ļ�**********
            ofstream imgFooo("Image222.jpg", ios::binary);
            if (!imgFooo)
            {
                QMessageBox::information(NULL, "title", "Image2.jpg write error");
            }
            imgFooo.write((char*)data, nLen);
            imgFooo.close();
            // ************����д�ļ�**********

            Packet pack;                      //���յ��İ�
            memcpy(&pack.head, m_Buffer.m_pBuffer, headLen);   //��ͷ
            int bodyLen = pack.head.nLen;     //�����С
            int packLen = headLen + bodyLen;  //һ�����ݴ�С
            if (totalLen < packLen)            //����һ�����ݣ��ȹ����ٽ���
            {
                break;
            }

            // ************����д�ļ�**********
            ofstream imgFoooo("Image2222.jpg", ios::binary);
            if (!imgFoooo)
            {
                QMessageBox::information(NULL, "title", "Image2.jpg write error");
            }
            imgFoooo.write((char*)data, nLen);
            imgFoooo.close();
            // ************����д�ļ�**********


            //�����㹻��
            pack.body = new unsigned char[bodyLen];
            memcpy(pack.body, m_Buffer.m_pBuffer + headLen, bodyLen);  //����
            //recv(pack.body);         //����õ��İ���

            // ��ʾͼƬ
            QImage Image;
            Image.loadFromData(reinterpret_cast<unsigned char*>(pack.body), bodyLen);
            QPixmap pixmap = QPixmap::fromImage(Image);
            int with = ui->label_image_show->width();
            int height = ui->label_image_show->height();
            //QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // �������
            QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // ����������
            ui->label_image_show->setPixmap(fitpixmap);

            m_Buffer.deleteFirst(packLen);  //�Ƴ��������е�һ�����ݰ�
            totalLen -= (packLen);
        }
    }
}
// ����minicap������ ����
void showpic::on_pushButton_connect_clicked()
{
    // �½�tcpsocket�ӿ�
    visualTcpSocket = new QTcpSocket(this);
    // �����źźͲۺ���
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

        // ************����д�ļ�**********
        ofstream imgFo("Image2.jpg", ios::binary);
        if (!imgFo)
        {
            QMessageBox::information(NULL, "title", "Image2.jpg write error");
        }
        imgFo.write((char*)buf, readdata.size());
        imgFo.close();
        // ************����д�ļ�**********

        unpackData(buf, readdata.size());
    }
}

void showpic::onConnected()
{
    ui->label_image_show->setText("connected ! waiting image data");
}

// minicap������׼������ ����
void showpic::on_pushButton_startsever_clicked()
{
    // ����QProcess����
    QProcess p(0);
    p.start("adb devices");
    p.waitForStarted();
    p.waitForFinished();
    QString devices = QString::fromLocal8Bit(p.readAllStandardOutput());
    devices = devices.trimmed();
    ui->textBrowser_server->append("adb devices =" + devices);

    // ��ȡminicap.so sdk
    p.start("adb shell getprop ro.build.version.sdk");
    p.waitForStarted();
    p.waitForFinished();
    QString sdkversion = "android-" + QString::fromLocal8Bit(p.readAllStandardOutput());
    sdkversion = sdkversion.trimmed();
    ui->textBrowser_server->append("SDK =" + sdkversion); // android-29

    // ��ȡminicap abi
    p.start("adb shell getprop ro.product.cpu.abi");
    p.waitForStarted();
    p.waitForFinished();
    QString abiversion = QString::fromLocal8Bit(p.readAllStandardOutput());
    abiversion = abiversion.trimmed();
    ui->textBrowser_server->append("ABI =" + abiversion); // arm64-v8a

    // ��ȡ��������·��+stf_lib https://blog.csdn.net/liyuanbhu/article/details/53710249
    QString exepath = QCoreApplication::applicationDirPath();
    QString minicapso = exepath + "/stf_libs/minicap-shared/aosp/libs";
    QString minicap = exepath + "/stf_libs";

    // ��ȡ�ļ������������ļ����� so 
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
    // ����minicap chmodȨ��
    p.start("adb shell chmod 777 /data/local/tmp/minicap");
    p.waitForStarted();
    p.waitForFinished();

    // �󶨶˿�
    p.start("adb forward tcp:1313 localabstract:minicap");
    p.waitForStarted();
    p.waitForFinished();

    // ����minicap server
    servercmd = new QProcess(this);
    connect(servercmd, SIGNAL(readyReadStandardOutput()), this, SLOT(on_readoutput()));
    connect(servercmd, SIGNAL(readyReadStandardError()), this, SLOT(on_readerror()));
    servercmd->start("adb shell LD_LIBRARY_PATH=/data/local/tmp /data/local/tmp/minicap -P 720x1440@720x1440/0"); // ����ָ��
    servercmd->waitForStarted();
}
// cmd�����
void showpic::on_readoutput()
{
    QMessageBox::information(NULL, "title", "???");
    ui->textBrowser_server->append(QString::fromLocal8Bit(servercmd->readAllStandardOutput().data()));
}
// cmdʧ�������
void showpic::on_readerror()
{
    ui->textBrowser_server->append(QString::fromLocal8Bit(servercmd->readAllStandardError().data()));
}
