#include "databuffer.h"
#include "string.h"
#include <QException>

//构造
DataBuffer::DataBuffer()
{
	m_nBufferSize = BUFFER_SIZE;  //缓冲区大小
	m_nOffset = 0;      //缓冲区当前现有数据大小
	m_pBuffer = new char[m_nBufferSize];      //分配缓冲区
	memset(m_pBuffer, 0, sizeof(m_pBuffer));  //清空缓冲区
}

//析构
DataBuffer::~DataBuffer()
{
	delete[] m_pBuffer;  //释放缓冲区
	m_pBuffer = NULL;
	m_nBufferSize = 0;
	m_nOffset = 0;
}

//获得缓冲区中数据大小
int DataBuffer::getDataLen()
{
	return m_nOffset;
}

//重置缓冲区大小
bool DataBuffer::reBufferSize(int nLen)
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
		memset(m_pBuffer, 0, sizeof(m_pBuffer));
		memcpy(m_pBuffer, oBuffer, m_nOffset); //将原缓冲区中的内容拷贝到新缓冲区
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
bool DataBuffer::addMsg(char *pBuf, int nLen)
{
	try
	{
		if (m_nOffset + nLen > m_nBufferSize)        //如果缓冲过小，重新调整其大小
			reBufferSize(m_nOffset + nLen);
		memcpy(m_pBuffer + m_nOffset, pBuf, nLen); //将新数据拷贝到缓冲区尾
		m_nOffset += nLen;  //修改数据偏移
	}
	catch (QException e)
	{
		return false;
	}
	return true;
}

//缓冲区复位
void DataBuffer::reset()
{
	if (m_nOffset > 0)
	{
		memset(m_pBuffer, 0, sizeof(m_pBuffer));
		m_nOffset = 0;
	}
}

//移除缓冲区首部第一个数据包
//nLen：一个数据包的大小
void DataBuffer::deleteFirst(int nLen)
{
	if (m_nOffset == 0 || m_pBuffer == NULL)
		return;

	if (m_nOffset >= nLen)
	{
		memcpy(m_pBuffer, m_pBuffer + nLen, m_nOffset - nLen);
		m_nOffset -= nLen;
	}
}