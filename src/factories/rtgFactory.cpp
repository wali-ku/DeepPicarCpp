#include <rtgFactory.hpp>

RtgFactory::RtgFactory (int id, int read_bw_mbps, int write_bw_mbps)
{
	barrier = rtg_member_setup (id, read_bw_mbps, write_bw_mbps);
}

void RtgFactory::sync ()
{
	rtg_member_sync (barrier);
	return;
}
