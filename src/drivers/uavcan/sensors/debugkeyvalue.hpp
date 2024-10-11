/**
* @file debugkeyvalue.hpp
* @author Caglar Yilmaz <yilmaz.caglar@tubitak.gov.tr>
* @brief DroneCAN reception for Debug Key Value messages.
*/

#pragma once

#include "sensor_bridge.hpp"
#include <uavcan/protocol/debug/KeyValue.hpp>  // Include UAVCAN DSDL
#include <uORB/topics/debug_key_value.h>          // Include uORB topic

// Class definition for the bridge
class UavcanDebugKeyValueBridge : public UavcanSensorBridgeBase {
public:
    static const char *const NAME;

    UavcanDebugKeyValueBridge(uavcan::INode &node);

    const char *get_name() const override { return NAME; }

    int init() override;

private:
    void key_value_sub_cb(const uavcan::ReceivedDataStructure<uavcan::protocol::debug::KeyValue> &msg);

    int init_driver(uavcan_bridge::Channel *channel) override;

    typedef uavcan::MethodBinder<UavcanDebugKeyValueBridge *,
                                 void (UavcanDebugKeyValueBridge::*) (const uavcan::ReceivedDataStructure<uavcan::protocol::debug::KeyValue>&)>
        KeyValueCbBinder;

    uavcan::Subscriber<uavcan::protocol::debug::KeyValue, KeyValueCbBinder> _sub_key_value_data;
};
