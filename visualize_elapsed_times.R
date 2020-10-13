library(readr)
library(dplyr)
library(stringr)
library(ggplot2)
library(tidyr)
library(rstudioapi)
setwd(dirname(getActiveDocumentContext()$path))

data <- read_csv("elapsed_times.csv") %>%
  mutate(PROGRAM=str_split_fixed(as.character(PROGRAM), pattern="", n=2),
         PROGRAM=str_c(PROGRAM[,1], ".", PROGRAM[,2]),
         PROGRAM=as.factor(PROGRAM)) %>%
  select(-RESULT)

data <- data %>%
  pivot_longer(c(TIME_SAMPLING, TIME_DIST), names_to = "MODE", values_to = "TIME") %>%
  group_by(PROGRAM, NPROC, MODE) %>%
  summarize(TIME = min(TIME))

data %>%
  ggplot(aes(x=NPROC, y=TIME, color=PROGRAM, linetype=PROGRAM, shape=PROGRAM)) +
  geom_point() +
  geom_line() +
  facet_wrap(~MODE, scales = "free") +
  xlab("#Processes") +
  ylab("Time [sec]") +
  ggtitle("Wall-clock time for each subtask [minimum over 2 runs]")
