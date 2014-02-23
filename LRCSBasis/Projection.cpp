#include "Projection.h"
#include "BDBEnv.h"

Projection::Projection(const char* projName_)
{
	projName = projName_;
}


Projection::~Projection()
{
}

PROJECTION_REC* Projection::getProjectionRec(){
   return &projRec;
};

vector<COLUMN_REC>* Projection::getProjColumns(){
	return &colRec;
};

void Projection::setProjectionRec(PROJECTION_REC* projection_){
	projRec = *projection_;
}

void Projection::setProjColumns(vector<COLUMN_REC>* columns_){
	colRec = *columns_;
}

void Projection::truncateProjection(){

}
void Projection::deleteProjectcion(){

}
void Projection::addColumns(vector<COLUMN_REC>* columns_){

}
void Projection::removeColumns(vector<COLUMN_REC>* columns_){

}
