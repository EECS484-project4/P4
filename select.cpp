#include "catalog.h"
#include "query.h"
#include "index.h"


using namespace std;

/*
 * Selects records from the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */
Status Operators::Select(const string & result,      // name of the output relation
	                 const int projCnt,          // number of attributes in the projection
		         const attrInfo projNames[], // the list of projection attributes
		         const attrInfo *attr,       // attribute used inthe selection predicate 
		         const Operator op,         // predicate operation
		         const void *attrValue)     // literal value in the predicate
{

	cout<<"result: "<<result<<endl;

	for(int i = 0; i < projCnt; i++){
		cout<<"projNames["<<i<<"].relName: "<<projNames[i].relName<<endl;
		cout<<"projNames["<<i<<"].attrName: "<<projNames[i].attrName<<endl;
		cout<<"projNames["<<i<<"].attrType: "<<projNames[i].attrType<<endl;
		cout<<"projNames["<<i<<"].attrLen: "<<projNames[i].attrLen<<endl;
	}	

	Status status;
	AttrDesc attrDesc;
	// sselect all attributes
	AttrDesc projNamesDesc[ projCnt ];	
	
	int projLength = 0;
	for (int i = 0; i < projCnt; i++) {
		attrCat->getInfo(projNames[i].relName, projNames[i].attrName, projNamesDesc[i]);
		projLength += projNamesDesc[i].attrLen;
	}

	if(attr == 0){
		ScanSelect(result, projCnt, projNamesDesc, 0, op, attrValue, projLength);

		return OK;
	}

	status = attrCat->getInfo(string(attr->relName), string(attr->attrName), attrDesc);
		
	if (op == EQ && attrDesc.indexed) {
		IndexSelect(result, projCnt, projNamesDesc, &attrDesc, op, attrValue, projLength);
	}
	else {
		ScanSelect(result, projCnt, projNamesDesc, &attrDesc, op, attrValue, projLength);
	}

	return OK;
}

