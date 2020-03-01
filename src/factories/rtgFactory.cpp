#include <rtgFactory.hpp>
#include <iostream>
#include <chrono>


RtgFactory::RtgFactory (int id, bool rtgang_mode, bool gang_leader,
		int read_bw_mbps, int write_bw_mbps, unsigned long cmap):
	gid (id), rtgang (rtgang_mode), leader (gang_leader)
{
#ifdef RTG_SYNCH_DEBUG
	debug_setup_ftrace ();
#endif

	if (rtgang)
		register_gang_with_kernel (id, 0, read_bw_mbps, 0);
	else
		barrier = rtg_member_setup (id, cmap, read_bw_mbps,
				write_bw_mbps);
}

void RtgFactory::sync ()
{
	if (!rtgang) {
		rtg_member_sync (barrier);
		std::cout << std::chrono::system_clock::to_time_t (std::chrono::system_clock::now ()) << " | Synced!" << std::endl;
	}

	return;
}
