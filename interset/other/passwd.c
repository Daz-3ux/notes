/*
GET getpass() declartion from <unistd.h>
GET crypt() declartion from <unistd.h>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <errno.h>
#include <shadow.h>
#include <stdbool.h>

/*
gcc passwd.c -o sl -lcrypt
sudo ./sl
*/


int main(int argc, char **argv)
{
  char *username, *password;
  char *encrypted, *p;
  struct passwd *pwd;
  struct spwd *spwd;
  bool authOK;
  size_t len;
  long lnmax;
  // sysconf: get configuration information at run time
  lnmax = sysconf(_SC_LOGIN_NAME_MAX);  //71
  if(lnmax == -1) {
    lnmax = -256;
  }

  username = malloc(lnmax);
  if(username == NULL) {
    perror("malloc");
    exit(1);
  }
  printf("username: ");
  fflush(stdout);
  if(fgets(username,lnmax, stdin) == NULL) {
    exit(1);
  }
  
  len = strlen(username);
  if(username[len-1] == '\n') {
    username[len-1] = '\0';
  }
  
  pwd = getpwnam(username);
  if(pwd == NULL) {
    perror("getpwname");
    exit(1);
  }
  spwd = getspnam(username);
  if(spwd == NULL && errno == EACCES) {
    perror("no permission to read shadow password file");
    exit(1);
  }
  if(spwd != NULL) {
    pwd->pw_passwd = spwd->sp_pwdp;
  }

  password = getpass("Password: ");

  // encrypt password and erase cleartext version immediately
  encrypted = crypt(password, pwd->pw_passwd);
  for(p = password; *p != '\0'; *p++ = '\0') {
    ;
  }

  if(encrypted == NULL) {
    printf("Error: %d\n", errno);
    perror("crypt");
    exit(EXIT_FAILURE);
  }

  authOK = strcmp(encrypted, pwd->pw_passwd) == 0;
  if(!authOK) {
    printf("password mismatch");
    exit(EXIT_FAILURE);
  }

  printf("Successfully authenticated: UID=%ld\n", (long)pwd->pw_uid);

  exit(EXIT_SUCCESS);
}