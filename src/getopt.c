/*
 * Copyright (C) 2009 Robrecht Dewaele
 *
 * This file is part of Elbow.
 *
 * Elbow is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Elbow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Elbow.  If not, see <http://www.gnu.org/licenses/>.
 */
#define VERSION "0.1"
#define YEAR "2009"
#define AUTHOR "Robrecht Dewaele"
#define LICENSE "GNU GPLv3"
#define DISCLAIMER \
 "Elbow is distributed in the hope that it will be useful,\n"\
 "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
 "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"\
 "GNU General Public License for more details."

#include <unistd.h>
#include <termios.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "getopt.h"

#define DEFAULT_BAUDRATE     B230400
#define DEFAULT_DEVICE      "/dev/ttyUSB0"
#define DEFAULT_END_OF_LINE "\n"
#define DEFAULT_STARTSTOP   0

#ifndef __linux__
#define B57600  57600
#define B115200 115200
#define B230400 230400
#endif /* #ifndef __linux__ */

struct options options = {
  DEFAULT_BAUDRATE,
  DEFAULT_DEVICE,
  DEFAULT_END_OF_LINE,
  NULL,
	DEFAULT_STARTSTOP
};

void setOptions(int argc, char * const argv[]) {
  int opt, rate;

  /*
   * b = baudrate
   * d = device
   * e = end of line
   * f = file
   */
  while ((opt = getopt(argc, argv, "b:d:e:f:hV")) != -1) {
    switch (opt) {
      case 'b':
        rate = atoi(optarg);
        switch (rate) {
          case 50:
            options.rate = B50;
            break;
          case 75:
            options.rate = B75;
            break;
          case 110:
            options.rate = B110;
            break;
          case 134:
            options.rate = B134;
            break;
          case 150:
            options.rate = B150;
            break;
          case 200:
            options.rate = B200;
            break;
          case 300:
            options.rate = B300;
            break;
          case 600:
            options.rate = B600;
            break;
          case 1200:
            options.rate = B1200;
            break;
          case 1800:
            options.rate = B1800;
            break;
          case 2400:
            options.rate = B2400;
            break;
          case 4800:
            options.rate = B4800;
            break;
          case 9600:
            options.rate = B9600;
            break;
          case 19200:
            options.rate = B19200;
            break;
          case 38400:
            options.rate = B38400;
            break;
          case 57600:
            options.rate = B57600;
            break;
          case 115200:
            options.rate = B115200;
            break;
          case 230400:
            options.rate = B230400;
            break;
          default:
            fprintf(stderr,
                "Valid Baudrates are: "
                "50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, "
                "9600, 19200, 38400, 57600, 115200, 230400\n");
            exit(EXIT_FAILURE);
        }
        break;
      case 'd':
        options.device = optarg;
        break;
      case 'e':
        if (0 == strcmp("cr", optarg)) {
          options.eol = "\r";
          break;
        }
        if (0 == strcmp("lf", optarg)) {
          options.eol = "\n";
          break;
        }
        if (0 == strcmp("crlf", optarg)) {
          options.eol = "\r\n";
          break;
        }
        fprintf(stderr,
            "Valid end of line characters are:\n"
            "'cr' (carriage return)\n"
            "'lf' (line feed)\n"
            "'crlf' (carriage return followed by a line feed)\n");
        exit(EXIT_FAILURE);
      case 'f':
        options.file = optarg;
        break;
			case 'V':
				printf("Elbow %s - (C) %s %s\nReleased under %s.\n\n%s\n", VERSION, YEAR, AUTHOR, LICENSE, DISCLAIMER);
				exit(EXIT_SUCCESS);
				break;
			case 'h':
      default: /* '?' */
        fprintf(stderr, "Usage: %s [options]\n"
						"option" "\t<argument>" "\tdefault     " "\tdescription:\n"

            "-b"     "\t<baudrate>" "\t230400      " "\tcommunication baudrate\n"
            "-d"     "\t<device>  " "\t/dev/ttyUSB0" "\tdevice node to connect to\n"
            "-e"     "\t<EOL>     " "\tlf          " "\tEnd Of Line character - valid: cr, lf, crlf\n"
            "-f"     "\t<file>    " "\t            " "\tchoose which file to transfer to the device\n"
						"-h"     "\t          " "\t            " "\tshow this information\n"
						,argv[0]);
        exit(EXIT_FAILURE);
    }
  }
}
