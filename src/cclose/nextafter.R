# These are the ones used in the background of my dissertation
library(ggplot2)
EPS_FLT <- 2^-24
EPS_DBL <- 2^-53
DEL_FLT <- 2^-149
MIN_NORMAL <- 2^-126 # Smallest positive normal single-precision float
# Colorbrewer2. [1]: Dark2, [2-4]: Set2, [5]: Pastel1
mp <- c("#1b9e77", "#66c2a5", "#fc8d62", "#8da0cb", "#ccebc5")

# NOTE: This takes forever otherwise and is not very useful.
df <- read.csv("nextafter.tsv", sep = '\t', nrows=100000)
n <- nrow(df)
max_relerr <- max(df$RelErr)
start_x <- min(df$x)

# XXX: I wanted to change this into an expression but am having difficulty
# Maybe try .(sprintf(...))?
cap_fst <- sprintf(
    "n = %s, max err = %0.3e\n%.1f <= y <= %.1f\n",
    format(n, big.mark=","),
    max_relerr,
    min(df$y), max(df$y))
cap_end <- sprintf("%.2e", max(df$x) - start_x)
p <- ggplot(df, aes(y = RelErr, x = x)) +
    geom_point(shape = "square", size = 0.25) +
    labs(title = "Relative Error of Double-Precision Multiply",
         caption = paste0(cap_fst, "2^-10 <= x <= 2^-10 +", cap_end)) +
    geom_hline(yintercept = max_relerr, color = 'red') +
    xlab(bquote('Difference from'~2^-10)) +
    ylab("Relative Error") +
    # theme(axis.text.x = element_text(angle = 30, hjust = 1, size = 10)) +
    scale_y_continuous(
        breaks = seq(0, max_relerr, length.out = 5),
        labels = function(x) sprintf("%0.1e", x)) +
    scale_x_continuous(
        breaks = seq(min(df$x), max(df$x), length.out = 4),
        labels = function(x) sprintf("%0.1e", (x-start_x)))
ggsave(paste0("figures/nextafter.pdf"),
    plot = p, scale = 0.9, height = 3.5, width = 7)

# geom_smooth(method = "lm", formula = y ~ x, color = "blue") +
#     # Add in a legend just for the correlation coefficient
df <- read.csv("sqrt.tsv", sep = '\t')
n <- nrow(df)
max_relerr <- max(df$RelErr)
start_x <- min(df$x)
p <- ggplot(df, aes(y = RelErr, x = x)) +
    geom_point(shape = "square", size = 0.5) +
    geom_hline(yintercept = max_relerr, color = 'red') +
    ylab("Relative Error") +
    labs(title = "Relative Error of Double-Precision Square Root",
         caption = sprintf("n = %s, max err = %0.3e",
                           format(n, big.mark=","),
                           max_relerr)) +
    scale_y_continuous(
        breaks = seq(0, max_relerr, length.out = 5),
        labels = function(x) sprintf("%0.1e", x))
ggsave(paste0("figures/sqrt.pdf"),
    plot = p, scale = 0.9, height = 3.5, width = 7)

# NOTE: It must be the case that y is fixed
dfn <- read.csv("nasty.tsv", sep = '\t')
n <- nrow(dfn)
max_relerr <- max(dfn$RelErr)
start_x <- min(dfn$x)
end_x <- max(dfn$x)
x_norm_lub <- min(dfn$x[dfn$Result > MIN_NORMAL])
p <- ggplot(dfn, aes(y = RelErr, x = x)) +
    geom_bin2d(bins = 100) +
    geom_hline(yintercept = max_relerr, color = 'red') +
    geom_text(data=data.frame(x = end_x*0.8, y = max_relerr), aes(x,y),
	          label='max error', vjust=1.0, hjust=0.0) +
    geom_hline(yintercept = EPS_FLT, color = 'blue') +
    geom_text(data=data.frame(x = end_x, y = EPS_FLT), aes(x,y),
	          label=expression(epsilon / 2), vjust=-0.5) +
    geom_vline(xintercept = x_norm_lub, color = mp[1], linetype = 'dashed') +
    geom_text(data=data.frame(x = x_norm_lub, y = 0.6*max(dfn$RelErr)),
              aes(x,y), label=expression(paste(x^2, ' subnormal')),
	          angle = 90, vjust = -.5) +
    ylab("Relative Error (linear)") +
    labs(title = expression(paste("Relative Error of Single-Precision Multiply, ", x^2)),
         caption = sprintf("n = %s, max err = %0.3e",
                           format(n, big.mark=","),
                           max_relerr)) +
    scale_y_continuous(
        breaks = seq(0, max_relerr, length.out = 5),
        labels = function(x) sprintf("%0.1e", x)) +
    scale_x_continuous(
        breaks = seq(start_x, end_x, length.out = 4),
        labels = function(x) sprintf("%0.1e", (x)))

message(sprintf("Range: [%.10e,%.10e]\n", start_x, end_x))
message(sprintf("Normal values ~ 0.9 (eps/2) = %d / %d\n",
		sum(abs((dfn[dfn$x >= x_norm_lub,]$RelErr-EPS_FLT)/EPS_FLT) > 0.9),
		sum(dfn$x > x_norm_lub)))
ggsave(paste0("figures/subn-relerr.pdf"),
    plot = p, scale = 0.9, height = 3.5, width = 7)
ggsave(paste0("figures/subn-relerr.png"),
    plot = p, scale = 0.9, height = 3.5, width = 7)

# Density plot normal
# XXX: I think these two are flat wrong -- error_ulps doesn't correspond to actual error
# error_ulps <- abs((dfn[dfn$x >= x_norm_lub,]$RelErr - EPS_FLT)/(2*EPS_FLT))
# mean_x <- mean(error_ulps)
# pd <- ggplot(data.frame(x=error_ulps), aes(x=x)) +
# 	geom_density(fill=mp[5], alpha=0.5) +
# 	geom_vline(aes(xintercept=mean_x)) +
# 	# Note: setting y = 1.5 is not very robust. Should look up ylim
# 	geom_text(data = data.frame(x = mean_x, y = 1.5), aes(x,y),
# 	          label = sprintf("mean = %.2f", mean_x),
# 	          angle = 90, vjust = -.5) +
#     labs(title = expression(paste("Error Distribution for Normal Single-Precision ", x^2)),
#          caption = sprintf("n = %s\nx : [%.2e, %.2e]",
# 	                       format(length(error_ulps), big.mark=","),
# 	                       x_norm_lub, max(dfn$x))) +
# 	ylab("Density") +
# 	xlab("Error (ulps)")
# Density plot subnormal.
# message("Running density for subnormal. This uses >16GB memory.")
# error_ulps <- abs((dfn[dfn$x < x_norm_lub,]$RelErr - EPS_FLT)/(2*EPS_FLT))
# mean_x <- mean(error_ulps)
# pd <- ggplot(data.frame(x=error_ulps), aes(x=x)) +
# 	geom_density(fill=mp[3], alpha=0.5) +
# 	geom_vline(aes(xintercept=mean_x)) +
# 	# Note: setting y = 1.5 is not very robust. Should look up ylim
# 	geom_text(data = data.frame(x = mean_x, y = 1.5), aes(x,y),
# 	          label = sprintf("mean = %.2f", mean_x),
# 	          angle = 90, vjust = -.5) +
#     labs(title = expression(paste("Error Distribution for Subnormal Single-Precision ", x^2)),
#          caption = sprintf("n = %s\nx : [%.2e, %.2e]",
# 	                       format(length(error_ulps), big.mark=","),
# 	                       min(dfn$x), x_norm_lub)) +
# 	ylab("Density") +
# 	xlab("Error (ulps)")
# ggsave(paste0("figures/density-subn.pdf"),
#     plot = pd, scale = 0.9, height = 3.5, width = 7)


# Density plot, normal, no absolute value to see if biased
dfn$Exact <- dfn$x * dfn$y
dfn$ulps <- (dfn$Exact - dfn$Result)/(dfn$Exact * EPS_FLT * 2)
ulps_norm <- data.frame(ulps=dfn[dfn$x >= x_norm_lub,]$ulps)
mean_ulps <- mean(ulps_norm$ulps)

pd <- ggplot(ulps_norm, aes(x=ulps)) +
	geom_density(fill=mp[5], alpha=0.5) +
	geom_vline(aes(xintercept=mean_ulps)) +
	# Note: setting y = 1.5 is not very robust. Should look up ylim
	geom_text(data = data.frame(x = mean_ulps, y = 0.75), aes(x,y),
	          label = sprintf("mean %.2e", mean_ulps),
	          angle = 90, vjust = -.5) +
    labs(title = expression(paste("Error Distribution for Normal Single-Precision ", x^2)),
         caption = sprintf("n = %s\nx : [%.2e, %.2e]",
	                       format(nrow(ulps_norm), big.mark=","),
	                       x_norm_lub, max(dfn$x))) +
	ylab("Density") +
	xlab("Error (ulps)")
ggsave(paste0("figures/density-normal.pdf"),
    plot = pd, scale = 0.9, height = 3.5, width = 7)

# Density plot, subnormal
# For speed, takes forever with 35,000,000 elements
# tmp <- sample(dfn[dfn$x < x_norm_lub,]$ulps, 2000000)
# ulps_subn <- data.frame(ulps=tmp)
ulps_subn <- data.frame(ulps=dfn[dfn$x < x_norm_lub,]$ulps)
mean_ulps <- mean(ulps_subn$ulps)
pd <- ggplot(ulps_subn, aes(x=ulps)) +
	geom_density(fill=mp[3], alpha=0.5) +
	geom_vline(aes(xintercept=mean_ulps)) +
	# Note: setting y = 1.5 is not very robust. Should look up ylim
	geom_text(data = data.frame(x = mean_ulps, y = 0.0), aes(x,y),
	          label = sprintf("mean %.2e", mean_ulps),
	          angle = 90, vjust = -.5, hjust = 0.0) +
    labs(title = expression(paste("Error Distribution for Subnormal Single-Precision ", x^2)),
         caption = sprintf("n = %s\nx : [%.2e, %.2e]",
	                       format(nrow(ulps_subn), big.mark=","),
	                       min(dfn$x), x_norm_lub)) +
	ylab("Density") +
	xlab("Error (ulps)")
ggsave(paste0("figures/density-subn.pdf"),
    plot = pd, scale = 0.9, height = 3.5, width = 7)

# dfs <- read.csv("squared.tsv", sep = '\t')
#dfs <- read.csv("prod.tsv", sep = '\t')

# Add in plotting of absolute error
df$AbsErr <- abs(df$Result - df$x * df$y)
max_abserr <- max(df$AbsErr)
p <- ggplot(df, aes(y = AbsErr, x = x)) +
    geom_bin2d(bins = 100) +
    geom_hline(yintercept = max_abserr, color = 'red') +
    geom_text(data=data.frame(x = end_x*0.9, y = max_abserr), aes(x,y), label='max error', vjust=-0.5) +
    geom_hline(yintercept = DEL_FLT, color = 'blue') +
    geom_text(data=data.frame(x = end_x, y = DEL_FLT), aes(x,y), label=expression(delta), vjust=1.0) +
    ylab("Absolute Error") +
    labs(title = sprintf("Absolute Error of Single-Precision Multiply, %.4f * x",
                         df$y[1]),
         caption = sprintf("n = %s, max err = %0.3e",
                           format(n, big.mark=","),
                           max_abserr)) +
    scale_y_continuous(
		limit  = c(0, DEL_FLT),
        breaks = seq(0, max(DEL_FLT, max_abserr), length.out = 5),
        labels = function(x) sprintf("%0.1e", x)) +
    scale_x_continuous(
        breaks = seq(start_x, end_x, length.out = 4),
        labels = function(x) sprintf("%0.1e", (x)))
message(sprintf("Number of zero-error: %d\nmax_abserr = %.13e\n",
	sum(df$AbsErr == 0), max_abserr))
ggsave(paste0("figures/subn-abserr.pdf"),
    plot = p, scale = 0.9, height = 5, width = 9)

df <- read.csv("notsonasty.tsv", sep = '\t')
n <- nrow(df)
max_relerr <- max(df$RelErr)
start_x <- min(df$x)
end_x <- max(df$x)
p <- ggplot(df, aes(y = RelErr, x = x)) +
    geom_bin2d(bins = 100) +
    geom_hline(yintercept = max_relerr, color = 'red') +
    geom_text(data=data.frame(x = end_x, y = max_relerr), aes(x,y),
              label='max error', vjust=1.0, hjust=1.0) +
    geom_hline(yintercept = EPS_FLT, color = 'blue') +
    geom_text(data=data.frame(x = end_x, y = EPS_FLT), aes(x,y),
              label='eps', vjust=-0.7, hjust=1) +
    ylab("Relative Error") +
    scale_y_continuous(
        breaks = seq(0, max_relerr, length.out = 5),
        labels = function(x) sprintf("%0.1e", x)) +
    labs(title = sprintf("Relative Error of Single-Precision Multiply, %.4f * x",
                         df$y[1]),
         caption = sprintf("n = %s, max err = %0.3e",
                           format(n, big.mark=","),
                           max_relerr)) +
    scale_x_continuous(
        breaks = seq(min(df$x), max(df$x), length.out = 4),
        labels = function(x) sprintf("%0.1e", (x)))
ggsave(paste0("figures/notsonasty.pdf"),
    plot = p, scale = 0.9, height = 4, width = 9)

# Absolute error, but with a specifically-chosen subset
# Looks cool with df[seq(1,nrow(df),256),]
df_c <- df[seq(1,nrow(df),10000),]
df_c$AbsErr <- abs(df_c$Result - df_c$x * df_c$y)
max_abserr <- max(df$AbsErr)
pc <- ggplot(df_c, aes(y = AbsErr, x = x)) +
    geom_bin2d(bins = 100) +
    geom_hline(yintercept = max(df_c$AbsErr), color = 'red') +
    geom_text(data=data.frame(x = max(df_c$x)*0.9, y = max(df_c$AbsErr)), aes(x,y),
              label='max error', vjust=-0.5) +
    geom_hline(yintercept = DEL_FLT, color = 'blue') +
    geom_text(data=data.frame(x = max(df_c$x), y = DEL_FLT), aes(x,y),
              label=expression(delta), vjust=1.0) +
    ylab("Absolute Error") +
    # scale_y_continuous(
    #     breaks = seq(0, max(max(df_c$AbsErr), DEL_FLT), length.out = 5),
    #     labels = function(x) sprintf("%0.1e", x)) +
    labs(title = sprintf("Absolute Error of Single-Precision Multiply, %.4f * x",
                         df_c$y[1]),
         caption = sprintf("n = %s, max err = %0.3e",
                           format(nrow(df_c), big.mark=","),
                           max_abserr)) +
    scale_y_continuous(
		limit = c(0, DEL_FLT),
        breaks = seq(0, max(DEL_FLT, max_abserr), length.out = 5),
        labels = function(x) sprintf("%0.1e", x)) +
    scale_x_continuous(
        breaks = seq(min(df_c$x), max(df_c$x), length.out = 4),
        labels = function(x) sprintf("%0.1e", (x)))
ggsave(paste0("figures/select-few.pdf"),
    plot = pc, scale = 0.9, height = 5, width = 9)

