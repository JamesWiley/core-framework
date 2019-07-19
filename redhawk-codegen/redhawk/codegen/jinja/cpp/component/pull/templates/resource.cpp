/*#
 * This file is protected by Copyright. Please refer to the COPYRIGHT file 
 * distributed with this source distribution.
 * 
 * This file is part of REDHAWK core.
 * 
 * REDHAWK core is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation, either version 3 of the License, or (at your 
 * option) any later version.
 * 
 * REDHAWK core is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 #*/
/*{% block license %}*/
/*# Allow child templates to include license #*/
/*{% endblock %}*/
//% set className = component.userclass.name
//% set baseClass = component.baseclass.name
//% set artifactType = component.artifacttype
/**************************************************************************

    This is the ${artifactType} code. This file contains the child class where
    custom functionality can be added to the ${artifactType}. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

**************************************************************************/

#include "${component.userclass.header}"

PREPARE_LOGGING(${className})

/*{% if component is device %}*/
${className}::${className}(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl) :
    ${baseClass}(devMgr_ior, id, lbl, sftwrPrfl)
{
/*{% block ctorBody %}*/
/*{% endblock %}*/
}

${className}::${className}(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev) :
    ${baseClass}(devMgr_ior, id, lbl, sftwrPrfl, compDev)
{
${ self.ctorBody() -}
}

${className}::${className}(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities) :
    ${baseClass}(devMgr_ior, id, lbl, sftwrPrfl, capacities)
{
${ self.ctorBody() -}
}

${className}::${className}(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev) :
    ${baseClass}(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev)
{
${ self.ctorBody() -}
}
/*{% else %}*/
${className}::${className}(const char *uuid, const char *label) :
    ${baseClass}(uuid, label)
{
    // Avoid placing constructor code here. Instead, use the "constructor" function.
${ self.ctorBody() }
}
/*{% endif %}*/

${className}::~${className}()
{
/*{% block dtorBody %}*/
/*{% endblock %}*/
}

void ${className}::constructor()
{
/*{% block constructorBody %}*/
    /***********************************************************************************
     This is the RH constructor. All properties are properly initialized before this function is called 
    ***********************************************************************************/
/*{% endblock %}*/
}

/*{% if component is device %}*/
/*{%   block updateUsageState %}*/
/**************************************************************************

    This is called automatically after allocateCapacity or deallocateCapacity are called.
    Your implementation should determine the current state of the device:

       setUsageState(CF::Device::IDLE);   // not in use
       setUsageState(CF::Device::ACTIVE); // in use, with capacity remaining for allocation
       setUsageState(CF::Device::BUSY);   // in use, with no capacity remaining for allocation

**************************************************************************/
void ${className}::updateUsageState()
{
}

/*{%   endblock %}*/
/*{% endif %}*/
/***********************************************************************************************

    Basic functionality:

        The service function is called by the serviceThread object (of type ProcessThread).
        This call happens immediately after the previous call if the return value for
        the previous call was NORMAL.
        If the return value for the previous call was NOOP, then the serviceThread waits
        an amount of time defined in the serviceThread's constructor.
        
    SRI:
        To create a StreamSRI object, use the following code:
                std::string stream_id = "testStream";
                BULKIO::StreamSRI sri = bulkio::sri::create(stream_id);
/*{% if component is device %}*/
/*{% if 'FrontendTuner' in component.implements %}*/

        To create a StreamSRI object based on tuner status structure index 'idx' and collector center frequency of 100:
                std::string stream_id = "my_stream_id";
                BULKIO::StreamSRI sri = this->create(stream_id, this->frontend_tuner_status[idx], 100);
/*{% endif %}*/
/*{% endif %}*/

    Time:
        To create a PrecisionUTCTime object, use the following code:
                BULKIO::PrecisionUTCTime tstamp = bulkio::time::utils::now();

        
    Ports:

        Data is passed to the serviceFunction through by reading from input streams
        (BulkIO only). The input stream class is a port-specific class, so each port
        implementing the BulkIO interface will have its own type-specific input stream.
        UDP multicast (dataSDDS and dataVITA49) ports do not support streams.

        The input stream from which to read can be requested with the getCurrentStream()
        method. The optional argument to getCurrentStream() is a floating point number that
        specifies the time to wait in seconds. A zero value is non-blocking. A negative value
        is blocking.  Constants have been defined for these values, bulkio::Const::BLOCKING and
        bulkio::Const::NON_BLOCKING.

        More advanced uses of input streams are possible; refer to the REDHAWK documentation
        for more details.

        Input streams return data blocks that automatically manage the memory for the data
        and include the SRI that was in effect at the time the data was received. It is not
        necessary to delete the block; it will be cleaned up when it goes out of scope.

        To send data using a BulkIO interface, create an output stream and write the
        data to it. When done with the output stream, the close() method sends and end-of-
        stream flag and cleans up.

        NOTE: If you have a BULKIO dataSDDS or dataVITA49  port, you must manually call 
              "port->updateStats()" to update the port statistics when appropriate.

        Example:
            // This example assumes that the ${artifactType} has two ports:
            //  An input (provides) port of type bulkio::InShortPort called dataShort_in
            //  An output (uses) port of type bulkio::OutFloatPort called dataFloat_out
            // The mapping between the port and the class is found
            // in the ${artifactType} base class header file

            bulkio::InShortStream inputStream = dataShort_in->getCurrentStream();
            if (!inputStream) { // No streams are available
                return NOOP;
            }

            // Get the output stream, creating it if it doesn't exist yet
            bulkio::OutFloatStream outputStream = dataFloat_out->getStream(inputStream.streamID());
            if (!outputStream) {
                outputStream = dataFloat_out->createStream(inputStream.sri());
            }

            bulkio::ShortDataBlock block = inputStream.read();
            if (!block) { // No data available
                // Propagate end-of-stream
                if (inputStream.eos()) {
                   outputStream.close();
                }
                return NOOP;
            }

            if (block.sriChanged()) {
                // Update output SRI
                outputStream.sri(block.sri());
            }

            // Get read-only access to the input data
            redhawk::shared_buffer<short> inputData = block.buffer();

            // Acquire a new buffer to hold the output data
            redhawk::buffer<float> outputData(inputData.size());

            // Transform input data into output data
            for (size_t index = 0; index < inputData.size(); ++index) {
                outputData[index] = (float) inputData[index];
            }

            // Write to the output stream; outputData must not be modified after
            // this method call
            outputStream.write(outputData, block.getStartTime());

            return NORMAL;

        If working with complex data (i.e., the "mode" on the SRI is set to
        true), the data block's complex() method will return true. Data blocks
        provide a cxbuffer() method that returns a complex interpretation of the
        buffer without making a copy:

            if (block.complex()) {
                redhawk::shared_buffer<std::complex<short> > inData = block.cxbuffer();
                redhawk::buffer<std::complex<float> > outData(inData.size());
                for (size_t index = 0; index < inData.size(); ++index) {
                    outData[index] = inData[index];
                }
                outputStream.write(outData, block.getStartTime());
            }

        Interactions with non-BULKIO ports are left up to the ${artifactType} developer's discretion
        
    Messages:
    
        To receive a message, you need (1) an input port of type MessageEvent, (2) a message prototype described
        as a structure property of kind message, (3) a callback to service the message, and (4) to register the callback
        with the input port.
        
        Assuming a property of type message is declared called "my_msg", an input port called "msg_input" is declared of
        type MessageEvent, create the following code:
        
        void ${className}::my_message_callback(const std::string& id, const my_msg_struct &msg){
        }
        
        Register the message callback onto the input port with the following form:
        this->msg_input->registerMessage("my_msg", this, &${className}::my_message_callback);
        
        To send a message, you need to (1) create a message structure, (2) a message prototype described
        as a structure property of kind message, and (3) send the message over the port.
        
        Assuming a property of type message is declared called "my_msg", an output port called "msg_output" is declared of
        type MessageEvent, create the following code:
        
        ::my_msg_struct msg_out;
        this->msg_output->sendMessage(msg_out);

/*{% if component is device %}*/
    Accessing the Device Manager and Domain Manager:
    
        Both the Device Manager hosting this Device and the Domain Manager hosting
        the Device Manager are available to the Device.
        
        To access the Domain Manager:
            CF::DomainManager_ptr dommgr = this->getDomainManager()->getRef();
        To access the Device Manager:
            CF::DeviceManager_ptr devmgr = this->getDeviceManager()->getRef();
/*{% else %}*/
    Accessing the Application and Domain Manager:
    
        Both the Application hosting this Component and the Domain Manager hosting
        the Application are available to the Component.
        
        To access the Domain Manager:
            CF::DomainManager_ptr dommgr = this->getDomainManager()->getRef();
        To access the Application:
            CF::Application_ptr app = this->getApplication()->getRef();
/*{% endif %}*/
    
    Properties:
        
        Properties are accessed directly as member variables. For example, if the
        property name is "baudRate", it may be accessed within member functions as
        "baudRate". Unnamed properties are given the property id as its name.
        Property types are mapped to the nearest C++ type, (e.g. "string" becomes
        "std::string"). All generated properties are declared in the base class
        (${baseClass}).
    
        Simple sequence properties are mapped to "std::vector" of the simple type.
        Struct properties, if used, are mapped to C++ structs defined in the
        generated file "struct_props.h". Field names are taken from the name in
        the properties file; if no name is given, a generated name of the form
        "field_n" is used, where "n" is the ordinal number of the field.
        
        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            //  - A boolean called scaleInput
              
            if (scaleInput) {
                dataOut[i] = dataIn[i] * scaleValue;
            } else {
                dataOut[i] = dataIn[i];
            }
            
        Callback methods can be associated with a property so that the methods are
        called each time the property value changes.  This is done by calling 
        addPropertyListener(<property>, this, &${className}::<callback method>)
        in the constructor.

        The callback method receives two arguments, the old and new values, and
        should return nothing (void). The arguments can be passed by value,
        receiving a copy (preferred for primitive types), or by const reference
        (preferred for strings, structs and vectors).

        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            //  - A struct property called status
            
        //Add to ${component.userclass.file}
        ${className}::${className}(const char *uuid, const char *label) :
            ${baseClass}(uuid, label)
        {
            addPropertyListener(scaleValue, this, &${className}::scaleChanged);
            addPropertyListener(status, this, &${className}::statusChanged);
        }

        void ${className}::scaleChanged(float oldValue, float newValue)
        {
            RH_DEBUG(this->_baseLog, "scaleValue changed from" << oldValue << " to " << newValue);
        }
            
        void ${className}::statusChanged(const status_struct& oldValue, const status_struct& newValue)
        {
            RH_DEBUG(this->_baseLog, "status changed");
        }
            
        //Add to ${component.userclass.header}
        void scaleChanged(float oldValue, float newValue);
        void statusChanged(const status_struct& oldValue, const status_struct& newValue);

    Logging:

        The member _baseLog is a logger whose base name is the component (or device) instance name.
        New logs should be created based on this logger name.

        To create a new logger,
            rh_logger::LoggerPtr my_logger = this->_baseLog->getChildLogger("foo");

        Assuming component instance name abc_1, my_logger will then be created with the 
        name "abc_1.user.foo".

/*{% if component is device %}*/
    Allocation:
    
        Allocation callbacks are available to customize the Device's response to 
        allocation requests. For example, if the Device contains the allocation 
        property "my_alloc" of type string, the allocation and deallocation
        callbacks follow the pattern (with arbitrary function names
        my_alloc_fn and my_dealloc_fn):
        
        bool ${className}::my_alloc_fn(const std::string &value)
        {
            // perform logic
            return true; // successful allocation
        }
        void ${className}::my_dealloc_fn(const std::string &value)
        {
            // perform logic
        }
        
        The allocation and deallocation functions are then registered with the Device
        base class with the setAllocationImpl call. Note that the variable for the property is used rather
        than its id:
        
        this->setAllocationImpl(my_alloc, this, &${className}::my_alloc_fn, &${className}::my_dealloc_fn);
        
        
/*{% endif %}*/

************************************************************************************************/
int ${className}::serviceFunction()
{
    RH_DEBUG(this->_baseLog, "serviceFunction() example log message");
    
    return NOOP;
}
/*{% block extensions %}*/
/*{% block portFunctionDelegates %}*/
/*{% if 'FrontendTuner' in component.implements %}*/
/*************************************************************
Functions servicing the tuner control port
*************************************************************/
std::string ${className}::getTunerType(const std::string& allocation_id) {
    return std::string("none");
}

bool ${className}::getTunerDeviceControl(const std::string& allocation_id) {
    return true;
}

std::string ${className}::getTunerGroupId(const std::string& allocation_id) {
    return std::string("none");
}

std::string ${className}::getTunerRfFlowId(const std::string& allocation_id) {
    return std::string("none");
}
/*{% endif %}*/
/*{% if 'AnalogTuner' in component.implements %}*/

void ${className}::setTunerCenterFrequency(const std::string& allocation_id, double freq) {
}

double ${className}::getTunerCenterFrequency(const std::string& allocation_id) {
    return 0;
}

void ${className}::setTunerBandwidth(const std::string& allocation_id, double bw) {
}

double ${className}::getTunerBandwidth(const std::string& allocation_id) {
    return 0;
}

void ${className}::setTunerAgcEnable(const std::string& allocation_id, bool enable)
{
}

bool ${className}::getTunerAgcEnable(const std::string& allocation_id)
{
    return true;
}

void ${className}::setTunerGain(const std::string& allocation_id, float gain)
{
}

float ${className}::getTunerGain(const std::string& allocation_id)
{
    return 0;
}

void ${className}::setTunerReferenceSource(const std::string& allocation_id, long source)
{
}

long ${className}::getTunerReferenceSource(const std::string& allocation_id)
{
    return 0;
}

void ${className}::setTunerEnable(const std::string& allocation_id, bool enable) {
}

bool ${className}::getTunerEnable(const std::string& allocation_id) {
    return true;
}
/*{% endif %}*/
/*{% if 'DigitalTuner' in component.implements %}*/

void ${className}::setTunerOutputSampleRate(const std::string& allocation_id, double sr) {
}

double ${className}::getTunerOutputSampleRate(const std::string& allocation_id){
    return 0;
}
/*{% endif %}*/
/*{% if 'ScanningTuner' in component.implements %}*/
frontend::ScanStatus ${className}::getScanStatus(const std::string& allocation_id) {
    frontend::ManualStrategy* tmp = new frontend::ManualStrategy(0);
    frontend::ScanStatus retval(tmp);
    return retval;
}

void ${className}::setScanStartTime(const std::string& allocation_id, const BULKIO::PrecisionUTCTime& start_time) {
}

void ${className}::setScanStrategy(const std::string& allocation_id, const frontend::ScanStrategy* scan_strategy) {
}
/*{% endif %}*/
/*{% if 'GPS' in component.implements %}*/

frontend::GPSInfo ${className}::get_gps_info(const std::string& port_name)
{
    frontend::GPSInfo gps_info;
    return gps_info;
}

void ${className}::set_gps_info(const std::string& port_name, const frontend::GPSInfo &gps_info)
{
}

frontend::GpsTimePos ${className}::get_gps_time_pos(const std::string& port_name)
{
    frontend::GpsTimePos gps_time_pos;
    return gps_time_pos;
}

void ${className}::set_gps_time_pos(const std::string& port_name, const frontend::GpsTimePos &gps_time_pos)
{
}
/*{% endif %}*/
/*{% if 'NavData' in component.implements %}*/

frontend::NavigationPacket ${className}::get_nav_packet(const std::string& port_name)
{
    frontend::NavigationPacket nav_info;
    return nav_info;
}

void ${className}::set_nav_packet(const std::string& port_name, const frontend::NavigationPacket &nav_info)
{
}
/*{% endif %}*/
/*{% if 'RFInfo' in component.implements %}*/

/*************************************************************
Functions servicing the RFInfo port(s)
- port_name is the port over which the call was received
*************************************************************/
std::string ${className}::get_rf_flow_id(const std::string& port_name)
{
    return std::string("none");
}

void ${className}::set_rf_flow_id(const std::string& port_name, const std::string& id)
{
}

frontend::RFInfoPkt ${className}::get_rfinfo_pkt(const std::string& port_name)
{
    frontend::RFInfoPkt pkt;
    return pkt;
}

void ${className}::set_rfinfo_pkt(const std::string& port_name, const frontend::RFInfoPkt &pkt)
{
}
/*{% endif %}*/
/*{% if 'RFSource' in component.implements %}*/

std::vector<frontend::RFInfoPkt> ${className}::get_available_rf_inputs(const std::string& port_name)
{
    std::vector<frontend::RFInfoPkt> inputs;
    return inputs;
}

void ${className}::set_available_rf_inputs(const std::string& port_name, const std::vector<frontend::RFInfoPkt> &inputs)
{
}

frontend::RFInfoPkt ${className}::get_current_rf_input(const std::string& port_name)
{
    frontend::RFInfoPkt pkt;
    return pkt;
}

void ${className}::set_current_rf_input(const std::string& port_name, const frontend::RFInfoPkt &pkt)
{
}
/*{% endif %}*/
/*{% endblock %}*/
/*{% endblock %}*/
