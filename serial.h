/*****************************************************************************
 *
 * Copyright Hewell Technology Ltd. 2014,2015
 *
 * This file is part of vbus-collector.
 *
 * vbus-collector is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * vbus-collector is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vbus-collector.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#ifndef SERIAL_H
#define SERIAL_H

extern bool serial_open_port(char *port);
extern bool serial_set_baud_rate(int rate);
extern bool serial_close_port(void);
extern ssize_t serial_write(const void *buf, size_t count);
extern ssize_t serial_read(void *buf, size_t count);
extern const char *serial_get_error(void);
extern void debug_comment(const char *comment, ...);

#endif //SERIAL_H


