/**
* @file debugkeyvalue.cpp
* @author Caglar Yilmaz <yilmaz.caglar@tubitak.gov.tr>
*/

#include "debugkeyvalue.hpp"
#include <cstring> 			// For memset
#include <parameters/param.h>

const char *const UavcanDebugKeyValueBridge::NAME = "debug_key_value";

UavcanDebugKeyValueBridge::UavcanDebugKeyValueBridge(uavcan::INode &node) :
    UavcanSensorBridgeBase("uavcan_debug_key_value", ORB_ID(debug_key_value)),
    _sub_key_value_data(node)
{
}

int UavcanDebugKeyValueBridge::init() {
    int res = _sub_key_value_data.start(KeyValueCbBinder(this, &UavcanDebugKeyValueBridge::key_value_sub_cb));

    if (res < 0) {
        DEVICE_LOG("failed to start uavcan sub: %d", res);
        return res;
    }

    return 0;
}

// This is the callback function which is going to be called whenever we receive a Debug Key Value frame
void UavcanDebugKeyValueBridge::key_value_sub_cb(const uavcan::ReceivedDataStructure<uavcan::protocol::debug::KeyValue> &msg) {
	auto report = ::debug_key_value_s();

	report.timestamp = hrt_absolute_time();

	memset(report.key, 0, sizeof(report.key)); // Null terminate

	// Since we will only deal with 3 ASCII characters we exclude the rest of key length.
	// Will be fixed in the future.
	for (int i=0; i < 3; ++i) { // i < sizeof(msg.key)
		report.key[i] = static_cast<char>(msg.key[i]);
	}
	report.value = msg.value;

	publish(msg.getSrcNodeID().get(), &report);
}

int UavcanDebugKeyValueBridge::init_driver(uavcan_bridge::Channel *channel) {
	return PX4_OK;
}
