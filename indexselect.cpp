#include "catalog.h"
#include "query.h"
#include "index.h"
#include <iostream>
#include <map>
#include <string.h>
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
Status Operators::IndexSelect(const string& result,       // Name of the output relation
                              const int projCnt,          // Number of attributes in the projection
                              const AttrDesc projNames[], // Projection list (as AttrDesc)
                              const AttrDesc* attrDesc,   // Attribute in the selection predicate
                              const Operator op,          // Predicate operator
                              const void* attrValue,      // Pointer to the literal value in the predicate
                              const int reclen)           // Length of a tuple in the output relation
{
	cout << "Algorithm: Index Select" << endl;

	/* Your solution goes here */
	Status status;
	RID outRid;
	Record record;

	HeapFile heapFile(result, status);
	if (status != OK){
		return status;
	}

	HeapFileScan heapFileScan(attrDesc->relName, status);
	if (status != OK){
		return status;
	}

	Index index(attrDesc->relName, attrDesc->attrOffset, attrDesc->attrLen, (Datatype)attrDesc->attrType, 0, status);
	if (status != OK){
		return status;
	}

	status = index.startScan(attrValue);	
	if (status != OK){
		return status;
	}

	status = index.scanNext(outRid);
	if (status != OK){
		return status;
	}
	while(status == OK){
		heapFileScan.getRandomRecord(outRid, record);
			
		Record outputRecord;
		outputRecord.data = malloc(reclen);
		outputRecord.length = reclen;
		int attrOffset = 0;
		for (int i = 0; i < projCnt; i++){	
			memcpy((char *)outputRecord.data + attrOffset,(char *) record.data + projNames[i].attrOffset, projNames[i].attrLen);
			attrOffset += projNames[i].attrLen;
		}
		
		heapFile.insertRecord(outputRecord, outRid);

		status = index.scanNext(outRid);
	}

	return OK;
}

