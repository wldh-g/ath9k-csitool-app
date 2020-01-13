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

#define BUFSIZE 16384

bool recording = true;

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
  FILE *log = NULL;
  if (1 == argc)
  {
    file_flag = 0;
    printf("NOTE:\n");
    printf("  To log CSI values in file, use below.\n");
    printf("  => %s [FILE_NAME]\n\n", argv[0]);
  } else if (
    (strncmp("--help", argv[1], 6) == 0)
    || (strncmp("help", argv[1], 4) == 0)
    || (strncmp("-h", argv[1], 2) == 0)
  ) {
    printf("Usage  : %s [FILE_NAME]\n", argv[0]);
    printf("Example: %s\n", argv[0]);
    printf("Example: %s test.dat\n", argv[0]);
    exit(0);
  } else {
    log = fopen(argv[1], "w");
    if (!log) {
      printf("Failed to open %s for write!\n", argv[1]);
      exit(errno);
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
  int read_result;
  int log_count = 0;
  unsigned char buf_addr[BUFSIZE + 2];
  unsigned char data_buf[1500];
  COMPLEX csi_matrix[3][3][114];
  CSI *csi_status = (CSI *)malloc(sizeof(CSI));
  size_t write_result;
  u_int16_t buf_len;

  printf("Receiving data... Press Ctrl+C to quit.\n");
  signal(SIGINT, sigHandler);
  setbuf(stdout, NULL);
  while (recording)
  {
    /* keep listening to the kernel and waiting for the csi report */
    read_result = read_csi_buf(&buf_addr[2], csi_device, BUFSIZE);

    if (read_result)
    {
      log_count += 1;

      /* fill the status struct with information about the rx packet */
      record_status(&buf_addr[2], read_result, csi_status);

      /* fill the payload buffer with the payload
       * fill the CSI matrix with the extracted CSI value
       */
      record_csi_payload(&buf_addr[2], csi_status, data_buf, csi_matrix);
      fprintf(
        stdout,
        "%d: rate(0x%02x) payload_len(%d) tx_cnt(%d) rx_cnt(%d) -> ",
        log_count, csi_status->rate, csi_status->payload_len,
        csi_status->nt, csi_status->nr
      );

      /* log the received data */
      if (file_flag)
      {
        buf_addr[0] = csi_status->buf_len & 0xFF;
        buf_addr[1] = csi_status->buf_len >> 8;
        write_result = fwrite(buf_addr, 1, csi_status->buf_len + 2, log);

        if (1 > write_result) {
          fprintf(stdout, "ERROR!\n");
          perror("fwrite");
          exit(1);
        } else {
          fprintf(stdout, "OK!\n");
        }
      }
    }
  }

  /* clean */
  if (file_flag) fclose(log);
  close_csi_device(csi_device);
  free(csi_status);

  return 0;
}
