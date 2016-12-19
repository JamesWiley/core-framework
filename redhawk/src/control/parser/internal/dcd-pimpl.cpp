/*
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
 */
// Not copyrighted - public domain.
//
// This sample parser implementation was generated by CodeSynthesis XSD,
// an XML Schema to C++ data binding compiler. You may use it in your
// programs without any restrictions.
//

#include "dcd-pimpl.h"
#include "ossie/debug.h"
#include "ossie/ossieSupport.h"

CREATE_LOGGER(dcd_parser);

namespace dcd
{
  // deviceconfiguration_pimpl
  //

  void deviceconfiguration_pimpl::
  pre ()
  {
    _dcd.reset(new ossie::DeviceManagerConfiguration::DCD());
  }

  void deviceconfiguration_pimpl::
  description (const ::std::string& description)
  {
    // TODO
    //
  }

  void deviceconfiguration_pimpl::
  devicemanagersoftpkg (const ::std::string& devicemanagersoftpkg)
  {
      assert(_dcd.get() != 0);
      _dcd->deviceManagerSoftPkg = devicemanagersoftpkg;
  }

  void deviceconfiguration_pimpl::
  componentfiles (const ::std::vector<ossie::ComponentFile>& componentfiles)
  {
      assert(_dcd.get() != 0);
      _dcd->componentFiles = componentfiles;
  }

  void deviceconfiguration_pimpl::
  partitioning (const ::std::vector<ossie::DevicePlacement>& partitioning)
  {
      LOG_TRACE(dcd_parser, "set partitioning")
      assert(_dcd.get() != 0);
      _dcd->componentPlacements = partitioning;
  }

  void deviceconfiguration_pimpl::
  domainmanager (const ::std::string& domainmanager)
  {
      LOG_TRACE(dcd_parser, "set domainmanager")
      assert(_dcd.get() != 0);
      _dcd->domainManagerName = domainmanager;
  }

  void deviceconfiguration_pimpl::
  connections (const ::std::vector<ossie::Connection>& connections)
  {
      LOG_TRACE(dcd_parser, "set connections")
      assert(_dcd.get() != 0);
      _dcd->connections = connections;
  }

  void deviceconfiguration_pimpl::
  filesystemnames ()
  {
  }

  void deviceconfiguration_pimpl::
  id (const ::std::string& id)
  {
      assert(_dcd.get() != 0);
      _dcd->id = id;
  }

  void deviceconfiguration_pimpl::
  name (const ::std::string& name)
  {
      assert(_dcd.get() != 0);
      _dcd->name = name;
      if (name.find("/", 0) != std::string::npos) {
        throw ::xsd::cxx::parser::validating::invalid_value<char>("name", name);
     }
  }

  ::std::auto_ptr<ossie::DeviceManagerConfiguration::DCD> deviceconfiguration_pimpl::
  post_deviceconfiguration ()
  {
      LOG_TRACE(dcd_parser, "post deviceconfiguration")
      return _dcd;
  }

  // devicemanagersoftpkg_pimpl
  //

  void devicemanagersoftpkg_pimpl::
  pre ()
  {
      _localfile = "";
  }

  void devicemanagersoftpkg_pimpl::
  localfile (const ::std::string& localfile)
  {
      this->_localfile = localfile;
  }

  ::std::string devicemanagersoftpkg_pimpl::
  post_devicemanagersoftpkg ()
  {
      return _localfile;
  }

  // componentfiles_pimpl
  //

  void componentfiles_pimpl::
  pre ()
  {
      componentFiles.clear();
  }

  void componentfiles_pimpl::
  componentfile (const ::ossie::ComponentFile& componentfile)
  {
      componentFiles.push_back(componentfile);
  }

 const ::std::vector<ossie::ComponentFile> &componentfiles_pimpl::
  post_componentfiles ()
  {
    return componentFiles;
  }

  // componentfile_pimpl
  //

  void componentfile_pimpl::
  pre ()
  {
  }

  void componentfile_pimpl::
  localfile (const ::std::string& localfile)
  {
    componentFile.filename = localfile;
  }

  void componentfile_pimpl::
  id (const ::std::string& id)
  {
    componentFile.id = id;
  }

  void componentfile_pimpl::
  type (const ::std::string& type)
  {
    componentFile.type = type;
  }

const ::ossie::ComponentFile &componentfile_pimpl::
  post_componentfile ()
  {
    return componentFile;
  }

  // localfile_pimpl
  //

  void localfile_pimpl::
  pre ()
  {
    filename = "";
  }

  void localfile_pimpl::
  name (const ::std::string& name)
  {
    filename = name;
  }

  ::std::string localfile_pimpl::
  post_localfile ()
  {
    return filename;
  }

  // partitioning_pimpl
  //

  void partitioning_pimpl::
  pre ()
  {
    LOG_TRACE(dcd_parser, "pre partitioning");
    componentPlacements.clear();
  }

  void partitioning_pimpl::
  componentplacement (const ::ossie::DevicePlacement& componentplacement)
  {
    LOG_TRACE(dcd_parser, "adding component placement " << componentplacement.getFileRefId());
    componentPlacements.push_back(componentplacement);
  }

  ::std::vector<ossie::DevicePlacement> partitioning_pimpl::
  post_partitioning ()
  {
    LOG_TRACE(dcd_parser, "post partitioning");
    return componentPlacements;
  }

  // componentplacement_pimpl
  //

  void componentplacement_pimpl::
  pre ()
  {
    componentPlacement =  ossie::DevicePlacement();
  }

  void componentplacement_pimpl::
  componentfileref (const ::std::string& componentfileref)
  {
    componentPlacement._componentFileRef = componentfileref;
  }

  void componentplacement_pimpl::
  deployondevice (const ::std::string& deployondevice)
  {
    componentPlacement.deployOnDeviceID = deployondevice;
  }

  void componentplacement_pimpl::
  compositepartofdevice (const ::std::string& compositepartofdevice)
  {
    LOG_TRACE(dcd_parser, "composite part of device " << compositepartofdevice);
    componentPlacement.compositePartOfDeviceID = compositepartofdevice;
  }

  void componentplacement_pimpl::
  devicepkgfile ()
  {
  }

  void componentplacement_pimpl::
  componentinstantiation (const ::ossie::ComponentInstantiation& componentinstantiation)
  {
    LOG_TRACE(dcd_parser, "adding component instantiation " << componentinstantiation.getID() << " " << componentinstantiation.getUsageName());
    componentPlacement.instantiations.push_back(componentinstantiation);
  }

  const ::ossie::DevicePlacement& componentplacement_pimpl::
  post_componentplacement ()
  {
    LOG_TRACE(dcd_parser, "post componentplacement");
    return componentPlacement;
  }

  // componentfileref_pimpl
  //

  void componentfileref_pimpl::
  pre ()
  {
    _refid = "";
  }

  void componentfileref_pimpl::
  refid (const ::std::string& refid)
  {
    this->_refid = refid;
  }

  ::std::string componentfileref_pimpl::
  post_componentfileref ()
  {
    return _refid;
  }

  // deployondevice_pimpl
  //

  void deployondevice_pimpl::
  pre ()
  {
    deployondevice = "";
  }

  void deployondevice_pimpl::
  refid (const ::std::string& refid)
  {
    deployondevice = refid;
  }

  ::std::string deployondevice_pimpl::
  post_deployondevice ()
  {
    return deployondevice;
  }

  // compositepartofdevice_pimpl
  //

  void compositepartofdevice_pimpl::
  pre ()
  {
    compositepartofdevice = "";
  }

  void compositepartofdevice_pimpl::
  refid (const ::std::string& refid)
  {
    LOG_TRACE(dcd_parser, "compositepartofdevice refid" << compositepartofdevice);
    compositepartofdevice = refid;
  }

  ::std::string compositepartofdevice_pimpl::
  post_compositepartofdevice ()
  {
    LOG_TRACE(dcd_parser, "post compositepartofdevice" << compositepartofdevice);
    return compositepartofdevice;
  }

  // devicepkgfile_pimpl
  //

  void devicepkgfile_pimpl::
  pre ()
  {
  }

  void devicepkgfile_pimpl::
  localfile (const ::std::string& localfile)
  {
    // TODO
    //
  }

  void devicepkgfile_pimpl::
  type (const ::std::string& type)
  {
    // TODO
    //
  }

  void devicepkgfile_pimpl::
  post_devicepkgfile ()
  {
  }

  // componentinstantiation_pimpl
  //

  void componentinstantiation_pimpl::
  pre ()
  {
    componentInstantiation.properties.clear();
    componentInstantiation = ossie::ComponentInstantiation();
  }

  void componentinstantiation_pimpl::
  usagename (const ::std::string& usagename)
  {
    LOG_TRACE(dcd_parser, "setting usage name" << usagename);
    componentInstantiation.usageName = usagename;
  }

  void componentinstantiation_pimpl::
  componentproperties (const ossie::ComponentPropertyList& componentproperties)
  {
    LOG_TRACE(dcd_parser, "component properties");
    componentInstantiation.properties = componentproperties;
  }

  void componentinstantiation_pimpl::
  id (const ::std::string& id)
  {
    componentInstantiation.instantiationId = id;
  }

  void componentinstantiation_pimpl::
  affinity (const ossie::ComponentInstantiation::AffinityProperties& affinityProperties)
  {
    LOG_TRACE(dcd_parser, "affinity properties")
    componentInstantiation.affinityProperties = affinityProperties;
  }

  void componentinstantiation_pimpl::loggingconfig ( const ossie::ComponentInstantiation::LoggingConfig& log_cfg )
  {
    LOG_TRACE(dcd_parser, "component instantiation - logging config: " << log_cfg.first << "/" << log_cfg.second );
    componentInstantiation.loggingConfig = log_cfg;
  }

  void componentinstantiation_pimpl::deployerrequires (const ossie::ComponentPropertyList& deployerrequires)
  {
    LOG_TRACE(dcd_parser, "deployer requires");
    componentInstantiation.deployerrequires = deployerrequires;
  }



const ::ossie::ComponentInstantiation& componentinstantiation_pimpl::
  post_componentinstantiation ()
  {
    LOG_TRACE(dcd_parser, "post component instantiation " << componentInstantiation.getID() << " " << componentInstantiation.getUsageName());
    return componentInstantiation;
  }

  //
  // affinity_pimpl
  //

  void affinity_pimpl::
  pre ()
  {
    affinityProperties.clear();
  }

  void affinity_pimpl::
  simpleref (const ossie::SimplePropertyRef& simpleref)
  {
    LOG_TRACE(dcd_parser, "Adding simpleref ");
    affinityProperties.push_back(simpleref.clone());
  }

  void affinity_pimpl::
  simplesequenceref (const ossie::SimpleSequencePropertyRef& simplesequenceref)
  {
    LOG_TRACE(dcd_parser, "Adding simplesequenceref");
    affinityProperties.push_back(simplesequenceref.clone());
  }

  void affinity_pimpl::
  structref (const ossie::StructPropertyRef& structref)
  {
    LOG_TRACE(dcd_parser, "Adding structref");
    affinityProperties.push_back(structref.clone());
  }

  void affinity_pimpl::
  structsequenceref (const ossie::StructSequencePropertyRef& structsequenceref)
  {
    LOG_TRACE(dcd_parser, "Adding structsequenceref");
    affinityProperties.push_back(structsequenceref.clone());
  }

  const ossie::ComponentInstantiation::AffinityProperties&
  affinity_pimpl::post_affinity ()
  {
    return affinityProperties;
  }


  //
  // loggingconfig_pimpl
  //
  void loggingconfig_pimpl::pre ()
  {
    info = ossie::ComponentInstantiation::LoggingConfig("","");
  }

  void loggingconfig_pimpl::level ( const ::std::string &v )
  {
    info.second=v;
  }

  const ossie::ComponentInstantiation::LoggingConfig& loggingconfig_pimpl::post_loggingconfig ( )
  {
    info.first = this->post_string();
    return info;
  }


  // deployerrequires_pimpl
  //

  void deployerrequires_pimpl::
  pre ()
  {
    deployerrequires.clear();
  }

  void deployerrequires_pimpl::
  requires (const ossie::IdValue& idvalue)
  {
    LOG_TRACE(dcd_parser, "Adding idvalue " << idvalue._id << " value " << idvalue._value );
    deployerrequires.push_back(idvalue.clone());
  }

  const ossie::ComponentPropertyList& deployerrequires_pimpl::
  post_deployerrequires ()
  {
    return deployerrequires;
  }



  // componentproperties_pimpl
  //

  void componentproperties_pimpl::
  pre ()
  {
    componentProperties.clear();
  }

  void componentproperties_pimpl::
  simpleref (const ossie::SimplePropertyRef& simpleref)
  {
    LOG_TRACE(dcd_parser, "Adding simpleref ");
    componentProperties.push_back(simpleref.clone());
  }

  void componentproperties_pimpl::
  simplesequenceref (const ossie::SimpleSequencePropertyRef& simplesequenceref)
  {
    LOG_TRACE(dcd_parser, "Adding simplesequenceref");
    componentProperties.push_back(simplesequenceref.clone());
  }

  void componentproperties_pimpl::
  structref (const ossie::StructPropertyRef& structref)
  {
    LOG_TRACE(dcd_parser, "Adding structref");
    componentProperties.push_back(structref.clone());
  }

  void componentproperties_pimpl::
  structsequenceref (const ossie::StructSequencePropertyRef& structsequenceref)
  {
    LOG_TRACE(dcd_parser, "Adding structsequenceref");
    componentProperties.push_back(structsequenceref.clone());
  }

  const ossie::ComponentPropertyList& componentproperties_pimpl::
  post_componentproperties ()
  {
    return componentProperties;
  }

  // devicethatloadedthiscomponentref_pimpl
  //

  void devicethatloadedthiscomponentref_pimpl::
  pre ()
  {
      _refid = "";
  }

  void devicethatloadedthiscomponentref_pimpl::
  refid (const ::std::string& refid)
  {
      this->_refid = refid;
  }

  std::string devicethatloadedthiscomponentref_pimpl::
  post_devicethatloadedthiscomponentref ()
  {
      return _refid;
  }

  // deviceusedbythiscomponentref_pimpl
  //

  void deviceusedbythiscomponentref_pimpl::
  pre ()
  {
      info = std::pair<std::string, std::string>();
  }

  void deviceusedbythiscomponentref_pimpl::
  refid (const ::std::string& refid)
  {
      info.first = refid;
  }

  void deviceusedbythiscomponentref_pimpl::
  usesrefid (const ::std::string& usesrefid)
  {
      info.second = usesrefid;
  }

  std::pair<std::string, std::string> deviceusedbythiscomponentref_pimpl::
  post_deviceusedbythiscomponentref ()
  {
      return info;
  }

  // idvalueref_pimpl
  //

  void idvalue_pimpl::
  pre ()
  {
    LOG_TRACE(dcd_parser, "pre idvalue");
    simple = ossie::IdValue();
  }

  void idvalue_pimpl::
  id (const ::std::string& id)
  {
    LOG_TRACE(dcd_parser, "idvalue id: " << id);
    simple._id = id;
  }

  void idvalue_pimpl::
  value (const ::std::string& value)
  {
    LOG_TRACE(dcd_parser, "idvalue value: " << value);
    simple._value = value;
  }

  const ossie::IdValue& idvalue_pimpl::
  post_idvalue ()
  {
    LOG_TRACE(dcd_parser, "post idvalue");
    return simple;
  }


  // simpleref_pimpl
  //

  void simpleref_pimpl::
  pre ()
  {
    LOG_TRACE(dcd_parser, "pre simpleref");
    simple = ossie::SimplePropertyRef();
  }

  void simpleref_pimpl::
  refid (const ::std::string& refid)
  {
    LOG_TRACE(dcd_parser, "simpleref id: " << refid);
    simple._id = refid;
  }

  void simpleref_pimpl::
  value (const ::std::string& value)
  {
    LOG_TRACE(dcd_parser, "simpleref value: " << value);
    simple._value = value;
  }

  const ossie::SimplePropertyRef& simpleref_pimpl::
  post_simpleref ()
  {
    LOG_TRACE(dcd_parser, "post simpleref");
    return simple;
  }

  // simplesequenceref_pimpl
  //

  void simplesequenceref_pimpl::
  pre ()
  {
    simpleseq._values.clear();
    simpleseq = ossie::SimpleSequencePropertyRef();
  }

  void simplesequenceref_pimpl::
  values (const ::std::vector<std::string>& values)
  {
      simpleseq._values = values;
  }

  void simplesequenceref_pimpl::
  refid (const ::std::string& refid)
  {
      simpleseq._id = refid;
  }

  const ossie::SimpleSequencePropertyRef& simplesequenceref_pimpl::
  post_simplesequenceref ()
  {
      return simpleseq;
  }

  // structref_pimpl
  //

  void structref_pimpl::
  pre ()
  {
    structref._values.clear();
    structref = ossie::StructPropertyRef();
  }

  void structref_pimpl::
  simpleref (const ossie::SimplePropertyRef& simpleref)
  {
    structref._values.insert(simpleref._id,std::auto_ptr<ossie::ComponentProperty>(simpleref.clone()) );
  }

  void structref_pimpl::
  simplesequenceref (const ossie::SimpleSequencePropertyRef& simplesequenceref)
  {
    structref._values.insert(simplesequenceref._id,std::auto_ptr<ossie::ComponentProperty>(simplesequenceref.clone()) );
  }

  void structref_pimpl::
  refid (const ::std::string& refid)
  {
      structref._id = refid;
  }

  const ossie::StructPropertyRef& structref_pimpl::
  post_structref ()
  {
      return structref;
  }

  // structsequenceref_pimpl
  //

  void structsequenceref_pimpl::
  pre ()
  {
    structsequenceref._values.clear();
    structsequenceref =  ossie::StructSequencePropertyRef();
  }

  void structsequenceref_pimpl::
  structvalue (const ossie::ComponentPropertyMap& value)
  {
      structsequenceref._values.push_back(value);
  }

  void structsequenceref_pimpl::
  refid (const std::string& refid)
  {
    structsequenceref._id = refid;
  }

const ossie::StructSequencePropertyRef& structsequenceref_pimpl::
  post_structsequenceref ()
  {
      return structsequenceref;
  }

  // structvalue_pimpl
  //

  void structvalue_pimpl::
  pre ()
  {
      values.clear();
  }

  void structvalue_pimpl::
  simpleref (const ossie::SimplePropertyRef& simpleref)
  {
    values.insert(simpleref._id,std::auto_ptr<ossie::ComponentProperty>(simpleref.clone()) );
  }

  void structvalue_pimpl::
  simplesequenceref (const ossie::SimpleSequencePropertyRef& simplesequenceref)
  {
    values.insert(simplesequenceref._id,std::auto_ptr<ossie::ComponentProperty>(simplesequenceref.clone()) );
  }

  const ossie::ComponentPropertyMap& structvalue_pimpl::
  post_structvalue ()
  {
    return values;
  }

  // values_pimpl
  //

  void values_pimpl::
  pre ()
  {
      values.clear();
  }

  void values_pimpl::
  value (const ::std::string& value)
  {
      values.push_back(value);
  }

  ::std::vector<std::string> values_pimpl::
  post_values ()
  {
      return values;
  }

  // componentinstantiationref_pimpl
  //

  void componentinstantiationref_pimpl::
  pre ()
  {
      _refid = "";
  }

  void componentinstantiationref_pimpl::
  refid (const ::std::string& refid)
  {
      _refid = refid;
  }

  ::std::string componentinstantiationref_pimpl::
  post_componentinstantiationref ()
  {
      return _refid;
  }

  // domainmanager_pimpl
  //

  void domainmanager_pimpl::
  pre ()
  {
    domainmanager = "";
  }

  void domainmanager_pimpl::
  namingservice (const ::std::string& namingservice)
  {
    domainmanager = namingservice;
  }

  void domainmanager_pimpl::
  stringifiedobjectref (const ::std::string& stringifiedobjectref)
  {
    // TODO
    //
  }

  ::std::string domainmanager_pimpl::
  post_domainmanager ()
  {
    return domainmanager;
  }

  // findby_pimpl
  //

  void findby_pimpl::
  pre ()
  {
      findby.reset(new ossie::FindBy());
  }

  void findby_pimpl::
  namingservice (const ::std::string& namingservice)
  {
      findby->findByNamingService = namingservice;
  }

  void findby_pimpl::
  stringifiedobjectref (const ::std::string& stringifiedobjectref)
  {
      findby->findByStringifiedObjectRef = stringifiedobjectref;
  }

  void findby_pimpl::
  domainfinder (const ::std::pair<std::string, std::string>& domainfinder)
  {
      findby->findByDomainFinderName = domainfinder.first;
      findby->findByDomainFinderType = domainfinder.second;
  }

  ::ossie::FindBy findby_pimpl::
  post_findby ()
  {
    return *findby;
  }

  // namingservice_pimpl
  //

  void namingservice_pimpl::
  pre ()
  {
    _name = "";
  }

  void namingservice_pimpl::
  name (const ::std::string& name)
  {
    this->_name = name;
  }

  ::std::string namingservice_pimpl::
  post_namingservice ()
  {
    return _name;
  }

  // domainfinder_pimpl
  //

  void domainfinder_pimpl::
  pre ()
  {
      domainfinder = std::pair<std::string, std::string>();
  }

  void domainfinder_pimpl::
  type (const ::std::string& type)
  {
      domainfinder.second = type;
  }

  void domainfinder_pimpl::
  name (const ::std::string& name)
  {
      domainfinder.first = name;
  }

  ::std::pair<std::string, std::string> domainfinder_pimpl::
  post_domainfinder ()
  {
    return domainfinder;
  }

  // filesystemnames_pimpl
  //

  void filesystemnames_pimpl::
  pre ()
  {
  }

  void filesystemnames_pimpl::
  filesystemname ()
  {
  }

  void filesystemnames_pimpl::
  post_filesystemnames ()
  {
  }

  // filesystemname_pimpl
  //

  void filesystemname_pimpl::
  pre ()
  {
  }

  void filesystemname_pimpl::
  mountname (const ::std::string& mountname)
  {
    // TODO
    //
  }

  void filesystemname_pimpl::
  deviceid (const ::std::string& deviceid)
  {
    // TODO
    //
  }

  void filesystemname_pimpl::
  post_filesystemname ()
  {
  }

  // connections_pimpl
  //

  void connections_pimpl::
  pre ()
  {
    LOG_TRACE(dcd_parser, "pre connections");
    connections.clear();
  }

  void connections_pimpl::
  connectinterface (const ::ossie::Connection& connectinterface)
  {
      connections.push_back(connectinterface);

      // Output ConnectionID
      if (strlen(connectinterface.getID()) == 0) {
          connections.back().connectionId = ossie::generateUUID();;
      }
      LOG_TRACE(dcd_parser, "added connection id " << connections.back().getID() << " type " << connections.back().getType());
  }

  ::std::vector<ossie::Connection> connections_pimpl::
  post_connections ()
  {
    LOG_TRACE(dcd_parser, "post connections");
    return connections;
  }

  // connectinterface_pimpl
  //

  void connectinterface_pimpl::
  pre ()
  {
    LOG_TRACE(dcd_parser, "pre connect interface");
    connection.reset(new ossie::Connection());
  }

  void connectinterface_pimpl::
  usesport (const ::ossie::UsesPort& usesport)
  {
      connection->usesPort = usesport;
  }

  void connectinterface_pimpl::
  providesport (const ::ossie::ProvidesPort& providesport)
  {
      connection->providesPort = providesport;
      connection->type = ::ossie::Connection::PROVIDESPORT;
  }

  void connectinterface_pimpl::
  componentsupportedinterface (const ::ossie::ComponentSupportedInterface& componentsupportedinterface)
  {
      connection->componentSupportedInterface = componentsupportedinterface;
      connection->type = ::ossie::Connection::COMPONENTSUPPORTEDINTERFACE;
  }

  void connectinterface_pimpl::
  findby (const ::ossie::FindBy& findby)
  {
      connection->findBy = findby;
      connection->type = ::ossie::Connection::FINDBY;
  }

  void connectinterface_pimpl::
  id (const ::std::string& id)
  {
      LOG_TRACE(dcd_parser, "connection id " << id);
      connection->connectionId = id;
  }

  ::ossie::Connection connectinterface_pimpl::
  post_connectinterface ()
  {
    LOG_TRACE(dcd_parser, "post connect interface");
    return *connection;
  }
  
  // usesport_pimpl
  //

  void usesport_pimpl::
  pre ()
  {
      usesPort.reset(new ossie::UsesPort());
  }

  void usesport_pimpl::
  usesidentifier (const ::std::string& usesidentifier)
  {
      usesPort->identifier = usesidentifier;
  }

  void usesport_pimpl::
  componentinstantiationref (const ::std::string& componentinstantiationref)
  {
      usesPort->setComponentInstantiationRef(componentinstantiationref);
  }

  void usesport_pimpl::
  devicethatloadedthiscomponentref (const std::string& devicethatloadedthiscomponentref)
  {
      usesPort->setDeviceThatLoadedThisComponentRef(devicethatloadedthiscomponentref);
  }

  void usesport_pimpl::
  deviceusedbythiscomponentref (const ::std::pair<std::string, std::string>& ref)
  {
      usesPort->setDeviceUsedByThisComponentRef(ref.first, ref.second);
  }

  void usesport_pimpl::
  findby (const ::ossie::FindBy& findby)
  {
      usesPort->setFindBy(findby);
  }

  ::ossie::UsesPort usesport_pimpl::
  post_usesport ()
  {
    return *usesPort;
  }

  // providesport_pimpl
  //

  void providesport_pimpl::
  pre ()
  {
      provPort.reset(new ossie::ProvidesPort());
  }

  void providesport_pimpl::
  providesidentifier (const ::std::string& providesidentifier)
  {
      provPort->identifier = providesidentifier;
  }

  void providesport_pimpl::
  componentinstantiationref (const ::std::string& componentinstantiationref)
  {
      provPort->setComponentInstantiationRef(componentinstantiationref);
  }

  void providesport_pimpl::
  devicethatloadedthiscomponentref (const std::string& devicethatloadedthiscomponentref)
  {
      provPort->setDeviceThatLoadedThisComponentRef(devicethatloadedthiscomponentref);
  }

  void providesport_pimpl::
  deviceusedbythiscomponentref (const ::std::pair<std::string, std::string>& ref)
  {
      provPort->setDeviceUsedByThisComponentRef(ref.first, ref.second);
  }

  void providesport_pimpl::
  findby (const ::ossie::FindBy& findby)
  {
      provPort->setFindBy(findby);
  }

  ::ossie::ProvidesPort providesport_pimpl::
  post_providesport ()
  {
    return *provPort;
  }

  // componentsupportedinterface_pimpl
  //

  void componentsupportedinterface_pimpl::
  pre ()
  {
      ciface.reset(new ossie::ComponentSupportedInterface());
  }

  void componentsupportedinterface_pimpl::
  supportedidentifier (const ::std::string& supportedidentifier)
  {
      ciface->identifier = supportedidentifier;
  }

  void componentsupportedinterface_pimpl::
  componentinstantiationref (const ::std::string& componentinstantiationref)
  {
      ciface->setComponentInstantiationRef(componentinstantiationref);
  }

  void componentsupportedinterface_pimpl::
  findby (const ::ossie::FindBy& findby)
  {
      ciface->setFindBy(findby);
  }

  ::ossie::ComponentSupportedInterface componentsupportedinterface_pimpl::
  post_componentsupportedinterface ()
  {
    return *ciface;
  }
  
}

