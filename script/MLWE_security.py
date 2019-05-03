from math import *
from model_BKZ import *

log_infinity = 9999

STEPS_b = 5
STEPS_m = 5


class MLWEParameterSet:
    def __init__(self, n, d, m, k, q, distr="uniform"):
        self.n = n          # Ring Dimension
        self.d = d          # MLWE Dimension (over the ring)
        self.m = m          # Number of Ring-Samples
        self.k = k          # Error Parameter
        self.q = q          # Modulus
        self.distr = distr  # Type of distribution : binomial or uniform (parameter k)


def LWE_primal_cost(fp, q, n, m, s, b, cost_svp=svp_classical, verbose=False):
    """ Return the cost of the primal attack using m samples and blocksize b (infinity = fail)
    """
    if verbose:
        fp.write("Primal attacks uses block-size %d and %d samples \n"%(b, m))
    
    if s * sqrt(b) < BKZ_last_block_length(q, m, n, b):
        return cost_svp(b)
    else:
        return log_infinity


def LWE_dual_cost(fp, q, n, m, s, b, cost_svp=svp_classical, verbose=False):
    """ Return the cost of the dual attack using m samples and blocksize b (infinity = fail)
    """
    l = BKZ_first_length(q, n, m, b)
    tau = l * s / q
    log2_eps = - 2 * pi * pi * tau**2 / log(2)
    log2_R = max(0, - 2 * log2_eps - nvec_sieve(b))
    if verbose:
        fp.write("Dual attacks uses block-size %d and %d samples \n"%(b, m))
        fp.write("shortest vector used has length l=%.2f, q=%d, `l<q'= %d \n"%(l, q, l<q))
        fp.write("log2(epsilon) = %.2f, log2 nvector per run %.2f \n"%(log2_eps, nvec_sieve(b)))
    return cost_svp(b) + log2_R


def MLWE_optimize_attack(fp, q, n, max_m, s, cost_attack=LWE_primal_cost, cost_svp=svp_classical, verbose=True):
    """ Find optimal parameters for a given attack
    """
    best_cost = log_infinity
    best_m = log_infinity
    best_b = log_infinity
    for b in range(50, n+max_m, STEPS_b):
        if cost_svp(b) > best_cost:
            break
        for m in range(max(5, b-n), max_m, STEPS_m):
            cost = cost_attack(fp, q, n, m, s, b, cost_svp)
            if cost<best_cost:
                best_cost = cost
                best_m = m
                best_b = b

    cost_attack(fp, q, n, best_m, s, best_b, cost_svp=svp_classical, verbose=verbose)
    return (best_m, best_b, best_cost)


def check_eq(m_pc, m_pq, m_pp, fp):
    if (m_pc != m_pq):
        fp.write("m and b not equals among the three models \n")
    if (m_pq != m_pp):
        fp.write("m and b not equals among the three models \n")


def MLWE_summarize_attacks(fp, ps):
    """ Create a report on the best primal and dual BKZ attacks on an MLWE instance
    """
    q = ps.q
    n = ps.n * ps.d
    max_m = ps.n * ps.m
    
    if ps.distr=="binomial":
        s = sqrt(ps.k /2.)
    elif ps.distr=="uniform":
        k = ps.k
        s = sqrt(sum([i**2 for i in range(-k, k+1)])/(2*k))
    else:
        raise ValueError("Unknown distribution "+ps.distr)
    
    (m_pc, b_pc, c_pc) = MLWE_optimize_attack(fp, q, n, max_m, s, cost_attack=LWE_primal_cost, cost_svp=svp_classical, verbose=True)
    (m_pq, b_pq, c_pq) = MLWE_optimize_attack(fp, q, n, max_m, s, cost_attack=LWE_primal_cost, cost_svp=svp_quantum, verbose=False)
    (m_pp, b_pp, c_pp) = MLWE_optimize_attack(fp, q, n, max_m, s, cost_attack=LWE_primal_cost, cost_svp=svp_plausible, verbose=False)

    check_eq(m_pc, m_pq, m_pp, fp)
    check_eq(b_pc, b_pq, b_pp, fp)

    fp.write("Primal & %d & %d & %d & %d & %d \n"%(m_pq, b_pq, int(floor(c_pc)), int(floor(c_pq)), int(floor(c_pp))))

    (m_pc, b_pc, c_pc) = MLWE_optimize_attack(fp, q, n, max_m, s, cost_attack=LWE_dual_cost, cost_svp=svp_classical, verbose=True)
    (m_pq, b_pq, c_pq) = MLWE_optimize_attack(fp, q, n, max_m, s, cost_attack=LWE_dual_cost, cost_svp=svp_quantum, verbose=False)
    (m_pp, b_pp, c_pp) = MLWE_optimize_attack(fp, q, n, max_m, s, cost_attack=LWE_dual_cost, cost_svp=svp_plausible, verbose=False)

    check_eq(m_pc, m_pq, m_pp, fp)
    check_eq(b_pc, b_pq, b_pp, fp)

    fp.write("Dual & %d & %d & %d & %d & %d \n"%(m_pq, b_pq, int(floor(c_pc)), int(floor(c_pq)), int(floor(c_pp))))
    return (b_pq, int(floor(c_pc)), int(floor(c_pq)), int(floor(c_pp)))
