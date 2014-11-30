#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <cmath>
#include <cstring>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define DOUBLEERROR 1e-07

/*
 * Joins two relations
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

Status Operators::Join(const string& result,           // Name of the output relation 
                       const int projCnt,              // Number of attributes in the projection
    	               const attrInfo projNames[],     // List of projection attributes
    	               const attrInfo* attr1,          // Left attr in the join predicate
    	               const Operator op,              // Predicate operator
    	               const attrInfo* attr2)          // Right attr in the join predicate
{
    /* Your solution goes here */

    Status status;
    AttrDesc attrDesc1;
    AttrDesc attrDesc2;
    status = attrCat->getInfo(string(attr1->relName), string(attr1->attrName), attrDesc1);

    if( status != OK){
        return status;
    }

    status = attrCat->getInfo(string(attr2->relName), string(attr2->attrName), attrDesc2);

    if( status != OK){
        return status;
    }

        
    int projLength = 0;

    AttrDesc projNamesDesc[ projCnt ];  

    for (int i = 0; i < projCnt; i++) {
        attrCat->getInfo(projNames[i].relName, projNames[i].attrName, projNamesDesc[i]);
        projLength += projNamesDesc[i].attrLen;
    }

    if (op == EQ){

        if(!attrDesc1.indexed && !attrDesc2.indexed){
            status = SMJ(result, projCnt, projNamesDesc, attrDesc1, op, attrDesc2, projLength);
        }else{
            status = INL(result, projCnt, projNamesDesc, attrDesc1, op, attrDesc2, projLength);
        }

    }else{

        status = SNL(result, projCnt, projNamesDesc, attrDesc1, op, attrDesc2, projLength);  
    }

    if( status != OK){
        return status;
    }
  

	return OK;
}

// Function to compare two record based on the predicate. Returns 0 if the two attributes 
// are equal, a negative number if the left (attrDesc1) attribute is less that the right 
// attribute, otherwise this function returns a positive number.
int Operators::matchRec(const Record& outerRec,     // Left record
                        const Record& innerRec,     // Right record
                        const AttrDesc& attrDesc1,  // Left attribute in the predicate
                        const AttrDesc& attrDesc2)  // Right attribute in the predicate
{
    int tmpInt1, tmpInt2;
    double tmpFloat1, tmpFloat2, floatDiff;

    // Compare the attribute values using memcpy to avoid byte alignment issues
    switch(attrDesc1.attrType)
    {
        case INTEGER:
            memcpy(&tmpInt1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof(int));
            memcpy(&tmpInt2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof(int));
            return tmpInt1 - tmpInt2;

        case DOUBLE:
            memcpy(&tmpFloat1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof(double));
            memcpy(&tmpFloat2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof(double));
            floatDiff = tmpFloat1 - tmpFloat2;
            return (fabs(floatDiff) < DOUBLEERROR) ? 0 : (floatDiff < 0?floor(floatDiff):ceil(floatDiff));

        case STRING:
            return strncmp(
                (char *) outerRec.data + attrDesc1.attrOffset, 
                (char *) innerRec.data + attrDesc2.attrOffset, 
                MAX(attrDesc1.attrLen, attrDesc2.attrLen));
    }

    return 0;
}
