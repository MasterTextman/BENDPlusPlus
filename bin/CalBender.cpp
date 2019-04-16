#include "Bender.h"
#include "CalMutations.cpp"
#include <fstream>
#include <sstream>
#include <direct.h>

// For cross-platforming.
#if _MSC_VER
    #define mkdir(x) _mkdir(x) 
#endif

class CalBender : public Bender{
    public:
        void loadFile(std::string filename) override{
            std::ifstream ifile = std::ifstream(filename, ios::in | ios::binary | ios::ate);

            string ss; // ? May be replaced with stringstream

            if(ifile.is_open()){
                fname = filename;
                size_t size = ifile.tellg();
                ifile.seekg(0, ios::beg);

                bufferSize = 1024;

                if(size >= 1024*1024*1024){
                    // If the file is over a gigabyte, increase buffersize to a megabyte.
                    bufferSize *= 1024;
                }

                unique_ptr<char[]> buffer (new char[bufferSize]);

                LoadingBar lbar = LoadingBar((size/bufferSize));
                lbar.setLabel("Load("+fname+")");

                while(ifile){
                    // Reads a chunk of bytes from the file.
                    ifile.read(buffer.get(), bufferSize);
                    string x = string(buffer.get(), bufferSize);
                    ss.append(x); // ? May be replaced with stringstream
                    lbar.nextStep();
                }

                // Sets dumb safety minimum to a 50th of a file.
                // This assumes a heuristic that the file is large enough that -
                //  - the first 50th of the bytes contain the entire header.
                // This heuristic may overcompensate a lot, but it avoids the header consistently.
                safetyMin = size / 50;
                contents = ss;

                
                if(contents == ""){
                    cout << "File is empty. Aborting bender..." << endl;
                    exit(EXIT_FAILURE);
                }

                _mkdir("output");
                extension = fname.substr(fname.rfind('.'));

                // Used to avoid problems with paths including folders.
                // Without this, if the filename is <folder>/<name>,
                // the output name is output/<folder>/<name>.
                size_t indexFolder = fname.rfind('/');
                indexFolder = (indexFolder == string::npos) ? 0 : indexFolder+1;
                fname = fname.substr(indexFolder, fname.rfind('.'));
                
                backup.assign(contents); // backs up file for later restoration
            }
            else{
                cout << "No file loaded - are you sure that file exists?" << endl;
                exit(EXIT_FAILURE);
            }
        }

        void addMutation(std::string mutname, Mutation *m) override {
            CalMutation *cm = static_cast<CalMutation*>(m);
            cm->safetymin = 10;
            mutations.emplace(mutname, cm);
        }

        void mutateUsing(std::string mutname, std::map<std::string, std::any> options) override {
            this->mutations[mutname]->mutate(contents, options);
        }

        void saveContents() override{
            stringstream ss;
            ss << "output/" << fname << extension;
            string savefile = ss.str();

            // Opens a file with [filename]
            ofstream ofile (savefile, ios::out | ios::binary);

            size_t filesize = contents.size();
            LoadingBar lbar = LoadingBar(filesize/bufferSize);

            // Buffers contents into file according to amount of buffers used to load file.
            for(int i = 0; (i*bufferSize) < filesize; i++){
                // Saves a chunk of bytes into the file.
                string bufferstr = contents.substr(i*bufferSize, bufferSize);
                ofile << bufferstr;
                lbar.nextStep();
            }

            cout << "\tSaved [" << savefile << "]" << endl;
        }

    private:
        std::string contents;
        std::string backup;

        size_t bufferSize;
        size_t safetyMin;

        std::string fname;
        std::string extension;
};