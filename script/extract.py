#!/usr/bin/python

import math
import linecache

leng = 128

q = 65536
lgq = 16

#n = 128
#klist = [5,6]
#mlist = [4,8,16,32]

n=256
klist = [3]
mlist = [4,8,16,32]


B = [2,4,8]

fname = "reformat" + str(n) + ".txt"

def extract(idx):

	minparam = []

	minct = 9999999999

	f = open(fname,'a')

	k = klist[idx]

	for b in range(len(B)):
		filename = "out/n" + str(n) + "/q" + str(q) + "k" + str(klist[idx]) + "B" + str(B[b]) + ".txt"
		print filename

		line = 1

		for k in range (1):
			for m in mlist:
				data = []
				spt = []

				skip1 = False
				skip2 = False

				raw = linecache.getline(filename,line+1)
				spt = raw.split(" ")
				print line+1,spt
				data.append(spt[2][0:len(spt[2])-1])	# q
				data.append(spt[6][0:len(spt[6])-1])	# m
				data.append(spt[8][0:len(spt[8])-1])	# k
				data.append(spt[12][0:len(spt[12])-1])	# b [-b,...,-1,1,...,b]

				raw = linecache.getline(filename,line+3)
				spt = raw.split(" & ")
				if (len(spt) < 4) : print "primal",line+3,spt
				data.append(spt[4])
				data.append(spt[5][0:len(spt[5])-2])
				if ((int(spt[1]) == 5) or (int(spt[2]) == 50)) : skip1 = True


				extraline = 0
				raw = linecache.getline(filename,line+7)
				spt = raw.split(" & ")
				while len(spt) < 4:
					extraline += 1
					raw = linecache.getline(filename,line+7+extraline)
					spt = raw.split(" & ")
				data.append(spt[4])
				data.append(spt[5][0:len(spt[5])-2])
				if ((int(spt[1]) == 5) or (int(spt[2]) == 50)) : skip2 = True


				line += 10 + extraline


				dq = int(data[0])
				dm = int(data[1])
				dk = int(data[2])
				data[3] = int(math.log(2*int(data[3]),2))
                                dB = data[3]


				lgp = lgq - dB

				insize = dk * n * dB	# bits
				outsize = dm * n * lgp	# bits


				multi = int(math.floor(1.*outsize / insize))
				if multi < 2: continue

				multi = int(math.floor(math.log(multi,2)))
				multi = 2**multi

				if int(data[4]) < 128: continue
				if int(data[6]) < 128: continue

				data.append(insize)
				data.append(outsize)

				cost = int(dk * (dm/multi) * 3**(math.log((n / 16),2)) * 16 * 16)

				data.append(multi)
				data.append(cost)

				mem = int(math.ceil(dm * dk  * n * lgq / 8))		# bytes

				depth = int(math.ceil(leng / math.log(multi,2)))
				data.append("\t")
				data.append(mem)
				data.append(depth)

				tcost = cost * depth
				data.append(tcost)


				if (tcost < minct):
					minct = tcost
					minparam = []
					for i in range (len(data)):
						minparam.append(data[i])

				for i in range (len(data)):
					f.write(str(data[i]) + "\t")
				f.write("\n")

			line += 2

	f.close()


	if (minparam == []):
		print "empty minparam"
		return

	sec = minparam[4]
	if (minparam[6] < sec): sec = minparam[6]

	#print minparam
	if (minparam == []): return
	print minparam




########
# main #
########

f = open(fname,'w')
f.write("q \tm \tk \tb \tprimal \tprimal \tdual \tdual \tin \tout \tmulti \tcost \t\t \tmem \tdepth \ttcost \n");
f.close()

for i in range (len(klist)):
	extract(i)

