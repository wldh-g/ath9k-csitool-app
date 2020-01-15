/*
 * =============================================================================
 *       Filename:  logcsi.c
 *
 *    Description:  Here is an example for receiving CSI matrix
 *                  Basic CSI processing function is also implemented and called
 *                  Check logcsi.h for the detail of the processing function
 *        Version:  1.1
 *
 *         Author:  Yaxiong Xie
 *          Email:  <xieyaxiongfly@gmail.com>
 *   Organization:  WANDS group @ Nanyang Technological University
 *
 *  Refactored by:  Jio Gim <jio@wldh.org>
 *
 *   Copyright (c)  WANDS group @ Nanyang Technological University
 * =============================================================================
 */

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <termios.h>

#include "logcsi.h"

#define BUFSIZE 65536

bool recording = true;
int log_recv_count = 0;
int log_write_count = 0;

void sigHandler(int signo)
{
  recording = false;
}

int main(int argc, char *argv[])
{
  /* check is got privileged */
  if (geteuid() != 0) {
    printf("Run this with the root privilege for correct socket descriptor.\n");
    exit(0);
  }

  /* check usage and open file */
  bool file_flag = true;
  bool verbose_flag = false;
  char *file_name = NULL;
  FILE *log = NULL;
  if (1 == argc)
  {
    file_flag = false;
    printf("NOTE:\n");
    printf("  To log CSI values in file, use below.\n");
    printf("  => %s [FILE_NAME]\n\n", argv[0]);
  } else if (
    (strncmp("--help", argv[1], 6) == 0)
    || (strncmp("help", argv[1], 4) == 0)
    || (strncmp("-h", argv[1], 2) == 0)
  ) {
    printf("Usage  : %s [-v|--verbose] [FILE_NAME]\n", argv[0]);
    printf("Example: %s\n", argv[0]);
    printf("         Just see CSI log without saving them\n");
    printf("Example: %s test.dat\n", argv[0]);
    printf("         Write CSI log to test.dat\n");
    printf("Example: %s -v test.dat\n", argv[0]);
    printf("         Write CSI log to test.dat with printing details\n");
    exit(0);
  } else {
    file_name = argv[1];
    if (
      (strncmp("-v", argv[1], 2) == 0)
      || (strncmp("--verbose", argv[1], 9) == 0)
    ) {
      verbose_flag = true;
      if (argv[2] == NULL) {
        file_flag = false;
      } else {
        file_name = argv[2];
      }
    }
    if (file_flag) {
      log = fopen(file_name, "w");
      if (!log) {
        printf("Failed to open %s for write!\n", file_name);
        exit(errno);
      }
    }
  }

  /* open CSI_dev */
  int csi_device = open_csi_device();
  if (csi_device < 0)
  {
    if (log) fclose(log);
    perror("Failed to open CSI device!");
    exit(errno);
  }

  /* get CSI values */
  ssize_t read_size;
  unsigned char buf_addr[BUFSIZE + 2];
  CSISTAT *csi_status = (CSISTAT *)malloc(sizeof(CSISTAT));
  size_t write_size;
  const bool disp_info = !file_flag || verbose_flag;
  const char *okay_sign = disp_info ? " -> OK" : ".";
  const char *csi_broken_sign = disp_info ? " -> CSI Broken = Throw" : "C";
  const char *write_fail_sign = disp_info ? " -> Write Fail" : "W";

  printf("Receiving data... Press Ctrl+C to quit.\n");
  signal(SIGINT, sigHandler);
  setbuf(stdout, NULL);
  while (recording)
  {
    /* keep listening to the kernel and waiting for the csi report */
    read_size = read_csi_buf(&buf_addr[2], csi_device, BUFSIZE);

    if (read_size)
    {
      log_recv_count += 1;

      /* fill the status struct with information about the rx packet */
      record_status(&buf_addr[2], read_size, csi_status);

      if (disp_info) {
        fprintf(
          stdout,
          "%d (%dB): phyerr(%d) payload(%dB) csi(%dB) rate(0x%x) nt(%d) nr(%d) timestamp(%lld)",
          log_write_count, csi_status->buf_len, csi_status->phyerr,
          csi_status->payload_len, csi_status->csi_len, csi_status->rate,
          csi_status->nt, csi_status->nr,
          csi_status->timestamp
        );
      }

      /* log the received data */
      if (file_flag)
      {
        buf_addr[0] = csi_status->buf_len & 0xFF;
        buf_addr[1] = csi_status->buf_len >> 8;

        if (csi_status->nt == 0) {
          fprintf(stdout, csi_broken_sign);
        } else {
          write_size = fwrite(buf_addr, 1, csi_status->buf_len + 2, log);

          if (1 > write_size) {
            fprintf(stdout, write_fail_sign);
            perror("fwrite");
            recording = 0;
          } else {
            fprintf(stdout, okay_sign);
            log_write_count += 1;
          }
        }
      }

      if (disp_info) fprintf(stdout, "\n");
    }
  }

  /* clean */
  printf("\nReceived %d packets.\n", log_recv_count);
  if (file_flag) {
    printf("Wrote %d packets to \"%s\".\n", log_write_count, file_name);
    fclose(log);
  }
  close_csi_device(csi_device);
  free(csi_status);

  return 0;
}
