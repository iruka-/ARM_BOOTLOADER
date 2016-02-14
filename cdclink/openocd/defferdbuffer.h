#ifndef	defferdbuffer_h_
#define	defferdbuffer_h_

#define	NUM_DEFFERD_BUFFERS	65536
/*********************************************************************
 *	buffer ‚Ì’x‰„ŠJ•ú.
 *********************************************************************
 */
static	uchar *defferd_buffers[NUM_DEFFERD_BUFFERS];
static	int    defferd_buffer_cnt = 0;

/*********************************************************************
 *
 *********************************************************************
 */
#if	0
static  void   defferd_buf_free(int cnt)
{
	int i;
	for(i=0;i<cnt;i++) {
		if(	defferd_buffers[i] ) {
			free(defferd_buffers[i]);
			defferd_buffers[i]=NULL;
		}
	}
	defferd_buffer_cnt = 0;
}
#endif
/*********************************************************************
 *
 *********************************************************************
 */
#if	0
static  uchar *defferd_buf_malloc(int idx,int size)
{
	uchar *p=NULL;
	if(idx>=NUM_DEFFERD_BUFFERS) {
		RS_error_exit("FATAL:defferd_buf_malloc(65536)! overrun\n")
		exit(-1);
	}
	if(size>0) {
		p = malloc(size);
	}
	defferd_buffers[idx]=p;
	return p;
}
#endif
/*********************************************************************
 *
 *********************************************************************
 */
static  uchar *defferd_buf_get(int idx)
{
	if(idx>=NUM_DEFFERD_BUFFERS) {
		RS_error_exit("FATAL:defferd_buf_get(65536)! overrun\n");
		exit(-1);
	}
	return defferd_buffers[idx];
}

static  void defferd_buf_set(int idx,uchar *ptr)
{
	if(idx>=NUM_DEFFERD_BUFFERS) {
		RS_error_exit("FATAL:defferd_buf_get(65536)! overrun\n");
		exit(-1);
	}
	defferd_buffers[idx] = ptr;
}




/*********************************************************************
 *
 *********************************************************************
 */


#endif

