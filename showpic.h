#ifndef SHOWPIC_H
#define SHOWPIC_H

#include <QMainWindow>
#include <databuffer.h>
#include <QtNetwork/QtNetwork>
#include <QTextBrowser>

// ͷ��Ϣ�궨��


// ͷ�����ṹ��
#pragma pack(push,1) // ����1�ֽڶ��뷽ʽ

// ͷ�ṹ��
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
// ��ͷ
typedef struct
{
	uint32_t nLen; // ��ͷ
}PacketHead;
// һ������
typedef struct 
{
	PacketHead head; // ��ͷ
	unsigned char *body; // ����
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
	QTcpSocket *m_tcpSocket; // ͨ���׽���
	DataBuffer m_Buffer; // �׽��ֹ����Ļ�����

};
#endif // SHOWPIC_H
