# TestTask
A call center has N operators who receive calls and record them in text files (one file per operator, one line per record). The format of
the file records is: numeric ID, space, encoded message details, end of line. The call ID is unique and monotonically growing, i.e. if an
operator receives a call and gets ID1, other operator, when receives the next call, will get ID2 > ID1. At the end of the day all operator
files are copied to one host and a program is executed to merge these files into one file ordering the records by their ID.
