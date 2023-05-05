#include "graph.h"
#include "search.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

//using namespace std;

// ��������� ������ �� ������ �����, ����������� ��������� � ��������
void tokenize( const std::string &str, std::vector<std::string> &out )
{
    out.clear( );
    const char *delimeters1 = "\"";
    const char *delimeters2 = " \"";
    int token_num = 1;
    char *token = strtok( const_cast<char*>( str.c_str( ) ), delimeters1 );

    while ( token != nullptr )
    { 
        if (std::string( token ) != " ") {
            out.push_back( std::string( token ) );
            token_num++;
        }

        if ( token_num <= 2 )
            token = strtok( nullptr, delimeters1 );
        else
            token = strtok( nullptr, delimeters2 );
    }
}

void read_data( std::string file_path, 
                std::vector<Edge> &arcs, 
                IDStore &city_id_store, 
                IDStore &transpor_id_store ) {

    std::string line; // ������ ����������� ������
    std::vector<std::string> tokens; // ������ ������ ���� ����������� ��������� � ����������� ������
    std::ifstream in( file_path );

    if ( in.is_open( ) ) {
        while ( std::getline( in, line, '\n' ) )
        {   
            // ������� ������������
            if ( *line.begin( ) == '#' ) 
                continue;

            // ������� ������ �����
            if ( line.empty( ) )
                continue;

            tokenize( line, tokens );
            arcs.push_back( Edge( tokens, city_id_store, transpor_id_store ) );
        }
    }

    in.close( );
}

int main( ) {
    while ( true ) {
        SetConsoleCP(1251);// ��������� ������� �������� win-cp 1251 � ����� �����
        SetConsoleOutputCP(1251); // ��������� ������� �������� win-cp 1251 � ����� ������

        //setlocale( LC_ALL, "Russian" );

        std::string file_path = "big_data.txt";
        std::vector<Edge> arcs;
        IDStore city_id_store;
        IDStore transpor_id_store;

        // ��������� ������
        read_data( file_path, arcs, city_id_store, transpor_id_store );
        Graph test_graph { arcs, city_id_store.size( ) };
        Path shortest_path { city_id_store.size( ) };

        // ������� ������������
        std::cout << "��������, ����� ������� ������ ����� (������� �����):" << std::endl;
        std::cout << "1 - ������������� �� ������� � ���������" << std::endl;
        std::cout << "2 - ����������� �� ����� ���������� �������" << std::endl;

        int cruise_type;
        std::cin >> cruise_type;

        std::cout << "������� ����� ����������:" << std::endl;

        std::string start_city_name;
        std::cin >> start_city_name;
        std::cout << std::endl;
        
        city_id start_city = city_id_store.get_id( start_city_name );

        //std::cout << "������� ����� ������ ���������, ������� ���������� �����������, �� ���������� ������: �������, �����, �������, �����, ������" << std::endl;

        std::set<transport_id> allowed_transport { 0, 1, 2, 3, 4 };
        
        if ( cruise_type == 1 )
            dijkstra(test_graph, shortest_path, start_city, allowed_transport);
        else if ( cruise_type == 2 )
            dfs(test_graph, shortest_path, start_city, allowed_transport);

        std::cout << "������� ����� ����������:" << std::endl;

        std::string end_city_name;
        std::cin >> end_city_name;
        std::cout << std::endl;

        city_id end_city = city_id_store.get_id( end_city_name );

        // ������ ������� �� start_city �� end_city
        std::vector<Edge> path;

        shortest_path.get_path( start_city, end_city, path );

        // ����� ����������� ��������
        std::cout << "����������� �������:" << std::endl << std::endl;
        for ( std::vector<Edge>::reverse_iterator it = path.rbegin( ); it != path.rend( ); ++it )
            std::cout << "����� �����������: " 
                    << city_id_store.get_name( (*it).from_city )
                    << std::endl
                    << "����� ��������: "
                    << city_id_store.get_name( (*it).to_city )
                    << std::endl
                    << "��� ����������: "
                    << transpor_id_store.get_name( (*it).transport_type )
                    << std::endl
                    << "������������ �������: "
                    << (*it).time
                    << std::endl
                    << "��������� �������: "
                    << (*it).fare
                    << std::endl
                    << std::endl;

        bool exit;
        std::cout << "������� 0 ���� ������ ���������� ��� ����� ������ ������� ���� ������ �����:" << std::endl;
        std::cin >> exit;

        if ( exit )
            break;
    }
    return 0;
}