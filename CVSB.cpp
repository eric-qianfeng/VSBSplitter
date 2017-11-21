#include "CVSB.h"

const unsigned char HARDWARE_TIMESTAMP_ID_NEOv6_VCAN = 6;
const unsigned char HARDWARE_TIMESTAMP_ID_DOUBLE_SEC = 7;
const unsigned char HARDWARE_TIMESTAMP_ID_NEORED_25NS = 9;
const double NEOVI_RED_TIMESTAMP_2_25NS = 107.3741824;
const double NEOVI_RED_TIMESTAMP_1_25NS = 0.000000025;
//const double NEOVI_RED_TIMESTAMP_2_25NS = 0.1048576;
//const double NEOVI_RED_TIMESTAMP_1_25NS = 0.0000016;


CVSB::CVSB()
{

}

double CVSB::CalculateTimeStamp(const icsSpyMessage& msg)
{
    if ((msg.TimeStampHardwareID & 0x7F) == HARDWARE_TIMESTAMP_ID_DOUBLE_SEC)
    {
        //return reinterpret_cast<double>(((long)msg.TimeHardware << 32) | msg.TimeHardware2);
    }

    if ((msg.TimeStampHardwareID & 0x7F) == HARDWARE_TIMESTAMP_ID_NEOv6_VCAN)
    {
        return ((double)msg.TimeHardware2) * 0.065536 + ((double)msg.TimeHardware) * 0.000001;
    }

    if ((msg.TimeStampHardwareID & 0x7F) == HARDWARE_TIMESTAMP_ID_NEORED_25NS)
    {
        return NEOVI_RED_TIMESTAMP_2_25NS * ((double)msg.TimeHardware2) + NEOVI_RED_TIMESTAMP_1_25NS * ((double)msg.TimeHardware);
    }

    return 0.0;
}

void CVSB::SetTimeStamp(icsSpyMessage& msg, double newTimestamp)
{
    if ((msg.TimeStampHardwareID & 0x7F) == HARDWARE_TIMESTAMP_ID_DOUBLE_SEC)
    {
        //long timestamp = reinterpret_cast<long>(newTimestamp);
        //msg.TimeHardware = (unsigned)(timestamp >> 32);
        //msg.TimeHardware2 = (unsigned)timestamp;
    }
    else if ((msg.TimeStampHardwareID & 0x7F) == HARDWARE_TIMESTAMP_ID_NEOv6_VCAN)
    {
        msg.TimeHardware2 = (unsigned)(newTimestamp / 0.065536);
        msg.TimeHardware = (unsigned)(newTimestamp * 1e6 - msg.TimeHardware2 * 65535);
    }
    else if ((msg.TimeStampHardwareID & 0x7F) == HARDWARE_TIMESTAMP_ID_NEORED_25NS)
    {
        msg.TimeHardware2 = (unsigned)(newTimestamp / NEOVI_RED_TIMESTAMP_2_25NS);
        msg.TimeHardware = (unsigned)(newTimestamp / NEOVI_RED_TIMESTAMP_1_25NS - msg.TimeHardware2 * NEOVI_RED_TIMESTAMP_2_25NS / NEOVI_RED_TIMESTAMP_1_25NS);
    }
}
