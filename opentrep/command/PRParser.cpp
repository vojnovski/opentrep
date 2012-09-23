// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// OpenTREP
#include <opentrep/basic/BasFileMgr.hpp>
#include <opentrep/service/Logger.hpp>
#include <opentrep/command/PRParserHelper.hpp>
#include <opentrep/command/PRParser.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void PRParser::PRGeneration (const PRFilePath_T& iPRFilename) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      BasFileMgr::doesExistAndIsReadable (iPRFilename);
    if (doesExistAndIsReadable == false) {
      OPENTREP_LOG_ERROR ("The PR input file, '" << iPRFilename
                          << "', can not be retrieved on the file-system");
      throw FileNotFoundException ("The PR input file '" + iPRFilename
                                   + "' does not exist or can not be read");
    }

    // Initialise the PR file parser.
    PRFileParser lPrFileParser (iPRFilename);

    // Parse the CSV-formatted PR input file and generate the corresponding
    // PR structures.
    lPrFileParser.generateLocations();
  }

}