//
//  FitBit.cpp
//  HMM
//
//  Created by Mikhail Yudelson on 10/12/12.
//
//

#include "FitBit.h"

FitBit::FitBit(struct param *p) {
    this->nS = p->nO;
    this->nO = p->nS;
    this->nG = p->nG;
    this->nK = p->nK;
    this->tol = p->tol;
    this->PI = NULL;
    this->A = NULL;
    this->B = NULL;
    this->PIm1 = NULL;
    this->Am1 = NULL;
    this->Bm1 = NULL;
    this->gradPI = NULL;
    this->gradA = NULL;
    this->gradB = NULL;
    this->gradPIm1 = NULL;
    this->gradAm1 = NULL;
    this->gradBm1 = NULL;
    this->PIcopy = NULL;
    this->Acopy = NULL;
    this->Bcopy = NULL;
    this->dirPIm1 = NULL;
    this->dirAm1 = NULL;
    this->dirBm1 = NULL;
}

FitBit::~FitBit() {
//    if(this->PI != NULL) free(this->PI); // these are usually linked
//    if(this->A != NULL) free2DNumber(this->A, this->nS); // these are usually linked
//    if(this->B != NULL) free2DNumber(this->B, this->nS); // these are usually linked
    if(this->PIm1 != NULL) free(this->PIm1);
    if(this->Am1 != NULL) free2DNumber(this->Am1, this->nS);
    if(this->Bm1 != NULL) free2DNumber(this->Bm1, this->nS);
    if(this->gradPI != NULL) free(this->gradPI);
    if(this->gradA != NULL) free2DNumber(this->gradA, this->nS);
    if(this->gradB != NULL) free2DNumber(this->gradB, this->nS);
    if(this->gradPIm1 != NULL) free(this->gradPIm1);
    if(this->gradAm1 != NULL) free2DNumber(this->gradAm1, this->nS);
    if(this->gradBm1 != NULL) free2DNumber(this->gradBm1, this->nS);
    if(this->PIcopy != NULL) free(this->PIcopy);
    if(this->Acopy != NULL) free2DNumber(this->Acopy, this->nS);
    if(this->Bcopy != NULL) free2DNumber(this->Bcopy, this->nS);
    if(this->dirPIm1 != NULL) free(this->dirPIm1);
    if(this->dirAm1 != NULL) free2DNumber(this->dirAm1, this->nS);
    if(this->dirBm1 != NULL) free2DNumber(this->dirBm1, this->nS);
}

void FitBit::init(NUMBER* &a_PI, NUMBER** &a_A, NUMBER** &a_B) {
//    if(this->PI != NULL) {
        if(a_PI == NULL)
            a_PI = init1DNumber(this->nS);
        else
            toZero1DNumber(a_PI, this->nS);
//    }
//    if(this->A  != NULL) {
        if(a_A == NULL)
            a_A  = init2DNumber(this->nS, this->nS);
        else
            toZero2DNumber(a_A,  this->nS, this->nS);
//    }
//    if(this->B  != NULL) {
        if(a_B == NULL)
            a_B  = init2DNumber(this->nS, this->nO);
        else
            toZero2DNumber(a_B,  this->nS, this->nO);
//    }
}

void FitBit::linkPar(NUMBER *a_PI, NUMBER **a_A, NUMBER **a_B) {
    this->PI = a_PI;
    this->A  = a_A;
    this->B  = a_B;
}

void FitBit::toZero(NUMBER *a_PI, NUMBER **a_A, NUMBER **a_B) {
    if(this->PI != NULL) toZero1DNumber(a_PI, this->nS);
    if(this->A  != NULL) toZero2DNumber(a_A,  this->nS, this->nS);
    if(this->B  != NULL) toZero2DNumber(a_B,  this->nS, this->nO);
}

void FitBit::copy(NUMBER* &soursePI, NUMBER** &sourseA, NUMBER** &sourseB, NUMBER* &targetPI, NUMBER** &targetA, NUMBER** &targetB){
    if(this->PI != NULL) cpy1DNumber(soursePI, targetPI, this->nS);
    if(this->A  != NULL) cpy2DNumber(sourseA,  targetA,  this->nS, this->nS);
    if(this->B  != NULL) cpy2DNumber(sourseB,  targetB,  this->nS, this->nO);
}

void FitBit::add(NUMBER *soursePI, NUMBER **sourseA, NUMBER **sourseB, NUMBER *targetPI, NUMBER **targetA, NUMBER **targetB){
    if(this->PI != NULL) add1DNumbersWeighted(soursePI, targetPI, this->nS, 1.0);
    if(this->A  != NULL) add2DNumbersWeighted(sourseA,  targetA,  this->nS, this->nS, 1.0);
    if(this->B  != NULL) add2DNumbersWeighted(sourseB,  targetB,  this->nS, this->nO, 1.0);
}

void FitBit::destroy(NUMBER* &a_PI, NUMBER** &a_A, NUMBER** &a_B) {
    if(this->PI != NULL) free(a_PI);
    if(this->A  != NULL) free2DNumber(a_A, this->nS);
    if(this->B  != NULL) free2DNumber(a_B, this->nS);
    a_PI = NULL;
    a_A  = NULL;
    a_B  = NULL;
}

void FitBit::init(enum FIT_BIT_SLOT fbs){
    switch (fbs) {
        case FBS_PAR:
            init(this->PI, this->A, this->B);
            break;
        case FBS_PARm1:
            init(this->PIm1, this->Am1, this->Bm1);
            break;
        case FBS_GRAD:
            init(this->gradPI, this->gradA, this->gradB);
            break;
        case FBS_GRADm1:
            init(this->gradPIm1, this->gradAm1, this->gradBm1);
            break;
        case FBS_PARcopy:
            init(this->PIcopy, this->Acopy, this->Bcopy);
            break;
        case FBS_DIRm1:
            init(this->dirPIm1, this->dirAm1, this->dirBm1);
            break;
        default:
            break;
    }
}

void FitBit::toZero(enum FIT_BIT_SLOT fbs){
    switch (fbs) {
        case FBS_PAR:
            toZero(this->PI, this->A, this->B);
            break;
        case FBS_PARm1:
            toZero(this->PIm1, this->Am1, this->Bm1);
            break;
        case FBS_GRAD:
            toZero(this->gradPI, this->gradA, this->gradB);
            break;
        case FBS_GRADm1:
            toZero(this->gradPIm1, this->gradAm1, this->gradBm1);
            break;
        case FBS_PARcopy:
            toZero(this->PIcopy, this->Acopy, this->Bcopy);
            break;
        case FBS_DIRm1:
            toZero(this->dirPIm1, this->dirAm1, this->dirBm1);
            break;
        default:
            break;
    }
}

void FitBit::destroy(enum FIT_BIT_SLOT fbs){
    switch (fbs) {
        case FBS_PAR:
            destroy(this->PI, this->A, this->B);
            break;
        case FBS_PARm1:
            destroy(this->PIm1, this->Am1, this->Bm1);
            break;
        case FBS_GRAD:
            destroy(this->gradPI, this->gradA, this->gradB);
            break;
        case FBS_GRADm1:
            destroy(this->gradPIm1, this->gradAm1, this->gradBm1);
            break;
        case FBS_PARcopy:
            destroy(this->PIcopy, this->Acopy, this->Bcopy);
            break;
        case FBS_DIRm1:
            destroy(this->dirPIm1, this->dirAm1, this->dirBm1);
            break;
        default:
            break;
    }
}

void FitBit::get(enum FIT_BIT_SLOT fbs, NUMBER* &a_PI, NUMBER** &a_A, NUMBER** &a_B) {
    switch (fbs) {
        case FBS_PAR:
            a_PI = this->PI;
            a_A  = this->A;
            a_B  = this->B;
            break;
        case FBS_PARm1:
            a_PI = this->PIm1;
            a_A  = this->Am1;
            a_B  = this->Bm1;
            break;
        case FBS_GRAD:
            a_PI = this->gradPI;
            a_A  = this->gradA;
            a_B  = this->gradB;
            break;
        case FBS_GRADm1:
            a_PI = this->gradPIm1;
            a_A  = this->gradAm1;
            a_B  = this->gradBm1;
            break;
        case FBS_PARcopy:
            a_PI = this->PIcopy;
            a_A  = this->Acopy;
            a_B  = this->Bcopy;
            break;
        case FBS_DIRm1:
            a_PI = this->dirPIm1;
            a_A  = this->dirAm1;
            a_B  = this->dirBm1;
            break;
        default:
            break;
    }
}

void FitBit::copy(enum FIT_BIT_SLOT sourse_fbs, enum FIT_BIT_SLOT target_fbs) {
    NUMBER *soursePI;
    NUMBER **sourseA;
    NUMBER **sourseB;
    get(sourse_fbs, soursePI, sourseA, sourseB);
    NUMBER *targetPI;
    NUMBER **targetA;
    NUMBER **targetB;
    get(target_fbs, targetPI, targetA, targetB);
    
    copy(soursePI, sourseA, sourseB, targetPI, targetA, targetB);
}

void FitBit::add(enum FIT_BIT_SLOT sourse_fbs, enum FIT_BIT_SLOT target_fbs) {
    NUMBER *soursePI;
    NUMBER **sourseA;
    NUMBER **sourseB;
    get(sourse_fbs, soursePI, sourseA, sourseB);
    NUMBER *targetPI;
    NUMBER **targetA;
    NUMBER **targetB;
    get(target_fbs, targetPI, targetA, targetB);
    
    add(soursePI, sourseA, sourseB, targetPI, targetA, targetB);
}

bool FitBit::checkConvergence() {
	NUMBER critetion = 0;
	for(NPAR i=0; i<this->nS; i++)
	{
		if(this->PI != NULL) critetion += pow( this->PI[i]-this->PIm1[i], 2 )/*:0*/;
		for(NPAR j=0; (this->A != NULL) && j<this->nS; j++) {
			critetion += pow(this->A[i][j] - this->Am1[i][j],2);
		}
		for(NPAR k=0; (this->B != NULL) && k<this->nO; k++) {
			critetion += pow(this->B[i][k] - this->Bm1[i][k],2);
		}
	}
	return sqrt(critetion) < this->tol;
}

void FitBit::doLog10ScaleGentle(enum FIT_BIT_SLOT fbs) {
    // fbs - gradient or direction
    NUMBER *a_PI;
    NUMBER **a_A;
    NUMBER **a_B;
    get(fbs, a_PI, a_A, a_B);
	if(this->PI != NULL) doLog10Scale1DGentle(a_PI, this->PI, this->nS);
	if(this->A  != NULL) doLog10Scale2DGentle(a_A,  this->A,  this->nS, this->nS);
	if(this->B  != NULL) doLog10Scale2DGentle(a_B,  this->B,  this->nS, this->nO);
}

