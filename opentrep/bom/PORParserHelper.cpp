// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <vector>
#include <fstream>
// Boost
#include <boost/lexical_cast.hpp>
//#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_UNICODE
// OpenTREP
#include <opentrep/basic/BasParserTypes.hpp>
#include <opentrep/bom/PORParserHelper.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  /** Namespaces. */
  namespace bsq = boost::spirit::qi;
  //namespace bsa = boost::spirit::ascii;
  namespace bsu = boost::spirit::unicode;
  namespace bsc = boost::spirit::classic;

  namespace PorParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::ParserSemanticAction (Location& ioLocation)
      : _location (ioLocation) {
    }      
   
    // //////////////////////////////////////////////////////////////////
    storeIataCode::storeIataCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeIataCode::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {

      const std::string lIataCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::IATACode_T lIataCode (lIataCodeStr);
      _location.setIataCode (lIataCode);

      // Reset the values
      //_location.resetMatrix();
      //_location._itAltNameShortList.clear();

      // DEBUG
      //OPENTREP_LOG_DEBUG ("IATA code: " << _location.getIataCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeIcaoCode::storeIcaoCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeIcaoCode::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {

      const std::string lIcaoCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::ICAOCode_T lIcaoCode (lIcaoCodeStr);
      _location.setIcaoCode (lIcaoCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("ICAO code: " << _location.getIcaoCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeFaaCode::storeFaaCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeFaaCode::operator() (std::vector<uchar_t> iChar,
                                   bsq::unused_type, bsq::unused_type) const {

      const std::string lFaaCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::FAACode_T lFaaCode (lFaaCodeStr);
      _location.setFaaCode (lFaaCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("FAA code: " << _location.getFaaCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeGeonamesID::storeGeonamesID (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeGeonamesID::operator() (unsigned int iPorId,
                                      bsq::unused_type, bsq::unused_type) const {
      _location.setGeonamesID (iPorId);
      
      // DEBUG
      //OPENTREP_LOG_DEBUG ("POR ID: " << _location.getPorID());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeValidityID::storeValidityID (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeValidityID::operator() (unsigned int iValidityId,
                                      bsq::unused_type, bsq::unused_type) const {
      //_location.setValidityID (iPorId);
      
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Validity ID: " << _location.getValidityID());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeCommonName::storeCommonName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCommonName::operator() (std::vector<uchar_t> iChar,
                                      bsq::unused_type, bsq::unused_type) const {

      const std::string lCommonNameStr (iChar.begin(), iChar.end());
      const OPENTREP::CommonName_T lCommonName (lCommonNameStr);
      _location.setCommonName (lCommonName);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Common name: " << _location.getCommonName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAsciiName::storeAsciiName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAsciiName::operator() (std::vector<uchar_t> iChar,
                                     bsq::unused_type, bsq::unused_type) const {

      const std::string lAsciiNameStr (iChar.begin(), iChar.end());
      const OPENTREP::ASCIIName_T lAsciiName (lAsciiNameStr);
      _location.setAsciiName (lAsciiName);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("ASCII name: " << _location.getAsciiName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAltNameShort::storeAltNameShort (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltNameShort::operator() (std::vector<uchar_t> iChar,
                                        bsq::unused_type,
                                        bsq::unused_type)const {

      const std::string lAltNameShortStr (iChar.begin(), iChar.end());
      const OPENTREP::AltNameShortListString_T lAltNameShort (lAltNameShortStr);
      _location._itAltNameShortList.push_back (lAltNameShort);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt name short: " << lAltNameShortStr);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltNameShortListString::
    storeAltNameShortListString (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltNameShortListString::
    operator() (bsq::unused_type, bsq::unused_type, bsq::unused_type)const {
      _location.consolidateAltNameShortListString();
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alternative name short list: " << _location.getAltNameShortListString());
    }

    // //////////////////////////////////////////////////////////////////
    storeTvlPORCode::storeTvlPORCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTvlPORCode::operator() (std::vector<uchar_t> iChar,
                                      bsq::unused_type,
                                      bsq::unused_type)const {

      const std::string lTvlPORCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::TvlPORListString_T lTvlPORCode (lTvlPORCodeStr);
      _location._itTvlPORList.push_back (lTvlPORCode);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Travel-related IATA code: " << lTvlPORCodeStr);
    }

    // //////////////////////////////////////////////////////////////////
    storeTvlPORListString::storeTvlPORListString (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTvlPORListString::operator() (bsq::unused_type, bsq::unused_type,
                                            bsq::unused_type)const {
      _location.consolidateTvlPORListString();
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Travel-related POR list: " << _location.getTvlPORListString());
    }

    // //////////////////////////////////////////////////////////////////
    storeLatitude::storeLatitude (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeLatitude::operator() (double iLatitude,
                                    bsq::unused_type, bsq::unused_type) const {
      _location.setLatitude (iLatitude);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Latitude: " << _location.getLatitude());
    }

    // //////////////////////////////////////////////////////////////////
    storeLongitude::storeLongitude (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeLongitude::operator() (double iLongitude,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setLongitude (iLongitude);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Longitude: " << _location.getLongitude());
    }

    // //////////////////////////////////////////////////////////////////
    storeFeatureClass::storeFeatureClass (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeFeatureClass::operator() (std::vector<uchar_t> iChar,
                                        bsq::unused_type,
                                        bsq::unused_type) const {
      const std::string lFeatClassStr (iChar.begin(), iChar.end());
      const FeatureClass_T lFeatClass (lFeatClassStr);
      _location.setFeatureClass (lFeatClass);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Feature class: " << _location.getFeatureClass());
    }

    // //////////////////////////////////////////////////////////////////
    storeFeatureCode::storeFeatureCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeFeatureCode::operator() (std::vector<uchar_t> iChar,
                                       bsq::unused_type,
                                       bsq::unused_type) const {
      const std::string lFeatCodeStr (iChar.begin(), iChar.end());
      const FeatureClass_T lFeatCode (lFeatCodeStr);
      _location.setFeatureCode (lFeatCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Feature code: " << _location.getFeatureCode());
    }

    // //////////////////////////////////////////////////////////////////
    storePageRank::storePageRank (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePageRank::operator() (double iPageRank,
                                    bsq::unused_type, bsq::unused_type) const {
      _location.setPageRank (100.0 * iPageRank);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("PageRank: " << _location.getPageRank());
    }

    // //////////////////////////////////////////////////////////////////
    storeDateFrom::storeDateFrom (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDateFrom::operator() (bsq::unused_type,
                                    bsq::unused_type, bsq::unused_type) const {
      /*
      const OPENTREP::Date_T& lDateFrom = _location.calculateDate();
      _location.setDateFrom (lDateFrom);
      */

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Date from: " << _location.getDateFrom());
    }

    // //////////////////////////////////////////////////////////////////
    storeDateUntil::storeDateUntil (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDateUntil::operator() (bsq::unused_type,
                                     bsq::unused_type, bsq::unused_type) const {
      /*
      const OPENTREP::Date_T& lDateUntil = _location.calculateDate();
      _location.setDateUntil (lDateUntil);
      */

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Date until: " << _location.getDateUntil());
    }

    // //////////////////////////////////////////////////////////////////
    storeComments::storeComments (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeComments::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {
      /*
      const std::string lCommentsStr (iChar.begin(), iChar.end());
      const CountryCode_T lComments (lCommentsStr);
      _location.setComments (lComments);
      */

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Comments: " << _location.getComments());
    }

    // //////////////////////////////////////////////////////////////////
    storeCountryCode::storeCountryCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeCountryCode::operator() (std::vector<uchar_t> iChar,
                                       bsq::unused_type,
                                       bsq::unused_type) const {
      const std::string lCountryCodeStr (iChar.begin(), iChar.end());
      const CountryCode_T lCountryCode (lCountryCodeStr);
      _location.setCountryCode (lCountryCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Country code: " << _location.getCountryCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeAltCountryCode::storeAltCountryCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAltCountryCode::operator() (std::vector<uchar_t> iChar,
                                          bsq::unused_type,
                                          bsq::unused_type) const {
      const std::string lCountryCodeStr (iChar.begin(), iChar.end());
      const CountryCode_T lCountryCode (lCountryCodeStr);
      _location.setAltCountryCode (lCountryCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Alt country code: " << _location.getAltCountryCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeCountryName::storeCountryName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeCountryName::operator() (std::vector<uchar_t> iChar,
                                       bsq::unused_type,
                                       bsq::unused_type) const {
      const std::string lCountryNameStr (iChar.begin(), iChar.end());
      const CountryName_T lCountryName (lCountryNameStr);
      _location.setCountryName (lCountryName);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Country name: " << _location.getCountryName());
    }

    // //////////////////////////////////////////////////////////////////
    storeContinentName::storeContinentName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeContinentName::operator() (std::vector<uchar_t> iChar,
                                         bsq::unused_type,
                                         bsq::unused_type) const {
      const std::string lContinentNameStr (iChar.begin(), iChar.end());
      const ContinentName_T lContinentName (lContinentNameStr);
      _location.setContinentName (lContinentName);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Continent name: " << _location.getContinentName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm1Code::storeAdm1Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm1Code::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin1Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin1Code (lAdmCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm1 code: " << _location.getAdmin1Code());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm1UtfName::storeAdm1UtfName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm1UtfName::operator() (std::vector<uchar_t> iChar,
                                       bsq::unused_type,
                                       bsq::unused_type) const {
      const std::string lAdmNameStr (iChar.begin(), iChar.end());
      const Admin1UTFName_T lAdmName (lAdmNameStr);
      _location.setAdmin1UtfName (lAdmName);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm1 UTF8 name: " << _location.getAdmin1UtfName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm1AsciiName::storeAdm1AsciiName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm1AsciiName::operator() (std::vector<uchar_t> iChar,
                                         bsq::unused_type,
                                         bsq::unused_type) const {
      const std::string lAdmNameStr (iChar.begin(), iChar.end());
      const Admin1ASCIIName_T lAdmName (lAdmNameStr);
      _location.setAdmin1AsciiName (lAdmName);
      // DEBUG
      //OPENTREP_LOG_DEBUG("Adm1 ASCII name: "<< _location.getAdmin1AsciiName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm2Code::storeAdm2Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm2Code::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin2Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin2Code (lAdmCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm2 code: " << _location.getAdmin2Code());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm2UtfName::storeAdm2UtfName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm2UtfName::operator() (std::vector<uchar_t> iChar,
                                       bsq::unused_type,
                                       bsq::unused_type) const {
      const std::string lAdmNameStr (iChar.begin(), iChar.end());
      const Admin2UTFName_T lAdmName (lAdmNameStr);
      _location.setAdmin2UtfName (lAdmName);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm2 UTF8 name: " << _location.getAdmin2UtfName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm2AsciiName::storeAdm2AsciiName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm2AsciiName::operator() (std::vector<uchar_t> iChar,
                                         bsq::unused_type,
                                         bsq::unused_type) const {
      const std::string lAdmNameStr (iChar.begin(), iChar.end());
      const Admin2ASCIIName_T lAdmName (lAdmNameStr);
      _location.setAdmin2AsciiName (lAdmName);
      // DEBUG
      //OPENTREP_LOG_DEBUG("Adm2 ASCII name: "<< _location.getAdmin2AsciiName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm3Code::storeAdm3Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm3Code::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin3Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin3Code (lAdmCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm3 code: " << _location.getAdmin3Code());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm4Code::storeAdm4Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm4Code::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin4Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin4Code (lAdmCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm4 code: " << _location.getAdmin4Code());
    }

    // //////////////////////////////////////////////////////////////////
    storePopulation::storePopulation (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePopulation::operator() (unsigned int iPopulation,
                                      bsq::unused_type, bsq::unused_type) const {
      _location.setPopulation (iPopulation);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Population: " << _location.getPopulation());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeElevation::storeElevation (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeElevation::operator() (int iElevation,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setElevation (iElevation);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Elevation: " << _location.getElevation());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeGTopo30::storeGTopo30 (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeGTopo30::operator() (int iGTopo30,
                                   bsq::unused_type, bsq::unused_type) const {
      _location.setGTopo30 (iGTopo30);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("GTopo30: " << _location.getGTopo30());
    }

    // //////////////////////////////////////////////////////////////////
    storeTimeZone::storeTimeZone (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeTimeZone::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lTimeZoneStr (iChar.begin(), iChar.end());
      const TimeZone_T lTimeZone (lTimeZoneStr);
      _location.setTimeZone (lTimeZone);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Time-zone code: " << _location.getTimeZone());
    }

    // //////////////////////////////////////////////////////////////////
    storeGMTOffset::storeGMTOffset (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeGMTOffset::operator() (float iOffset,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setGMTOffset (iOffset);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("GMT offset: " << _location.getGMTOffset());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeDSTOffset::storeDSTOffset (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDSTOffset::operator() (float iOffset,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setDSTOffset (iOffset);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("DST offset: " << _location.getDSTOffset());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeRawOffset::storeRawOffset (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeRawOffset::operator() (float iOffset,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setRawOffset (iOffset);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Raw offset: " << _location.getRawOffset());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeModDate::storeModDate (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeModDate::operator() (bsq::unused_type,
                                   bsq::unused_type, bsq::unused_type) const {
      const OPENTREP::Date_T& lModDate = _location.calculateDate();
      _location.setModificationDate (lModDate);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Modification date: " << _location.getModificationDate());
    }

    // //////////////////////////////////////////////////////////////////
    storeCityCode::storeCityCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCityCode::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {

      const std::string lCityCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::CityCode_T lCityCode (lCityCodeStr);
      _location.setCityCode (lCityCode);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("City code: " << _location.getCityCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeCityUtfName::storeCityUtfName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCityUtfName::operator() (std::vector<uchar_t> iChar,
                                       bsq::unused_type,
                                       bsq::unused_type) const {

      const std::string lCityUtfNameStr (iChar.begin(), iChar.end());
      const OPENTREP::CityUTFName_T lCityUtfName (lCityUtfNameStr);
      _location.setCityUtfName (lCityUtfName);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("City UTF8 name: " << _location.getCityUtfName());
    }

    // //////////////////////////////////////////////////////////////////
    storeCityAsciiName::storeCityAsciiName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCityAsciiName::operator() (std::vector<uchar_t> iChar,
                                         bsq::unused_type,
                                         bsq::unused_type) const {

      const std::string lCityAsciiNameStr (iChar.begin(), iChar.end());
      const OPENTREP::CityASCIIName_T lCityAsciiName (lCityAsciiNameStr);
      _location.setCityAsciiName (lCityAsciiName);
       // DEBUG
       //OPENTREP_LOG_DEBUG("City ASCII name: " << _location.getCityAsciiName());
    }

    // //////////////////////////////////////////////////////////////////
    storeStateCode::storeStateCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeStateCode::operator() (std::vector<uchar_t> iChar,
                                     bsq::unused_type, bsq::unused_type) const {

      const std::string lStateCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::StateCode_T lStateCode (lStateCodeStr);
      _location.setStateCode (lStateCode);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("State code: " << _location.getStateCode());
    }

    // //////////////////////////////////////////////////////////////////
    storePORType::storePORType (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storePORType::operator() (std::vector<uchar_t> iChar,
                                   bsq::unused_type, bsq::unused_type) const {
      const std::string lIATATypeStr (iChar.begin(), iChar.end());
      const IATAType lIATAType (lIATATypeStr);
      _location.setIATAType (lIATAType);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("IATA type: " << _location.getIATAType());
    }

    // //////////////////////////////////////////////////////////////////
    storeWikiLink::storeWikiLink (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeWikiLink::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {

      const std::string lWikiLinkStr (iChar.begin(), iChar.end());
      const OPENTREP::WikiLink_T lWikiLink (lWikiLinkStr);
      _location.setWikiLink (lWikiLink);
       // DEBUG
       // OPENTREP_LOG_DEBUG ("Wiki link: " << _location.getWikiLink());
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCodeFull::storeAltLangCodeFull (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltLangCodeFull::operator() (std::vector<uchar_t> iChar,
                                           bsq::unused_type, bsq::unused_type) const {

      const std::string lAltLangCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::LanguageCode_T lAltLangCode (lAltLangCodeStr);
      _location._itLanguageCode = lAltLangCode;
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang full code: " << _location._itLanguageCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCode2Char::storeAltLangCode2Char (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltLangCode2Char::operator() (std::vector<uchar_t> iChar,
                                            bsq::unused_type, bsq::unused_type) const {

      const std::string lAltLangCodeStr (iChar.begin(), iChar.end());
      _location._itLangCode2Char = lAltLangCodeStr;
      _location._itLangCodeExt = "";
      _location._itLangCodeHist = "";
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang 2-char code: " << _location._itLangCode2Char);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCodeExt::storeAltLangCodeExt (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltLangCodeExt::operator() (std::vector<uchar_t> iChar,
                                          bsq::unused_type, bsq::unused_type) const {

      const std::string lAltLangCodeStr (iChar.begin(), iChar.end());
      std::ostringstream oStr;
      oStr << _location._itLangCode2Char << "-" << lAltLangCodeStr;
      _location._itLangCodeExt = oStr.str();
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang 2-char code: " << _location._itLangCodeExt);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCodeHist::storeAltLangCodeHist (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltLangCodeHist::operator() (std::vector<uchar_t> iChar,
                                           bsq::unused_type, bsq::unused_type) const {

      const std::string lAltLangCodeStr (iChar.begin(), iChar.end());
      std::ostringstream oStr;
      oStr << _location._itLangCode2Char << "_" << lAltLangCodeStr;
      _location._itLangCodeHist = oStr.str();
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang 2-char code: " << _location._itLangCodeHist);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltName::storeAltName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltName::operator() (std::vector<uchar_t> iChar,
                                   bsq::unused_type, bsq::unused_type) const {

      const std::string lAltNameStr (iChar.begin(), iChar.end());
      const OPENTREP::AltNameShortListString_T lAltName (lAltNameStr);
      //_location.addName (_location._itLanguageCodeNum, lAltName);
      _location.addName (_location._itLanguageCode, lAltName);
      // Reset the values
      //_location._itLanguageCodeNum = OPENTREP::Language::LAST_VALUE;
      _location._itLanguageCode = LanguageCode_T ("");
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt name: " << _location.getAltNameShortList());
    }

    
    // //////////////////////////////////////////////////////////////////
    doEndPor::doEndPor (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void doEndPor::operator() (bsq::unused_type,
                               bsq::unused_type, bsq::unused_type) const {
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Do End. Location structure: " << _location);
    }  

    // ///////////////////////////////////////////////////////////////////
    //
    //  Utility Parsers
    //
    // ///////////////////////////////////////////////////////////////////
    
    /** 1-digit-integer parser */
    OPENTREP::uint1_p_t uint1_p;
    
    /** 2-digit-integer parser */
    OPENTREP::uint2_p_t uint2_p;

    /** Up-to-2-digit-integer parser */
    OPENTREP::int1_2_p_t int1_2_p;

    /** 4-digit-integer parser */
    OPENTREP::uint4_p_t uint4_p;
    
    /** Up-to-5-digit-integer parser */
    OPENTREP::int1_5_p_t int1_5_p;
    OPENTREP::int0_5_p_t int0_5_p;

    /** Up-to-4-digit-integer parser */
    OPENTREP::uint1_4_p_t uint1_4_p;

    /** Up-to-9-digit-integer parser */
    OPENTREP::uint1_9_p_t uint1_9_p;
    OPENTREP::uint0_9_p_t uint0_9_p;

    /** Time element parsers. */
    OPENTREP::hour_p_t hour_p;
    OPENTREP::minute_p_t minute_p;
    OPENTREP::second_p_t second_p;

    /** Date element parsers. */
    OPENTREP::year_p_t year_p;
    OPENTREP::month_p_t month_p;
    OPENTREP::day_p_t day_p;

    /** Boolean parser */
    OPENTREP::boolean_p_t boolean_p;

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**

       -- Geonames-related part:
       -- ----------------------
       -- iata_code         : IATA code; varchar(3). See also:
       --                     http://www.iata.org/ps/publications/Pages/code-search.aspx
       -- icao_code         : ICAO code; varchar(4)
       -- faa_code          : FAA code; varchar(4)
       -- geoname_id        : Integer ID of record in geonames database
       -- name              : Name of geographical point;
       --                     (UTF8) varchar(200)
       -- asciiname         : Name of geographical point in plain ASCII
       --                     characters;
       --                     (ASCII) varchar(200)
       -- alternatenames    : Alternate names, comma separated
       --                     varchar(5000)
       -- latitude          : Latitude in decimal degrees (WGS84)
       -- longitude         : Longitude in decimal degrees (WGS84)
       -- feature class     : See http://www.geonames.org/export/codes.html
       --                     char(1)
       -- feature code      : See http://www.geonames.org/export/codes.html
       --                     varchar(10)
       -- country code      : ISO-3166 2-letter country code, 2 characters
       -- cc2               : Alternate country codes, comma separated,
       --                     ISO-3166 2-letter country code, 60 characters
       -- country name      : Name of the country, as found in the Geonames
       --                     countryInfo.txt data file
       --                     varchar(80)
       -- continent name    : The name of the related continent, as found in
       --                     the Geonames continentCodes.txt data file;
       --                     see also below the 'Continents' section
       --                     varchar(80)
       -- admin1 code       : FIPS code (subject to change to ISO code),
       --                     see exceptions below. See file admin1Codes.txt
       --                     for display names of this code;
       --                     varchar(20)
       -- admin1 UTF8 name  : UTF8 name for the first administrative division
       -- admin1 ASCII name : ASCII name for the first administrative division
       -- admin2 code       : Code for the second administrative division,
       --                     a county in the US. See file admin2Codes.txt;
       --                     varchar(80)
       -- admin2 UTF8 name  : UTF8 name for the second administrative division
       -- admin2 ASCII name : ASCII name for the second administrative division
       -- admin3 code       : Code for third level administrative division;
       --                     varchar(20)
       -- admin4 code       : Code for fourth level administrative division;
       --                     varchar(20)
       -- population        : bigint (8 byte int) 
       -- elevation         : In meters, integer
       -- dem               : Digital elevation model, srtm3 or gtopo30, average
       --                     elevation of 3''x3'' (ca 90mx90m) or 30''x30''
       --                     (ca 900mx900m) area in meters, integer.
       --                     srtm processed by cgiar/ciat.
       -- timezone          : The time-zone ID (see file timeZone.txt)
       -- gmt offset        : GMT offset on 1st of January
       -- dst offset        : DST offset to GMT on 1st of July (of the
       --                     current year)
       -- raw offset        : Raw Offset without DST
       -- modification date : Date of last modification in yyyy-MM-dd format
       --
       --
       -- ORI-related part:
       -- -----------------
       --
       -- is_geonames       : Whether that POR is known by Geonames; varchar(1)
       --                     varchar(1)
       -- city code         : The IATA code of the related city, when known;
       --                     varchar(3)
       -- city UTF8 name    : UTF8 name for the related city, when known;
       --                     varchar(200)
       -- city ASCII name   : ASCII name for the related city, when known;
       --                     varchar(200)
       -- state code        : The ISO code of the related state; varchar(3)
       -- location type     : A/APT airport; B/BUS bus/coach station;
       --                     C/CITY City;
       --                     G/GRD ground transport (this code is used for SK in
       --                     Sweden only); H/HELI Heliport;
       --                     O/OFF-PT off-line point, i.e. a city without
       --                     an airport;
       --                     R/RAIL railway Station;
       --                     S/ASSOC a location without its own IATA code,
       --                     but attached to an IATA location.
       --
       -- Continents:
       -- -----------
       -- AF Africa  6255146
       -- AS Asia    6255147
       -- EU Europe  6255148
       -- NA North America   6255149
       -- OC Oceania 6255151
       -- SA South America   6255150
       -- AN Antarctica      6255152
       --
       -- Samples:
       -- CDG^LFPG^^Y^6269554^^Paris - Charles-de-Gaulle^Paris - Charles-de-Gaulle^49.012779^2.55^S^AIRP^0.650357283214^^^^FR^^France^Europe^A8^Île-de-France^Ile-de-France^95^Département du Val-d'Oise^Departement du Val-d'Oise^^^0^119^106^Europe/Paris^1.0^2.0^1.0^2008-07-09^PAR^Paris^Paris^^^A^http://en.wikipedia.org/wiki/Paris-Charles_de_Gaulle_Airport^es|París - Charles de Gaulle|p=|Roissy Charles de Gaulle|
       -- PAR^ZZZZ^^Y^2988507^^Paris^Paris^48.85341^2.3488^P^PPLC^1.0^^^^FR^^France^Europe^A8^Île-de-France^Ile-de-France^75^Paris^Paris^751^75056^2138551^^42^Europe/Paris^1.0^2.0^1.0^2012-08-19^PAR^Paris^Paris^BVA,CDG,JDP,JPU,LBG,ORY,POX,TNF,VIY,XCR,XEX,XGB,XHP,XJY,XPG,XTT^^C^http://en.wikipedia.org/wiki/Paris^la|Lutetia Parisorum|=fr|Lutece|h=fr|Ville-Lumière|c=eo|Parizo|=es|París|ps=de|Paris|=en|Paris|p=af|Parys|=als|Paris|=an|París|=ar|باريس|=ast|París|=be|Горад Парыж|=bg|Париж|=ca|París|=cs|Paříž|=cy|Paris|=da|Paris|=el|Παρίσι|=et|Pariis|=eu|Paris|=fa|پاریس|=fi|Pariisi|=fr|Paris|p=ga|Páras|=gl|París|=he|פריז|=hr|Pariz|=hu|Párizs|=id|Paris|=io|Paris|=it|Parigi|=ja|パリ|=ka|პარიზი|=kn|ಪ್ಯಾರಿಸ್|=ko|파리|=ku|Parîs|=kw|Paris|=lb|Paräis|=li|Paries|=lt|Paryžius|=lv|Parīze|=mk|Париз|=ms|Paris|=na|Paris|=nds|Paris|=nl|Parijs|=nn|Paris|=no|Paris|=oc|París|=pl|Paryż|=pt|Paris|=ro|Paris|=ru|Париж|=scn|Pariggi|=sco|Paris|=sl|Pariz|=sq|Paris|=sr|Париз|=sv|Paris|=ta|பாரிஸ்|=th|ปารีส|=tl|Paris|=tr|Paris|=uk|Париж|=vi|Paris|p=zh|巴黎|=ia|Paris|=fy|Parys|=ln|Pari|=os|Париж|=pms|Paris|=sk|Paríž|=sq|Parisi|=sw|Paris|=tl|Lungsod ng Paris|=ug|پارىژ|=fr|Paname|c=fr|Pantruche|c=am|ፓሪስ|=arc|ܦܐܪܝܣ|=br|Pariz|=gd|Paris|=gv|Paarys|=hy|Փարիզ|=ksh|Paris|=lad|Paris|=lmo|Paris|=mg|Paris|=mr|पॅरिस|=tet|París|=tg|Париж|=ty|Paris|=ur|پیرس|=vls|Parys|=is|París|=vi|Pa-ri|=ml|പാരിസ്|=uz|Parij|=rue|Паріж|=ne|पेरिस|=jbo|paris|=mn|Парис|=lij|Pariggi|=vec|Parixe|=yo|Parisi|=yi|פאריז|=mrj|Париж|=hi|पैरिस|=fur|Parîs|=tt|Париж|=szl|Paryż|=mhr|Париж|=te|పారిస్|=tk|Pariž|=bn|প্যারিস|=ha|Pariis|=sah|Париж|=mzn|پاریس|=bo|ཕ་རི།|=haw|Palika|=mi|Parī|=ext|París|=ps|پاريس|=pa|ਪੈਰਿਸ|=ckb|پاریس|=cu|Парижь|=cv|Парис|=co|Parighji|=bs|Pariz|=so|Baariis|=sh|Pariz|=gu|પૅરિસ|=xmf|პარიზი|=ba|Париж|=pnb|پیرس|=arz|باريس|=la|Lutetia|=kk|Париж|=kv|Париж|=gn|Parĩ|=ky|Париж|=myv|Париж ош|=nap|Parigge|=km|ប៉ារីស|=krc|Париж|=udm|Париж|=wo|Pari|=gan|巴黎|=sc|Parigi|=za|Bahliz|=my|ပါရီမြို့|
       -- HDQ^ZZZZ^^N^0^^Headquarters ZZ^Headquarters ZZ^^^X^XXXX^^^^^ZZ^^Not relevant/available^Not relevant/available^^^^^^^^^^^^^^^^-1^HDQ^Headquarters ZZ^Headquarters ZZ^^^O^^
       --

       iata_code          varchar(3)
       icao_code          varchar(4)
       faa_code           varchar(4)
       is_geonames        varchar(1)
       geoname_id         int(11)
       valid_id           int(2)
       name               varchar(200)
       asciiname          varchar(200)
       latitude           decimal(10,7)
       longitude          decimal(10,7)
       fclass             varchar(1)
       fcode              varchar(10)
       page_rank          decimal(15,12)
       date_from          date
       date_until         date
       comments           varchar(4000)
       country_code       varchar(2)
       cc2                varchar(60)
       country_name       varchar(200)
       continent_name     varchar(80)
       adm1_code          varchar(20)
       adm1_name_utf      varchar(400)
       adm1_name_ascii    varchar(400)
       adm2_code          varchar(80)
       adm2_name_utf      varchar(400)
       adm2_name_ascii    varchar(400)
       adm3_code          varchar(20)
       adm4_code          varchar(20)
       population         int(11)
       elevation          int(11)
       gtopo30            int(11)
       timezone           varchar(40)
       gmt_offset         decimal(3,1)
       dst_offset         decimal(3,1)
       raw_offset         decimal(3,1)
       moddate            date
       city_code          varchar(3)
       city_name_utf      varchar(200)
       city_name_ascii    varchar(200)
       tvl_por_list       varchar(200)
       state_code         varchar(3)
       location_type      varchar(4)
       wiki_link          varchar(200)
       alt_name_section   text

       iata_code^icao_code^faa_code^is_geonames^geoname_id^valid_id^
       name^asciiname^
       latitude^longitude^fclass^fcode^
       page_rank^date_from^date_until^comment^
       country_code^cc2^country_name^continent_name^
       adm1_code^adm1_name_utf^adm1_name_ascii^
       adm2_code^adm2_name_utf^adm2_name_ascii^
       adm3_code^adm4_code^
       population^elevation^gtopo30^
       timezone^gmt_offset^dst_offset^raw_offset^moddate^
       city_code^city_name_utf^city_name_ascii^tvl_por_list^
       state_code^location_type^wiki_link^
       alt_name_section
    */ 

    /**
     * Grammar for the Por-Rule parser.
     */
    template <typename Iterator>	
    struct LocationParser : public bsq::grammar<Iterator, bsu::blank_type> {

      LocationParser (Location& ioPORRule) : 
        LocationParser::base_type(start), _location(ioPORRule) {

        start = bsq::eps
          >> *(header | por_rule);

        header = bsq::lit("iata_code") >> +(bsu::char_ - bsq::eoi - bsq::eol)
                                       >> (bsq::eoi | bsq::eol);

        por_rule = por_key
          >> '^' >> por_details
          >> '^' >> -alt_name_section
          >> por_rule_end[doEndPor(_location)];
        // >> +( '^' >> segment )

        por_rule_end = bsq::eps;

        por_key = iata_code
          >> '^' >> -icao_code
          >> '^' >> -faa_code
          >> '^' >> is_geonames
          >> '^' >> geoname_id
          >> '^' >> -validity_id
          ;

        por_details = common_name
          >> '^' >> ascii_name
          >> '^' >> -latitude
          >> '^' >> -longitude
          >> '^' >> feat_class
          >> '^' >> feat_code
          >> '^' >> -page_rank
          >> '^' >> -date_from
          >> '^' >> -date_until
          >> '^' >> -comments
          >> '^' >> country_code
          >> '^' >> -country_code2
          >> '^' >> country_name
          >> '^' >> -continent_name
          >> '^' >> -adm1_code
          >> '^' >> -adm1_name_utf
          >> '^' >> -adm1_name_ascii
          >> '^' >> -adm2_code
          >> '^' >> -adm2_name_utf
          >> '^' >> -adm2_name_ascii
          >> '^' >> -adm3_code
          >> '^' >> -adm4_code
          >> '^' >> -population
          >> '^' >> -elevation
          >> '^' >> -gtopo30
          >> '^' >> -time_zone
          >> '^' >> -gmt_offset
          >> '^' >> -dst_offset
          >> '^' >> -raw_offset
          >> '^' >> (mod_date | bsq::lit("-1"))
          >> '^' >> city_code
          >> '^' >> -city_name_utf
          >> '^' >> -city_name_ascii
          >> '^' >> -tvl_por_code_list[storeTvlPORListString(_location)]
          >> '^' >> -state_code
          >> '^' >> por_type
          >> '^' >> -wiki_link
          ;
        // >> '^' >> -alt_name_short_list[storeAltNameShortListString(_location)]

        iata_code =
          bsq::repeat(3)[bsu::char_('A', 'Z')][storeIataCode(_location)];

        icao_code =
          bsq::repeat(4)[bsu::char_("A-Z0-9")][storeIcaoCode(_location)];

        faa_code =
          bsq::repeat(1,4)[bsu::char_("A-Z0-9")][storeFaaCode(_location)];

        geoname_id = uint1_9_p[storeGeonamesID(_location)];

        validity_id = uint1_4_p[storeValidityID(_location)];

        is_geonames = boolean_p;

        common_name =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeCommonName(_location)]
          ;

        ascii_name =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAsciiName(_location)]
          ;

        alt_name_short_list = alt_name_short % ',';

        alt_name_short =
          (bsq::no_skip[+~bsu::char_("^,")]
           - (bsq::eoi|bsq::eol))[storeAltNameShort(_location)]
          ;

        tvl_por_code_list = tvl_por_code % ',';

        tvl_por_code =
          (bsq::no_skip[+~bsu::char_("^,")]
           - (bsq::eoi|bsq::eol))[storeTvlPORCode(_location)]
          ;

        latitude = bsq::double_[storeLatitude(_location)];

        longitude = bsq::double_[storeLongitude(_location)];

        feat_class =
          bsq::repeat(1)[bsu::char_("A-Z")][storeFeatureClass(_location)]
          ;

        feat_code =
          bsq::repeat(2,5)[bsu::char_("A-Z1-5")][storeFeatureCode(_location)]
          ;

        page_rank = bsq::double_[storePageRank(_location)];

        date_from = date[storeDateFrom(_location)];

        date_until = date[storeDateUntil(_location)];

        comments =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeComments(_location)]
          ;

        country_code =
          bsq::repeat(2,3)[bsu::char_("A-Z")][storeCountryCode(_location)]
          ;

        country_code2 =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAltCountryCode(_location)]
          ;

        country_name =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeCountryName(_location)]
          ;

        continent_name =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeContinentName(_location)]
          ;

        adm1_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm1Code(_location)]
          ;

        adm1_name_utf =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm1UtfName(_location)]
          ;

        adm1_name_ascii =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm1AsciiName(_location)]
          ;

        adm2_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm2Code(_location)]
          ;

        adm2_name_utf =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm2UtfName(_location)]
          ;

        adm2_name_ascii =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm2AsciiName(_location)]
          ;

        adm3_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm3Code(_location)]
          ;

        adm4_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm4Code(_location)]
          ;

        population = uint1_9_p[storePopulation(_location)];

        elevation = int1_5_p[storeElevation(_location)];

        gtopo30 = int1_5_p[storeGTopo30(_location)];

        time_zone =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeTimeZone(_location)]
          ;

        gmt_offset = bsq::float_[storeGMTOffset(_location)];

        dst_offset = bsq::float_[storeDSTOffset(_location)];

        raw_offset = bsq::float_[storeRawOffset(_location)];

        mod_date = date[storeModDate(_location)];

        date = bsq::lexeme
          [year_p[boost::phoenix::ref(_location._itYear) = bsq::labels::_1]
           >> '-'
           >> month_p[boost::phoenix::ref(_location._itMonth) = bsq::labels::_1]
           >> '-'
           >> day_p[boost::phoenix::ref(_location._itDay) = bsq::labels::_1] ];

        city_code =
          bsq::repeat(3)[bsu::char_('A', 'Z')][storeCityCode(_location)]
          ;

        city_name_utf =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeCityUtfName(_location)]
          ;

        city_name_ascii =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeCityAsciiName(_location)]
          ;

        state_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeStateCode(_location)]
          ;

        por_type =
          bsq::repeat(1,3)[bsu::char_("ABCGHOPRZ")][storePORType(_location)]
          ;
      
        wiki_link =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeWikiLink(_location)]
          ;

        alt_name_section = alt_name_details % '=';

        alt_name_details =
          -alt_lang_code
          >> '|' >> alt_name
          >> '|' >> -alt_name_qualifiers
          ;

        alt_lang_code =
          (+~bsu::char_("|=")
           - (bsq::eoi|bsq::eol))[storeAltLangCodeFull(_location)]
          ;

        alt_lang_code_ftd = lang_code_2char >> lang_code_opt;

        lang_code_opt = -(lang_code_ext | lang_code_hist);

        lang_code_2char =
          bsq::repeat(2,4)[bsu::char_("a-z")][storeAltLangCode2Char(_location)]
          ;

        lang_code_ext =
          '-' >> bsq::repeat(1,4)[bsu::char_('A', 'Z')][storeAltLangCodeExt(_location)];

        lang_code_hist =
          '_' >> bsq::repeat(1,4)[bsu::char_("a-z0-9")][storeAltLangCodeHist(_location)];

        alt_name =
          (bsq::no_skip[+~bsu::char_("|=")]
           - (bsq::eoi|bsq::eol))[storeAltName(_location)]
          ;

        alt_name_qualifiers =
          bsq::repeat(1,4)[bsu::char_("shpc")]
          ;

        //BOOST_SPIRIT_DEBUG_NODE (LocationParser);
        BOOST_SPIRIT_DEBUG_NODE (start);
        BOOST_SPIRIT_DEBUG_NODE (header);
        BOOST_SPIRIT_DEBUG_NODE (por_rule);
        BOOST_SPIRIT_DEBUG_NODE (por_rule_end);
        BOOST_SPIRIT_DEBUG_NODE (por_key);
        BOOST_SPIRIT_DEBUG_NODE (por_details);
        BOOST_SPIRIT_DEBUG_NODE (iata_code);
        BOOST_SPIRIT_DEBUG_NODE (icao_code);
        BOOST_SPIRIT_DEBUG_NODE (faa_code);
        BOOST_SPIRIT_DEBUG_NODE (geoname_id);
        BOOST_SPIRIT_DEBUG_NODE (validity_id);
        BOOST_SPIRIT_DEBUG_NODE (is_geonames);
        BOOST_SPIRIT_DEBUG_NODE (common_name);
        BOOST_SPIRIT_DEBUG_NODE (ascii_name);      
        BOOST_SPIRIT_DEBUG_NODE (alt_name_short_list);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_short);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_sep);
        BOOST_SPIRIT_DEBUG_NODE (tvl_por_code_list);
        BOOST_SPIRIT_DEBUG_NODE (tvl_por_code);
        BOOST_SPIRIT_DEBUG_NODE (tvl_por_sep);
        BOOST_SPIRIT_DEBUG_NODE (latitude);
        BOOST_SPIRIT_DEBUG_NODE (longitude);
        BOOST_SPIRIT_DEBUG_NODE (feat_class);
        BOOST_SPIRIT_DEBUG_NODE (feat_code);
        BOOST_SPIRIT_DEBUG_NODE (page_rank);
        BOOST_SPIRIT_DEBUG_NODE (date_from);
        BOOST_SPIRIT_DEBUG_NODE (date_until);
        BOOST_SPIRIT_DEBUG_NODE (comments);
        BOOST_SPIRIT_DEBUG_NODE (country_code);
        BOOST_SPIRIT_DEBUG_NODE (country_code2);
        BOOST_SPIRIT_DEBUG_NODE (country_name);
        BOOST_SPIRIT_DEBUG_NODE (continent_name);
        BOOST_SPIRIT_DEBUG_NODE (adm1_code);
        BOOST_SPIRIT_DEBUG_NODE (adm1_name_utf);
        BOOST_SPIRIT_DEBUG_NODE (adm1_name_ascii);
        BOOST_SPIRIT_DEBUG_NODE (adm2_code);
        BOOST_SPIRIT_DEBUG_NODE (adm2_name_utf);
        BOOST_SPIRIT_DEBUG_NODE (adm2_name_ascii);
        BOOST_SPIRIT_DEBUG_NODE (adm3_code);
        BOOST_SPIRIT_DEBUG_NODE (adm4_code);
        BOOST_SPIRIT_DEBUG_NODE (population);
        BOOST_SPIRIT_DEBUG_NODE (elevation);
        BOOST_SPIRIT_DEBUG_NODE (gtopo30);
        BOOST_SPIRIT_DEBUG_NODE (time_zone);
        BOOST_SPIRIT_DEBUG_NODE (gmt_offset);
        BOOST_SPIRIT_DEBUG_NODE (raw_offset);
        BOOST_SPIRIT_DEBUG_NODE (dst_offset);
        BOOST_SPIRIT_DEBUG_NODE (mod_date);
        BOOST_SPIRIT_DEBUG_NODE (date);
        BOOST_SPIRIT_DEBUG_NODE (city_code);
        BOOST_SPIRIT_DEBUG_NODE (city_name_utf);
        BOOST_SPIRIT_DEBUG_NODE (city_name_ascii);
        BOOST_SPIRIT_DEBUG_NODE (state_code);
        BOOST_SPIRIT_DEBUG_NODE (por_type);
        BOOST_SPIRIT_DEBUG_NODE (wiki_link);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_section);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_details);
        BOOST_SPIRIT_DEBUG_NODE (alt_lang_code);
        BOOST_SPIRIT_DEBUG_NODE (alt_lang_code_ftd);
        BOOST_SPIRIT_DEBUG_NODE (alt_name);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_qualifiers);
        BOOST_SPIRIT_DEBUG_NODE (lang_code_opt);
        BOOST_SPIRIT_DEBUG_NODE (lang_code_2char);
        BOOST_SPIRIT_DEBUG_NODE (lang_code_ext);
        BOOST_SPIRIT_DEBUG_NODE (lang_code_hist);
      }

      // Instantiation of rules
      bsq::rule<Iterator, bsu::blank_type>
      start, header, por_rule, por_rule_end, por_key, por_details,
        iata_code, icao_code, faa_code, geoname_id, validity_id, is_geonames,
        common_name, ascii_name,
        alt_name_short_list, alt_name_short, alt_name_sep,
        tvl_por_code_list, tvl_por_code, tvl_por_sep,
        latitude, longitude, feat_class, feat_code,
        page_rank, date_from, date_until, comments,
        country_code, country_code2, country_name, continent_name,
        adm1_code, adm1_name_utf, adm1_name_ascii,
        adm2_code, adm2_name_utf, adm2_name_ascii, adm3_code, adm4_code,
        population, elevation, gtopo30,
        time_zone, gmt_offset, dst_offset, raw_offset,
        mod_date, date,
        city_code, city_name_utf, city_name_ascii, state_code,
        por_type, wiki_link,
        alt_name_section, alt_name_details,
        alt_lang_code, alt_lang_code_ftd, alt_name, alt_name_qualifiers,
        lang_code_opt, lang_code_2char, lang_code_ext, lang_code_hist,
        destination;
      
      // Parser Context
      Location& _location;
    };    
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the string parser
  //
  /////////////////////////////////////////////////////////////////////////
    
  // //////////////////////////////////////////////////////////////////////
  PORStringParser::PORStringParser (const std::string& iString)
    : _string (iString) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void PORStringParser::init() {
    // Store the raw data string
    _location.setRawDataString (_string);
  }
    
  // //////////////////////////////////////////////////////////////////////
  PORStringParser::~PORStringParser() {
  }

  // //////////////////////////////////////////////////////////////////////
  const Location& PORStringParser::generateLocation() {
    // DEBUG
    // OPENTREP_LOG_DEBUG ("Parsing POR string: '" << _string << "'");

    // String to be parsed
    std::istringstream stringToBeParsed (_string);
    
    // Create an input iterator
    OPENTREP::base_iterator_t iStr (stringToBeParsed);

    // Convert input iterator to an iterator usable by spirit parser  
    OPENTREP::iterator_t fwd_start(boost::spirit::make_default_multi_pass(iStr));
    OPENTREP::iterator_t fwd_end;

    // Initialise the positional iterators
    OPENTREP::pos_iterator_t pos_start (fwd_start, fwd_end, _string);
    OPENTREP::pos_iterator_t pos_end;

    // Initialise the parser (grammar) with the helper/staging structure.
    PorParserHelper::LocationParser<OPENTREP::iterator_t> lPORParser (_location);
      
    // Launch the parsing of the file and, thanks to the doEndPor
    // call-back structure, the building of the whole BomRoot BOM
    bool hasParsingBeenSuccesful = false;
    try {

      hasParsingBeenSuccesful = bsq::phrase_parse (fwd_start, fwd_end,
                                                   lPORParser, bsu::blank);

    } catch (const bsq::expectation_failure<pos_iterator_t>& e) {
      const bsc::file_position_base<std::string>& pos = e.first.get_position();
      std::ostringstream oStr;
      oStr << "Parse error on POR string '" << _string
           << "', position " << pos.column << std::endl
           << "'" << e.first.get_currentline() << "'" << std::endl
           << std::setw(pos.column) << " " << "^- here";
      OPENTREP_LOG_ERROR (oStr.str());
      throw PorFileParsingException (oStr.str());
    }
      
    if (hasParsingBeenSuccesful == false) {
      OPENTREP_LOG_ERROR ("Parsing of POR input string: '" << _string
                          << "' failed");
      throw PorFileParsingException ("Parsing of POR input string: '"
                                     + _string + "' failed");
    }
    
    if  (fwd_start != fwd_end) {
      OPENTREP_LOG_ERROR ("Parsing of POR input string: '" << _string
                          << "' failed");
      throw PorFileParsingException ("Parsing of POR input file: '"
                                     + _string + "' failed");
    }
    
    //
    if (hasParsingBeenSuccesful == true && fwd_start == fwd_end) {
      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("Parsing of POR input string: '" << _string
                          << "' succeeded");
      */
    }

    return _location;
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////
    
  // //////////////////////////////////////////////////////////////////////
  PORFileParser::PORFileParser (const PORFilePath_T& iFilename)
    : _filename (iFilename) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void PORFileParser::init() {
  }
    
  // //////////////////////////////////////////////////////////////////////
  void PORFileParser::generateLocations() {
      
    OPENTREP_LOG_DEBUG ("Parsing por input file: " << _filename);

    // File to be parsed
    const char* lFilenameStr = _filename.c_str();
    std::ifstream fileToBeParsed (lFilenameStr, std::ios_base::in);

    // Check if the filename exist and can be open
    if (fileToBeParsed.is_open() == false) {
      OPENTREP_LOG_ERROR ("The por file " << _filename << " can not be open."
                          << std::endl);

      throw FileNotFoundException ("The file " + _filename
                                   + " does not exist or can not be read");
    }
    
    // Create an input iterator
    OPENTREP::base_iterator_t iStr (fileToBeParsed);

    // Convert input iterator to an iterator usable by spirit parser  
    OPENTREP::iterator_t start (boost::spirit::make_default_multi_pass (iStr));
    OPENTREP::iterator_t end;

    // Initialise the parser (grammar) with the helper/staging structure.
    PorParserHelper::LocationParser<OPENTREP::iterator_t> lPORParser (_location);
      
    // Launch the parsing of the file and, thanks to the doEndPor
    // call-back structure, the building of the whole BomRoot BOM
    const bool hasParsingBeenSuccesful = 
       bsq::phrase_parse (start, end, lPORParser, bsu::blank);
      
    if (hasParsingBeenSuccesful == false) {
      OPENTREP_LOG_ERROR ("Parsing of por input file: " << _filename
                          << " failed");
      throw PorFileParsingException ("Parsing of por input file: "
                                     + _filename + " failed");
    }
    
    if  (start != end) {
      OPENTREP_LOG_ERROR ("Parsing of por input file: " << _filename
                          << " failed");
      throw PorFileParsingException ("Parsing of por input file: "
                                     + _filename + " failed");
    }
    
    if (hasParsingBeenSuccesful == true && start == end) {
      OPENTREP_LOG_DEBUG ("Parsing of por input file: " << _filename
                          << " succeeded");
    } 
  }

}
