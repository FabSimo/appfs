/**
 * Exercise 2 : Read in a xml-file, validate via sheme and generate a csv with the values.
 *
 * @author FirstSanny
 */

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include <iostream>
#include <string>
#include <stdlib.h>

#include "XMLParser.h"

namespace {

	const char* FILEPATHMEASUREDXSD = "/home/alesan/git/appfs/Sanny/ex2/src/recources/EX2/measured-1-1-0.xsd";
	const char* FILEPATHFRAMEWORKXSD = "/home/alesan/git/appfs/Sanny/ex2/src/recources/EX2/Framework-1-1-0.xsd";
	const char* FILEPATHPHYSICSXSD = "/home/alesan/git/appfs/Sanny/ex2/src/recources/EX2/PhysicalValues-1-1-0.xsd";
	const char* FILEPATHTOPOXSD = "/home/alesan/git/appfs/Sanny/ex2/src/recources/EX2/Topology-1-1-0.xsd";
	//const char* URLMEASUREDXSD = "http://gaslab.zib.de/kwpt/measured ../schemes/measured-1-1-0.xsd";

}

using namespace std;

void inline printException(const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cerr << "Exception:" << endl << message << endl;
	XMLString::release(&message);
}

char* getRealPath(const char* path) {
	char actualpath[PATH_MAX + 1];
	char* ptr;
	ptr = realpath(path, actualpath);
	return ptr;
}

bool initGrammarIsSuccessfull(XMLParser* parser, const char* path) {
	if (parser->loadGrammar(path,
			Grammar::GrammarType::SchemaGrammarType, true) == nullptr) {
		cerr << "ERROR : Couldn't initialize XSD-Shema for file "
				<< path << endl;
		return 0;
	}
	return 1;
}

bool initGrammarAndCheckIfSuccessfull(XMLParser* parser) {
	if(!initGrammarIsSuccessfull(parser, FILEPATHMEASUREDXSD)){
		return 0;
	}
	if(!initGrammarIsSuccessfull(parser, FILEPATHPHYSICSXSD)){
		return 0;
	}
	if(!initGrammarIsSuccessfull(parser, FILEPATHFRAMEWORKXSD)){
		return 0;
	}
	if(!initGrammarIsSuccessfull(parser, FILEPATHTOPOXSD)){
		return 0;
	}
	return 1;
}

int main(int argn, char *argv[]) {

	if (argn <= 1) {
		cerr << "ERROR : There was no filename" << endl;
		return 1;
	}

	try {
	    XMLPlatformUtils::Initialize();
	}catch (const XMLException& toCatch) {
		cerr << "ERROR : Couldn't initialize XML-Parser."<<endl;
		printException(toCatch);
		return 1;
	}

	XMLParser* parser = new XMLParser();

	bool success = initGrammarAndCheckIfSuccessfull(parser);

	if(!success){
		return 1;
	}

	try {
		parser->parse(argv[1]);

		if(parser->getErrorCount() != 0){
		  cerr << "ERROR : File doesn't match sheme." << endl;
		  cerr << "There were " << parser->getErrorCount() << " errors." << endl;
		  cerr << "Exceptions: "<< endl;
		  cerr << parser->getErrors();
		  return 1;
		}


		DOMDocument* document = parser->getDocument();

	}

	catch (const XMLException& toCatch) {
		cerr << "ERROR" <<endl;
		printException(toCatch);
		return 1;
	}

	catch (const SAXParseException& toCatch) {
		cerr << "ERROR : Exception while Parsing."<<endl;
		char* message = XMLString::transcode(toCatch.getMessage());
		cerr << "Exception:" << endl << message << endl;
		XMLString::release(&message);
	    return 1;
	}

	delete parser;

	XMLPlatformUtils::Terminate();

	return 0;
}
