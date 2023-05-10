#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "graph.hpp"


// разбивает входную строку на вектор: {город отправления, город прибытия, транспорт, время, стоимость}
void tokenize( const std::string &str, std::vector<std::string> &out )
{
    out.clear( );
    const char *delimeters1 = "\"";
    const char *delimeters2 = " \"";
    int token_num = 1;
    char *token = strtok( const_cast<char*>( str.c_str( ) ), delimeters1 );

    while ( token != nullptr )
    { 
        if (std::string( token ) != " ") 
        {
            out.push_back( std::string( token ) );
            token_num++;
        }

        if ( token_num <= 2 )
            token = strtok( nullptr, delimeters1 );
        else
            token = strtok( nullptr, delimeters2 );
    }
}

// считывает данные, записывает их в вектор ребер графа
void read_data( std::string file_path,
                std::vector<Edge> &arcs,
                IDStore &city_id_store,
                IDStore &transpor_id_store ) {

    std::string line; // хранит считываемую строку
    std::vector<std::string> tokens; // хранит список слов разделенных пробелами в считываемой строке
    std::ifstream in( file_path );

    if ( in.is_open( ) ) 
    {
        while ( std::getline( in, line, '\n' ) )
        {   
            // пропуск комментариев
            if ( *line.begin( ) == '#' ) 
                continue;

            // пропуск пустых строк
            if ( line.empty( ) )
                continue;

            tokenize( line, tokens );
            arcs.push_back( Edge( tokens, city_id_store, transpor_id_store ) );
        }
    }

    in.close( );
}
