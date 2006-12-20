#include <errno.h>
#include <string.h>
#include <vsprintf.h>
#include <gpxe/errortab.h>

/** @file
 *
 * Error descriptions.
 *
 * The error numbers used by Etherboot are a superset of those defined
 * by the PXE specification version 2.1.  See errno.h for a listing of
 * the error values.
 *
 * To save space in ROM images, error string tables are optional.  Use
 * the ERRORMSG_XXX options in config.h to select which error string
 * tables you want to include.  If an error string table is omitted,
 * strerror() will simply return the text "Error 0x<errno>".
 *
 */

static struct errortab errortab_start[0] __table_start(errortab);
static struct errortab errortab_end[0] __table_end(errortab);

/**
 * Retrieve string representation of error number.
 *
 * @v errno		Error number
 * @ret strerror	Pointer to error text
 *
 * If the error is not found in the linked-in error tables, generates
 * a generic "Error 0x<errno>" message.
 *
 * The pointer returned by strerror() is valid only until the next
 * call to strerror().
 *
 */
const char * strerror ( int errno ) {
	static char *generic_message = "Error 0x0000";
	struct errortab *errortab;

	/* Allow for strerror(rc) as well as strerror(errno) */
	if ( errno < 0 )
		errno = -errno;

	for ( errortab = errortab_start ; errortab < errortab_end ;
	      errortab++ ) {
		if ( errortab->errno == errno )
			return errortab->text;
	}

	sprintf ( generic_message + 8, "%hx", errno );
	return generic_message;
}

/** The most common errors */
struct errortab enoem __errortab = { ENOMEM, "Out of memory" };
struct errortab einval __errortab = { EINVAL, "Invalid argument" };
struct errortab enospc __errortab = { ENOSPC, "No space left on device" };
struct errortab eio __errortab = { EIO, "Input/output error" };