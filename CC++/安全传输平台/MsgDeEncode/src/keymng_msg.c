#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "keymng_msg.h"
#include "itcast_asn1_der.h"

//Teacher�ṹ�����
int TeacherEncode(Teacher *pTeacher, ITCAST_ANYBUF **outData)
{
	/* 
	typedef struct _Teacher
	{
		char name[64];
		int age;
		char *p;
		int plen;
	}Teacher;
	 */
	int					ret = 0;
	ITCAST_ANYBUF		*pTmp = NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF		*pTmpBuf = NULL;
	ITCAST_ANYBUF		*pOutData = NULL;
	unsigned char		*tmpout = NULL;
	int					tmpoutlen = 0;

	//����name
	// 1 ��Char ���� ת����  ITCAST_ANYBUF
	ret = DER_ITCAST_String_To_AnyBuf(&pTmpBuf, pTeacher->name, strlen(pTeacher->name));
	if (ret != 0)
	{
		if(pTmpBuf != NULL){
			DER_ITCAST_FreeQueue(pTmpBuf);
		}
		printf("func  DER_ITCAST_String_To_AnyBuf() ת��teacher ΪanyBuf ���� err:%d \n", ret);
		return ret;
	}

	// 2 ���� name
	ret = DER_ItAsn1_WritePrintableString(pTmpBuf, &pHeadBuf);
	if (ret != 0)
	{
		if(pTmpBuf != NULL){
			DER_ITCAST_FreeQueue(pTmpBuf);
		}
		printf("func  DER_ITCAST_String_To_AnyBuf()  encode name err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmpBuf);
	pTmp = pHeadBuf;

	//���� age
	ret = DER_ItAsn1_WriteInteger(pTeacher->age, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf); //�ͷ����� �����ڴ�й©
		//��ʧ�ܺ󣬽�pHeadBuf ָ���ͷţ�ͬʱ��Ҫ��pTmp ָ���ÿգ���ֹҰָ��
		pTmp = NULL;
		printf("func DER_ItAsn1_WriteInteger() encode age err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//���� p
	ret = EncodeChar(pTeacher->p, pTeacher->plen, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf); //�ͷ����� �����ڴ�й©
		//��ʧ�ܺ󣬽�pHeadBuf ָ���ͷţ�ͬʱ��Ҫ��pTmp ָ���ÿգ���ֹҰָ��
		pTmp = NULL;
		printf("func EncodeChar() encode * P err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//���� plen
	ret = DER_ItAsn1_WriteInteger(pTeacher->plen, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf); //�ͷ����� �����ڴ�й©
		//��ʧ�ܺ󣬽�pHeadBuf ָ���ͷţ�ͬʱ��Ҫ��pTmp ָ���ÿգ���ֹҰָ��
		pTmp = NULL;
		printf("func DER_ItAsn1_WriteInteger() encode plen err:%d \n", ret);
		return ret;
	}

	//����teacher �ṹ��
	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf); //�ͷ����� �����ڴ�й©
		//��ʧ�ܺ󣬽�pHeadBuf ָ���ͷţ�ͬʱ��Ҫ��pTmp ָ���ÿգ���ֹҰָ��
		pTmp = NULL;
		printf("func DER_ItAsn1_WriteSequence() encode  teacher stuct err:%d \n", ret);
		return ret;
	}

	*outData = pOutData; //��ʦ�ṹ�� ��ڵ� ������
	DER_ITCAST_FreeQueue(pHeadBuf); //�ͷ����� �����ڴ�й©
	pTmp = NULL;

	/* 
	*out = (unsigned char *)malloc(pOutData->dataLen);
	if (*out == NULL)
	{
		ret = 2;
		DER_ITCAST_FreeQueue(pOutData);
		printf("func TeacherEncode() malloc err:%d \n", ret);
		return ret;
	}
	memcpy(*out, pOutData->pData, pOutData->dataLen); //��������copy���·�����ڴ����
	*outlen = pOutData->dataLen;
	*/
	//DER_ITCAST_FreeQueue(pOutData);

	return ret;
}
//Teacher�ṹ�����
int TeacherDecode(unsigned char *inData, int inLen, Teacher **pStruct)
{
	int					ret = 0;
	ITCAST_ANYBUF		*pTmp = NULL, *pHead = NULL;
	ITCAST_ANYBUF		*pOutData = NULL;
	ITCAST_ANYBUF		*inAnyBuf = NULL;

	Teacher				*pTmpStru = NULL;

/*
	//��c����buf ת���� ITCAST_ANYBUF
	inAnyBuf = (ITCAST_ANYBUF *)malloc(sizeof(ITCAST_ANYBUF));
	if (inAnyBuf == NULL)
	{
		ret = 1;
		printf("func TeacherDecode malloc error: %d", ret);
		return ret;
	}
	memset(inAnyBuf, 0, sizeof(ITCAST_ANYBUF));

	inAnyBuf->pData = (unsigned char *)malloc(inLen);
	if (inAnyBuf->pData == NULL)
	{
		DER_ITCAST_FreeQueue(inAnyBuf);
		ret = 2;
		printf("func TeacherDecode malloc error: %d", ret);
		return ret;
	}
	memcpy(inAnyBuf->pData, indata, inLen);
	inAnyBuf->dataLen = inLen;
*/

	//�� ber ��ʽ���ֽ�����ת����AnyBuf
	ret = DER_ITCAST_String_To_AnyBuf(&inAnyBuf, inData, inLen);
	if (ret != 0)
	{
		if(inAnyBuf != NULL) {
			DER_ITCAST_FreeQueue(inAnyBuf);
		}
		printf("func DER_ITCAST_String_To_AnyBuf error : %d", ret);
		return ret;
	}

	//���� Teacher �ṹ�� �õ�4���ڵ�
	ret = DER_ItAsn1_ReadSequence(inAnyBuf, &pHead);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(inAnyBuf);
		printf("func DER_ItAsn1_ReadSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(inAnyBuf);

		//����ʦ�ṹ����������
	pTmpStru = (Teacher *)malloc(sizeof(Teacher));
	if (pTmpStru == NULL)
	{
		ret = 3;
		DER_ITCAST_FreeQueue(pHead); //�ͷ�����
		printf("func malloc() err:%d \n", ret);
		return ret;
	}
	memset(pTmpStru, 0, sizeof(Teacher));

	pTmp = pHead;

	//���� name
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead); //�ͷ�����
		pTmp = NULL; //��ֹҰָ��
		//����ṹ���е�����ֵʧ�ܣ��ͷŽṹ��ռ䣬������
		TeacherDecode_Free(&pTmpStru);
		printf("func DER_ItAsn1_ReadPrintableString() err:%d \n", ret);
		return ret;
	}
	//��name������ֵ
	memcpy(pTmpStru->name, pOutData->pData, pOutData->dataLen);
	pTmp = pTmp->next; //������һ���ڵ�
	DER_ITCAST_FreeQueue(pOutData);

	//����age
	ret = DER_ItAsn1_ReadInteger(pTmp, &(pTmpStru->age));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead); //�ͷ�pHead ����
		DER_ITCAST_FreeQueue(pTmp); //�ͷ�pTmp �����˴�Ϊʲô����ҪдpTmp���ͷţ���Ϊ��ʱ��pTmp!= pHead��
		//����ṹ���е�����ֵʧ�ܣ��ͷŽṹ��ռ䣬������
		TeacherDecode_Free(&pTmpStru);
		printf("func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next; //������һ���ڵ�

	//���� p
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead); //�ͷ�����
		DER_ITCAST_FreeQueue(pTmp);
		//����ṹ���е�����ֵʧ�ܣ��ͷŽṹ��ռ䣬������
		TeacherDecode_Free(&pTmpStru);
		printf("func DER_ItAsn1_ReadPrintableString() err:%d \n", ret);
		return ret;
	}
	pTmpStru->p = (char *)malloc(pOutData->dataLen + 1);
	if (pTmpStru->p == NULL)
	{
		ret = 4;
		printf("func malloc err:%d \n", ret);
		return ret;
	}
	memcpy(pTmpStru->p, pOutData->pData, pOutData->dataLen);
	pTmpStru->p[pOutData->dataLen] = '\0';

	pTmp = pTmp->next; //������һ���ڵ�
	DER_ITCAST_FreeQueue(pOutData);

	//���� plen
	ret = DER_ItAsn1_ReadInteger(pTmp, &(pTmpStru->plen));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead); //�ͷ�����
		DER_ITCAST_FreeQueue(pTmp);
		//����ṹ���е�����ֵʧ�ܣ��ͷŽṹ��ռ䣬������
		TeacherDecode_Free(&pTmpStru);
		printf("func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);

	*pStruct = pTmpStru; //��Ӹ�ֵ

	return ret;
}
//Teacher�ṹ�� �ͷ��ڴ�
int TeacherDecode_Free(Teacher **pStruct)
{
	Teacher *tmp = NULL;
	if (pStruct == NULL)
	{
		return 0;
	}

	tmp = *pStruct;

	if (tmp != NULL)
	{
		free(tmp->p);
		free(tmp);
	}
	*pStruct = NULL; // 1 ������ָ����ָ����ڴ���ͷŵ� 2 ͬʱ��ʵ�θ�ֵ NULL ����Ұָ��
	return 0;
}

//MsgKey_Req ��Կ������ �ṹ�� ����
int MsgKey_ReqEncode(MsgKey_Req *msgKey_Req , ITCAST_ANYBUF **outData){

	/*  �����ĸ�ʽ
	typedef struct _MsgKey_Req
	{
		//1 ��ԿЭ��  	//2 ��ԿУ��; 	//3 ��Կע��
		int				cmdType;		//���������� 
		char			clientId[12];	//�ͻ��˱��
		char			AuthCode[16];	//��֤��
		char			serverId[12];	//�������˱�� 
		char			r1[64];			//�ͻ��������
	}MsgKey_Req;
	*/

	ITCAST_ANYBUF *pTmp=NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF *pTmpBuf=NULL;
	int ret=0;
	ITCAST_ANYBUF *pOutData=NULL;
	
	// ���� cmdType
	ret = DER_ItAsn1_WriteInteger(msgKey_Req->cmdType, &pHeadBuf);
	if (ret != 0)
	{
		if(pHeadBuf != NULL) {
			DER_ITCAST_FreeQueue(pHeadBuf);
		}
		printf("DER_ItAsn1_WriteInteger ����cmdType error: %d\n", ret);
		return ret;
	}
	pTmp = pHeadBuf; //����ͷbuf

	//����ͻ��˱�� clientId[12]
		//1 �Ƚ� char  ���͵�name ת���� ITCAST_ANYBUF ����
	ret = DER_ITCAST_String_To_AnyBuf(&pTmpBuf, msgKey_Req->clientId, strlen(msgKey_Req->clientId));
	if (ret != 0)
	{	
		if(pTmpBuf != NULL) {
			DER_ITCAST_FreeQueue(pTmpBuf);
		}
		printf("DER_ITCAST_String_To_AnyBuf  msgKey_Req->clientId תΪITCAST_ANYBUF ���� error: %d\n", ret);
		return ret;
	}
		// 2 clientId  --> TLV
	ret = DER_ItAsn1_WritePrintableString(pTmpBuf, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		pTmp = NULL;
		printf("DER_ItAsn1_WritePrintableString �� clientId ���� error: %d\n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//����AuthCode[16] ��֤��
		//1 �Ƚ� char  ����AuthCode ת���� ITCAST_ANYBUF ���� 
	DER_ITCAST_FreeQueue(pTmpBuf); //����� pTmpBuf
	ret = DER_ITCAST_String_To_AnyBuf(&pTmpBuf, msgKey_Req->AuthCode, strlen(msgKey_Req->AuthCode));
	if (ret != 0)
	{
		if(pTmpBuf != NULL) {
			DER_ITCAST_FreeQueue(pTmpBuf);
		}
		printf("DER_ITCAST_String_To_AnyBuf  msgKey_Req->AuthCode תΪITCAST_ANYBUF ���� error: %d\n", ret);
		return ret;
	}
		// 2 AuthCode  --> TLV
	ret = DER_ItAsn1_WritePrintableString(pTmpBuf, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmp);
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("DER_ItAsn1_WritePrintableString �� msgKey_Req->AuthCode ���� error: %d\n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//����serverId[12] �������˱�� 
		//1 �Ƚ� char  ����serverId ת���� ITCAST_ANYBUF ���� 
	DER_ITCAST_FreeQueue(pTmpBuf); //����� pTmpBuf
	ret = DER_ITCAST_String_To_AnyBuf(&pTmpBuf, msgKey_Req->serverId, strlen(msgKey_Req->serverId));
	if (ret != 0)
	{
		if(pTmpBuf != NULL) {
			DER_ITCAST_FreeQueue(pTmpBuf);
		}
		printf("DER_ITCAST_String_To_AnyBuf  msgKey_Req->serverId תΪITCAST_ANYBUF ���� error: %d\n", ret);
		return ret;
	}
		// 2 serverId  --> TLV
	ret = DER_ItAsn1_WritePrintableString(pTmpBuf, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmp);
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("DER_ItAsn1_WritePrintableString �� msgKey_Req->serverId ���� error: %d\n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//����r1[64] �ͻ��������
		//1 �Ƚ� char  ����serverId ת���� ITCAST_ANYBUF ���� 
	DER_ITCAST_FreeQueue(pTmpBuf); //����� pTmpBuf
	ret = DER_ITCAST_String_To_AnyBuf(&pTmpBuf, msgKey_Req->r1, strlen(msgKey_Req->r1));
	if (ret != 0)
	{
		if(pTmpBuf != NULL) {
			DER_ITCAST_FreeQueue(pTmpBuf);
		}
		printf("DER_ITCAST_String_To_AnyBuf  msgKey_Req->r1 תΪITCAST_ANYBUF ���� error: %d\n", ret);
		return ret;
	}
		// 2 r1  --> TLV
	ret = DER_ItAsn1_WritePrintableString(pTmpBuf, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmp);
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("DER_ItAsn1_WritePrintableString �� r1 ���� error: %d\n", ret);
		return ret;
	}
	pTmp = pTmp->next;
	//��MsgKey_Req �ṹ����б���
	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		DER_ITCAST_FreeQueue(pTmp);
		if(pOutData != NULL) {
			DER_ITCAST_FreeQueue(pOutData);
		}
		printf("DER_ItAsn1_WriteSequence error: %d\n", ret);
		return ret;
	}
	*outData = pOutData;
	DER_ITCAST_FreeQueue(pHeadBuf);
	DER_ITCAST_FreeQueue(pTmp);
	return 0;
}
//MsgKey_Req ��Կ������ �ṹ�� ����
int MsgKey_ReqDecode(unsigned char *inData, int inLen, MsgKey_Req **msgKey_Req){
	ITCAST_ANYBUF *pHead=NULL , *pTmp=NULL;
	ITCAST_ANYBUF *tmpAnyBuf=NULL;
	ITCAST_ANYBUF *pOutData=NULL;
	int ret=0;
	MsgKey_Req *msgReq=NULL;
	/* 
	typedef struct _MsgKey_Req
	{
		//1 ��ԿЭ��  	//2 ��ԿУ��; 	//3 ��Կע��
		int				cmdType;		//���������� 
		char			clientId[12];	//�ͻ��˱��
		char			AuthCode[16];	//��֤��
		char			serverId[12];	//�������˱�� 
		char			r1[64];			//�ͻ��������
	}MsgKey_Req;
	 */
	//�򴫹�������unsigned char *inData,int inLen,������Ҫ����ת��Ϊbuf����
	// ת�� BER ���� unsigned char * --> ITCAST_ANYBUF
	ret = DER_ITCAST_String_To_AnyBuf(&tmpAnyBuf, inData, inLen);
	if (ret != 0)
	{
		if (tmpAnyBuf != NULL)
			DER_ITCAST_FreeQueue(tmpAnyBuf);

		printf(" Decode DER_ITCAST_String_To_AnyBuf error: %d\n", ret);
		return ret;
	}

	// �����MsgKey_Req �ṹ��
	ret = DER_ItAsn1_ReadSequence(tmpAnyBuf, &pHead);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(tmpAnyBuf);
		if(pHead != NULL) {
			DER_ITCAST_FreeQueue(pHead);
		}
		printf(" Decode DER_ItAsn1_ReadSequence error: %d\n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(tmpAnyBuf);
	// ��MsgKey_Req �ṹ�� malloc �ռ䡣
	if (msgReq == NULL)
	{
		msgReq = (MsgKey_Req *)malloc(sizeof(MsgKey_Req));
		if (msgReq == NULL)
		{
			DER_ITCAST_FreeQueue(pHead);
			ret = -1;
			printf("MsgKey_Req malloc error: %d\n", ret);
			return ret;
		}
		memset(msgReq, 0, sizeof(MsgKey_Req));
	}
	pTmp = pHead;

	//���� r1[64] �ͻ��������
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		pTmp = NULL;
		MsgKey_ReqDecode_Free(&msgReq);
		printf(" Decode DER_ItAsn1_ReadPrintableString msgReq error: %d\n", ret);
		return ret;
	}
	// ppPrintString -> pData; ---> msgReq
	memcpy(msgReq->r1, pOutData->pData, pOutData->dataLen);
	pTmp = pTmp->next;

	//serverId�������˱�� 
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmp);
		DER_ITCAST_FreeQueue(pHead);
		MsgKey_ReqDecode_Free(&msgReq);
		printf(" Decode DER_ItAsn1_ReadPrintableString serverId error: %d\n", ret);
		return ret;
	}
	// ppPrintString -> pData; ---> msgReq
	memcpy(msgReq->serverId, pOutData->pData, pOutData->dataLen);
	pTmp = pTmp->next;

	//AuthCode[16];	//��֤�� ����
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		DER_ITCAST_FreeQueue(pTmp);
		MsgKey_ReqDecode_Free(&msgReq);
		printf(" Decode DER_ItAsn1_ReadPrintableString AuthCode error: %d\n", ret);
		return ret;
	}
	// ppPrintString -> pData; ---> msgReq
	memcpy(msgReq->AuthCode, pOutData->pData, pOutData->dataLen);
	pTmp = pTmp->next;

	//	char clientId[12];	//�ͻ��˱��
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		DER_ITCAST_FreeQueue(pTmp);
		MsgKey_ReqDecode_Free(&msgReq);
		printf(" Decode DER_ItAsn1_ReadPrintableString clientId error: %d\n", ret);
		return ret;
	}
	// ppPrintString -> pData; ---> msgReq
	memcpy(msgReq->clientId, pOutData->pData, pOutData->dataLen);
	pTmp = pTmp->next;

	//	int	cmdType;		//����������  ����
	ret = DER_ItAsn1_ReadInteger(pTmp, &msgReq->cmdType);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		DER_ITCAST_FreeQueue(pTmp);
		MsgKey_ReqDecode_Free(&msgReq);
		printf(" Decode DER_ItAsn1_ReadInteger cmdType error: %d\n", ret);
		return ret;
	}
	*msgKey_Req = msgReq;
	DER_ITCAST_FreeQueue(pTmp);
	DER_ITCAST_FreeQueue(pHead);
	return 0;
}
//MsgKey_Req  ��Կ������ �ṹ�� �ͷ��ڴ�
int MsgKey_ReqDecode_Free( MsgKey_Req **msgKey_Req){
{
	if (msgKey_Req == NULL)
	{
		return 0;
	}
	if(* msgKey_Req != NULL) {
		* msgKey_Req = NULL; 
	}
	return 0;
}
}

//MsgKey_Res ��ԿӦ���� �ṹ�� ����
int MsgKey_ResEncode(MsgKey_Res *msgKey_Res , ITCAST_ANYBUF **outData){
	/* 
	typedef struct  _MsgKey_Res
	{
	int					rv;				//����ֵ
	char				clientId[12];	//�ͻ��˱��
	char				serverId[12];	//���������
	unsigned char		r2[64];			//�������������
	int					seckeyid;		//�Գ���Կ��� //modfy 2015.07.20
	}MsgKey_Res;
	 */
	ITCAST_ANYBUF *pTmp=NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF *pTmpBuf=NULL;
	int ret=0;
	ITCAST_ANYBUF *pOutData=NULL;
	
	// ���� rv ����ֵ
	ret = DER_ItAsn1_WriteInteger(msgKey_Res->rv , &pHeadBuf);
	if (ret != 0)
	{
		if(pHeadBuf != NULL) {
			DER_ITCAST_FreeQueue(pHeadBuf);
		}
		printf("DER_ItAsn1_WriteInteger ����rv error: %d\n", ret);
		return ret;
	}
	pTmp = pHeadBuf; //����ͷbuf

	//����ͻ��˱�� clientId[12]
		//1 �Ƚ� char  ���͵�name ת���� ITCAST_ANYBUF ����
	ret = DER_ITCAST_String_To_AnyBuf(&pTmpBuf, msgKey_Res->clientId, strlen(msgKey_Res->clientId));
	if (ret != 0)
	{
		if(pTmpBuf != NULL){
			DER_ITCAST_FreeQueue(pTmpBuf);
		}
		DER_ITCAST_FreeQueue(pHeadBuf);
		pTmp = NULL;
		printf("DER_ITCAST_String_To_AnyBuf  msgKey_Res->clientId תΪITCAST_ANYBUF ���� error: %d\n", ret);
		return ret;
	}
		// 2 clientId  --> TLV
	ret = DER_ItAsn1_WritePrintableString(pTmpBuf, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmp);
		pHeadBuf = NULL;
		DER_ITCAST_FreeQueue(pTmpBuf);
		printf("DER_ItAsn1_WritePrintableString �� clientId ���� error: %d\n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//����serverId[12] �������˱�� 
		//1 �Ƚ� char  ����serverId ת���� ITCAST_ANYBUF ���� 
	DER_ITCAST_FreeQueue(pTmpBuf); //����� pTmpBuf
	ret = DER_ITCAST_String_To_AnyBuf(&pTmpBuf, msgKey_Res->serverId, strlen(msgKey_Res->serverId));
	if (ret != 0)
	{
		if(pTmpBuf != NULL) {
			DER_ITCAST_FreeQueue(pTmpBuf);
		}
		DER_ITCAST_FreeQueue(pTmp);
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("DER_ITCAST_String_To_AnyBuf  msgKey_Res->serverId תΪITCAST_ANYBUF ���� error: %d\n", ret);
		return ret;
	}
		// 2 serverId  --> TLV
	ret = DER_ItAsn1_WritePrintableString(pTmpBuf, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmpBuf);
		DER_ITCAST_FreeQueue(pHeadBuf);
		DER_ITCAST_FreeQueue(pTmp);
		printf("DER_ItAsn1_WritePrintableString �� msgKey_Res->serverId ���� error: %d\n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//����r2[64] �ͻ��������
		//1 �Ƚ� char  ����r2ת���� ITCAST_ANYBUF ���� 
	DER_ITCAST_FreeQueue(pTmpBuf); //����� pTmpBuf
	ret = DER_ITCAST_String_To_AnyBuf(&pTmpBuf, msgKey_Res->r2, strlen(msgKey_Res->r2));
	if (ret != 0)
	{
		if(pTmpBuf != NULL) {
			DER_ITCAST_FreeQueue(pTmpBuf);
		}
		DER_ITCAST_FreeQueue(pTmp);
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("DER_ITCAST_String_To_AnyBuf  msgKey_Res->r2 תΪITCAST_ANYBUF ���� error: %d\n", ret);
		return ret;
	}
		// 2 r1  --> TLV
	ret = DER_ItAsn1_WritePrintableString(pTmpBuf, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmpBuf);
		DER_ITCAST_FreeQueue(pTmp);
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("DER_ItAsn1_WritePrintableString �� r2 ���� error: %d\n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//���� int seckeyid;	�Գ���Կ��� //modfy 2015.07.20
	ret = DER_ItAsn1_WriteInteger(msgKey_Res->seckeyid , &pTmp);
	if (ret != 0)
	{
		printf("DER_ItAsn1_WriteInteger ����seckeyid error: %d\n", ret);
		return ret;
	}
	//��MsgKey_Res �ṹ����б���
	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("DER_ItAsn1_WriteSequence error: %d\n", ret);
		return ret;
	}
	*outData = pOutData;
	DER_ITCAST_FreeQueue(pHeadBuf);
	DER_ITCAST_FreeQueue(pTmp);
	return 0;

}
//MsgKey_Res ��ԿӦ���� �ṹ�� ����
int MsgKey_ResDecode(unsigned char *inData, int inLen, MsgKey_Res **msgKey_Res){
	/* 
	typedef struct  _MsgKey_Res
	{
		int					rv;				//����ֵ
		char				clientId[12];	//�ͻ��˱��
		char				serverId[12];	//���������
		unsigned char		r2[64];			//�������������
		int					seckeyid;		//�Գ���Կ��� //modfy 2015.07.20
	}MsgKey_Res;
	 */
	ITCAST_ANYBUF *pHead=NULL , *pTmp=NULL;
	ITCAST_ANYBUF *tmpAnyBuf=NULL;
	ITCAST_ANYBUF *pOutData=NULL;
	int ret=0;
	MsgKey_Res *msgRes=NULL;
	
	//�򴫹�������unsigned char *inData,int inLen,������Ҫ����ת��Ϊbuf����
	// ת�� BER ���� unsigned char * --> ITCAST_ANYBUF
	ret = DER_ITCAST_String_To_AnyBuf(&tmpAnyBuf, inData, inLen);
	if (ret != 0)
	{
		if (tmpAnyBuf != NULL)
			DER_ITCAST_FreeQueue(tmpAnyBuf);

		printf(" Decode DER_ITCAST_String_To_AnyBuf error: %d\n", ret);
		return ret;
	}

	// �����MsgKey_Res �ṹ��
	ret = DER_ItAsn1_ReadSequence(tmpAnyBuf, &pHead);
	if (ret != 0)
	{
		if (tmpAnyBuf != NULL)
			DER_ITCAST_FreeQueue(tmpAnyBuf);
		printf(" Decode DER_ItAsn1_ReadSequence error: %d\n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(tmpAnyBuf);
	// ��MsgKey_Res �ṹ�� malloc �ռ䡣
	if (msgRes == NULL)
	{
		msgRes = (MsgKey_Res *)malloc(sizeof(MsgKey_Res));
		if (msgRes == NULL)
		{
			DER_ITCAST_FreeQueue(pHead);
			ret = -1;
			printf("MsgKey_Req malloc error: %d\n", ret);
			return ret;
		}
		memset(msgRes, 0, sizeof(MsgKey_Res));
	}
	pTmp = pHead;
	
	// int seckeyid �Գ���Կ���
	ret = DER_ItAsn1_ReadInteger(pTmp, &msgRes->seckeyid);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		pTmp = NULL;
		MsgKey_ResDecode_Free(&msgRes);
		printf(" Decode DER_ItAsn1_ReadPrintableString seckeyid error: %d\n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//���� r2[64] �ͻ��������
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		DER_ITCAST_FreeQueue(pTmp);
		MsgKey_ResDecode_Free(&msgRes);
		printf(" Decode DER_ItAsn1_ReadPrintableString msgRes error: %d\n", ret);
		return ret;
	}
	// ppPrintString -> pData; ---> msgReq
	memcpy(msgRes->r2, pOutData->pData, pOutData->dataLen);
	pTmp = pTmp->next;

	//serverId�������˱�� 
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		DER_ITCAST_FreeQueue(pTmp);
		MsgKey_ResDecode_Free(&msgRes);
		printf(" Decode DER_ItAsn1_ReadPrintableString serverId error: %d\n", ret);
		return ret;
	}
	// ppPrintString -> pData; ---> msgReq
	memcpy(msgRes->serverId, pOutData->pData, pOutData->dataLen);
	pTmp = pTmp->next;

	//clientId[12];	//�ͻ��˱��
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		DER_ITCAST_FreeQueue(pTmp);
		MsgKey_ResDecode_Free(&msgRes);
		printf(" Decode DER_ItAsn1_ReadPrintableString AuthCode error: %d\n", ret);
		return ret;
	}
	// ppPrintString -> pData; ---> msgReq
	memcpy(msgRes->clientId, pOutData->pData, pOutData->dataLen);
	pTmp = pTmp->next;

	//	int	rv //����ֵ;
	ret = DER_ItAsn1_ReadInteger(pTmp, &msgRes->rv);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		DER_ITCAST_FreeQueue(pTmp);
		MsgKey_ResDecode_Free(&msgRes);
		printf(" Decode DER_ItAsn1_ReadInteger cmdType error: %d\n", ret);
		return ret;
	}
	*msgKey_Res = msgRes;
	DER_ITCAST_FreeQueue(pHead);
	DER_ITCAST_FreeQueue(pTmp);
	return 0;
}
//MsgKey_Res ��ԿӦ���� �ṹ�� �ͷ��ڴ�
int MsgKey_ResDecode_Free( MsgKey_Res **msgKey_Res){
	if (msgKey_Res == NULL)
	{
		return 0;
	}
	if(* msgKey_Res != NULL) {
		* msgKey_Res = NULL; 
	}
	return 0;
}


// �� type ��  �ṹ����б��� ��װ��
int MsgEncode(
	void			*pStruct, /*in*/
	int				type,
	unsigned char	**outData, /*out*/
	int				*outLen)
{
	ITCAST_ANYBUF	*pHeadbuf = NULL, *pTemp = NULL;
	ITCAST_ANYBUF	*pOutData = NULL;
	int				ret = 0;

	if (pStruct == NULL && type < 0 || outData == NULL || outLen == NULL)
	{
		ret = KeyMng_ParamErr;
		printf("func MsgEncode() err:%d \n", ret);
		return ret;
	}

	//�Բ����е� int type ����
	ret = DER_ItAsn1_WriteInteger(type, &pHeadbuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("func DER_ItAsn1_WriteInteger()��err:%d \n", ret);
		return ret;
	}
	pTemp = pHeadbuf;

	//���ݴ����type���ͣ��Բ�ͬ�Ľṹ����б��� 
	switch (type)
	{
	case ID_MsgKey_Teacher:
		//����techer�ṹ��
		ret=TeacherEncode(pStruct , &(pTemp->next));
		if(ret != 0 ){
			DER_ITCAST_FreeQueue(pHeadbuf);
			pTemp = NULL;
			printf("func TeacherEncode() encode teacher stuct��err:%d \n", ret);
			return ret;
		}
		break;

	case ID_MsgKey_Req:
		//��Կ������  ����
		ret = MsgKey_ReqEncode(pStruct , &(pTemp->next));
		if(ret != 0 ){
			DER_ITCAST_FreeQueue(pHeadbuf);
			pTemp = NULL;
			printf("func MsgKey_ReqEncode() encode Msg_Req stuct��err:%d \n", ret);
			return ret;
		}
		break;

	case ID_MsgKey_Res:
		//��ԿӦ���� ����
		ret = MsgKey_ResEncode(pStruct , &(pTemp->next));
		if(ret != 0 ){
			DER_ITCAST_FreeQueue(pHeadbuf);
			pTemp = NULL;
			printf("func MsgKey_ResEncode() encode Msg_Res stuct��err:%d \n", ret);
			return ret;
		}
		break;

	default:
		ret = KeyMng_TypeErr;
		printf("��������ʧ��()��itype:%d err:%d \n", type, ret);
		break;
	}

	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("����ʧ��err:%d \n", ret);
		return ret;
	}

	//��type �� �ṹ�� ����һ�� TLV
	ret = DER_ItAsn1_WriteSequence(pHeadbuf, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		pTemp != NULL;
		printf("func DER_ItAsn1_WriteSequence()��err:%d \n", ret);
		return ret;
	}

	*outData = (unsigned char *)malloc(pOutData->dataLen);
	if (*outData == NULL)
	{
		DER_ITCAST_FreeQueue(pOutData);
		DER_ITCAST_FreeQueue(pHeadbuf);
		ret = KeyMng_MallocErr;
		printf("malloc()��err:%d \n", ret);
		return ret;
	}
	memcpy(*outData, pOutData->pData, pOutData->dataLen);
	*outLen = pOutData->dataLen;
	DER_ITCAST_FreeQueue(pOutData);
	DER_ITCAST_FreeQueue(pHeadbuf);
	return ret;
}

int MsgDecode(
	unsigned char *inData,/*in*/
	int           inLen,
	void          **pStruct /*out*/,
	int           *type /*out*/)
{
	ITCAST_ANYBUF		*pHeadBuf = NULL, *inAnyBuf = NULL;
	int					ret = 0;
	unsigned long		itype = 0;

	//�� ber ��ʽ���ֽ�����ת���� AnyBuf
	ret = DER_ITCAST_String_To_AnyBuf(&inAnyBuf, inData, inLen);
	if (ret != 0)
	{
		if(inAnyBuf != NULL){
			DER_ITCAST_FreeQueue(inAnyBuf);
		}
		printf("func DER_ITCAST_String_To_AnyBuf ���� ���� �� anybuf ʧ�� error : %d", ret);
		return ret;
	}

	ret = DER_ItAsn1_ReadSequence(inAnyBuf, &pHeadBuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(inAnyBuf);
		printf("func DER_ItAsn1_ReadSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(inAnyBuf);

	//���� type
	ret = DER_ItAsn1_ReadInteger(pHeadBuf, &itype);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf); 
		printf("func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}

	switch (itype)
	{
	case ID_MsgKey_Teacher:
		//���� ��ʦ�ṹ��
		ret = TeacherDecode(pHeadBuf->next->pData, pHeadBuf->next->dataLen, (Teacher **)pStruct);
		break;
	case ID_MsgKey_Req:
		//���� ������ 
		ret = MsgKey_ReqDecode(pHeadBuf->next->pData, pHeadBuf->next->dataLen, (MsgKey_Req **)pStruct);
		break;
	case ID_MsgKey_Res:
		//���� Ӧ����
		ret = MsgKey_ResDecode(pHeadBuf->next->pData, pHeadBuf->next->dataLen, (MsgKey_Res **)pStruct);
		break;
	default:
		ret = KeyMng_TypeErr;
		printf("itype:%dʧ�� :%d \n", itype, ret);
		break;
	}
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		return ret;
	}
	*type = itype;
	DER_ITCAST_FreeQueue(pHeadBuf);
	return ret;
}


int MsgMemFree(void **point, int type)
{
	if (point == NULL)
	{
		return 0;
	}

	if (type == 0) //�ͷ� Encode�����Ժ���ڴ��
	{ 
		if (*point) free(*point);
		*point = NULL;
		return 0;
	}

	switch (type)
	{
	case ID_MsgKey_Teacher:
		TeacherDecode_Free((Teacher **)point);
		break;
	case ID_MsgKey_Req:
		MsgKey_ReqDecode_Free((MsgKey_Req **)point);
		break;
	case ID_MsgKey_Res:
		MsgKey_ResDecode_Free((MsgKey_Res **)point);
		break;
	default:
		break;
	}

	return 0;
}
