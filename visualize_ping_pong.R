library(readr)
library(dplyr)
library(stringr)
library(ggplot2)
library(tidyr)
library(rstudioapi)
setwd(dirname(getActiveDocumentContext()$path))

data.single <- read_tsv("ping_pong_single_node.dat", col_names = c("MessageSize", "Time"))
data.dual <- read_tsv("ping_pong_dual_node.dat", col_names = c("MessageSize", "Time"))
data <- bind_rows(list(Single = data.single, Dual = data.dual), .id = "Mode")
data <- data %>%
  mutate(LogMessageSize = log2(MessageSize))

data %>%
  ggplot(aes(x = MessageSize, y = Time)) +
  geom_line() +
  facet_wrap(~Mode) +
  xlab("Message size [bytes]") +
  ylab("Time [sec]")

# Linear regression
lm(Time ~ MessageSize, data=data)
