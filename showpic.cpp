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

//���������
//data: Ҫ���͵�����
//nLen: Ҫ�������ݵĳ���
void showpic::packData(unsigned char * data, int nLen)
{
	Packet p;
	int headLen = sizeof(PacketHead);         //��ͷ��С
	p.head.nLen = nLen;                       //�����С
	char *buf = new char[headLen + nLen];
	memcpy(buf, &p.head, headLen);            //��ͷ
	memcpy(buf + headLen, data, nLen);        //����
	if (m_tcpSocket != NULL)
		m_tcpSocket->write(buf, headLen + nLen);  //����
	else
		qDebug() << "socket δ������";
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
		//������ͷ��������
		if (totalLen < headLen)
		{
			break;
		}

		Packet pack;                      //���յ��İ�
		memcpy(&pack.head, m_Buffer.m_pBuffer, headLen);   //��ͷ
		int bodyLen = pack.head.nLen;     //�����С
		int packLen = headLen + bodyLen;  //һ�����ݴ�С
		if (totalLen < packLen)            //����һ�����ݣ��ȹ����ٽ���
		{
			break;
		}

		//�����㹻��
		pack.body = new unsigned char[bodyLen];
		memcpy(pack.body, m_Buffer.m_pBuffer + headLen, bodyLen);  //����
		//recv(pack.body);         //����õ��İ���

		m_Buffer.poll(packLen);  //�Ƴ��������е�һ�����ݰ�
		totalLen -= (packLen);
	}
}


void showpic::on_pushButton_connect_clicked()
{
    QMessageBox::information(NULL,"title","connect clieck!");
}
