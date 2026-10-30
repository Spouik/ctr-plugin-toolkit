#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H
#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    enum Location { NONE, TOP, BOTTOM, BOTH };

    struct Config {
        bool debug = true;
        Location notify = TOP;
        bool delock = false;
        bool old_odds = false;
        std::string test;
        std::string test2;
        int result;
    };

    class ConfigFile {
    public:
        // Check if config.ini already exist, and if not, create it and fill it
        static bool exists(std::string path) {
            if (!(File::Exists(path)))
            {
                File* configFile = new File();
                File::Create(path);
                File::Open(*configFile, path);
                configFile->WriteLine("# SETTINGS\ndebug = true\ndelock = false\nold_odds = false\nnotify = TOP");
                configFile->Close();
                return false;
            }
            return true;
        }

        // Load content of config.ini into gConfig
        static void load(Config& config) {
            File configFile;
            config.result = File::Open(configFile, "config.ini");
            std::string content(configFile.GetSize(), '\0');
            configFile.Read(&content[0], content.size());
            config.test = content;
            configFile.Close();
            // UNFINISHED
            config.debug = readParamBool("debug", content);
            config.delock = readParamBool("delock", content);
            config.old_odds = readParamBool("old_odds", content);
            config.notify = readParamLocation("notify", content);

        };

        // Write content of gConfig in config.ini
        static bool write(Config& config, std::string path) {
            // check existence
            if (!(File::Exists(path)))
            {
                return false;
            }
            File configFile = File(path, File::RW);
            // clear
            configFile.Clear();
            // write
            configFile.WriteLine("# SETTINGS\ndebug = " + boolToString(config.debug) +
                "\ndelock = " + boolToString(config.delock) +
                "\nold_odds = " + boolToString(config.old_odds) +
                "\nnotify = " + locationToString(config.notify));
            configFile.Close();
            return true;
        }

        static std::string getContentFromPath(std::string path) {
            File configFile = File(path, File::READ);
            std::string content(configFile.GetSize(), '\0');
            configFile.Read(&content[0], content.size());
            configFile.Close();
            return content;
        }

        // return line at index line
        // Read from an unopened path
        static std::string readLineFromPath(std::string path, int line) {
            std::string myLine;
            File configFile = File(path, File::READ);
            std::string content(configFile.GetSize(), '\0');
            configFile.Read(&content[0], content.size());
            configFile.Close();

            int lineCount = 0;
            if (countLineFromContent(content) < line)
            {
                return "LINE OUT OF RANGE";
            }
            for (size_t i = 0; i < content.size(); i++)
            {
                if (content[i] == '\n')
                {
                    lineCount++;
                }
                if (lineCount == line && content[i] != '\n')
                {
                    myLine = myLine + content[i];
                }
            }
            return myLine;
        }

        // Read from an already opened File
        static std::string readLineFromFile(File& myFile, int line) {
            std::string myLine;
            std::string content(myFile.GetSize(), '\0');
            myFile.Read(&content[0], content.size());

            int lineCount = 0;
            for (size_t i = 0; i < content.size(); i++)
            {
                if (content[i] == '\n')
                {
                    lineCount++;
                }
                if (lineCount == line && content[i] != '\n')
                {
                    myLine = myLine + content[i];
                }
                if (lineCount < line)
                {
                    return "LINE OUT OF RANGE";
                }
            }
            return myLine;
        }

        static std::string readLineFromContent(std::string myContent, int line) {
            std::string myLine;

            int lineCount = 0;
            // for (size_t i = 0; i < myContent.size(); i++)
            // {
            //     if (myContent[i] == '\n')
            //     {
            //         lineCount++;
            //     }
            //     if (lineCount == line && myContent[i] != '\n')
            //     {
            //         myLine = myLine + myContent[i];
            //     }
            //     if (lineCount < line)
            //     {
            //         return "LINE OUT OF RANGE";
            //     }
            // }
            // return myLine;
            if (countLineFromContent(myContent) < line)
            {
                return "LINE OUT OF RANGE";
            }
            for (size_t i = 0; i < myContent.size(); i++)
            {
                if (myContent[i] == '\n')
                {
                    lineCount++;
                }
                if (lineCount == line && myContent[i] != '\n')
                {
                    myLine = myLine + myContent[i];
                }
            }
            return myLine;
        }

        // count line from path
        static int countLineFromPath(std::string path) {
            File configFile = File(path, File::READ);
            std::string content(configFile.GetSize(), '\0');
            configFile.Read(&content[0], content.size());
            configFile.Close();

            int lineCount = 0;
            for (size_t i = 0; i < content.size(); i++)
            {
                if (content[i] == '\n')
                {
                    lineCount++;
                }
            }
            return lineCount;
        }

        // count line from file
        static int countLineFromFile(File& myFile) {
            std::string content(myFile.GetSize(), '\0');
            myFile.Read(&content[0], content.size());

            int lineCount = 0;
            for (size_t i = 0; i < content.size(); i++)
            {
                if (content[i] == '\n')
                {
                    lineCount++;
                }
            }
            return lineCount;
        }

        // count line from content string
        static int countLineFromContent(std::string myContent) {
            int lineCount = 0;
            for (size_t i = 0; i < myContent.size(); i++)
            {
                if (myContent[i] == '\n')
                {
                    lineCount++;
                }
            }
            return lineCount;
        }

        // clean a string
        static std::string clean(std::string myString) {
            std::string newString;
            for (size_t i = 0; i < myString.size(); i++)
            {
                if (myString[i] != ' ' && myString[i] != '\n')
                {
                    newString = newString + myString[i];
                }
            }
            return newString;
        }

        static bool readParamBool(std::string param, std::string myContent) {
            // if (param == "debug")
            // {
            //     return true;
            // }

            // return false;



            for (int i = 0; i < countLineFromContent(myContent); i++)
            {
                std::string myParam;
                std::string myLine = clean(readLineFromContent(myContent, i));
                for (size_t j = 0; j < param.size(); j++)
                {
                    myParam += myLine[j];
                }
                if (myParam == param)
                {
                    if (myLine[param.size() + 1] == std::tolower('t'))
                    {
                        return true;
                    }
                    return false;
                }

            }
            return false;
        }

        // static std::string testReadParamBool(std::string param, std::string myContent) {
        //     // if (param == "debug")
        //     // {
        //     //     return true;
        //     // }

        //     // return false;



        //     for (int i = 0; i < countLineFromContent(myContent); i++)
        //     {
        //         std::string myParam;
        //         std::string myLine = clean(readLineFromContent(myContent, i));
        //         for (size_t j = 0; j < param.size(); j++)
        //         {
        //             myParam += myLine[j];
        //         }

        //         if (i == 1)
        //         {
        //             return myParam;
        //         }


        //         if (myParam == param)
        //         {

        //             if (myLine[param.size() + 1] == std::tolower('t'))
        //             {
        //                 // return true;
        //             }
        //             // return false;
        //         }

        //     }
        //     // return true;
        // }


        static Location readParamLocation(std::string param, std::string myContent) {
            for (int i = 0; i < countLineFromContent(myContent); i++)
            {
                std::string myParam;
                std::string myLine = clean(readLineFromContent(myContent, i));
                for (size_t j = 0; j < param.size(); j++)
                {
                    myParam += myLine[j];
                }
                if (myParam == param)
                {
                    if (myLine[param.size() + 1] == 't' || myLine[param.size() + 1] == 'T')
                    {
                        return TOP;
                    }
                    else if (myLine[param.size() + 1] == 'n' || myLine[param.size() + 1] == 'N')
                    {
                        return NONE;
                    }
                    else if (myLine[param.size() + 1] == 'b' || myLine[param.size() + 1] == 'B')
                    {
                        if (myLine[param.size() + 3] == 'h' || myLine[param.size() + 3] == 'H')
                        {
                            return BOTH;
                        }
                        else
                        {
                            return BOTTOM;
                        }
                    }
                }

            }
            return NONE;
        }

        // static std::string testReadParamLocation(std::string param, std::string myContent) {
        //     for (int i = 0; i < countLineFromContent(myContent); i++)
        //     {
        //         std::string myParam;
        //         std::string myLine = clean(readLineFromContent(myContent, i));
        //         for (size_t j = 0; j < param.size(); j++)
        //         {
        //             myParam += myLine[j];
        //         }
        //         if (myParam == param)
        //         {
        //             if (myLine[param.size() + 1] == std::tolower('t'))
        //             {
        //                 // return "" + myLine[param.size() + 1] + myLine[param.size() + 2] + myLine[param.size() + 3];
        //                 return myLine;
        //             }
        //             else if (myLine[param.size() + 1] == std::tolower('n'))
        //             {
        //                 // return "" + myLine[param.size() + 1] + myLine[param.size() + 2] + myLine[param.size() + 3];
        //                 return myLine;
        //             }
        //             else if (myLine[param.size() + 3] == std::tolower('h'))
        //             {
        //                 // return "" + myLine[param.size() + 1] + myLine[param.size() + 2] + myLine[param.size() + 3];
        //                 return myLine;
        //             }
        //             else
        //             {
        //                 // return "" + myLine[param.size() + 1] + myLine[param.size() + 2] + myLine[param.size() + 3];
        //                 return myLine;
        //             }
        //         }

        //     }
        //     return "WHAT";
        // }

        static std::string boolToString(bool myBool) {
            if (myBool)
            {
                return "true";
            }
            return "false";
        }

        static std::string locationToString(Location myLocation) {
            if (myLocation == TOP)
            {
                return "TOP";
            }
            else if (myLocation == BOTTOM)
            {
                return "BOTTOM";
            }
            else if (myLocation == BOTH)
            {
                return "BOTH";
            }
            return "NONE";
            
        }
    };
}

#endif