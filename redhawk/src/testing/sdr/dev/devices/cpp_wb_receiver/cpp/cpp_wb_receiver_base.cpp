#include "cpp_wb_receiver_base.h"

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    The following class functions are for the base class for the device class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/

cpp_wb_receiver_base::cpp_wb_receiver_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl) :
    Device_impl(devMgr_ior, id, lbl, sftwrPrfl),
    AggregateDevice_impl(),
    ThreadedComponent()
{
    construct();
}

cpp_wb_receiver_base::cpp_wb_receiver_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev) :
    Device_impl(devMgr_ior, id, lbl, sftwrPrfl, compDev),
    AggregateDevice_impl(),
    ThreadedComponent()
{
    construct();
}

cpp_wb_receiver_base::cpp_wb_receiver_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities) :
    Device_impl(devMgr_ior, id, lbl, sftwrPrfl, capacities),
    AggregateDevice_impl(),
    ThreadedComponent()
{
    construct();
}

cpp_wb_receiver_base::cpp_wb_receiver_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev) :
    Device_impl(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev),
    AggregateDevice_impl(),
    ThreadedComponent()
{
    construct();
}

cpp_wb_receiver_base::~cpp_wb_receiver_base()
{
}

void cpp_wb_receiver_base::construct()
{
    loadProperties();
    this->setHost(this);

}

/*******************************************************************************************
    Framework-level functions
    These functions are generally called by the framework to perform housekeeping.
*******************************************************************************************/
void cpp_wb_receiver_base::start()
{
    Device_impl::start();
    ThreadedComponent::startThread();
}

void cpp_wb_receiver_base::stop()
{
    Device_impl::stop();
    if (!ThreadedComponent::stopThread()) {
        throw CF::Resource::StopError(CF::CF_NOTSET, "Processing thread did not die");
    }
}

void cpp_wb_receiver_base::releaseObject()
{
    // This function clears the device running condition so main shuts down everything
    try {
        stop();
    } catch (CF::Resource::StopError& ex) {
        // TODO - this should probably be logged instead of ignored
    }

    Device_impl::releaseObject();
}

void cpp_wb_receiver_base::loadProperties()
{
    addProperty(device_kind,
                "FRONTEND::TUNER",
                "DCE:cdc5ee18-7ceb-4ae6-bf4c-31f983179b4d",
                "device_kind",
                "readonly",
                "",
                "eq",
                "allocation");

    addProperty(device_model,
                "DCE:0f99b2e4-9903-4631-9846-ff349d18ecfb",
                "device_model",
                "readonly",
                "",
                "eq",
                "allocation");

    addProperty(frontend_coherent_feeds,
                "FRONTEND::coherent_feeds",
                "frontend_coherent_feeds",
                "readwrite",
                "",
                "external",
                "allocation");

    addProperty(frontend_listener_allocation,
                frontend_listener_allocation_struct(),
                "FRONTEND::listener_allocation",
                "frontend_listener_allocation",
                "writeonly",
                "",
                "external",
                "allocation");

    addProperty(frontend_tuner_allocation,
                frontend_tuner_allocation_struct(),
                "FRONTEND::tuner_allocation",
                "frontend_tuner_allocation",
                "writeonly",
                "",
                "external",
                "allocation");

    addProperty(frontend_scanner_allocation,
                frontend_scanner_allocation_struct(),
                "FRONTEND::scanner_allocation",
                "frontend_scanner_allocation",
                "writeonly",
                "",
                "external",
                "allocation");

    addProperty(frontend_tuner_status,
                "FRONTEND::tuner_status",
                "frontend_tuner_status",
                "readonly",
                "",
                "external",
                "property");

}



