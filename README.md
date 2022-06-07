# LPUK-LCD
LCD Driver for UsluKukla V3.2 Booster

## writedata function limitations
writedata() function is limited to char input by the ST7066u. Any numerical data needs to be typecasted into char, which can be done by using [sprintf()](https://devdocs.io/c/io/fprintf) under <stdio.h>. Reference [main.c](https://github.com/TheCodeNugget/LPUK-LCD/blob/main/main.c) for usage.

Of Note: MSP430 family limits sprintf() to integers only any other numerical variable (double, float) needs to be typecasted into int.
