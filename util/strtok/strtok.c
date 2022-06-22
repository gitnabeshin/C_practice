#include <stdio.h>
#include <string.h>
 
int main(void)
{
  char *p, *token, *saveptr;
  char *key, *val, *saveptr2;
  char str[] = "key1=1,key2=2,key3=3";
 
  p = str;
  saveptr = NULL;
  printf("str = %s\n", str);
 
  while ((token = strtok_r(p, ",", &saveptr)) != NULL) {
    printf("----------------------\n");
    printf("token = %s\n", token);
    printf("saveptr = %s\n", saveptr);

    key = strtok_r(token, "=", &saveptr2);
    printf("  saveptr2 = %s\n", saveptr2);
    val = strtok_r(NULL, "=", &saveptr2);
    printf("  key = %s\n", key);
    printf("  val = %s\n", val);

    p = NULL;
  }

  return 0;
}