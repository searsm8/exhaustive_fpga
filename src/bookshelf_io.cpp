#include "bookshelf_io.h"

using namespace std;
// a set of useful functions for reading and writing bookshelf format

namespace bookshelf {

string parseExtension(string filename)
{ return filename.substr(filename.find('.')); }

FileParser::FileParser(string filepath)
{
    log::info("Begin parsing file: " + filepath);
    fs = ifstream(filepath);
    if(!fs.good()) log::error("CANNOT OPEN FILE!");
}

FileParser::~FileParser()
{
    log::info("Finished parsing file!");
    fs.close();
}

bool FileParser::parseNextLine(vector<string> &tokens)
{
    tokens.clear();
    string next_line, token;
    while(fs && tokens.empty())
    {
        getline(fs, next_line);
        stringstream ss(next_line);
        while(getline(ss, token, ' '))
        {
            if(token.size() > 0)
                tokens.push_back(token);
        }
    }
    return !tokens.empty();
}

DesignFiles::DesignFiles(string aux_filepath_)
{
    aux_filepath = aux_filepath_;

    design_filepath = aux_filepath.substr(0, aux_filepath.find_last_of('/')+1);
    log::info("Reading design from: " + design_filepath);

    read_aux_file();

    for(string filename : design_filenames)
    {
        string extension = parseExtension(filename);
        if(extension == ".lib")
            lib_filename = filename;
        else if(extension == ".nets")
            nets_filename = filename;
        else if(extension == ".nodes")
            nodes_filename = filename;
        else if(extension == ".scl")
            scl_filename = filename;
        else if(extension == ".pl")
            pl_filename = filename;
        else log::warning("Invalid file extension: " + filename);
    }
}

void DesignFiles::read_aux_file()
{
    FileParser parser(aux_filepath);
    vector<string> tokens;
    while(parser.parseNextLine(tokens))
    {
        if(tokens[0] == "#") // skip comments
            continue;
        for(string word : tokens)
            if(word.find('.') != string::npos)
                design_filenames.push_back(word);
    }
} 

CellLibrary DesignFiles::read_lib_file()
{
    FileParser parser(design_filepath + lib_filename);
    vector<string> tokens;
    CellLibrary cell_lib;
    CellType new_celltype;
    while(parser.parseNextLine(tokens))
    {
        if(tokens[0] == "CELL")
            new_celltype = CellType(tokens[1]);
        if(tokens[0] == "PIN")
            new_celltype.addPin(Pin(tokens[1], tokens[2]));
        if(tokens[0] == "END" && tokens[1] == "CELL") 
           cell_lib.insert({new_celltype.getName(), new_celltype});
    }

    log::info("Cell Library:");
    for(auto it = cell_lib.begin(); it != cell_lib.end(); it++)
        log::info(it->first + ": Pin count: " + to_string(it->second.getPinList().size()));

    return cell_lib;
}

void DesignFiles::read_nodes_file()
{
}

void DesignFiles::read_scl_file()
{
}

void DesignFiles::read_pl_file()
{
}

void DesignFiles::read_nets_file()
{
}

} //end bookshelf_io