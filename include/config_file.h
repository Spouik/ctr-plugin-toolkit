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
            configFile.WriteLine("# SETTINGS\ndebug = " + std::to_string(config.debug) +
                "\ndelock = " + std::to_string(config.delock) +
                "\nold_odds = " + std::to_string(config.old_odds) +
                "\nnotify = " + std::to_string(config.notify));
            configFile.Close();
            return true;
        }

        // return line at index line
        // Read from an unopened path
        static std::string readLine(std::string path, int line) {
            std::string myLine;
            File configFile = File(path, File::READ);
            std::string content(configFile.GetSize(), '\0');
            configFile.Read(&content[0], content.size());
            configFile.Close();

            int lineCount = 0;
            if (countLine(path) < line)
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
        static std::string readLine(File& myFile, int line) {
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

        // count line from path
        // static int countLine(std::string path) {
        //     std::string myLine;
        //     File configFile = File(path, File::READ);
        //     std::string content(configFile.GetSize(), '\0');
        //     configFile.Read(&content[0], content.size());
        //     configFile.Close();

        //     int lineCount = 0;
        //     for (size_t i = 0; i < content.size(); i++)
        //     {
        //         if (content[i] == '\n')
        //         {
        //             lineCount++;
        //         }
        //     }
        //     return lineCount;
        // }

        // count line from file
        static int countLine(File& myFile) {
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
        static int countLine(std::string myContent) {
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
                if (myString[i] != ' ' || myString[i] != '\n')
                {
                    newString = newString + myString[i];
                }
            }
            return newString;
        }

        static bool readParamBool(std::string param, std::string myContent) {
            std::string myParam;
            for (int i = 0; i < countLine(myContent); i++)
            {
                std::string myLine = readLine(myContent, i);
                for (int j = 0; j < myLine.size(); j++)
                {
                    myParam = "";
                    if (myLine[j] != param[j])
                    {
                        break;
                    }
                    if (myLine[j] == '=')
                    {
                        j++;
                    }
                    if (myParam != param && myParam.size() < param.size())
                    {
                        myParam += param[j];
                    }
                    if (j > param.size())
                    {
                        if (myLine[j] == std::tolower('t'))
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }

                    }

                }
            }
            return false;
        }


        static Location readParamLocation(std::string param, std::string myContent) {
            std::string myParam;
            for (int i = 0; i < countLine(myContent); i++)
            {
                std::string myLine = readLine(myContent, i);
                for (int j = 0; j < myLine.size(); j++)
                {
                    myParam = "";
                    if (myLine[j] != param[j])
                    {
                        break;
                    }
                    if (myLine[j] == '=')
                    {
                        j++;
                    }
                    if (myParam != param && myParam.size() < param.size())
                    {
                        myParam += param[j];
                    }
                    if (j > param.size())
                    {
                        if (myLine[j] == std::tolower('t'))
                        {
                            return TOP;
                        }
                        else if (myLine[j] == std::tolower('n'))
                        {
                            return NONE;
                        }
                        else if (myLine[j + 3] == std::tolower('h')) {
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
    };
}

#endif