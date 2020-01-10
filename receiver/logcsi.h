/*
 * =============================================================================
 *       Filename:  logcsi.h
 *
 *    Description:  header file for csi processing function
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

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define CSI_ST_LEN 23

typedef struct
{
  int real;
  int imag;
} COMPLEX;

typedef struct
{
  u_int64_t timestamp;   // h/w assigned time stamp

  u_int16_t channel;     // wireless channel (represented in Hz)
  u_int8_t bandwidth;    // channel bandwidth (0->20MHz, 1->40MHz)

  u_int8_t rate;         // transmission rate
  u_int8_t nr;           // number of receiving antenna
  u_int8_t nt;           // number of transmitting antenna
  u_int8_t nc;           // number of tones (subcarriers)
  u_int8_t noise;        // noise floor (to be updated)

  u_int8_t phyerr;       // phy error code (set to 0 if correct)

  u_int8_t rssi;         // rx frame RSSI
  u_int8_t rssi_0;       // rx frame RSSI [ctl, chain 0]
  u_int8_t rssi_1;       // rx frame RSSI [ctl, chain 1]
  u_int8_t rssi_2;       // rx frame RSSI [ctl, chain 2]

  u_int16_t payload_len; // payload length (bytes)
  u_int16_t csi_len;     // csi data length (bytes)
  u_int16_t buf_len;     // data length in buffer
} CSI;

inline bool is_big_endian()
{
  const unsigned int a = 0x1;
  const unsigned char b = *(unsigned char *)&a;
  if (b == 0)
  {
    return true;
  }
  return false;
}

inline int bit_convert(int data, int maxbit)
{
  if (data & (1 << (maxbit - 1)))
  {
    /* negative */
    data -= (1 << maxbit);
  }
  return data;
}

inline int open_csi_device()
{
  int fd;
  fd = open("/dev/CSI_dev", O_RDWR);
  return fd;
}

inline void close_csi_device(int fd)
{
  close(fd);
}

inline int read_csi_buf(unsigned char *buf_addr, int fd, int BUFSIZE)
{
  /* listen to the port
   * read when 1, a csi is reported from kernel
   *           2, time out
   */
  int cnt = read(fd, buf_addr, BUFSIZE);
  if (cnt)
    return cnt;
  else
    return 0;
}

void record_status(unsigned char *buf_addr, int cnt, CSI *csi_status)
{
  if (is_big_endian())
  {
    csi_status->timestamp =
        (((u_int64_t)buf_addr[0] << 56) & 0x00000000000000ff)
      | (((u_int64_t)buf_addr[1] << 48) & 0x000000000000ff00)
      | (((u_int64_t)buf_addr[2] << 40) & 0x0000000000ff0000)
      | (((u_int64_t)buf_addr[3] << 32) & 0x00000000ff000000)
      | (((u_int64_t)buf_addr[4] << 24) & 0x000000ff00000000)
      | (((u_int64_t)buf_addr[5] << 16) & 0x0000ff0000000000)
      | (((u_int64_t)buf_addr[6] << 8) & 0x00ff000000000000)
      | (((u_int64_t)buf_addr[7]) & 0xff00000000000000);
    csi_status->csi_len =
        ((buf_addr[8] << 8) & 0xff00)
      | (buf_addr[9] & 0x00ff);
    csi_status->channel =
        ((buf_addr[10] << 8) & 0xff00)
      | (buf_addr[11] & 0x00ff);
    csi_status->buf_len =
        ((buf_addr[cnt - 2] << 8) & 0xff00)
      | (buf_addr[cnt - 1] & 0x00ff);
    csi_status->payload_len =
        ((buf_addr[CSI_ST_LEN] << 8) & 0xff00)
      | ((buf_addr[CSI_ST_LEN + 1]) & 0x00ff);
  }
  else
  {
    csi_status->timestamp =
        (((u_int64_t)buf_addr[7] << 56) & 0x00000000000000ff)
      | (((u_int64_t)buf_addr[6] << 48) & 0x000000000000ff00)
      | (((u_int64_t)buf_addr[5] << 40) & 0x0000000000ff0000)
      | (((u_int64_t)buf_addr[4] << 32) & 0x00000000ff000000)
      | (((u_int64_t)buf_addr[3] << 24) & 0x000000ff00000000)
      | (((u_int64_t)buf_addr[2] << 16) & 0x0000ff0000000000)
      | (((u_int64_t)buf_addr[1] << 8) & 0x00ff000000000000)
      | (((u_int64_t)buf_addr[0]) & 0xff00000000000000);
    csi_status->csi_len =
        ((buf_addr[9] << 8) & 0xff00)
      | (buf_addr[8] & 0x00ff);
    csi_status->channel =
        ((buf_addr[11] << 8) & 0xff00)
      | (buf_addr[10] & 0x00ff);
    csi_status->buf_len =
        ((buf_addr[cnt - 1] << 8) & 0xff00)
      | (buf_addr[cnt - 2] & 0x00ff);
    csi_status->payload_len =
        ((buf_addr[CSI_ST_LEN + 1] << 8) & 0xff00)
      | (buf_addr[CSI_ST_LEN] & 0x00ff);
  }

  csi_status->phyerr = buf_addr[12];
  csi_status->noise = buf_addr[13];
  csi_status->rate = buf_addr[14];
  csi_status->bandwidth = buf_addr[15];
  csi_status->nc = buf_addr[16];
  csi_status->nr = buf_addr[17];
  csi_status->nt = buf_addr[18];

  csi_status->rssi = buf_addr[19];
  csi_status->rssi_0 = buf_addr[20];
  csi_status->rssi_1 = buf_addr[21];
  csi_status->rssi_2 = buf_addr[22];
}

void fill_csi_matrix(
    u_int8_t *csi_addr, int nr, int nt, int nc, COMPLEX (*csi_matrix)[3][114])
{
  /* init bits_left - we process 16 bits at a time */
  u_int8_t bits_left = 16;

  /* according to the h/w, we have 10 bit resolution
   * for each real and imag value of the csi matrix H
   */
  u_int32_t bitmask = (1 << 10) - 1;
  u_int32_t idx = 2;

  /* get 16 bits for processing */
  u_int32_t h_data = csi_addr[0] + (csi_addr[1] << 8);
  u_int32_t current_data = h_data & ((1 << 16) - 1);

  /* fill the matrix */
  u_int8_t c, t, r;
  int real, imag;

  /* loop for every subcarrier */
  for (c = 0; c < nc; c += 1)
  {
    /* loop for each tx antenna */
    for (t = 0; t < nt; t += 1)
    {
      /* loop for each rx antenna */
      for (r = 0; r < nr; r += 1)
      {
        /* bits number less than 10, get next 16 bits */
        if ((bits_left - 10) < 0)
        {
          h_data = csi_addr[idx] + (csi_addr[idx + 1] << 8);
          current_data += h_data << bits_left;
          bits_left += 16;
          idx += 2;
        }

        imag = current_data & bitmask;
        imag = bit_convert(imag, 10);
        csi_matrix[t][r][c].imag = imag;

        bits_left -= 10;
        current_data >>= 10;

        /* bits number less than 10, get next 16 bits */
        if ((bits_left - 10) < 0)
        {
          h_data = csi_addr[idx] + (csi_addr[idx + 1] << 8);
          current_data += h_data << bits_left;
          bits_left += 16;
          idx += 2;
        }

        real = current_data & bitmask;
        real = bit_convert(real, 10);
        csi_matrix[t][r][c].real = real;

        bits_left -= 10;
        current_data >>= 10;
      }
    }
  }
}

void record_csi_payload(unsigned char *buf_addr, CSI *csi_status, unsigned char *data_buf, COMPLEX (*csi_matrix)[3][114])
{
  int nr = csi_status->nr;
  int nt = csi_status->nt;
  int nc = csi_status->nc;
  u_int16_t payload_len = csi_status->payload_len;
  u_int16_t csi_len = csi_status->csi_len;
  u_int8_t *csi_addr = buf_addr + CSI_ST_LEN + 2;

  /* record the data to the data buffer*/
  int i;
  for (i = 1; i <= payload_len; i += 1)
  {
    data_buf[i - 1] = buf_addr[CSI_ST_LEN + csi_len + i + 1];
  }

  /* extract the CSI and fill the complex matrix */
  fill_csi_matrix(csi_addr, nr, nt, nc, csi_matrix);
}
