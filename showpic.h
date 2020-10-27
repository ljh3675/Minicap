#ifndef SHOWPIC_H
#define SHOWPIC_H

#include <QMainWindow>
#include <QtNetwork/QtNetwork>
#include <QTextBrowser>

#define BUFFER_SIZE  1024  //��ʼ��������С

// ͷ��Ϣ�궨��
#define HEAD_VERSION              0
#define HEAD_SIZEHEADER           1
#define HEAD_PID                  2
#define HEAD_RWIDTH               6
#define HEAD_RHEIGHT              10
#define HEAD_VWIDTH               14
#define HEAD_VHEIGHT              18
#define HEAD_DORIENTATION         22
#define HEAD_QBITFLAGS            23

// ͷ�����ṹ��
#pragma pack(push,1) // ����1�ֽڶ��뷽ʽ

// ͷ�ṹ��
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
    void unpackData(std::string &packdata, int nLen);

// ����������
public:
    char *m_pBuffer;    //������
    int m_nBufferSize;  //��������С
    int m_nOffset;      //�������е�ǰ���ݴ�С
    int getDataLen();         //��û����������ݴ�С
    bool reBufferSize(int nLen); //������������С
    bool addMsg(std::string &pBuf, int nLen);  //�����Ϣ��������
    void reset();          //��������λ
    void deleteFirst(int nLen);   //�Ƴ��������ײ��ĵ�һ�����ݰ�

private slots:
    void on_pushButton_startsever_clicked();
    void on_pushButton_connect_clicked();
    void on_readoutput();
    void on_readerror();
    void readClient();
    void onConnected();

private:
    Ui::showpic *ui;
    QTcpSocket *visualTcpSocket; // ͨ���׽���
    //showpic m_Buffer; // �׽��ֹ����Ļ�����
    QProcess *servercmd = nullptr;
    FirstHeader header;
};
#endif // SHOWPIC_H
