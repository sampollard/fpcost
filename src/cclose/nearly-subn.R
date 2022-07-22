library(ggplot2)
EPS_FLT <- 2^-24
EPS_DBL <- 2^-53
DEL_FLT <- 2^-150
MIN_NORMAL <- 2^-126 # Smallest positive normal number
mp <- c("#1b9e77", "#d95f02", "#7570b3", "#e7298a") # Colorbrewer2 3-class Dark2

df <- read.csv("nearly-subn.tsv", sep = '\t')
n <- nrow(df)
df$AbsErr <- abs(df$Result - df$x * df$y)
max_abserr <- max(df$AbsErr)
max_relerr <- max(na.omit(df$RelErr))
start_x <- min(df$x)
end_x <- max(df$x)
x_norm_lub <- min(df$x[df$Result > MIN_NORMAL])
stopifnot(abs((df$y[1] - 1e-4)/1e-4) < EPS_FLT)

# Absolute Error
p1 <- ggplot(df, aes(y = AbsErr, x = x)) +
    geom_bin2d(bins = 100) +
    geom_hline(yintercept = max_abserr, color = 'red', linetype = 'solid') +
    geom_text(data=data.frame(x = end_x*0.9, y = max_abserr),
              aes(x,y), label='max error', vjust=-0.5) +
    geom_hline(yintercept = DEL_FLT, color = mp[2], linetype = 'solid') +
    geom_text(data=data.frame(x = start_x, y = DEL_FLT),
              aes(x,y), label=expression(delta / 2), vjust=-0.5, hjust = -0.5) +
    geom_vline(xintercept = x_norm_lub, color = mp[1], linetype = 'dashed') +
    geom_text(data=data.frame(x = x_norm_lub, y = 0.6*max(df$AbsErr)),
              aes(x,y), label=expression(paste('x*', 10^-4, ' subnormal')),
	          angle = 90, vjust = -.5) +
    ylab("Absolute Error (linear)") +
    expand_limits(y = max_abserr * 1.1) +
    scale_y_continuous(
        breaks = seq(0, max(max_abserr, DEL_FLT), length.out = 5),
        labels = function(x) sprintf("%0.1e", x)) +
    labs(title = expression(paste("Single-Precision Multiply, x*", 10^-4)),
         caption = sprintf("n = %s, step = %0.3e\nmax error = %0.3e",
                           format(n, big.mark=","),
                           df$x[2]-df$x[1],
                           max_abserr)) +
    scale_x_continuous(
        breaks = seq(start_x, end_x, length.out = 4),
        labels = function(x) sprintf("%0.1e", (x)))

# Relative Error
p2 <- ggplot(df, aes(y = RelErr, x = x)) +
    geom_bin2d(bins = 100) +
    geom_hline(yintercept = max_relerr, color = 'red', linetype = 'solid') +
    geom_text(data=data.frame(x = end_x*0.9, y = max_relerr),
              aes(x,y), label='max error', vjust=-0.5) +
    geom_hline(yintercept = DEL_FLT, color = mp[2], linetype = 'solid') +
    geom_text(data=data.frame(x = 0.9*end_x, y = DEL_FLT),
              aes(x,y), label=expression(delta / 2), vjust=-0.5) +
    geom_hline(yintercept = EPS_FLT, color = 'blue', linetype = 'solid') +
    geom_text(data=data.frame(x = 0.9*end_x, y = EPS_FLT),
              aes(x,y), label=expression(epsilon / 2), vjust=-0.5) +
    geom_vline(xintercept = x_norm_lub, color = mp[1], linetype = 'dashed') +
    geom_text(data=data.frame(x = x_norm_lub, y = 1e-28),
              aes(x,y), label=expression(paste('x*', 10^-4, ' subnormal')),
	          angle = 90, vjust = -.5) +
    ylab("Relative Error (logarithmic)") +
    expand_limits(y = max_relerr * 5) +
    scale_y_log10() +
    # scale_y_continuous(
    #     breaks = seq(0, max_relerr, length.out = 5),
    #     labels = function(x) sprintf("%0.1e", x)) +
    labs(title = expression(paste("Single-Precision Multiply, x*", 10^-4)),
         caption = sprintf("n = %s, step = %0.3e\nmax error = %0.3e",
                           format(n, big.mark=","),
                           df$x[2]-df$x[1],
                           max_relerr)) +
    scale_x_continuous(
        breaks = seq(start_x, end_x, length.out = 4),
        labels = function(x) sprintf("%0.1e", (x)))

# Just some sanity checks
message(paste("number > eps: ", sum(na.omit(df$RelErr > EPS_FLT))))
message(sprintf("Range: [%.10e,%.10e]\n", min(df$x), max(df$x)))
message(sprintf("Number of zero-error: %d\n", sum(df$AbsErr == 0)))
message(paste("number normal > eps (Should be 0)",
              sum(df$RelErr[df$x > x_norm_lub] > EPS_FLT)))

# This is nice if we need to smoosh for a page limit
# ggsave(paste0("figures/nearly-subn.pdf"),
#        plot = arrangeGrob(p1, p2, nrow = 1, ncol = 2),
#        scale = 0.9,
#        height = 5,
#        width = 10)
ggsave(paste0("figures/nearly-subn-abs.pdf"),
       plot = p1,
       scale = 0.9,
       height = 4,
       width = 8)
ggsave(paste0("figures/nearly-subn-rel.pdf"),
       plot = p2,
       scale = 0.9,
       height = 4,
       width = 8)
