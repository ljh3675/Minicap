#ifndef SHOWPIC_H
#define SHOWPIC_H

#include <QMainWindow>
#include <QtNetwork/QtNetwork>
#include <QTextBrowser>

#define BUFFER_SIZE  1024  //初始缓冲区大小

// 头信息宏定义
#define HEAD_VERSION              0
#define HEAD_SIZEHEADER           1
#define HEAD_PID                  2
#define HEAD_RWIDTH               6
#define HEAD_RHEIGHT              10
#define HEAD_VWIDTH               14
#define HEAD_VHEIGHT              18
#define HEAD_DORIENTATION         22
#define HEAD_QBITFLAGS            23

// 头、包结构体
#pragma pack(push,1) // 采用1字节对齐方式

// 头结构体
typedef struct
{
    unsigned char Version;
    unsigned char SizeHeader;
    uint32_t Pid;
    uint32_t RWidth;
    uint32_t RHeight;
    uint32_t VWidth;
    uint32_t VHeight;
    unsigned char DOrientation;
    unsigned char QBitflags;
}FirstHeader;
// 包头
typedef struct
{
    uint32_t nLen; // 包头
}PacketHead;
// 一包数据
typedef struct
{
    PacketHead head; // 包头
    unsigned char *body; // 包体
}Packet;

#pragma pack(pop)

QT_BEGIN_NAMESPACE
namespace Ui { class showpic; }
QT_END_NAMESPACE

class showpic : public QMainWindow
{
    Q_OBJECT

public:
    showpic(QWidget *parent = nullptr);
    ~showpic();

public:
    void unpackData(std::string &packdata, int nLen);

// 缓冲区部分
public:
    char *m_pBuffer;    //缓冲区
    int m_nBufferSize;  //缓冲区大小
    int m_nOffset;      //缓冲区中当前数据大小
    int getDataLen();         //获得缓冲区中数据大小
    bool reBufferSize(int nLen); //调整缓冲区大小
    bool addMsg(std::string &pBuf, int nLen);  //添加消息到缓冲区
    void reset();          //缓冲区复位
    void deleteFirst(int nLen);   //移除缓冲区首部的第一个数据包

private slots:
    void on_pushButton_startsever_clicked();
    void on_pushButton_connect_clicked();
    void on_readoutput();
    void on_readerror();
    void readClient();
    void onConnected();

private:
    Ui::showpic *ui;
    QTcpSocket *visualTcpSocket; // 通信套接字
    //showpic m_Buffer; // 套接字关联的缓冲区
    QProcess *servercmd = nullptr;
    FirstHeader header;
};
#endif // SHOWPIC_H
