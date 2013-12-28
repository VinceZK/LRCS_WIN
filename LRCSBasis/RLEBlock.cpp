#include "RLEBlock.h"

RLEBlock::RLEBlock(bool valSorted_) : Block() {
	triple = NULL;
	vp = NULL;
	utilityVP = NULL;
	valSorted = valSorted_;
	adjustedCurPos = 0;
	valsize = 0;
	createdTriple = false;
}

RLEBlock::RLEBlock(RLETriple* trip_, bool valSorted_) : Block() {
	if (trip_ == NULL)
		throw new CodingException("RLEBlock: Constructing with a NULL triple");
	triple = trip_;

	vp = NULL;
	utilityVP = NULL;
	if (trip_->value)
		setType(trip_->value);
	else
		valsize = 0;
	valSorted = valSorted_;
	adjustedCurPos = 0;
	createdTriple = false;
}
RLEBlock::RLEBlock(const RLEBlock& block_) : Block(block_)
{
	vp = NULL;
	utilityVP = NULL;
	setType(block_.vp);
	triple = block_.triple == NULL ? NULL : new RLETriple(*block_.triple);
	createdTriple = true;
	valSorted = block_.valSorted;
	adjustedCurPos = 0;
}

Block* RLEBlock::clone(Block& block_)
{
	Block* newBlock = new RLEBlock((RLEBlock&)block_);
	return newBlock;
}

RLEBlock::~RLEBlock()
{
	delete vp;
	delete utilityVP;
	if (createdTriple)
		delete triple;
}


// Iterator access to block
bool RLEBlock::hasNext() {
	return ((adjustedCurPos)<(triple->reps));
}

ValPos* RLEBlock::getNext() {
	if ((adjustedCurPos)<(triple->reps)) {
		vp->position = adjustedCurPos + triple->startPos;
		adjustedCurPos++;
		return vp;
	}
	else {
		throw new CodingException("RLEBlock: No next pair");
	}
}

ValPos* RLEBlock::peekNext() {
	//assert(triple != NULL);
	if (hasNext()) {
		utilityVP->position = adjustedCurPos + triple->startPos;
		return utilityVP;
	}
	else {
		throw new CodingException("RLEBlock: No next pair");
	}
}
// Zero indexed, gets the pair at this pos_
ValPos* RLEBlock::getPairAtLoc(unsigned int loc_) {
	if (triple == NULL) throw new UnexpectedException("RLEBlock: error triple is NULL");
	if ((loc_<triple->reps) && (loc_ >= 0)) {
		utilityVP->position = triple->startPos + loc_;
		return utilityVP;
	}
	else {
		throw new CodingException("RLEBlock: No pair at this position, position invalid.");
	}

	return utilityVP;
}

//Like getPairAtLoc except set the regular pair variable rather than 
//the utilityPair variable so can be called by getNext().
ValPos* RLEBlock::getPairAtLocNotUtility(unsigned int loc_) {
	if (triple == NULL) throw new UnexpectedException("RLEBlock: error triple is NULL");
	if ((loc_<triple->reps) && (loc_ >= 0)) {
		vp->position = triple->startPos + loc_;
		return vp;
	}
	else {
		throw new CodingException("RLEBlock: No pair at this position, position invalid.");
	}

	return vp;
}

bool RLEBlock::skipToLoc(unsigned int loc_) {
	if (triple == NULL) throw new UnexpectedException("RLEBlock: error triple is NULL");
	if ((loc_<triple->reps) && (loc_ >= 0)) {
		adjustedCurPos = loc_;
		return true;
	}
	else {
		throw new CodingException("RLEBlock: No pair at this position, position invalid.");
	}

	return false;
}

int RLEBlock::getCurrLoc() {
	return adjustedCurPos;
}
void RLEBlock::resetBlock() {
	adjustedCurPos = 0;
}
// return size of block (number of values)
int RLEBlock::getSize() {
	if (triple == NULL) throw new UnexpectedException("RLEBlock: error triple is NULL");
	return triple->reps;
}

//return buffer size in block
int RLEBlock::getSizeInBits() {
	throw new UnimplementedException("RLEBlock: Not sure what this means in light of the new type generality");
	//return sizeof(RLETriple)*8;	
}

ValPos* RLEBlock::getStartPair() {
	vp->position = triple->startPos;
	return vp;
}

unsigned int RLEBlock::getEndPosition() {
	return triple->startPos + triple->reps - 1;
}

bool RLEBlock::isOneValue() {
	return true;
}

bool RLEBlock::isValueSorted() {
	return valSorted;
}

bool RLEBlock::isPosSorted() {
	return true;
}

bool RLEBlock::isPosContiguous() {
	return true;
}
bool RLEBlock::isBlockValueSorted() {
	return true;
}
bool RLEBlock::isBlockPosSorted() {
	return true;
}

RLETriple* RLEBlock::getTriple()
{
	if (triple == NULL) throw new UnexpectedException("RLEBlock: error triple is NULL");
	return triple;
}

void RLEBlock::setTriple(RLETriple* trip_) {
	if (trip_ == NULL)
		throw new CodingException("RLEBlock: Setting with a NULL triple");
	if (createdTriple)
		delete triple;
	createdTriple = false;
	if ((triple == NULL) || (!triple->value)) {
		triple = trip_;
		if (trip_->value)
			setType(trip_->value);
		else {
			delete vp;
			delete utilityVP;
			vp = new NullValPos();
			utilityVP = new NullValPos();
		}
	}
	else {
		triple = trip_;
		vp->set(trip_->value->value);
		utilityVP->set(trip_->value->value);
	}
	adjustedCurPos = 0;
}

bool RLEBlock::isSplittable() {
	return true;
}

// We keep the low values and spill the high values
Block* RLEBlock::split(int npositions) {
	// Make sure the split is reasonable--both blocks non-empty
	//assert((unsigned int)npositions < triple->reps);

	// Create the triple for the leftover block
	RLETriple* leftoverTrip = new RLETriple(triple->value,
		triple->startPos + npositions,
		triple->reps - npositions);
	// Update our triple
	triple->reps = npositions;

	return new RLEBlock(leftoverTrip, true);
}
