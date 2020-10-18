#define BUFFER_SIZE  1024  //初始缓冲区大小

class DataBuffer
{
public:
	char *m_pBuffer;    //缓冲区
	int m_nBufferSize;  //缓冲区大小
	int m_nOffset;      //缓冲区中当前数据大小

	int getDataLen();         //获得缓冲区中数据大小
	bool reBufferSize(int nLen); //调整缓冲区大小
	bool addMsg(char *pBuf, int nLen);  //添加消息到缓冲区
	void reset();          //缓冲区复位
	void poll(int nLen);   //移除缓冲区首部的第一个数据包

public:
	DataBuffer();
	~DataBuffer();
};
