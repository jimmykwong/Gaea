/*
 *  Copyright Beijing 58 Information Technology Co.,Ltd.
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */
#include "byteHelper.h"
#include "structHelper.h"
#include "serializeList.h"
#include <objc/hash.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
void byteArrayPutData(array *outArray, const void *data, int dataLen) {
	if (data == NULL) {
		int i = 0;
		byteArrayPutData(outArray, &i, dataLen);
		printf("arrayPutData error data ------------------\n");
		return;
	}
	if (ceil(outArray->byteLength / 128.0) < ceil((outArray->byteLength + dataLen) / 128.0)) {
		int i = (int) ceil((outArray->byteLength + dataLen) / 128.0) * 128;
		if (outArray->data == NULL) {
			outArray->data = malloc(i);
		} else {
			outArray->data = realloc(outArray->data, i);
		}
	}
	memcpy(outArray->data + outArray->byteLength, data, dataLen);
	outArray->byteLength += dataLen;
}
void *byteArrayPopData(array *inArray, int dataLen) {
	if (dataLen == 0) {
		return NULL;
	}
	void *ret = inArray->data;
	if (inArray->byteLength < dataLen) {
		inArray->byteLength = 0;
		inArray->data = NULL;
		ret = NULL;
	} else {
		inArray->byteLength -= dataLen;
		inArray->data += dataLen;
	}
	return ret;
}
void structPutData(array *outArray, const void *data, int dataLen) {
	if (data == NULL) {
		int i = 0;
		byteArrayPutData(outArray, &i, dataLen);
		printf("arrayPutData error data ------------------\n");
		return;
	}
	memcpy(outArray->data + outArray->byteLength, data, dataLen);
	outArray->byteLength += dataLen;
}

void writeInt16(short data, array *outArray) {
	byteArrayPutData(outArray, &data, 2);
}
void writeInt32(int data, array *outArray) {
	byteArrayPutData(outArray, &data, 4);
}
void writeInt64(long long data, array *outArray) {
	byteArrayPutData(outArray, &data, 8);
}
int getObjectSize(int typeId) {

	int size = 0;
	switch (typeId) {
	case SERIALIZE_CHAR_N:
	case SERIALIZE_BOOL_N:
		size = 1;
		break;
	case SERIALIZE_SHORT_INT_N:
		size = sizeof(short);
		break;
	case SERIALIZE_INT_N:
		size = 4;
		break;
	case SERIALIZE_TIME_N:
		size = sizeof(time_t);
		break;
	case SERIALIZE_FLOAT_N:
		size = 4;
		break;
	case SERIALIZE_DOUBLE_N:
		size = 8;
		break;
	case SERIALIZE_LONG_LONG_N:
		size = 8;
		break;
	case SERIALIZE_ARRAY_N:
		size = sizeof(array);
		break;
	case SERIALIZE_LIST_N:
		size = sizeof(struct serialize_list);
		break;
	case SERIALIZE_MAP_N:
		size = sizeof(cache_ptr);
		break;
	case SERIALIZE_STRING_N:
		size = sizeof(void*);
		break;
	case SERIALIZE_VOID_N:
		size = sizeof(void*);
		break;
	default: {
		array *structInfo = objc_hash_value_for_key(structInfoMap, &typeId);
		if (structInfo) {
			structFieldInfo *sfi = structInfo->data;
			size = sfi->offset;
		}
	}
		break;
	}
	return size;
}
char IsPrimitive(int typeId) {
	char c = 0;
	switch (typeId) {
	case SERIALIZE_CHAR_N:
	case SERIALIZE_BOOL_N:
	case SERIALIZE_SHORT_INT_N:
	case SERIALIZE_INT_N:
	case SERIALIZE_FLOAT_N:
	case SERIALIZE_DOUBLE_N:
	case SERIALIZE_LONG_LONG_N:
		c = 1;
		break;
	}
	return c;
}
