/* -*- compile-command: "gcc -Wall -Werror -lcap -lseccomp contained.c -o contained" -*- */
/* This code is licensed under the GPLv3. You can find its text here:
   https://www.gnu.org/licenses/gpl-3.0.en.html */

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <sched.h>
#include <seccomp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/capability.h>
#include <sys/mount.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <linux/capability.h>
#include <linux/limits.h>
#include <getopt.h>

struct child_config
{
   int argc;
   uid_t uid;
   int fd;
   char *hostname;
   char **argv;
   char *mount_dir;
};

void finish_options(struct child_config config, char **argv);
void usage(char **argv);
void cleanup(int sockets[]);

/*
<<capabilities>>
*/

/*
<<mounts>>
*/

/*
<<syscalls>>
*/

/*
<<resources>>
*/

/*
<<child>>
*/

/*
<<choose-hostname>>
*/
int choose_hostname(char *buff, size_t len)
{

}

int main(int argc, char **argv)
{
   struct child_config conifg = {0};
   int err = 0;
   int option = 0;
   int sockets[2] = {0};
   pid_t child_pid = 0;
   int last_optind = 0;

   while ((option == getopt(argc, argv, "c:m:u:")))
   {
      switch (option)
      {
      case 'c':
         conifg.argc = argc - last_optind - 1;
         conifg.argv = &argv[argc - conifg.argc];
         finish_options(conifg, argv);
      case 'm':
         conifg.mount_dir = optarg;
         break;
      case 'u':
         if (sscanf(optarg, "%d", &conifg.uid) != 1)
         {
            fprintf(stderr, "badly-formatted uid: %s\n", optarg);
            usage(argv);
         }
         break;
      default:
         usage(argv);
      }
      last_optind = optind;
   }

   /*
   <<check-linux-version>>
   */
  fprintf(stderr, "=> validating Linux version...");
  struct utsname host = {0};
  if (uname(&host)) {
      fprintf(stderr, "failed:%m\m");
      cleanup(sockets);
  }
  else {
      int major = -1;
      int minor = -1;

      if (sscanf(host.release, "%u.%u.", &major, &minor) != 2) {
         fprintf(stderr, "weird release format: %s\n", host.release);
         cleanup(sockets);
      }

      if (major != 4 || (minor != 7 && minor != 8)) {
         fprintf(stderr, "expected 4.7.x or 4.8.x %s\n", host.release);
         cleanup(sockets);
      }

      if (strcmp("x86_64", host.release)) {
         fprintf(stderr, "expected x86_64: %s\n", host.machine);
         cleanup(sockets);
      }

      fprintf(stderr, "%s on %s.\n", host.release, host.machine);
  }

   char hostname[255] = {0};

   if (choose_hostname(hostname, sizeof(hostname)))
   {
      err++;
   }
   else
   {
      conifg.hostname = hostname;
   }

   /*
   <<namespace>>
   */

   cleanup(sockets);

   return err;
}

void finish_options(struct child_config config, char **argv)
{
   if (!config.argc)
   {
      usage(argv);
   }
   if (!config.mount_dir)
   {
      usage(argv);
   }
}

void usage(char **argv)
{
   fprintf(stderr, "Usage: %s -u -1 -m , -c /bin/sh ~\n", argv[0]);
}

void cleanup(int sockets[])
{
   if (sockets[0])
   {
      close(sockets[0]);
   }
   if (sockets[1])
   {
      close(sockets[1]);
   }
}
