#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include <assert.h>

using namespace std;
/* Consider using Operators::matchRec() defined in join.cpp
 * to compare records when joining the relations */
  
Status Operators::SMJ(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
 	 cout << "Algorithm: SM Join" << endl;

  	/* Your solution goes here */
	Status status;
	Status status1;
	Status status2;
	
	RID outRid;
    HeapFile heapFile(result, status);

	unsigned int numUnpinnedPages;
	numUnpinnedPages = bufMgr->numUnpinnedPages();
	unsigned int k = numUnpinnedPages * 0.8;
	unsigned int numBytesSorting = PAGESIZE * k;
	
	AttrDesc *AttrDescArray1;	
	AttrDesc *AttrDescArray2;
	
	int attrDescCnt1;	
	int attrDescCnt2;	

	attrCat->getRelInfo(attrDesc1.relName, attrDescCnt1, AttrDescArray1);
	attrCat->getRelInfo(attrDesc2.relName, attrDescCnt2, AttrDescArray2);

	unsigned int tupleLength1 = AttrDescArray1[attrDescCnt1-1].attrOffset + AttrDescArray1[attrDescCnt1-1].attrLen;
	unsigned int tupleLength2 = AttrDescArray2[attrDescCnt2-1].attrOffset + AttrDescArray2[attrDescCnt2-1].attrLen;	

	unsigned int maxItems1 = numBytesSorting / tupleLength1;
	unsigned int maxItems2 = numBytesSorting / tupleLength2;
	
	SortedFile sortedFile1(attrDesc1.relName, attrDesc1.attrOffset, attrDesc1.attrLen,  (Datatype)attrDesc1.attrType, maxItems1, status1);
	
	if(status1 != OK){
		return status1;
	}
	
	SortedFile sortedFile2(attrDesc2.relName, attrDesc2.attrOffset, attrDesc2.attrLen,  (Datatype)attrDesc2.attrType, maxItems2, status2);
	
	if(status2 != OK){
		return status2;
	}

	Record record1;
	Record record2;
	Record prevRecord1;
	
	status1 = sortedFile1.next(record1);
	status2 = sortedFile2.next(record2);

	sortedFile2.setMark();
	while(status1 == OK){
		
		while(status2 == OK){
			
			if(matchRec(record1, record2, attrDesc1, attrDesc2) == 0){
				// insert
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
				status2 = sortedFile2.next(record2);

			}else if(matchRec(record1, record2, attrDesc1, attrDesc2) > 0){
				status2 = sortedFile2.next(record2);
			}else{
				break;
			}
		}		
		prevRecord1 = record1;
		status1 = sortedFile1.next(record1);
		if( matchRec(prevRecord1, record1, attrDesc1, attrDesc1) != 0){			
			sortedFile2.setMark();
		}else{
			sortedFile2.gotoMark();
		}
	}

  	return OK;
}

