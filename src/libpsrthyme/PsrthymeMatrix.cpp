//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <TKmatrix.h>
#include <stdexcept>
#include <TKlog.h>


// include header generated by makeheaders
#include "PsrthymeMatrix.hpp"

#if INTERFACE
#include <boost/shared_ptr.hpp>
#include <vector>
#include <sstream>
#include <inttypes.h>
class PsrthymeMatrix {
   private:
	  uint64_t x;
	  uint64_t y;
	  double ** matrix;
   public:
	  typedef boost::shared_ptr<PsrthymeMatrix>  Ptr;
	  PsrthymeMatrix(const uint64_t x);
	  PsrthymeMatrix(const uint64_t x, const uint64_t y);
	  ~PsrthymeMatrix();
	  double* &operator[](const uint64_t r){
		 return matrix[r];
	  }
	  std::string toSimpleString(){
		 std::stringstream ss;
		 ss << "("<<this->x<<","<<this->y<<")";
		 return ss.str();
	  }
	  double ** c_arr(){
		 return this->matrix;
	  }

	  const Ptr T() const{
		 Ptr p = Ptr(new PsrthymeMatrix(this->y,this->x));
		 for (uint64_t i=0; i<this->x; i++){
			for (uint64_t j=0; j<this->y; j++){
			   p->matrix[j][i]=this->matrix[i][j];
			}
		 }
		 return p;
	  }

	  const Ptr operator*(const Ptr m);
	  const Ptr operator*(const PsrthymeMatrix &m);
	  friend const Ptr operator*(const Ptr &m1, const Ptr &m2);
	  friend const std::vector<double> operator*(const Ptr &m, const std::vector<double> &vec);
	  const std::vector<double> operator*(const std::vector<double> &vec);
	  const std::vector<double> operator*(const boost::shared_ptr<double[]> vec);
	  void addDiagonal(std::vector<double> diag);
	  void addDiagonal(double diag);
	  void addCVF(std::vector<double> diag);
};

#endif
PsrthymeMatrix::PsrthymeMatrix(const uint64_t x){
   this->matrix=malloc_uinv(x);
   this->x=x;
   this->y=x;
}
PsrthymeMatrix::PsrthymeMatrix(const uint64_t x, const uint64_t y){
   this->matrix=malloc_blas(x,y);
   this->x=x;
   this->y=y;
}
PsrthymeMatrix::~PsrthymeMatrix(){
   free_uinv(this->matrix);
}

const PsrthymeMatrix::Ptr PsrthymeMatrix::operator*(const PsrthymeMatrix::Ptr m){
   if (this->y != m->x){
	  logerr("%s","Can't multiply these matricies");
	  logerr("%s",this->toSimpleString().c_str());
	  logerr("%s",m->toSimpleString().c_str());
	  throw std::invalid_argument("Can't multiply these matricies");
   }
   PsrthymeMatrix::Ptr out = PsrthymeMatrix::Ptr(new PsrthymeMatrix(this->x,m->y));
   TKmultMatrix(this->matrix,m->matrix,this->x, this->y, m->y,out->matrix);
   return out;
}
const PsrthymeMatrix::Ptr PsrthymeMatrix::operator*(const PsrthymeMatrix &mm){
   const PsrthymeMatrix* m=&mm;
   if (this->y != m->x)throw std::invalid_argument("Can't multiply these matricies");
   PsrthymeMatrix::Ptr out = PsrthymeMatrix::Ptr(new PsrthymeMatrix(this->x,m->y));
   TKmultMatrix(this->matrix,m->matrix,this->x, this->y, m->y,out->matrix);
   return out;
}

const std::vector<double> PsrthymeMatrix::operator*(const std::vector<double> &vec){
   if (this->y != vec.size()){
	  logerr("%s", "Can't multiply these matricies");
	  logerr("%s",this->toSimpleString().c_str());
	  logerr("%d",vec.size());
	  throw std::invalid_argument("Can't multiply these matricies");
   }
   std::vector<double> out;
   out.resize(this->x);
   TKmultMatrixVec(this->matrix,(double*) &vec[0],this->x, this->y, &out[0]);
   return out;
}


const std::vector<double> PsrthymeMatrix::operator*(const boost::shared_ptr<double[]> vec){
   std::vector<double> out;
   out.resize(this->x,0.0);
   TKmultMatrixVec(this->matrix,(double*) &vec[0],this->x, this->y, &out[0]);
   return out;
}



void PsrthymeMatrix::addDiagonal(std::vector<double> diag){
   if(this->x!=this->y)throw std::invalid_argument("Can't add diagonal of non-square matrix");
   for(uint64_t i=0;i<this->x; i++){
	  this->matrix[i][i] += diag[i];
   }
}
void PsrthymeMatrix::addDiagonal(double diag){
   if(this->x!=this->y){
	  logerr("Can't add diagonal of non-square matrix x=%ld y=%ld ",this->x,this->y);
	  throw std::invalid_argument("Can't add diagonal of non-square matrix");
   }
   for(uint64_t i=0;i<this->x; i++){
	  this->matrix[i][i] += diag;
   }
}

void PsrthymeMatrix::addCVF(std::vector<double> diag){
   if(x!=y)throw std::invalid_argument("Can't make cvm from cvf for non-square matrix");
   for(int64_t i=0;i<this->x; i++){
	  for(int64_t j=0;j<this->y; j++){
		 int64_t k=i-j;
		 if(k<0)k+=this->x;
		 this->matrix[i][j] = diag[k];
	  }
   }

}

const PsrthymeMatrix::Ptr operator*(const PsrthymeMatrix::Ptr &m1, const PsrthymeMatrix::Ptr &m2){
   return (*m1)*m2;
}

const std::vector<double> operator*(const PsrthymeMatrix::Ptr &m, const std::vector<double> &vec){
   return (*m)*vec;
}

