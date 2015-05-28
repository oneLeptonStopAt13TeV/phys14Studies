def listOfeventId(file_ref, file_other):

    #read reference file
    #fref = open('ref_dump/T2tt_850_100_muons_uclaucsb.txt.sort')
    fref = open(file_ref)
    eId_ref = []
    for line in fref:
        values_ref = line.split()
        eId_ref.append((values_ref[1],values_ref[2]))

    #read our file
    #fstr = open('T2tt_850_100_mu_Strasbourg.dump.sort')
    fstr = open(file_other)
    eId_str = []
    for line in fstr:
         values_str = line.split()
         eId_str.append((values_str[1],values_str[2]))

    #check for event present in ref and not in strasbourg' dump
    eId_notFound = []
    for i,k in eId_ref:
   	found = False
	for j,l in eId_str:
		if i == j and k == l:
			found = True
			break
	if not found:
		eId_notFound.append((i,k))

    #print(eId_notFound)
    return eId_notFound
