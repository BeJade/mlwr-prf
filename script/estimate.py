from math import log
from MLWE_security import MLWE_summarize_attacks, MLWEParameterSet

q = 65536
mlist = [4,8,16,32]
B = [1,2,4,8]

#n = 128
#klist = [5,6]

n = 256
klist = [3]

class ParameterSet:
    def __init__(self, n, m, ks, ke,  q, rqk, rqc, rq2):
        self.n = n
        self.m = m
        self.ks= ks
        self.ke = ke
        self.q = q 
        self.rqk = rqk
        self.rqc = rqc
        self.rq2 = rq2


def To_MLWE(kps):
     return MLWEParameterSet(kps.n, kps.m, kps.ke, kps.ks, kps.q)


def summarize(ps,fp):
    fp.write("params: " + str(ps.__dict__) + "\n")


####################################################################################################

for k in klist:
  for b in range (len(B)):
    filename = "out/n" + str(n) + "/q65536k" + str(k) + "B" + str(B[b]) + ".txt"

    for m in mlist:
      ps_recommended = ParameterSet(n, k, B[b], m, q, 0, 0, 0)

      fp = open(filename,'a')
      fp.write("\n")
      summarize(ps_recommended,fp)
      MLWE_summarize_attacks(fp,To_MLWE(ps_recommended))
      fp.write("\n--------------------\n")
      fp.close()

    fp = open(filename,'a')
    fp.write("====================\n\n")
    fp.close()

