/*
 *  Copyright 2014 Embedded Artists AB
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "ff.h"
#include "mbed.h"
#include "rtos.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

/******************************************************************************
 * External global variables
 *****************************************************************************/

/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Global Functions
 *****************************************************************************/

#if _FS_REENTRANT
/*------------------------------------------------------------------------*/
/* Create a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to create a new
/  synchronization object, such as semaphore and mutex. When a zero is
/  returned, the f_mount function fails with FR_INT_ERR.
*/

int ff_cre_syncobj (	/* TRUE:Function succeeded, FALSE:Could not create due to any error */
	BYTE vol,			/* Corresponding logical drive being processed */
	_SYNC_t *sobj		/* Pointer to return the created sync object */
)
{
	int ret;

//	*sobj = CreateMutex(NULL, FALSE, NULL);	/* Win32 */
//	ret = (*sobj != INVALID_HANDLE_VALUE);

//	*sobj = SyncObjects[vol];	/* uITRON (give a static sync object) */
//	ret = 1;					/* The initial value of the semaphore must be 1. */

//	*sobj = OSMutexCreate(0, &err);			/* uC/OS-II */
//	ret = (err == OS_NO_ERR);

//	*sobj = xSemaphoreCreateMutex();		/* FreeRTOS */
//	ret = (*sobj != NULL);

	*sobj = new Mutex();					/* MBED RTOS */
//	*sobj = new Semaphore(1);				/* MBED RTOS */
	ret = (*sobj != NULL);

	return ret;
}



/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to delete a synchronization
/  object that created with ff_cre_syncobj function. When a zero is
/  returned, the f_mount function fails with FR_INT_ERR.
*/

int ff_del_syncobj (	/* TRUE:Function succeeded, FALSE:Could not delete due to any error */
	_SYNC_t sobj		/* Sync object tied to the logical drive to be deleted */
)
{
	BOOL ret;

//	ret = CloseHandle(sobj);	/* Win32 */

//	ret = 1;					/* uITRON (nothing to do) */

//	OSMutexDel(sobj, OS_DEL_ALWAYS, &err);	/* uC/OS-II */
//	ret = (err == OS_NO_ERR);

//	ret = 1;					/* FreeRTOS (nothing to do) */

	if (sobj != NULL) {			/* MBED RTOS */
		free(sobj);
	}
	ret = 1;

	return ret;
}



/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a zero is returned, the file function fails with FR_TIMEOUT.
*/

int ff_req_grant (	/* TRUE:Got a grant to access the volume, FALSE:Could not get a grant */
	_SYNC_t sobj	/* Sync object to wait */
)
{
	int ret;

//	ret = (WaitForSingleObject(sobj, _FS_TIMEOUT) == WAIT_OBJECT_0);	/* Win32 */

//	ret = (wai_sem(sobj) == E_OK);	/* uITRON */

//	OSMutexPend(sobj, _FS_TIMEOUT, &err));			/* uC/OS-II */
//	ret = (err == OS_NO_ERR);

//	ret = (xSemaphoreTake(sobj, _FS_TIMEOUT) == pdTRUE);	/* FreeRTOS */

	ret = ((*((Mutex*)sobj)).lock(_FS_TIMEOUT) == osOK);   /* MBED RTOS */
//    static volatile osThreadId lastid = Thread::gettid();
//	ret = ((*((Semaphore*)sobj)).wait(_FS_TIMEOUT) == 1);   /* MBED RTOS */

	return ret;
}



/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant (
	_SYNC_t sobj	/* Sync object to be signaled */
)
{
//	ReleaseMutex(sobj);		/* Win32 */

//	sig_sem(sobj);			/* uITRON */

//	OSMutexPost(sobj);		/* uC/OS-II */

//	xSemaphoreGive(sobj);	/* FreeRTOS */

	(*((Mutex*)sobj)).unlock();			/* MBED RTOS */
//	(*((Semaphore*)sobj)).release();	/* MBED RTOS */
}

#endif




#if _USE_LFN == 3	/* LFN with a working buffer on the heap */
/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/
/* If a NULL is returned, the file function fails with FR_NOT_ENOUGH_CORE.
*/

void* ff_memalloc (	/* Returns pointer to the allocated memory block */
	UINT size		/* Number of bytes to allocate */
)
{
	return malloc(size);
}


/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/

void ff_memfree(
	void* mblock	/* Pointer to the memory block to free */
)
{
	free(mblock);
}

#endif


