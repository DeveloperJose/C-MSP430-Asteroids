#include "str_util.h"

/*
 * From https://www.daniweb.com/programming/software-development/threads/11049/convert-int-to-string
 *
 * The itoa function is used to convert an int to a String 
 */
unsigned int strlen(const char *string) {
  const char *s;
  s = string;
  while (*s)
    s++;
  return s - string;
}

char *strrev(char *str) {
  char *p1, *p2;

  if (!str || !*str)
    return str;

  for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
    *p1 ^= *p2;
    *p2 ^= *p1;
    *p1 ^= *p2;
  }
  return str;
}

char *itoa(int n, char *s, int b) {
  static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  int i=0, sign;
  if ((sign = n) < 0)
    n = -n;
  do {
    s[i++] = digits[n % b];
  } while ((n /= b) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  return strrev(s);
}
