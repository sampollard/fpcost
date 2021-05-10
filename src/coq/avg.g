@rnd = float<ieee_64, ne>;
# putting rnd before = means apply the round to each operation
x = rnd(xx);
y = rnd(yy);
avg1 rnd= (x + y) / 2;
avg2 rnd= x/2 + y/2;

# Logical property
{
	x in [0,1000] /\ y in [0,1] -> avg1 in ?
}

# {
# 	x in [0,1000] /\ y in [0,1] -> avg2 in ?
# }

# Hints

