// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
// OpenTrep
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/BasChronometer.hpp>
#include <opentrep/factory/FacWorld.hpp>
#include <opentrep/command/SociSessionManager.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/command/IndexBuilder.hpp>
#include <opentrep/command/RequestInterpreter.hpp>
#include <opentrep/factory/FacOpenTrepServiceContext.hpp>
#include <opentrep/service/OPENTREP_ServiceContext.hpp>
#include <opentrep/service/ServiceUtilities.hpp>
#include <opentrep/service/Logger.hpp>
#include <opentrep/OPENTREP_Service.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::
  OPENTREP_Service (std::ostream& ioLogStream, const DBParams& iDBParams,
                    const TravelDatabaseName_T& iXapianDatabaseFilepath)
    : _opentrepServiceContext (NULL) {
    init (ioLogStream, iDBParams, iXapianDatabaseFilepath);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::OPENTREP_Service()
    : _opentrepServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::OPENTREP_Service (const OPENTREP_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::~OPENTREP_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void logInit (const LOG::EN_LogLevel iLogLevel,
                std::ostream& ioLogOutputFile) {
    Logger::instance().setLogParameters (iLogLevel, ioLogOutputFile);
  }

  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::init(std::ostream& ioLogStream,
                              const DBParams& iDBParams,
                              const TravelDatabaseName_T& iTravelDatabaseName) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Check that the Xapian travel database is not empty
    if (iTravelDatabaseName.empty() == true) {
      std::ostringstream errorStr;
      errorStr << "The filepath for the Xapian travel database is "
               << "not specified.";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw XapianTravelDatabaseWrongPathnameException (errorStr.str());
    }

    // Check that the parameters for the SQL database are not empty
    if (iDBParams.check() == false) {
      std::ostringstream errorStr;
      errorStr << "At least one of the parameters for the SQL "
               << "database is empty: " << iDBParams;
      OPENTREP_LOG_ERROR (errorStr.str());
      throw XapianTravelDatabaseEmptyException (errorStr.str());
    }
    
    // Initialise the context
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = 
      FacOpenTrepServiceContext::instance().create (iTravelDatabaseName);
    _opentrepServiceContext = &lOPENTREP_ServiceContext;

    // Initialise the SOCI Session
    soci::session* lSociSession_ptr = lOPENTREP_ServiceContext.getSociSession();
    SociSessionManager::init (lSociSession_ptr, iDBParams);
    assert (lSociSession_ptr != NULL);
    lOPENTREP_ServiceContext.setSociSession (lSociSession_ptr);

    // Instanciate an empty World object
    World& lWorld = FacWorld::instance().create();
    lOPENTREP_ServiceContext.setWorld (lWorld);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::finalise() {
    assert (_opentrepServiceContext != NULL);

    // Terminate the SOCI Session
    soci::session* lSociSession_ptr = _opentrepServiceContext->getSociSession();
    SociSessionManager::finalise (lSociSession_ptr);
    _opentrepServiceContext->setSociSession (lSociSession_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T OPENTREP_Service::buildSearchIndex() {
    NbOfDBEntries_T oNbOfEntries = 0;
    
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
					    " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext= *_opentrepServiceContext;

    // Retrieve the SOCI Session
    soci::session& lSociSession =
      lOPENTREP_ServiceContext.getSociSessionHandler();
      
    // Retrieve the Xapian database name (directorty of the index)
    const TravelDatabaseName_T& lTravelDatabaseName =
      lOPENTREP_ServiceContext.getTravelDatabaseName();
      
    // Delegate the index building to the dedicated command
    BasChronometer lBuildSearchIndexChronometer;
    lBuildSearchIndexChronometer.start();
    oNbOfEntries = IndexBuilder::buildSearchIndex (lSociSession,
                                                   lTravelDatabaseName);
    const double lBuildSearchIndexMeasure =
      lBuildSearchIndexChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Built Xapian database (index): "
                        << lBuildSearchIndexMeasure << " - "
                        << lOPENTREP_ServiceContext.display());

    return oNbOfEntries;
  }
  
  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T OPENTREP_Service::
  interpretTravelRequest (const std::string& iTravelQuery,
                          LocationList_T& ioLocationList,
                          WordList_T& ioWordList) {
    NbOfMatches_T nbOfMatches = 0;

    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext= *_opentrepServiceContext;

    // Get the date-time for the present time
    boost::posix_time::ptime lNowDateTime =
      boost::posix_time::second_clock::local_time();
    // boost::gregorian::date lNowDate = lNowDateTime.date();

    // DEBUG
    OPENTREP_LOG_DEBUG (std::endl
                        << "==================================================="
                        << std::endl
                        << lNowDateTime << " - Match query '" << iTravelQuery
                        << "' on Xapian database (index)");
    
    // Check that the travel request is not empty
    if (iTravelQuery.empty() == true) {
      std::ostringstream errorStr;
      errorStr << "The travel request is empty.";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw TravelRequestEmptyException (errorStr.str());
    }
    
    // Retrieve the SOCI Session
    soci::session& lSociSession =
      lOPENTREP_ServiceContext.getSociSessionHandler();
      
    // Retrieve the Xapian database name (directorty of the index)
    const TravelDatabaseName_T& lTravelDatabaseName =
      lOPENTREP_ServiceContext.getTravelDatabaseName();
      
    // Delegate the query execution to the dedicated command
    BasChronometer lRequestInterpreterChronometer;
    lRequestInterpreterChronometer.start();
    nbOfMatches =
      RequestInterpreter::interpretTravelRequest (lSociSession,
                                                  lTravelDatabaseName,
                                                  iTravelQuery,
                                                  ioLocationList,
                                                  ioWordList);
    const double lRequestInterpreterMeasure =
      lRequestInterpreterChronometer.elapsed();

    // DEBUG
    OPENTREP_LOG_DEBUG ("Match query on Xapian database (index): "
                        << lRequestInterpreterMeasure);
      
    return nbOfMatches;
  }
  
}
