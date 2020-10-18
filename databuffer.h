#define BUFFER_SIZE  1024  //��ʼ��������С

class DataBuffer
{
public:
	char *m_pBuffer;    //������
	int m_nBufferSize;  //��������С
	int m_nOffset;      //�������е�ǰ���ݴ�С

	int getDataLen();         //��û����������ݴ�С
	bool reBufferSize(int nLen); //������������С
	bool addMsg(char *pBuf, int nLen);  //�����Ϣ��������
	void reset();          //��������λ
	void poll(int nLen);   //�Ƴ��������ײ��ĵ�һ�����ݰ�

public:
	DataBuffer();
	~DataBuffer();
};
