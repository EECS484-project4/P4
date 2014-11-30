#include "catalog.h"
#include "query.h"
#include "index.h"
#include <iostream>
#include <map>
#include <string.h>
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
using namespace std;


/*
 * Inserts a record into the specified relation
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

Status Updates::Insert(const string& relation,      // Name of the relation
                       const int attrCnt,           // Number of attributes specified in INSERT statement
                       const attrInfo attrList[])   // Value of attributes specified in INSERT statement
{
    /* Your solution goes here */
	
	int attrDescCnt;
	Status status;
	AttrDesc *AttrDescArray;
	map<string, attrInfo> attrInfoMap;

	HeapFile heapFile(relation, status);

	
	attrCat->getRelInfo(relation, attrDescCnt, AttrDescArray);

	if(attrCnt > attrDescCnt){
		return ATTRTOOLONG;
	}

	if(attrCnt < attrDescCnt){
		return ATTRNOTFOUND;
	}

	for(int i = 0; i < attrCnt; i++){
		attrInfoMap[string(attrList[i].attrName)] = attrList[i];
	}
	
	for(int i = 0; i < attrDescCnt; i++){
		if(attrInfoMap.find(string(AttrDescArray[i].attrName)) == attrInfoMap.end()){
			return ATTRNOTFOUND;
		}
	}

	Record record;
	
	RID outRid;	
	int recordSize;
	
	recordSize = AttrDescArray[attrDescCnt-1].attrOffset + AttrDescArray[attrDescCnt-1].attrLen;	

	record.data = malloc(recordSize);
	record.length = recordSize;

	for(int i = 0; i < attrDescCnt; i++){
		memcpy((char*)record.data + AttrDescArray[i].attrOffset, attrInfoMap[string(AttrDescArray[i].attrName)].attrValue, AttrDescArray[i].attrLen);	
	}

	status = heapFile.insertRecord(record, outRid);

	if(status != OK){
		return status;
	}


	// update index
	for(int i = 0;i < attrDescCnt; i++){
		if(AttrDescArray[i].indexed){
			
			Index index(AttrDescArray[i].relName, AttrDescArray[i].attrOffset, AttrDescArray[i].attrLen, (Datatype)AttrDescArray[i].attrType, 0, status);
			
			if(status != OK){
				return status;
			}
			index.insertEntry((char*)record.data + AttrDescArray[i].attrOffset, outRid);
	
		}
	}	
	

	// Utilities utilities;
	// status = utilities.Print(relation);

    return OK;
}
