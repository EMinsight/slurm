/*****************************************************************************\
 *  jobcomp_kafka_message.h - Kafka message helper header for jobcomp/kafka.
 *****************************************************************************
 *  Copyright (C) SchedMD LLC.
 *
 *  This file is part of Slurm, a resource management program.
 *  For details, see <https://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  Slurm is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  Slurm is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Slurm; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifndef _HAVE_JOBCOMP_KAFKA_MESSAGE_H
#define _HAVE_JOBCOMP_KAFKA_MESSAGE_H

#include <librdkafka/rdkafka.h>

typedef struct {
	uint32_t event; /* event from jobcomp_event_t */
	uint32_t job_id;
} kafka_msg_opaque_t;

typedef struct {
	kafka_msg_opaque_t *opaque;
	char *payload;
} kafka_msg_t;

extern kafka_msg_opaque_t *jobcomp_kafka_message_init_opaque(uint32_t event,
							     uint32_t job_id);
extern int jobcomp_kafka_message_init(void);
extern void jobcomp_kafka_message_fini(void);
extern void jobcomp_kafka_message_produce(kafka_msg_opaque_t *opaque,
					  char *payload);

#endif
