MPI_Bcast implementation via binary tree
-
Each node sends data to 2 of it's direct childs. Because childs are working in parallel, broadcasting large amounts of data works faster (while bcasting large amount of data among n processes)
