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
/// Map TTree into RNTuple
///
/// param[in] file - Unique pointer of input file. 
/// param[in] treeName - Name of TTree, given as directory location.
///
/// Creates a new ROOT file that houses the RNTuple created from original TTree
/// Checks for duplicates and any branch that contains a TObject. 

void treeReading(std::unique_ptr<TFile> & file,const char  treeName[]){
  //This function takes in the name of the tree, and the unique pointer of the 
  std::string newfileName = "RNT"+std::string(file->GetName());
  std::string rntupleName = "RNT"+std::string(treeName);
  int rnameSize = rntupleName.size();
  for(int i = 0; i < rnameSize - 1; i++){
    if(rntupleName[i]=='/'){
      rntupleName.replace(i, 1,"_");
    };
  };
  auto tree = file->Get<TTree>(treeName);
    for (auto branch : TRangeDynCast<TBranch>(*tree->GetListOfBranches())) {
      // The dynamic cast to TBranch should never fail for GetListOfBranches()
      assert(branch);

      // We assume every branch has a single leaf
      TLeaf *leaf = static_cast<TLeaf*>(branch->GetListOfLeaves()->First());
      //We check the type of each branch, and get a pointer referring to its TClass, 
      //If it is not a TClass, it returns a nullptr

      // To check if the object in question is a TObject, we simply check if the
      //type is not a nullptr, thus allowing us to see if the branch is related to 
      // a TObject. (Since nullptr is read as False)
      if(TClass::GetClass(leaf->GetTypeName())){
        tree->SetBranchStatus(branch->GetName(),0);
      };
    };
    //A root file is temporarily created to copy the TTree
    TFile * tempfile = new TFile("small.root", "recreate");
    TTree *outputtree = tree->CloneTree(-1,"fast");
    //First, we check if the file already exists, and then if the rntuple already exists in the file. 
    if (!gSystem->AccessPathName(newfileName.c_str())){
        std::unique_ptr<TFile> newfile{TFile::Open(newfileName.c_str(), "UPDATE")};
      if (newfile->FindKey(rntupleName.c_str()) == nullptr){
        newfile->Close();
        auto importer = ROOT::Experimental::RNTupleImporter::Create(outputtree, newfileName.c_str());
        importer->SetNTupleName(rntupleName.c_str());
        importer->Import();
      } 

    }
    else{
      auto importer = ROOT::Experimental::RNTupleImporter::Create(outputtree, newfileName.c_str());
      importer->SetNTupleName(rntupleName.c_str());
      importer->Import();
    }
  //Once the importing is done, then the Copy file is deleted. 
  tempfile->Close();
  gSystem->Unlink(tempfile->GetName());
  delete tempfile;
}





/////////////////////////////////////////////////////////////////////////////////
///Directory Iterator
///
/// param[in] file - Unique pointer of input file. 
/// param[in] dirName - The name of the directory, included any parent directories 
/// 
/// Iterates through a TDirectoryfile, searching for sub-directories or TTrees
/// If sub-directory found, searches the sub-directories. Occurs recursively through each one.  


void directoryIterator(std::unique_ptr<TFile> & file,const char dirName[]){
  TDirectory* dir = gFile->GetDirectory(dirName);
  // similar to fileIterator, except accounts for directory
  for (auto key12 : TRangeDynCast<TKey>(dir->GetListOfKeys())) {
    if(TClass::GetClass(key12->GetClassName())->InheritsFrom("TDirectoryFile")){
      //If TDirectory is found, then iterate through its contents, defined in DirectoryIterator
      auto destination = std::string(dirName)+"/"+std::string(key12->GetName());
      std::cout<<destination<<std::endl;
      directoryIterator(file,destination.c_str());
    }
    else if(TClass::GetClass(key12->GetClassName())->InheritsFrom("TTree")){
      std::cout<<"THIS WORKS";
      //If TTree is found, convert TTree to RNTuple, as described in treeReading
      auto destination = std::string(dirName)+"/"+std::string(key12->GetName());
      treeReading(file,destination.c_str());
    }
    else {
      std::unique_ptr<TFile> myFile{TFile::Open(newfileName.c_str(), "UPDATE")};
      auto destination = std::string(dirName)+"/"+std::string(key12->GetName());
      myFile->WriteObject(key1->ReadObj(), destination.c_str());
      myFile->Close();
    };
  };
};




/////////////////////////////////////////////////////////////////////////////////
///ROOT File iterator
///
/// param[in] file - Unique pointer of input file. 
///
/// 
/// Iterates through a file, looking for either TDirectories or TTrees
/// If TDirectory found, continues the search inside said Directory


void fileIterator(std::unique_ptr<TFile> & file){
  //iterate through all TKeys in the file 
  for (auto key1 : TRangeDynCast<TKey>(file->GetListOfKeys())) {
    //If TDirectory is found, then iterate through its contents, defined in DirectoryIterator
    if(TClass::GetClass(key1->GetClassName())->InheritsFrom("TDirectoryFile")){
      std::cout<<key1->GetName()<<std::endl;
      directoryIterator(file,key1->GetName());
      std::unique_ptr<TFile> myFile{TFile::Open(newfileName.c_str(), "UPDATE")};
      myFile->WriteObject(key1->ReadObj(), key1->GetName());
      myFile->Close();
    }
    else if(TClass::GetClass(key1->GetClassName())->InheritsFrom("TTree")){
      //If TTree is found, convert TTree to RNTuple, as described in treeReading
      std::cout<<"THIS WORKS";
      treeReading(file,key1->GetName());
    }
    else {
      std::unique_ptr<TFile> myFile{TFile::Open(newfileName.c_str(), "UPDATE")};
      myFile->WriteObject(key1->ReadObj(), key1->GetName());
      myFile->Close();
    };
  };
};







int main(int argc, char *argv[]) {
  std::unique_ptr<TFile> myFile{TFile::Open(argv[1], "UPDATE")};
  fileIterator(myFile);
  return 0;


}
