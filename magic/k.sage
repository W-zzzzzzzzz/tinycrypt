

#!/usr/bin/sage

from sage.all import *

# arithmetic machinery
N = 8
X = GF(2).polynomial_ring().gen()
F = GF(2**8, name="a", modulus=X**8+X**4+X**3+X**2+1)
alpha = F.gen()

for x in xrange(1, 2**N):
    l = int(F.fetch_int(x)._log_repr())
    i, j = l % 17, floor(l / 17)
    print l
