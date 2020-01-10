/*
 * =============================================================================
 *  Original Name:  sendData.c
 *       Filename:  inject.c
 *
 *    Description:  send packets
 *        Version:  1.0
 *
 *         Author:  Yaxiong Xie
 *          Email:  <xieyaxiongfly@gmail.com>
 *   Organization:  WANS group @ Nanyang Technological University
 *
 *    Modified by:  Jio Gim
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * =============================================================================
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/random.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/* Define the defult destination MAC address */
#define DEFAULT_DEST_MAC0 0x12
#define DEFAULT_DEST_MAC1 0x34
#define DEFAULT_DEST_MAC2 0x56
#define DEFAULT_DEST_MAC3 0x78
#define DEFAULT_DEST_MAC4 0x90
#define DEFAULT_DEST_MAC5 0xFF

#define INTERFACE_NAME "wlp1s0"
#define BUF_SIZE 2048

int main(int argc, char *argv[]) {

  /* Check is help message time */
  if (
    argc == 1
    || (argc >= 2 && strncmp("help", argv[1], 4) == 0)
    || (argc >= 2 && strncmp("-h", argv[1], 2) == 0)
    || (argc >= 2 && strncmp("--help", argv[1], 6) == 0)
  ) {
    printf("Usage  : %s PACKETS_TO_SEND SEND_DELAY [MAC_ADDRESS]\n", argv[0]);
    printf("Example: %s 100 10000 12:34:56:78:90:FF\n", argv[0]);
    printf("Example: %s 100 10000\n\n", argv[0]);
    printf("PACKETS_TO_SEND\n  The number of packets to inject. Set this zero for infinite injection.\n\n");
    printf("SEND_DELAY\n  Injection interval in microseconds. Set this zero for no-delay injection.\n");
    exit(0);
  }

  char* readStrEnd;

  /* Get the number of packets to send */
  const unsigned long long cnt = strtoull(argv[1], &readStrEnd, 10);
  const bool infCnt = cnt == 0;

  /* Get packet injection delay */
  const unsigned long delay = strtoul(argv[2], &readStrEnd, 10);
  const bool delayExist = delay > 0;

  /* Get destination address */
  char dstAddr[6] = { 0 };
  if (argc > 3) {
    sscanf(argv[3], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
      &dstAddr[0], &dstAddr[1], &dstAddr[2],
      &dstAddr[3], &dstAddr[4], &dstAddr[5]);
  } else {
    dstAddr[0] = DEFAULT_DEST_MAC0;
    dstAddr[1] = DEFAULT_DEST_MAC1;
    dstAddr[2] = DEFAULT_DEST_MAC2;
    dstAddr[3] = DEFAULT_DEST_MAC3;
    dstAddr[4] = DEFAULT_DEST_MAC4;
    dstAddr[5] = DEFAULT_DEST_MAC5;
  }

  /* Open RAW socket to send on */
  int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sock == -1) {
    perror("socket");
  }

  /* Get the index & mac of the interface to send on */
  struct ifreq intf;
  memset(&intf, 0, sizeof(struct ifreq));
  strncpy(intf.ifr_name, INTERFACE_NAME, IFNAMSIZ - 1);
  if (ioctl(sock, SIOCGIFINDEX, &intf) < 0) perror("SIOCGIFINDEX");
  if (ioctl(sock, SIOCGIFHWADDR, &intf) < 0) perror("SIOCGIFHWADDR");

  /* Initialize buffer */
  char sendBuf[BUF_SIZE];
  memset(sendBuf, 0, BUF_SIZE);

  /* Construct the Ethernet header */
  struct ether_header *eh = (struct ether_header *) sendBuf;
  eh->ether_shost[0] = ((uint8_t *)&intf.ifr_hwaddr.sa_data)[0];
  eh->ether_shost[1] = ((uint8_t *)&intf.ifr_hwaddr.sa_data)[1];
  eh->ether_shost[2] = ((uint8_t *)&intf.ifr_hwaddr.sa_data)[2];
  eh->ether_shost[3] = ((uint8_t *)&intf.ifr_hwaddr.sa_data)[3];
  eh->ether_shost[4] = ((uint8_t *)&intf.ifr_hwaddr.sa_data)[4];
  eh->ether_shost[5] = ((uint8_t *)&intf.ifr_hwaddr.sa_data)[5];
  eh->ether_dhost[0] = dstAddr[0];
  eh->ether_dhost[1] = dstAddr[1];
  eh->ether_dhost[2] = dstAddr[2];
  eh->ether_dhost[3] = dstAddr[3];
  eh->ether_dhost[4] = dstAddr[4];
  eh->ether_dhost[5] = dstAddr[5];
  eh->ether_type = htons(ETH_P_IP);

  /* Set packet payload */
  const size_t ehSize = sizeof(struct ether_header);
  const size_t plSize = BUF_SIZE - ehSize;
  const unsigned int rbufSize = getrandom(&sendBuf[ehSize], plSize, 0);
  const double injectionRate = (double) 1000000 / (double) delay;
  if (rbufSize < 0) perror("getrandom");
  else {
    printf("[Injection Information]\n");
    printf("Packet Length  : %zu bytes", ehSize);
    printf("Payload Length : %du bytes\n", rbufSize);
    printf("Packet Count   : %llu pkts\n", cnt);
    printf("Injection Rate : %.3f pkts/s\n\n", injectionRate);
    fflush(stdout);
  }

  /* Set socket address */
  struct sockaddr_ll sockAddr;
  sockAddr.sll_ifindex = intf.ifr_ifindex;
  sockAddr.sll_family = PF_PACKET;
  sockAddr.sll_protocol = htons(ETH_P_IP);
  sockAddr.sll_hatype = ARPHRD_ETHER;
  sockAddr.sll_pkttype = PACKET_OTHERHOST;
  sockAddr.sll_halen = ETH_ALEN;
  sockAddr.sll_addr[0] = dstAddr[0];
  sockAddr.sll_addr[1] = dstAddr[1];
  sockAddr.sll_addr[2] = dstAddr[2];
  sockAddr.sll_addr[3] = dstAddr[3];
  sockAddr.sll_addr[4] = dstAddr[4];
  sockAddr.sll_addr[5] = dstAddr[5];

  /* Set clock variables */
  struct timespec start, now;
  int32_t diff;
  if (delayExist) clock_gettime(CLOCK_MONOTONIC, &start);

  /* Send packets */
  unsigned long long i;
  const size_t saSize = sizeof(struct sockaddr_ll);
  const struct sockaddr* saPointer = (struct sockaddr*) &sockAddr;
  for (i = 0; infCnt || i < cnt; i += 1)
  {
    if (delayExist) {
      clock_gettime(CLOCK_MONOTONIC, &now);
      diff = delay * i - (
              (now.tv_sec - start.tv_sec) * 1000000
               + (now.tv_nsec - start.tv_nsec + 500) / 1000
             );
      if (diff > 0 && diff < delay) usleep(diff);
    }
    if (sendto(sock, sendBuf, BUF_SIZE, 0, saPointer, saSize) < 0)
    {
      printf("X");
      fflush(stdout);
    } else {
      printf(".");
      if ((i + 1) % 1000 == 0) {
        printf(" %lluk\n", (i + 1) / 1000);
      }
      fflush(stdout);
    }
  }

  return 0;

}
