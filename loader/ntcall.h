/*
 * nt loader
 *
 * Copyright 2006-2008 Mike McCormack
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __NTCALL_H__
#define __NTCALL_H__

// FIXME: the following should go in a different file

#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS do_nt_syscall(ULONG id, ULONG func, ULONG *uargs, ULONG retaddr);
NTSTATUS copy_to_user( void *dest, const void *src, size_t len );
NTSTATUS copy_from_user( void *dest, const void *src, size_t len );
NTSTATUS verify_for_write( void *dest, size_t len );

#ifdef __cplusplus
};
#endif

#ifdef __cplusplus

class address_space;
class thread_t;
class process_t;

#include "list.h"

typedef list_anchor<process_t,0> process_list_t;
typedef list_iter<process_t,0> process_iter_t;
typedef list_element<process_t> process_element_t;

#include "thread.h"
#include "process.h"

extern thread_t *current;
extern process_list_t processes;
extern ULONG unique_counter;
extern object_t *ntdll_section;

NTSTATUS copy_oa_from_user( OBJECT_ATTRIBUTES *koa, UNICODE_STRING *kus, const OBJECT_ATTRIBUTES *uoa );
void free_oa( OBJECT_ATTRIBUTES *oa );
void free_us( UNICODE_STRING *us );

NTSTATUS process_from_handle( HANDLE handle, process_t **process );
NTSTATUS thread_from_handle( HANDLE handle, thread_t **thread );
thread_t *find_thread_by_client_id( CLIENT_ID *id );

NTSTATUS process_alloc_user_handle( process_t *process, object_t *obj, ACCESS_MASK access, HANDLE *out, HANDLE *copy );

static inline NTSTATUS alloc_user_handle( object_t *obj, ACCESS_MASK access, HANDLE *out )
{
	return process_alloc_user_handle( current->process, obj, access, out, 0 );
}

static inline NTSTATUS alloc_user_handle( object_t *obj, ACCESS_MASK access, HANDLE *out, HANDLE *copy )
{
	return process_alloc_user_handle( current->process, obj, access, out, copy );
}

// from reg.cpp
void init_registry( void );
void free_registry( void );

// from ntl.cpp
extern int option_trace;

// from section.cpp
const char *get_section_symbol( object_t *section, ULONG address );

class win32k_info_t
{
	int x;
};

// from ntgdi.cpp
NTSTATUS win32k_process_init(process_t *p);
NTSTATUS win32k_thread_init(thread_t *t);

// from kthread.cpp
void create_kthread(void);
void shutdown_kthread(void);

#endif // __cplusplus

#endif // __NTCALL_H__
