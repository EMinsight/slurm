/*****************************************************************************\
 *  src/common/env.h - environment vector manipulation
 *****************************************************************************
 *  Copyright (C) 2002-2006 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Mark Grondona <mgrondona@llnl.gov>.
 *  CODE-OCEC-09-009. All rights reserved.
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
 *  Slurm is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Slurm; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/
#ifndef _ENV_H
#define _ENV_H

#include <sys/types.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "slurm/slurm.h"
#include "src/common/macros.h"
#include "src/common/slurm_opt.h"
#include "src/common/slurm_protocol_api.h"
#include "src/common/xregex.h"

typedef struct env_options {
	int ntasks;		/* --ntasks=n,      -n n	*/
	char *task_count;
	bool ntasks_set;	/* true if ntasks explicitly set */
	bool cpus_set;		/* true if cpus_per_task explicitly set */
	task_dist_states_t distribution; /* --distribution=, -m dist	*/
	uint16_t plane_size;         /* plane_size for SLURM_DIST_PLANE */
	cpu_bind_type_t
		cpu_bind_type;	/* --cpu-bind=			*/
	char *cpu_bind;		/* binding map for map/mask_cpu	*/
	uint32_t cpu_freq_min;  /* Minimum cpu frequency  */
	uint32_t cpu_freq_max;  /* Maximum cpu frequency  */
	uint32_t cpu_freq_gov;  /* cpu frequency governor */
	mem_bind_type_t
		mem_bind_type;	/* --mem-bind=			*/
	char *mem_bind;		/* binding map for tasks to memory	*/
	bool overcommit;	/* --overcommit,   -O		*/
	int  slurmd_debug;	/* --slurmd-debug, -D           */
	bool labelio;		/* --label-output, -l		*/
	int nhosts;
	char *nodelist;		/* nodelist in string form */
	char *partition;	/* partition name */
	char **env;             /* job environment */
	uint16_t comm_port;	/* srun's communication port */
	slurm_addr_t *cli;	/* launch node address */
	char *job_name;		/* assigned job name */
	int jobid;		/* assigned job id */
	int stepid;	        /* assigned step id */
	int procid;		/* global task id (across nodes) */
	int localid;		/* local task id (within node) */
	int nodeid;
	int cpus_per_task;	/* --cpus-per-task=n, -c n	*/
	int ntasks_per_node;	/* --ntasks-per-node=n		*/
	int ntasks_per_socket;	/* --ntasks-per-socket=n	*/
	int ntasks_per_core;	/* --ntasks-per-core=n		*/
	int ntasks_per_gpu;	/* --ntasks-per-gpu		*/
	int ntasks_per_tres;	/* --ntasks-per-tres		*/
	uint16_t threads_per_core; /* --threads-per-core=n	*/
	int cpus_on_node;
	pid_t task_pid;
	char *sgtids;		/* global ranks array of integers */
	uint16_t pty_port;	/* used to communicate window size changes */
	uint16_t ws_col;	/* window size, columns */
	uint16_t ws_row;	/* window size, row count */
	uint16_t restart_cnt;	/* count of job restarts	*/
	uint16_t batch_flag;	/* 1 if batch: queued job with script */
	uint32_t uid;		/* user ID */
	char *user_name;	/* user name */
	uint32_t gid;		/* group ID */
	char *group_name;	/* group name */
	char *account;          /* job's account */
	char *qos;              /* job's qos */
	char *resv_name;        /* job's reservation */
	time_t job_end_time;    /* job's end time */
	char *job_licenses;	/* job's licenses */
	time_t job_start_time;  /* job's start time */
	uint16_t oom_kill_step;   /* --oom-kill-step */
} env_t;


/* NOTE: These functions operate on the job's current environment
 * if env is NULL, otherwise they operate on the argument array */
int	envcount (char **env);
char *	getenvp(char **env, const char *name);
int	setenvf(char ***envp, const char *name, const char *fmt, ...)
		__attribute__ ((format (printf, 3, 4)));
int	setenvfs(const char *fmt, ...);
void	unsetenvp(char **env, const char *name);

int	setup_env(env_t *env, bool preserve_env);

/**********************************************************************
 * Newer environment variable handling scheme
 **********************************************************************/
/*
 * Set in "dest" the environment variables relevant to a Slurm job
 * allocation, overwriting any environment variables of the same name.
 * If the address pointed to by "dest" is NULL, memory will automatically be
 * xmalloc'ed.  The array is terminated by a NULL pointer, and thus is
 * suitable for use by execle() and other env_array_* functions.
 *
 * dest OUT - array in which to the set environment variables
 * alloc IN - resource allocation response
 * desc IN - job allocation request
 * het_job_offset IN - component offset into hetjob, -1 if not hetjob
 *
 * Sets the variables:
 *	SLURM_JOB_ID
 *	SLURM_JOB_NUM_NODES
 *	SLURM_JOB_NODELIST
 *	SLURM_JOB_CPUS_PER_NODE
 *
 * Sets OBSOLETE variables:
 *	? probably only needed for users...
 */
extern int env_array_for_job(char ***dest,
			     const resource_allocation_response_msg_t *alloc,
			     const job_desc_msg_t *desc, int het_job_offset);

/*
 * Set in "dest" the environment variables relevant to a Slurm batch
 * job allocation, overwriting any environment variables of the same name.
 * If the address pointed to by "dest" is NULL, memory will automatically be
 * xmalloc'ed.  The array is terminated by a NULL pointer, and thus is
 * suitable for use by execle() and other env_array_* functions.
 *
 * Sets the variables:
 *	SLURM_JOB_ID
 *	SLURM_JOB_NUM_NODES
 *	SLURM_JOB_NODELIST
 *	SLURM_JOB_CPUS_PER_NODE
 *	ENVIRONMENT=BATCH
 *	HOSTNAME
 *
 * Sets OBSOLETE variables:
 *	SLURM_JOBID
 *	SLURM_NNODES
 *	SLURM_NODELIST
 *	SLURM_TASKS_PER_NODE <- poorly named, really CPUs per node
 *	? probably only needed for users...
 */
extern int env_array_for_batch_job(char ***dest,
				   const batch_job_launch_msg_t *batch,
				   const char* node_name);

/*
 * Set in "dest" the environment variables relevant to a Slurm job step,
 * overwriting any environment variables of the same name.  If the address
 * pointed to by "dest" is NULL, memory will automatically be xmalloc'ed.
 * The array is terminated by a NULL pointer, and thus is suitable for
 * use by execle() and other env_array_* functions.  If preserve_env is
 * true, the variables SLURM_NNODES and SLURM_NTASKS remain unchanged.
 *
 * Sets variables:
 *	SLURM_STEP_ID
 *	SLURM_STEP_NUM_NODES
 *	SLURM_STEP_NUM_TASKS
 *	SLURM_STEP_TASKS_PER_NODE
 *	SLURM_STEP_LAUNCHER_PORT
 *	SLURM_STEP_LAUNCHER_IPADDR
 *	SLURM_STEP_RESV_PORTS
 *
 * Sets OBSOLETE variables:
 *	SLURM_STEPID
 *      SLURM_NNODES
 *	SLURM_NTASKS
 *	SLURM_NODELIST
 *	SLURM_TASKS_PER_NODE
 *	SLURM_SRUN_COMM_HOST
 *	SLURM_SRUN_COMM_PORT
 *	SLURM_LAUNCH_NODE_IPADDR
 *
 */
extern void
env_array_for_step(char ***dest,
		   const job_step_create_response_msg_t *step,
		   launch_tasks_request_msg_t *launch,
		   uint16_t launcher_port,
		   bool preserve_env);

/*
 * Return an empty environment variable array (contains a single
 * pointer to NULL).
 */
char **env_array_create(void);

/*
 * Unset all of the environment variables in a user's current
 * environment.
 */
void env_unset_environment(void);

/*
 * Merge all of the environment variables in src_array into the
 * array dest_array.  Any variables already found in dest_array
 * will be overwritten with the value from src_array.
 */
void env_array_merge(char ***dest_array, const char **src_array);

/*
 * Merge the environment variables in src_array beginning with "SLURM" or
 * SPANK_OPTION_ENV_PREFIX into the array dest_array.  Any variables already
 * found in dest_array will be overwritten with the value from src_array.
 */
void env_array_merge_slurm_spank(char ***dest_array, const char **src_array);

/*
 * Remove environment variables in env_ptr matching regex.
 *
 * IN env_ptr - source env array to remove matching variables
 * IN regex - regex to select variables to remove
 * RET new array with regex matching items removed.
 * 	Caller must env_array_free();
 */
extern char **env_array_exclude(const char **env_ptr, const regex_t *regex);

/*
 * Copy env_array must be freed by env_array_free
 */
char **env_array_copy(const char **array);

/*
 * Free the memory used by an environment variable array.
 */
void env_array_free(char **env_array);

/*
 * Append a single environment variable to an environment variable array,
 * if and only if a variable by that name does not already exist in the
 * array.
 *
 * Return 1 on success, and 0 on error.
 */
int env_array_append(char ***array_ptr, const char *name,
		     const char *value);

/*
 * Append a single environment variable to an environment variable array,
 * if and only if a variable by that name does not already exist in the
 * array.
 *
 * "value_fmt" supports printf-style formatting.
 *
 * Return 1 on success, and 0 on error.
 */
int env_array_append_fmt(char ***array_ptr, const char *name,
			 const char *value_fmt, ...)
  __attribute__ ((format (printf, 3, 4)));

/*
 * Append a single environment variable to an environment variable array
 * if a variable by that name does not already exist.  If a variable
 * by the same name is found in the array, it is overwritten with the
 * new value.
 *
 * Return 1 on success, and 0 on error.
 */
int env_array_overwrite(char ***array_ptr, const char *name,
			const char *value);

/*
 * Append a single environment variable to an environment variable array
 * if a variable by that name does not already exist.  If a variable
 * by the same name is found in the array, it is overwritten with the
 * new value.  The "value_fmt" string may contain printf-style options.
 *
 * "value_fmt" supports printf-style formatting.
 *
 * Return 1 on success, and 0 on error.
 */
int env_array_overwrite_fmt(char ***array_ptr, const char *name,
			    const char *value_fmt, ...)
  __attribute__ ((format (printf, 3, 4)));

/*
 * Append a single environment variable to an environment variable array
 * if a variable by that name does not already exist.  If a variable
 * by the same name is found in the array, it is overwritten with the
 * new value.  The "value_fmt" string may contain printf-style options.
 *
 * "value_fmt" supports printf-style formatting.
 *
 * Return 1 on success, and 0 on error.
 */
int env_array_overwrite_het_fmt(char ***array_ptr, const char *name,
				int het_job_offset,
				const char *value_fmt, ...)
  __attribute__ ((format (printf, 4, 5)));

/*
 * Set in the running process's environment all of the environment
 * variables in a supplied environment variable array.
 */
void env_array_set_environment(char **env_array);

/*
 * load environment from specified file name.
 */
char **env_array_from_file(const char *filename);


/*
 * Write environment to specified file name.
 * IN newline - if true, write 1 env variable terminated with \n
 * 	if false, write 1 env variable terminated with \0
 */
int env_array_to_file(const char *filename, const char **env_array,
		      bool newline);

/*
 * Return an array of strings representing the specified user's default
 * environment variables:
 *    Execute (more or less): "/bin/su - <username> -c /usr/bin/env"
 *    Depending upon the user's login scripts, this may take a very
 *    long time to complete or possibly never return
 *
 * On error, returns NULL.
 *
 * NOTE: The calling process must have an effective uid of root for
 * this function to succeed.
 */
char **env_array_user_default(const char *username);

/*
 * Return a string representation of an array of uint16_t elements.
 * Each value in the array is printed in decimal notation and elements
 * are separated by a comma.  If sequential elements in the array
 * contain the same value, the value is written out just once followed
 * by "(xN)", where "N" is the number of times the value is repeated.
 *
 * Example:
 *   The array "1, 2, 1, 1, 1, 3, 2" becomes the string "1,2,1(x3),3,2"
 *
 * Returns an xmalloc'ed string.  Free with xfree().
 */
extern char *uint16_array_to_str(int array_len, const uint16_t *array);

/*
 * The cpus-per-node representation in Slurm (and perhaps tasks-per-node
 * in the future) is stored in a compressed format comprised of two
 * equal-length arrays, and an integer holding the array length. In one
 * array an element represents a count (number of cpus, number of tasks,
 * etc.), and the corresponding element in the other array contains the
 * number of times the count is repeated sequentially in the uncompressed
 * something-per-node array.
 *
 * This function returns the string representation of the compressed
 * array.  Free with xfree().
 */
char *uint32_compressed_to_str(uint32_t array_len,
			       const uint16_t *array,
			       const uint32_t *array_reps);

/*
 * Set TRES related env vars. Set here rather than env_array_for_job() since
 * we don't have array of opt values and the raw values are not stored in the
 * job_desc_msg_t structure (only the strings with possibly combined TRES)
 *
 * opt IN - options set by command parsing
 * dest IN/OUT - location to write environment variables
 * het_job_offset IN - component offset into hetjob, -1 if not hetjob
 */
extern void set_env_from_opts(slurm_opt_t *opt, char ***dest,
			      int het_job_offset);

/*
 * Parse token's skipping nested commas.
 *
 * Similar to strtok_r but handles nested commas.
 *
 * e.g. FOO='a,b,c',BAR='d,e,f'
 */
extern char *find_quote_token(char *tmp, char *sep, char **last);

/*
 * Propagate select user environment variables to the job.
 * If ALL is among the specified export variables, propagate the entire user
 * environment as well.
 */
extern void env_merge_filter(slurm_opt_t *opt, job_desc_msg_t *desc);

/*
 * Set the internal SLURM_PRIO_PROCESS environment variable to support
 * the propagation of the users nice value and the "PropagatePrioProcess"
 * config keyword.
 */
extern void set_prio_process_env(void);

#endif
