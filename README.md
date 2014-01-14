LRCS_WIN
========

LRCS Window Version
cd to ..\LRCS_WIN\Release, run LRCS_TST [args] to test
1. LRCS_TST 1 1 Query1S
First arg "1" means create the columns, "0" means not create
Second arg "1" means force split the columns from source csv file, "0" means not do that.
Third arg "Query1S" is the test suit name. Query1S creates column and running query with no data compression.

2. LRCS_TST 1 1 Query2S
Running a LZO query

3. LRCS_TST 1 1 Query3S
Running a bitmap compression query

4. LRCS_TST 1 1 Query4S
Running a RLE compression query.

5. LRCS_TST LogTst
Examples of how to use LRCS log utility

6. LRCS_TST ExceptionTst
Examples of how to handle errors 

7. LRCS_TST LRCSEnvTst
Examples of how to using LRCS enviroment configuration

8. LRCS_TST ProjMakerTst
Examples of how to create and use projection & column meta data
