#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include <assert.h>

/* 
 * Indexed nested loop evaluates joins with an index on the 
 * inner/right relation (attrDesc2)
 */

Status Operators::INL(const string& result,           // Name of the output relation
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // The projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // Length of a tuple in the output relation
{
  cout << "Algorithm: Indexed NL Join" << endl;

  /* Your solution goes here */

	Status status, status1, status2;
	Record record1, record2;
	RID outRid, outRid1, outRid2;
	HeapFile heapFile(result, status);

	HeapFileScan heapFileScan[] = { HeapFileScan(attrDesc1.relName, status1), HeapFileScan(attrDesc2.relName, status2) };
	AttrDesc attrDesc[] = {attrDesc1, attrDesc2};

	if(status1 != OK){
		return status1;
	}

	if(status2 != OK){
		return status2;
	}

	int idx1, idx2;
	if (attrDesc1.indexed){
		idx1 = 1;
	}else{
		idx1 = 0;
	}
	idx2 = 1 - idx1;

	Index index(attrDesc[idx2].relName, attrDesc[idx2].attrOffset, attrDesc[idx2].attrLen, (Datatype)attrDesc[idx2].attrType, 0, status);
	if (status != OK){
		return status;
	}

	status1 = heapFileScan[idx1].scanNext(outRid1, record1);

	void* attrValue;	
	while(status1 == OK) {

		attrValue = malloc(attrDesc[idx1].attrLen);
		memset((char *)attrValue, 0, attrDesc[idx1].attrLen);
		memcpy(attrValue, (char *) record1.data + attrDesc[idx1].attrOffset, attrDesc[idx1].attrLen);
		
		status2 = index.startScan(attrValue);
		status2 = index.scanNext(outRid2);

		while(status2 == OK) {

			heapFileScan[idx2].getRandomRecord(outRid2, record2);

			Record outputRecord;
			outputRecord.data = malloc(reclen);
			outputRecord.length = reclen;
			int attrOffset = 0;
			for (int i = 0; i < projCnt; i++) {	
				if (strcmp(attrDescArray[i].relName, attrDesc[idx1].relName) == 0) {
					memcpy((char *)outputRecord.data + attrOffset, (char *) record1.data + attrDescArray[i].attrOffset, attrDescArray[i].attrLen);
				}else if(strcmp(attrDescArray[i].relName, attrDesc[idx2].relName) == 0){
					memcpy((char *)outputRecord.data + attrOffset, (char *) record2.data + attrDescArray[i].attrOffset, attrDescArray[i].attrLen);
				}else{
					cout<<"relName not match"<<endl;
					assert(false);
				}
				attrOffset += attrDescArray[i].attrLen;
			}
			heapFile.insertRecord(outputRecord, outRid);

			status2 = index.scanNext(outRid2);
		}
		status1 = heapFileScan[idx1].scanNext(outRid1, record1);
	}

	status = heapFileScan[idx1].endScan();
	if(status != OK){
		return status;
	}
	
	status = index.endScan();
	if(status != OK){
		return status;
	}

  	return OK;
}

