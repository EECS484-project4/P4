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

	Utilities utilities;
	attrCat->getRelInfo(relation, attrDescCnt, AttrDescArray);

	for(int i = 0; i < attrCnt; i++){
		attrInfoMap[string(attrList[i].attrName)] = attrList[i];
	}
	
	Record record;
	
	RID outRid;	
	int recordSize;
	
	recordSize = AttrDescArray[attrDescCnt-1].attrOffset + AttrDescArray[attrDescCnt-1].attrLen;	

	record.data = malloc(recordSize);
	record.length = recordSize;

	cout<<"AttrDescArray: "<<endl;
	for(int i = 0; i < attrDescCnt; i++){
		cout<<"AttrDescArray["<<i<<"].relName = "<<AttrDescArray[i].relName<<endl;
		cout<<"AttrDescArray["<<i<<"].attrName = "<<AttrDescArray[i].attrName<<endl;
		cout<<"AttrDescArray["<<i<<"].attrOffset = "<<AttrDescArray[i].attrOffset<<endl;
		cout<<"AttrDescArray["<<i<<"].attrType = "<<AttrDescArray[i].attrType<<endl;
		cout<<"AttrDescArray["<<i<<"].attrLen = "<<AttrDescArray[i].attrLen<<endl;
		cout<<"AttrDescArray["<<i<<"].indexed = "<<AttrDescArray[i].indexed<<endl;
		cout<<"attrInfoMap[string(AttrDescArray[i].attrName)].attrName: "<<attrInfoMap[string(AttrDescArray[i].attrName)].attrName<<endl;	
		memcpy(record.data + AttrDescArray[i].attrOffset, attrInfoMap[string(AttrDescArray[i].attrName)].attrValue, AttrDescArray[i].attrLen);	
	}

	status = heapFile.insertRecord(record, outRid);

	// update index
	for(int i = 0;i < attrDescCnt; i++){
		if(AttrDescArray[i].indexed){
			
			Index index(AttrDescArray[i].relName, AttrDescArray[i].attrOffset, AttrDescArray[i].attrLen, (Datatype)AttrDescArray[i].attrType, 0, status);
			index.insertEntry(record.data + AttrDescArray[i].attrOffset, outRid);
	
		}
	}	
	
	
	// // error handling
	// if(attrCnt != attrDescCCnt){
	// 	cout<<"attrCnt != attrDescCCnt"<<endl;
	// 	// return
	// }

	// for(int i = 0; i < attrDescCnt; i++){
	// 	if(attrInfoMap.find(string(AttrDescArray[i].attrName)) == attrInfoMap.end()){
	// 		cout<<"Cannot find the corresponding attribute."<<endl; 
	// 		// return 
	// 	}
	// }

	// for(int i = 0; i < attrCnt; i++){
	// 	status = getInfo(attrList[i].relName, attrList[i].attrName, record);

	// 	if(status != OK){
	// 		return status;
	// 	}
	// }
	
	status = utilities.Print(relation);

    return OK;
}
