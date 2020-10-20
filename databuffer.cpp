#include "databuffer.h"
#include "string.h"
#include <QException>

//����
DataBuffer::DataBuffer()
{
	m_nBufferSize = BUFFER_SIZE;  //��������С
	m_nOffset = 0;      //��������ǰ�������ݴ�С
	m_pBuffer = new char[m_nBufferSize];      //���仺����
	memset(m_pBuffer, 0, sizeof(m_pBuffer));  //��ջ�����
}

//����
DataBuffer::~DataBuffer()
{
	delete[] m_pBuffer;  //�ͷŻ�����
	m_pBuffer = NULL;
	m_nBufferSize = 0;
	m_nOffset = 0;
}

//��û����������ݴ�С
int DataBuffer::getDataLen()
{
	return m_nOffset;
}

//���û�������С
bool DataBuffer::reBufferSize(int nLen)
{
	char *oBuffer = m_pBuffer;  //����ԭ��������ַ
	try
	{
		nLen = nLen < 64 ? 64 : nLen;  //��֤��С��С
		while (m_nBufferSize < nLen)
		{
			m_nBufferSize *= 2;
		}
		m_pBuffer = new char[m_nBufferSize]; //�����»�����
		memset(m_pBuffer, 0, sizeof(m_pBuffer));
		memcpy(m_pBuffer, oBuffer, m_nOffset); //��ԭ�������е����ݿ������»�����
		delete[]oBuffer;  //�ͷ�ԭ������
	}
	catch (QException e)
	{
		return false;
	}
	return true;
}

//�򻺳����������Ϣ
/*
 * pBuf��Ҫ��ӵ�����
 * nLen�����ݳ���
 * �ɹ�����true��ʧ�ܷ���false
 */
bool DataBuffer::addMsg(char *pBuf, int nLen)
{
	try
	{
		if (m_nOffset + nLen > m_nBufferSize)        //��������С�����µ������С
			reBufferSize(m_nOffset + nLen);
		memcpy(m_pBuffer + m_nOffset, pBuf, nLen); //�������ݿ�����������β
		m_nOffset += nLen;  //�޸�����ƫ��
	}
	catch (QException e)
	{
		return false;
	}
	return true;
}

//��������λ
void DataBuffer::reset()
{
	if (m_nOffset > 0)
	{
		memset(m_pBuffer, 0, sizeof(m_pBuffer));
		m_nOffset = 0;
	}
}

//�Ƴ��������ײ���һ�����ݰ�
//nLen��һ�����ݰ��Ĵ�С
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