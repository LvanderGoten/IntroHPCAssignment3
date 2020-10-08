library(readr)
library(dplyr)
library(stringr)
library(ggplot2)
library(rstudioapi)
setwd(dirname(getActiveDocumentContext()$path))

data <- read_csv("elapsed_times.csv") %>%
  mutate(PROGRAM=str_split_fixed(as.character(PROGRAM), pattern="", n=2),
         PROGRAM=str_c(PROGRAM[,1], ".", PROGRAM[,2]),
         PROGRAM=as.factor(PROGRAM))

data %>%
  ggplot(aes(x=NPROC, y=TIME, color=PROGRAM, linetype=PROGRAM, shape=PROGRAM)) +
  geom_point() +
  geom_line() +
  xlab("#Processes") +
  ylab("Time [sec]") +
  ggtitle("Wall-clock time for each subtask [averaged over 10 runs]")
