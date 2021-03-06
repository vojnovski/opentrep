#ifndef __OPENTREP_BOM_PLACE_HPP
#define __OPENTREP_BOM_PLACE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
#include <set>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/NameMatrix.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/PlaceList.hpp>

namespace OPENTREP {

  // Forward declarations
  class World;
  class PlaceHolder;
  struct OTransliterator;
  
  /**
   * @brief Class modelling a place/POR (point of reference).
   */
  class Place : public BomAbstract {
    friend class FacWorld;
    friend class FacPlace;
    friend class FacPlaceHolder;
    friend class DbaPlace;
  public:
    // /////////// Type definitions //////////
    /**
     * (STL) Set of strings, to be added in the Xapian database (for
     * indexing, spelling, stemming, synonyms, etc).
     */
    typedef std::set<std::string> StringSet_T;


  public:
    // //////////////// Getters ///////////////
    /**
     * Get the underlying Location structure, which is an extract of the
     * Place object.
     *
     * @return const Location& The underlying Location structure.
     */
    const Location& getLocation() const {
      return _location;
    }

    /**
     * Get the primary key (IATA and ICAO codes, Geonames ID) of the place.
     */
    const LocationKey& getKey() const {
      return _location.getKey();
    }
    
    /**
     * Get the IATA code.
     */
    const IATACode_T& getIataCode() const {
      return _location.getIataCode();
    }
    
    /**
     * Get the ICAO code.
     */
    const ICAOCode_T& getIcaoCode() const {
      return _location.getIcaoCode();
    }
    
    /**
     * Get the Geonames ID.
     */
    const GeonamesID_T& getGeonamesID() const {
      return _location.getGeonamesID();
    }

    /**
     * State whether that POR is referenced by Geonames.
     */
    const IsGeonames_T& isGeonames() const {
      return _location.isGeonames();
    }

    /** 
     * Get the common name (usually in American English, but not necessarily
     * in ASCII).
     */
    const CommonName_T& getCommonName() const {
      return _location.getCommonName();
    }
    
    /** 
     * Get the ASCII name (not necessarily in English).
     */
    const ASCIIName_T& getAsciiName() const {
      return _location.getAsciiName();
    }
    
    /** 
     * Get the short list of alternate names (without language codes).
     */
    const AltNameShortListString_T& getAltNameShortListString() const {
      return _location.getAltNameShortListString();
    }

    /**
     * Get the FAA code.
     */
    const FAACode_T& getFaaCode() const {
      return _location.getFaaCode();
    }
    
    /**
     * Get the beginning date of the validity period
     */
    const Date_T& getDateFrom() const {
      return _location.getDateFrom();
    }

    /**
     * Get the end date of the validity period
     */
    const Date_T& getDateEnd() const {
      return _location.getDateEnd();
    }

    /**
     * Get the commentaries.
     */
    const Comment_T& getComment() const {
      return _location.getComment();
    }

    /**
     * Get the related/served IATA city code.
     */
    const CityCode_T& getCityCode() const {
      return _location.getCityCode();
    }

    /**
     * Get the city name in UTF8.
     */
    const CityUTFName_T& getCityUtfName() const {
      return _location.getCityUtfName();
    }
    
    /**
     * Get the city name in ASCII (not necessarily in English).
     */
    const CityASCIIName_T& getCityAsciiName() const {
      return _location.getCityAsciiName();
    }
    
    /**
     * Get the state code.
     */
    const StateCode_T& getStateCode() const {
      return _location.getStateCode();
    }
    
    /**
     * Get the country code.
     */
    const CountryCode_T& getCountryCode() const {
      return _location.getCountryCode();
    }
    
    /**
     * Get the alternative country code. 
     */
    const AltCountryCode_T& getAltCountryCode() const {
      return _location.getAltCountryCode();
    }
    
    /**
     * Get the country name.
     */
    const CountryName_T& getCountryName() const {
      return _location.getCountryName();
    }
    
    /**
     * Get the continent code.
     */
    const ContinentCode_T& getContinentCode() const {
      return _location.getContinentCode();
    }
    
    /**
     * Get the continent name.
     */
    const ContinentName_T& getContinentName() const {
      return _location.getContinentName();
    }
    
    /**
     * Get the time-zone.
     */
    const TimeZone_T& getTimeZone() const {
      return _location.getTimeZone();
    }
    
    /**
     * Get the GMT offset (e.g., 1)
     */
    const GMTOffset_T& getGMTOffset() const {
      return _location.getGMTOffset();
    }

    /**
     * Get the DST offset (e.g., 2)
     */
    const DSTOffset_T& getDSTOffset() const {
      return _location.getDSTOffset();
    }

    /**
     * Get the raw offset (e.g., -1)
     */
    const RawOffset_T& getRawOffset() const {
      return _location.getRawOffset();
    }

    /**
     * Get the geographical latitude.
     */
    const Latitude_T& getLatitude() const {
      return _location.getLatitude();
    }

    /**
     * Get the geographical longitude.
     */
    const Longitude_T& getLongitude() const {
      return _location.getLongitude();
    }
    
    /**
     * Get the feature class (e.g., S for spot).
     */
    const FeatureClass_T& getFeatureClass() const {
      return _location.getFeatureClass();
    }

    /**
     * Get the feature code (e.g., AIRP for airport).
     */
    const FeatureCode_T& getFeatureCode() const {
      return _location.getFeatureCode();
    }

    /**
     * Get the IATA location type (e.g., A for airport).
     */
    const IATAType& getIATAType() const {
      return _location.getIATAType();
    }

    /**
     * Get the administrative code of rank 1 (e.g., IL for Illinois).
     */
    const Admin1Code_T& getAdmin1Code() const {
      return _location.getAdmin1Code();
    }

    /**
     * Get the administrative UTF8 name of rank 1 (e.g., Illinois).
     */
    const Admin1UTFName_T& getAdmin1UtfName() const {
      return _location.getAdmin1UtfName();
    }

    /**
     * Get the administrative ASCII name of rank 1 (e.g., Illinois).
     */
    const Admin1ASCIIName_T& getAdmin1AsciiName() const {
      return _location.getAdmin1AsciiName();
    }

    /**
     * Get the administrative code of rank 2.
     */
    const Admin2Code_T& getAdmin2Code() const {
      return _location.getAdmin2Code();
    }

    /**
     * Get the administrative UTF8 name of rank 2 (e.g., Alpes-Maritimes).
     */
    const Admin2UTFName_T& getAdmin2UtfName() const {
      return _location.getAdmin2UtfName();
    }

    /**
     * Get the administrative ASCII name of rank 2 (e.g., Alpes-Maritimes).
     */
    const Admin2ASCIIName_T& getAdmin2AsciiName() const {
      return _location.getAdmin2AsciiName();
    }

    /**
     * Get the administrative code of rank 3.
     */
    const Admin3Code_T& getAdmin3Code() const {
      return _location.getAdmin3Code();
    }

    /**
     * Get the administrative code of rank 4.
     */
    const Admin4Code_T& getAdmin4Code() const {
      return _location.getAdmin4Code();
    }

    /**
     * Get the population (number of inhabitants).
     */
    const Population_T& getPopulation() const {
      return _location.getPopulation();
    }

    /**
     * Get the elevation.
     */
    const Elevation_T& getElevation() const {
      return _location.getElevation();
    }

    /**
     * Get the GTopo30.
     */
    const GTopo30_T& getGTopo30() const {
      return _location.getGTopo30();
    }

    /**
     * Get the PageRank/importance. 
     */
    const PageRank_T& getPageRank() const {
      return _location.getPageRank();
    }
    
    /**
     * Get the modification date. 
     */
    const Date_T& getModificationDate() const {
      return _location.getModificationDate();
    }
    
    /**
     * Get the Wikipedia link.
     */
    const WikiLink_T& getWikiLink() const {
      return _location.getWikiLink();
    }
    
    /**
     * Get the map of name lists.
     */
    const NameMatrix& getNameMatrix() const {
      return _location.getNameMatrix();
    }

    /**
     * Get, for a given language (code), the corresponding list of names.
     *
     * @param const LanguageCode_T& Language code.
     * @param NameList_T& Empty list of names, which will be filled by the
     *        method if a list exists for that language code.
     * @return bool Whether or not such a list exists for the given
     *         language.
     */
    bool getNameList (const LanguageCode_T& iLanguageCode,
                      NameList_T& ioNameList) const {
      return _location.getNameList (iLanguageCode, ioNameList);
    }

    /**
     * Get the original keywords.
     */
    const std::string& getOriginalKeywords() const {
      return _location.getOriginalKeywords();
    }
    
    /**
     * Get the corrected keywords.
     */
    const std::string& getCorrectedKeywords() const {
      return _location.getCorrectedKeywords();
    }
    
    /**
     * Get the matching percentage.
     */
    const MatchingPercentage_T& getPercentage() const {
      return _location.getPercentage();
    }

    /**
     * Get the allowed edit distance/error.
     */
    const NbOfErrors_T& getEditDistance() const {
      return _location.getEditDistance();
    }

    /**
     * Get the maximal allowable edit distance/error, with which the
     * matching has been made.
     */
    const NbOfErrors_T& getAllowableEditDistance() const {
      return _location.getAllowableEditDistance();
    }

    /**
     * Get the raw data string, as stored and retrieved by Xapian.
     */
    const RawDataString_T& getRawDataString() const {
      return _location.getRawDataString();
    }

    /**
     * Get the Xapian document ID.
     */
    const XapianDocID_T& getDocID() const {
      return _docID;
    }

    /**
     * Get the list of extra matching (similar) places.
     */
    const PlaceOrderedList_T& getExtraPlaceList() const {
      return _extraPlaceList;
    }

    /**
     * Get the list of alternate matching (less similar) places.
     */
    const PlaceOrderedList_T& getAlternatePlaceList() const {
      return _alternatePlaceList;
    }

    /**
     * Get the (STL) set of terms (for the Xapian index).
     */
    const StringSet_T& getTermSet() const {
      return _termSet;
    }

    /**
     * Get the (STL) set of spelling terms (for the Xapian spelling dictionary).
     */
    const StringSet_T& getSpellingSet() const {
      return _spellingSet;
    }

    /**
     * Get the (STL) set of stemming terms (for the Xapian stemming dictionary).
     */
    const StringSet_T& getStemmingSet() const {
      return _stemmingSet;
    }

    /**
     * Get the (STL) set of synonym terms (for the Xapian synonym dictionary).
     */
    const StringSet_T& getSynonymSet() const {
      return _synonymSet;
    }


  public:
    // ////////////////// Setters /////////////////
    /**
     * Set the full Location structure.
     */
    void setLocation (const Location& iLocation) {
      _location = iLocation;
    }
    
    /**
     * Set the primary key (IATA and ICAO codes, Geonames ID) of the place.
     */
    void setKey (const LocationKey& iKey) {
      _location.setKey (iKey);
    }
    
    /**
     * Set the IATA code.
     */
    void setIataCode (const std::string& iIataCode) {
      _location.setIataCode (iIataCode);
    }
    
    /**
     * Set the ICAO code.
     */
    void setIcaoCode (const std::string& iIcaoCode) {
      _location.setIcaoCode (iIcaoCode);
    }
    
    /**
     * Set the Geonames ID.
     */
    void setGeonamesID (const GeonamesID_T& iGeonamesID) {
      _location.setGeonamesID (iGeonamesID);
    }

    /** 
     * Set the common name (usually in American English, but not necessarily
     * in ASCII).
     */
    void setCommonName (const std::string& iName) {
      _location.setCommonName (iName);
    }
    
    /** 
     * Set the ASCII name (not necessarily in English).
     */
    void setAsciiName (const std::string& iName) {
      _location.setAsciiName (iName);
    }
    
    /** 
     * Set the short list of alternate names (without language codes).
     */
    void setAltNameShortListString (const std::string& iNameListString) {
      _location.setAltNameShortListString (iNameListString);
    }

    /**
     * Set the FAA code.
     */
    void setFaaCode (const std::string& iFaaCode) {
      _location.setFaaCode (iFaaCode);
    }
    
    /**
     * Set the beginning date of the validity period
     */
    void setDateFrom (const Date_T& iDate) {
      _location.setDateFrom (iDate);
    }

    /**
     * Set the end date of the validity period
     */
    void setDateEnd (const Date_T& iDate) {
      _location.setDateEnd (iDate);
    }

    /**
     * Set the commentaries.
     */
    void setComment (const std::string& iComment) {
      _location.setComment (iComment);
    }

    /**
     * Set the related/served IATA city code.
     */
    void setCityCode (const std::string& iCityCode) {
      _location.setCityCode (iCityCode);
    }
    
    /**
     * Set the city name in UTF8.
     */
    void setCityUtfName (const std::string& iCityUtfName) {
      _location.setCityUtfName (iCityUtfName);
    }
    
    /**
     * Set the city name in ASCII (not necessarily in English).
     */
    void setCityAsciiName (const std::string& iCityAsciiName) {
      _location.setCityAsciiName (iCityAsciiName);
    }
    
    /**
     * Set the state code.
     */
    void setStateCode (const std::string& iStateCode) {
      _location.setStateCode (iStateCode);
    }
    
    /**
     * Set the country code.
     */
    void setCountryCode (const std::string& iCountryCode) {
      _location.setCountryCode (iCountryCode);
    }
    
    /**
     * Set the alternative country code. 
     */
    void setAltCountryCode (const std::string& iCountryCode) {
      _location.setAltCountryCode (iCountryCode);
    }
    
    /**
     * Set the continent code.
     */
    void setContinentCode (const std::string& iContinentCode) {
      _location.setContinentCode (iContinentCode);
    }
    
    /**
     * Set the continent name.
     */
    void setContinentName (const std::string& iContinentName) {
      _location.setContinentName (iContinentName);
    }
    
    /**
     * Set the time-zone.
     */
    void setTimeZone (const std::string& iTimeZone) {
      _location.setTimeZone (iTimeZone);
    }
    
    /**
     * Set the GMT offset (e.g., 1)
     */
    void setGMTOffset (const GMTOffset_T& iOffset) {
      _location.setGMTOffset (iOffset);
    }

    /**
     * Set the DST offset (e.g., 2)
     */
    void setDSTOffset (const DSTOffset_T& iOffset) {
      _location.setDSTOffset (iOffset);
    }

    /**
     * Set the raw offset (e.g., 1)
     */
    void setRawOffset (const RawOffset_T& iOffset) {
      _location.setRawOffset (iOffset);
    }

    /**
     * Set the geographical latitude.
     */
    void setLatitude (const Latitude_T& iLatitude) {
      _location.setLatitude (iLatitude);
    }

    /**
     * Set the geographical longitude.
     */
    void setLongitude (const Longitude_T& iLongitude) {
      _location.setLongitude (iLongitude);
    }
    
    /**
     * Set the feature class (e.g., S for spot).
     */
    void setFeatureClass (const std::string& iFeatClass) {
      _location.setFeatureClass (iFeatClass);
    }

    /**
     * Set the feature code (e.g., AIRP for airport).
     */
    void setFeatureCode (const std::string& iFeatCode) {
      _location.setFeatureCode (iFeatCode);
    }

    /**
     * Set the IATA location type (e.g., A for airport).
     */
    void setIATAType (const std::string& iIATAType) {
      _location.setIATAType (iIATAType);
    }

    /**
     * Set the administrative code of rank 1 (e.g., IL for Illinois).
     */
    void setAdmin1Code (const std::string& iAdminCode) {
      _location.setAdmin1Code (iAdminCode);
    }

    /**
     * Set the administrative code of rank 2.
     */
    void setAdmin2Code (const std::string& iAdminCode) {
      _location.setAdmin2Code (iAdminCode);
    }

    /**
     * Set the administrative code of rank 3.
     */
    void setAdmin3Code (const std::string& iAdminCode) {
      _location.setAdmin3Code (iAdminCode);
    }

    /**
     * Set the administrative code of rank 4.
     */
    void setAdmin4Code (const std::string& iAdminCode) {
      _location.setAdmin4Code (iAdminCode);
    }

    /**
     * Set the population (number of inhabitants).
     */
    void setPopulation (const Population_T& iPopulation) {
      _location.setPopulation (iPopulation);
    }

    /**
     * Set the elevation.
     */
    void setElevation (const Elevation_T& iElevation) {
      _location.setElevation (iElevation);
    }

    /**
     * Set the GTopo30.
     */
    void setGTopo30 (const GTopo30_T& iGTopo30) {
      _location.setGTopo30 (iGTopo30);
    }

    /**
     * Set the PageRank.
     */
    void setPageRank (const PageRank_T& iPageRank) {
      _location.setPageRank (iPageRank);
    }
    
    /**
     * Set the modification date. 
     */
    void setModificationDate (const Date_T& iModDate) {
      _location.setModificationDate (iModDate);
    }

    /**
     * Set the Wikipedia link.
     */
    void setWikiLink (const std::string& iWikiLink) {
      _location.setWikiLink (iWikiLink);
    }
    
    /**
     * Set the original keywords.
     */
    void setOriginalKeywords (const std::string& iOriginalKeywords) {
      _location.setOriginalKeywords (iOriginalKeywords);
    }
    
    /**
     * Set the corrected keywords.
     */
    void setCorrectedKeywords (const std::string& iCorrectedKeywords) {
      _location.setCorrectedKeywords (iCorrectedKeywords);
    }
    
    /**
     * Set the Xapian matching percentage.
     */
    void setPercentage (const MatchingPercentage_T& iPercentage) {
      _location.setPercentage (iPercentage);
    }

    /**
     * Set the allowed edit distance/error.
     */
    void setEditDistance (const NbOfErrors_T& iEditDistance) {
      _location.setEditDistance (iEditDistance);
    }

    /**
     * Set the maxiaml allowable edit distance/error, with which the
     * matching has been made.
     */
    void setAllowableEditDistance (const NbOfErrors_T& iAllowableEditDistance) {
      _location.setAllowableEditDistance (iAllowableEditDistance);
    }

    /**
     * Set the raw data string, as stored and retrieved by Xapian.
     */
    void setRawDataString (const std::string& iRawDataString) {
      _location.setRawDataString (iRawDataString);
    }

    /**
     * Set the Xapian document ID.
     */
    void setDocID (const XapianDocID_T& iDocID) {
      _docID = iDocID;
    }

    
  public:
    // ////////// Setters in underlying names ////////
    /**
     * Add a name for the place.
     *
     * @param const LanguageCode_T& Language in which to add the name.
     * @param const std::string& Name to be added.
     */
    void addName (const LanguageCode_T& iLanguageCode, const std::string& iName) {
      _location.addName (iLanguageCode, iName);
    }

    /**
     * Reset the map of name lists.
     */
    void resetMatrix() {
      _location.resetMatrix();
    }

    /**
     * Reset the index/spelling (STL) sets.
     */
    void resetIndexSets();

    
  public:
    // /////////// Business methods /////////
    /**
     * Get the underlying Location structure, which is an extract of the
     * Place object. That (Location) structure is passed back to the caller
     * of the service.
     *
     * @return Location The Location structure just created and filled
     *                  with the parameters of the Place object.
     */
    const Location& completeLocation();

    /**
     * Add the given name to the Xapian index and spelling dictionary.
     * Tokenise and re-assemble the given name, so as to replace all the
     * punctuations and other separators by mere spaces.
     * For instance, "Paris/FR/Gare" is transformed into "Paris FR Gare".
     *
     * @param const LocationName_T& Name of the POR (point of reference)
     * @param const FeatureCode_T& Geonames feature code
     * @param const CityUTFName_T& UTF8 name of the served city
     * @param const CityASCIIName_T& ASCII name of the served city
     * @param const Admin1UTFName_T& UTF8 Name of the administrative level 1
     *                               of the POR
     * @param const Admin1ASCIIName_T& ASCII Name of the administrative level 1
     *                                 of the POR
     * @param const Admin2UTFName_T& UTF8 Name of the administrative level 2
     *                               of the POR
     * @param const Admin2ASCIIName_T& ASCII Name of the administrative level 2
     *                                 of the POR
     * @param const StateCode_T& Code of the state of the POR
     * @param const CountryCode_T& ISO code of the country of the POR
     * @param const CountryName_T& Name of the country of the POR
     * @param const ContinentName_T& Name of the continent of the POR
     * @param const OTransliterator& Unicode transliterator
     */
    void addNameToXapianSets (const LocationName_T&, const FeatureCode_T&,
                              const CityUTFName_T&, const CityASCIIName_T&,
                              const Admin1UTFName_T&, const Admin1ASCIIName_T&,
                              const Admin2UTFName_T&, const Admin2ASCIIName_T&,
                              const StateCode_T&,
                              const CountryCode_T&, const CountryName_T&,
                              const ContinentName_T&, const OTransliterator&);

    /**
     * Build the (STL) sets of (Xapian-related) terms, spelling,
     * synonyms, etc.
     *
     * @param const OTransliterator& Unicode transliterator
     */
    void buildIndexSets (const OTransliterator&);

    /**
     * Add the given name to the Xapian index. Derive a list of feature names
     * from the feature code. See the Location::getFeatureList() method
     * for more information.
     *
     * @param const std::string& Name of the POR (point of reference)
     * @param const FeatureCode_T& Geonames feature code
     */
    void addNameToXapianSets(const std::string& iBaseName, const FeatureCode_T&);


  public:
    // ///////// Display methods ////////
    /**
     * Dump a Business Object into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream&) const;

    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream&);

    /**
     * Get a string describing the whole key (IATA and ICAO codes, Geonames ID).
     */
    std::string describeKey() const {
      return _location.describeKey();
    }

    /**
     * Get a string describing the whole key (IATA and ICAO codes, Geonames ID).
     */
    std::string describeShortKey() const {
      return _location.describeShortKey();
    }
    
    /**
     * Get the serialised version of the Place object.
     */
    std::string toString() const;

    /**
     * Get a short display of the Business Object.
     */
    std::string toShortString() const;
    
    /**
     * Get a string describing the (STL) sets of terms for the Xapian database.
     */
    std::string describeSets() const;

    /**
     * Display the full Place context.
     */
    std::string display() const;

    /**
     * Display a short Place context.
     */
    std::string shortDisplay() const;

    
  private:
    /**
     * Main constructor from a primary key.
     */
    Place (const LocationKey&);

    /**
     * Main constructor from a full Location structure.
     */
    Place (const Location&);

    /**
     * Default constructor.
     */
    Place();

    /**
     * Copy constructor.
     */
    Place (const Place&);
    
    /**
     * Destructor.
     */
    virtual ~Place();


  private:
    // ////////////////////// Parent objects ////////////////////
    /**
     * Parent: World object.
     */
    World* _world;
    
    /**
     * Parent PlaceHolder (not always defined,for instance if the
     * current Place object is an extra or alternate one).
     */
    PlaceHolder* _placeHolder;
    
    /**
     * Parent (main) Place (not always defined, for instance if the
     * current Place object is itself a main one).
     */
    Place* _mainPlace;

    
  private:
    // ///////////////// Attributes ///////////////////////
    /**
     * Location structure, containing a key (IATA and ICAO codes, Geonames ID)
     * and the full details of the POR (point of reference): name, geographical
     * coordinates, etc.
     */
    Location _location;

    
  private:
    // ///////////// Full-text matching process support attributes //////////
    /**
     * Xapian document ID.
     *
     * That ID is set only when inserting the Document structure into
     * the Xapian index (database).
     */
    XapianDocID_T _docID;

    /**
     * List of extra matching (similar) places.
     */
    PlaceOrderedList_T _extraPlaceList;

    /**
     * List of alternate matching (less similar) places.
     */
    PlaceOrderedList_T _alternatePlaceList;

    /**
     * Set of unique terms (strings), which serve as indexing the
     * Xapian document corresponding to the current Place object.
     */
    StringSet_T _termSet;

    /**
     * Set of unique terms (strings), which serve as basis for right
     * spelling. They are added to the Xapian database.
     */
    StringSet_T _spellingSet;

    /**
     * Set of unique terms (strings), which serve as basis for
     * stemming. They are added to the Xapian database.
     */
    StringSet_T _stemmingSet;

    /**
     * Set of unique terms (strings), which serve as basis for
     * synonyms. They are added to the Xapian database.
     */
    StringSet_T _synonymSet;
  };

}
#endif // __OPENTREP_BOM_PLACE_HPP
