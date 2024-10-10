
1. test1   is a one-file testcase, which contains two kinds of bitcasts and mismatch caused by these two kinds of bitcasts. 
In naivetest.c we assume to use the most naive bitcast analysis, i.e. analyze all the bitcasts of type1 and type2, adding all the bitcasted function to jump table regardless of dataflow information. The proposed jump table and results are appended

2. test2  is a multifile test case. It shows that TyPM and dataflow analysis would help with better analysis result. See operation.c for more detail
We assume we use simple dataflow analysis and TyPM along with bitcast analysis. The proposed results and the analysis details can be found in operations.c