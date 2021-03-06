#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include <assert.h>

bool checkMatch(int cmp, Operator op)
{
	switch(op){
		case LT:
			return cmp < 0;
		case LTE:
			return cmp <= 0;
		case EQ:
			return cmp == 0;
		case GTE:
			return cmp >= 0;
		case GT:
			return cmp > 0;
		case NE:
			return cmp != 0;
		case NOTSET:
			break;
	}

	return false;

}


Status Operators::SNL(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
  	cout << "Algorithm: Simple NL Join" << endl;

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
	if( heapFileScan[0].getRecCnt() <= heapFileScan[1].getRecCnt() ) {
		idx1 = 1;
	}else{
		idx1 = 0;
	}
	idx2 = 1 - idx1;

	status1 = heapFileScan[idx1].scanNext(outRid1, record1);
	status2 = heapFileScan[idx2].scanNext(outRid2, record2);	
	heapFileScan[idx2].setMarker();
	while(status1 == OK) {
		status2 = heapFileScan[idx2].gotoMarker(outRid2, record2);
		while(status2 == OK) {

			int cmp;
			if (idx1 == 0){
				cmp = matchRec(record1, record2, attrDesc1, attrDesc2);
			}else{
				cmp = matchRec(record2, record1, attrDesc1, attrDesc2);
			}
			if(checkMatch(cmp, op) == true){
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
			}
			status2 = heapFileScan[idx2].scanNext(outRid2, record2);
		}
		status1 = heapFileScan[idx1].scanNext(outRid1, record1);
	}

  	return OK;
}

