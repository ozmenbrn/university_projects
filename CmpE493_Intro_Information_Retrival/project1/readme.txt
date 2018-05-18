It is written in python language. You can simpley run the info_retrival.py without 
giving any parameter from terminal by typing 'python info_retrival.py'.

It looks for the directory named "Dataset". And take all the files ends with ".sgm" insdie
of the directory. After the dictionary is created from dataset, no need to have dataset 
again. 

If the dictionary not exist, it builds the dictionary first. After dictionary is created,
inverted index is created from the file dictionary. Without having any dataset, dictionary
is enough to create inverted index file.

After all files are created, input needs be given. type 1 for conjunctive query, 2 for
phrase query, 3 for proximity query.

After query type is written, write the query to the console, it returns the Document id's
that needs to be found to the console by ascending order of the document ids.

When it finished, type 'y' to type query again. 