#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <src/common/slurm_protocol_pack.h>
#include <src/common/pack.h>
#include <src/common/log.h>
#include <src/common/nodelist.h>
#include <src/slurmctld/slurmctld.h>

/* pack_header
 * packs a slurm protocol header that proceeds every slurm message
 * header 	- the header structure to pack
 * buffer	- destination of the pack, note buffer will be incremented by underlying pack routines
 * length	- length of buffer, note length will be decremented by underlying pack routines
 */
void pack_header ( header_t * header, char ** buffer , uint32_t * length )
{
	pack16 ( header -> version , ( void ** ) buffer , length ) ;
	pack16 ( header -> flags , ( void ** ) buffer , length ) ;
	pack16 ( header -> msg_type , ( void ** ) buffer , length ) ;
	pack32 ( header -> body_length , ( void ** ) buffer , length ) ;
}

/* unpack_header
 * unpacks a slurm protocol header that proceeds every slurm message
 * header 	- the header structure to unpack
 * buffer	- destination of the pack, note buffer will be incremented by underlying unpack routines
 * length	- length of buffer, note length will be decremented by underlying unpack routines
 */
void unpack_header ( header_t * header , char ** buffer , uint32_t * length )
{
	unpack16 ( & header -> version , ( void ** ) buffer , length ) ;
	unpack16 ( & header -> flags , ( void ** ) buffer , length ) ;
	unpack16 ( & header -> msg_type , ( void ** ) buffer , length ) ;
	unpack32 ( & header -> body_length , ( void ** ) buffer , length ) ;
}

/* pack_msg
 * packs a slurm protocol mesg body
 * header 	- the body structure to pack
 * buffer	- destination of the pack, note buffer will be incremented by underlying pack routines
 * length	- length of buffer, note length will be decremented by underlying pack routines
 */
int pack_msg ( slurm_msg_t const * msg , char ** buffer , uint32_t * buf_len )
{
	switch ( msg -> msg_type )
	{
		case REQUEST_BUILD_INFO :
		case REQUEST_JOB_INFO :
		case REQUEST_JOB_STEP_INFO :
		case REQUEST_NODE_INFO :
		case REQUEST_PARTITION_INFO :
		case REQUEST_ACCTING_INFO :
			pack_last_update ( ( last_update_msg_t * ) msg -> data , ( void ** ) buffer , buf_len ) ;
			break;
		case RESPONSE_BUILD_INFO:
			pack_build_info ( ( build_info_msg_t * ) msg -> data , (void ** ) buffer , buf_len ) ;
			break ;
		case RESPONSE_JOB_INFO:
			pack_job_info_msg ( ( slurm_msg_t * ) msg , (void ** ) buffer , buf_len ) ;
			break ;
		case RESPONSE_PARTITION_INFO:
			pack_partition_info_msg ( ( slurm_msg_t * ) msg , (void ** ) buffer , buf_len ) ;
			break ;
		case RESPONSE_NODE_INFO:
			pack_node_info_msg ( ( slurm_msg_t * ) msg , (void ** ) buffer , buf_len ) ;
			break ;
		case REQUEST_NODE_REGISRATION_STATUS :
			break ;
		case MESSAGE_NODE_REGISRATION_STATUS :
			break ;
		case REQUEST_RESOURCE_ALLOCATION :
		case REQUEST_SUBMIT_BATCH_JOB :
			pack_job_desc ( (job_desc_msg_t * )  msg -> data , ( void ** ) buffer , buf_len )  ;
			break ;
		case RESPONSE_RESOURCE_ALLOCATION :
			break ;
		case RESPONSE_SUBMIT_BATCH_JOB :
			break ;
		case REQUEST_CANCEL_JOB :
			break ;
		case REQUEST_CANCEL_JOB_STEP :
			break ;
		case REQUEST_SIGNAL_JOB :
			break ;
		case REQUEST_SIGNAL_JOB_STEP :
			break ;
		case REQUEST_RECONFIGURE :
			/* Message contains no body/information */
			break ;
		case RESPONSE_CANCEL_JOB :
		case RESPONSE_RECONFIGURE :
		case RESPONSE_CANCEL_JOB_STEP :
		case RESPONSE_SIGNAL_JOB :
		case RESPONSE_SIGNAL_JOB_STEP :
			break ;
		case REQUEST_JOB_ATTACH :
			break ;
		case RESPONSE_JOB_ATTACH :
			break ;
		case REQUEST_LAUNCH_TASKS :
			break ;
		case REQUEST_GET_JOB_STEP_INFO :
			break ;
		case RESPONSE_GET_JOB_STEP_INFO :
			break ;
		case REQUEST_JOB_RESOURCE :
			break ;
		case RESPONSE_JOB_RESOURCE :
			break ;
		case REQUEST_RUN_JOB_STEP :
			break ;
		case RESPONSE_RUN_JOB_STEP:
			break ;
		case REQUEST_GET_KEY :
			break ;
		case RESPONSE_GET_KEY :
			break ;
		case MESSAGE_TASK_EXIT :
			break ;
		case REQUEST_BATCH_JOB_LAUNCH :
			break ;
		case MESSAGE_UPLOAD_ACCOUNTING_INFO :
			break ;
		case RESPONSE_SLURM_RC:
			pack_return_code ( ( return_code_msg_t * ) msg -> data , ( void ** ) buffer , buf_len ) ;
			break;
		default :
			debug ( "No pack method for msg type %i",  msg -> msg_type ) ;
			return EINVAL ;
			break;
		
	}
	return 0 ;
}

/* unpack_msg
 * unpacks a slurm protocol msg body
 * header 	- the body structure to unpack
 * buffer	- destination of the pack, note buffer will be incremented by underlying unpack routines
 * length	- length of buffer, note length will be decremented by underlying unpack routines
 */
int unpack_msg ( slurm_msg_t * msg , char ** buffer , uint32_t * buf_len )
{
	switch ( msg-> msg_type )
	{
		case REQUEST_BUILD_INFO :
		case REQUEST_JOB_INFO :
		case REQUEST_JOB_STEP_INFO :
		case REQUEST_NODE_INFO :
		case REQUEST_PARTITION_INFO :
		case REQUEST_ACCTING_INFO :
			unpack_last_update ( ( last_update_msg_t **) &(msg -> data)  , ( void ** ) buffer , buf_len ) ;
			break;
		case RESPONSE_BUILD_INFO:
			unpack_build_info ( ( build_info_msg_t ** ) &(msg -> data) , (void ** ) buffer , buf_len ) ;
			break;
		case RESPONSE_JOB_INFO:
			unpack_job_info_msg ( ( job_info_msg_t ** ) &(msg -> data) , (void ** ) buffer , buf_len ) ;
			break;
		case RESPONSE_PARTITION_INFO:
			unpack_partition_info_msg ( ( partition_info_msg_t ** ) &(msg -> data) , (void ** ) buffer , buf_len ) ;
			break;
		case RESPONSE_NODE_INFO:
			unpack_node_info_msg ( ( node_info_msg_t ** ) &(msg -> data) , (void ** ) buffer , buf_len ) ;
			break;
		case REQUEST_NODE_REGISRATION_STATUS :
			break ;
		case MESSAGE_NODE_REGISRATION_STATUS :
			break ;
		case REQUEST_RESOURCE_ALLOCATION :
		case REQUEST_SUBMIT_BATCH_JOB :
			unpack_job_desc ( ( job_desc_msg_t **) & ( msg-> data ), ( void ** ) buffer , buf_len ) ;
			break ;
		case RESPONSE_RESOURCE_ALLOCATION :
			break ;
		case RESPONSE_SUBMIT_BATCH_JOB :
			break ;
		case REQUEST_CANCEL_JOB :
			break ;
		case REQUEST_CANCEL_JOB_STEP :
			break ;
		case REQUEST_SIGNAL_JOB :
			break ;
		case REQUEST_SIGNAL_JOB_STEP :
			break ;
		case REQUEST_RECONFIGURE :
			/* Message contains no body/information */
			break ;
		case RESPONSE_CANCEL_JOB :
		case RESPONSE_RECONFIGURE :
		case RESPONSE_CANCEL_JOB_STEP :
		case RESPONSE_SIGNAL_JOB :
		case RESPONSE_SIGNAL_JOB_STEP :
			break ;
		case REQUEST_JOB_ATTACH :
			break ;
		case RESPONSE_JOB_ATTACH :
			break ;
		case REQUEST_LAUNCH_TASKS :
			break ;
		case REQUEST_GET_JOB_STEP_INFO :
			break ;
		case RESPONSE_GET_JOB_STEP_INFO :
			break ;
		case REQUEST_JOB_RESOURCE :
			break ;
		case RESPONSE_JOB_RESOURCE :
			break ;
		case REQUEST_RUN_JOB_STEP :
			break ;
		case RESPONSE_RUN_JOB_STEP:
			break ;
		case REQUEST_GET_KEY :
			break ;
		case RESPONSE_GET_KEY :
			break ;
		case MESSAGE_TASK_EXIT :
			break ;
		case REQUEST_BATCH_JOB_LAUNCH :
			break ;
		case MESSAGE_UPLOAD_ACCOUNTING_INFO :
			break ;
		case RESPONSE_SLURM_RC:
			unpack_return_code ( ( return_code_msg_t **) &(msg -> data)  , ( void ** ) buffer , buf_len ) ;
			break;
		default :
			debug ( "No pack method for msg type %i",  msg -> msg_type ) ;
			return EINVAL ;
			break;
		
	}
	return 0 ;
}

void pack_node_registration_status_msg ( node_registration_status_msg_t * msg, char ** buffer , uint32_t * length )
{
	pack32 ( msg -> timestamp , ( void ** ) buffer , length ) ;
	packstr ( msg -> node_name , ( void ** ) buffer , length ) ;
	pack32 ( msg -> cpus , ( void ** ) buffer , length ) ;
	pack32 ( msg -> real_memory_size , ( void ** ) buffer , length ) ;
	pack32 ( msg -> temporary_disk_space , ( void ** ) buffer , length ) ;
}

int unpack_node_registration_status_msg ( node_registration_status_msg_t ** msg , char ** buffer , uint32_t * length )
{
	uint16_t uint16_tmp;
	node_registration_status_msg_t * node_reg_ptr ;
	/* alloc memory for structure */	
	node_reg_ptr = xmalloc ( sizeof ( node_registration_status_msg_t ) ) ;
	if (node_reg_ptr == NULL) 
	{
		return ENOMEM;
	}

	/* load the data values */
	/* unpack timestamp of snapshot */
	unpack32 ( & node_reg_ptr -> timestamp , ( void ** ) buffer , length ) ;
	unpackstr_xmalloc ( & node_reg_ptr -> node_name , &uint16_tmp,  ( void ** ) buffer , length ) ;
	unpack32 ( & node_reg_ptr -> cpus , ( void ** ) buffer , length ) ;
	unpack32 ( & node_reg_ptr -> real_memory_size , ( void ** ) buffer , length ) ;
	unpack32 ( & node_reg_ptr -> temporary_disk_space , ( void ** ) buffer , length ) ;
	*msg = node_reg_ptr ;
	return 0 ;
}

void pack_node_info_msg ( slurm_msg_t * msg, void ** buf_ptr , int * buffer_size )
{	
	assert ( msg != NULL );
	assert (  sizeof(*msg) == sizeof(slurm_msg_t) ) ;
	assert ( buf_ptr != NULL && (*buf_ptr) != NULL ) ;
	assert ( ( buffer_size ) != NULL ) ;
	assert ( *buffer_size >= msg->data_size );

	 memcpy ( *buf_ptr , msg->data , msg->data_size );
	(*buf_ptr) += msg->data_size;
	(*buffer_size) -= msg->data_size;
}

int unpack_node_info_msg ( node_info_msg_t ** msg , void ** buf_ptr , int * buffer_size )
{
	int i;
	node_table_t *node;
	
	*msg = xmalloc ( sizeof ( node_info_msg_t ) );
	if ( *msg == NULL )
		return ENOMEM ;

	/* load buffer's header (data structure version and time) */
	unpack32 (&((*msg) -> record_count), buf_ptr, buffer_size);
	unpack32 (&((*msg) -> last_update ) , buf_ptr, buffer_size);

	node = (*msg) -> node_array = xmalloc ( sizeof ( node_table_t ) * (*msg)->record_count ) ;

	/* load individual job info */
	for (i = 0; i < (*msg)->record_count ; i++) {
	unpack_node_table ( & node[i] , buf_ptr , buffer_size ) ;

	}
	return 0;
}


int unpack_node_table_msg ( node_table_msg_t ** node , void ** buf_ptr , int * buffer_size )
{
		*node = xmalloc ( sizeof(node_table_t) );
		if (node == NULL) {
			return ENOMEM;
		}
		unpack_node_table ( *node , buf_ptr , buffer_size ) ;
		return 0 ;
}

int unpack_node_table ( node_table_msg_t * node , void ** buf_ptr , int * buffer_size )
{
	uint16_t uint16_tmp;

        unpackstr_xmalloc (&node->name, &uint16_tmp, buf_ptr, buffer_size);
        unpack16  (&node->node_state, buf_ptr, buffer_size);
        unpack32  (&node->cpus, buf_ptr, buffer_size);
        unpack32  (&node->real_memory, buf_ptr, buffer_size);
        unpack32  (&node->tmp_disk, buf_ptr, buffer_size);
        unpack32  (&node->weight, buf_ptr, buffer_size);
        unpackstr_xmalloc (&node->features, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_xmalloc (&node->partition, &uint16_tmp, buf_ptr, buffer_size);

        return 0;
}

void pack_partition_info_msg ( slurm_msg_t * msg, void ** buf_ptr , int * buffer_size )
{	
	assert ( msg != NULL );
	assert (  sizeof(*msg) == sizeof(slurm_msg_t) ) ;
	assert ( buf_ptr != NULL && (*buf_ptr) != NULL ) ;
	assert ( (buffer_size) != NULL ) ;
	assert ( *buffer_size >= msg->data_size );

	memcpy ( *buf_ptr , msg->data , msg->data_size );
	(*buf_ptr) += msg->data_size;
	(*buffer_size) -= msg->data_size;
}

int unpack_partition_info_msg ( partition_info_msg_t ** msg , void ** buf_ptr , int * buffer_size )
{
        int i;
        partition_table_t *partition;

        *msg = xmalloc ( sizeof ( partition_info_msg_t ) );
        if ( *msg == NULL )
                return ENOMEM ;

        /* load buffer's header (data structure version and time) */
        unpack32 (&((*msg) -> record_count), buf_ptr, buffer_size);
        unpack32 (&((*msg) -> last_update ) , buf_ptr, buffer_size);

        partition = (*msg) -> partition_array = xmalloc ( sizeof ( partition_table_t ) * (*msg)->record_count ) ;

        /* load individual job info */
        for (i = 0; i < (*msg)->record_count ; i++) {
		unpack_partition_table ( & partition[i] , buf_ptr , buffer_size ) ;

        }
        return 0;
}




int unpack_partition_table_msg ( partition_table_msg_t ** part , void ** buf_ptr , int * buffer_size )
{
		*part = xmalloc ( sizeof(partition_table_t) );
		if (part == NULL) {
			return ENOMEM;
		}
		unpack_partition_table ( *part , buf_ptr , buffer_size ) ;
		return 0 ;
}

int unpack_partition_table ( partition_table_msg_t * part , void ** buf_ptr , int * buffer_size )
{
	uint16_t uint16_tmp;
	char * node_inx_str;

	unpackstr_xmalloc (&part->name, &uint16_tmp, buf_ptr, buffer_size);
	if (part->name == NULL)
		part->name = "";
	unpack32  (&part->max_time, buf_ptr, buffer_size);
	unpack32  (&part->max_nodes, buf_ptr, buffer_size);
	unpack32  (&part->total_nodes, buf_ptr, buffer_size);

	unpack32  (&part->total_cpus, buf_ptr, buffer_size);
	unpack16  (&part->default_part, buf_ptr, buffer_size);
	unpack16  (&part->key, buf_ptr, buffer_size);
	unpack16  (&part->shared, buf_ptr, buffer_size);

	unpack16  (&part->state_up, buf_ptr, buffer_size);
	unpackstr_xmalloc (&part->allow_groups, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_xmalloc (&part->nodes, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_xmalloc (&node_inx_str, &uint16_tmp, buf_ptr, buffer_size);
	if (node_inx_str == NULL)
		part->node_inx = bitfmt2int("");
	else {
		part->node_inx = bitfmt2int(node_inx_str);
		xfree ( node_inx_str );
	}
	return 0;
}

void pack_job_info_msg ( slurm_msg_t * msg, void ** buf_ptr , int * buffer_size )
{	
	assert ( msg != NULL );
	assert (  sizeof(*msg) == sizeof(slurm_msg_t) ) ;
	assert ( buf_ptr != NULL && (*buf_ptr) != NULL ) ;
	assert ( (buffer_size) != NULL ) ;
	assert ( *buffer_size >= msg->data_size );

	memcpy ( *buf_ptr , msg->data , msg->data_size );
	(*buf_ptr) += msg->data_size;
	(*buffer_size) -= msg->data_size;
}

int unpack_job_info_msg ( job_info_msg_t ** msg , void ** buf_ptr , int * buffer_size )
{
	int i;
	job_table_t *job;
	
	*msg = xmalloc ( sizeof ( job_info_msg_t ) );
	if ( *msg == NULL )
		return ENOMEM ;


	/* load buffer's header (data structure version and time) */
	unpack32 (&((*msg) -> record_count), buf_ptr, buffer_size);
	unpack32 (&((*msg) -> last_update ) , buf_ptr, buffer_size);

	job = (*msg) -> job_array = xmalloc ( sizeof ( job_table_t ) * (*msg)->record_count ) ;

	/* load individual job info */
	for (i = 0; i < (*msg)->record_count ; i++) {
		unpack_job_table ( & job[i] , buf_ptr , buffer_size ) ;
	}
	return 0;
}

int unpack_job_table_msg ( job_table_msg_t ** msg , void ** buf_ptr , int * buffer_size )
{
	*msg = xmalloc ( sizeof ( job_table_t ) ) ;
	if ( *msg == NULL )
		return ENOMEM ;
	unpack_job_table ( *msg , buf_ptr , buffer_size ) ;
	return 0 ;
}

int unpack_job_table ( job_table_t * job , void ** buf_ptr , int * buffer_size )
{
	uint16_t uint16_tmp;
	uint32_t uint32_tmp;
	char * node_inx_str;

	unpack32  (&job->job_id, buf_ptr, buffer_size);
	unpack32  (&job->user_id, buf_ptr, buffer_size);
	unpack16  (&job->job_state, buf_ptr, buffer_size);
	unpack32  (&job->time_limit, buf_ptr, buffer_size);

	unpack32  (&uint32_tmp, buf_ptr, buffer_size);
	job->start_time = (time_t) uint32_tmp;
	unpack32  (&uint32_tmp, buf_ptr, buffer_size);
	job->end_time = (time_t) uint32_tmp;
	unpack32  (&job->priority, buf_ptr, buffer_size);

	unpackstr_xmalloc (&job->nodes, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_xmalloc (&job->partition, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_xmalloc (&job->name, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_ptr (&node_inx_str, &uint16_tmp, buf_ptr, buffer_size);
	if (node_inx_str == NULL)
		job->node_inx = bitfmt2int("");
	else {
		job->node_inx = bitfmt2int(node_inx_str);
		xfree ( node_inx_str );
	}

	unpack32  (&job->num_procs, buf_ptr, buffer_size);
	unpack32  (&job->num_nodes, buf_ptr, buffer_size);
	unpack16  (&job->shared, buf_ptr, buffer_size);
	unpack16  (&job->contiguous, buf_ptr, buffer_size);

	unpack32  (&job->min_procs, buf_ptr, buffer_size);
	unpack32  (&job->min_memory, buf_ptr, buffer_size);
	unpack32  (&job->min_tmp_disk, buf_ptr, buffer_size);

	unpackstr_xmalloc (&job->req_nodes, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_ptr (&node_inx_str, &uint16_tmp, buf_ptr, buffer_size);
	if (node_inx_str == NULL)
		job->req_node_inx = bitfmt2int("");
	else {
		job->node_inx = bitfmt2int(node_inx_str);
		xfree ( node_inx_str );
	}
	unpackstr_xmalloc (&job->features, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_xmalloc (&job->job_script, &uint16_tmp, buf_ptr, buffer_size);
	return 0 ;
}

void pack_build_info ( build_info_msg_t * build_ptr, void ** buf_ptr , int * buffer_size )
{	
	pack32 (build_ptr->last_update, buf_ptr, buffer_size);
	pack16 (build_ptr->backup_interval, buf_ptr, buffer_size);
	packstr (build_ptr->backup_location, buf_ptr, buffer_size);
	packstr (build_ptr->backup_machine, buf_ptr, buffer_size);
	packstr (build_ptr->control_daemon, buf_ptr, buffer_size);
	packstr (build_ptr->control_machine, buf_ptr, buffer_size);
	pack16 (build_ptr->controller_timeout, buf_ptr, buffer_size);
	packstr (build_ptr->epilog, buf_ptr, buffer_size);
	pack16 (build_ptr->fast_schedule, buf_ptr, buffer_size);
	pack16 (build_ptr->hash_base, buf_ptr, buffer_size);
	pack16 (build_ptr->heartbeat_interval, buf_ptr, buffer_size);
	packstr (build_ptr->init_program, buf_ptr, buffer_size);
	pack16 (build_ptr->kill_wait, buf_ptr, buffer_size);
	packstr (build_ptr->prioritize, buf_ptr, buffer_size);
	packstr (build_ptr->prolog, buf_ptr, buffer_size);
	packstr (build_ptr->server_daemon, buf_ptr, buffer_size);
	pack16 (build_ptr->server_timeout, buf_ptr, buffer_size);
	packstr (build_ptr->slurm_conf, buf_ptr, buffer_size);
	packstr (build_ptr->tmp_fs, buf_ptr, buffer_size);
}

int unpack_build_info ( build_info_msg_t **build_buffer_ptr, void ** buffer , int * buffer_size )
{	
	uint16_t uint16_tmp;
	build_info_msg_t * build_ptr ;
	/* alloc memory for structure */	
	build_ptr = xmalloc ( sizeof ( struct build_table ) ) ;
	if (build_ptr == NULL) 
	{
		return ENOMEM;
	}

	/* load the data values */
	/* unpack timestamp of snapshot */
	unpack32 (&build_ptr->last_update, buffer, buffer_size);
	unpack16 (&build_ptr->backup_interval, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->backup_location, &uint16_tmp, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->backup_machine, &uint16_tmp, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->control_daemon, &uint16_tmp, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->control_machine, &uint16_tmp, buffer, buffer_size);
	unpack16 (&build_ptr->controller_timeout, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->epilog, &uint16_tmp, buffer, buffer_size);
	unpack16 (&build_ptr->fast_schedule, buffer, buffer_size);
	unpack16 (&build_ptr->hash_base, buffer, buffer_size);
	unpack16 (&build_ptr->heartbeat_interval, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->init_program, &uint16_tmp, buffer, buffer_size);
	unpack16 (&build_ptr->kill_wait, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->prioritize, &uint16_tmp, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->prolog, &uint16_tmp, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->server_daemon, &uint16_tmp, buffer, buffer_size);
	unpack16 (&build_ptr->server_timeout, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->slurm_conf, &uint16_tmp, buffer, buffer_size);
	unpackstr_xmalloc (&build_ptr->tmp_fs, &uint16_tmp, buffer, buffer_size);
	*build_buffer_ptr = build_ptr ;
	return 0 ;
}

/* pack_job_desc
 * packs a job_desc struct 
 * header 	- the body structure to pack
 * buf_ptr	- destination of the pack, note buffer will be incremented by underlying pack routines
 * buffer_size	- length of buffer, note length will be decremented by underlying pack routines
 */
void pack_job_desc ( job_desc_msg_t * job_desc_ptr, void ** buf_ptr , int * buffer_size )
{	
	/* load the data values */
	/* unpack timestamp of snapshot */

	pack16 (job_desc_ptr->contiguous, buf_ptr, buffer_size);
	packstr (job_desc_ptr->features, buf_ptr, buffer_size);
	packstr (job_desc_ptr->groups, buf_ptr, buffer_size);
	pack32 (job_desc_ptr->job_id, buf_ptr, buffer_size);
	packstr (job_desc_ptr->name, buf_ptr, buffer_size);
	packmem (job_desc_ptr->partition_key, 32, buf_ptr, buffer_size);
	
	pack32 (job_desc_ptr->min_procs, buf_ptr, buffer_size);
	pack32 (job_desc_ptr->min_memory, buf_ptr, buffer_size);
	pack32 (job_desc_ptr->min_tmp_disk, buf_ptr, buffer_size);
	
	packstr (job_desc_ptr->partition, buf_ptr, buffer_size);
	pack32 (job_desc_ptr->priority, buf_ptr, buffer_size);
	
	packstr (job_desc_ptr->partition, buf_ptr, buffer_size);
	packstr (job_desc_ptr->partition, buf_ptr, buffer_size);
	pack16 (job_desc_ptr->shared, buf_ptr, buffer_size);

	pack32 (job_desc_ptr->time_limit, buf_ptr, buffer_size);
	
	pack32 (job_desc_ptr->num_procs, buf_ptr, buffer_size);
	pack32 (job_desc_ptr->num_nodes, buf_ptr, buffer_size);
	pack32 (job_desc_ptr->user_id, buf_ptr, buffer_size);

}

/* unpack_msg
 * unpacks a job_desc struct
 * header 	- the body structure to unpack
 * buf_ptr	- destination of the pack, note buffer will be incremented by underlying unpack routines
 * buffer_size	- length of buffer, note length will be decremented by underlying unpack routines
 */
int unpack_job_desc ( job_desc_msg_t **job_desc_buffer_ptr, void ** buf_ptr , int * buffer_size )
{	
	uint16_t uint16_tmp;
	job_desc_msg_t * job_desc_ptr ;

	/* alloc memory for structure */
	job_desc_ptr = xmalloc ( sizeof ( job_desc_msg_t ) ) ;
	if (job_desc_ptr== NULL) 
	{
		*job_desc_buffer_ptr = NULL ;
		return ENOMEM ;
	}

	/* load the data values */
	/* unpack timestamp of snapshot */

	unpack16 (&job_desc_ptr->contiguous, buf_ptr, buffer_size);
	unpackstr_xmalloc (&job_desc_ptr->features, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_xmalloc (&job_desc_ptr->groups, &uint16_tmp, buf_ptr, buffer_size);
	unpack32 (&job_desc_ptr->job_id, buf_ptr, buffer_size);
	unpackstr_xmalloc (&job_desc_ptr->name, &uint16_tmp, buf_ptr, buffer_size);
	unpackmem_xmalloc ( ( char ** ) &job_desc_ptr->partition_key, &uint16_tmp, buf_ptr, buffer_size);
	
	unpack32 (&job_desc_ptr->min_procs, buf_ptr, buffer_size);
	unpack32 (&job_desc_ptr->min_memory, buf_ptr, buffer_size);
	unpack32 (&job_desc_ptr->min_tmp_disk, buf_ptr, buffer_size);
	
	unpackstr_xmalloc (&job_desc_ptr->partition, &uint16_tmp, buf_ptr, buffer_size);
	unpack32 (&job_desc_ptr->priority, buf_ptr, buffer_size);
	
	unpackstr_xmalloc (&job_desc_ptr->partition, &uint16_tmp, buf_ptr, buffer_size);
	unpackstr_xmalloc (&job_desc_ptr->partition, &uint16_tmp, buf_ptr, buffer_size);
	unpack16 (&job_desc_ptr->shared, buf_ptr, buffer_size);
	
	unpack32 (&job_desc_ptr->time_limit, buf_ptr, buffer_size);
	
	unpack32 (&job_desc_ptr->num_procs, buf_ptr, buffer_size);
	unpack32 (&job_desc_ptr->num_nodes, buf_ptr, buffer_size);
	unpack32 (&job_desc_ptr->user_id, buf_ptr, buffer_size);

	*job_desc_buffer_ptr = job_desc_ptr ;
	return 0 ;
}

void pack_last_update ( last_update_msg_t * msg , void ** buffer , uint32_t * length )
{
	pack32 ( msg -> last_update , buffer , length ) ;
}

int unpack_last_update ( last_update_msg_t ** msg , void ** buffer , uint32_t * length )
{
        last_update_msg_t * last_update_msg ;

        last_update_msg = xmalloc ( sizeof ( last_update_msg_t ) ) ;
        if ( last_update_msg == NULL)
        {
                *msg = NULL ;
                return ENOMEM ;
        }

	unpack32 ( & last_update_msg -> last_update , buffer , length ) ;
	*msg = last_update_msg ;
	return 0 ;
}

void pack_return_code ( return_code_msg_t * msg , void ** buffer , uint32_t * length )
{
	pack32 ( msg -> return_code , buffer , length ) ;
}

int unpack_return_code ( return_code_msg_t ** msg , void ** buffer , uint32_t * length )
{
        return_code_msg_t * return_code_msg ;

        return_code_msg = xmalloc ( sizeof ( return_code_msg_t ) ) ;
        if ( return_code_msg == NULL)
        {
                *msg = NULL ;
                return ENOMEM ;
        }

	unpack32 ( & return_code_msg -> return_code , buffer , length ) ;
	*msg = return_code_msg ;
	return 0 ;
}

/* template 
void pack_ ( * msg , void ** buffer , uint32_t * length )
{
	pack16 ( msg -> , buffer , length ) ;
	pack32 ( msg -> , buffer , length ) ;
}

void unpack_ ( ** msg , void char ** buffer , uint32_t * length )
{
	uint16_t uint16_tmp;
	job_desc_msg_t * job_desc_ptr ;

	job_desc_ptr = xmalloc ( sizeof ( job_desc_msg_t ) ) ;
	if (job_desc_ptr== NULL) 
	{
		*msg = NULL ;
		return ;
	}

	unpack16 ( & msg -> , buffer , length ) ;
	unpack32 ( & msg -> , buffer , length ) ;
}
*/
