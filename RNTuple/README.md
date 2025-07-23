# ROOT TTree-to-RNTuple Converter
This repository is focused on the conversion from ROOT's TTree storage format to its new storage format, RNTuples. It contains a file that is meant to map TTrees to RNtuple, Importer.cpp, then the other file is meant to check the original file and the new file for any errors after importing it. This was made in ROOT 6.34.04.

## Importer
This program takes as an input, a ROOT file containing some amount of TTrees. It will search through the file and its subdirectories for any TTrees in the data. Once it finds a TTree, it will convert the TTree using ROOT's built-in RNTuple Importer, which only include the branches that are natively compatible with RNTuples. This will create a new ROOT file containing the RNTuple.

For future improvements, we hope to bring in the subdirectories within the original TTree. As of now, the names and hierarchy of directories will be maintained through the names of the RNTuples in the new file. Furthermore, with the release of ROOT 7, we hope to include conversions from an outdated data source and send it to the new one. 

## File Comparison

This programs takes as an input, two ROOT files, where one is the original ROOT file with the TTrees and the other is the new ROOT file with the RNTuples. It will make sure that the two files are viable, uncorrupted files. This is to make sure that the previous file has done its job correctly, not creating a completely invalid file.

For future improvements, we hope to add the ability to check each branch/field, of both TTrees and its corresponding RNTuple, seeing if both files contain the same data, and the same strucuture. This would allow the user to be more sure that the mapping was a success, with the exception of the columns that contain data not native to RNTuples. 
