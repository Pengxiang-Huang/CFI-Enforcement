#ifndef _UTILS_H_
#define _UTILS_H_

#include <llvm/IR/Module.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/IR/DebugInfo.h>
#include <set>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
using namespace llvm;
using namespace std;

#define LOG(lv, stmt)							\
	do {											\
		if (VerboseLevel >= lv)						\
		errs() << stmt;							\
	} while(0)


#define OP llvm::errs()

#ifdef DEBUG
    #define DBG OP
#else
    #define DBG if (false) OP
#endif

#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

#define WARN(stmt) LOG(1, "\n[WARN] " << stmt);

#define ERR(stmt)													\
	do {																\
		errs() << "ERROR (" << __FUNCTION__ << "@" << __LINE__ << ")";	\
		errs() << ": " << stmt;											\
		exit(-1);														\
	} while(0)

/// Different colors for output
#define KNRM  "\x1B[0m"   /* Normal */
#define KRED  "\x1B[31m"  /* Red */
#define KGRN  "\x1B[32m"  /* Green */
#define KYEL  "\x1B[33m"  /* Yellow */
#define KBLU  "\x1B[34m"  /* Blue */
#define KMAG  "\x1B[35m"  /* Magenta */
#define KCYN  "\x1B[36m"  /* Cyan */
#define KWHT  "\x1B[37m"  /* White */


extern cl::opt<unsigned> VerboseLevel;

// struct CallsiteLocation {
//     std::string filename;
//     unsigned line;
// };


//debugging
class CallsiteLocation{
    public:
        std::string filename;
        unsigned line;
        size_t hash;
    CallsiteLocation(std::string filename, unsigned line): filename(filename), line(line) {
        hash=std::hash<std::string>{}(filename) ^ std::hash<unsigned>{}(line);
    }   
    // Overload == operator
    // must be used if you want to use this class as key in std::map
    bool operator==(const CallsiteLocation& other) const {
        
        return hash == other.hash;  // Customize as per your class's logic
    }

    // Overload < operator (if needed for ordering or sorting)
    bool operator<(const CallsiteLocation& other) const {
        return hash < other.hash;  // Customize as per your class's logic
    }
    bool operator>(const CallsiteLocation& other) const {
        
        return hash>other.hash;  // Customize as per your class's logic
    }

};

std::map<CallsiteLocation, vector<std::string>> readMismatchFile(std::string filename);

std::map<CallsiteLocation, vector<std::string>> 
getMismatchFunctionTypes(std::vector<std::shared_ptr<Module>> modules,
                         std::map<CallsiteLocation, vector<std::string>> &mismatchMap);
#endif
