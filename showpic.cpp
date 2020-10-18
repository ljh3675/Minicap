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

void showpic::on_pushButton_save_clicked()
{
	QMessageBox::information(NULL, "title", "save clicked!");

	// ��ȡ�ļ������������ļ����� test success
	//QString exepath = QCoreApplication::applicationDirPath();
	//QDir *dir = new QDir(exepath); //��Ҫ�޸�
	//QStringList filter;
	//QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
	//for (int i = 0; i < fileInfo->count(); i++)
	//{
	//	//qDebug() << fileInfo->at(i).filePath();
	//	//qDebug() << fileInfo->at(i).fileName();
	//	ui->textBrowser->append(fileInfo->at(i).fileName());
	//}
	// ��ȡ�ļ������������ļ����� test success

}


void showpic::on_pushButton_connect_clicked()
{
    
	// ����QProcess����
	QProcess p(0);
	p.start("adb devices");
	p.waitForStarted();
	p.waitForFinished();
	ui->textBrowser->append(QString::fromLocal8Bit(p.readAllStandardOutput()));

	// ��ȡminicap.so
	QProcess sdk(0);
	sdk.start("adb shell getprop ro.build.version.sdk");
	sdk.waitForStarted();
	sdk.waitForFinished();
	QString sdkversion = QString::fromLocal8Bit(sdk.readAllStandardOutput());
	// ��ȡ��������·�� https://blog.csdn.net/liyuanbhu/article/details/53710249
	QString exepath = QCoreApplication::applicationDirPath();

	// ��ȡ�ļ������������ļ����� test success
	QDir *dir = new QDir(exepath); //��Ҫ�޸�
	QStringList filter;
	QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
	for (int i = 0; i < fileInfo->count(); i++)
	{
		ui->textBrowser->append(fileInfo->at(i).fileName());
	}
	// ��ȡ�ļ������������ļ����� test success

	// �Ƚ������ַ����Ƿ����
	if (sdkversion.compare("") == 0)
	{
		//ִ��adb pushָ��
	}

	// ��ȡminicap
	QProcess abi(0);
	abi.start("adb shell getprop ro.product.cpu.abi");
	abi.waitForStarted();
	abi.waitForFinished();
	QString abiversion = QString::fromLocal8Bit(abi.readAllStandardOutput());

	
	
	/*QProcess p(0);
	p.start("cmd");
	p.waitForStarted();
	p.write("dir\n");
	p.closeWriteChannel();
	p.waitForFinished();
	qDebug() << QString::fromLocal8Bit(p.readAllStandardOutput());
	
	QProcess p(0);
	p.start("cmd", QStringList() << "/c" << "dir");
	p.waitForStarted();
	p.waitForFinished();
	qDebug() << QString::fromLocal8Bit(p.readAllStandardOutput());*/

}
