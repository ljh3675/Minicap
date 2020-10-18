#include "showpic.h"
#include "ui_showpic.h"
#include <QMessageBox>
showpic::showpic(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::showpic)
{
    ui->setupUi(this);
}

showpic::~showpic()
{
    delete ui;
}

//封包，发送
//data: 要发送的数据
//nLen: 要发送数据的长度
void showpic::packData(unsigned char * data, int nLen)
{
	Packet p;
	int headLen = sizeof(PacketHead);         //包头大小
	p.head.nLen = nLen;                       //包体大小
	char *buf = new char[headLen + nLen];
	memcpy(buf, &p.head, headLen);            //包头
	memcpy(buf + headLen, data, nLen);        //包体
	if (m_tcpSocket != NULL)
		m_tcpSocket->write(buf, headLen + nLen);  //发包
	else
		qDebug() << "socket 未建立！";
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
		//不够包头，不处理
		if (totalLen < headLen)
		{
			break;
		}

		Packet pack;                      //接收到的包
		memcpy(&pack.head, m_Buffer.m_pBuffer, headLen);   //包头
		int bodyLen = pack.head.nLen;     //包体大小
		int packLen = headLen + bodyLen;  //一包数据大小
		if (totalLen < packLen)            //不够一包数据，等够了再解析
		{
			break;
		}

		//数据足够多
		pack.body = new unsigned char[bodyLen];
		memcpy(pack.body, m_Buffer.m_pBuffer + headLen, bodyLen);  //包体
		//recv(pack.body);         //处理得到的包体

		m_Buffer.poll(packLen);  //移除缓冲区中第一个数据包
		totalLen -= (packLen);
	}
}


void showpic::on_pushButton_connect_clicked()
{
    QMessageBox::information(NULL,"title","connect clieck!");
}
