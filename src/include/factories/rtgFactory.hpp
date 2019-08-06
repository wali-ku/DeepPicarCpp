/**
 * RtgFactory Class Declaration File.
 *
 * This class is responsible for interfacing with RTG-Synch framework for
 * providing virtual-gang related services.
 */
#ifndef __RTG_FACTORY_HPP__
#define __RTG_FACTORY_HPP__

extern "C" {
#include <rtg_lib.h>
}

class RtgFactory
{
	pthread_barrier_t	*barrier;

	public:
		/**
		 * Default Constructor. Should not be used.
		 */
		RtgFactory ();

		/**
		 * Class Constructor. Register with RTG-Synch framework using
		 * the provided virtual gang id and memory bandwidth
		 * thresholds.
		 */
		RtgFactory (int id, int read_bw_mbps, int write_bw_mbps);

		/**
		 * Synchronization Method. Use RTG-Synch framework to
		 * synchronize with other members of the virtual gang.
		 */
		void sync ();
};

#endif /* __RTG_FACTORY_HPP__ */
