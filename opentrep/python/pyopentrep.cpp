// C
#include <cassert>
// STL
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
// Boost String
#include <boost/python.hpp>
// OpenTREP
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/Location.hpp>

namespace OPENTREP {

  struct OpenTrepSearcher {
  public:

    /** Wrapper around the search use case. */
    std::string search (const std::string& iTravelQuery) {
      const bool areFullDetailsRequired = false;
      return searchImpl (iTravelQuery, areFullDetailsRequired);
    }

    /** Wrapper around the search use case. */
    std::string searchWithFullDetails (const std::string& iTravelQuery) {
      const bool areFullDetailsRequired = true;
      return searchImpl (iTravelQuery, areFullDetailsRequired);
    }

  private:
    /** Wrapper around the search use case. */
    std::string searchImpl (const std::string& iTravelQuery,
                            const bool areFullDetailsRequired) {
      std::ostringstream oNoDetailedStr;
      std::ostringstream oDetailedStr;

      // Sanity check
      if (_logOutputStream == NULL) {
        oNoDetailedStr << "The log filepath is not valid." << std::endl;
        return oNoDetailedStr.str();
      }
      assert (_logOutputStream != NULL);
      
      try {

        // DEBUG
        *_logOutputStream << "Python search for '" << iTravelQuery << "'"
                          << std::endl;
      
        if (_opentrepService == NULL) {
          oNoDetailedStr << "The OpenTREP service has not been initialised, "
                         << "i.e., the init() method has not been called "
                         << "correctly on the OpenTrepSearcher object. Please "
                         << "check that all the parameters are not empty and "
                         << "point to actual files.";
          *_logOutputStream << oNoDetailedStr.str();
          return oNoDetailedStr.str();
        }
        assert (_opentrepService != NULL);
        
        // Query the Xapian database (index)
        WordList_T lNonMatchedWordList;
        LocationList_T lLocationList;
        const NbOfMatches_T nbOfMatches =
          _opentrepService->interpretTravelRequest (iTravelQuery, lLocationList,
                                                    lNonMatchedWordList);
      
        // DEBUG
        *_logOutputStream << "Python search for '" << iTravelQuery << "' gave "
                          << nbOfMatches << " matches." << std::endl;
      
        if (nbOfMatches != 0) {
          NbOfMatches_T idx = 0;
          
          for(LocationList_T::const_iterator itLocation = lLocationList.begin();
               itLocation != lLocationList.end(); ++itLocation, ++idx) {
            const Location& lLocation = *itLocation;
            
            if (idx != 0) {
              oNoDetailedStr << ",";
            }
            
            oNoDetailedStr << lLocation.getLocationCode();
            oDetailedStr << idx+1 << ". " << lLocation.toShortString()
                         << std::endl;

            // List of extra matching locations (those with the same
            // matching weight/percentage)
            const LocationList_T& lExtraLocationList =
              lLocation.getExtraLocationList();
            if (lExtraLocationList.empty() == false) {
              oDetailedStr << "  Extra matches: " << std::endl;
              
              NbOfMatches_T idxExtra = 0;
              for (LocationList_T::const_iterator itLoc =
                     lExtraLocationList.begin();
                   itLoc != lExtraLocationList.end(); ++itLoc, ++idxExtra) {
                oNoDetailedStr << ":";
                oDetailedStr << "    " << idx+1 << "." << idxExtra+1 << ". ";
                
                const Location& lExtraLocation = *itLoc;
                oNoDetailedStr << lExtraLocation.getLocationCode();
                oDetailedStr << lExtraLocation << std::endl;
              }
            }

            // The matching weight/percentage is the same for the main
            // and the extra matching locations
            oNoDetailedStr << "/" << lLocation.getPercentage();
            
            // List of alternate matching locations (those with a lower
            // matching weight/percentage)
            const LocationList_T& lAlternateLocationList =
              lLocation.getAlternateLocationList();
            if (lAlternateLocationList.empty() == false) {
              oDetailedStr << "  Alternate matches: " << std::endl;
              
              NbOfMatches_T idxAlter = 0;
              for (LocationList_T::const_iterator itLoc =
                     lAlternateLocationList.begin();
                   itLoc != lAlternateLocationList.end(); ++itLoc, ++idxAlter) {
                oNoDetailedStr << "-";
                oDetailedStr << "    " << idx+1 << "." << idxAlter+1 << ". ";
                
                const Location& lAlternateLocation = *itLoc;
                oNoDetailedStr << lAlternateLocation.getLocationCode()
                               << "/" << lAlternateLocation.getPercentage();
                oDetailedStr << lAlternateLocation << std::endl;
              }
            }
          }
        }
        
        if (lNonMatchedWordList.empty() == false) {
          oNoDetailedStr << ";";
          oDetailedStr << "Not recognised words:" << std::endl;
          NbOfMatches_T idx = 0;
          for (WordList_T::const_iterator itWord = lNonMatchedWordList.begin();
               itWord != lNonMatchedWordList.end(); ++itWord, ++idx) {
            const Word_T& lWord = *itWord;
            if (idx != 0) {
              oNoDetailedStr << ",";
              oDetailedStr << idx+1 << "." << std::endl;
            }
            oNoDetailedStr << lWord;
            oDetailedStr << lWord;
          }
        }
        
        // DEBUG
        *_logOutputStream << "Python search for '" << iTravelQuery
                          << "' yielded:" << std::endl;
        
        // DEBUG
        *_logOutputStream << "Short version: "
                          << oNoDetailedStr.str() << std::endl;
        *_logOutputStream << "Long version: "
                          << oDetailedStr.str() << std::endl;

      } catch (const RootException& eOpenTrepError) {
        *_logOutputStream << "OpenTrep error: "  << eOpenTrepError.what()
                          << std::endl;
        
      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }

      // Return the string corresponding to the request (either with
      // or without details).
      if (areFullDetailsRequired == true) {
        return oDetailedStr.str();
      } else {
        return oNoDetailedStr.str();
      }
    }

  public:
    /** Default constructor. */
    OpenTrepSearcher() : _opentrepService (NULL), _logOutputStream (NULL) {
    }
    
    /** Default copy constructor. */
    OpenTrepSearcher (const OpenTrepSearcher& iOpenTrepSearcher)
      : _opentrepService (iOpenTrepSearcher._opentrepService),
        _logOutputStream (iOpenTrepSearcher._logOutputStream) {
    }

    /** Default constructor. */
    ~OpenTrepSearcher() {
      _opentrepService = NULL;
      _logOutputStream = NULL;
    }
    
    /** Wrapper around the search use case. */
    bool init (const std::string& iXapianDatabaseFilepath,
               const std::string& iLogFilepath,
               const std::string& iDBUser, const std::string& iDBPasswd,
               const std::string& iDBHost, const std::string& iDBPort,
               const std::string& iDBDBName) {
      bool isEverythingOK = true;

      try {
        
        // TODO: use Boost Filesystem to check the filepaths
        if (iXapianDatabaseFilepath.empty() == true
            || iLogFilepath.empty() == true) {
          isEverythingOK = false;
          return isEverythingOK;
        }
        
        // Set the log parameters
        _logOutputStream = new std::ofstream;
        assert (_logOutputStream != NULL);
        
        // Open and clean the log outputfile
        _logOutputStream->open (iLogFilepath.c_str());
        _logOutputStream->clear();
        
        // DEBUG
        *_logOutputStream << "Python wrapper initialisation" << std::endl;
        
        // Initialise the context
        DBParams lDBParams (iDBUser, iDBPasswd, iDBHost, iDBPort, iDBDBName);
        _opentrepService = new OPENTREP_Service (*_logOutputStream, lDBParams,
                                                 iXapianDatabaseFilepath);
        
        // DEBUG
        *_logOutputStream << "Python wrapper initialised" << std::endl;
        
      } catch (const RootException& eOpenTrepError) {
        *_logOutputStream << "OpenTrep error: "  << eOpenTrepError.what()
                          << std::endl;
        
      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }
      
      return isEverythingOK;
    }

    /** Wrapper around the search use case. */
    bool finalize () {
      bool isEverythingOK = true;

      try {
        
        // Finalize the context
        if (_opentrepService != NULL) {
          delete _opentrepService; _opentrepService = NULL;
        }
        
        // Close the output stream
        if (_logOutputStream != NULL) {
          // DEBUG
          *_logOutputStream << "Python wrapper finalization" << std::endl;
          _logOutputStream->close();
          delete _logOutputStream; _logOutputStream = NULL;
        }
        
      } catch (...) {
      }
      
      return isEverythingOK;
    }

  private:
    /** Handle on the OpenTREP services (API). */
    OPENTREP_Service* _opentrepService;
    std::ofstream* _logOutputStream;
  };

}

// /////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(libpyopentrep) {
  boost::python::class_<OPENTREP::OpenTrepSearcher> ("OpenTrepSearcher")
    .def ("search", &OPENTREP::OpenTrepSearcher::search)
    .def ("searchWithFullDetails", &OPENTREP::OpenTrepSearcher::searchWithFullDetails)
    .def ("init", &OPENTREP::OpenTrepSearcher::init)
    .def ("finalize", &OPENTREP::OpenTrepSearcher::finalize);
}