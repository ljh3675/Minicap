#ifndef SHOWPIC_H
#define SHOWPIC_H

#include <QMainWindow>
#include <databuffer.h>
#include <QtNetwork/QtNetwork>
#include <QTextBrowser>

// 头信息宏定义


// 头、包结构体
#pragma pack(push,1) // 采用1字节对齐方式

// 头结构体
typedef struct 
{
	unsigned char Version;
	unsigned char Headsize;
	uint32_t Pid;
	uint32_t RWidth;
	uint32_t RHeight;
	uint32_t VWidth;
	uint32_t VHeight;
	unsigned char Orientation;
	unsigned char Bitflags;
}HeadInfo;
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
	void packData(unsigned char *data, int nLen);
	void unpackData(unsigned char *data, int nLen);

private slots:
    void on_pushButton_connect_clicked();
	void on_pushButton_save_clicked();

private:
    Ui::showpic *ui;
	QTcpSocket *m_tcpSocket; // 通信套接字
	DataBuffer m_Buffer; // 套接字关联的缓冲区

};
#endif // SHOWPIC_H
