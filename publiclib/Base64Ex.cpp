#include "Base64Ex.h"
#include <time.h>
#include <QString>
Base64Ex::Base64Ex(const char* pszSrc, const unsigned char modeType, int nSrcLen)
	: m_pResult(NULL)
	, m_byteModeType(modeType)
	, m_nLen(0)
{
	if (!pszSrc)
	{
		return;
	}

	int nLenSrc = (0 == nSrcLen) ? strlen(pszSrc) : nSrcLen;
	if (nLenSrc <= 0)
	{
		return;
	}
	//����
	if (modeType == modeEncode)
	{
		int len = nLenSrc * 2 + 3;
		m_pResult = new char[len];
		memset(m_pResult, 0x00, len);
		m_nLen = EncodeBase64((unsigned char*)pszSrc, m_pResult, nLenSrc, nLenSrc * 2);
		if (m_nLen > 0 && m_nLen < nLenSrc * 2)
		{
			m_pResult[m_nLen] = '\0';
		}
	}
	else if (modeType == modeDecode)
	{
		int len = nLenSrc * 2 + 3;
		m_pResult = new char[len];
		memset(m_pResult, 0x00, len);
		m_nLen = DecodeBase64(pszSrc, (unsigned char*)m_pResult, nLenSrc);

		if (m_nLen>0 && m_nLen < len)
		{
			m_pResult[m_nLen] = '\0';
		}
	}
}


Base64Ex::~Base64Ex()
{
	if (m_pResult)
	{
		delete[]m_pResult;
		m_pResult = NULL;
	}
}

const char EnBase64Tab[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
int Base64Ex::EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)
{
	unsigned char c1, c2, c3;    // ���뻺��������3���ֽ�
	int nDstLen = 0;             // ������ַ�����
	int nLineLen = 0;            // ������г��ȼ���
	int nDiv = nSrcLen / 3;      // �������ݳ��ȳ���3�õ��ı���
	int nMod = nSrcLen % 3;      // �������ݳ��ȳ���3�õ�������

	// ÿ��ȡ3���ֽڣ������4���ַ�
	for (int i = 0; i < nDiv; i++)
	{
		// ȡ3���ֽ�
		c1 = *pSrc++;
		c2 = *pSrc++;
		c3 = *pSrc++;

		// �����4���ַ�
		*pDst++ = EnBase64Tab[c1 >> 2];
		*pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
		*pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
		*pDst++ = EnBase64Tab[c3 & 0x3f];
		nLineLen += 4;
		nDstLen += 4;

		// ������У�
		if (nLineLen > nMaxLineLen - 4)
		{
			*pDst++ = '\r';
			*pDst++ = '\n';
			nLineLen = 0;
			nDstLen += 2;
		}
	}

	// �������µ��ֽ�
	if (nMod == 1)
	{
		c1 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4)];
		*pDst++ = '=';
		*pDst++ = '=';
		nLineLen += 4;
		nDstLen += 4;
	}
	else if (nMod == 2)
	{
		c1 = *pSrc++;
		c2 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xf0) >> 4)];
		*pDst++ = EnBase64Tab[((c2 & 0x0f) << 2)];
		*pDst++ = '=';
		nDstLen += 4;
	}

	// ����Ӹ�������
	*pDst = '\0';

	return nDstLen;
}

/*
Base64���뷽���У���򵥵�Ҳ�ǲ������64���ɴ�ӡ�ַ���ֵ��Ϊ���������õ���ֵ����ΧΪ0-63��������������ƴ�ճ��ֽ���ʽ������͵õ���������
*/

const char DeBase64Tab[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	62,        // '+'
	0, 0, 0,
	63,        // '/'
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
	0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
	13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
	0, 0, 0, 0, 0, 0,
	26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
};

int Base64Ex::DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
	int nDstLen;            // ������ַ�����
	int nValue;             // �����õ��ĳ�����
	int i;

	i = 0;
	nDstLen = 0;

	// ȡ4���ַ������뵽һ�����������پ�����λ�õ�3���ֽ�
	while (i < nSrcLen)
	{
		if (*pSrc != '\r' && *pSrc != '\n')
		{
			nValue = DeBase64Tab[*pSrc++] << 18;
			nValue += DeBase64Tab[*pSrc++] << 12;
			*pDst++ = (nValue & 0x00ff0000) >> 16;
			nDstLen++;

			if (*pSrc != '=')
			{
				nValue += DeBase64Tab[*pSrc++] << 6;
				*pDst++ = (nValue & 0x0000ff00) >> 8;
				nDstLen++;

				if (*pSrc != '=')
				{
					nValue += DeBase64Tab[*pSrc++];
					*pDst++ = nValue & 0x000000ff;
					nDstLen++;
				}
			}

			i += 4;
		}
		else        // �س����У�����
		{
			pSrc++;
			i++;
		}
	}

	// ����Ӹ�������
	*pDst = '\0';

	return nDstLen;
}

const char* Base64Ex::GetResult()
{
	return m_pResult;
}

const int Base64Ex::GetLen()
{
	return m_nLen;
}
std::string eclassDecrypt(const char* pText, const char* pKey)
{
	if (!pText)
		return "";
	if (strlen(pText) <= 0)
		return "";

	std::string strResult = "";
	Base64Ex baseText(pText, Base64Ex::modeDecode);
	if (baseText.GetLen() <= 0)
		return "";
	char* pBuffer = new char[baseText.GetLen()];
	memset(pBuffer, 0x00, baseText.GetLen());
	Decryption(pKey, (unsigned char*)baseText.GetResult(), baseText.GetLen(), pBuffer);
	strResult = pBuffer;
	if (pBuffer)
	{
		delete[]pBuffer;
		pBuffer = NULL;
	}
	return strResult;
}

std::string eclassEncrypt(const char* pText, const char* pKey)
{
	if (!pText || !pKey)
		return "";
	if (strlen(pText) <= 0)
		return "";

	std::string strResult = "";
	int nResultLen = strlen(pText) + 2;

	unsigned char* pBuffer = new unsigned char[nResultLen];
	memset(pBuffer, 0x00, nResultLen);
	Encryption(pKey, pText, pBuffer);

	Base64Ex baseText((const char *)pBuffer, Base64Ex::modeEncode, nResultLen);
	strResult = baseText.GetResult();
	QString str = QString::fromStdString(strResult);

	if (pBuffer)
	{
		delete[]pBuffer;
		pBuffer = NULL;
	}
	str = str.replace("+", "%2B");
	str = str.replace("=", "%3D");
	strResult = str.toStdString();
	/*string str1 = "%2B";
	string str11 = "+";
	string str2 = "%3D";	
	string str22 = "=";
	if (strResult.find(str11)>0 && strResult.find(str11)<strResult.length())
	{
		strResult = strResult.replace(strResult.find(str11), str11.length(), str1);
	}
	if (strResult.find(str22)>0 && strResult.find(str22)<strResult.length())
	{
		strResult = strResult.replace(strResult.find(str22), str22.length(), str2);
	}
	*/

	return strResult;
}
bool Encryption(const char *pKey, const char *pText, unsigned char *pResult)
{
	if (!pKey || !pText)
		return 0;
	if (strlen(pKey) <= 0 || strlen(pText) <= 0)
		return 0;

	int nTextLen = strlen(pText);
	int nKeyLen = strlen(pKey);
	unsigned char* pPos = pResult;
	srand((int)time(0));
	unsigned char uRand = rand() % 255;

	//��һλΪУ��λ����Ҫͨ���ڶ�λУ��
	for (int j = 0; j < nKeyLen; j++)
	{
		*pPos += (unsigned char)(*(pKey + j));
	}
	*pPos += uRand;
	pPos++;

	//�ڶ�λΪ�����
	*pPos = uRand;
	pPos++;

	for (int i = 0; i < nTextLen; i++)
	{
		*pPos = 0x00;
		*pPos++ = (unsigned char)(*(pText + i)) + (unsigned char)(*(pKey + (i%nKeyLen))) + uRand;
	}

	return true;
}
bool Decryption(const char *pKey, const  unsigned char* pDate, int ndataLen, char *pResult)
{
	if (!pKey || !pDate || !pResult)
		return false;
	if (strlen(pKey) <= 0 || ndataLen <= 0)
		return false;
	int nKeyLen = strlen(pKey);
	char *pTemp = pResult;
	unsigned char uCheck = *pDate;//ȡ��У��λ
	unsigned char uRand = *(pDate + 1);//ȡ�������
	unsigned char uCauculate = 0;//����ֵ
	for (int j = 0; j < nKeyLen; j++)
	{
		uCauculate += (unsigned char)(*(pKey + j)); //242
	}
	uCauculate += uRand;
	if (uCauculate != uCheck)
		return false;

	for (int i = 2; i < ndataLen; i++)
	{
		*pTemp = (unsigned char)(*(pDate + i)) - (unsigned char)(*(pKey + ((i - 2) % nKeyLen))) - uRand;
		pTemp++;
	}
	return true;
}
