#include "catalog.h"
#include "query.h"
#include "index.h"
#include <iostream>
#include <map>
#include <string.h>
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

/* 
 * A simple scan select using a heap file scan
 */

Status Operators::ScanSelect(const string& result,       // Name of the output relation
                             const int projCnt,          // Number of attributes in the projection
                             const AttrDesc projNames[], // Projection list (as AttrDesc)
                             const AttrDesc* attrDesc,   // Attribute in the selection predicate
                             const Operator op,          // Predicate operator
                             const void* attrValue,      // Pointer to the literal value in the predicate
                             const int reclen)           // Length of a tuple in the result relation
{
	cout << "Algorithm: File Scan" << endl;
  
	/* Your solution goes here */
	Status status;
	Record record;
	RID outRid;
	
	HeapFile heapFile(result, status);
	if(status != OK){
		return status;
	}

	HeapFileScan heapFileScan(projNames[0].relName, status);
	
	if(status != OK){
		return status;
	}

	if(attrDesc != 0){
	   	status = heapFileScan.startScan(attrDesc->attrOffset, attrDesc->attrLen, (Datatype)(attrDesc->attrType), (char*)attrValue, op);		
		if(status != OK){
			return status;
		}
	}
	
	status = heapFileScan.scanNext(outRid, record);
	if(status != OK){
		return status;
	}

	cout<<"phase1"<<endl;

	while(status == OK){
		cout<<"phase2"<<endl;
		Record outputRecord;
		outputRecord.data = malloc(reclen);
		outputRecord.length = reclen;
		int attrOffset = 0;
		for (int i = 0; i < projCnt; i++){	
			memcpy((char *)outputRecord.data + attrOffset,(char *) record.data + projNames[i].attrOffset, projNames[i].attrLen);
			attrOffset += projNames[i].attrLen;
		}
		heapFile.insertRecord(outputRecord, outRid);
		status = heapFileScan.scanNext(outRid, record);
	}

	heapFileScan.endScan();

	Utilities utilities;
	cout<<"Begin: utilities.Print(result): "<<endl;
	utilities.Print(result);
	cout<<"End: utilities.Print(result): "<<endl;
	return status;
}


