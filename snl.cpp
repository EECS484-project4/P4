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
			if(cmp < 0 ){
				return true;
			}else{
				return false;
			}
		case LTE:
			if(cmp <= 0 ){
				return true;
			}else{
				return false;
			}
		case EQ:
			if(cmp == 0 ){
				return true;
			}else{
				return false;
			}
		case GTE:
			if(cmp >= 0 ){
				return true;
			}else{
				return false;
			}
		case GT:
			if(cmp > 0 ){
				return true;
			}else{
				return false;
			}
		case NE:
			if(cmp != 0 ){
				return true;
			}else{
				return false;
			}		
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
	Status status;
	Status status1;
	Status status2;
	Record record1;
	Record record2;	
	RID outRid;
	RID outRid1;
	RID outRid2;
	HeapFile heapFile(result, status);

	HeapFileScan heapFileScan1(attrDesc1.relName, status);
	HeapFileScan heapFileScan2(attrDesc2.relName, status);	
	
	if(status != OK){
		return status;
	}

	int count1 = 0;
	int count2 = 0;

	if(heapFileScan1.getRecCnt() < heapFileScan2.getRecCnt()){
		status1 = heapFileScan1.scanNext(outRid1, record1);
		status2 = heapFileScan2.scanNext(outRid2, record2);	
		heapFileScan2.setMarker();
		while(status1 == OK) {
				count1++;
				status2 = heapFileScan2.gotoMarker(outRid2, record2);
				cout << "count2 = " << count2 << endl;
				count2 = 0;
			while(status2 == OK) {
				count2++;
				int cmp;
				cmp = matchRec(record1, record2, attrDesc1, attrDesc2);

				if(checkMatch(cmp, op) == true){

					Record outputRecord;
					outputRecord.data = malloc(reclen);
					outputRecord.length = reclen;
					int attrOffset = 0;
					for (int i = 0; i < projCnt; i++) {	
						if (strcmp(attrDescArray[i].relName, attrDesc1.relName) == 0) {
							memcpy((char *)outputRecord.data + attrOffset, (char *) record1.data + attrDescArray[i].attrOffset, attrDescArray[i].attrLen);
						}else if(strcmp(attrDescArray[i].relName, attrDesc2.relName) == 0){
							memcpy((char *)outputRecord.data + attrOffset, (char *) record2.data + attrDescArray[i].attrOffset, attrDescArray[i].attrLen);
						}else{
							cout<<"relName not match"<<endl;
							assert(false);
						}
						attrOffset += attrDescArray[i].attrLen;
					}
					heapFile.insertRecord(outputRecord, outRid);
				}
				status2 = heapFileScan2.scanNext(outRid2, record2);
			}
			status1 = heapFileScan1.scanNext(outRid1, record1);
		}
			
	}else{
		status1 = heapFileScan1.scanNext(outRid1, record1);
		status2 = heapFileScan2.scanNext(outRid2, record2);	
		heapFileScan1.setMarker();
		while(status2 == OK) {
				status1 = heapFileScan1.gotoMarker(outRid1, record1);
				cout << "count2 = " << count2 << endl;
				count2 = 0;
			while(status1 == OK) {
				count2++;
				int cmp;
				cmp = matchRec(record1, record2, attrDesc1, attrDesc2);

				if(checkMatch(cmp, op) == true){

					Record outputRecord;
					outputRecord.data = malloc(reclen);
					outputRecord.length = reclen;
					int attrOffset = 0;
					for (int i = 0; i < projCnt; i++) {	
						if (strcmp(attrDescArray[i].relName, attrDesc1.relName) == 0) {
							memcpy((char *)outputRecord.data + attrOffset, (char *) record1.data + attrDescArray[i].attrOffset, attrDescArray[i].attrLen);
						}else if(strcmp(attrDescArray[i].relName, attrDesc2.relName) == 0){
							memcpy((char *)outputRecord.data + attrOffset, (char *) record2.data + attrDescArray[i].attrOffset, attrDescArray[i].attrLen);
						}else{
							cout<<"relName not match"<<endl;
							assert(false);							
						}
						attrOffset += attrDescArray[i].attrLen;
					}
					heapFile.insertRecord(outputRecord, outRid);
				}
				status1 = heapFileScan1.scanNext(outRid1, record1);
			}
			status2 = heapFileScan2.scanNext(outRid2, record2);
		}
	}

	cout << "count1 = " << count1 << endl;

	Utilities utilities;
	utilities.Print(result);

  	return OK;
}

