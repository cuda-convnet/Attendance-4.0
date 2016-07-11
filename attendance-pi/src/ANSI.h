#pragma once

//Resets all formatting to default
#define RESET	"\33[0m"

//Foreground colors
#define BLACK	"\33[30m"
#define RED		"\33[31m"
#define GREEN	"\33[32m"
#define YELLOW	"\33[33m"
#define BLUE	"\33[34m"
#define MAGENTA	"\33[35m"
#define CYAN	"\33[36m"
#define WHITE	"\33[37m"

//Background colors
#define BBLACK	"\33[40m"
#define BRED	"\33[41m"
#define BGREEN	"\33[42m"
#define BYELLOW	"\33[43m"
#define BBLUE	"\33[44m"
#define BMAGENTA	"\33[45m"
#define BCYAN	"\33[46m"
#define BWHITE	"\33[47m"

//Bold modifier
#define BOLD	"\33[1m"
#define ITALIC	"\33[2m"
#define UNDERLINE	"\33[3m"
#define INVERSE	"\33[7m"
#define STRIKE	"\33[9m"

//Logging message headers
#define INFO	"\r" RESET "[" CYAN  "INFO" RESET "] "
#define OKAY	"\r" RESET "[" GREEN "OKAY" RESET "] "
#define LOADING "\r" RESET "[" WHITE "----" RESET "] "
#define FAIL	"\r" RESET "[" RED   "FAIL" RESET "] "
#define WARN	"\r" RESET "[" YELLOW "WARN" RESET "] "
