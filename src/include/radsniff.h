/*
 *  radsniff.h	Structures and defines for the RADIUS sniffer.
 *
 *  Version:    $Id$
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 *  Copyright 2006  The FreeRADIUS server project
 *  Copyright 2006  Nicolas Baradakis <nicolas.baradakis@cegetel.net>
 */

RCSIDH(radsniff_h, "$Id$")

#include <sys/types.h>
#include <netinet/in.h>

/*
 *	Logging macros
 */
#undef DEBUG1
#define DEBUG1(fmt, ...)	if (fr_debug_flag > 2) fprintf(log_dst , fmt "\n", ## __VA_ARGS__)
#undef DEBUG
#define DEBUG(fmt, ...)		if (fr_debug_flag > 1) fprintf(log_dst , fmt "\n", ## __VA_ARGS__)
#undef INFO
#define INFO(fmt, ...)		if (fr_debug_flag > 0) fprintf(log_dst , fmt "\n", ## __VA_ARGS__)

#define ERROR(fmt, ...)		fr_perror("radsniff: " fmt "\n", ## __VA_ARGS__)

typedef struct radsniff {
	bool			from_file;		//!< Were reading pcap data from files.
	bool			from_dev;		//!< Were reading pcap data from devices.
	bool			from_stdin;		//!< Were reading pcap data from stdin.
	bool			to_file;		//!< Were writing pcap data to files.
	bool			to_stdout;		//!< Were writing pcap data to stdout.

	bool			from_auto;		//!< From list was auto-generated.

	int			stats_interval;		//!< Time between stats updates in seconds.
	bool			do_sort;		//!< Whether we sort attributes in the packet.
	char const		*radius_secret;		//!< Secret to decode encrypted attributes.

	char			*pcap_filter;		//!< PCAP filter string applied to live capture devices.
	char			*radius_filter;		//!< RADIUS filter string.


} radsniff_t;

/** Stats for a single interval
 *
 * And interval is defined as the time between a call to the stats output function.
 */
typedef struct radsniff_stats_t {
	int			intervals;		//!< Number of stats intervals.

	double			latency_cma;		//!< Cumulative moving average.
	int			latency_cma_count;	//!< Number of CMA datapoints processed.

	struct {
		uint64_t		requests;		//!< Number of requests we've seen.
		uint64_t		responses;		//!< Number of responses we've seen.
		uint64_t		forwarded;		//!< Number of forwarded requests
		uint64_t		linked;			//!< Number of request/response pairs.
		long double		latency_total;		//!< Total latency between requests/responses in the
								//!< interval.
		double			latency_average;	//!< Average latency (this iteration).

		double			latency_high;		//!< Latency high water mark.
		double			latency_low;		//!< Latency low water mark.
	} interval;
} radsniff_stats_t;

typedef struct radsniff_event {
	radsniff_t		*conf;			//!< radsniff configuration.
	fr_pcap_t		*in;			//!< PCAP handle event occurred on.
	fr_pcap_t		*out;			//!< Where to write output.
	radsniff_stats_t	*stats;			//!< Where to write stats.
} radsniff_event_t;

typedef struct radsniff_update {
	fr_event_list_t		*list;			//!< List to insert new event into.
	radsniff_t		*conf;			//!< radsniff configuration.
	radsniff_stats_t	*stats;			//!< stats to process.
} radsniff_update_t;
