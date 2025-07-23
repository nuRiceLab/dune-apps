#include "ROOT/RNTupleDS.hxx"
#include "ROOT/RNTupleImporter.hxx"
#include "ROOT/RNTupleReader.hxx"
#include "ROOT/RPageStorageFile.hxx"
#include "ROOT/RNTupleWriter.hxx"
#include "ROOT/RNTupleWriteOptions.hxx"
#include "ROOT/RNTupleReadOptions.hxx"
#include "ROOT/RDataFrame.hxx"
#include "exception"
#include "TKey.h"
#include "TFile.h"
#include "TROOT.h"
#include "TSystem.h"
#include "iostream"
#include "fstream" 
#include "ROOT/RNTupleWriteOptionsDaos.hxx"



/////////////////////////////////////////////////////////////////////////////////
///ROOT File iterator
///
/// param[in] treeFile - Unique pointer of file with TTrees
/// param[in] tupleFile - Unique pointer of file with RNTuples
/// 
/// Checks to see if these files exist, and see if they are not corrupted.
void treetupleComparison(std::unique_ptr<TFile> & treeFile,std::unique_ptr<TFile> & tupleFile[]){
  assert(treeFile && ! treeFile->IsZombie());
  assert(tupleFile && ! tupleFile->IsZombie());
  
};
int main(int argc, char *argv[]) {
    std::unique_ptr<TFile> oldf(TFile::Open(argv[1]));
    std::unique_ptr<TFile> newf(TFile::Open(argv[2]));
    treetupleComparison(oldf,newf)
    return 0
};