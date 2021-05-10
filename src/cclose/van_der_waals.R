# Distribution of some van der Waal's constants
# Source:
# http://www2.ucdsb.on.ca/tiss/stretton/database/van_der_waals_constants.html
# Related to digits of precision: https://xkcd.com/2170/
atm_pa <- 101325.
bar_pa <- 100000.
vdw <- read.csv("constants.tsv", sep = "\t")
message(sprintf("a\nmax: %3.4f\tmin: %3.4f\tmean: %3.4f\n",
                max(vdw[,3]), min(vdw[,3]), mean(vdw[,3])))
message(sprintf("b\nmax: %3.6f\tmin: %3.6f\tmean: %3.6f\n",
                max(vdw[,4]), min(vdw[,4]), mean(vdw[,4])))
# a max: 96.5000	min: 0.0346     mean: 20.5955
# b max: 0.695200	min: 0.007340   mean: 0.131436

