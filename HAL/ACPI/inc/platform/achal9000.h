/******************************************************************************
 *
 * Name: achal9000.h - OS specific defines, etc. for HAL9000
 *
 *****************************************************************************/

#ifndef __ACHAL9000_H__
#define __ACHAL9000_H__

#ifdef __GNUC__
#include "acgcc.h"
#else
#include "acintel.h"
#endif

#define ACPI_MACHINE_WIDTH 64

#ifdef _MSC_VER
#define ACPI_FLUSH_CPU_CACHE()
#else
#define ACPI_FLUSH_CPU_CACHE() __asm__ __volatile__("wbinvd" : : : "memory")
#endif

#define ACPI_ACQUIRE_GLOBAL_LOCK(GLptr, Acq)

#define ACPI_RELEASE_GLOBAL_LOCK(GLptr, Pnd)

#endif
