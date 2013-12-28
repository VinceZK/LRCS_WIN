#pragma once
#include <vector>
#include "Constants.h"

typedef struct __declspec(dllexport) projectionRec {
	char          projectionName[MAX_NAME_LENGTH];                  /* projection name */
	char          primaryColumn[MAX_NAME_LENGTH];                   /* primaryColumn */
	char          belongTable[MAX_NAME_LENGTH];                     /* belongs to this table*/
	char          createTime[TIME_LENGTH];                          /* projection create time */
	char          alterTime[TIME_LENGTH];                           /* projection alter time */
} PROJECTION_REC;

typedef struct __declspec(dllexport) columnRec {
	char	      columnID[GUID_LENGTH];                            /* column ID GUID */
	char          columnName[MAX_NAME_LENGTH];                      /* column name */
	char          projectionName[MAX_NAME_LENGTH];                  /* projection name */
	int           columnPosition;                                   /* column position in the project*/
	int           dataType;                                         /* data type of the column*/
	int           dataLength;                                       /* data length of each value */
	bool		  sorted;                                           /* column is sorted or not */
	bool          contiguous;                                       /* column is contiguous or not */
	int           compressionType;                                  /* compression type: RLE, BITMAP, LZO*/
	char          createTime[TIME_LENGTH];                          /* column create time */
} COLUMN_REC;
using namespace std;

class __declspec(dllexport) Projection
{
public:
	Projection(char* projName_);
	~Projection();
	PROJECTION_REC* getProjectionRec();
	void setProjectionRec(PROJECTION_REC* projection_);
	vector<COLUMN_REC>* getProjColumns();
	void setProjColumns(vector<COLUMN_REC>* columns_);
	void truncateProjection();
	void deleteProjectcion();
	void addColumns(vector<COLUMN_REC>* columns_);
	void removeColumns(vector<COLUMN_REC>* columns_);
private:
	char* projName;
	PROJECTION_REC projRec;
	vector<COLUMN_REC> colRec;
};

