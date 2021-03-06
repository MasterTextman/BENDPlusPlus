#include "Bender.h"
#include "CalMutations.h"
#include "Utility.h"
#include <fstream>
#include <direct.h>

#ifndef CALBENDER_H
#define CALBENDER_H

// For cross-platforming.
#if _MSC_VER
    #define mkdir(x) _mkdir(x) 
#endif

class CalBender : public Bender {
    public:
        void loadFile(std::string filename) override;
        void addMutation(std::string mutname, Mutation *m) override;
        void loadDefaultMutations() override;
        void mutateUsing(std::string mutname, std::map<std::string, std::any> options) override;
        void mutateUsing(std::string mutname, std::map<std::string, std::string> options);
        void saveContents() override;
        void resetFile() override;

        // For global configuration w/ all mutations.
        // Said options will apply to all mutations run through this bender.
        void setGlobalMutationOptions(std::map<std::string, std::string> options);

        // Retrieval of all global options.
        std::map<std::string, std::string> getGlobalMutationOptions();

        // Not to be used if `setGlobalMutationOptions` was not run.
        void mutateUsing(std::string mutname);

    private:
        std::string contents;
        std::string backup;

        std::map<std::string, std::string> mutoptions;

        std::string muthistory; // for use in filename;

        std::default_random_engine optionGenerator;
        std::uniform_int_distribution<int> intRand;
        std::uniform_real_distribution<double> realRand;

        size_t bufferSize;
        size_t safetyMin;

        std::string fname;
        std::string extension;
};

#endif